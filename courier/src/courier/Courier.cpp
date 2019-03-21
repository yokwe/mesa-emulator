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
// Courier.cpp
//

#include "../util/Debug.h"
#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("cr/courier");

#include "../courier/Courier.h"

void Courier::serialize  (Block& block, const QString& value) {
	int size = value.size();
	if (65535 <= size) {
		logger.error("Unexpected overflow  size = %ds", size);
		RUNTIME_ERROR();
	}

	Courier::serialize(block, (quint16)size);

	quint8 c;
	for(int i = 0; i < size; i++) {
		c = (quint8)value.at(i).toLatin1();
		Courier::serialize(block, c);
	}
	if (size & 1) {
		c = 0;
		Courier::serialize(block, c);
	}
}
void Courier::deserialize(Block& block, QString& value) {
	quint16 size;
	Courier::deserialize(block, size);

	quint8 c;
	value.resize(0);
	for(quint16 i = 0; i < size; i++) {
		Courier::deserialize(block, c);
		value.append(c);
	}
	if (size & 1) {
		Courier::deserialize(block, c);
	}
}
