/*
Copyright (c) 2019, Yasuhiro Hasegawa
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
// NIC.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("nic");

#include "../util/Debug.h"

#include "../idp/NIC.h"

#include <sys/socket.h>
#include <sys/ioctl.h>

#include <linux/if.h>
#include <linux/if_packet.h>
#include <net/if_arp.h>

#include <unistd.h>

#include <errno.h>

static inline quint64 get48_(quint8* p) {
	quint64 ret = p[0];
	ret <<= 8;
	ret |= p[1];
	ret <<= 8;
	ret |= p[2];
	ret <<= 8;
	ret |= p[3];
	ret <<= 8;
	ret |= p[4];
	ret <<= 8;
	ret |= p[5];
	return ret;
}

void NIC::attach(const char* name_, const quint16 protocol_) {
	name     = name_;
	protocol = protocol_;

    logger.info("name     = %s", name);
    logger.info("protocol = 0x%04X", protocol);

	// open socket
	fd = socket(AF_PACKET, SOCK_RAW, qToBigEndian((quint16)protocol));
	if (fd == -1) {
		int myErrno = errno;
		logger.fatal("socket returns -1.  errno = %d", myErrno);
		RUNTIME_ERROR();
	}

	// bind socket with named interface
	{
		// find hardware address of interface
		{
			struct ifreq ifr;

			memset(&ifr, 0, sizeof(ifr));
			strncpy(ifr.ifr_ifrn.ifrn_name, name, IFNAMSIZ);
		    int ret = ioctl(fd, SIOCGIFHWADDR, &ifr);
		    if (ret) {
				int myErrno = errno;
				logger.fatal("%s  %d  ioctl returns not 0.  errno = %d", __FUNCTION__, __LINE__, myErrno);
				RUNTIME_ERROR();
		    }
		    if (ifr.ifr_hwaddr.sa_family != ARPHRD_ETHER) {
				logger.fatal("%s  %d  this is not ethernet.  sa_family = %d", __FUNCTION__, __LINE__, ifr.ifr_hwaddr.sa_family);
				RUNTIME_ERROR();
		    }

		    address = get48_((quint8*)(ifr.ifr_hwaddr.sa_data));
		    logger.info("address  = %012llX", address);
		}

		// find interface index
		int ifindex;
		{
			struct ifreq ifr;

			memset(&ifr, 0, sizeof(ifr));
			strncpy(ifr.ifr_ifrn.ifrn_name, name, IFNAMSIZ);
		    int ret = ioctl(fd, SIOCGIFINDEX, &ifr);
		    if (ret) {
				int myErrno = errno;
				logger.fatal("%s  %d  ioctl returns not 0.  errno = %d", __FUNCTION__, __LINE__, myErrno);
				RUNTIME_ERROR();
		    }
		    ifindex = ifr.ifr_ifindex;
		    logger.info("ifindex  = %d", ifindex);
		}

		// bind socket with interface
		{
			struct sockaddr_ll sll;

			memset(&sll, 0xff, sizeof(sll));
			sll.sll_family   = AF_PACKET;
			sll.sll_protocol = qToBigEndian(protocol);
			sll.sll_ifindex  = ifindex;
			int ret = ::bind(fd, (struct sockaddr *)&sll, sizeof sll);
		    if (ret) {
				int myErrno = errno;
				logger.fatal("%s  %d  bind returns not 0.  errno = %d", __FUNCTION__, __LINE__, myErrno);
				RUNTIME_ERROR();
		    }
		}
	}
}

void NIC::detach() {
	int ret = ::close(fd);
	if (ret) {
		int myErrno = errno;
		logger.fatal("socket returns not 0.  errno = %d", myErrno);
		RUNTIME_ERROR();
	}
	fd = 0;
	name = 0;
}

int NIC::select(quint32 timeout, int& opErrno) {
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(fd, &fds);

	// unit is second
	struct timeval t;
	t.tv_sec  = timeout;
	t.tv_usec = 0;

	int ret = ::select(FD_SETSIZE, &fds, NULL, NULL, &t);
	opErrno = errno;
	return ret;
}

void NIC::discardRecievedPacket() {
	int count = 0;
	for(;;) {
		int opErrno = 0;
		int ret = select(0, opErrno);
		if (ret == -1) {
			logger.fatal("%s  %d  select returns -1.  errno = %d", __FUNCTION__, __LINE__, opErrno);
			RUNTIME_ERROR();
		}
		if (ret == 0) break;
		if (0 < ret) {
			discardOnePacket();
			count++;
		}
	}
	if (DEBUG_NIC_SHOW) logger.debug("discards %d packet", count);
}
void NIC::discardOnePacket() {
	int dataLen = ETH_FRAME_LEN;
	unsigned char data[dataLen];
	int opErrno = 0;
	receive(data, dataLen, opErrno);
}

int NIC::transmit(quint8* data, quint32 dataLen, int& opErrno) {
	if (DEBUG_NIC_TRACE) {
		logger.debug("TRANS    dataLen = %d", dataLen);
		char buf[dataLen * 2 + 1];
		char* q = buf;
		for(quint32 i = 0; i < dataLen; i++) {
			int h = data[i];
			int h1 = (h >> 4) & 0x0f;
			int h2 = h & 0x0f;
			*q++ = (h1 < 10) ? ('0' + h1) : ('A' + h1 - 10);
			*q++ = (h2 < 10) ? ('0' + h2) : ('A' + h2 - 10);
		}
		*q = 0;
		logger.debug("TRANS    %s", buf);
	}

	int ret = send(fd, data, dataLen, 0);
	opErrno = errno;
	if (DEBUG_NIC_SHOW) logger.debug("%-8s data = %p  dataLen = %4d  opErrno = %3d  ret = %4d", __FUNCTION__, data, dataLen, opErrno, ret);
	return ret;
}

int NIC::receive(quint8* data, quint32 dataLen, int& opErrno) {
	int ret = recv(fd, data, dataLen, 0);
	opErrno = errno;

	if (0 < ret) {
		if (DEBUG_NIC_TRACE) {
			logger.debug("RECV     ret = %d", ret);
			char buf[ret * 2 + 1];
			char* q = buf;
			for(int i = 0; i < ret; i++) {
				int h = data[i];
				int h1 = (h >> 4) & 0x0f;
				int h2 = h & 0x0f;
				*q++ = (h1 < 10) ? ('0' + h1) : ('A' + h1 - 10);
				*q++ = (h2 < 10) ? ('0' + h2) : ('A' + h2 - 10);
			}
			*q = 0;
			logger.debug("RECV     %s", buf);
		}
	}

	if (DEBUG_NIC_SHOW) logger.debug("%-8s data = %p  dataLen = %4d  opErrno = %3d  ret = %4d", __FUNCTION__, data, dataLen, opErrno, ret);
	return ret;
}
