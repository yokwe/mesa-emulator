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
// SocketPEX.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("sockPEX");

#include "../util/Debug.h"

#include "SocketPEX.h"

#include "../Courier.h"
#include "../stub/StubPacketExchange.h"

void SocketPEX::process(Socket::Context& socketContext, ByteBuffer& request, ByteBuffer& response) {
	using namespace Courier;

	// Sanity check of packetType
	{
		Error::Header reqError;
		Datagram::PacketType reqPacketType = getPacketType(socketContext.reqDatagram);
		switch(reqPacketType) {
		case Datagram::PacketType::PACKET_EXCHANGE:
			break;
		case Datagram::PacketType::ERROR:
			Courier::deserialize(request, reqError);
			logger.warn("packetType = ERROR");
			SocketManager::dumpPacket(socketContext.reqEthernet);
			SocketManager::dumpPacket(socketContext.reqDatagram);
			logger.debug("ERROR     %s (%d) %d", Courier::getName(reqError.number), reqError.number, reqError.parameter);
			response.clear();
			return;
		default:
			logger.fatal("Unknown packetType = %d", (quint16)reqPacketType);
			RUNTIME_ERROR();
		}
	}


	PEX::Context context(socketContext);
	Courier::PacketExchange::Header& reqPEX = context.reqPEX;
	Courier::PacketExchange::Header& resPEX = context.resPEX;

	deserialize(request, reqPEX);
	if (reqPEX.clientType != clientType) {
		logger.fatal("reqPEX.clientType = %d  clientType = %d", reqPEX.clientType, clientType);
		RUNTIME_ERROR();
	}

	resPEX.base       = reqPEX.base;
	resPEX.id         = reqPEX.id;
	resPEX.clientType = reqPEX.clientType;
	serialize(response, resPEX);

	pex->process(context, request, response);
}
