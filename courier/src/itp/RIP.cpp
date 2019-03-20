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
// RIP.cpp
//

#include "../util/Debug.h"
#include "../util/Util.h"

#include "../itp/RIP.h"

void ITP::RIP::Tupple::deserialize  (NetData& netData_) {
	network  = (IDP::Network)netData_.get32();
	hopCount = (IDP::HopCount)netData_.get16();
}
void ITP::RIP::Tupple::serialize  (NetData& netData_) {
	netData_.put32((quint32)network);
	netData_.put16((quint16)hopCount);
}

void ITP::RIP::deserialize(NetData& netData_) {
	netData_.reset();

	operation = (Operation)netData_.get16();
	while(netData_.remaining() != 0) {
		Tupple tupple;
		tupple.deserialize(netData_);
		tupples.append(tupple);
	}
}
void ITP::RIP::serialize(NetData& netData_) {
	netData_.clear();

	netData_.put16((quint16)operation);
	for(Tupple tupple: tupples) {
		tupple.serialize(netData_);
	}

	netData_.rewind();
}

QString toString(const ITP::RIP::Operation value) {
	static QMap<ITP::RIP::Operation, QString> map = {
		{ITP::RIP::Operation::REQUEST,  "REQUEST"},
		{ITP::RIP::Operation::RESPONSE, "RESPONSE"},
	};

	if (map.contains(value)) {
		return map[value];
	} else {
		return QString("%1").arg((quint8)value);
	}
}

QString toString(const ITP::RIP::Tupple& value) {
	QString ret;
	ret.append(QString("[%1 %2]").arg(toString(value.network)).arg(toString(value.hopCount)));
	return ret;
}

QString toString(const ITP::RIP& value) {
	QString ret;
	ret.append(QString("[%1 ").arg(toString(value.operation)));
	ret.append(QString("(%1)[").arg(value.tupples.size()));
	int i = 0;
	for(ITP::RIP::Tupple tupple: value.tupples) {
		if (i++) ret.append(" ");
		ret.append(toString(tupple));
	}
	ret.append("]]");
	return ret;
}
