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
// IDP.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("idp");

#include "../util/Debug.h"

#include <QtCore>

#include "../idp/IDP.h"

QString toString(const IDP::PacketType value) {
	static QMap<IDP::PacketType, QString>map = {
	    {IDP::PacketType::ROUTING,          "ROUTING"},
	    {IDP::PacketType::ECHO,             "ECHO"},
	    {IDP::PacketType::ERROR,            "ERROR"},
	    {IDP::PacketType::PACKET_EXCHANGE,  "PEX"},
	    {IDP::PacketType::SEQUENCED_PACKET, "SPP"},
	    {IDP::PacketType::BOOT_SERVER,      "BOOT"},
	};

	if (map.contains(value)) {
		return map[value];
	} else {
		return QString("%1").arg((quint8)value);
	}
}

QString toString(const IDP::Socket value) {
	static QMap<IDP::Socket, QString>map = {
	    {IDP::Socket::ROUTING,           "ROUTING"},
	    {IDP::Socket::ECHO,              "ECHO"},
	    {IDP::Socket::ERROR,             "ERROR"},
	    {IDP::Socket::ENVOY,             "ENVOY"},
	    {IDP::Socket::COURIER,           "COURIER"},
	    {IDP::Socket::CLEARINGHOUSE_OLD, "CLEARINGHOUSE_OLD"},
	    {IDP::Socket::TIME,              "TIME"},
	    {IDP::Socket::BOOT,              "BOOT"},
	    {IDP::Socket::DIAG,              "DIAG"},
	    {IDP::Socket::CLEARINGHOUSE,     "CLEARINGHOUSE"},
	    {IDP::Socket::AUTH,              "AUTH"},
	    {IDP::Socket::MAIL,              "MAIL"},
	    {IDP::Socket::NET_EXEC,          "NET_EXEC"},
	    {IDP::Socket::WS_INFO,           "WS_INFO"},
	    {IDP::Socket::BINDING,           "BINDING"},
	    {IDP::Socket::GERM,              "GERM"},
	    {IDP::Socket::TELE_DEBUG,        "TELE_DEBUG"},
	};

	if (map.contains(value)) {
		return map[value];
	} else {
		return QString("%1").arg((quint8)value);
	}
}

QString toString(const IDP::Network value) {
	static QMap<IDP::Network, QString>map = {
		    {IDP::Network::UNKNOWN, "UNKNOWN"},
		    {IDP::Network::ALL,     "ALL"},
	};

	if (map.contains(value)) {
		return map[value];
	} else {
		return QString("%1").arg((quint32)value, 0, 16).toUpper();
	}
}

QString toString(const IDP::Host value) {
	static QMap<IDP::Host, QString>map = {
		    {IDP::Host::ALL, "ALL"},
	};

	if (map.contains(value)) {
		return map[value];
	} else {
		return QString("%1").arg((quint64)value, 0, 16).toUpper();
	}
}

QString toString(const IDP::Checksum value) {
	static QMap<IDP::Checksum, QString>map = {
		    {IDP::Checksum::NONE, "NONE"},
	};

	if (map.contains(value)) {
		return map[value];
	} else {
		return QString("%1").arg((quint64)value, 4, 16, QLatin1Char('0')).toUpper();
	}
}

QString toString(const IDP::HopCount value) {
	static QMap<IDP::HopCount, QString>map = {
		    {IDP::HopCount::MAX, "MAX"},
	};

	if (map.contains(value)) {
		return map[value];
	} else {
		return QString("%1").arg((quint8)value);
	}
}

QString toString(const IDP& value) {
	return QString("[%1 %2 %3 %4   [%5 %6 %7]   [%8 %9 %10]]")
			.arg(toString(value.checksum)).arg(value.length, 4).arg(toString(value.hopCount)).arg(toString(value.packetType))
			.arg(toString(value.dst_net)).arg(toString(value.dst_host)).arg(toString(value.dst_socket))
			.arg(toString(value.src_net)).arg(toString(value.src_host)).arg(toString(value.src_socket));
}

void deserialize(NetData& netData, IDP& idp) {
	idp.checksum   = (IDP::Checksum)netData.get16();
	idp.length     = netData.get16();
	idp.hopCount   = (IDP::HopCount)netData.get8();
	idp.packetType = (IDP::PacketType)netData.get8();

	idp.dst_net    = (IDP::Network)netData.get32();
	idp.dst_host   = (IDP::Host)netData.get48();
	idp.dst_socket = (IDP::Socket)netData.get16();

	idp.src_net    = (IDP::Network)netData.get32();
	idp.src_host   = (IDP::Host)netData.get48();
	idp.src_socket = (IDP::Socket)netData.get16();
}
void serialize  (NetData& netData, IDP& idp) {
	netData.put16((quint16)idp.checksum);
	netData.put16(idp.length);
	netData.put8 ((quint8)idp.hopCount);
	netData.put8 ((quint8)idp.packetType);

	netData.put32((quint32)idp.dst_net);
	netData.put48((quint64)idp.dst_host);
	netData.put16((quint16)idp.dst_socket);

	netData.put32((quint32)idp.src_net);
	netData.put48((quint64)idp.src_host);
	netData.put16((quint16)idp.src_socket);
}

