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
// NIC.cpp
//

#include "../util/Debug.h"
#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("cr/nic");

#include "../courier/NIC.h"


const char* Courier::NIC::getName() const {
	return utilNIC.getName();
}
quint16 Courier::NIC::getType() const {
	return (quint16)utilNIC.getType();
}
quint64 Courier::NIC::getAddress() const {
	return (quint64) utilNIC.getAddress();
}

void Courier::NIC::attach(const char* name) {
	utilNIC.attach(name, Stub::Ethernet::frameTypeIDP);
}
void Courier::NIC::detach() {
	utilNIC.detach();
}

// discard already received packet
void Courier::NIC::discardPacket() const {
	utilNIC.discardRecievedPacket();
}

int Courier::NIC::select(quint32 timeoutInSec) const {
	int opErrno = 0;
	int ret = utilNIC.select(timeoutInSec, opErrno);
	if (ret < 0) {
		logger.fatal("receive select = %d  opErrno = %d %s", ret, opErrno, strerror(opErrno));
		RUNTIME_ERROR();
	}
	return ret;
}

void Courier::NIC::receive (Block& block) const {
	block.zero();
	int opErrno = 0;
	int ret = utilNIC.receive(block.getData(), block.getCapacity(), opErrno);
	if (ret < 0) {
		logger.fatal("receive ret = %d  opErrno = %d %s", ret, opErrno, strerror(opErrno));
		RUNTIME_ERROR();
	}
	block.clear();
	block.setLimit((quint16)ret);
	block.rewind();
}
void Courier::NIC::transmit(Block& block) const {
	int opErrno = 0;
	int limit = block.getLimit();

	int ret = utilNIC.transmit(block.getData(), limit, opErrno);
	if (ret == -1) {
		logger.fatal("transmit ret = %d  opErrno = %d %s", ret, opErrno, strerror(opErrno));
		RUNTIME_ERROR();
	}
	if (ret != limit) {
		logger.fatal("transmit ret = %d  limit = %d", ret, limit);
		RUNTIME_ERROR();
	}
}

QString Courier::toString(const Courier::NIC::Host value) {
    static QMap<Courier::NIC::Host, QString> map = {
        {Courier::NIC::Host::ALL, "ALL"},
    };

    if (map.contains(value)) {
        return map[value];
    } else {
        return QString("%1").arg((quint64)value, 0, 16).toUpper();
    }
}
QString Courier::toString(const Courier::NIC::Type value) {
    static QMap<Courier::NIC::Type, QString> map = {
        {Courier::NIC::Type::IDP, "IDP"},
    };

    if (map.contains(value)) {
        return map[value];
    } else {
        return QString("%1").arg((quint64)value, 0, 16).toUpper();
    }
}

QString Courier::NIC::toStarStyleAddress(quint64 value) {
	const QChar ZERO = QLatin1Char('0');

	// special case for less than 1000
	if (value < 1000) {
		return QString("0-%1").arg(QString("%1").arg(value, 3, 10, ZERO));
	}

	QString ret(QString("%1").arg(value % 1000, 3, 10, ZERO));
	value = value / 1000;

	for(;;) {
		quint64 nnn = value % 1000;

		if (value < 1000) {
			ret.prepend(QString("%1-").arg(nnn, 0, 10, ZERO));
			break;
		} else {
			ret.prepend(QString("%1-").arg(nnn, 3, 10, ZERO));
		}

		value = value / 1000;
	}

	return ret;
}



// Frame
QString Courier::toString(const NIC::Frame& value) {
	QString ret = QString("[%1 %2 %3] %4")
			.arg(toString(value.dst))
			.arg(toString(value.src))
			.arg(toString(value.type))
			.arg(toString(value.data));
	return ret;
}
void Courier::serialize(Block& block, const Courier::NIC::Frame& value) {
	Stub::Ethernet::Header header;
	header.destination = (decltype(header.destination))value.dst;
	header.source      = (decltype(header.source))     value.src;
	header.type        = (decltype(header.type))       value.type;

	Courier::serialize(block, header);
	Courier::serialize(block, value.data);
}
void Courier::deserialize(Block& block, Courier::NIC::Frame& value) {
	Stub::Ethernet::Header header;
	Courier::deserialize(block, header);
	value.data = block.remainder();

	value.dst  = (decltype(value.dst)) header.destination;
	value.src  = (decltype(value.src)) header.source;
	value.type = (decltype(value.type))header.type;
}
