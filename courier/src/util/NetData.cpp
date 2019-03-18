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
// NetData.cpp
//

#include "../util/Debug.h"
#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("netdata");

#include "../util/NetData.h"

static const quint32 SIZE_48 = 6;
static const quint32 SIZE_32 = 4;
static const quint32 SIZE_16 = 2;
static const quint32 SIZE_8  = 1;

void NetData::checkConsistency() {
	// capacity
	// limit
	if (limit <= capacity) {
		// OK
	} else {
		logger.fatal("pos = %d  limit = %d capacity = %d", pos, limit, capacity);
		RUNTIME_ERROR()
	}
	// pos
	if (pos <= limit) {
		// OK
	} else {
		logger.fatal("pos = %d  limit = %d capacity = %d", pos, limit, capacity);
		RUNTIME_ERROR()
	}
}

void NetData::setPos(quint32 newValue) {
	if (newValue <= limit) {
		pos = newValue;
		checkConsistency();
	} else {
		logger.fatal("setPos  limit = %d  newValue = %d", limit, newValue);
		RUNTIME_ERROR()
	}
}

void NetData::setLimit(quint32 newValue) {
	if (newValue <= capacity) {
		limit = newValue;
		checkConsistency();
	} else {
		logger.fatal("setLimit  capacity = %d  newValue = %d", capacity, newValue);
		RUNTIME_ERROR()
	}
}

void NetData::zero() {
	::bzero(data, capacity);
}
static inline quint64 get48_(quint8* p) {
	quint64 ret = p[0];
	ret <<= 8;
	ret |= p[1];
	ret <<= 8;
	ret |= p[2];
	ret <<= 8;
	ret |= p[3];
	ret <<= 8;
	ret |= p[4];
	ret <<= 8;
	ret |= p[5];
	return ret;
}
static inline quint32 get32_(quint8* p) {
	quint32 ret = p[0];
	ret <<= 8;
	ret |= p[1];
	ret <<= 8;
	ret |= p[2];
	ret <<= 8;
	ret |= p[3];
	return ret;
}
static inline quint16 get16_(quint8* p) {
	quint16 ret = p[0];
	ret <<= 8;
	ret |= p[1];
	return ret;
}
static inline quint8 get8_(quint8* p) {
	return p[0];
}

static inline void put48_(quint8* p, quint64 value) {
	p[5] = (quint8)value;
	value >>= 8;
	p[4] = (quint8)value;
	value >>= 8;
	p[3] = (quint8)value;
	value >>= 8;
	p[2] = (quint8)value;
	value >>= 8;
	p[1] = (quint8)value;
	value >>= 8;
	p[0] = (quint8)value;
}
static inline void put32_(quint8* p, quint32 value) {
	p[3] = (quint8)value;
	value >>= 8;
	p[2] = (quint8)value;
	value >>= 8;
	p[1] = (quint8)value;
	value >>= 8;
	p[0] = (quint8)value;
}
static inline void put16_(quint8* p, quint16 value) {
	p[1] = (quint8)value;
	value >>= 8;
	p[0] = (quint8)value;
}
static inline void put8_(quint8* p, quint8 value) {
	p[0] = value;
}

quint64 NetData::get48(quint32 at) {
	if (limit < (at + SIZE_48)) {
		logger.fatal("%s  limit = %d  at = %d  SIZE = %d", __FUNCTION__, limit, at, SIZE_48);
		RUNTIME_ERROR();
	}
	return get48_(data + at);
}
quint32 NetData::get32(quint32 at) {
	if (limit < (at + SIZE_32)) {
		logger.fatal("%s  limit = %d  at = %d  SIZE = %d", __FUNCTION__, limit, at, SIZE_32);
		RUNTIME_ERROR();
	}
	return get32_(data + at);
}
quint16 NetData::get16(quint32 at) {
	if (limit < (at + SIZE_16)) {
		logger.fatal("%s  limit = %d  at = %d  SIZE = %d", __FUNCTION__, limit, at, SIZE_16);
		RUNTIME_ERROR();
	}
	return get16_(data + at);
}
quint8 NetData::get8(quint32 at) {
	if (limit < (at + SIZE_8)) {
		logger.fatal("%s  limit = %d  at = %d  SIZE = %d", __FUNCTION__, limit, at, SIZE_8);
		RUNTIME_ERROR();
	}
	return get8_(data + at);
}

quint64 NetData::get48() {
	if (limit < (pos + SIZE_48)) {
		logger.fatal("%s  limit = %d  pos = %d  SIZE = %d", __FUNCTION__, limit, pos, SIZE_48);
		RUNTIME_ERROR();
	}
	quint64 ret = get48_(data + pos);
	pos += SIZE_48;
	return ret;
}
quint32 NetData::get32() {
	if (limit < (pos + SIZE_32)) {
		logger.fatal("%s  limit = %d  pos = %d  SIZE = %d", __FUNCTION__, limit, pos, SIZE_32);
		RUNTIME_ERROR();
	}
	quint64 ret = get32_(data + pos);
	pos += SIZE_32;
	return ret;
}
quint16 NetData::get16() {
	if (limit < (pos + SIZE_16)) {
		logger.fatal("%s  limit = %d  pos = %d  SIZE = %d", __FUNCTION__, limit, pos, SIZE_16);
		RUNTIME_ERROR();
	}
	quint64 ret = get16_(data + pos);
	pos += SIZE_16;
	return ret;
}
quint8 NetData::get8() {
	if (limit < (pos + SIZE_8)) {
		logger.fatal("%s  limit = %d  pos = %d  SIZE = %d", __FUNCTION__, limit, pos, SIZE_8);
		RUNTIME_ERROR();
	}
	quint64 ret = get8_(data + pos);
	pos += SIZE_8;
	return ret;
}



void NetData::set48(quint32 at, quint64 value) {
	if (capacity < (at + SIZE_48)) {
		logger.fatal("%s  capacity = %d  at = %d  SIZE = %d", __FUNCTION__, capacity, at, SIZE_48);
		RUNTIME_ERROR();
	}
	put48_(data + at, value);
}
void NetData::set32(quint32 at, quint32 value) {
	if (capacity < (at + SIZE_32)) {
		logger.fatal("%s  capacity = %d  at = %d  SIZE = %d", __FUNCTION__, capacity, at, SIZE_32);
		RUNTIME_ERROR();
	}
	put32_(data + at, value);
}
void NetData::set16(quint32 at, quint16 value) {
	if (capacity < (at + SIZE_16)) {
		logger.fatal("%s  capacity = %d  at = %d  SIZE = %d", __FUNCTION__, capacity, at, SIZE_16);
		RUNTIME_ERROR();
	}
	put32_(data + at, value);
}
void NetData::set8(quint32 at, quint8 value) {
	if (capacity < (at + SIZE_8)) {
		logger.fatal("%s  capacity = %d  at = %d  SIZE = %d", __FUNCTION__, capacity, at, SIZE_8);
		RUNTIME_ERROR();
	}
	put8_(data + at, value);
}

void NetData::put48(quint64 value) {
	if (capacity < (pos + SIZE_48)) {
		logger.fatal("%s  capacity = %d  pos = %d  SIZE = %d", __FUNCTION__, capacity, pos, SIZE_48);
		RUNTIME_ERROR();
	}
	put48_(data + pos, value);
	pos += SIZE_48;
	if (limit < pos) limit = pos;
}
void NetData::put32(quint32 value) {
	if (capacity < (pos + SIZE_32)) {
		logger.fatal("%s  capacity = %d  pos = %d  SIZE = %d", __FUNCTION__, capacity, pos, SIZE_32);
		RUNTIME_ERROR();
	}
	put32_(data + pos, value);
	pos += SIZE_32;
	if (limit < pos) limit = pos;
}
void NetData::put16(quint16 value) {
	if (capacity < (pos + SIZE_16)) {
		logger.fatal("%s  capacity = %d  pos = %d  SIZE = %d", __FUNCTION__, capacity, pos, SIZE_16);
		RUNTIME_ERROR();
	}
	put16_(data + pos, value);
	pos += SIZE_16;
	if (limit < pos) limit = pos;
}
void NetData::put8(quint8 value) {
	if (capacity < (pos + SIZE_8)) {
		logger.fatal("%s  capacity = %d  pos = %d  SIZE = %d", __FUNCTION__, capacity, pos, SIZE_8);
		RUNTIME_ERROR();
	}
	put8_(data + pos, value);
	pos += SIZE_8;
	if (limit < pos) limit = pos;
}

void NetData::put(const NetData& that, quint32 from) {
	quint32 limit = that.getLimit();
	if (limit < from) {
		logger.fatal("%s  from = %d  limit = %d", __FUNCTION__, from, limit);
		RUNTIME_ERROR();
	}
	for(quint32 i = from; i < limit; i++) {
		put8(that.data[i]);
	}
}

QString toString(const NetData value) {
	quint8* data = value.getData();
	quint32 limit = value.getLimit();

	QString ret;
	ret.append(QString("(%1)").arg(limit));
	for(quint32 i = 0; i < limit; i++) {
		ret.append(QString("%1").arg(data[i], 2, 16, QChar('0')));
	}

	return ret;
}

