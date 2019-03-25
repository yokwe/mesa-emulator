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

#define COURIER_ERROR() { logger.fatal("ERROR %s %d %s", __FILE__, __LINE__, __FUNCTION__); throw Courier::CourierError(__FILE__, __LINE__, __FUNCTION__); }

namespace Courier {

class CourierError {
public:
	const char *file;
	const int   line;
	const char *func;

	CourierError(const char *file_, const int line_, const char *func_) : file(file_), line(line_), func(func_) {}
};


class BLOCK {
public:
	const quint16 capacity;

	BLOCK(quint16 capacity_) : capacity(capacity_), state(State::write), pos(0), limit(0), freeData(true), data(new quint8[capacity]) {
		zero();
	}
	BLOCK(quint8* data_, quint16 capacity_) : capacity(capacity_), state(State::write), pos(0), limit(0), freeData(false), data(data_) {
		zero();
	}
	~BLOCK() {
		if (freeData) delete[] data;
	}

	QString toString() const;

	// Set pos with 0 for fresh write
	void clear() {
		state = State::write;
		limit = 0;
		pos   = 0;
	}
	// Fill data[0..capacity) with zero
	void zero();
	// Set pos with 0 for read after serialization
	void rewind();
	// Set pos with 0 for read after deserialization
	void reset();
	// Return remaining byte for read
	quint16 remaining() const;
	quint16 getPos() const {
		return pos;
	}
	quint16 getLimit() const {
		return limit;
	}
	bool equals(const BLOCK& that) const {
		if (this->limit == that.limit) {
			for(quint16 i = 0; i < limit; i++) {
				if (this->data[i] != that.data[i]) return false;
			}
			return true;
		} else {
			return false;
		}
	}

	// serialize - write value to block
	void serialize8 (const quint8  value);
	void serialize16(const quint16 value);
	// for update of checksum field
	void serialize16(const quint16 offset, const quint16 value);
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
	enum class State {read, write};
	State   state;
	quint16 pos;
	quint16 limit;

	const bool    freeData;
	quint8* const data;
};


struct Serializable {
	virtual ~Serializable() {}

	// read this and write to block
	virtual void serialize  (BLOCK& block) const = 0;
	// read block and write to this
	virtual void deserialize(BLOCK& block) = 0;
};


struct BYTE : public Serializable {
private:
	static const int MAX_VALUE = 0xFF;
	quint8 value;

public:
	BYTE() {
		value = 0;
	}

	BYTE(const BYTE& rhs) {
		this->value = rhs.value;
	}
	// BYTE a(10);
	BYTE(const int value) {
		if (0 <= value && value <= MAX_VALUE) {
			this->value = (quint8)value;
		} else {
			logger.error("Overflow  value = %d  MAX_VALUE = %d", value, MAX_VALUE);
			COURIER_ERROR();
		}
	}

	// BYTE a, b; a = b;s
	BYTE& operator= (const BYTE& rhs) {
		this->value = rhs.value;
		return *this;
	}
	// BYTE a; a = 100;
	BYTE& operator= (const int value) {
		if (0 <= value && value <= MAX_VALUE) {
			this->value = (quint8)value;
		} else {
			logger.error("Overflow  value = %d  MAX_VALUE = %d", value, MAX_VALUE);
			COURIER_ERROR();
		}
		return *this;
	}
	// BYTE b; quint8 a = b;
	operator quint8() const {
		return value;
	}
	bool equals(const BYTE& that) const {
		return value == that.value;
	}

	void serialize(BLOCK& block) const {
		block.serialize8(value);
	}
	void deserialize(BLOCK& block) {
		block.deserialize8(value);
	}
};
struct CARDINAL : public Serializable {
private:
	static const int MAX_VALUE = 0xFFFF;
	quint16 value;

public:
	CARDINAL() {
		value = 0;
		logger.info("CARDINAL::CARDINAL()");
	}
	~CARDINAL() {
		logger.info("CARDINAL::~CARDINAL()");
	}

	CARDINAL(const CARDINAL& rhs) {
		this->value = rhs.value;
	}
	// BYTE a(10);
	CARDINAL(const int value) {
		if (0 <= value && value <= MAX_VALUE) {
			this->value = (quint16)value;
		} else {
			logger.error("Overflow  value = %d  MAX_VALUE = %d", value, MAX_VALUE);
			COURIER_ERROR();
		}
	}

	// BYTE a, b; a = b;s
	CARDINAL& operator= (const CARDINAL& rhs) {
		this->value = rhs.value;
		return *this;
	}
	// BYTE a; a = 100;
	CARDINAL& operator= (const int value) {
		if (0 <= value && value <= MAX_VALUE) {
			this->value = (quint16)value;
		} else {
			logger.error("Overflow  value = %d  MAX_VALUE = %d", value, MAX_VALUE);
			COURIER_ERROR();
		}
		return *this;
	}
	// BYTE b; quint8 a = b;
	operator quint16() const {
		return value;
	}
	bool equals(const CARDINAL& that) const {
		return value == that.value;
	}

	void serialize(BLOCK& block) const {
		block.serialize16(value);
	}
	void deserialize(BLOCK& block) {
		block.deserialize16(value);
	}
};
struct LONG_CARDINAL : public Serializable {
private:
	static const long long MAX_VALUE = 0xFFFFFFFFLL;
	quint32 value;

public:
	LONG_CARDINAL() {
		value = 0;
	}

	LONG_CARDINAL(const LONG_CARDINAL& rhs) {
		this->value = rhs.value;
	}
	// BYTE a(10);
	LONG_CARDINAL(const long long value) {
		if (0 <= value && value <= MAX_VALUE) {
			this->value = (quint32)value;
		} else {
			logger.error("Overflow  value = %d  MAX_VALUE = %d", value, MAX_VALUE);
			COURIER_ERROR();
		}
	}

	// BYTE a, b; a = b;s
	LONG_CARDINAL& operator= (const LONG_CARDINAL& rhs) {
		this->value = rhs.value;
		return *this;
	}
	// BYTE a; a = 100;
	LONG_CARDINAL& operator= (const long long value) {
		if (0 <= value && value <= MAX_VALUE) {
			this->value = (quint32)value;
		} else {
			logger.error("Overflow  value = %d  MAX_VALUE = %d", value, MAX_VALUE);
			COURIER_ERROR();
		}
		return *this;
	}
	// BYTE b; quint8 a = b;
	operator quint32() const {
		return value;
	}
	bool equals(const LONG_CARDINAL& that) const {
		return value == that.value;
	}

	void serialize(BLOCK& block) const {
		block.serialize32(value);
	}
	void deserialize(BLOCK& block) {
		block.deserialize32(value);
	}
};
struct STRING : public Serializable {
private:
	static const int MAX_SIZE = 65535;
	QString value;

public:
	STRING() {
	}

	STRING(const STRING& rhs) {
		this->value = rhs.value;
	}
	// BYTE a(10);
	STRING(const QString& value) {
		int size = value.size();
		if (size <= MAX_SIZE) {
			this->value = value;
		} else {
			logger.error("Overflow  size = %d  MAX_SIZE = %d", size, MAX_SIZE);
			COURIER_ERROR();
		}
	}

	// BYTE a, b; a = b;s
	STRING& operator= (const STRING& rhs) {
		this->value = rhs.value;
		return *this;
	}
	// BYTE a; a = 100;
	STRING& operator= (const QString& value) {
		int size = value.size();
		if (size <= MAX_SIZE) {
			this->value = value;
		} else {
			logger.error("Overflow  size = %d  MAX_SIZE = %d", size, MAX_SIZE);
			COURIER_ERROR();
		}
		return *this;
	}
	// BYTE b; quint8 a = b;
	operator QString() const {
		return value;
	}
	bool equals(const STRING& that) const {
		return value.compare(that.value) == 0;
	}

	void serialize(BLOCK& block) const {
		block.serialize(value);
	}
	void deserialize(BLOCK& block) {
		block.deserialize(value);
	}
};
struct UNSPECIFIED : public Serializable {
private:
	static const int MAX_VALUE = 0xFFFF;
	quint16 value;

public:
	UNSPECIFIED() {
		value = 0;
	}

	UNSPECIFIED(const UNSPECIFIED& rhs) {
		this->value = rhs.value;
	}
	// BYTE a(10);
	UNSPECIFIED(const int value) {
		if (0 <= value && value <= MAX_VALUE) {
			this->value = (quint16)value;
		} else {
			logger.error("Overflow  value = %d  MAX_VALUE = %d", value, MAX_VALUE);
			COURIER_ERROR();
		}
	}

	// BYTE a, b; a = b;s
	UNSPECIFIED& operator= (const UNSPECIFIED& rhs) {
		this->value = rhs.value;
		return *this;
	}
	// BYTE a; a = 100;
	UNSPECIFIED& operator= (const int value) {
		if (0 <= value && value <= MAX_VALUE) {
			this->value = (quint16)value;
		} else {
			logger.error("Overflow  value = %d  MAX_VALUE = %d", value, MAX_VALUE);
			COURIER_ERROR();
		}
		return *this;
	}
	// BYTE b; quint8 a = b;
	operator quint16() const {
		return value;
	}
	bool equals(const UNSPECIFIED& that) const {
		return value == that.value;
	}

	void serialize(BLOCK& block) const {
		block.serialize16(value);
	}
	void deserialize(BLOCK& block) {
		block.deserialize16(value);
	}
};
struct UNSPECIFIED2 : public Serializable {
private:
	static const long long MAX_VALUE = 0xFFFFFFFFLL;
	quint32 value;

public:
	UNSPECIFIED2() {
		value = 0;
	}

	UNSPECIFIED2(const UNSPECIFIED2& rhs) {
		this->value = rhs.value;
	}
	// BYTE a(10);
	UNSPECIFIED2(const long long value) {
		if (0 <= value && value <= MAX_VALUE) {
			this->value = (quint32)value;
		} else {
			logger.error("Overflow  value = %d  MAX_VALUE = %d", value, MAX_VALUE);
			COURIER_ERROR();
		}
	}

	// BYTE a, b; a = b;s
	UNSPECIFIED2& operator= (const UNSPECIFIED2& rhs) {
		this->value = rhs.value;
		return *this;
	}
	// BYTE a; a = 100;
	UNSPECIFIED2& operator= (const long long value) {
		if (0 <= value && value <= MAX_VALUE) {
			this->value = (quint32)value;
		} else {
			logger.error("Overflow  value = %d  MAX_VALUE = %d", value, MAX_VALUE);
			COURIER_ERROR();
		}
		return *this;
	}
	// BYTE b; quint8 a = b;
	operator quint32() const {
		return value;
	}
	bool equals(const UNSPECIFIED2& that) const {
		return value == that.value;
	}

	void serialize(BLOCK& block) const {
		block.serialize32(value);
	}
	void deserialize(BLOCK& block) {
		block.deserialize32(value);
	}
};
struct UNSPECIFIED3 : public Serializable {
private:
	static const long long MAX_VALUE = 0xFFFFFFFFFFFFLL;
	quint64 value;

public:
	UNSPECIFIED3() {
		value = 0;
	}

	UNSPECIFIED3(const UNSPECIFIED3& rhs) {
		this->value = rhs.value;
	}
	// BYTE a(10);
	UNSPECIFIED3(const long long value) {
		if (0 <= value && value <= MAX_VALUE) {
			this->value = (quint64)value;
		} else {
			logger.error("Overflow  value = %d  MAX_VALUE = %d", value, MAX_VALUE);
			COURIER_ERROR();
		}
	}

	// BYTE a, b; a = b;s
	UNSPECIFIED3& operator= (const UNSPECIFIED3& rhs) {
		this->value = rhs.value;
		return *this;
	}
	// BYTE a; a = 100;
	UNSPECIFIED3& operator= (const long long value) {
		if (0 <= value && value <= MAX_VALUE) {
			this->value = (quint64)value;
		} else {
			logger.error("Overflow  value = %d  MAX_VALUE = %d", value, MAX_VALUE);
			COURIER_ERROR();
		}
		return *this;
	}
	// BYTE b; quint8 a = b;
	operator quint64() const {
		return value;
	}
	bool equals(const UNSPECIFIED3& that) const {
		return value == that.value;
	}

	void serialize(BLOCK& block) const {
		block.serialize48(value);
	}
	void deserialize(BLOCK& block) {
		block.deserialize48(value);
	}
};


// serialize - write value to block
void serialize(BLOCK& block, const BYTE&          value);
void serialize(BLOCK& block, const CARDINAL&      value);
void serialize(BLOCK& block, const LONG_CARDINAL& value);
void serialize(BLOCK& block, const STRING&        value);
void serialize(BLOCK& block, const UNSPECIFIED&   value);
void serialize(BLOCK& block, const UNSPECIFIED2&  value);
void serialize(BLOCK& block, const UNSPECIFIED3&  value);
// write whole value
void serialize(BLOCK& block, const BLOCK&         value);


// deserialize - read from block and write to value
void deserialize(BLOCK& block, BYTE&          value);
void deserialize(BLOCK& block, CARDINAL&      value);
void deserialize(BLOCK& block, LONG_CARDINAL& value);
void deserialize(BLOCK& block, STRING&        value);
void deserialize(BLOCK& block, UNSPECIFIED&   value);
void deserialize(BLOCK& block, UNSPECIFIED2&  value);
void deserialize(BLOCK& block, UNSPECIFIED3&  value);
// read rest of value
void deserialize(BLOCK& block, BLOCK&         value);


// Suppose T implements Serializable
template <typename T>
struct SEQUENCE : public Serializable {
public:
	static const int MAX_SIZE = 65535;
	const int maxSize;
	SEQUENCE(int maxSize_ = MAX_SIZE) : maxSize(maxSize_) {
		if (0 <= maxSize && maxSize <= MAX_SIZE) {
			data = new T [maxSize];
		} else {
			logger.error("Overflow  maxSize = %d  MAX_SIZE = %d", maxSize, MAX_SIZE);
			COURIER_ERROR();
		}
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
			COURIER_ERROR();
		}
		data[size++] = newValue;
	}
	T& operator[](int i) {
		if (0 <= i && i < maxSize) {
			// OK
		} else {
			logger.error("Unexpected overflow  i = %d  size = %d", i, size);
			COURIER_ERROR();
		}
		return data[i];
	}
	const T& operator[](int i) const {
		if (0 <= i && i < maxSize) {
			// OK
		} else {
			logger.error("Unexpected overflow  i = %d  size = %d", i, size);
			COURIER_ERROR();
		}
		return data[i];
	}

	// read this and write to block
	void serialize(BLOCK& block) const {
		block.serialize16(size);
		for(quint16 i = 0; i < size; i++) {
			data[i].serialize(block);
		}
	}
	// read block and write to this
	void deserialize(BLOCK& block) {
		quint16 t;
		block.deserialize16(t);
		size = t;
		if (maxSize <= size) {
			logger.error("Unexpected overflow  size = %d  maxSize = %d", size, maxSize);
			COURIER_ERROR();
		}
		for(quint16 i = 0; i < size; i++) {
			data[i].deserialize(block);
		}
	}

private:
	int size = 0;
	T* data;
};
template <typename T>
void serialize(BLOCK& block, SEQUENCE<T> value) {
	value.serialize(block);
}
template <typename T>
void deserialize(BLOCK& block, SEQUENCE<T> value) {
	value.deserialize(block);
}

template <typename T>
struct ARRAY : public Serializable {
	static const int MAX_SIZE = 65535;
	const int maxSize;

	ARRAY(int maxSize_) : maxSize(maxSize_) {
		if (0 <= maxSize && maxSize <= MAX_SIZE) {
			data = new (std::nothrow) T [maxSize];
			if (data == nullptr) {
				logger.error("Failed to allocate memory.  maxSize = %d  MAX_SIZE = %d", maxSize, MAX_SIZE);
				COURIER_ERROR();
			}
		} else {
			logger.error("Overflow  maxSize = %d  MAX_SIZE = %d", maxSize, MAX_SIZE);
			COURIER_ERROR();
		}
	}
	~ARRAY() {
		delete[] data;
	}

	void append(const T& newValue) {
		if (maxSize <= size) {
			logger.error("Unexpected overflow  size = %d  maxSize = %d", size, maxSize);
			COURIER_ERROR();
		}
		data[size++] = newValue;
	}
	quint16 getSize() {
		return size;
	}
	void clear() {
		size = 0;
	}

	T& operator[](int i) {
		if (0 <= i && i < maxSize) {
			// OK
		} else {
			logger.error("Unexpected overflow  i = %d  size = %d", i, size);
			COURIER_ERROR();
		}
		return data[i];
	}
	const T& operator[](int i) const {
		if (0 <= i && i < maxSize) {
			// OK
		} else {
			logger.error("Unexpected overflow  i = %d  size = %d", i, size);
			COURIER_ERROR();
		}
		return data[i];
	}

	void serialize(BLOCK& block) const {
		for(quint16 i = 0; i < maxSize; i++) {
			(data[i]).serialize(block);
		}
	}
	void deserialize(BLOCK& block) {
		for(quint16 i = 0; i < maxSize; i++) {
			(data[i]).deserialize(block);
		}
	}
private:
	int size = 0;
	T* data;
};
template <typename T>
void serialize(BLOCK& block, ARRAY<T> value) {
	value.serialize(block);
}
template <typename T>
void deserialize(BLOCK& block, ARRAY<T> value) {
	value.deserialize(block);
}

}

// Declare operator == outside namespace Courier
bool operator==(const Courier::BYTE&          a, const Courier::BYTE&          b);
bool operator==(const Courier::CARDINAL&      a, const Courier::CARDINAL&      b);
bool operator==(const Courier::LONG_CARDINAL& a, const Courier::LONG_CARDINAL& b);
bool operator==(const Courier::STRING&        a, const Courier::STRING&        b);
bool operator==(const Courier::UNSPECIFIED&   a, const Courier::UNSPECIFIED&   b);
bool operator==(const Courier::UNSPECIFIED2&  a, const Courier::UNSPECIFIED2&  b);
bool operator==(const Courier::UNSPECIFIED3&  a, const Courier::UNSPECIFIED3&  b);

#endif /* COURIER_COURIER_H_ */
