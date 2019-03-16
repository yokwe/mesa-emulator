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
// SPP.cpp
//

#include "../util/Debug.h"
#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("spp");

#include "../itp/SPP.h"

void ITP::SPP::deserialize(NetData& netData_) {
	control.u   = netData_.get8();
	sst         = (SST)netData_.get8();
	src_conn_id = netData_.get16();
	dst_conn_id = netData_.get16();
	sequence    = netData_.get16();
	acknowledge = netData_.get16();
	allocation  = netData_.get16();

	netData.clear();
	netData.put(netData_, netData_.getPos());
	netData.rewind();
}
void ITP::SPP::serialize(NetData& netData_) {
	netData_.put8(control.u);
	netData_.put8((quint8)sst);
	netData_.put16(src_conn_id);
	netData_.put16(dst_conn_id);
	netData_.put16(sequence);
	netData_.put16(acknowledge);
	netData_.put16(allocation);

	netData_.put(netData, 0);
}

QString toString(const ITP::SPP::Control value) {
	return QString("[%1 %2 %3 %4 %5]")
			.arg(value.systemPacket ? "SYS " : "")
			.arg(value.sendAcknowledge ? "SEND_ACK " : "")
			.arg(value.attention ? "ATT " : "")
			.arg(value.endOfMessage ? "EOM " : "")
			.arg(value.reserved);
}

QString toString(const ITP::SPP::SST value) {
	static QMap<ITP::SPP::SST, QString> map = {
		{ITP::SPP::SST::DATA,        "DATA"},
		{ITP::SPP::SST::BULK,        "BULK"},
		{ITP::SPP::SST::CLOSE,       "CLOSE"},
		{ITP::SPP::SST::CLOSE_REPLY, "CLOSE_REPLY"},
	};

	if (map.contains(value)) {
		return map[value];
	} else {
		return QString("%1").arg((quint8)value);
	}
}

QString toString(const ITP::SPP& value) {
	QString ret;
	ret.append(QString("[%1 %2 %3 %4 %5 %6 %7]")
			.arg(toString(value.control))
			.arg(toString(value.sst))
			.arg(value.src_conn_id, 4, 16)
			.arg(value.dst_conn_id, 4, 16)
			.arg(value.sequence)
			.arg(value.acknowledge)
			.arg(value.allocation)
			.arg(toString(value.netData)));
	return ret;
}
