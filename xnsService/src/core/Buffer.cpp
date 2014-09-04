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

