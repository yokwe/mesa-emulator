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

// serialize - write value to block
void Courier::BLOCK::serialize  (const quint8  value) {
	const quint16 size = 1;
	if (limit <= (pos + size)) {
		logger.error("Unexpected overflow  pos = %d  limit = %d  size = %d", pos, limit, size);
		RUNTIME_ERROR();
	}

	data[pos++] = value;
}


void Courier::BLOCK::serialize  (const quint16 value) {
	const quint16 size = 2;
	if (limit <= (pos + size)) {
		logger.error("Unexpected overflow  pos = %d  limit = %d  size = %d", pos, limit, size);
		RUNTIME_ERROR();
	}

	quint8 p1 = (quint8)(value >> 0);
	quint8 p0 = (quint8)(value >> 8);

	data[pos++] = p0;
	data[pos++] = p1;

}
void Courier::BLOCK::serialize  (const quint32 value) {
	const quint16 size = 4;
	if (limit <= (pos + size)) {
		logger.error("Unexpected overflow  pos = %d  limit = %d  size = %d", pos, limit, size);
		RUNTIME_ERROR();
	}

	quint8 p3 = (quint8)(value >> 0);
	quint8 p2 = (quint8)(value >> 8);
	quint8 p1 = (quint8)(value >> 16);
	quint8 p0 = (quint8)(value >> 24);

	data[pos++] = p0;
	data[pos++] = p1;
	data[pos++] = p2;
	data[pos++] = p3;
}
void Courier::BLOCK::serialize  (const quint64 value) {
	const quint16 size = 6;
	if (limit <= (pos + size)) {
		logger.error("Unexpected overflow  pos = %d  limit = %d  size = %d", pos, limit, size);
		RUNTIME_ERROR();
	}

	quint8 p5 = (quint8)(value >> 0);
	quint8 p4 = (quint8)(value >> 8);
	quint8 p3 = (quint8)(value >> 16);
	quint8 p2 = (quint8)(value >> 24);
	quint8 p1 = (quint8)(value >> 32);
	quint8 p0 = (quint8)(value >> 40);

	data[pos++] = p0;
	data[pos++] = p1;
	data[pos++] = p2;
	data[pos++] = p3;
	data[pos++] = p4;
	data[pos++] = p5;
}
// write whole value
void Courier::BLOCK::serialize  (const BLOCK&  value) {
	quint16 size = value.limit;
	if (limit <= (pos + size)) {
		logger.error("Unexpected overflow  pos = %d  limit = %d  size = %d", pos, limit, size);
		RUNTIME_ERROR();
	}

	for(quint16 i = 0; i < size; i++) {
		data[pos++] = value.data[i];
	}
}

// deserialize - read from block and write to value
void Courier::BLOCK::deserialize(quint8&  value) {
	quint16 size = 1;
	if (limit <= (pos + size)) {
		logger.error("Unexpected overflow  pos = %d  limit = %d  size = %d", pos, limit, size);
		RUNTIME_ERROR();
	}
	value = data[pos++];
}
void Courier::BLOCK::deserialize(quint16& value) {
	quint16 size = 2;
	if (limit <= (pos + size)) {
		logger.error("Unexpected overflow  pos = %d  limit = %d  size = %d", pos, limit, size);
		RUNTIME_ERROR();
	}

	quint16 p0 = data[pos++];
	quint16 p1 = data[pos++];

	value = (p0 << 8) | (p1 << 0);
}
void Courier::BLOCK::deserialize(quint32& value) {
	quint16 size = 4;
	if (limit <= (pos + size)) {
		logger.error("Unexpected overflow  pos = %d  limit = %d  size = %d", pos, limit, size);
		RUNTIME_ERROR();
	}

	quint32 p0 = data[pos++];
	quint32 p1 = data[pos++];
	quint32 p2 = data[pos++];
	quint32 p3 = data[pos++];

	value = (p0 << 24) | (p1 << 16) | (p2 << 8) | (p3 << 0);
}
// read 48 bit
void Courier::BLOCK::deserialize(quint64& value) {
	quint16 size = 6;
	if (limit <= (pos + size)) {
		logger.error("Unexpected overflow  pos = %d  limit = %d  size = %d", pos, limit, size);
		RUNTIME_ERROR();
	}

	quint64 p0 = data[pos++];
	quint64 p1 = data[pos++];
	quint64 p2 = data[pos++];
	quint64 p3 = data[pos++];
	quint64 p4 = data[pos++];
	quint64 p5 = data[pos++];

	value = (p0 << 40) | (p1 << 32) | (p2 << 24) | (p3 << 16) | (p4 << 8) | (p5 << 0);
}
// read rest of block
void Courier::BLOCK::deserialize(BLOCK&  value) {
	quint16 size = remaining();
	if (value.limit <= (value.pos + size)) {
		logger.error("Unexpected overflow  value.pos = %d  value.limit = %d  size = %d", value.pos, value.limit, size);
		RUNTIME_ERROR();
	}

	for(quint16 i = 0; i < size; i++) {
		value.data[value.pos++] = data[pos++];
	}
}

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
