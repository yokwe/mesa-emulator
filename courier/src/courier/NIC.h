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
	static const int headerSize  = Stub::Ethernet::headerLength;
	static const int minDataSize = Stub::Ethernet::minDataLength;
	static const int maxDataSize = Stub::Ethernet::maxDataLength;

	static const int minFrameSize = headerSize + minDataSize;
	static const int maxFrameSize = headerSize + maxDataSize;

	enum class Host : quint64 {
		ALL = Stub::Ethernet::broadcastAddress, // Broadcast
	};
	enum class Type : quint16 {
		IDP = Stub::Ethernet::frameTypeIDP, // Xerox IDP
	};

    using Header = Stub::Ethernet::Header;
    struct Frame {
    	Host  dst;  // destination
    	Host  src;  // source
        Type  type; // frame type
    	Block data{nullptr, 0};
    };

	class Data {
	public:
    	quint8 data[maxFrameSize];
		Block  block;

		Data() : block(data, sizeof(data)) {}
	};

	NIC() : utilNIC() {}

	const char* getName()    const;
	quint16     getType()    const;
	quint64     getAddress() const;

	void attach(const char* name);
	void detach();

	// discard already received packet
	void discardPacket() const;
	// unit of timeout is second
	int  select(quint32 timeoutInSec) const;


	void receive (Block& block) const;
	void transmit(Block& block) const;

	static QString toStarStyleAddress(quint64 value);
	static QString toStarStyleAddress(Host value) {
		return toStarStyleAddress((quint64)value);
	}

private:
	UtillNIC utilNIC;
};

QString toString(const Courier::NIC::Host value);
QString toString(const Courier::NIC::Type value);

// Frame
QString toString(const Courier::NIC::Frame& value);
void serialize(Block& block, const Courier::NIC::Frame& value);
void deserialize(Block& block, Courier::NIC::Frame& value);

}


#endif
