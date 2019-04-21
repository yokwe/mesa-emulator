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
static log4cpp::Category& logger = Logger::getLogger("courier-app");

#include "../courier/NIC.h"
#include "../courier/IDP.h"

int main(int /*argc*/, char** /*argv*/) {
	logger.info("START");

	setSignalHandler();

	Courier::NIC nic;
	nic.attach("ens192");

	logger.info("host = %s", Courier::NIC::toStarStyleAddress(nic.getAddress()).toLocal8Bit().constData());
	nic.discardPacket();

	Courier::NIC::Data  data;
	Courier::NIC::Frame ether;
	Courier::IDP::Frame idp;

	for(int i = 0; i < 10; i++) {
		nic.receive(data.block);

		Courier::deserialize(data.block, ether);
//		logger.info(" data %s", Courier::toString(ether).toLocal8Bit().constData());

		Courier::deserialize(ether.data, idp);
		logger.info("%s", Courier::toString(idp).toLocal8Bit().constData());
	}
	nic.detach();

	logger.info("STOP");
	return 0;
}
