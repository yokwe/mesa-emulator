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
// SocketEcho.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("sockEcho");

#include "../util/Debug.h"

#include "SocketEcho.h"

#include "../Courier.h"
#include "../stub/StubEthernet.h"
#include "../stub/StubDatagram.h"
#include "../stub/StubEcho.h"

void SocketEcho::process(const SocketManager::Context& context, ByteBuffer& request, ByteBuffer& response) {
	using namespace Courier;

	Ethernet::Header ethernet;
	Datagram::Header datagram;
	Echo::Header     echo;

	deserialize(request, ethernet);
	deserialize(request, datagram);
	deserialize(request, echo);

	logger.info("operation = %s", getName(echo.operation));

	switch(echo.operation) {
	case Echo::Operation::REQUEST: {
		// begin of building response
		//   build response of ethernet
		ethernet.destination = ethernet.source;
		ethernet.source = context.host;
		serialize(response, ethernet);

		//   build response of datagram
		datagram.checksum            = 0;
		datagram.length              = 0;
		datagram.flags               = (quint16)Datagram::PacketType::ECHO;
		datagram.destination.network = datagram.source.network;
		datagram.destination.host    = datagram.source.host;
		datagram.destination.socket  = datagram.source.socket;
		datagram.source.network      = context.network;
		datagram.source.host         = context.host;
		datagram.source.socket       = Datagram::SOCKET_ECHO;

		//   build response of echo
		echo.operation = Echo::Operation::RESPONSE;
		serialize(response, echo);
		//   build variant part of echo
		quint32 remaining = request.remaining();
		for(quint32 i = 0; i < remaining; i++) {
			quint8 value = request.get8();
			response.put8(value);
		}
		// end of building response
		response.rewind();
		datagram.length   = response.getLimit() - datagram.base;
		// Write datagram header to calculate checksum
		serialize(response, datagram);
		// Calculate checksum
		datagram.checksum = checksum(response.getData(), datagram.base + 2, datagram.length - 2);
		// Write datagram header again
		serialize(response, datagram);
	}
		break;
	case Echo::Operation::RESPONSE: {
			// DO NOTHING
		}
		break;
	default:
		RUNTIME_ERROR();
	}
}
