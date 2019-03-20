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
// RIP.cpp
//

#include "../util/Debug.h"
#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("rip");

#include "../itp/IDP.h"
#include "../itp/RIP.h"
#include "../itp/Error.h"

#include "../service/RIP.h"


ITP::IDP::Network Service::RIP::myNetwork = (ITP::IDP::Network)0;
void Service::RIP::setMyNetwork(ITP::IDP::Network network) {
	myNetwork = network;
	logger.info("%s %s", __FUNCTION__, toString(myNetwork).toLocal8Bit().constData());
}
ITP::IDP::Network Service::RIP::getMyNetwork() {
	return myNetwork;
}

QMap<ITP::IDP::Network, ITP::IDP::HopCount> Service::RIP::networkMap;
void Service::RIP::addNetwork(ITP::IDP::Network network, ITP::IDP::HopCount hopCount) {
	logger.info("%s %s %s", __FUNCTION__, toString(myNetwork).toLocal8Bit().constData(), toString(hopCount).toLocal8Bit().constData());

	if ((quint16)hopCount == 0) {
		logger.warn("Cannot add because hopCount == 0");
		return;
	}
	networkMap[network] = hopCount;
}
void Service::RIP::removeNetwork(ITP::IDP::Network network) {
	logger.info("%s %s", __FUNCTION__, toString(myNetwork).toLocal8Bit().constData());
	networkMap.remove(network);
}

QList<ITP::RIP::Tupple> Service::RIP::getNetworkList() {
	QList<ITP::RIP::Tupple> ret;

	ret.append(ITP::RIP::Tupple(myNetwork, (ITP::IDP::HopCount)0));

	QMapIterator<ITP::IDP::Network, ITP::IDP::HopCount> i(networkMap);
	while(i.hasNext()) {
		i.next();
		ret.append(ITP::RIP::Tupple(i.key(), i.value()));
	}
	return ret;
}

bool Service::RIP::process(ITP::IDP& idp_request, ITP::IDP& idp_response) {
	if (idp_request.dst_socket != ITP::IDP::Socket::RIP) {
		logger.error("Unexpected socket %s", toString(idp_request).toLocal8Bit().constData());
		RUNTIME_ERROR();
	}

	switch(idp_request.packetType) {
	case ITP::IDP::PacketType::RIP:
	{
		ITP::RIP rip_request;
		rip_request.deserialize(idp_request.netData);

		switch(rip_request.operation) {
		case ITP::RIP::Operation::REQUEST:
		{
			ITP::RIP rip_response;
			rip_response.operation = ITP::RIP::Operation::RESPONSE;
			rip_response.tupples.append(getNetworkList());

			rip_response.serialize(idp_response.netData);
		}
			return true; // transmit response
		case ITP::RIP::Operation::RESPONSE:
			logger.warn("Unexpected %s", toString(rip_request).toLocal8Bit().constData());
			break;
		default:
			logger.error("Unexpected operation    %s", toString(rip_request.operation).toLocal8Bit().constData());
			logger.error("Unexpected idp_request  %s", toString(idp_request).toLocal8Bit().constData());
			logger.error("Unexpected data_request %s", toString(rip_request).toLocal8Bit().constData());
			RUNTIME_ERROR();
			break;
		}
	}
		break;
	case ITP::IDP::PacketType::ERROR:
	{
		ITP::Error data_request;
		data_request.deserialize(idp_request.netData);

		logger.warn("Error %s", toString(data_request).toLocal8Bit().constData());
	}
		break;
	default:
		logger.error("Unexpected packetType %s", toString(idp_request).toLocal8Bit().constData());
		RUNTIME_ERROR();
		break;
	}

	return false; // Don't transmit response
}

