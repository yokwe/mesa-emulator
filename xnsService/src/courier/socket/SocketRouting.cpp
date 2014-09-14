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
// SocketRouting.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("sockRouting");

#include "../util/Debug.h"

#include "SocketRouting.h"

#include "../Courier.h"
#include "../stub/StubEthernet.h"
#include "../stub/StubDatagram.h"
#include "../stub/StubRouting.h"

void SocketRouting::addNetwork(quint32 networkNumber, quint16 hop) {
	// sanity check
	if (networkMap.contains(networkNumber)) {
		logger.fatal("networkNumber = %04X", networkNumber);
		RUNTIME_ERROR();
	}
	if (networkNumber == Courier::Datagram::NETWORK_ALL) {
		logger.fatal("networkNumber = %04X", networkNumber);
		RUNTIME_ERROR();
	}
	if (networkNumber == Courier::Datagram::NETWORK_UNKNOWN) {
		logger.fatal("networkNumber = %04X", networkNumber);
		RUNTIME_ERROR();
	}
	if (Courier::Routing::MAX_HOP <= hop) {
		logger.fatal("hop = %04X", hop);
		RUNTIME_ERROR();
	}

	networkMap.insert(networkNumber, hop);
}

void SocketRouting::process(const SocketManager::Context& context, ByteBuffer& request, ByteBuffer& response) {
	using namespace Courier;

	Ethernet::Header ethernet;
	Datagram::Header datagram;
	Routing::Header  routing;

	deserialize(request, ethernet);
	deserialize(request, datagram);
	deserialize(request, routing);

	logger.info("operation = %s", getName(routing.operation));

	switch(routing.operation) {
	case Routing::Operation::REQUEST:
	{
		QVector<Routing::Tuple>tuples;
		for(;;) {
			quint32 remaining = request.remaining();
			if (remaining == 0) break;
			Routing::Tuple tuple;
			deserialize(request, tuple);
			tuples.append(tuple);

			logger.debug("Tuple [%8X %2d]", tuple.network, tuple.hop);
		}
		// begin of building response
		//   build response of ethernet
		ethernet.destination = ethernet.source;
		ethernet.source = context.host;
		serialize(response, ethernet);

		//   build response of datagram
		datagram.checksum            = 0;
		datagram.length              = 0;
		datagram.flags               = (quint16)Datagram::PacketType::ROUTING;
		datagram.destination.network = datagram.source.network;
		datagram.destination.host    = datagram.source.host;
		datagram.destination.socket  = datagram.source.socket;
		datagram.source.network      = context.network;
		datagram.source.host         = context.host;
		datagram.source.socket       = Datagram::SOCKET_ROUTING;

		//   build response of routing
		routing.operation = Routing::Operation::RESPONSE;
		serialize(response, routing);

		// build response from tuples
		for(Routing::Tuple tuple: tuples) {
			// special case for NETWORK_ALL
			if (tuple.network == Datagram::NETWORK_ALL) {
				// return all known network
				QMapIterator<quint32, quint16> i(networkMap);
				while (i.hasNext()) {
				    i.next();
				    if (i.value() < tuple.hop) {
				    	Courier::Routing::Tuple responseTuple;
				    	responseTuple.base    = response.getPos();
				    	responseTuple.network = i.key();
				    	responseTuple.hop     = i.value();
				    	serialize(response, responseTuple);
				    }
				}
			} else if (tuple.network == Datagram::NETWORK_UNKNOWN) {
				// return local network
				Courier::Routing::Tuple responseTuple;
				responseTuple.base    = response.getPos();
				responseTuple.network = localNetworkNumber;
				responseTuple.hop     = 0;
				serialize(response, responseTuple);
			} else if (networkMap.contains(tuple.network)) {
				// return specified network
				quint16 hop = networkMap.value(tuple.network, Routing::MAX_HOP);
				if (hop != Routing::MAX_HOP) {
					Courier::Routing::Tuple responseTuple;
					responseTuple.base    = response.getPos();
					responseTuple.network = tuple.network;
					responseTuple.hop     = hop;
					serialize(response, responseTuple);
				}
			}
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
	case Routing::Operation::RESPONSE: {
		// DO NOTHING
		}
		break;
	default:
		RUNTIME_ERROR();
	}
}
