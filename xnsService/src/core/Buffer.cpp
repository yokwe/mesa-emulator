/*
Copyright (c) 2014, Yasuhiro Hasegawa
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
// Buffer.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("buffer");

#include "../util/Debug.h"

#include "Buffer.h"

Buffer::Buffer(quint8* data_, quint32 size_) : capacity(MAX_SIZE) {
	if (capacity < size_) ERROR();
	limit    = size_;
	pos      = 0;
	// copy data
	data     = rawData;
	for(quint32 i = 0; i < limit; i++) rawData[i] = data_[i];
}

void Buffer::setPos(quint32 newPos) {
	if (limit < newPos) ERROR();
	pos = newPos;
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

quint64 Buffer::get48(quint32 offset) {
	if (limit < (offset + SIZE_48)) {
		logger.fatal("%s  limit = %d  offset = %d  SIZE = %d", __FUNCTION__, limit, offset, SIZE_48);
		ERROR();
	}
	return get48_(data + offset);
}
quint32 Buffer::get32(quint32 offset) {
	if (limit < (offset + SIZE_32)) {
		logger.fatal("%s  limit = %d  offset = %d  SIZE = %d", __FUNCTION__, limit, offset, SIZE_32);
		ERROR();
	}
	return get32_(data + offset);
}
quint16 Buffer::get16(quint32 offset) {
	if (limit < (offset + SIZE_16)) {
		logger.fatal("%s  limit = %d  offset = %d  SIZE = %d", __FUNCTION__, limit, offset, SIZE_16);
		ERROR();
	}
	return get16_(data + offset);
}
quint8 Buffer::get8(quint32 offset) {
	if (limit < (offset + SIZE_8)) {
		logger.fatal("%s  limit = %d  offset = %d  SIZE = %d", __FUNCTION__, limit, offset, SIZE_8);
		ERROR();
	}
	return get8_(data + offset);
}

quint64 Buffer::get48() {
	if (limit < (pos + SIZE_48)) {
		logger.fatal("%s  limit = %d  pos = %d  SIZE = %d", __FUNCTION__, limit, pos, SIZE_48);
		ERROR();
	}
	quint64 ret = get48_(data + pos);
	pos += SIZE_48;
	return ret;
}
quint32 Buffer::get32() {
	if (limit < (pos + SIZE_32)) {
		logger.fatal("%s  limit = %d  pos = %d  SIZE = %d", __FUNCTION__, limit, pos, SIZE_32);
		ERROR();
	}
	quint64 ret = get32_(data + pos);
	pos += SIZE_32;
	return ret;
}
quint16 Buffer::get16() {
	if (limit < (pos + SIZE_16)) {
		logger.fatal("%s  limit = %d  pos = %d  SIZE = %d", __FUNCTION__, limit, pos, SIZE_16);
		ERROR();
	}
	quint64 ret = get16_(data + pos);
	pos += SIZE_16;
	return ret;
}
quint8 Buffer::get8() {
	if (limit < (pos + SIZE_8)) {
		logger.fatal("%s  limit = %d  pos = %d  SIZE = %d", __FUNCTION__, limit, pos, SIZE_8);
		ERROR();
	}
	quint64 ret = get8_(data + pos);
	pos += SIZE_8;
	return ret;
}



void Buffer::put48(quint32 offset, quint64 value) {
	if (capacity < (offset + SIZE_48)) {
		logger.fatal("%s  capacity = %d  offset = %d  SIZE = %d", __FUNCTION__, capacity, offset, SIZE_48);
		ERROR();
	}
	put48_(data + offset, value);
}
void Buffer::put32(quint32 offset, quint32 value) {
	if (capacity < (offset + SIZE_32)) {
		logger.fatal("%s  capacity = %d  offset = %d  SIZE = %d", __FUNCTION__, capacity, offset, SIZE_32);
		ERROR();
	}
	put32_(data + offset, value);
}
void Buffer::put16(quint32 offset, quint16 value) {
	if (capacity < (offset + SIZE_16)) {
		logger.fatal("%s  capacity = %d  offset = %d  SIZE = %d", __FUNCTION__, capacity, offset, SIZE_16);
		ERROR();
	}
	put32_(data + offset, value);
}
void Buffer::put8(quint32 offset, quint8 value) {
	if (capacity < (offset + SIZE_8)) {
		logger.fatal("%s  capacity = %d  offset = %d  SIZE = %d", __FUNCTION__, capacity, offset, SIZE_8);
		ERROR();
	}
	put8_(data + offset, value);
}

void Buffer::put48(quint64 value) {
	if (capacity < (pos + SIZE_48)) {
		logger.fatal("%s  capacity = %d  pos = %d  SIZE = %d", __FUNCTION__, capacity, pos, SIZE_48);
		ERROR();
	}
	put48_(data + pos, value);
	pos += SIZE_48;
	if (limit < pos) limit = pos;
}
void Buffer::put32(quint32 value) {
	if (capacity < (pos + SIZE_32)) {
		logger.fatal("%s  capacity = %d  pos = %d  SIZE = %d", __FUNCTION__, capacity, pos, SIZE_32);
		ERROR();
	}
	put32_(data + pos, value);
	pos += SIZE_32;
	if (limit < pos) limit = pos;
}
void Buffer::put16(quint16 value) {
	if (capacity < (pos + SIZE_16)) {
		logger.fatal("%s  capacity = %d  pos = %d  SIZE = %d", __FUNCTION__, capacity, pos, SIZE_16);
		ERROR();
	}
	put16_(data + pos, value);
	pos += SIZE_16;
	if (limit < pos) limit = pos;
}
void Buffer::put8(quint8 value) {
	if (capacity < (pos + SIZE_8)) {
		logger.fatal("%s  capacity = %d  pos = %d  SIZE = %d", __FUNCTION__, capacity, pos, SIZE_8);
		ERROR();
	}
	put8_(data + pos, value);
	pos += SIZE_8;
	if (limit < pos) limit = pos;
}
