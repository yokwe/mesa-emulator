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


#include <stdio.h>

#include "../util/Debug.h"
#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("app");


#include "../util/NIC.h"
#include "../util/NetData.h"

#include "../itp/IDP.h"
#include "../itp/Echo.h"
#include "../itp/PEX.h"
#include "../itp/RIP.h"
#include "../itp/SPP.h"
#include "../itp/Error.h"

#include "../rpc/Time.h"

int main(int /*argc*/, char** /*argv*/) {
	logger.info("START");

	setSignalHandler();

	NIC nic;
	nic.attach("ens33", (quint16)NIC::Type::IDP);

	for(int i = 0; i < 100; i++) {
		logger.info("# %3d", i);

		NIC::Ethernet ethernet;
		nic.receive(ethernet);

		logger.info("%-8s %s", "ETHER", toString(ethernet).toLocal8Bit().constData());

		ITP::IDP idp;
		idp.deserialize(ethernet.netData);
		logger.info("%-8s %s", "IDP", toString(idp).toLocal8Bit().constData());

		switch(idp.packetType) {
		case ITP::IDP::PacketType::RIP:
		{
			ITP::RIP data;
			data.deserialize(idp.netData);
			logger.info("%-8s %s", toString(idp.packetType).toLocal8Bit().constData(), toString(data).toLocal8Bit().constData());
		}
			break;
		case ITP::IDP::PacketType::ECHO:
		{
			ITP::Echo data;
			data.deserialize(idp.netData);
			logger.info("%-8s %s", toString(idp.packetType).toLocal8Bit().constData(), toString(data).toLocal8Bit().constData());
		}
			break;
		case ITP::IDP::PacketType::PEX:
		{
			ITP::PEX data;
			data.deserialize(idp.netData);
			logger.info("%-8s %s", toString(idp.packetType).toLocal8Bit().constData(), toString(data).toLocal8Bit().constData());
			switch(data.clientType) {
			case ITP::PEX::ClientType::TIME:
			{
				RPC::Time timeData;
				timeData.deserialize(data.netData);
				logger.info("%-8s %s", "TIME", toString(timeData).toLocal8Bit().constData());
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
			logger.info("%-8s %s", toString(idp.packetType).toLocal8Bit().constData(), toString(data).toLocal8Bit().constData());
		}
			break;
		case ITP::IDP::PacketType::ERROR:
		{
			ITP::Error data;
			data.deserialize(idp.netData);
			logger.info("%-8s %s", toString(idp.packetType).toLocal8Bit().constData(), toString(data).toLocal8Bit().constData());
		}
			break;
		default:
			break;
		}
	}

	nic.detach();

	logger.info("STOP");
	return 0;
}
