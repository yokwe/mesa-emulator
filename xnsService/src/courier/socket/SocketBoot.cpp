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
// SocketBoot.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("sockBoot");

#include "../util/Debug.h"

#include "SocketBoot.h"

#include "../Courier.h"
#include "../stub/StubBoot.h"

class SimpleNSIO {
public:
	void* map;
	quint32 mapSize;
	//
	quint16 connID;
	quint32 pos;

	SimpleNSIO(QString path) {
		map = Util::mapFile(path, mapSize);
		connID = 0;
		pos = 0;
	}

	~SimpleNSIO() {
		Util::unmapFile(map);
	}
};

//static SimpleNSIO simple("data/GVWin/SCAVGUAM.BOO");

SocketBoot::SocketBoot(QString path) : SocketManager::Socket("Boot") {
	map = Util::mapFile(path, mapSize);
	nextLocalID = 1;
}

void SocketBoot::process(Socket::Context& context, ByteBuffer& request, ByteBuffer& response) {
	using namespace Courier;

	// Sanity check of packetType
	{
		Error::Header reqError;

		Datagram::PacketType reqPacketType = getPacketType(context.reqDatagram);
		switch(reqPacketType) {
		case Datagram::PacketType::BOOT_SERVER_PACKET: {
			Boot::BootFileRequest bootFileRequest;
			deserialize(request, bootFileRequest);

			switch(bootFileRequest.tag) {
			case Boot::EtherBootPacketType::SPP_REQUEST: {
				logger.info("SPP_REQUEST %012X %04X", bootFileRequest.SPP_REQUEST.bootFileNumber, bootFileRequest.SPP_REQUEST.connectionID);
				//RUNTIME_ERROR();

				// See BootChannelSPP.mesa 250
				// Need to return SPP packet
				context.resDatagram.flags = (quint16)Datagram::PacketType::SEQUENCED_PACKET;

				Connection *connection;
				const quint48 remoteHost = context.reqDatagram.source.host;
				if (connectionMap.contains(remoteHost)) {
					// no need to allocate
					connection = connectionMap[remoteHost];
				} else {
					connection = new Connection;
					connectionMap[remoteHost] = connection;
				}

				// set fields of connection
				connection->pos    = 0;
				connection->remote = context.reqDatagram.source;
				//
				connection->header.control     = SequencedPacket::MASK_SYSTEM_PACKET | SequencedPacket::MASK_SEND_ACKNOWLEDGEMENT;
				connection->header.source      = nextLocalID++;
				connection->header.destination = bootFileRequest.SPP_REQUEST.connectionID;
				connection->header.sequence    = 1;
				connection->header.acknowledge = 0;
				connection->header.allocation  = 0;

				connection->header.base = response.getPos();

				Courier::serialize(response, connection->header);

				// end of building response
				response.rewind();
			}
				break;
			default:
				logger.info("UNKNOWN %4X", bootFileRequest.tag);
				RUNTIME_ERROR();
			}
		}
			break;
		case Datagram::PacketType::SEQUENCED_PACKET: {
			SequencedPacket::Header reqHeader;

			deserialize(request, reqHeader);

			logger.info("SPP  %04X  %4X  %4X  %5d  %5d  %5d",
					reqHeader.control, reqHeader.source, reqHeader.destination, reqHeader.sequence, reqHeader.acknowledge, reqHeader.allocation);

			// find connection
			Connection *connection;
			const quint48 remoteHost = context.reqDatagram.source.host;
			if (connectionMap.contains(remoteHost)) {
				connection = connectionMap[remoteHost];
				// sanity check
				if (connection->header.destination != reqHeader.source) {
					logger.error("dest  %04X => %04X", connection->header.destination, reqHeader.source);
					RUNTIME_ERROR();
				}
				if (connection->header.source != reqHeader.destination) {
					logger.error("source  %04X => %04X", connection->header.source, reqHeader.destination);
					RUNTIME_ERROR();
				}
			} else {
				logger.error("remoteHost  %012llX", remoteHost);
				RUNTIME_ERROR();
			}

			RUNTIME_ERROR();
		}
			break;
		case Datagram::PacketType::ERROR: {
			Courier::deserialize(request, reqError);
			logger.warn("packetType = ERROR");
			SocketManager::dumpPacket(context.reqEthernet, context.reqDatagram);
			logger.debug("ERROR     %s (%d) %d", Courier::getName(reqError.number), reqError.number, reqError.parameter);
			response.clear();
			return;
		}
		default:
			logger.fatal("Unknown packetType = %d", (quint16)reqPacketType);
			RUNTIME_ERROR();
		}
	}

}
