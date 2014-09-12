#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("courier");

#include "../util/Debug.h"

#include "Courier.h"
#include "stub/StubDatagram.h"

#include <stdio.h>

void Courier::serialize  (ByteBuffer& buffer, const quint48& value) {
	buffer.put48(value);
}
void Courier::deserialize(ByteBuffer& buffer, quint48& value) {
	value = buffer.get48();
}

void Courier::serialize  (ByteBuffer& buffer, const quint32& value) {
	buffer.put32(value);
}
void Courier::deserialize(ByteBuffer& buffer, quint32& value) {
	value = buffer.get32();
}

void Courier::serialize  (ByteBuffer& buffer, const quint16& value) {
	buffer.put16(value);
}
void Courier::deserialize(ByteBuffer& buffer, quint16& value) {
	value = buffer.get16();
}

void Courier::serialize  (ByteBuffer& buffer, const quint8& value) {
	buffer.put8(value);
}
void Courier::deserialize(ByteBuffer& buffer, quint8& value) {
	value = buffer.get8();
}

static char getSocketNameBuffer[6];
const char* Courier::getSocketName(quint16 value) {
	const char* ret = Courier::getName((Courier::Datagram::WellKnownSocket)value);
	if (ret) return ret;
	sprintf(getSocketNameBuffer, "%u", value);
	return getSocketNameBuffer;
}

quint16 Courier::checksum(quint8* data, quint32 offset, quint32 length) {
	quint32 s = 0;

	for(quint32 i = 0; i < length; i += 2) {
		quint16 w = data[offset++] & 0x00ffU;
		w = (w << 8) | (data[offset++] & 0x00ffU);
		// add w to s
		s += w;
		// if there is overflow, increment t
		if (0x10000U <= s) s = (s + 1) & 0xffffU;
		// shift left
		s <<= 1;
		// if there is overflow, increment t
		if (0x10000U <= s) s = (s + 1) & 0xffffU;
	}
	return (quint16)s;
}
