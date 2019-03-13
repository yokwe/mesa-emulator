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

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("app");

#include "../util/Debug.h"

#include "../idp/NIC.h"
#include "../idp/Packet.h"
#include "../idp/IDP.h"

int main(int /*argc*/, char** /*argv*/) {
	logger.info("START");

	setSignalHandler();

	NIC nic;
	nic.attach("ens33", NIC::ETH_P_IDP);

	Packet packet;
	int opErrno = 0;

	for(int i = 0; i < 10; i++) {
		logger.info("# %3d", i);

		packet.clear();
		opErrno = 0;
		int length = nic.receive(packet.getData(), packet.getCapacity(), opErrno);
		if (length == -1) {
			logger.info("opErrno * %d", opErrno);
			break;
		}
		if (opErrno != 0) {
			logger.info("opErrno # %d", opErrno);
			break;
		}

		packet.setLimit(length);

		logger.info("packet %4d %3d", length, opErrno);
	}

	nic.detach();

	logger.info("STOP");
	return 0;
}
