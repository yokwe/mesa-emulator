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
// NIC.h
//

#ifndef COURIER_NIC_H__
#define COURIER_NIC_H__

#include "../courier/Block.h"
#include "../stub/Ethernet.h"
#include "../util/NIC.h"

using UtillNIC = NIC;

namespace Courier {

class NIC {
public:
	static const int headerSize  = Ethernet::headerLength;
	static const int minDataSize = Ethernet::minDataLength;
	static const int maxDataSize = Ethernet::maxDataLength;

	static const int minFrameSize = headerSize + minDataSize;
	static const int maxFrameSize = headerSize + maxDataSize;

	// packet type of Xerox IDP
	enum class Type : quint16 {
		IDP = Ethernet::frameTypeIDP,
	};

    enum class Address : quint64 {
        BROADCAST = Ethernet::broadcastAddress,
    };

    using Header = Ethernet::Header;
    struct Frame {
    	Header header;
    	Block  data;
    };

	class Data {
	public:
    	quint8 data[maxFrameSize];
		Block  block;

		Data() : block(data, sizeof(data)) {}
	};

	NIC() : utilNIC() {}

	const char* getName()    const;
	Type        getType()    const;
	Address     getAddress() const;

	void attach(const char* name);
	void detach();

	// discard already received packet
	void discardPacket() const;

	void receive (Block& block) const;
	void transmit(Block& block) const;

private:
	UtillNIC utilNIC;
};

QString toString(const NIC::Address value);
QString toString(const NIC::Frame&  frame);

void serialize(Block& bloc, )

}


#endif
