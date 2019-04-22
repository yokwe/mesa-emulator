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
// RIP.h
//

#ifndef COURIER_RIP_H__
#define COURIER_RIP_H__

#include "../courier/Block.h"
#include "../courier/IDP.h"
#include "../stub/Routing.h"

namespace Courier {

class RIP {
public:
	using Network   = Courier::IDP::Network;
	using Tuple     = Courier::Stub::Routing::Tuple;
	using Operation = Courier::Stub::Routing::Operation;

	static void setLocalNetwork(Network newValue) {
		localNetwork = newValue;
	}
	static Network getLocalNetwork() {
		return localNetwork;
	}

	static void addNetwork(quint32 network, CARDINAL hopCout);
	static void removeNetwork(quint32 network);
	static QList<Tuple> getNetworkList();

	struct Frame {
		Operation    operation;
		QList<Tuple> tupleList;
	};

//	RIP() : Listener("rip", ITP::IDP::Socket::RIP) {}

//	bool process(Courier::IDP& request, Courier::IDP& response);

private:
	static Network                localNetwork;
	static QMap<Network, quint16> networkMap;
};

// Frame
QString toString(const Courier::RIP::Frame&  frame);
void serialize(Block& block, const Courier::RIP::Frame& frame);
void deserialize(Block& block, Courier::RIP::Frame& frame);

}

#endif

