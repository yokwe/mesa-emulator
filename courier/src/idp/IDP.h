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

// IDP stands for Internet Datagram Protocol

#ifndef IDP_H__
#define IDP_H__

#include <QtCore>

#include "../idp/NIC.h"
#include "../idp/NetData.h"

class IDP {
public:
    enum class PacketType : quint8 {
        ROUTING          = 1,
        ECHO             = 2,
        ERROR            = 3,
        PACKET_EXCHANGE  = 4, // PEX
        SEQUENCED_PACKET = 5, // SPP
        BOOT_SERVER      = 9, // BOOT
    };

    enum class Socket : quint16 {
        ROUTING           = 1,
        ECHO              = 2,
        ERROR             = 3,
        ENVOY             = 4,
        COURIER           = 5,
        CLEARINGHOUSE_OLD = 7,
        TIME              = 8,
        BOOT              = 10,
        DIAG              = 19,
        CLEARINGHOUSE     = 20,
        AUTH              = 21,
        MAIL              = 22,
        NET_EXEC          = 23,
        WS_INFO           = 24,
        BINDING           = 28,
        GERM              = 35,
        TELE_DEBUG        = 48,
		//
		MAX_WELL_KNOWN_SOCKET = 3000,
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

	static const quint16 MAX_PACKET_LIFETIME   = 60U;

	static quint16 computeChecksum(quint8* data, quint32 offset, quint32 length);

    void serialize  (NetData& netData);
    void deserialize(NetData& netData);

	// Data starts from here
    Checksum   checksum;
    quint16    length;     // length in byte including checksum
    HopCount   hopCount;
    PacketType packetType;

    Network dst_net;
    Host    dst_host;
    Socket  dst_socket;

    Network src_net;
    Host    src_host;
    Socket  src_socket;

    quint8  data[546];
    NetData netData; // access data through netData

    quint16 computedChecksum; // value is assigned after deserialize

    IDP() : checksum((Checksum)0), length(0), hopCount((HopCount)0), packetType((PacketType)0),
    		dst_net((Network)0), dst_host((Host)0), dst_socket((Socket)0),
    		src_net((Network)0), src_host((Host)0), src_socket((Socket)0),
			netData(data, sizeof(data)), computedChecksum(0) {
    	::bzero(data, sizeof(data));
    }

    // Length of Ethernet packet is always even number.
    // So if length of data is odd, need to output one garbage byte.
};

QString toString(const IDP::PacketType value);
QString toString(const IDP::Socket value);
QString toString(const IDP::Network value);
QString toString(const IDP::Host value);
QString toString(const IDP::Checksum value);
QString toString(const IDP::HopCount value);
QString toString(const IDP& value);

#endif

