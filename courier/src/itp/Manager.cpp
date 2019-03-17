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
// Manager.cpp
//

#include "../util/Debug.h"
#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("manager");

#include "../itp/Manager.h"

#include "../itp/IDP.h"
#include "../itp/Echo.h"
#include "../itp/PEX.h"
#include "../itp/RIP.h"
#include "../itp/SPP.h"
#include "../itp/Error.h"

#include "../rpc/Time.h"

void ITP::Manager::addListener(Listener* listener) {
	QString         name       = listener->name;
	IDP::Socket     socket     = listener->socket;
	IDP::PacketType packetType = listener->packetType;

	if (listenerMap.contains(socket)) {
		for(Listener* listener: listenerMap.values(socket)) {
			if (packetType == listener->packetType) {
				logger.fatal("Duplicate listener  %s  %s", toString(socket).toLocal8Bit().constData(), toString(packetType).toLocal8Bit().constData());
				RUNTIME_ERROR();
			}
		}
	}

	logger.info("add %8s %8s %s", name.toLocal8Bit().constData(), toString(socket).toLocal8Bit().constData(), toString(packetType).toLocal8Bit().constData());
	listenerMap.insertMulti(socket, listener);
}

void ITP::Manager::main() {
	for(;;) {
		NIC::Ethernet eth_request;
		NIC::Ethernet eth_response;
		nic.receive(eth_request);
		logger.info("----");
		logger.info(">> %8s %s", "ETHER", toString(eth_request).toLocal8Bit().constData());

		eth_response.dst  = eth_request.src;
		eth_response.src  = nic.getAddress();
		eth_response.type = nic.getType();

		IDP idp_request;
		IDP idp_response;
		idp_request.deserialize(eth_request.netData);
		logger.info(">> %8s %s", "IDP", toString(idp_request).toLocal8Bit().constData());

		{
			IDP idp(idp_request);
			switch(idp.packetType) {
			case ITP::IDP::PacketType::RIP:
			{
				ITP::RIP data;
				data.deserialize(idp.netData);
				logger.info(">> %8s %s", toString(idp.packetType).toLocal8Bit().constData(), toString(data).toLocal8Bit().constData());
			}
				break;
			case ITP::IDP::PacketType::ECHO:
			{
				ITP::Echo data;
				data.deserialize(idp.netData);
				logger.info(">> %8s %s", toString(idp.packetType).toLocal8Bit().constData(), toString(data).toLocal8Bit().constData());
			}
				break;
			case ITP::IDP::PacketType::PEX:
			{
				ITP::PEX data;
				data.deserialize(idp.netData);
				logger.info(">> %8s %s", toString(idp.packetType).toLocal8Bit().constData(), toString(data).toLocal8Bit().constData());
				switch(data.clientType) {
				case ITP::PEX::ClientType::TIME:
				{
					RPC::Time timeData;
					timeData.deserialize(data.netData);
					logger.info(">> %8s %s", "TIME", toString(timeData).toLocal8Bit().constData());
				}
					break;
				default:
					break;
				}
			}
				break;
			case ITP::IDP::PacketType::SPP:
			{
				ITP::SPP data;
				data.deserialize(idp.netData);
				logger.info(">> %8s %s", toString(idp.packetType).toLocal8Bit().constData(), toString(data).toLocal8Bit().constData());
			}
				break;
			case ITP::IDP::PacketType::ERROR:
			{
				ITP::Error data;
				data.deserialize(idp.netData);
				logger.info(">> %8s %s", toString(idp.packetType).toLocal8Bit().constData(), toString(data).toLocal8Bit().constData());
			}
				break;
			default:
				break;
			}
		}

	    idp_response.checksum   = (IDP::Checksum)0;       // dummy
	    idp_response.length     = 0;                      // dummy
	    idp_response.hopCount   = (IDP::HopCount)0;
		idp_response.packetType = idp_request.packetType;

		idp_response.dst_net    = idp_request.src_net;
		idp_response.dst_host   = idp_request.src_host;
		idp_response.dst_socket = idp_request.src_socket;
		idp_response.src_net    = idp_request.dst_net;
		idp_response.src_host   = (IDP::Host)(quint64)nic.getAddress();
		idp_response.src_socket = idp_request.dst_socket;


		bool foundError = false;
		try {
			if (idp_request.checksum != IDP::Checksum::NONE) {
				if (idp_request.checksum != (IDP::Checksum)idp_request.computedChecksum) {
					throw ITP::Error(ITP::Error::ErrorNumber::BAD_CHECKSUM);
				}
			}

			const IDP::Socket     socket     = idp_request.dst_socket;
			const IDP::PacketType packetType = idp_request.packetType;

			if (listenerMap.contains(socket)) {
				// has listener
				bool foundListener = false;
				for(Listener* listener: listenerMap.values(socket)) {
					if (packetType == listener->packetType) {
						foundListener = true;
						listener->process(idp_request, idp_response);
						break;
					}
				}
				if (!foundListener) {
					throw ITP::Error(ITP::Error::ErrorNumber::INVALID_PACKET_TYPE);
				}
			} else {
				// no listener
				throw ITP::Error(ITP::Error::ErrorNumber::NO_SOCKET);
			}
			// Output response
		} catch (const ITP::Error& error) {
			logger.info("        Error %s", toString(error).toLocal8Bit().constData());

			idp_response.packetType = IDP::PacketType::ERROR;
			idp_response.netData.clear();
			error.serialize(idp_response.netData);
			foundError = true;
		}

		bool transmitResponse = true;
		if (foundError) {
			if (idp_request.dst_host == IDP::Host::ALL) {
//				transmitResponse = false;
			}
		}
		if (transmitResponse) {
			idp_response.netData.rewind();
			idp_response.serialize(eth_response.netData);
			eth_response.netData.rewind();

			logger.info("<< %8s %s", "ETHER", toString(eth_response).toLocal8Bit().constData());
			logger.info("<< %8s %s", "IDP", toString(idp_response).toLocal8Bit().constData());

//			nic.transmit(eth_response);
		} else {
			logger.info("<< NO RESPONSE");
		}
	}
}

