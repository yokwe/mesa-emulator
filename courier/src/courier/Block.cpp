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
// Block.cpp
//

#include "../util/Debug.h"
#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("cr/Block");

#include "../courier/Courier.h"
#include "../courier/Block.h"

QString Courier::Block::toString() const {
	QString hexString;
	for(quint16 i = offset; i < limit; i++) {
		quint8 c = data[i];
		hexString.append(QString("%1").arg((quint16)c, 2, 16, QChar('0')).toUpper());
	}
	return QString("(%1)%2").arg(limit).arg(hexString);
}

Courier::Block Courier::Block::remnant() {
	Block ret(*this);
	ret.offset = pos;
	return ret;
}

void Courier::Block::clear() {
	state = State::write;
	limit = offset;
	pos   = offset;
}

void Courier::Block::zero() {
	for(quint16 i = offset; i < capacity; i++) {
		data[i] = 0;
	}
}

void Courier::Block::rewind() {
	if (state == State::write) {
		state = State::read;
		pos   = offset;
	} else {
		logger.error("Unexpected state");
		COURIER_FATAL_ERROR();
	}
}
void Courier::Block::reset() {
	if (state == State::read) {
		pos = offset;
	} else {
		logger.error("Unexpected state");
		COURIER_FATAL_ERROR();
	}
}
quint16 Courier::Block::remaining() const {
	if (state == State::read) {
		return limit - pos;
	} else {
		logger.error("Unexpected state");
		COURIER_FATAL_ERROR();
	}
}
quint16 Courier::Block::getPos() const {
	return (quint16)(pos - offset);
}
quint16 Courier::Block::getLimit() const {
	return (quint16)(limit - offset);
}
quint16 Courier::Block::getCapacity() const {
	return (quint16)(capacity - offset);
}
quint16 Courier::Block::getOffset() const {
	return offset;
}
bool Courier::Block::equals(const Block& that) const {
	if (this->offset == that.offset && this->limit == that.limit) {
		for(quint16 i = offset; i < limit; i++) {
			if (this->data[i] != that.data[i]) return false;
		}
		return true;
	} else {
		return false;
	}
}

// serialize - write value to Block
void Courier::Block::serialize8(const quint8  value) {
	if (state == State::write) {
		const quint16 size = 1;
		if (capacity < (pos + size)) {
			logger.error("Unexpected overflow  capacity = %d  pos = %d  size = %d", capacity, pos, size);
			COURIER_FATAL_ERROR();
		}

		data[pos++] = value;
		// Update limit
		if (limit < pos) {
			limit = pos;
		}
	} else {
		logger.error("Unexpected state");
		COURIER_FATAL_ERROR();
	}
}
void Courier::Block::serialize16(const quint16 value) {
	if (state == State::write) {
		const quint16 size = 2;
		if (capacity < (pos + size)) {
			logger.error("Unexpected overflow  capacity = %d  pos = %d  size = %d", capacity, pos, size);
			COURIER_FATAL_ERROR();
		}

		quint8 p1 = (quint8)(value >> 0);
		quint8 p0 = (quint8)(value >> 8);

		data[pos++] = p0;
		data[pos++] = p1;
		// Update limit
		if (limit < pos) {
			limit = pos;
		}
	} else {
		logger.error("Unexpected state");
		COURIER_FATAL_ERROR();
	}
}
void Courier::Block::serialize16(const quint16 offset, const quint16 value) {
	const quint16 size = 2;
	if (limit < (offset + size)) {
		logger.error("Unexpected overflow  limit = %d  offset = %d  size = %d", limit, offset, size);
		COURIER_FATAL_ERROR();
	}
	quint16 savePos = pos;
	pos = offset;
	serialize16(value);
	pos = savePos;
}
void Courier::Block::serialize32(const quint32 value) {
	if (state == State::write) {
		const quint16 size = 4;
		if (capacity < (pos + size)) {
			logger.error("Unexpected overflow  capacity = %d  pos = %d  size = %d", capacity, pos, size);
			COURIER_FATAL_ERROR();
		}

		quint8 p3 = (quint8)(value >> 0);
		quint8 p2 = (quint8)(value >> 8);
		quint8 p1 = (quint8)(value >> 16);
		quint8 p0 = (quint8)(value >> 24);

		data[pos++] = p0;
		data[pos++] = p1;
		data[pos++] = p2;
		data[pos++] = p3;
		// Update limit
		if (limit < pos) {
			limit = pos;
		}
	} else {
		logger.error("Unexpected state");
		COURIER_FATAL_ERROR();
	}
}
void Courier::Block::serialize48(const quint64 value) {
	if (state == State::write) {
		const quint16 size = 6;
		if (capacity < (pos + size)) {
			logger.error("Unexpected overflow  capacity = %d  pos = %d  size = %d", capacity, pos, size);
			COURIER_FATAL_ERROR();
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
		// Update limit
		if (limit < pos) {
			limit = pos;
		}
	} else {
		logger.error("Unexpected state");
		COURIER_FATAL_ERROR();
	}
}


// deserialize - read from Block and write to value
void Courier::Block::deserialize8(quint8&  value) {
	if (state == State::read) {
		quint16 size = 1;
		if (limit < (pos + size)) {
			logger.error("Unexpected overflow  limit = %d  pos = %d  size = %d", limit, pos, size);
			COURIER_FATAL_ERROR();
		}
		value = data[pos++];
	} else {
		logger.error("Unexpected state");
		COURIER_FATAL_ERROR();
	}
}
void Courier::Block::deserialize16(quint16& value) {
	if (state == State::read) {
		quint16 size = 2;
		if (limit < (pos + size)) {
			logger.error("Unexpected overflow  limit = %d  pos = %d  size = %d", limit, pos, size);
			COURIER_FATAL_ERROR();
		}

		quint16 p0 = data[pos++];
		quint16 p1 = data[pos++];

		value = (p0 << 8) | (p1 << 0);
	} else {
		logger.error("Unexpected state");
		COURIER_FATAL_ERROR();
	}
}
void Courier::Block::deserialize32(quint32& value) {
	if (state == State::read) {
		quint16 size = 4;
		if (limit < (pos + size)) {
			logger.error("Unexpected overflow  limit = %d  pos = %d  size = %d", limit, pos, size);
			COURIER_FATAL_ERROR();
		}

		quint32 p0 = data[pos++];
		quint32 p1 = data[pos++];
		quint32 p2 = data[pos++];
		quint32 p3 = data[pos++];

		value = (p0 << 24) | (p1 << 16) | (p2 << 8) | (p3 << 0);
	} else {
		logger.error("Unexpected state");
		COURIER_FATAL_ERROR();
	}
}
// read 48 bit
void Courier::Block::deserialize48(quint64& value) {
	if (state == State::read) {
		quint16 size = 6;
		if (limit < (pos + size)) {
			logger.error("Unexpected overflow  pos = %d  limit = %d  size = %d", pos, limit, size);
			COURIER_FATAL_ERROR();
		}

		quint64 p0 = data[pos++];
		quint64 p1 = data[pos++];
		quint64 p2 = data[pos++];
		quint64 p3 = data[pos++];
		quint64 p4 = data[pos++];
		quint64 p5 = data[pos++];

		value = (p0 << 40) | (p1 << 32) | (p2 << 24) | (p3 << 16) | (p4 << 8) | (p5 << 0);
	} else {
		logger.error("Unexpected state");
		COURIER_FATAL_ERROR();
	}
}

void Courier::Block::serialize(const QString& value) {
	if (state == State::write) {
		int size = value.size();
		if (65535 <= size) {
			logger.error("Unexpected overflow  size = %ds", size);
			COURIER_FATAL_ERROR();
		}

		// size of string
		serialize16((quint16)size);

		quint8 c;
		for(int i = 0; i < size; i++) {
			c = (quint8)value.at(i).toLatin1();
			serialize8(c);
		}
		// add padding to make even size
		if (size & 1) {
			c = 0;
			serialize8(c);
		}
		// Update limit
		if (limit < pos) {
			limit = pos;
		}
	} else {
		logger.error("Unexpected state");
		COURIER_FATAL_ERROR();
	}
}

void Courier::Block::deserialize(QString& value) {
	if (state == State::read) {
		quint16 size;
		deserialize16(size);

		value.resize(0);

		quint8 c;
		for(quint16 i = 0; i < size; i++) {
			deserialize8(c);
			value.append(c);
		}
		// discard padding to make even size
		if (size & 1) {
			deserialize8(c);
		}
	} else {
		logger.error("Unexpected state");
		COURIER_FATAL_ERROR();
	}
}

// write whole value
void Courier::Block::serialize(const Block&  value) {
	if (state == State::write && value.state == State::read) {
		quint16 size = value.limit;
		if (capacity < (pos + size)) {
			logger.error("Unexpected overflow  capacity = %d  pos = %d  size = %d", capacity, pos, size);
			COURIER_FATAL_ERROR();
		}

		for(quint16 i = 0; i < size; i++) {
			data[pos++] = value.data[i];
		}
		// Update limit
		if (limit < pos) {
			limit = pos;
		}
	} else {
		logger.error("Unexpected state");
		COURIER_FATAL_ERROR();
	}
}

// read rest of block
void Courier::Block::deserialize(Block&  value) {
	if (state == State::read && value.state == State::write) {
		quint16 size = remaining();
		if (value.capacity < (value.pos + size)) {
			logger.error("Unexpected overflow  value.capacity = %d  value.limit = %d  value.pos = %d", value.capacity, value.limit, value.pos);
			logger.error("                           capacity = %d  limit = %d  pos = %d", capacity, limit, pos);
			COURIER_FATAL_ERROR();
		}

		for(quint16 i = 0; i < size; i++) {
			value.data[value.pos++] = data[pos++];
		}
		// Update limit
		if (value.limit < value.pos) {
			value.limit = value.pos;
		}
	} else {
		logger.error("Unexpected state");
		COURIER_FATAL_ERROR();
	}
}
