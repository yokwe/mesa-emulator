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
// Buffer.h
//

#ifndef BUFFER_H__
#define BUFFER_H__

#include "../util/Util.h"

class Buffer {
public:
	static const quint32 MAX_SIZE = 1514;
	static const quint32 MIN_SIZE =   46;

protected:
	static const quint32 SIZE_48 = 6;
	static const quint32 SIZE_32 = 4;
	static const quint32 SIZE_16 = 2;
	static const quint32 SIZE_8  = 1;

	quint8*       data;
	quint32       limit;    // valid data range [0..limit)
	const quint32 capacity; // data capacity [0..limit)
	quint32       pos;      // cursor position

	quint8        rawData[MAX_SIZE];

public:
	Buffer(quint8* data_, quint32 limit_);
	Buffer(Buffer* that) : data(that->data), limit(that->limit), capacity(that->capacity), pos(that->pos) {}

	// Absolute get and put
	quint64 get48(quint32 offset);
	quint32 get32(quint32 offset);
	quint16 get16(quint32 offset);
	quint8  get8 (quint32 offset);

	void    set48(quint32 offset, quint64 value);
	void    set32(quint32 offset, quint32 value);
	void    set16(quint32 offset, quint16 value);
	void    set8 (quint32 offset, quint8  value);

	// cursor position
	quint32 getPos() {
		return pos;
	}
	// set cursor position. newValue must be [0..limit)
	void setPos(quint32 newValue);

	// limit
	quint32 getLimit() {
		return limit;
	}
	// set buffer limit. newValue must be [0..capacity)
	void setLimit(quint32 newValue);

	// clear buffer. pos = limit = 0
	void clear() {
		pos = limit = 0;
	}
	// rewind buffer. limit = pos; pos = 0;
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
	quint64 get48();
	quint32 get32();
	quint16 get16();
	quint8  get8 ();
	void    put48(quint64 value);
	void    put32(quint32 value);
	void    put16(quint16 value);
	void    put8 (quint8  value);
};

class EthernetBuffer : public Buffer {
public:
	static const quint32 SIZE          = 14;

	static const quint32 OFFSET_DEST   =  0;
	static const quint32 OFFSET_SOURCE =  6;
	static const quint32 OFFSET_TYPE   = 12;
	static const quint32 OFFSET_DATA   = SIZE;

	EthernetBuffer(quint8* data_, quint32 limit_) : Buffer(data_, limit_) {}
	EthernetBuffer(EthernetBuffer* that) : Buffer((Buffer*)that) {}

	quint64 getDest() {
		return get48(OFFSET_DEST);
	}
	void setDest(quint64 value) {
		set48(OFFSET_DEST, value);
	}

	quint64 getSource() {
		return get48(OFFSET_SOURCE);
	}
	void setSource(quint64 value) {
		set48(OFFSET_SOURCE, value);
	}

	quint16 getType() {
		return get16(OFFSET_TYPE);
	}
	void setType(quint16 value) {
		set16(OFFSET_TYPE, value);
	}
};

class NetworkAddressBuffer : public Buffer {
public:
	static const quint32 SIZE           = 12;

	static const quint32 OFFSET_NETWORK =  0;
	static const quint32 OFFSET_HOST    =  4;
	static const quint32 OFFSET_SOCKET  = 10;

	const quint32 offset;
	NetworkAddressBuffer(Buffer* buffer_, quint32 offset_) : Buffer(buffer_), offset(offset_) {}

	quint32 getNetwork() {
		return get32(offset + OFFSET_NETWORK);
	}
	void setNetwork(quint32 value) {
		set32(offset + OFFSET_NETWORK, value);
	}

	quint64 getHost() {
		return get48(offset + OFFSET_HOST);
	}
	void setHost(quint64 value) {
		set48(offset + OFFSET_HOST, value);
	}

	quint16 getSocket() {
		return get16(offset + OFFSET_SOCKET);
	}
	void setSocket(quint16 value) {
		set16(offset + OFFSET_SOCKET, value);
	}

	void set(quint32 network, quint64 host, quint16 socket) {
		set32(offset + OFFSET_NETWORK, network);
		set48(offset + OFFSET_HOST,    host);
		set16(offset + OFFSET_SOCKET,  socket);
	}
};

class DatagramBuffer : public EthernetBuffer {
public:
	static const quint32 SIZE            = 30;

	static const quint32 OFFSET_CHECKSUM = EthernetBuffer::SIZE +  0;
	static const quint32 OFFSET_LENGTH   = EthernetBuffer::SIZE +  2;
	static const quint32 OFFSET_HOP      = EthernetBuffer::SIZE +  4;
	static const quint32 OFFSET_TYPE     = EthernetBuffer::SIZE +  5;
	static const quint32 OFFSET_DEST     = EthernetBuffer::SIZE +  6;
	static const quint32 OFFSET_SOURCE   = EthernetBuffer::SIZE + 18;
	static const quint32 OFFSET_DATA     = EthernetBuffer::SIZE + SIZE;

	NetworkAddressBuffer dest;
	NetworkAddressBuffer source;

	DatagramBuffer() :
		EthernetBuffer(0, 0),
		dest(this, OFFSET_DEST), source(this, OFFSET_SOURCE) {}
	DatagramBuffer(quint8* data_, quint32 limit_) :
		EthernetBuffer(data_, limit_),
		dest(this, OFFSET_DEST), source(this, OFFSET_SOURCE) {}
	DatagramBuffer(DatagramBuffer* that) :
		EthernetBuffer(that),
		dest(this, OFFSET_DEST), source(this, OFFSET_SOURCE) {}

	quint16 getChecksum() {
		return get16(OFFSET_CHECKSUM);
	}
	void setChecksum(quint16 value) {
		set16(OFFSET_CHECKSUM, value);
	}

	quint16 getLength() {
		return get16(OFFSET_LENGTH);
	}
	void setLength(quint16 value) {
		set16(OFFSET_LENGTH, value);
	}

	quint8 getHop() {
		return get8(OFFSET_HOP);
	}
	void setHop(quint8 value) {
		set8(OFFSET_HOP, value);
	}

	quint8 getType() {
		return get8(OFFSET_TYPE);
	}
	void setType(quint8 value) {
		set8(OFFSET_TYPE, value);
	}

	quint32 getDataLength() {
		return get16(OFFSET_LENGTH) - SIZE;
	}
	void setPosData() {
		setPos(OFFSET_DATA);
	}
};
#endif
