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

#ifndef SERVICE_RIP_H__
#define SERVICE_RIP_H__

#include "../itp/RIP.h"

#include "../service/Listener.h"

namespace Service {
class RIP : public Listener {
public:
	static void setMyNetwork(ITP::IDP::Network network);
	static ITP::IDP::Network getMyNetwork();

	static void addNetwork(ITP::IDP::Network network, ITP::IDP::HopCount hopCout);
	static void removeNetwork(ITP::IDP::Network network);
	static QList<ITP::RIP::Tupple> getNetworkList();

	RIP() : Listener("rip", ITP::IDP::Socket::RIP) {}

	bool process(ITP::IDP& request, ITP::IDP& response);

private:
	static ITP::IDP::Network                           myNetwork;
	static QMap<ITP::IDP::Network, ITP::IDP::HopCount> networkMap;
};
}

#endif
