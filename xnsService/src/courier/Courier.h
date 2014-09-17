#ifndef COURIER_H__
#define COURIER_H__

#include "../util/Util.h"

#include <QtCore>
typedef quint64 quint48;

#include "ByteBuffer.h"

namespace Courier {
	void serialize  (ByteBuffer& buffer, const quint48& value);
    void deserialize(ByteBuffer& buffer, quint48& value);

	void serialize  (ByteBuffer& buffer, const quint32& value);
    void deserialize(ByteBuffer& buffer, quint32& value);

	void serialize  (ByteBuffer& buffer, const quint16& value);
    void deserialize(ByteBuffer& buffer, quint16& value);

	void serialize  (ByteBuffer& buffer, const quint8& value);
    void deserialize(ByteBuffer& buffer, quint8& value);

    struct CourierData {
    	static const quint32 UNITILIAZED_BASE = 0xffffffff;
    	quint32 base;
    	CourierData() : base(UNITILIAZED_BASE) {}
    };

    const char* getSocketName(quint16 value);

    quint16 checksum(quint8* base, quint32 offset, quint32 length);
}

#define COURIER_ERROR() RUNTIME_ERROR()

#endif
