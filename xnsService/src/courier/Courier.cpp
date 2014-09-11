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
