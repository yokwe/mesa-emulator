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
// Stream.cpp
//

#include "../util/Debug.h"
#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("cr/stream");

#include "../courier/Stream.h"
#include "../courier/Courier.h"

void Courier::Stream::put8 (const quint8         value) {
	const quint16 nByte = 1;
	if (size < (pos + nByte)) {
		logger.error("Unexpected overflow  size = %d  pos = %d  nByte = %d", size, pos, nByte);
		COURIER_FATAL_ERROR();
	}

	quint8 p0 = (quint8)(value >> 0);

	data[pos++] = p0;

	// Update limit
	if (limit < pos) {
		limit = pos;
	}
}
void Courier::Stream::put16(const quint16        value) {
	const quint16 nByte = 2;
	if (size < (pos + nByte)) {
		logger.error("Unexpected overflow  size = %d  pos = %d  nByte = %d", size, pos, nByte);
		COURIER_FATAL_ERROR();
	}

	quint8 p1 = (quint8)(value >> 0);
	quint8 p0 = (quint8)(value >> 8);

	data[pos++] = p0;
	data[pos++] = p1;

	// Update limit
	if (limit < pos) {
		limit = pos;
	}
}
void Courier::Stream::put32(const quint32        value) {
	const quint16 nByte = 4;
	if (size < (pos + nByte)) {
		logger.error("Unexpected overflow  size = %d  pos = %d  nByte = %d", size, pos, nByte);
		COURIER_FATAL_ERROR();
	}

	quint8 p3 = (quint8)(value >> 0);
	quint8 p2 = (quint8)(value >> 8);
	quint8 p1 = (quint8)(value >> 16);
	quint8 p0 = (quint8)(value >> 24);

	data[pos++] = p0;
	data[pos++] = p1;
	data[pos++] = p2;
	data[pos++] = p3;

	// Update limit
	if (limit < pos) {
		limit = pos;
	}
}
void Courier::Stream::put48(const quint64        value) {
	const quint16 nByte = 6;
	if (size < (pos + nByte)) {
		logger.error("Unexpected overflow  size = %d  pos = %d  nByte = %d", size, pos, nByte);
		COURIER_FATAL_ERROR();
	}

	quint8 p5 = (quint8)(value >> 0);
	quint8 p4 = (quint8)(value >> 8);
	quint8 p3 = (quint8)(value >> 16);
	quint8 p2 = (quint8)(value >> 24);
	quint8 p1 = (quint8)(value >> 32);
	quint8 p0 = (quint8)(value >> 40);

	data[pos++] = p0;
	data[pos++] = p1;
	data[pos++] = p2;
	data[pos++] = p3;
	data[pos++] = p4;
	data[pos++] = p5;

	// Update limit
	if (limit < pos) {
		limit = pos;
	}
}
void Courier::Stream::put  (const QByteArray&    value) {
	int nByte = value.size();
	if (size < (pos + 2 + nByte + (nByte & 1))) {
		logger.error("Unexpected overflow  size = %d  pos = %d  nByte = %d", size, pos, nByte);
		COURIER_FATAL_ERROR();
	}

	// write length
	quint8 p1 = (quint8)(nByte >> 0);
	quint8 p0 = (quint8)(nByte >> 8);

	data[pos++] = p0;
	data[pos++] = p1;

	// write data
	for(int i = 0; i < nByte; i++) {
		data[pos++] = value.at(i);
	}

	// Write pad byte to make length even
	if (nByte & 1) {
		data[pos++] = 0;
	}

	// Update limit
	if (limit < pos) {
		limit = pos;
	}
}
// write whole value. pos of value will not be change.
void Courier::Stream::put  (const Stream& value) {
	int nByte = value.limit;
	if (size < (pos + nByte)) {
		logger.error("Unexpected overflow  size = %d  pos = %d  nByte = %d", size, pos, nByte);
		COURIER_FATAL_ERROR();
	}

	for(int i = 0; i < nByte; i++) {
		data[pos++] = value.data[i];
	}

	// Update limit
	if (limit < pos) {
		limit = pos;
	}
}

quint8 Courier::Stream::get8 () {
	const quint16 nByte = 1;
	if (limit < (pos + nByte)) {
		logger.error("Unexpected overflow  limit = %d  pos = %d  nByte = %d", limit, pos, nByte);
		COURIER_FATAL_ERROR();
	}

	return data[pos++];
}
quint16 Courier::Stream::get16() {
	const quint16 nByte = 2;
	if (limit < (pos + nByte)) {
		logger.error("Unexpected overflow  limit = %d  pos = %d  nByte = %d", limit, pos, nByte);
		COURIER_FATAL_ERROR();
	}

	quint16 p0 = data[pos++];
	quint16 p1 = data[pos++];

	return (p0 << 8) | (p1 << 0);
}
quint32 Courier::Stream::get32() {
	const quint16 nByte = 4;
	if (limit < (pos + nByte)) {
		logger.error("Unexpected overflow  limit = %d  pos = %d  nByte = %d", limit, pos, nByte);
		COURIER_FATAL_ERROR();
	}

	quint32 p0 = data[pos++];
	quint32 p1 = data[pos++];
	quint32 p2 = data[pos++];
	quint32 p3 = data[pos++];

	return (p0 << 24) | (p1 << 16) | (p2 << 8) | (p3 << 0);
}
quint64 Courier::Stream::get48() {
	const quint16 nByte = 6;
	if (limit < (pos + nByte)) {
		logger.error("Unexpected overflow  limit = %d  pos = %d  nByte = %d", limit, pos, nByte);
		COURIER_FATAL_ERROR();
	}

	quint64 p0 = data[pos++];
	quint64 p1 = data[pos++];
	quint64 p2 = data[pos++];
	quint64 p3 = data[pos++];
	quint64 p4 = data[pos++];
	quint64 p5 = data[pos++];

	return (p0 << 40) | (p1 << 32) | (p2 << 24) | (p3 << 16) | (p4 << 8) | (p5 << 0);
}
void Courier::Stream::get  (QByteArray&    value) {
	int nByte = get16();
	if (limit < (pos + nByte + (nByte & 1))) {
		logger.error("Unexpected overflow  limit = %d  pos = %d  nByte = %d", limit, pos, nByte);
		COURIER_FATAL_ERROR();
	}

	value.clear();
	value.reserve(nByte);

	value.append((char*)data + pos, nByte);
	pos += nByte;

	if (nByte & 1) {
		pos++;
	}
}
// read rest of stream and pos of stream will change.
void Courier::Stream::get  (Stream& value) {
	int nByte = remaining();
	if (value.limit < (value.pos + nByte)) {
		logger.error("Unexpected overflow  limit = %d  pos = %d  nByte = %d", value.limit, value.pos, nByte);
		COURIER_FATAL_ERROR();
	}

	for(int i = 0; i < nByte; i++) {
		value.data[value.pos++] = data[pos++];
	}

	// Update limit of value
	if (value.limit < value.pos) {
		value.limit = value.pos;
	}
}

QString Courier::Stream::toString() const {
	QString hexString;
	for(quint16 i = 0; i < limit; i++) {
		quint8 c = data[i];
		hexString.append(QString("%1").arg((quint16)c, 2, 16, QChar('0')).toUpper());
	}
	return QString("(%1)%2").arg(limit).arg(hexString);
}

QString& operator<<(QString& out, const Courier::Stream& value) {
	return out.append(value.toString());
}
