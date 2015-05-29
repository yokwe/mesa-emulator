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
// FloppyByteBuffer.h
//

#ifndef FLOPPYBYTEBUFFER_H__
#define FLOPPYBYTEBUFFER_H__

#include "../mesa/Pilot.h"
#include "../agent/DiskFile.h"

class FloppyByteBuffer {
protected:
	static const quint32 SIZE_32 = 4;
	static const quint32 SIZE_16 = 2;
	static const quint32 SIZE_8  = 1;

	quint8        rawData[sizeof(DiskFile::Page)];
	quint8*       data;
	const quint32 capacity; // data capacity [0..limit)
	quint32       limit;    // valid data range [0..limit)
	quint32       pos;      // cursor position

public:
	FloppyByteBuffer() : data(rawData), capacity(sizeof(rawData)), limit(capacity), pos(0) {}
	FloppyByteBuffer(FloppyByteBuffer* that) : data(that->data), capacity(that->capacity), limit(that->limit), pos(that->pos) {}

	// NOTE
	//   Floppy image is array of big endian CARD16.
	//   Layout of 32 bit data is MESA order(low CARD16 and high CARD16. byte order of each CARD16 is big endian).

	// Absolute get and put
	quint32 get32(quint32 offset);
	quint16 get16(quint32 offset);
	quint8  get8 (quint32 offset);

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
	quint32 get32();
	quint16 get16();
	quint8  get8 ();
	void    put32(quint32 value);
	void    put16(quint16 value);
	void    put8 (quint8  value);
};
#endif
