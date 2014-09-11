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


#include <stdio.h>

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("test");

#include "../util/Debug.h"

#include "../courier/Network.h"
#include "../courier/SocketManager.h"
#include "../courier/stub/StubDatagram.h"
#include "../courier/socket/SocketRouting.h"

int main(int /*argc*/, char** /*argv*/) {
	logger.info("START");

	Network network;

	network.attach("eth1");

	quint64 address = network.getAddress();
	logger.info("%012llX", address);

	SocketManager socketManager(network);

	SocketRouting socketRouting;
	socketManager.add((quint16)Courier::Datagram::WellKnownSocket::ROUTING, &socketRouting);

	logger.info("START Socket");
	socketManager.start();
	for(int i = 0; i < 20; i++) {
		if (!socketManager.isRunning()) break;
		QThread::sleep(1);
	}
	logger.info("STOP Socket");
	socketManager.stop();

	network.detach();

	logger.info("STOP");
	return 0;
}
