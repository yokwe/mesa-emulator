#ifndef COURIER_H__
#define COURIER_H__

#include <QtCore>
typedef quint64 quint48;

#include "ByteBuffer.h"

namespace Courier {
	void serialize  (ByteBuffer* buffer, const quint48& value) {
		buffer->put48(value);
	}
    void deserialize(ByteBuffer* buffer, quint48& value) {
		value = buffer->get48();
    }

	void serialize  (ByteBuffer* buffer, const quint32& value) {
		buffer->put32(value);
	}
    void deserialize(ByteBuffer* buffer, quint32& value) {
		value = buffer->get32();
    }

	void serialize  (ByteBuffer* buffer, const quint16& value) {
		buffer->put16(value);
	}
    void deserialize(ByteBuffer* buffer, quint16& value) {
		value = buffer->get16();
    }

	void serialize  (ByteBuffer* buffer, const quint8& value) {
		buffer->put8(value);
	}
    void deserialize(ByteBuffer* buffer, quint8& value) {
		value = buffer->get8();
    }
}

#endif
