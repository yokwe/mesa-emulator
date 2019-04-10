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
// CourierStream.h
//

#ifndef COURIER_STREAM_H_
#define COURIER_STREAM_H_

#include <QtCore>

namespace Courier {

class Stream {
public:
	Stream(const quint8* data_, const int size_) : data(data_), size(size_), limit(0), pos(0) {}

	// Copy constructor
	Stream(const Stream& that) : data(that.data), size(that.size), limit(that.limit), pos(that.pos) {}
	Stream& operator=(const Stream& that) {
		data  = that.data;
		size  = that.size;
		limit = that.limit;
		pos   = that.pos;

		return *this;
	}

	// Move constructor
	Stream(Stream&& that) : data(that.data), size(that.size), limit(that.limit), pos(that.pos) {
		that.data = nullptr;
	}
	Stream& operator=(Stream&& that) {
		data  = that.data;
		size  = that.size;
		limit = that.limit;
		pos   = that.pos;

		that.data = nullptr;

		return *this;
	}

	// start fresh write
	void clear() {
		limit = 0;
		pos   = 0;
	}
	// start fresh read
	void reweind() {
		pos = 0;
	}

	int getSize() {
		return size;
	}
	int getLimit() {
		return limit;
	}
	int getPos() {
		return pos;
	}
	int remaining() {
		return limit - pos;
	}

	Stream& operator<<(const quint8         value) {
		put8(value);
		return *this;
	}
	Stream& operator<<(const quint16        value) {
		put16(value);
		return *this;
	}
	Stream& operator<<(const quint32        value) {
		put32(value);
		return *this;
	}
	Stream& operator<<(const quint64        value) {
		put48(value);
		return *this;
	}
	Stream& operator<<(const QByteArray&    value) {
		put(value);
		return *this;
	}
	Stream& operator<<(const Stream& value) {
		put(value);
		return *this;
	}

	Stream& operator>>(quint8&        value) {
		value = get8();
		return *this;
	}
	Stream& operator>>(quint16&       value) {
		value = get16();
		return *this;
	}
	Stream& operator>>(quint32&       value) {
		value = get32();
		return *this;
	}
	Stream& operator>>(quint64&       value) {
		value = get48();
		return *this;
	}
	Stream& operator>>(QByteArray&    value) {
		get(value);
		return *this;
	}
	Stream& operator>>(Stream& value) {
		get(value);
		return *this;
	}

	QString toString() const;
private:
	quint8* data;
	int     size;
	int     limit;
	int     pos;

	void put8 (const quint8         value);
	void put16(const quint16        value);
	void put32(const quint32        value);
	void put48(const quint64        value);
	void put  (const QByteArray&    value);
	// write whole value. pos of value will not be change.
	void put  (const Stream& value);

	quint8  get8 ();
	quint16 get16();
	quint32 get32();
	quint64 get48();
	void get  (QByteArray&    value);
	// read rest of stream and pos of stream will change.
	void get  (Stream& value);
};

// QString out; Stream value; out << value;
QString& operator<<(QString& out, const Courier::Stream& value);

template <int SIZE>
class StreamData {
public:
	Stream stream;
	quint8 data[SIZE];

	StreamData() : stream(&data[0], sizeof(data)) {
		::bzero(data, sizeof(data));
	}

	operator Stream() {
		return stream;
	}

	QString toString() {
		return stream.toString();
	}

	template <typename T>
	Stream& operator<<(const T& value) {
		return stream << value;
	}
	template <typename T>
	Stream& operator>>(T& value) {
		return stream >> value;
	}
};

template <int SIZE>
QString& operator<<(QString& out, const Courier::StreamData<SIZE>& value) {
	return value.toString();
}

}

#endif /* COURIER_STREAM_H_ */
