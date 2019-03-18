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
// Error.cpp
//

#include "../util/Debug.h"
#include "../util/Util.h"

#include "../itp/Error.h"

void ITP::Error::deserialize(NetData& netData_) {
	errorNumber    = (ErrorNumber)netData_.get16();
	errorParameter = netData_.get16();

	netData.clear();
	netData.put(netData_, netData_.getPos());
	netData.rewind();
}
void ITP::Error::serialize(NetData& netData_) const {
	netData_.put16((quint16)errorNumber);
	netData_.put16(errorParameter);

	netData_.put(netData, 0);
}

QString toString(const ITP::Error::ErrorNumber value) {
	static QMap<ITP::Error::ErrorNumber, QString> map = {
        {ITP::Error::ErrorNumber::UNSPECIFIED,          "UNSPECIFIED"},
        {ITP::Error::ErrorNumber::BAD_CHECKSUM,         "BAD_CHECKSUM"},
        {ITP::Error::ErrorNumber::NO_SOCKET,            "NO_SOCKET"},
        {ITP::Error::ErrorNumber::RESOURCE_LIMIT,       "RESOURCE_LIMIT"},
        {ITP::Error::ErrorNumber::LISTEN_REJECT,        "LISTEN_REJECT"},
        {ITP::Error::ErrorNumber::INVALID_PACKET_TYPE,  "INVALID_PACKET_TYPE"},
        {ITP::Error::ErrorNumber::PROTOCOL_VIOLATION,   "PROTOCOL_VIOLATION"},
        {ITP::Error::ErrorNumber::UNSPECIFIED_IN_ROUTE, "UNSPECIFIED_IN_ROUTE"},
        {ITP::Error::ErrorNumber::INCONSISTENT,         "INCONSISTENT"},
        {ITP::Error::ErrorNumber::CANT_GET_THERE,       "CANT_GET_THERE"},
        {ITP::Error::ErrorNumber::EXCESS_HOPS,          "EXCESS_HOPS"},
        {ITP::Error::ErrorNumber::TOO_BIG,              "TOO_BIG"},
        {ITP::Error::ErrorNumber::CONGESTION_WARNING,   "CONGESTION_WARNING"},
        {ITP::Error::ErrorNumber::CONGESTION_DISCARD,   "CONGESTION_DISCARD"},
	};

	if (map.contains(value)) {
		return map[value];
	} else {
		return QString("%1").arg((quint8)value);
	}
}

QString toString(const ITP::Error& value) {
	QString ret;
	ret.append(QString("[%1 %2 %3]").arg(toString(value.errorNumber)).arg(value.errorParameter).arg(toString(value.netData)));
	return ret;
}
