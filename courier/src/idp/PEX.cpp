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
// Routing.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("pex");

#include "../util/Debug.h"
#include "../idp/PEX.h"

void PEX::deserialize(NetData& netData_) {
	id = netData_.get32();
	clientType = (ClientType)netData_.get16();

	netData.clear();
	netData.put(netData_, netData_.getPos());
	netData.rewind();
}
void PEX::serialize(NetData& netData_) {
	netData_.put32(id);
	netData_.put16((quint16)clientType);

	netData_.put(netData, 0);
}

QString toString(const PEX::ClientType value) {
	static QMap<PEX::ClientType, QString> map = {
		{PEX::ClientType::UNSPECIFIED, "UNSPECIFIED"},
		{PEX::ClientType::TIME,        "TIME"},
		{PEX::ClientType::CHS,         "CHS"},
		{PEX::ClientType::TELEDEBUG,   "TELEDEBUG"},
	};

	if (map.contains(value)) {
		return map[value];
	} else {
		return QString("%1").arg((quint8)value);
	}
}

QString toString(const PEX& value) {
	QString ret;
	ret.append(QString("[%1 %2 %3]").arg(value.id, 0, 16).arg(toString(value.clientType)).arg(toString(value.netData)));
	return ret;
}
