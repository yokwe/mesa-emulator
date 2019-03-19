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

#include "../service/Manager.h"
#include "../service/Echo.h"
#include "../service/RIP.h"

#include "../itp/IDP.h"
#include "../itp/Echo.h"
#include "../itp/PEX.h"
#include "../itp/RIP.h"
#include "../itp/SPP.h"
#include "../itp/Error.h"

#include "../rpc/Time.h"

void Service::Manager::addListener(Listener* listener) {
	QString          name   = listener->name;
	ITP::IDP::Socket socket = listener->socket;

	if (listenerMap.contains(socket)) {
		Listener* old = listenerMap[socket];
		logger.fatal("Duplicate listener");
		logger.fatal("  new  %s  %s", name.toLocal8Bit().constData(), toString(socket).toLocal8Bit().constData());
		logger.fatal("  old  %s  %s", old->name.toLocal8Bit().constData(), toString(old->socket).toLocal8Bit().constData());
		RUNTIME_ERROR();
	}

	logger.info("add %8s %s", name.toLocal8Bit().constData(), toString(socket).toLocal8Bit().constData());
	listenerMap[socket] = listener;
}

void Service::Manager::main() {
	// add listener
	Echo echo;
	RIP  rip;

	RIP::setMyNetwork((ITP::IDP::Network)0x11223344);
	RIP::addNetwork((ITP::IDP::Network)0x11229999, (ITP::IDP::HopCount)1);

	addListener(&echo);
	addListener(&rip);

	ITP::IDP::Network myNetwork = RIP::getMyNetwork();

	for(;;) {
		NIC::Ethernet eth_request;
		NIC::Ethernet eth_response;
		nic.receive(eth_request);

		eth_response.dst  = eth_request.src;
		eth_response.src  = nic.getAddress();
		eth_response.type = nic.getType();

		ITP::IDP idp_request;
		idp_request.deserialize(eth_request.netData);

		logger.info("----");
		logger.info(">> %8s %s", "ETHER", toString(eth_request).toLocal8Bit().constData());
		dump(">> ", idp_request);

		ITP::IDP idp_response;
	    idp_response.checksum   = (ITP::IDP::Checksum)0;  // dummy
	    idp_response.length     = 0;                      // dummy
	    idp_response.hopCount   = (ITP::IDP::HopCount)0;
		idp_response.packetType = idp_request.packetType;

		idp_response.dst_net    = myNetwork;
		idp_response.dst_host   = idp_request.src_host;
		idp_response.dst_socket = idp_request.src_socket;
		idp_response.src_net    = myNetwork;
		idp_response.src_host   = (ITP::IDP::Host)(quint64)nic.getAddress();
		idp_response.src_socket = idp_request.dst_socket;

		bool transmitResponse = true;
		bool foundError = false;
		try {
			// Sanity check - verify checksum
			if (idp_request.checksum != ITP::IDP::Checksum::NONE) {
				if (idp_request.checksum != (ITP::IDP::Checksum)idp_request.computedChecksum) {
					throw ITP::Error(ITP::Error::ErrorNumber::BAD_CHECKSUM);
				}
			}

			const ITP::IDP::Socket socket = idp_request.dst_socket;
			if (listenerMap.contains(socket)) {
				// has listener
				Listener* listener = listenerMap[socket];
				transmitResponse = listener->process(idp_request, idp_response);
			} else {
				// no listener
				throw ITP::Error(ITP::Error::ErrorNumber::NO_SOCKET);
			}
			// Output response
		} catch (const ITP::Error& error) {
			logger.info("        Error %s", toString(error).toLocal8Bit().constData());

			idp_response.packetType = ITP::IDP::PacketType::ERROR;
			// Clear idp_response.netData
			idp_response.netData.clear();
			error.serialize(idp_response.netData);
			foundError = true;
		}

		if (foundError) {
			if (idp_request.dst_host == ITP::IDP::Host::ALL) {
				transmitResponse = false;
			}
		}
		if (transmitResponse) {
			idp_response.netData.rewind();
			idp_response.serialize(eth_response.netData);
			eth_response.netData.rewind();
			nic.transmit(eth_response);

			logger.info("<< %8s %s", "ETHER", toString(eth_response).toLocal8Bit().constData());
			dump("<< ", idp_response);
		} else {
			logger.info("<< NO RESPONSE");
		}
	}
}

