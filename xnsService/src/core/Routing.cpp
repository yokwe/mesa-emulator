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
// Routing.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("routing");

#include "../util/Debug.h"

#include "Routing.h"

static QMap<quint8, const char*>operationNameMap {
	{Routing::OPERATION_REQUEST,  "REQUEST"},
	{Routing::OPERATION_RESPONSE, "RESPONSE"},
};
const char* getOperationName(quint16 value) {
	const char* ret = operationNameMap.value(value, 0);
	if (ret == 0) {
		logger.fatal("%s  value = %d", __FUNCTION__, value);
		ERROR();
	}
	return ret;
}


void RoutingListener::process(DatagramBuffer* request_, DatagramBuffer* response_) {
	logger.info("process");
	RoutingBuffer request(request_);
	RoutingBuffer response(response_);

	request.setPosData();
	quint32 dataLen = request.getDataLength();
	logger.info("dataLen   %3d", dataLen);

	logger.info("operation %s", getOperationName(request.getOperation()));
	if (request.getOperation() == Routing::OPERATION_REQUEST) {
		Routing routing;
		routing.operation = request.get16();
		for(quint32 i = 2; i < dataLen; i += TupleBufer::SIZE) {
			quint32 network = request.get32();
			quint16 delay   = request.get16();
			logger.debug("Tuple     %08X %2d", network, delay);
			Tuple tupple(network, delay);
			routing.contents.append(tupple);
		}
		// sanity check
		if (request.getPos() != (DatagramBuffer::OFFSET_DATA + dataLen)) {
			logger.fatal("pos = %d", request.getPos());
			ERROR();
		}

		{
			request.setPosData();
			char buf[dataLen * 2 + 1];
			char* q = buf;
			for(quint32 i = 0; i < dataLen; i++) {
				int h = request.get8();
				int h1 = (h >> 4) & 0x0f;
				int h2 = h & 0x0f;
				*q++ = (h1 < 10) ? ('0' + h1) : ('A' + h1 - 10);
				*q++ = (h2 < 10) ? ('0' + h2) : ('A' + h2 - 10);
			}
			*q = 0;
			logger.debug("DATA      %s", buf);
		}
	} else {
		ERROR();
	}
}
