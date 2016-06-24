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

//
// SocketBoot::BootFile
//
QMap<quint48, SocketBoot::BootFile*> SocketBoot::BootFile::map;

SocketBoot::BootFile::BootFile(quint48 bfn_, QString path_) : bfn(bfn_), path(path_) {
	this->address = Util::mapFile(path_, this->size, true);
}
SocketBoot::BootFile::~BootFile() {
	Util::unmapFile(this->address);
}
void SocketBoot::BootFile::add(quint48 bfn, QString path) {
	if (map.contains(bfn)) {
		BootFile* old = map[bfn];
		logger.error("Duplicate OLD bfn = %012X  path = %s", old->bfn, old->path.toLatin1().constData());
		logger.error("Duplicate NEW bfn = %012X  path = %s", bfn, path.toLatin1().constData());
		RUNTIME_ERROR();
	}
	BootFile* entry = new BootFile(bfn, path);
	map[bfn] = entry;

	logger.info("BootFile::add bfn = %012X  path = %s", entry->bfn, entry->path.toLatin1().constData());
}
SocketBoot::BootFile* SocketBoot::BootFile::getInstance(quint48 bfn) {
	if (!map.contains(bfn)) {
		logger.error("Unknown bfn = %012X", bfn);
		RUNTIME_ERROR();
	}
	return map[bfn];
}


//
// Connection
//
QMap<quint48, SocketBoot::Connection*> SocketBoot::Connection::map;
quint16 SocketBoot::Connection::nextLocalID = 1;

SocketBoot::Connection::Connection(quint48 host_, quint48 bfn, quint16 connectionID) : host(host_) {
	using namespace Courier;

	this->bootFile = BootFile::getInstance(bfn);

	this->pos = 0;

	this->header.control     = SequencedPacket::MASK_SYSTEM_PACKET | SequencedPacket::MASK_SEND_ACKNOWLEDGEMENT;
	this->header.source      = nextLocalID++;
	this->header.destination = connectionID;
	this->header.sequence    = 1;
	this->header.acknowledge = 0;
	this->header.allocation  = 0;
}
void SocketBoot::Connection::add(quint48 host, quint48 bfn, quint16 connectionID) {
	if (map.contains(host)) {
		Connection* old = map[bfn];
		logger.warn("Connection::add delete old instance host = %012X", old->host);
		delete old;
	}

	Connection* entry = new Connection(host, bfn, connectionID);
	map[host] = entry;

	logger.info("Connection::add host = %012X  bfn = %012X  connectionID = %04X", host, bfn, connectionID);
}
SocketBoot::Connection* SocketBoot::Connection::getInstance(quint48 host) {
	if (!map.contains(host)) {
		logger.error("Unknown host = %012X", host);
		RUNTIME_ERROR();
	}
	return map[host];
}


//
// SocketBoot
//
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

				quint48 host         = context.reqDatagram.source.host;
				quint48 bfn          = bootFileRequest.SPP_REQUEST.bootFileNumber;
				quint16 connectionID = bootFileRequest.SPP_REQUEST.connectionID;

				Connection::add(host, bfn, connectionID);

				Connection *connection = Connection::getInstance(host);
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
			quint48     host       = context.reqDatagram.source.host;
			Connection *connection = Connection::getInstance(host);

			// sanity check
			if (connection->header.destination != reqHeader.source) {
				logger.error("dest    %04X => %04X", connection->header.destination, reqHeader.source);
				RUNTIME_ERROR();
			}
			if (connection->header.source != reqHeader.destination) {
				logger.error("source  %04X => %04X", connection->header.source, reqHeader.destination);
				RUNTIME_ERROR();
			}

			// Set header
			if (connection->pos == connection->bootFile->size) {
				// Already reached EOF
				// Send EOF
				connection->header.control = SequencedPacket::MASK_SYSTEM_PACKET | SequencedPacket::MASK_ATTENTION;
			} else {
				// Send data
				connection->header.control = 0;
				connection->header.sequence++;
				connection->header.acknowledge++;
				connection->header.allocation++;
			}
			connection->header.base = response.getPos();
			Courier::serialize(response, connection->header);

			// Send data
			if (connection->pos == connection->bootFile->size) {
				// No data
				logger.info("EOF");
			} else {
				quint32 nextPos = connection->pos + 512;
				if (connection->bootFile->size < nextPos) {
					nextPos = connection->bootFile->size;
				}
				// Send data [pos..nextPos)
				quint8* base = (quint8*)connection->bootFile->address;
				for(quint32 i = connection->pos; i < nextPos; i++) {
					response.put8(base[i]);
				}
				logger.info("SEND %4X => %4X", connection->pos, nextPos);
				connection->pos = nextPos;
			}

			// end of building response
			response.rewind();
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
