#ifndef COURIER_H__
#define COURIER_H__

#include <stdio.h>

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

    class CourierError {
    public:
    	const char* func;
    	const char* file;
    	const int   line;
    	CourierError(const char* func_, const char* file_, const int line_) : func(func_), file(file_), line(line_) {}
    };

    const char* getSocketName(quint16 value);
}

#define COURIER_ERROR() {throw Courier::CourierError(__FUNCTION__, __FILE__, __LINE__);}

#endif
