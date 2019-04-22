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
// IDP.h
//

#ifndef COURIER_IDP_H__
#define COURIER_IDP_H__

#include "../courier/Block.h"

namespace Courier {

class IDP {
public:
    enum class PacketType : quint8 {
        RIP   = 1, // Routing
        ECHO  = 2,
        ERROR = 3,
        PEX   = 4, // Packet Exchange
        SPP   = 5, // Sequenced Packet
        BOOT  = 9, // BOOT Server
    };

    enum class Socket : quint16 {
        RIP       = 1,
        ECHO      = 2,
        ERROR     = 3,
        ENVOY     = 4,
        COURIER   = 5,
        CHS_OLD   = 7,
        TIME      = 8,
        BOOT      = 10,
        DIAG      = 19,
        CHS       = 20,
        AUTH      = 21,
        MAIL      = 22,
        NET_EXEC  = 23,
        WS_INFO   = 24,
        BINDING   = 28,
        GERM      = 35,
        TELEDEBUG = 48,
    };

    enum class Network : quint32 {
    	UNKNOWN = 0,
    	ALL     = 0xFFFFFFFF,
    };

    enum class Host : quint64 {
    	ALL = 0xFFFFFFFFFFFFLL,
    };

    enum class Checksum : quint16 {
    	NONE = 0xFFFF,
    };

    enum class HopCount : quint8 {
    	MAX = 16,
    };

    struct NetworkAddress {
        Network network;
        Host    host;
        Socket  socket;
    };

    struct Frame {
    	Checksum       checksum;
        quint16        length;
        HopCount       hopCount;
        PacketType     packetType;
        NetworkAddress dst;
        NetworkAddress src;

    	Block  data{nullptr, 0};
    };

};

QString toString(const Courier::IDP::PacketType value);
QString toString(const Courier::IDP::Socket     value);
QString toString(const Courier::IDP::Network    value);
QString toString(const Courier::IDP::Host       value);
QString toString(const Courier::IDP::Checksum   value);
QString toString(const Courier::IDP::HopCount   value);

QString toString(const Courier::IDP::NetworkAddress& value);


// Frame
QString toString(const IDP::Frame&  frame);
void serialize(Block& block, const IDP::Frame& frame);
void deserialize(Block& block, IDP::Frame& frame);

}

#endif
