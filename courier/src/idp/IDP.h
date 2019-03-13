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
#include "../idp/Packet.h"

class IDP {
public:
    enum class PacketType : quint8 {
        ROUTING          = 1,
        ECHO             = 2,
        ERROR            = 3,
        PACKET_EXCHANGE  = 4,
        SEQUENCED_PACKET = 5,
        BOOT_SERVER      = 9,
    };
    enum class WellKnownSocket : quint16 {
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

	static const quint32 NETWORK_UNKNOWN       = 0x0;
	static const quint32 NETWORK_ALL           = 0xFFFFFFFF;

	static const quint64 HOST_ALL              = 0xFFFFFFFFFFFFLL;

	static const quint16 CHECKSUM_NONE         = 0xFFFF;
	static const quint16 MAX_HOP_COUNT         = 16U;
	static const quint16 MAX_PACKET_LIFETIME   = 60U;

	static const quint32 IDP_HEADER_LENGTH     = 30;
	static const quint32 MIN_DATA_LENGTH       = 16;

	// Data starts from here
    quint64 eth_dst;  // 48 bit mac address
    quint64 eth_src;  // 48 bit mac address
    quint16 eth_type; // must be 0600

    quint64 ether_dst;
    quint64 ether_src;
    quint16 ether_type; // must be 0600

    quint16 checksum;
    quint16 length;     // length in byte including checksum
    quint8  hopCount;
    quint8  packetType;

    quint32 dst_net;
    quint64 dst_host;
    quint16 dst_socket;

    quint32 src_net;
    quint64 src_host;
    quint16 src_socket;

    // Length of Ethernet packet is always even number.
    // So if length of data is odd, need to output one garbage byte.
    // Minimum data length of ethernet datagram is 46. And header length of IDP is 30.
    // So if data length of IDP is less than 16, add padding to make data length 16.
    quint8  data[546];

    void receive (NIC& nic);
    void transmit(NIC& nic);
};

#endif

