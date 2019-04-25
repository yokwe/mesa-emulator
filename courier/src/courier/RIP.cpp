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
static log4cpp::Category& logger = Logger::getLogger("cr/rip");

#include "../courier/RIP.h"


// Frame

QString Courier::toString(const RIP::Frame&  value) {
	QStringList list;
	int size = value.tupleList.size();
	for(int i = 0; i < size; i++) {
		const RIP::Tuple& tuple = value.tupleList.at(i);
		list << QString("[%1 %2]").arg(Courier::toString((Courier::IDP::Network)tuple.network)).arg(Courier::toString((Courier::IDP::HopCount)tuple.hop));
	}
	return QString("[%1 (%2)[%3]]").arg(Courier::toString(value.operation)).arg(list.size()).arg(list.join(" "));
}
void Courier::serialize(Block& block, const RIP::Frame& value) {
	Courier::serialize(block, value.operation);
	for(RIP::Tuple tuple: value.tupleList) {
		Courier::serialize(block, tuple.network);
		Courier::serialize(block, tuple.hop);
	}
}
void Courier::deserialize(Block& block, RIP::Frame& value) {
	Courier::deserialize(block, value.operation);
	while(0 < block.remaining()) {
		RIP::Tuple tuple;
		Courier::deserialize(block, tuple);
		value.tupleList.append(tuple);
	}
}
