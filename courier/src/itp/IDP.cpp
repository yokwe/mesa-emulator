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
static log4cpp::Category& logger = Logger::getLogger("idp");

#include "../itp/IDP.h"

QString toString(const ITP::IDP::PacketType value) {
	static QMap<ITP::IDP::PacketType, QString> map = {
	    {ITP::IDP::PacketType::RIP,   "RIP"},
	    {ITP::IDP::PacketType::ECHO,  "ECHO"},
	    {ITP::IDP::PacketType::ERROR, "ERROR"},
	    {ITP::IDP::PacketType::PEX,   "PEX"},
	    {ITP::IDP::PacketType::SPP,   "SPP"},
	    {ITP::IDP::PacketType::BOOT,  "BOOT"},
	};

	if (map.contains(value)) {
		return map[value];
	} else {
		return QString("%1").arg((quint16)value);
	}
}

QString toString(const ITP::IDP::Socket value) {
	static QMap<ITP::IDP::Socket, QString> map = {
	    {ITP::IDP::Socket::RIP,       "RIP"},
	    {ITP::IDP::Socket::ECHO,      "ECHO"},
	    {ITP::IDP::Socket::ERROR,     "ERROR"},
	    {ITP::IDP::Socket::ENVOY,     "ENVOY"},
	    {ITP::IDP::Socket::COURIER,   "COURIER"},
	    {ITP::IDP::Socket::CHS_OLD,   "CHS_OLD"},
	    {ITP::IDP::Socket::TIME,      "TIME"},
	    {ITP::IDP::Socket::BOOT,      "BOOT"},
	    {ITP::IDP::Socket::DIAG,      "DIAG"},
	    {ITP::IDP::Socket::CHS,       "CHS"},
	    {ITP::IDP::Socket::AUTH,      "AUTH"},
	    {ITP::IDP::Socket::MAIL,      "MAIL"},
	    {ITP::IDP::Socket::NET_EXEC,  "NET_EXEC"},
	    {ITP::IDP::Socket::WS_INFO,   "WS_INFO"},
	    {ITP::IDP::Socket::BINDING,   "BINDING"},
	    {ITP::IDP::Socket::GERM,      "GERM"},
	    {ITP::IDP::Socket::TELEDEBUG, "TELEDEBUG"},
	};

	if (map.contains(value)) {
		return map[value];
	} else {
		return QString("%1").arg((quint8)value);
	}
}

QString toString(const ITP::IDP::Network value) {
	static QMap<ITP::IDP::Network, QString> map = {
		    {ITP::IDP::Network::UNKNOWN, "UNKNOWN"},
		    {ITP::IDP::Network::ALL,     "ALL"},
	};

	if (map.contains(value)) {
		return map[value];
	} else {
		return QString("%1").arg((quint32)value, 0, 16).toUpper();
	}
}

QString toString(const ITP::IDP::Host value) {
	static QMap<ITP::IDP::Host, QString> map = {
		    {ITP::IDP::Host::ALL, "ALL"},
	};

	if (map.contains(value)) {
		return map[value];
	} else {
		return QString("%1").arg((quint64)value, 0, 16).toUpper();
	}
}

QString toString(const ITP::IDP::Checksum value) {
	static QMap<ITP::IDP::Checksum, QString> map = {
		    {ITP::IDP::Checksum::NONE, "NONE"},
	};

	if (map.contains(value)) {
		return map[value];
	} else {
		return QString("%1").arg((quint64)value, 4, 16, QLatin1Char('0')).toUpper();
	}
}

QString toString(const ITP::IDP::HopCount value) {
	static QMap<ITP::IDP::HopCount, QString> map = {
		    {ITP::IDP::HopCount::MAX, "MAX"},
	};

	if (map.contains(value)) {
		return map[value];
	} else {
		return QString("%1").arg((quint8)value);
	}
}

QString toString(const ITP::IDP& value) {
	return QString("[%1 %2 %3 %4   [%5 %6 %7]   [%8 %9 %10] %11]")
			.arg(toString(value.checksum)).arg(value.length, 4).arg(toString(value.hopCount)).arg(toString(value.packetType))
			.arg(toString(value.dst_net)).arg(toString(value.dst_host)).arg(toString(value.dst_socket))
			.arg(toString(value.src_net)).arg(toString(value.src_host)).arg(toString(value.src_socket))
			.arg(toString(value.netData));
}

void ITP::IDP::deserialize(NetData& netData_) {
	quint32 pos = netData_.getPos();

	checksum   = (ITP::IDP::Checksum)netData_.get16();
	length     = netData_.get16();
	hopCount   = (ITP::IDP::HopCount)netData_.get8();
	packetType = (ITP::IDP::PacketType)netData_.get8();

	dst_net    = (ITP::IDP::Network)netData_.get32();
	dst_host   = (ITP::IDP::Host)netData_.get48();
	dst_socket = (ITP::IDP::Socket)netData_.get16();

	src_net    = (ITP::IDP::Network)netData_.get32();
	src_host   = (ITP::IDP::Host)netData_.get48();
	src_socket = (ITP::IDP::Socket)netData_.get16();

	netData.clear();
	for(;;) {
		quint32 pos = netData_.getPos();
		if (pos == length) break;
		netData.put8(netData_.get8());
	}
	netData.rewind();

	// 2 for checksum field
	computedChecksum = computeChecksum(netData_.getData(), pos + 2, length - 2);

	if (checksum != Checksum::NONE && (quint16)checksum != computedChecksum) {
		logger.warn("CHECKSUM %04X %04X", (quint16)checksum, computedChecksum);
	}
}
void ITP::IDP::serialize  (NetData& netData_) {
	quint32 pos = netData_.getPos();

	netData_.put16((quint16)checksum);
	netData_.put16((quint16)length);
	netData_.put8 ((quint8)hopCount);
	netData_.put8 ((quint8)packetType);

	netData_.put32((quint32)dst_net);
	netData_.put48((quint64)dst_host);
	netData_.put16((quint16)dst_socket);

	netData_.put32((quint32)src_net);
	netData_.put48((quint64)src_host);
	netData_.put16((quint16)src_socket);

	netData_.put(netData, 0);

	// Update length
	length = netData_.getPos();
	netData_.set16(pos + 2, length);

	// Append garbage byte if number of data is odd
	if (netData.getLimit() & 1) {
		netData_.put8(0);
	}

	// Update checksum
	// 2 for checksum field
	computedChecksum = computeChecksum(netData_.getData(), pos + 2, length - 2);
	checksum = (Checksum)computedChecksum;
	netData_.set16(pos + 0, (quint16)checksum);
}

quint16 ITP::IDP::computeChecksum(quint8* data, quint32 offset, quint32 length) {
	quint32 s = 0;

	// Calculate checksum based 16 bit word. not 8 bit.
	// So if length is odd number, one bye beyond length is used to calculate checksum
	for(quint32 i = 0; i < length; i += 2) {
		quint16 w = data[offset++] & 0x00ffU;
		w = (w << 8) | (data[offset++] & 0x00ffU);
		// add w to s
		s += w;
		// if there is overflow, increment t
		if (0x10000U <= s) s = (s + 1) & 0xffffU;
		// shift left
		s <<= 1;
		// if there is overflow, increment t
		if (0x10000U <= s) s = (s + 1) & 0xffffU;
	}
	return (quint16)s;
}
