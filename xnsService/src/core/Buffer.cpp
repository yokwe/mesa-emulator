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
// Buffer.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("buffer");

#include "../util/Debug.h"

#include "Buffer.h"

quint64 Buffer::get48(quint32 offset) {
	const quint32 SIZE = 6;
	if (MAX_SIZE <= (offset + SIZE)) ERROR();
	quint8* p = data + offset;
	quint64 ret = 0;
	for(quint32 i = 0; i < SIZE; i++) {
		ret <<= 8;
		ret |= *p++;
	}
	return ret;
}
quint32 Buffer::get32(quint32 offset) {
	const quint32 SIZE = 4;
	if (MAX_SIZE <= (offset + SIZE)) ERROR();
	quint8* p = data + offset;
	quint32 ret = 0;
	for(quint32 i = 0; i < SIZE; i++) {
		ret <<= 8;
		ret |= *p++;
	}
	return ret;
}
quint16 Buffer::get16(quint32 offset) {
	const quint32 SIZE = 2;
	if (MAX_SIZE <= (offset + SIZE)) ERROR();
	quint8* p = data + offset;
	quint16 ret = 0;
	for(quint32 i = 0; i < SIZE; i++) {
		ret <<= 8;
		ret |= *p++;
	}
	return ret;
}
quint8 Buffer::get8(quint32 offset) {
	const quint32 SIZE = 1;
	if (MAX_SIZE <= (offset + SIZE)) ERROR();
	return data[offset];
}

void Buffer::set48(quint32 offset, quint64 value) {
	const quint32 SIZE = 6;
	if (MAX_SIZE <= (offset + SIZE)) ERROR();
	quint8* p = data + offset + SIZE;
	for(quint32 i = 0; i < SIZE; i++) {
		*--p = (quint8)value;
		value >>= 8;
	}
}
void Buffer::set32(quint32 offset, quint32 value) {
	const quint32 SIZE = 4;
	if (MAX_SIZE <= (offset + SIZE)) ERROR();
	quint8* p = data + offset + SIZE;
	for(quint32 i = 0; i < SIZE; i++) {
		*--p = (quint8)value;
		value >>= 8;
	}
}
void Buffer::set16(quint32 offset, quint16 value) {
	const quint32 SIZE = 2;
	if (MAX_SIZE <= (offset + SIZE)) ERROR();
	quint8* p = data + offset + SIZE;
	for(quint32 i = 0; i < SIZE; i++) {
		*--p = (quint8)value;
		value >>= 8;
	}
}
void Buffer::set8(quint32 offset, quint8 value) {
	const quint32 SIZE = 1;
	if (MAX_SIZE <= (offset + SIZE)) ERROR();
	data[offset] = value;
}

