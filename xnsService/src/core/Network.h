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
// Network.h
//

#ifndef NETWORK_H__
#define NETWORK_H__

#include <linux/if_ether.h>

#include <QtCore>

class Network {
public:
	// packet type of Xerox IDP
	static const int ETH_P_IDP = 0x0600;

	Network() : name(0), fd(0), address(0), network(0) {}

	const char* getName() {
		return name;
	}
	int getFileDescriptor() {
		return fd;
	}
	quint64 getAddress() {
		return address;
	}
	quint32 getNetwork() {
		return network;
	}
	void setNetwork(quint32 value) {
		network = value;
	}

	void attach(const char* name_);
	void detach();

	// discard already received packet
	void discardRecievedPacket();
	void discardOnePacket();
	int  select(quint32 timeout, int& opErrno);

	// returns return code of send and recv. no error checking
	int transmit(quint8* data, quint32 dataLen, int& opErrno);
	int receive (quint8* data, quint32 dataLen, int& opErrno);

private:
	const char* name;
	int         fd;
	quint64     address;
	quint32     network;
};

#endif
