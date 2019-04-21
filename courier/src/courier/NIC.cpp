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

#include "../util/Debug.h"
#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("cr/nic");

#include "../courier/NIC.h"


const char* Courier::NIC::getName() const {
	return utilNIC.getName();
}
Courier::NIC::Type Courier::NIC::getType() const {
	return (Courier::NIC::Type)utilNIC.getType();
}
Courier::NIC::Address Courier::NIC::getAddress() const {
	return (Courier::NIC::Address) utilNIC.getAddress();
}

void Courier::NIC::attach(const char* name) {
	utilNIC.attach(name, (quint16)Type::IDP);
}
void Courier::NIC::detach() {
	utilNIC.detach();
}

// discard already received packet
void Courier::NIC::discardPacket() const {
	utilNIC.discardRecievedPacket();
}

void Courier::NIC::receive (Block& block) const {
	int opErrno = 0;
	quint8 frame[maxFrameSize];

	block.zero();
	int ret = utilNIC.receive(block.getData(), block.getCapacity(), opErrno);
	if (ret < 0) {
		logger.fatal("receive ret = %d  opErrno = %d %s", ret, opErrno, strerror(opErrno));
		RUNTIME_ERROR();
	}
	block.clear();
	block.setLimit((quint16)ret);
}
void Courier::NIC::transmit(Block& block) const {
	int opErrno = 0;
	int limit = block.getLimit();

	int ret = utilNIC.transmit(block.getData(), limit, opErrno);
	if (ret == -1) {
		logger.fatal("transmit ret = %d  opErrno = %d %s", ret, opErrno, strerror(opErrno));
		RUNTIME_ERROR();
	}
	if ((quint32)ret != limit) {
		logger.fatal("transmit ret = %d  limit = %d", ret, limit);
		RUNTIME_ERROR();
	}
}
