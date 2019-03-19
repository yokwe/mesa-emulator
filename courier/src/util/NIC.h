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

#ifndef IDP_NIC_H__
#define IDP_NIC_H__

#include "../util/NetData.h"

class NIC {
public:
	// packet type of Xerox IDP
	enum class Type : quint16 {
		IDP = 0x0600,
	};

    enum class Address : quint64 {
        BROADCAST = 0xFFFFFFFFFFFFLL,
    };

	class Ethernet {
	public:
		static const quint32 MAX_FRAME_SIZE = 1514;
    	static const quint32 MIN_FRAME_SIZE =   60;
		static const quint32 HEADER_SIZE    =   14;

		static const quint32 MAX_DATA_SIZE  = MAX_FRAME_SIZE - HEADER_SIZE;
		static const quint32 MIN_DATA_SIZE  = MIN_FRAME_SIZE - HEADER_SIZE;

	    void serialize  (NetData& netData);
	    void deserialize(NetData& netData);

	    Address dst;  // 6 - 48 bit mac address
	    Address src;  // 6 - 48 bit mac address
	    Type    type; // 2 - 0600 for IDP

	    quint8  data[MAX_DATA_SIZE];
	    NetData netData; // access data through netData

	    Ethernet() : dst((Address)0), src((Address)0), type((Type)0), netData(data, sizeof(data)) {
	    	::bzero(data, sizeof(data));
	    }
	};

	class Data {
	public:
    	quint8  data[Ethernet::MAX_FRAME_SIZE];
		NetData netData;

		Data() : netData(data, sizeof(data)) {}
	};


	NIC() : name(0), type(0), fd(0), address(0) {}

	const char* getName() const {
		return name;
	}
	Type getType() const {
		return (Type)type;
	}
	Address getAddress() const {
		return (Address)address;
	}

	void attach(const char* name, const quint16 type);
	void attach(const char* name, const Type type) {
		attach(name, (quint16)type);
	}
	void detach();

	// discard already received packet
	void discardRecievedPacket() const;
	void discardOnePacket() const;
	int  select(quint32 timeout, int& opErrno) const;

	// returns return code of send and recv. no error checking
	int receive (quint8* data, quint32 dataLen, int& opErrno) const;
	int transmit(quint8* data, quint32 dataLen, int& opErrno) const;

	void receive (NetData& netData) const;
	void transmit(NetData& netData) const;

	void receive (Data& data) const {
		receive(data.netData);
	}
	void transmit(Data& data) const {
		transmit(data.netData);
	}

	void receive (Ethernet& ethernet) const;
	void transmit(Ethernet& ethernet) const;

private:
	const char* name;
	quint16     type;
	int         fd;
	quint64     address;
};

QString toString(const NIC::Address value);
QString toString(const NIC::Ethernet& ethernet);

#endif
