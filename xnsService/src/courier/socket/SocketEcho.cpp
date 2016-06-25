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
#include "../stub/StubEcho.h"

void SocketEcho::process(Socket::Context& context, ByteBuffer& request, ByteBuffer& response) {
	using namespace Courier;

	// Sanity check of packetType
	{
		Error::Header reqError;

		Datagram::PacketType reqPacketType = getPacketType(context.reqDatagram);
		switch(reqPacketType) {
		case Datagram::PacketType::ECHO:
			break;
		case Datagram::PacketType::ERROR:
			Courier::deserialize(request, reqError);
			logger.warn("packetType = ERROR");
			SocketManager::dumpPacket(context.reqEthernet);
			SocketManager::dumpPacket(context.reqDatagram);

			logger.debug("ERROR     %s (%d) %d", Courier::getName(reqError.number), reqError.number, reqError.parameter);
			response.clear();
			return;
		default:
			logger.fatal("Unknown packetType = %d", (quint16)reqPacketType);
			RUNTIME_ERROR();
		}
	}

	Echo::Header echo;
	deserialize(request, echo);

	logger.info("operation = %s", getName(echo.operation));

	switch(echo.operation) {
	case Echo::Operation::REQUEST: {
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
	}
		break;
	case Echo::Operation::RESPONSE: {
		logger.warn("RESPONSE %08X-%012X-%04X", context.reqDatagram.source.network, context.reqDatagram.source.host, context.reqDatagram.source.socket);
	}
		break;
	default:
		RUNTIME_ERROR();
	}
}
