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
// Echo.cpp
//

#include "../util/Debug.h"
#include "../util/Util.h"

#include "../itp/Echo.h"

void ITP::Echo::deserialize(NetData& netData_) {
	netData_.reset();

	operation = (Operation)netData_.get16();

	netData.clear();
	netData.put(netData_, netData_.getPos());
	netData.rewind();
}
void ITP::Echo::serialize(NetData& netData_) {
	netData_.clear();

	netData_.put16((quint16)operation);

	netData_.put(netData, 0);

	netData_.rewind();
}

QString toString(const ITP::Echo::Operation value) {
	static QMap<ITP::Echo::Operation, QString> map = {
		{ITP::Echo::Operation::REQUEST,  "REQUEST"},
		{ITP::Echo::Operation::RESPONSE, "RESPONSE"},
	};

	if (map.contains(value)) {
		return map[value];
	} else {
		return QString("%1").arg((quint8)value);
	}
}

QString toString(const ITP::Echo& value) {
	QString ret;
	ret.append(QString("[%1 %2]").arg(toString(value.operation), 8).arg(toString(value.netData)));
	return ret;
}
