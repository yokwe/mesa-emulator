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


#include <errno.h>

void NetworkPacket::attach(const QString& name_) {
	name = name_;
    logger.info("DUMMY NETWORK PACKE");
    logger.info("name     = %s", name.toLatin1().constData());
    logger.info("protocol = 0x%04X", ETH_P_IDP);

    fd = 0;
}

void NetworkPacket::discardRecievedPacket() {
	int count = 0;
	if (DEBUG_SHOW_NETWORK_PACKET) logger.debug("discards %d packet", count);
}
void NetworkPacket::discardOnePacket() {
}

void NetworkPacket::transmit(EthernetIOFaceGuam::EthernetIOCBType* iocb) {
	iocb->status = EthernetIOFaceGuam::S_completedOK;
}
int NetworkPacket::transmit(CARD8* /*data*/, CARD32 dataLen, int& opErrno) {
	opErrno = 0;
	return dataLen;
}

void NetworkPacket::receive(EthernetIOFaceGuam::EthernetIOCBType* /* iocb */) {
	ERROR();
	return;
}

int NetworkPacket::receive(CARD8* /* data */, CARD32 /* dataLen */, int& opErrno) {
	ERROR();
	int ret = 0;
	opErrno = 0;
	return ret;
}

int NetworkPacket::select(CARD32 /*timeout*/, int& opErrno) {
	opErrno = 0;
	return 0;
}
