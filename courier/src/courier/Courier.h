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

#include "../courier/Block.h"

#define COURIER_FATAL_ERROR() { logger.fatal("FATAL ERROR %s %d %s", __FILE__, __LINE__, __FUNCTION__); throw Courier::CourierFatalError(__FILE__, __LINE__, __FUNCTION__); }

namespace Courier {

class CourierError {
};

class CourierFatalError : public CourierError {
public:
	const char *file;
	const int   line;
	const char *func;

	CourierFatalError(const char *file_, const int line_, const char *func_) : file(file_), line(line_), func(func_) {}
};

using BLOCK         = Block;
using BYTE          = quint8;
using BOOLEAN       = bool;
using CARDINAL      = quint16;
using LONG_CARDINAL = quint32;
using STRING        = QString;
using UNSPECIFIED   = quint16;
using UNSPECIFIED2  = quint32;
using UNSPECIFIED3  = quint64;

QString toString(const Block&   value);
QString toString(const quint8   value);
QString toString(const bool     value);
QString toString(const QString& value);
QString toString(const quint16  value);
QString toString(const quint32  value);
QString toString(const quint64  value);

// Write whole value to block
void serialize(Block& block, const Block&   value);
void serialize(Block& block, const quint8   value);
void serialize(Block& block, const bool     value);
void serialize(Block& block, const QString& value);
void serialize(Block& block, const quint16  value);
void serialize(Block& block, const quint32  value);
void serialize(Block& block, const quint64  value);

// Read rest of block
void deserialize(Block& block, Block&    value);
void deserialize(Block& block, quint8&   value);
void deserialize(Block& block, bool&     value);
void deserialize(Block& block, QString&  value);
void deserialize(Block& block, quint16&  value);
void deserialize(Block& block, quint32&  value);
void deserialize(Block& block, quint64&  value);


template <typename T>
class SEQUENCE {
public:
	static const int MAX_SIZE = 65535;
	using     TYPE = T;

	SEQUENCE(int capacity_) {
		allocate(capacity_);
	}
	~SEQUENCE() {
		delete[] data;
	}

	SEQUENCE& operator=(std::initializer_list<T> initList) {
		int j = 0;
		for(auto i = initList.begin(); i != initList.end(); i++) {
			data[j++] = *i;
		}
		// Update size -- only for SEQUENCE
		int size = initList.size();
		setSize(size);

		return *this;
	}

	// Copy constructor
	SEQUENCE(const SEQUENCE& that) {
		int capacity_ = that.capacity;
		allocate(capacity_);
		// Update size -- only for SEQUENCE
		setSize(that.size);
		for(int i = 0; i < size; i++) {
			data[i] = that.data[i];
		}
	}
	SEQUENCE& operator=(const SEQUENCE& that) {
		delete[] data;

		allocate(that.capacity);
		// Update size -- only for SEQUENCE
		setSize(that.size);
		for(int i = 0; i < size; i++) {
			data[i] = that.data[i];
		}

		return *this;
	}

	// Move constructor
	SEQUENCE(SEQUENCE&& that) {
		capacity  = that.capacity;
		size      = that.size;
		data      = that.data;
		that.data = nullptr;
	}
	SEQUENCE& operator=(SEQUENCE&& that) {
		capacity  = that.capacity;
		size      = that.size;
		data      = that.data;
		that.data = nullptr;

		return *this;
	}

	quint16 getSize() const {
		return (quint16)size;
	}
	void setSize(int newValue) {
		if (0 <= newValue && newValue <= capacity) {
			size = newValue;
		} else {
			logger.error("Unexpected overflow  newValue = %d  maxSize = %d", newValue, capacity);
			COURIER_FATAL_ERROR();
		}
	}
	quint16 getCapacity() const {
		return (quint16)capacity;
	}

	T* begin() {
		return data;
	}
	T* end() {
		return data + size;
	}

	T& operator[](int i) {
		if (0 <= i && i < size) {
			// OK
		} else {
			logger.error("Unexpected overflow  i = %d  size = %d", i, size);
			COURIER_FATAL_ERROR();
		}
		return data[i];
	}
	const T& operator[](int i) const {
		if (0 <= i && i < size) {
			// OK
		} else {
			logger.error("Unexpected overflow  i = %d  size = %d", i, size);
			COURIER_FATAL_ERROR();
		}
		return data[i];
	}

protected:
	int capacity;
	int size;
	T*  data;

	void allocate(int capacity_) {
		capacity = capacity_;
		size     = capacity;
		if (0 <= capacity && capacity <= MAX_SIZE) {
			data = new (std::nothrow) T [capacity];
			if (data == nullptr) {
				logger.error("Failed to allocate memory.  capacity = %d  MAX_SIZE = %d", capacity, MAX_SIZE);
				COURIER_FATAL_ERROR();
			}
		} else {
			logger.error("Overflow  capacity = %d  MAX_SIZE = %d", capacity, MAX_SIZE);
			COURIER_FATAL_ERROR();
		}
	}
};
//
// Explicit instantiation of basic type
//
template class SEQUENCE<bool>;
template class SEQUENCE<QString>;
template class SEQUENCE<quint8>;
template class SEQUENCE<quint16>;
template class SEQUENCE<quint32>;
template class SEQUENCE<quint64>;


template <typename T>
class ARRAY {
public:
	static const int MAX_SIZE = 65535;
	using     TYPE = T;

	ARRAY(int capacity_) {
		allocate(capacity_);
	}
	~ARRAY() {
		delete[] data;
	}

	ARRAY& operator=(std::initializer_list<T> initList) {
		int j = 0;
		for(auto i = initList.begin(); i != initList.end(); i++) {
			data[j++] = *i;
		}
		return *this;
	}

	// Copy constructor
	ARRAY(const ARRAY& that) {
		int capacity_ = that.capacity;
		allocate(capacity_);
		for(int i = 0; i < size; i++) {
			data[i] = that.data[i];
		}
	}
	ARRAY& operator=(const ARRAY& that) {
		delete[] data;

		allocate(that.capacity);
		for(int i = 0; i < size; i++) {
			data[i] = that.data[i];
		}

		return *this;
	}

	// Move constructor
	ARRAY(ARRAY&& that) {
		capacity  = that.capacity;
		size      = that.size;
		data      = that.data;
		that.data = nullptr;
	}
	ARRAY& operator=(ARRAY&& that) {
		capacity  = that.capacity;
		size      = that.size;
		data      = that.data;
		that.data = nullptr;

		return *this;
	}

	quint16 getSize() const {
		return (quint16)size;
	}
	// No setSize for ARRAY

	quint16 getCapacity() const {
		return (quint16)capacity;
	}

	T* begin() {
		return data;
	}
	T* end() {
		return data + size;
	}

	T& operator[](int i) {
		if (0 <= i && i < size) {
			// OK
		} else {
			logger.error("Unexpected overflow  i = %d  size = %d", i, size);
			COURIER_FATAL_ERROR();
		}
		return data[i];
	}
	const T& operator[](int i) const {
		if (0 <= i && i < size) {
			// OK
		} else {
			logger.error("Unexpected overflow  i = %d  size = %d", i, size);
			COURIER_FATAL_ERROR();
		}
		return data[i];
	}

protected:
	int capacity;
	int size;
	T*  data;

	void allocate(int capacity_) {
		capacity = capacity_;
		size     = capacity;
		if (0 <= capacity && capacity <= MAX_SIZE) {
			data = new (std::nothrow) T [capacity];
			if (data == nullptr) {
				logger.error("Failed to allocate memory.  capacity = %d  MAX_SIZE = %d", capacity, MAX_SIZE);
				COURIER_FATAL_ERROR();
			}
		} else {
			logger.error("Overflow  capacity = %d  MAX_SIZE = %d", capacity, MAX_SIZE);
			COURIER_FATAL_ERROR();
		}
	}
};
//
// Explicit instantiation of basic type
//
template class ARRAY<bool>;
template class ARRAY<QString>;
template class ARRAY<quint8>;
template class ARRAY<quint16>;
template class ARRAY<quint32>;
template class ARRAY<quint64>;


//
// Template SEQUENCE_N
//
template <typename T, int N = 65535>
class SEQUENCE_N : public SEQUENCE<T> {
public:
	int         SIZE;

	SEQUENCE_N() : SEQUENCE<T>(N), SIZE(N) {}

	SEQUENCE_N(std::initializer_list<T> initList) : SEQUENCE<T>(N), SIZE(N) {
		SEQUENCE<T>::operator =(initList);
	}

	// Copy constructor
	SEQUENCE_N(const SEQUENCE_N& that) : SEQUENCE<T>((SEQUENCE<T>)that), SIZE(that.SIZE) {}
	SEQUENCE_N& operator=(const SEQUENCE_N&& that) {
		SEQUENCE<T>::operator=(that);
		return *this;
	}

	// Move constructor
	SEQUENCE_N(SEQUENCE_N&& that) : SEQUENCE<T>((SEQUENCE<T>)that), SIZE(that.SIZE) {}
	SEQUENCE_N& operator=(SEQUENCE_N&& that) {
		SEQUENCE<T>::operator=(that);
		return *this;
	}
};

//
// Template for ARRAY_N
//
template <typename T, int N>
class ARRAY_N : public ARRAY<T> {
public:
	int      SIZE;

	ARRAY_N() : ARRAY<T>(N), SIZE(N) {}

	ARRAY_N(std::initializer_list<T> initList) : ARRAY<T>(N), SIZE(N) {
		ARRAY<T>::operator =(initList);
	}

	// Copy constructor
	ARRAY_N(const ARRAY_N& that) : ARRAY<T>((ARRAY<T>)that), SIZE(that.SIZE) {}
	ARRAY_N& operator=(const ARRAY_N& that) {
		ARRAY<T>::operator=(that);
		return *this;
	}

	// Move constructor
	ARRAY_N(ARRAY_N&& that) : ARRAY<T>((ARRAY<T>)that), SIZE(that.SIZE) {}
	ARRAY_N& operator=(ARRAY_N&& that) {
		ARRAY<T>::operator=(that);
		return *this;
	}
};

}

#endif /* COURIER_COURIER_H_ */
