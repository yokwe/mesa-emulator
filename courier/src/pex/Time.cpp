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
// Time.cpp
//

#include "../util/Debug.h"
#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("pex/time");

#include "../pex/Time.h"

void PEX::Time::Request::deserialize(NetData& /*netData_*/) {
}
void PEX::Time::Request::serialize(NetData& /*netData_*/) {
}

void PEX::Time::Response::deserialize(NetData& netData_) {
    currentTime     = netData_.get32();
    offsetDirection = (OffsetDirection)netData_.get16();
    offsetHours     = netData_.get16();
    offsetMinutes   = netData_.get16();
    startOfDST      = netData_.get16();
    endOfDST        = netData_.get16();
    toleranceType   = (ToleranceType)netData_.get16();
    tolerance       = netData_.get32();
}
void PEX::Time::Response::serialize(NetData& netData_) {
	netData_.put32(currentTime);
	netData_.put16((quint16)offsetDirection);
	netData_.put16(offsetHours);
	netData_.put16(offsetMinutes);
	netData_.put16(startOfDST);
	netData_.put16(endOfDST);
	netData_.put16((quint16)toleranceType);
	netData_.put32(tolerance);
}

void PEX::Time::deserialize(NetData& netData_) {
	netData_.reset();

	version   = netData_.get16();
	if (version != VERSION_NUMBER) {
		logger.fatal("Unexpected %d", version);
		RUNTIME_ERROR();
	}
	operation = (Operation)netData_.get16();

	switch(operation) {
	case Operation::REQUEST:
		value.request.deserialize(netData_);
		break;
	case Operation::RESPONSE:
		value.response.deserialize(netData_);
		break;
	default:
		logger.fatal("Unexpected %d", (quint16)operation);
		RUNTIME_ERROR();
		break;
	}
}
void PEX::Time::serialize(NetData& netData_) {
	if (version != VERSION_NUMBER) {
		logger.fatal("Unexpected %d", version);
		RUNTIME_ERROR();
	}

	netData_.clear();

	netData_.put16(version);
	netData_.put16((quint16)operation);

	switch(operation) {
	case Operation::REQUEST:
		value.request.serialize(netData_);
		break;
	case Operation::RESPONSE:
		value.response.serialize(netData_);
		break;
	default:
		logger.fatal("Unexpected %d", (quint16)operation);
		RUNTIME_ERROR();
		break;
	}

	netData_.rewind();
}

QString toString(const PEX::Time::Operation value) {
	static QMap<PEX::Time::Operation, QString> map = {
		{PEX::Time::Operation::REQUEST,  "REQUEST"},
		{PEX::Time::Operation::RESPONSE, "RESPONSE"},
	};

	if (map.contains(value)) {
		return map[value];
	} else {
		return QString("%1").arg((quint16)value);
	}
}

QString toString(const PEX::Time::OffsetDirection value) {
	static QMap<PEX::Time::OffsetDirection, QString> map = {
		{PEX::Time::OffsetDirection::EAST, "EAST"},
		{PEX::Time::OffsetDirection::WEST, "WEST"},
	};

	if (map.contains(value)) {
		return map[value];
	} else {
		return QString("%1").arg((quint16)value);
	}
}

QString toString(const PEX::Time::ToleranceType value) {
	static QMap<PEX::Time::ToleranceType, QString> map = {
		{PEX::Time::ToleranceType::UNKNOWN,          "UNKNOWN"},
		{PEX::Time::ToleranceType::IN_MILLI_SECONDS, "IN_MILLI_SECONDS"},
	};

	if (map.contains(value)) {
		return map[value];
	} else {
		return QString("%1").arg((quint16)value);
	}
}

QString toString(const PEX::Time::Request /*value*/) {
	QString ret;
	return ret;
}
QString toString(const PEX::Time::Response value) {
	QString ret;
	ret.append(QString("[%1 %2 %3 %4 %5 %6 %7 %8]")
			.arg(value.currentTime)
			.arg(toString(value.offsetDirection))
			.arg(value.offsetHours)
			.arg(value.offsetMinutes)
			.arg(value.startOfDST)
			.arg(value.endOfDST)
			.arg(toString(value.toleranceType))
			.arg(value.tolerance));
	return ret;
}
QString toString(const PEX::Time& value) {
	QString ret;
	ret.append(QString("[%1 %2")
			.arg(value.version)
			.arg(toString(value.operation)));
	switch(value.operation) {
	case PEX::Time::Operation::REQUEST:
		ret.append(toString(value.value.request));
		break;
	case PEX::Time::Operation::RESPONSE:
		ret.append(toString(value.value.response));
		break;
	default:
		break;
	}
	ret.append("]");
	return ret;
}
