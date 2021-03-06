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
// Courier.cpp
//

#include "../util/Debug.h"
#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("cr/courier");

#include "../courier/Courier.h"


//
// Courier::toString
//
QString Courier::toString(const Block&  value) {
	return value.toString();
}
QString Courier::toString(const quint8  value) {
	if (value == 0) return "0";
	if (0xF0 <= value) {
		return QString("0x%1").arg(QString("%1").arg((quint32)value, 0, 16).toUpper());
	}
	return QString("%1").arg((quint32)value);
}
QString Courier::toString(const bool     value) {
	return QString("%1").arg(value ? "T" : "F");
}
QString Courier::toString(const QString& value) {
	return QString("\"%1\"").arg(value);
}
QString Courier::toString(const quint16  value) {
	if (value == 0) return "0";
	if ((value & 0x00FF) == 0) {
		return QString("0x%1").arg(QString("%1").arg(value, 0, 16).toUpper());
	}
	if (0xFFF0 <= value) {
		return QString("0x%1").arg(QString("%1").arg(value, 0, 16).toUpper());
	}
	return QString("%1").arg(value);
}
QString Courier::toString(const quint32  value) {
	if (value == 0) return "0";
	if ((value & 0x0000FFFF) == 0) {
		return QString("0x%1").arg(QString("%1").arg(value, 0, 16).toUpper());
	}
	if (0xFFFFFFF0U <= value) {
		return QString("0x%1").arg(QString("%1").arg(value, 0, 16).toUpper());
	}

	return QString("%1").arg(value);
}
QString Courier::toString(const quint64  value) {
	if (value == 0) return "0";
	if ((value & 0x0000FFFF) == 0) {
		return QString("0x%1").arg(QString("%1").arg(value, 0, 16).toUpper());
	}
	if (0xFFFFFFFFFFF0U <= value && value <= 0xFFFFFFFFFFFFU) {
		return QString("0x%1").arg(QString("%1").arg(value, 0, 16).toUpper());
	}
	return QString("%1").arg(value);
}


//
// Courier::serialize
//

// Write whole value
void Courier::serialize(Block& block, const Block&   value) {
	block.serialize(value);
}
void Courier::serialize(Block& block, const quint8   value) {
	block.serialize8(value);
}
void Courier::serialize(Block& block, const bool     value) {
	quint16 t = value ? 1 : 0;
	block.serialize16(t);
}
void Courier::serialize(Block& block, const QString& value) {
	block.serialize(value);
}
void Courier::serialize(Block& block, const quint16  value) {
	block.serialize16(value);
}
void Courier::serialize(Block& block, const quint32  value) {
	block.serialize32(value);
}
void Courier::serialize(Block& block, const quint64  value) {
	block.serialize48(value);
}


//
// Courier::deserialize
//

// Read rest of block
void Courier::deserialize(Block& block, Block&    value) {
	block.deserialize(value);
}
void Courier::deserialize(Block& block, quint8&   value) {
	block.deserialize8(value);
}
void Courier::deserialize(Block& block, bool&     value) {
	quint16 t;
	block.deserialize16(t);
	value = (t != 0);
}
void Courier::deserialize(Block& block, QString&  value) {
	block.deserialize(value);
}
void Courier::deserialize(Block& block, quint16&  value) {
	block.deserialize16(value);
}
void Courier::deserialize(Block& block, quint32&  value) {
	block.deserialize32(value);
}
void Courier::deserialize(Block& block, quint64&  value) {
	block.deserialize48(value);
}

template <>void Courier::SEQUENCE<bool>::initData() {
	for(int i = 0; i < capacity; i++) {
		data[i] = false;
	}
}
template <>void Courier::SEQUENCE<quint8>::initData() {
	for(int i = 0; i < capacity; i++) {
		data[i] = 0;
	}
}
template <>void Courier::SEQUENCE<quint16>::initData() {
	for(int i = 0; i < capacity; i++) {
		data[i] = 0;
	}
}
template <>void Courier::SEQUENCE<quint32>::initData() {
	for(int i = 0; i < capacity; i++) {
		data[i] = 0;
	}
}
template <>void Courier::SEQUENCE<quint64>::initData() {
	for(int i = 0; i < capacity; i++) {
		data[i] = 0;
	}
}


// Specialization of method initData()s
template <>void Courier::ARRAY<bool>::initData() {
	for(int i = 0; i < capacity; i++) {
		data[i] = false;
	}
}
template <>void Courier::ARRAY<quint8>::initData() {
	for(int i = 0; i < capacity; i++) {
		data[i] = 0;
	}
}
template <>void Courier::ARRAY<quint16>::initData() {
	for(int i = 0; i < capacity; i++) {
		data[i] = 0;
	}
}
template <>void Courier::ARRAY<quint32>::initData() {
	for(int i = 0; i < capacity; i++) {
		data[i] = 0;
	}
}
template <>void Courier::ARRAY<quint64>::initData() {
	for(int i = 0; i < capacity; i++) {
		data[i] = 0;
	}
}
