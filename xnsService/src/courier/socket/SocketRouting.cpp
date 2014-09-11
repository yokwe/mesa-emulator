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

void SocketRouting::process(ByteBuffer& request, ByteBuffer& /*response*/) {
	Courier::Ethernet::Header ethernet;
	Courier::Datagram::Header datagram;
	Courier::Routing::Header  routing;

	Courier::deserialize(request, ethernet);
	Courier::deserialize(request, datagram);
	Courier::deserialize(request, routing);

	logger.info("operation = %s", Courier::getName(routing.operation));

	QVector<Courier::Routing::Tuple>tuples;

	for(;;) {
		quint32 remaining = request.remaining();
		if (remaining == 0) break;
		Courier::Routing::Tuple tuple;
		Courier::deserialize(request, tuple);
		tuples.append(tuple);

		logger.debug("Tuple [%8X %2d]", tuple.network, tuple.hop);
	}
}
