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
// ByteBuffer.h
//

#ifndef BYTEBUFFER_H__
#define BYTEBUFFER_H__

#include "../util/Util.h"
#include "../mesa/Pilot.h"

class ByteBuffer {
protected:
	static log4cpp::Category& logger;

	quint8*       data;
	const quint32 capacity; // data capacity [0..limit)
	quint32       limit;    // valid data range [0..limit)
	quint32       pos;      // cursor position

	const bool    deleteData;

	ByteBuffer(quint8* data_, quint32 dataSize_) : data(data_), capacity(dataSize_), limit(capacity), pos(0), deleteData(false) {}
	ByteBuffer(quint32 size) : data(new quint8[size]), capacity(size), limit(capacity), pos(0), deleteData(true) {}
	virtual ~ByteBuffer() {
		if (deleteData) delete[] data;
	}

	virtual quint32 getOffset(quint32 offset) = 0;

public:
	// Absolute get and put
	quint32 get32(quint32 offset) {
		quint32 ret = get16(offset + 2);
		ret <<= 16;
		ret |= get16(offset + 0);
		return ret;
	}
	quint16 get16(quint32 offset) {
		quint16 ret = get8(offset + 0);
		ret <<= 8;
		ret |= get8(offset + 1);
		return ret;
	}
	quint8  get8 (quint32 offset_) {
		const quint32 offset = getOffset(offset_);
		if (capacity <= offset) {
			logger.fatal("%s  capacity = %d  offset_ = %d  offset = %d", __FUNCTION__, capacity, offset_, offset);
			ERROR();
		}
		return data[offset];
	}

	void    set32(quint32 offset, quint32 value) {
		set16(offset + 0, (quint16)value);
		set16(offset + 2, (quint16)(value >> 16));
	}
	void    set16(quint32 offset, quint16 value) {
		set8(offset + 0, (quint8)(value >> 8));
		set8(offset + 1, (quint8)(value >> 0));
	}
	void    set8 (quint32 offset_, quint8  value) {
		const quint32 offset = getOffset(offset_);
		if (capacity <= offset) {
			logger.fatal("%s  limit = %d  offset_ = %d  offset = %d", __FUNCTION__, limit, offset_, offset);
			ERROR();
		}
		data[offset] = value;
	}

	// cursor position
	quint32 getPos() {
		return pos;
	}
	// set cursor position. newValue must be [0..limit)
	void setPos(quint32 newValue) {
		if (limit < newValue) {
			logger.fatal("%s  limit = %d  newValue = %d", __FUNCTION__, limit, newValue);
			ERROR();
		}
		pos = newValue;
	}
	void skipByte(quint32 bytes) {
		setPos(pos + bytes);
	}
	void skipWord(quint32 bytes) {
		setPos(pos + (bytes * Environment::bytesPerWord));
	}

	// limit
	quint32 getLimit() {
		return limit;
	}
	// set buffer limit. newValue must be [0..capacity)
	void setLimit(quint32 newValue) {
		if (capacity < newValue) {
			logger.fatal("%s  capacity = %d  newValue = %d", __FUNCTION__, capacity, newValue);
			ERROR();
		}
		limit = newValue;
	}

	quint32 getCapacity() {
		return capacity;
	}

	// reset buffer for fresh write
	void clear() {
		limit = capacity;
		pos   = 0;
	}
	// reset buffer for read written content
	void rewind() {
		limit = pos;
		pos   = 0;
	}
	// remaining
	quint32 remaining() {
		return limit - pos;
	}
	// data
	quint8* getData() {
		return data;
	}

	// get and put from cursor position and position is advanced after get/put
	quint32 get32() {
		const quint32 ret = get32(pos);
		pos += sizeof(ret);
		return ret;
	}
	quint16 get16() {
		const quint16 ret = get16(pos);
		pos += sizeof(ret);
		return ret;
	}
	quint8  get8() {
		return get8(pos++);
	}
	void    put32(quint32 value) {
		set32(pos, value);
		pos += sizeof(value);
		if (limit < pos) limit = pos;
	}
	void    put16(quint16 value) {
		set16(pos, value);
		pos += sizeof(value);
		if (limit < pos) limit = pos;
	}
	void    put8 (quint8  value) {
		set8(pos, value);
		pos += sizeof(value);
		if (limit < pos) limit = pos;
	}
	void    putAll(const quint8* data, qint32 dataSize) {
		for(qint32 i = 0; i < dataSize; i++) put8(data[i]);
	}
};

class BigEndianByteBuffer final : public ByteBuffer {
private:
	quint32 getOffset(quint32 offset) {
		return offset;
	};

public:
	BigEndianByteBuffer(quint32 size) : ByteBuffer(size) {}
	BigEndianByteBuffer(quint8* data_, quint32 dataSize_) : ByteBuffer(data_, dataSize_) {}
};

class LittleEndianByteBuffer final : public ByteBuffer {
private:
	quint32 getOffset(quint32 offset) {
		return offset ^ 1;
	};

public:
	LittleEndianByteBuffer(quint32 size) : ByteBuffer(size) {}
	LittleEndianByteBuffer(quint8* data_, quint32 dataSize_) : ByteBuffer(data_, dataSize_) {}
};
#endif
