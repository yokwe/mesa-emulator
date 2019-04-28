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
// Block.h
//

#ifndef COURIER_BLOCK_H__
#define COURIER_BLOCK_H__

#include <QtCore>

namespace Courier {

class Block {
public:
	Block(quint8* data_, quint16 capacity_) : state(State::write), capacity(capacity_), offset(0), pos(0), limit(0), data(data_) {}

	// Destructor
	~Block() {}

	// Copy Constructor
	Block(const Block& that) : state(that.state), capacity(that.capacity), offset(that.offset), pos(that.pos), limit(that.limit), data(that.data) {}

	// Copy Assignment
	Block& operator= (const Block& that) {
		if (this != &that) {
			this->state    = that.state;
			this->capacity = that.capacity;
			this->offset   = that.offset;
			this->pos      = that.pos;
			this->limit    = that.limit;
			this->data     = that.data;
		}

		return *this;
	}

	QString toString() const;

	Block remnant();

	// Set pos with 0 for fresh write
	void clear();
	// Fill data[0..capacity) with zero
	void zero();
	// Set pos with 0 for read after serialization
	void rewind();
	// Set pos with 0 for read after deserialization
	void reset();
	// Return remaining byte for read
	quint16 remaining()   const;
	quint16 getPos()      const;
	quint16 getLimit()    const;
	quint16 getCapacity() const;
	quint16 getOffset()   const;

	quint8* getData();
	void    setLimit(quint16 newValue);

	Block remainder() {
		Block ret(*this);
		ret.offset = ret.pos;
		return ret;
	}

	bool equals(const Block& that) const;

	// serialize - write value to Block
	void serialize8 (const quint8  value);
	void serialize16(const quint16 value);
	// for update of checksum field
	void serialize16(const quint16 offset, const quint16 value);
	void serialize32(const quint32 value);
	void serialize48(const quint64 value);

	void serialize(const QString& value);
	// write whole value. pos of value will not be change.
	void serialize(const Block&   value);

	// deserialize - read from Block and write to value
	void deserialize8 (quint8&  value);
	void deserialize16(quint16& value);
	void deserialize32(quint32& value);
	void deserialize48(quint64& value);

	void deserialize(QString& value);
	// read rest of value and pos of value will change
	void deserialize(Block&   value);

	quint16 computeChecksum() const;
private:
	// Make private to detect unintentional invocation of default constructor
	Block() : state(State::write), capacity(0), offset(0), pos(0), limit(0), data(nullptr) {}

	enum class State {read, write} state;

	quint16 capacity;
	quint16 offset;
	quint16 pos;
	quint16 limit;

	quint8* data;
};

template <int SIZE>
class BlockData {
public:
	Block  block;
	quint8 data[SIZE];

	BlockData() : block(&data[0], (quint16)sizeof(data)) {
		::bzero(data, sizeof(data));
	}

	operator Block() {
		return block;
	}
};


}

#endif
