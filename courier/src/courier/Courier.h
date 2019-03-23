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
// Courier.h
//

#ifndef COURIER_COURIER_H_
#define COURIER_COURIER_H_

#include <QtCore>

#include "../util/NetData.h"

namespace Courier {

class BLOCK {
public:
	const quint16 capacity;

	BLOCK(quint16 capacity_) : capacity(capacity_), pos(0), limit(0) {
		data = new quint16[capacity];
	}
	~BLOCK() {
		delete[] data;
	}

	// reset buffer for fresh write
	void clear() {
		limit = capacity;
		pos   = 0;
	}
	// fill data[0..capacity) with zero
	void zero();
	// reset buffer for read written content
	void rewind() {
		limit = pos;
		pos   = 0;
	}
	// reset position for read
	void reset() {
		pos = 0;
	}
	// remaining
	quint16 remaining() const {
		return limit - pos;
	}

	// serialize - write value to block
	void serialize  (const quint8  value);
	void serialize  (const quint16 value);
	void serialize  (const quint32 value);
	void serialize  (const quint64 value); // write 48 bit
	void serialize  (const BLOCK&  value); // write whole value. pos of value will not be change.

	// deserialize - read from block and write to value
	void deserialize(quint8&  value);
	void deserialize(quint16& value);
	void deserialize(quint32& value);
	void deserialize(quint64& value); // read 48 bit
	void deserialize(BLOCK&   value); // read rest of block. pos of value will be change.

private:
	quint16 pos;
	quint16 limit;

	quint16* data;
};

// serialize - write value to block
void serialize  (BLOCK& block, const quint8  value) {
	block.serialize(value);
}
void serialize  (BLOCK& block, const quint16 value) {
	block.serialize(value);
}
void serialize  (BLOCK& block, const quint32 value) {
	block.serialize(value);
}
// write 48 bit of value
void serialize  (BLOCK& block, const quint64 value) {
	block.serialize(value);
}
// write whole value
void serialize  (BLOCK& block, const BLOCK&  value) {
	block.serialize(value);
}

// deserialize - read from block and write to value
void deserialize(BLOCK& block, quint8  value) {
	block.deserialize(value);
}
void deserialize(BLOCK& block, quint16 value) {
	block.deserialize(value);
}
void deserialize(BLOCK& block, quint32 value) {
	block.deserialize(value);
}
// read 48 bit of block
void deserialize(BLOCK& block, quint64 value) {
	block.deserialize(value);
}
// read rest of block
void deserialize(BLOCK& block, BLOCK&  value) {
	block.deserialize(value);
}


// Use NetData as Block
using Block = NetData;

// define predefined Courier type
using BOOLEAN       = quint16;
using BYTE          = quint8;
using CARDINAL      = quint16;
using LONG_CARDINAL = quint32;
using STRING        = QString;
using UNSPECIFIED   = quint16;
using UNSPECIFIED2  = quint32;
using UNSPECIFIED3  = quint64;

void serialize  (Block& block, const quint8& value) {
	block.put8(value);
}
void deserialize(Block& block, quint8& value) {
	value = block.get8();
}

void serialize  (Block& block, const quint16& value) {
	block.put16(value);
}
void deserialize(Block& block, quint16& value) {
	value = block.get16();
}

void serialize  (Block& block, const quint32& value) {
	block.put32(value);
}
void deserialize(Block& block, quint32& value) {
	value = block.get32();
}

void serialize  (Block& block, const quint64& value) {
	block.put48(value);
}
void deserialize(Block& block, quint64& value) {
	value = block.get48();
}

void serialize  (Block& block, const QString& value);
void deserialize(Block& block, QString& value);

template <typename T>
struct SEQUENCE {
public:
	const quint16 maxSize;
	SEQUENCE(quint16 maxSize_ = 65535) : maxSize(maxSize_) {
		data = new T [maxSize];
	}
	~SEQUENCE() {
		delete[] data;
	}

	quint16 getSize() {
		return size;
	}
	void clear() {
		size = 0;
	}

	void append(const T& newValue) {
		if (maxSize <= size) {
			logger.error("Unexpected overflow  size = %d  maxSize = %d", size, maxSize);
			RUNTIME_ERROR();
		}
		data[size++] = newValue;
	}
	T& operator[](quint16 i) {
		if (size <= i) {
			logger.error("Unexpected overflow  i = %d  size = %d", i, size);
			RUNTIME_ERROR();
		}
		return data[i];
	}
	const T& operator[](quint16 i) const {
		if (size <= i) {
			logger.error("Unexpected overflow  i = %d  size = %d", i, size);
			RUNTIME_ERROR();
		}
		return data[i];
	}

	void serialize(Block& block) {
		Courier::serialize(block, size);
		for(quint16 i = 0; i < size; i++) {
			Courier::serialize(block, data[i]);
		}
	}
	void deserialize(Block& block) {
		Courier::deserialize(block, size);
		if (maxSize <= size) {
			logger.error("Unexpected overflow  size = %d  maxSize = %d", size, maxSize);
			RUNTIME_ERROR();
		}
		for(quint16 i = 0; i < size; i++) {
			Courier::deserialize(block, data[i]);
		}
	}

private:
	quint16 size = 0;
	T* data;
};
template <typename T>
void deserialize (Block& block, SEQUENCE<T>& value) {
	value.deserialize(block);
}
template <typename T>
void serialize   (Block& block, SEQUENCE<T>& value) {
	value.serialize(block);
}

template <typename T>
struct ARRAY {
	const quint16 maxSize;

	ARRAY(quint16 maxSize_) : maxSize(maxSize_) {
		data = new T [maxSize];
	}
	~ARRAY() {
		delete[] data;
	}

	void append(const T& newValue) {
		if (maxSize <= size) {
			logger.error("Unexpected overflow  size = %d  maxSize = %d", size, maxSize);
			RUNTIME_ERROR();
		}
		data[size++] = newValue;
	}
	quint16 getSize() {
		return size;
	}
	void clear() {
		size = 0;
	}

	T& operator[](quint16 i) {
		if (maxSize <= i) {
			logger.error("Unexpected overflow  size = %d  maxSize = %d", size, maxSize);
			RUNTIME_ERROR();
		}
		return data[i];
	}
	const T& operator[](quint16 i) const {
		if (maxSize <= i) {
			logger.error("Unexpected overflow  size = %d  maxSize = %d", size, maxSize);
			RUNTIME_ERROR();
		}
		return data[i];
	}

	void serialize(Block& block) {
		for(quint16 i = 0; i < maxSize; i++) {
			serialize(block, data[i]);
		}
	}
	void deserialize(Block& block) {
		for(quint16 i = 0; i < maxSize; i++) {
			deserialize(block, data[i]);
		}
	}
private:
	quint16 size = 0;
	T* data;
};
template <typename T>
void deserialize (Block& block, ARRAY<T>& value) {
	value.deserialize(block);
}
template <typename T>
void serialize   (Block& block, ARRAY<T>& value) {
	value.serialize(block);
}

}

#endif /* COURIER_COURIER_H_ */
