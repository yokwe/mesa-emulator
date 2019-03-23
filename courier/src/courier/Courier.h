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


// Forward declaration
struct BYTE;
struct CARDINAL;
struct LONG_CARDINAL;
struct STRING;
struct UNSPECIFIED;
struct UNSPECIFIED2;
struct UNSPECIFIED3;


class BLOCK {
public:
	const quint16 capacity;

	BLOCK(quint16 capacity_) : capacity(capacity_), pos(0), limit(0), freeData(true), data(new quint8[capacity]) {}
	BLOCK(quint8* data_, quint16 capacity_) : capacity(capacity_), pos(0), limit(0), freeData(false), data(data_) {}
	~BLOCK() {
		if (freeData) delete[] data;
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
	void serialize8 (const quint8  value);
	void serialize16(const quint16 value);
	void serialize32(const quint32 value);
	void serialize48(const quint64 value);

	void serialize(const QString& value);
	void serialize(const BLOCK&   value); // write whole value. pos of value will not be change.

	// deserialize - read from block and write to value
	void deserialize8(quint8&   value);
	void deserialize16(quint16& value);
	void deserialize32(quint32& value);
	void deserialize48(quint64& value);

	void deserialize(QString& value);
	void deserialize(BLOCK&   value); // write whole value. pos of value will not be change.

private:
	quint16 pos;
	quint16 limit;

	const bool    freeData;
	quint8* data;
};


template <typename T>
struct RAW_TYPE {
protected:
	T value;

public:
	// Default constructor
	RAW_TYPE() {
		value = 0;
	}
	// BYTE a; BYTE b(a);
	RAW_TYPE(const T& rhs) {
		this->value = rhs.value;
	}
	// BYTE a(10);
	RAW_TYPE(const T value_) {
		value = value_;
	}
	virtual ~RAW_TYPE() {}

	// BYTE a, b; a = b;s
	RAW_TYPE& operator= (const RAW_TYPE& rhs) {
		this->value = rhs.value;
		return *this;
	}
	// BYTE a; a = 100;
	RAW_TYPE& operator= (const T rhs) {
		this->value = rhs;
		return *this;
	}
	// BYTE b; quint8 a = b;
	operator T() const {
		return value;
	}

	virtual void serialize  (BLOCK& block) = 0;
	virtual void deserialize(BLOCK& block) = 0;
};


struct BYTE          : public RAW_TYPE<quint8>  {
	void serialize(BLOCK& block) const {
		block.serialize8(value);
	}
	void deserialize(BLOCK& block) {
		block.deserialize8(value);
	}
};
struct CARDINAL      : public RAW_TYPE<quint16> {
	void serialize(BLOCK& block) const {
		block.serialize16(value);
	}
	void deserialize(BLOCK& block) {
		block.deserialize16(value);
	}
};
struct LONG_CARDINAL : public RAW_TYPE<quint32> {
	void serialize(BLOCK& block) const {
		block.serialize32(value);
	}
	void deserialize(BLOCK& block) {
		block.deserialize32(value);
	}
};
struct STRING        : public RAW_TYPE<QString> {
	void serialize(BLOCK& block) const {
		block.serialize(value);
	}
	void deserialize(BLOCK& block) {
		block.deserialize(value);
	}
};
struct UNSPECIFIED   : public RAW_TYPE<quint16> {
	void serialize(BLOCK& block) const {
		block.serialize16(value);
	}
	void deserialize(BLOCK& block) {
		block.deserialize16(value);
	}
};
struct UNSPECIFIED2  : public RAW_TYPE<quint32> {
	void serialize(BLOCK& block) const {
		block.serialize32(value);
	}
	void deserialize(BLOCK& block) {
		block.deserialize32(value);
	}
};
struct UNSPECIFIED3  : public RAW_TYPE<quint64> {
	void serialize(BLOCK& block) const {
		block.serialize48(value);
	}
	void deserialize(BLOCK& block) {
		block.deserialize48(value);
	}
};

// serialize - write value to block
void serialize(BLOCK& block, const BYTE&          value) {
	value.serialize(block);
}
void serialize(BLOCK& block, const CARDINAL&      value) {
	value.serialize(block);
}
void serialize(BLOCK& block, const LONG_CARDINAL& value) {
	value.serialize(block);
}
void serialize(BLOCK& block, const STRING&        value) {
	value.serialize(block);
}
void serialize(BLOCK& block, const UNSPECIFIED&   value) {
	value.serialize(block);
}
void serialize(BLOCK& block, const UNSPECIFIED2&  value) {
	value.serialize(block);
}
void serialize(BLOCK& block, const UNSPECIFIED3&  value) {
	value.serialize(block);
}
// write whole value
void serialize(BLOCK& block, const BLOCK&         value) {
	block.serialize(value);
}


// deserialize - read from block and write to value
void deserialize(BLOCK& block, BYTE&          value) {
	value.deserialize(block);
}
void deserialize(BLOCK& block, CARDINAL&      value) {
	value.deserialize(block);
}
void deserialize(BLOCK& block, LONG_CARDINAL& value) {
	value.deserialize(block);
}
void deserialize(BLOCK& block, STRING&        value) {
	value.deserialize(block);
}
void deserialize(BLOCK& block, UNSPECIFIED&   value) {
	value.deserialize(block);
}
void deserialize(BLOCK& block, UNSPECIFIED2&  value) {
	value.deserialize(block);
}
void deserialize(BLOCK& block, UNSPECIFIED3&  value) {
	value.deserialize(block);
}
// read rest of value
void deserialize(BLOCK& block, BLOCK&         value) {
	value.deserialize(block);
}


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

	void serialize(BLOCK& block) {
		block.serialize16(size);
		for(quint16 i = 0; i < size; i++) {
			block.serialize8(data[i]);
		}
	}
	void deserialize(BLOCK& block) {
		block.deserialize16(size);
		if (maxSize <= size) {
			logger.error("Unexpected overflow  size = %d  maxSize = %d", size, maxSize);
			RUNTIME_ERROR();
		}
		for(quint16 i = 0; i < size; i++) {
			block.deserialize8(data[i]);
		}
	}

private:
	quint16 size = 0;
	T* data;
};

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

	void serialize(BLOCK& block) {
		for(quint16 i = 0; i < maxSize; i++) {
			serialize(block, data[i]);
		}
	}
	void deserialize(BLOCK& block) {
		for(quint16 i = 0; i < maxSize; i++) {
			deserialize(block, data[i]);
		}
	}
private:
	quint16 size = 0;
	T* data;
};

}

#endif /* COURIER_COURIER_H_ */
