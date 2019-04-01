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

#define COURIER_ERROR() { logger.fatal("ERROR %s %d %s", __FILE__, __LINE__, __FUNCTION__); throw Courier::CourierError(__FILE__, __LINE__, __FUNCTION__); }

namespace Courier {

class CourierError {
public:
	const char *file;
	const int   line;
	const char *func;

	CourierError(const char *file_, const int line_, const char *func_) : file(file_), line(line_), func(func_) {}
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

QString toString(const Block&  value);
QString toString(const quint8  value);
QString toString(const bool    value);
QString toString(const STRING& value);
QString toString(const quint16 value);
QString toString(const quint32 value);
QString toString(const quint64 value);

void serialize(Block& block, const Block&  value);
void serialize(Block& block, const quint8  value);
void serialize(Block& block, const bool    value);
void serialize(Block& block, const STRING& value);
void serialize(Block& block, const quint16 value);
void serialize(Block& block, const quint32 value);
void serialize(Block& block, const quint64 value);

void deserialize(Block& block, Block&   value);
void deserialize(Block& block, quint8&  value);
void deserialize(Block& block, bool&    value);
void deserialize(Block& block, STRING&  value);
void deserialize(Block& block, quint16& value);
void deserialize(Block& block, quint32& value);
void deserialize(Block& block, quint64& value);


template <typename T>
struct SEQUENCE {
public:
	static const int MAX_SIZE = 65535;
	const int maxSize;
	SEQUENCE(int maxSize_ = MAX_SIZE) : maxSize(maxSize_) {
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
		size = 0;
	}
	~SEQUENCE() {
		delete[] data;
	}

//	QString toString() const {
//		QStringList list;
//		for(quint16 i = 0; i < size; i++) {
//			list << Courier::toString(data[i]);
//		}
//		return QString("(%1)[%2]").arg(size).arg(list.join(" "));
//	}
//	void serialize(BLOCK& block) {
//		Courier::serialize(block, size);
//		for(int i = 0; i < size; i++) {
//			Courier::serialize(block, data[i]);
//		}
//	}
//	void deserialize(BLOCK& block) {
//		Courier::deserialize(block, size);
//		for(int i = 0; i < size; i++) {
//			T t;
//			Courier:deserialize(block, t);
//			data[i] = t;
//		}
//	}

	quint16 getSize() const {
		return size;
	}
	void setSize(int newValue) {
		if (0 <= newValue && newValue <= maxSize) {
			size = (quint16)newValue;
		} else {
			logger.error("Unexpected overflow  newValue = %d  maxSize = %d", newValue, maxSize);
			COURIER_ERROR();
		}
	}
	T& operator[](int i) {
		if (0 <= i && i < maxSize) {
			// OK
		} else {
			logger.error("Unexpected overflow  i = %d  maxSize = %d", i, maxSize);
			COURIER_ERROR();
		}
		return data[i];
	}
	const T& operator[](int i) const {
		if (0 <= i && i < maxSize) {
			// OK
		} else {
			logger.error("Unexpected overflow  i = %d  maxSize = %d", i, maxSize);
			COURIER_ERROR();
		}
		return data[i];
	}

private:
	quint16 size;
	T*      data;
};
template <typename T>
QString toString(const SEQUENCE<T>& value) {
	QStringList list;
	quint16 size = value.getSize();
	for(quint16 i = 0; i < size; i++) {
		list << Courier::toString(value[i]);
	}
	return QString("(%1)[%2]").arg(size).arg(list.join(" "));
}
template <typename T>
void serialize(BLOCK& block, const SEQUENCE<T>& value) {
	quint16 size = value.getSize();
	Courier::serialize(block, size);
	for(int i = 0; i < size; i++) {
		Courier::serialize(block, value[i]);
	}
}
template <typename T>
void deserialize(BLOCK& block, SEQUENCE<T>& value) {
	quint16 size;
	Courier::deserialize(block, size);
	value.setSize(size);
	for(int i = 0; i < size; i++) {
		Courier::deserialize(block, value[i]);
	}
}


template <typename T>
struct ARRAY {
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

//	QString toString() const {
//		QStringList list;
//		for(int i = 0; i < maxSize; i++) {
//			list << Courier::toString(data[i]);
//		}
//		return QString("(%1)[%2]").arg(maxSize).arg(list.join(" "));
//	}
//	void serialize(BLOCK& block) {
//		for(int i = 0; i < maxSize; i++) {
//			Courier::serialize(block, data[i]);
//		}
//	}
//	void deserialize(BLOCK& block) {
//		for(int i = 0; i < maxSize; i++) {
//			Courier:deserialize(block, data[i]);
//		}
//	}

	T& operator[](int i) {
		if (0 <= i && i < maxSize) {
			// OK
		} else {
			logger.error("Unexpected overflow  i = %d  maxSize = %d", i, maxSize);
			COURIER_ERROR();
		}
		return data[i];
	}
	const T& operator[](int i) const {
		if (0 <= i && i < maxSize) {
			// OK
		} else {
			logger.error("Unexpected overflow  i = %d  maxSize = %d", i, maxSize);
			COURIER_ERROR();
		}
		return data[i];
	}

private:
	T*      data;
};
template <typename T>
QString toString(const ARRAY<T>& value) {
	quint16 maxSize = value.maxSize;
	QStringList list;
	for(int i = 0; i < maxSize; i++) {
		list << Courier::toString(value[i]);
	}
	return QString("(%1)[%2]").arg(maxSize).arg(list.join(" "));
}
template <typename T>
void serialize(BLOCK& block, const ARRAY<T>& value) {
	for(int i = 0; i < value.maxSize; i++) {
		Courier::serialize(block, value[i]);
	}
}
template <typename T>
void deserialize(BLOCK& block, ARRAY<T>& value) {
	for(int i = 0; i < value.maxSize; i++) {
		Courier::deserialize(block, value[i]);
	}
}

}

#endif /* COURIER_COURIER_H_ */
