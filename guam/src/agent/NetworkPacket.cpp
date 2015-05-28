/*
Copyright (c) 2014, Yasuhiro Hasegawa
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
  3. The name of the author may not be used to endorse or promote products derived
     from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/


//
// NetworkPacket.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("packet");

#include "../util/Debug.h"

#include "../mesa/Memory.h"

#include "NetworkPacket.h"

#include <sys/socket.h>
#include <sys/ioctl.h>

#include <linux/if.h>
#include <linux/if_packet.h>
#include <net/if_arp.h>

#include <errno.h>

void NetworkPacket::attach(const QString& name_) {
	name = name_;
    logger.info("name     = %s", name.toLatin1().constData());
    logger.info("protocol = 0x%04X", ETH_P_IDP);

	// open socket
	fd = socket(AF_PACKET, SOCK_RAW, qToBigEndian((CARD16)ETH_P_IDP));
	if (fd == -1) {
		int myErrno = errno;
		logger.fatal("socket returns -1.  errno = %d", myErrno);
		ERROR();
	}

	// bind socket with named interface
	{
		// find hardware address of interface
		{
			struct ifreq ifr;

			memset(&ifr, 0, sizeof(ifr));
			strncpy(ifr.ifr_ifrn.ifrn_name, name.toLatin1().constData(), IFNAMSIZ);
		    int ret = ioctl(fd, SIOCGIFHWADDR, &ifr);
		    if (ret) {
				int myErrno = errno;
				logger.fatal("%s  %d  ioctl returns not 0.  errno = %d", __FUNCTION__, __LINE__, myErrno);
				ERROR();
		    }
		    if (ifr.ifr_hwaddr.sa_family != ARPHRD_ETHER) {
				logger.fatal("%s  %d  this is not ethernet.  sa_family = %d", __FUNCTION__, __LINE__, ifr.ifr_hwaddr.sa_family);
				ERROR();
		    }
		    for(int i = 0; i < ETH_ALEN; i++) address[i] = ifr.ifr_hwaddr.sa_data[i];
		    logger.info("address  = %02X-%02X-%02X-%02X-%02X-%02X", address[0], address[1], address[2], address[3], address[4], address[5]);
		}

		// find interface index
		int    ifindex;
		{
			struct ifreq ifr;

			memset(&ifr, 0, sizeof(ifr));
			strncpy(ifr.ifr_ifrn.ifrn_name, name.toLatin1().constData(), IFNAMSIZ);
		    int ret = ioctl(fd, SIOCGIFINDEX, &ifr);
		    if (ret) {
				int myErrno = errno;
				logger.fatal("%s  %d  ioctl returns not 0.  errno = %d", __FUNCTION__, __LINE__, myErrno);
				ERROR();
		    }
		    ifindex = ifr.ifr_ifindex;
		    logger.info("ifindex  = %d", ifindex);
		}

		// bind socket with interface
		{
			struct sockaddr_ll sll;

			memset(&sll, 0xff, sizeof(sll));
			sll.sll_family   = AF_PACKET;
			sll.sll_protocol = qToBigEndian(ETH_P_IDP);
			sll.sll_ifindex  = ifindex;
			int ret = ::bind(fd, (struct sockaddr *)&sll, sizeof sll);
		    if (ret) {
				int myErrno = errno;
				logger.fatal("%s  %d  bind returns not 0.  errno = %d", __FUNCTION__, __LINE__, myErrno);
				ERROR();
		    }
		}
	}
}

void NetworkPacket::discardRecievedPacket() {
	int count = 0;
	for(;;) {
		int opErrno = 0;
		int ret = select(0, opErrno);
		if (ret == -1) {
			logger.fatal("%s  %d  select returns -1.  errno = %d", __FUNCTION__, __LINE__, opErrno);
			ERROR();
		}
		if (ret == 0) break;
		if (0 < ret) {
			discardOnePacket();
			count++;
		}
	}
	if (DEBUG_SHOW_NETWORK_PACKET) logger.debug("discards %d packet", count);
}
void NetworkPacket::discardOnePacket() {
	int dataLen = ETH_FRAME_LEN;
	unsigned char data[dataLen];
	int opErrno = 0;
	receive(data, dataLen, opErrno);
}

void NetworkPacket::transmit(EthernetIOFaceGuam::EthernetIOCBType* iocb) {
	if (iocb == 0) ERROR();
	if (iocb->bufferLength == 0) ERROR();
	if (iocb->bufferAddress == 0) ERROR();

	CARD32 dataLen = iocb->bufferLength;
	CARD8* data    = (CARD8*)Fetch(iocb->bufferAddress);
	int    opErrno = 0;

	int ret = transmit(data, dataLen, opErrno);

	if (ret == -1) {
		// set iocb->status if possibble

		//static const CARD16 S_inProgress              =   1;
		//static const CARD16 S_completedOK             =   2;
		//static const CARD16 S_tooManyCollisions       =   4;
		//static const CARD16 S_badCRC                  =   8;
		//static const CARD16 S_alignmentError          =  16;
		//static const CARD16 S_packetTooLong           =  32;
		//static const CARD16 S_bacCRDAndAlignmentError = 128;
		logger.fatal("%s  %d  sendto returns -1.  errno = %d", __FUNCTION__, __LINE__, opErrno);
		ERROR();

		switch (opErrno) {
		default:
			iocb->status = EthernetIOFaceGuam::S_tooManyCollisions;
		}
		return;
	}
	if (ret != (int)dataLen) {
		logger.fatal("%s  %d  ret != dataLen.  ret = %d  dataLen = %d", __FUNCTION__, __LINE__, ret, dataLen);
		ERROR();
	}
	iocb->status = EthernetIOFaceGuam::S_completedOK;
}
int NetworkPacket::transmit(CARD8* data, CARD32 dataLen, int& opErrno) {
	// Buffer for changing of byte order
	CARD8  buffer[dataLen + 1]; // +1 for odd dataLen
	// Change byte order
	//for(CARD32 i = 0; i < dataLen; i++) buffer[i ^ 1] = data[i];
	{
		CARD16* p = (CARD16*)data;
		CARD16* q = (CARD16*)buffer;

		if (dataLen & 1) {
			logger.fatal("dataLen = %d", dataLen);
			ERROR();
		}
		CARD32 len = dataLen / 2;
		for(CARD32 i = 0; i < len; i++) q[i] = qToBigEndian(p[i]);
	}

	if (DEBUG_TRACE_NETWORK_PACKET) {
		logger.debug("TRANS    dataLen = %d", dataLen);
		char buf[dataLen * 2 + 1];
		char* q = buf;
		for(CARD32 i = 0; i < dataLen; i++) {
			int h = data[i ^ 1];
			int h1 = (h >> 4) & 0x0f;
			int h2 = h & 0x0f;
			*q++ = (h1 < 10) ? ('0' + h1) : ('A' + h1 - 10);
			*q++ = (h2 < 10) ? ('0' + h2) : ('A' + h2 - 10);
		}
		*q = 0;
		logger.debug("TRANS    %s", buf);
	}

	int ret = send(fd, buffer, dataLen, 0);
	opErrno = errno;
	if (DEBUG_SHOW_NETWORK_PACKET) logger.debug("%-8s data = %p  dataLen = %4d  opErrno = %3d  ret = %4d", __FUNCTION__, data, dataLen, opErrno, ret);
	return ret;
}

void NetworkPacket::receive(EthernetIOFaceGuam::EthernetIOCBType* iocb) {
	if (iocb == 0) ERROR();
	if (iocb->bufferLength == 0) ERROR();
	if (iocb->bufferAddress == 0) ERROR();

	CARD8* data    = (CARD8*)Store(iocb->bufferAddress);
	CARD32 dataLen = iocb->bufferLength;
	int    opErrno = 0;

	iocb->status = EthernetIOFaceGuam::S_inProgress;
	int ret = receive(data, dataLen, opErrno);

	if (ret == -1) {
		// set iocb->status if possible

		//static const CARD16 S_inProgress              =   1;
		//static const CARD16 S_completedOK             =   2;
		//static const CARD16 S_tooManyCollisions       =   4;
		//static const CARD16 S_badCRC                  =   8;
		//static const CARD16 S_alignmentError          =  16;
		//static const CARD16 S_packetTooLong           =  32;
		//static const CARD16 S_bacCRDAndAlignmentError = 128;
		logger.fatal("%s  %d  recv   returns -1.  errno = %d", __FUNCTION__, __LINE__, opErrno);
		ERROR();

		switch (opErrno) {
		default:
			iocb->status = EthernetIOFaceGuam::S_badCRC;
		}
		return;
	}
	if (ret < 0) {
		logger.fatal("unknown ret = %d", ret);
		ERROR();
	}

	if (dataLen < (CARD32)ret) {
		iocb->status = EthernetIOFaceGuam::S_packetTooLong;
		return;
	}

	iocb->actualLength = ret;
	iocb->status = EthernetIOFaceGuam::S_completedOK;
}

int NetworkPacket::receive(CARD8* data, CARD32 dataLen, int& opErrno) {
	// Buffer for changing of byte order
	CARD8  buffer[ETH_FRAME_LEN];

	int ret = recv(fd, buffer, sizeof(buffer), 0);
	opErrno = errno;

	if (0 < ret) {
		// change byte order
		//for(int i = 0; i < ret; i++) data[i] = buffer[i ^ 1];
		{
			CARD16* p = (CARD16*)buffer;
			CARD16* q = (CARD16*)data;

			if (dataLen & 1) {
				logger.fatal("dataLen = %d", dataLen);
				ERROR();
			}
			CARD32 len = dataLen / 2;
			for(CARD32 i = 0; i < len; i++) q[i] = qFromBigEndian(p[i]);
		}

		if (DEBUG_TRACE_NETWORK_PACKET) {
			logger.debug("RECV     ret = %d", ret);
			char buf[ret * 2 + 1];
			char* q = buf;
			for(int i = 0; i < ret; i++) {
				int h = data[i ^ 1];
				int h1 = (h >> 4) & 0x0f;
				int h2 = h & 0x0f;
				*q++ = (h1 < 10) ? ('0' + h1) : ('A' + h1 - 10);
				*q++ = (h2 < 10) ? ('0' + h2) : ('A' + h2 - 10);
			}
			*q = 0;
			logger.debug("RECV     %s", buf);
		}
	}

	if (DEBUG_SHOW_NETWORK_PACKET) logger.debug("%-8s data = %p  dataLen = %4d  opErrno = %3d  ret = %4d", __FUNCTION__, data, dataLen, opErrno, ret);
	return ret;
}

int NetworkPacket::select(CARD32 timeout, int& opErrno) {
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(fd, &fds);

	// 1 second
	struct timeval t;
	t.tv_sec  = timeout;
	t.tv_usec = 0;

	int ret = ::select(FD_SETSIZE, &fds, NULL, NULL, &t);
	opErrno = errno;
	return ret;
}
