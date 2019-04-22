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

#include "../util/Debug.h"
#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("cr/idp");

#include "../courier/IDP.h"


QString Courier::toString(const Courier::IDP::PacketType value) {
	static QMap<Courier::IDP::PacketType, QString> map = {
	    {Courier::IDP::PacketType::RIP,   "RIP"},
	    {Courier::IDP::PacketType::ECHO,  "ECHO"},
	    {Courier::IDP::PacketType::ERROR, "ERROR"},
	    {Courier::IDP::PacketType::PEX,   "PEX"},
	    {Courier::IDP::PacketType::SPP,   "SPP"},
	    {Courier::IDP::PacketType::BOOT,  "BOOT"},
	};

	if (map.contains(value)) {
		return map[value];
	} else {
		return QString("%1").arg((quint8)value);
	}
}
QString Courier::toString(const Courier::IDP::Socket     value) {
	static QMap<Courier::IDP::Socket, QString> map = {
	    {Courier::IDP::Socket::RIP,       "RIP"},
	    {Courier::IDP::Socket::ECHO,      "ECHO"},
	    {Courier::IDP::Socket::ERROR,     "ERROR"},
	    {Courier::IDP::Socket::ENVOY,     "ENVOY"},
	    {Courier::IDP::Socket::COURIER,   "COURIER"},
	    {Courier::IDP::Socket::CHS_OLD,   "CHS_OLD"},
	    {Courier::IDP::Socket::TIME,      "TIME"},
	    {Courier::IDP::Socket::BOOT,      "BOOT"},
	    {Courier::IDP::Socket::DIAG,      "DIAG"},
	    {Courier::IDP::Socket::CHS,       "CHS"},
	    {Courier::IDP::Socket::AUTH,      "AUTH"},
	    {Courier::IDP::Socket::MAIL,      "MAIL"},
	    {Courier::IDP::Socket::NET_EXEC,  "NET_EXEC"},
	    {Courier::IDP::Socket::WS_INFO,   "WS_INFO"},
	    {Courier::IDP::Socket::BINDING,   "BINDING"},
	    {Courier::IDP::Socket::GERM,      "GERM"},
	    {Courier::IDP::Socket::TELEDEBUG, "TELEDEBUG"},
	};

	if (map.contains(value)) {
		return map[value];
	} else {
		return QString("%1").arg((quint16)value, 4, 16).toUpper();
	}
}
QString Courier::toString(const Courier::IDP::Network    value) {
	static QMap<Courier::IDP::Network, QString> map = {
		    {Courier::IDP::Network::UNKNOWN, "UNKNOWN"},
		    {Courier::IDP::Network::ALL,     "ALL"},
	};

	if (map.contains(value)) {
		return map[value];
	} else {
		return QString("%1").arg((quint32)value, 0, 16).toUpper();
	}
}
QString Courier::toString(const Courier::IDP::Host       value) {
	static QMap<Courier::IDP::Host, QString> map = {
		    {Courier::IDP::Host::ALL, "ALL"},
	};

	if (map.contains(value)) {
		return map[value];
	} else {
		return QString("%1").arg((quint64)value, 0, 16).toUpper();
	}
}
QString Courier::toString(const Courier::IDP::Checksum value) {
	static QMap<Courier::IDP::Checksum, QString> map = {
		    {Courier::IDP::Checksum::NONE, "NONE"},
	};

	if (map.contains(value)) {
		return map[value];
	} else {
		return QString("%1").arg((quint64)value, 4, 16, QLatin1Char('0')).toUpper();
	}
}

QString Courier::toString(const Courier::IDP::HopCount value) {
	static QMap<Courier::IDP::HopCount, QString> map = {
		    {Courier::IDP::HopCount::MAX, "MAX"},
	};

	if (map.contains(value)) {
		return map[value];
	} else {
		return QString("%1").arg((quint8)value, 2);
	}
}

QString Courier::toString(const IDP::NetworkAddress&  value) {
	return QString("[%1 %2 %3]")
			.arg(Courier::toString(value.network))
			.arg(Courier::toString(value.host))
			.arg(Courier::toString(value.socket));
}

// Frame

QString Courier::toString(const IDP::Frame&  value) {
	QString ret = QString("[%1 %2 %3 %4 %5 %6 %7]")
			.arg(Courier::toString(value.checksum))
			.arg(value.length, 4)
			.arg(Courier::toString(value.hopCount))
			.arg(Courier::toString(value.packetType))
			.arg(Courier::toString(value.dst))
			.arg(Courier::toString(value.src))
			.arg(Courier::toString(value.data));
	return ret;
}
void Courier::serialize(Block& block, const IDP::Frame& value) {
	Datagram::Header header;
	header.checksum    = (decltype(header.checksum))   value.checksum;
	header.length      = (decltype(header.length))     value.length;
	header.hopCount    = (decltype(header.hopCount))   value.hopCount;
	header.packetType  = (decltype(header.packetType)) value.packetType;

	header.destination.network = (decltype(header.destination.network))value.dst.network;
	header.destination.host    = (decltype(header.destination.host))   value.dst.host;
	header.destination.socket  = (decltype(header.destination.socket)) value.dst.socket;

	header.source.network = (decltype(header.source.network))value.src.network;
	header.source.host    = (decltype(header.source.host))   value.src.host;
	header.source.socket  = (decltype(header.source.socket)) value.src.socket;

	Courier::serialize(block, header);
	Courier::serialize(block, value.data);
}
void Courier::deserialize(Block& block, IDP::Frame& value) {
	Datagram::Header header;
	Courier::deserialize(block, header);
	// reflect header.length in block
	block.setLimit(header.length);
	value.data = block.remainder();

	value.checksum   = (decltype(value.checksum))  header.checksum;
	value.length     = (decltype(value.length))    header.length;
	value.hopCount   = (decltype(value.hopCount))  header.hopCount;
	value.packetType = (decltype(value.packetType))header.packetType;

	value.dst.network = (decltype(value.dst.network)) header.destination.network;
	value.dst.host    = (decltype(value.dst.host))    header.destination.host;
	value.dst.socket  = (decltype(value.dst.socket))  header.destination.socket;

	value.src.network = (decltype(value.src.network)) header.source.network;
	value.src.host    = (decltype(value.src.host))    header.source.host;
	value.src.socket  = (decltype(value.src.socket))  header.source.socket;
}

