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
// testCourier.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("testCourier");

#include "testBase.h"

class testCourier : public testBase {
	CPPUNIT_TEST_SUITE(testCourier);
	CPPUNIT_TEST(testBlock);
	CPPUNIT_TEST(testBlock8);
	CPPUNIT_TEST(testBlock16);
	CPPUNIT_TEST(testBlock32);
	CPPUNIT_TEST(testBlock48);
	CPPUNIT_TEST(testBlockQString);
	CPPUNIT_TEST(testBlockBlockSerialize);
	CPPUNIT_TEST(testBlockBlockDeserialize);

	CPPUNIT_TEST(testBYTE);
	CPPUNIT_TEST(testCARDINAL);
	CPPUNIT_TEST(testLONGCARDINAL);
	CPPUNIT_TEST(testSTRING);
	CPPUNIT_TEST(testUNSPECIFIED);
	CPPUNIT_TEST(testUNSPECIFIED2);
	CPPUNIT_TEST(testUNSPECIFIED3);

	CPPUNIT_TEST(testARRAY);
	CPPUNIT_TEST(testSEQUENCE);

	CPPUNIT_TEST_SUITE_END();

public:
	void testBlock() {
		const quint16 capacity = 256;
		quint8 data[capacity];
		for(quint16 i = 0; i < capacity; i++) {
			data[i] = i;
		}
		Courier::BLOCK block(data, sizeof(data));

		CPPUNIT_ASSERT_EQUAL(capacity, block.capacity);
		CPPUNIT_ASSERT_EQUAL((quint16)0, block.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)0, block.getLimit());
	}
	void testBlock8() {
		const quint16 capacity = 2;
		quint8 data[capacity];
		for(quint16 i = 0; i < capacity; i++) {
			data[i] = i;
		}
		Courier::BLOCK block(data, sizeof(data));

		quint8 a0 = 0x11;
		quint8 a1 = 0x22;
		block.serialize8(a0);
		block.serialize8(a1);

		{
			bool errorObserved = false;
			try {
				block.serialize8(a1);
			} catch (Courier::CourierError& e) {
				errorObserved = true;
			}
			CPPUNIT_ASSERT_EQUAL(true, errorObserved);
		}

		CPPUNIT_ASSERT_EQUAL((quint16)2, block.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)2, block.getLimit());

		block.rewind();

		CPPUNIT_ASSERT_EQUAL((quint16)0, block.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)2, block.getLimit());

		quint8 b0, b1;
		block.deserialize8(b0);
		block.deserialize8(b1);

		CPPUNIT_ASSERT_EQUAL((quint16)2, block.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)2, block.getLimit());

		CPPUNIT_ASSERT_EQUAL(a0, b0);
		CPPUNIT_ASSERT_EQUAL(a1, b1);

		{
			bool errorObserved = false;
			try {
				block.deserialize8(b1);
			} catch (Courier::CourierError& e) {
				errorObserved = true;
			}
			CPPUNIT_ASSERT_EQUAL(true, errorObserved);
		}
	}
	void testBlock16() {
		const quint16 capacity = 4;
		quint8 data[capacity];
		for(quint16 i = 0; i < capacity; i++) {
			data[i] = i;
		}
		Courier::BLOCK block(data, sizeof(data));

		quint16 a0 = 0x1111;
		quint16 a1 = 0x2222;
		block.serialize16(a0);
		block.serialize16(a1);

		{
			bool errorObserved = false;
			try {
				block.serialize16(a1);
			} catch (Courier::CourierError& e) {
				errorObserved = true;
			}
			CPPUNIT_ASSERT_EQUAL(true, errorObserved);
		}

		CPPUNIT_ASSERT_EQUAL((quint16)4, block.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)4, block.getLimit());

		block.rewind();

		CPPUNIT_ASSERT_EQUAL((quint16)0, block.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)4, block.getLimit());

		quint16 b0, b1;
		block.deserialize16(b0);
		block.deserialize16(b1);

		CPPUNIT_ASSERT_EQUAL((quint16)4, block.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)4, block.getLimit());

		CPPUNIT_ASSERT_EQUAL(a0, b0);
		CPPUNIT_ASSERT_EQUAL(a1, b1);

		{
			bool errorObserved = false;
			try {
				block.deserialize16(b1);
			} catch (Courier::CourierError& e) {
				errorObserved = true;
			}
			CPPUNIT_ASSERT_EQUAL(true, errorObserved);
		}
	}
	void testBlock32() {
		const quint16 capacity = 8;
		quint8 data[capacity];
		for(quint16 i = 0; i < capacity; i++) {
			data[i] = i;
		}
		Courier::BLOCK block(data, sizeof(data));

		quint32 a0 = 0x1111;
		quint32 a1 = 0x2222;
		block.serialize32(a0);
		block.serialize32(a1);

		{
			bool errorObserved = false;
			try {
				block.serialize16(a1);
			} catch (Courier::CourierError& e) {
				errorObserved = true;
			}
			CPPUNIT_ASSERT_EQUAL(true, errorObserved);
		}

		CPPUNIT_ASSERT_EQUAL((quint16)8, block.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)8, block.getLimit());

		block.rewind();

		CPPUNIT_ASSERT_EQUAL((quint16)0, block.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)8, block.getLimit());

		quint32 b0, b1;
		block.deserialize32(b0);
		block.deserialize32(b1);

		CPPUNIT_ASSERT_EQUAL((quint16)8, block.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)8, block.getLimit());

		CPPUNIT_ASSERT_EQUAL(a0, b0);
		CPPUNIT_ASSERT_EQUAL(a1, b1);

		{
			bool errorObserved = false;
			try {
				block.deserialize32(b1);
			} catch (Courier::CourierError& e) {
				errorObserved = true;
			}
			CPPUNIT_ASSERT_EQUAL(true, errorObserved);
		}
	}
	void testBlock48() {
		const quint16 capacity = 12;
		quint8 data[capacity];
		for(quint16 i = 0; i < capacity; i++) {
			data[i] = i;
		}
		Courier::BLOCK block(data, sizeof(data));

		quint64 a0 = 0x111111;
		quint64 a1 = 0x222222;
		block.serialize48(a0);
		block.serialize48(a1);

		{
			bool errorObserved = false;
			try {
				block.serialize16(a1);
			} catch (Courier::CourierError& e) {
				errorObserved = true;
			}
			CPPUNIT_ASSERT_EQUAL(true, errorObserved);
		}

		CPPUNIT_ASSERT_EQUAL((quint16)12, block.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)12, block.getLimit());

		block.rewind();

		CPPUNIT_ASSERT_EQUAL((quint16)0,  block.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)12, block.getLimit());

		quint64 b0, b1;
		block.deserialize48(b0);
		block.deserialize48(b1);

		CPPUNIT_ASSERT_EQUAL((quint16)12, block.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)12, block.getLimit());

		CPPUNIT_ASSERT_EQUAL(a0, b0);
		CPPUNIT_ASSERT_EQUAL(a1, b1);

		{
			bool errorObserved = false;
			try {
				block.deserialize48(b1);
			} catch (Courier::CourierError& e) {
				errorObserved = true;
			}
			CPPUNIT_ASSERT_EQUAL(true, errorObserved);
		}
	}
	void testBlockQString() {
		const quint16 capacity = 12;
		quint8 data[capacity];
		for(quint16 i = 0; i < capacity; i++) {
			data[i] = i;
		}
		Courier::BLOCK block(data, sizeof(data));

		QString a0("ABCD");
		QString a1("EFG");
		block.serialize(a0);
		block.serialize(a1);

		{
			bool errorObserved = false;
			try {
				block.serialize(a1);
			} catch (Courier::CourierError& e) {
				errorObserved = true;
			}
			CPPUNIT_ASSERT_EQUAL(true, errorObserved);
		}

		CPPUNIT_ASSERT_EQUAL((quint16)12, block.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)12, block.getLimit());

		block.rewind();

		CPPUNIT_ASSERT_EQUAL((quint16)0,  block.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)12, block.getLimit());

		QString b0, b1;
		block.deserialize(b0);
		block.deserialize(b1);

		CPPUNIT_ASSERT_EQUAL((quint16)12, block.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)12, block.getLimit());

		CPPUNIT_ASSERT_EQUAL(0, QString::compare(a0, b0));
		CPPUNIT_ASSERT_EQUAL(0, QString::compare(a1, b1));

		{
			bool errorObserved = false;
			try {
				block.deserialize(b1);
			} catch (Courier::CourierError& e) {
				errorObserved = true;
			}
			CPPUNIT_ASSERT_EQUAL(true, errorObserved);
		}
	}
	void testBlockBlockSerialize() {
		const quint16 capacity = 4;
		quint8 src_data[capacity + 2];
		Courier::BLOCK src(src_data, sizeof(src_data));

		quint8 dst_data[capacity + 2 + 2];
		Courier::BLOCK dst(dst_data, sizeof(dst_data));

		quint8 expect_data[capacity + 2 + 2];
		Courier::BLOCK expect(expect_data, sizeof(expect_data));

		{
			quint8 c = 0x11;
			src.serialize8(c);
			src.serialize8(c);
			// src = 1111
			c = 0x22;
			src.serialize8(c);
			src.serialize8(c);
			// src = 11112222
			src.rewind();
			src.deserialize8(c);
			src.deserialize8(c);
		}
		CPPUNIT_ASSERT_EQUAL((quint16)2, src.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)4, src.getLimit());

		{
			quint8 c = 0x33;
			dst.serialize8(c);
			dst.serialize8(c);
			// dst = 3333
		}
		CPPUNIT_ASSERT_EQUAL((quint16)2, dst.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)2, dst.getLimit());

		dst.serialize(src);
		// dst = 333311112222
		CPPUNIT_ASSERT_EQUAL((quint16)6, dst.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)6, dst.getLimit());


		// expect = 333311112222
		{
			quint8 c = 0x33;
			expect.serialize8(c);
			expect.serialize8(c);
			c = 0x11;
			expect.serialize8(c);
			expect.serialize8(c);
			c = 0x22;
			expect.serialize8(c);
			expect.serialize8(c);
		}
		CPPUNIT_ASSERT_EQUAL(true, expect.equals(dst));

		{
			bool errorObserved = false;
			try {
				dst.serialize(src);
			} catch (Courier::CourierError& e) {
				errorObserved = true;
			}
			CPPUNIT_ASSERT_EQUAL(true, errorObserved);
		}
	}
	void testBlockBlockDeserialize() {
		const quint16 capacity = 4;
		quint8 src_data[capacity + 2];
		Courier::BLOCK src(src_data, sizeof(src_data));

		quint8 dst_data[capacity + 2 + 2];
		Courier::BLOCK dst(dst_data, sizeof(dst_data));

		quint8 expect_data[capacity + 2 + 2];
		Courier::BLOCK expect(expect_data, sizeof(expect_data));

		{
			quint8 c;
			c = 0x11;
			src.serialize8(c);
			src.serialize8(c);
			// src = 1111
			c = 0x22;
			src.serialize8(c);
			src.serialize8(c);
			// src = 11112222
			src.rewind();
			src.deserialize8(c);
			src.deserialize8(c);
		}

		{
			quint8 c;
			c = 0x33;
			dst.serialize8(c);
			dst.serialize8(c);
			// dst = 33
		}
		src.deserialize(dst);
		// dst = 33332222

		{
			quint8 c;
			c = 0x33;
			expect.serialize8(c);
			expect.serialize8(c);
			c = 0x22;
			expect.serialize8(c);
			expect.serialize8(c);
		}
		// expect = 33332222

		CPPUNIT_ASSERT_EQUAL(true, expect.equals(dst));

		{
			bool errorObserved = false;
			try {
				src.reset();
				dst.deserialize(src);
			} catch (Courier::CourierError& e) {
				errorObserved = true;
			}
			CPPUNIT_ASSERT_EQUAL(true, errorObserved);
		}
	}

	void testBYTE() {
		quint8 data[100];
		Courier::BLOCK block(data, sizeof(data));

		const quint8 v = 2;

		{
			Courier::BYTE a;
			CPPUNIT_ASSERT_EQUAL((quint8)0, (quint8)a);
		}
		{
			Courier::BYTE b(v);
			CPPUNIT_ASSERT_EQUAL(v, (quint8)b);
			Courier::BYTE a(b);
			CPPUNIT_ASSERT_EQUAL(v, (quint8)a);
		}
		{
			Courier::BYTE b(v);
			CPPUNIT_ASSERT_EQUAL(v, (quint8)b);
			Courier::BYTE a;
			CPPUNIT_ASSERT_EQUAL((quint8)0, (quint8)a);
			a = b;
			CPPUNIT_ASSERT_EQUAL(v, (quint8)a);
		}
		{
			Courier::BYTE a;
			CPPUNIT_ASSERT_EQUAL((quint8)0, (quint8)a);
			a = v;
			CPPUNIT_ASSERT_EQUAL(v, (quint8)a);
		}

		{
			block.zero();
			block.clear();

			Courier::BYTE a(v);
			CPPUNIT_ASSERT_EQUAL((quint8)0, data[0]);
			CPPUNIT_ASSERT_EQUAL((quint16)0, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)0, block.getLimit());
			Courier::serialize(block, a);
			CPPUNIT_ASSERT_EQUAL(v, data[0]);
			CPPUNIT_ASSERT_EQUAL((quint16)1, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)1, block.getLimit());

			block.rewind();
			Courier::BYTE b;
			CPPUNIT_ASSERT_EQUAL((quint8)0, (quint8)b);
			CPPUNIT_ASSERT_EQUAL((quint16)0, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)1, block.getLimit());
			Courier::deserialize(block, b);
			CPPUNIT_ASSERT_EQUAL(v, (quint8)a);
			CPPUNIT_ASSERT_EQUAL((quint16)1, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)1, block.getLimit());
		}
	}
	void testCARDINAL() {
		quint8 data[100];
		Courier::BLOCK block(data, sizeof(data));

		const quint8  v0 = 0x11;
		const quint8  v1 = 0x22;
		const quint16 v = (quint16)(v0 << 8 | v1 << 0);

		{
			Courier::CARDINAL a;
			CPPUNIT_ASSERT_EQUAL((quint8)0, (quint8)a);
		}
		{
			Courier::CARDINAL b(v);
			CPPUNIT_ASSERT_EQUAL(v, (quint16)b);
			Courier::CARDINAL a(b);
			CPPUNIT_ASSERT_EQUAL(v, (quint16)a);
		}
		{
			Courier::CARDINAL b(v);
			CPPUNIT_ASSERT_EQUAL(v, (quint16)b);
			Courier::CARDINAL a;
			CPPUNIT_ASSERT_EQUAL((quint16)0, (quint16)a);
			a = b;
			CPPUNIT_ASSERT_EQUAL(v, (quint16)a);
		}
		{
			Courier::CARDINAL a;
			CPPUNIT_ASSERT_EQUAL((quint16)0, (quint16)a);
			a = v;
			CPPUNIT_ASSERT_EQUAL(v, (quint16)a);
		}

		{
			block.zero();
			block.clear();

			Courier::CARDINAL a(v);
			CPPUNIT_ASSERT_EQUAL((quint8)0, data[0]);
			CPPUNIT_ASSERT_EQUAL((quint16)0, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)0, block.getLimit());
			Courier::serialize(block, a);
			CPPUNIT_ASSERT_EQUAL(v0, data[0]);
			CPPUNIT_ASSERT_EQUAL(v1, data[1]);
			CPPUNIT_ASSERT_EQUAL((quint16)2, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)2, block.getLimit());

			block.rewind();
			Courier::CARDINAL b;
			CPPUNIT_ASSERT_EQUAL((quint8)0, (quint8)b);
			CPPUNIT_ASSERT_EQUAL((quint16)0, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)2, block.getLimit());
			Courier::deserialize(block, b);
			CPPUNIT_ASSERT_EQUAL(v, (quint16)a);
			CPPUNIT_ASSERT_EQUAL((quint16)2, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)2, block.getLimit());
		}
	}
	void testLONGCARDINAL() {
		quint8 data[100];
		Courier::BLOCK block(data, sizeof(data));

		const quint8  v0 = 0x11;
		const quint8  v1 = 0x22;
		const quint8  v2 = 0x33;
		const quint8  v3 = 0x44;
		const quint32 v = (quint32)(v0 << 24 | v1 << 16 | v2 << 8 | v3 << 0);

		{
			Courier::LONG_CARDINAL a;
			CPPUNIT_ASSERT_EQUAL((quint32)0, (quint32)a);
		}
		{
			Courier::LONG_CARDINAL b(v);
			CPPUNIT_ASSERT_EQUAL(v, (quint32)b);
			Courier::LONG_CARDINAL a(b);
			CPPUNIT_ASSERT_EQUAL(v, (quint32)a);
		}
		{
			Courier::LONG_CARDINAL b(v);
			CPPUNIT_ASSERT_EQUAL(v, (quint32)b);
			Courier::LONG_CARDINAL a;
			CPPUNIT_ASSERT_EQUAL((quint32)0, (quint32)a);
			a = b;
			CPPUNIT_ASSERT_EQUAL(v, (quint32)a);
		}
		{
			Courier::LONG_CARDINAL a;
			CPPUNIT_ASSERT_EQUAL((quint32)0, (quint32)a);
			a = v;
			CPPUNIT_ASSERT_EQUAL(v, (quint32)a);
		}

		{
			block.zero();
			block.clear();

			Courier::LONG_CARDINAL a(v);
			CPPUNIT_ASSERT_EQUAL((quint8)0, data[0]);
			CPPUNIT_ASSERT_EQUAL((quint16)0, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)0, block.getLimit());
			Courier::serialize(block, a);
			CPPUNIT_ASSERT_EQUAL(v0, data[0]);
			CPPUNIT_ASSERT_EQUAL(v1, data[1]);
			CPPUNIT_ASSERT_EQUAL(v2, data[2]);
			CPPUNIT_ASSERT_EQUAL(v3, data[3]);
			CPPUNIT_ASSERT_EQUAL((quint16)4, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)4, block.getLimit());

			block.rewind();
			Courier::LONG_CARDINAL b;
			CPPUNIT_ASSERT_EQUAL((quint32)0, (quint32)b);
			CPPUNIT_ASSERT_EQUAL((quint16)0, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)4, block.getLimit());
			Courier::deserialize(block, b);
			CPPUNIT_ASSERT_EQUAL(v, (quint32)a);
			CPPUNIT_ASSERT_EQUAL((quint16)4, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)4, block.getLimit());
		}
	}

#define CPPUNIT_ASSERT_EQUAL_QSTRING(a,b) CPPUNIT_ASSERT_EQUAL((a).toStdString(), ((QString)b).toStdString())

	void testSTRING() {
		quint8 data[100];
		Courier::BLOCK block(data, sizeof(data));

		const quint8  v0 = 0x11;
		const quint8  v1 = 0x22;

		QString v;
		v.append(v0);
		v.append(v1);

		QString z;

		{
			Courier::STRING a;
			CPPUNIT_ASSERT_EQUAL_QSTRING(z, a);
		}
		{
			Courier::STRING b(v);
			CPPUNIT_ASSERT_EQUAL_QSTRING(v, b);
			Courier::STRING a(b);
			CPPUNIT_ASSERT_EQUAL_QSTRING(v, a);
			CPPUNIT_ASSERT_EQUAL_QSTRING(v, (QString)a);
		}
		{
			Courier::STRING b(v);
			CPPUNIT_ASSERT_EQUAL_QSTRING(v, b);
			Courier::STRING a;
			CPPUNIT_ASSERT_EQUAL_QSTRING(z, a);
			a = b;
			CPPUNIT_ASSERT_EQUAL_QSTRING(v, a);
		}
		{
			Courier::STRING a;
			CPPUNIT_ASSERT_EQUAL_QSTRING(z, a);
			a = v;
			CPPUNIT_ASSERT_EQUAL_QSTRING(v, a);
		}

		{
			block.zero();
			block.clear();

			Courier::STRING a(v);
			CPPUNIT_ASSERT_EQUAL((quint8)0, data[0]);
			CPPUNIT_ASSERT_EQUAL((quint16)0, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)0, block.getLimit());
			Courier::serialize(block, a);
			CPPUNIT_ASSERT_EQUAL((quint8)0, data[0]);
			CPPUNIT_ASSERT_EQUAL((quint8)2, data[1]);
			CPPUNIT_ASSERT_EQUAL(v0, data[2]);
			CPPUNIT_ASSERT_EQUAL(v1, data[3]);
			CPPUNIT_ASSERT_EQUAL((quint16)4, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)4, block.getLimit());

			block.rewind();
			Courier::STRING b;
			CPPUNIT_ASSERT_EQUAL(z.toStdString(), ((QString)b).toStdString());
			CPPUNIT_ASSERT_EQUAL((quint16)0, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)4, block.getLimit());
			Courier::deserialize(block, b);
			CPPUNIT_ASSERT_EQUAL(v.toStdString(), ((QString)a).toStdString());
			CPPUNIT_ASSERT_EQUAL((quint16)4, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)4, block.getLimit());
		}
	}
	void testUNSPECIFIED() {
		quint8 data[100];
		Courier::BLOCK block(data, sizeof(data));

		const quint8  v0 = 0x11;
		const quint8  v1 = 0x22;
		const quint16 v = (quint16)(v0 << 8 | v1 << 0);

		{
			Courier::UNSPECIFIED a;
			CPPUNIT_ASSERT_EQUAL((quint8)0, (quint8)a);
		}
		{
			Courier::UNSPECIFIED b(v);
			CPPUNIT_ASSERT_EQUAL(v, (quint16)b);
			Courier::UNSPECIFIED a(b);
			CPPUNIT_ASSERT_EQUAL(v, (quint16)a);
		}
		{
			Courier::UNSPECIFIED b(v);
			CPPUNIT_ASSERT_EQUAL(v, (quint16)b);
			Courier::UNSPECIFIED a;
			CPPUNIT_ASSERT_EQUAL((quint16)0, (quint16)a);
			a = b;
			CPPUNIT_ASSERT_EQUAL(v, (quint16)a);
		}
		{
			Courier::UNSPECIFIED a;
			CPPUNIT_ASSERT_EQUAL((quint16)0, (quint16)a);
			a = v;
			CPPUNIT_ASSERT_EQUAL(v, (quint16)a);
		}

		{
			block.zero();
			block.clear();

			Courier::UNSPECIFIED a(v);
			CPPUNIT_ASSERT_EQUAL((quint8)0, data[0]);
			CPPUNIT_ASSERT_EQUAL((quint16)0, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)0, block.getLimit());
			Courier::serialize(block, a);
			CPPUNIT_ASSERT_EQUAL(v0, data[0]);
			CPPUNIT_ASSERT_EQUAL(v1, data[1]);
			CPPUNIT_ASSERT_EQUAL((quint16)2, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)2, block.getLimit());

			block.rewind();
			Courier::UNSPECIFIED b;
			CPPUNIT_ASSERT_EQUAL((quint8)0, (quint8)b);
			CPPUNIT_ASSERT_EQUAL((quint16)0, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)2, block.getLimit());
			Courier::deserialize(block, b);
			CPPUNIT_ASSERT_EQUAL(v, (quint16)a);
			CPPUNIT_ASSERT_EQUAL((quint16)2, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)2, block.getLimit());
		}
	}
	void testUNSPECIFIED2() {
		quint8 data[100];
		Courier::BLOCK block(data, sizeof(data));

		const quint8  v0 = 0x11;
		const quint8  v1 = 0x22;
		const quint8  v2 = 0x33;
		const quint8  v3 = 0x44;
		const quint32 v = (quint32)(v0 << 24 | v1 << 16 | v2 << 8 | v3 << 0);

		{
			Courier::UNSPECIFIED2 a;
			CPPUNIT_ASSERT_EQUAL((quint32)0, (quint32)a);
		}
		{
			Courier::UNSPECIFIED2 b(v);
			CPPUNIT_ASSERT_EQUAL(v, (quint32)b);
			Courier::UNSPECIFIED2 a(b);
			CPPUNIT_ASSERT_EQUAL(v, (quint32)a);
		}
		{
			Courier::UNSPECIFIED2 b(v);
			CPPUNIT_ASSERT_EQUAL(v, (quint32)b);
			Courier::UNSPECIFIED2 a;
			CPPUNIT_ASSERT_EQUAL((quint32)0, (quint32)a);
			a = b;
			CPPUNIT_ASSERT_EQUAL(v, (quint32)a);
		}
		{
			Courier::UNSPECIFIED2 a;
			CPPUNIT_ASSERT_EQUAL((quint32)0, (quint32)a);
			a = v;
			CPPUNIT_ASSERT_EQUAL(v, (quint32)a);
		}

		{
			block.zero();
			block.clear();

			Courier::UNSPECIFIED2 a(v);
			CPPUNIT_ASSERT_EQUAL((quint8)0, data[0]);
			CPPUNIT_ASSERT_EQUAL((quint16)0, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)0, block.getLimit());
			Courier::serialize(block, a);
			CPPUNIT_ASSERT_EQUAL(v0, data[0]);
			CPPUNIT_ASSERT_EQUAL(v1, data[1]);
			CPPUNIT_ASSERT_EQUAL(v2, data[2]);
			CPPUNIT_ASSERT_EQUAL(v3, data[3]);
			CPPUNIT_ASSERT_EQUAL((quint16)4, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)4, block.getLimit());

			block.rewind();
			Courier::UNSPECIFIED2 b;
			CPPUNIT_ASSERT_EQUAL((quint32)0, (quint32)b);
			CPPUNIT_ASSERT_EQUAL((quint16)0, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)4, block.getLimit());
			Courier::deserialize(block, b);
			CPPUNIT_ASSERT_EQUAL(v, (quint32)a);
			CPPUNIT_ASSERT_EQUAL((quint16)4, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)4, block.getLimit());
		}
	}
	void testUNSPECIFIED3() {
		quint8 data[100];
		Courier::BLOCK block(data, sizeof(data));

		const quint8  v0 = 0x11;
		const quint8  v1 = 0x22;
		const quint8  v2 = 0x33;
		const quint8  v3 = 0x44;
		const quint8  v4 = 0x55;
		const quint8  v5 = 0x66;
		const quint64 v = (quint64)((quint64)v0 << 40 | ((quint64)v1) << 32 | ((quint64)v2) << 24 | ((quint64)v3) << 16 | ((quint64)v4) << 8 | ((quint64)v5) << 0);

		{
			Courier::UNSPECIFIED3 a;
			CPPUNIT_ASSERT_EQUAL((quint64)0, (quint64)a);
		}
		{
			Courier::UNSPECIFIED3 b(v);
			CPPUNIT_ASSERT_EQUAL(v, (quint64)b);
			Courier::UNSPECIFIED3 a(b);
			CPPUNIT_ASSERT_EQUAL(v, (quint64)a);
		}
		{
			Courier::UNSPECIFIED3 b(v);
			CPPUNIT_ASSERT_EQUAL(v, (quint64)b);
			Courier::UNSPECIFIED3 a;
			CPPUNIT_ASSERT_EQUAL((quint64)0, (quint64)a);
			a = b;
			CPPUNIT_ASSERT_EQUAL(v, (quint64)a);
		}
		{
			Courier::UNSPECIFIED3 a;
			CPPUNIT_ASSERT_EQUAL((quint64)0, (quint64)a);
			a = v;
			CPPUNIT_ASSERT_EQUAL(v, (quint64)a);
		}

		{
			block.zero();
			block.clear();

			Courier::UNSPECIFIED3 a(v);
			CPPUNIT_ASSERT_EQUAL((quint8)0, data[0]);
			CPPUNIT_ASSERT_EQUAL((quint16)0, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)0, block.getLimit());
			Courier::serialize(block, a);
			CPPUNIT_ASSERT_EQUAL(v0, data[0]);
			CPPUNIT_ASSERT_EQUAL(v1, data[1]);
			CPPUNIT_ASSERT_EQUAL(v2, data[2]);
			CPPUNIT_ASSERT_EQUAL(v3, data[3]);
			CPPUNIT_ASSERT_EQUAL(v4, data[4]);
			CPPUNIT_ASSERT_EQUAL(v5, data[5]);
			CPPUNIT_ASSERT_EQUAL((quint16)6, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)6, block.getLimit());

			block.rewind();
			Courier::UNSPECIFIED3 b;
			CPPUNIT_ASSERT_EQUAL((quint64)0, (quint64)b);
			CPPUNIT_ASSERT_EQUAL((quint16)0, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)6, block.getLimit());
			Courier::deserialize(block, b);
			CPPUNIT_ASSERT_EQUAL(v, (quint64)a);
			CPPUNIT_ASSERT_EQUAL((quint16)6, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)6, block.getLimit());
		}
	}
	void testARRAY() {
		{
			int s = 2;
			Courier::ARRAY<Courier::CARDINAL> a{s};
			CPPUNIT_ASSERT_EQUAL(s, a.maxSize);

			Courier::CARDINAL v1 = 0x1111;
			Courier::CARDINAL v2 = 0x2222;
			a.append(v1);
			CPPUNIT_ASSERT_EQUAL(1, a.getSize());
			a.append(v2);
			CPPUNIT_ASSERT_EQUAL(2, a.getSize());

			CPPUNIT_ASSERT_EQUAL(v1, a[0]);
			CPPUNIT_ASSERT_EQUAL(v2, a[1]);

			bool errorObserved = false;
			try {
				a.append(v2);
			} catch (Courier::CourierError& e) {
				errorObserved = true;
			}
			CPPUNIT_ASSERT_EQUAL(true, errorObserved);
		}
		{
			int s = 2;
			Courier::ARRAY<Courier::CARDINAL> a{s};
			CPPUNIT_ASSERT_EQUAL(s, a.maxSize);

			Courier::CARDINAL v1 = 0x1111;
			Courier::CARDINAL v2 = 0x2222;
			a[0] = v1;
			a[1] = v2;

			CPPUNIT_ASSERT_EQUAL(v1, a[0]);
			CPPUNIT_ASSERT_EQUAL(v2, a[1]);

			{
				bool errorObserved = false;
				try {
					Courier::CARDINAL t = 0x3333;
					a[s + 1] = t;
				} catch (Courier::CourierError& e) {
					errorObserved = true;
				}
				CPPUNIT_ASSERT_EQUAL(true, errorObserved);
			}
			{
				bool errorObserved = false;
				try {
					Courier::CARDINAL t;
					t = a[s + 1];
				} catch (Courier::CourierError& e) {
					errorObserved = true;
				}
				CPPUNIT_ASSERT_EQUAL(true, errorObserved);
			}
		}
		{
			Courier::ARRAY<Courier::CARDINAL> a{2};
			CPPUNIT_ASSERT_EQUAL(2, a.maxSize);

			Courier::CARDINAL v1 = 0x1122;
			Courier::CARDINAL v2 = 0x3344;
			a[0] = v1;
			a[1] = v2;

			CPPUNIT_ASSERT_EQUAL(v1, a[0]);
			CPPUNIT_ASSERT_EQUAL(v2, a[1]);

			quint8 data[6];
			Courier::BLOCK block(data, sizeof(data));

			Courier::serialize(block, a);
			CPPUNIT_ASSERT_EQUAL((quint16)4, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)4, block.getLimit());
			CPPUNIT_ASSERT_EQUAL((quint8)0x11, data[0]);
			CPPUNIT_ASSERT_EQUAL((quint8)0x22, data[1]);
			CPPUNIT_ASSERT_EQUAL((quint8)0x33, data[2]);
			CPPUNIT_ASSERT_EQUAL((quint8)0x44, data[3]);
			CPPUNIT_ASSERT_EQUAL((quint8)0x00, data[4]);

			Courier::ARRAY<Courier::CARDINAL> b{2};
			block.rewind();
			Courier::deserialize(block, b);
			CPPUNIT_ASSERT_EQUAL(a.maxSize, b.maxSize);
			CPPUNIT_ASSERT_EQUAL(a[0], b[0]);
			CPPUNIT_ASSERT_EQUAL(a[1], b[1]);
		}
	}
	void testSEQUENCE() {
		{
			int s = 2;
			Courier::SEQUENCE<Courier::CARDINAL> a{s};
			CPPUNIT_ASSERT_EQUAL(s, a.maxSize);

			Courier::CARDINAL v1 = 0x1111;
			Courier::CARDINAL v2 = 0x2222;
			a.append(v1);
			CPPUNIT_ASSERT_EQUAL(1, a.getSize());
			a.append(v2);
			CPPUNIT_ASSERT_EQUAL(2, a.getSize());

			CPPUNIT_ASSERT_EQUAL(v1, a[0]);
			CPPUNIT_ASSERT_EQUAL(v2, a[1]);

			bool errorObserved = false;
			try {
				a.append(v2);
			} catch (Courier::CourierError& e) {
				errorObserved = true;
			}
			CPPUNIT_ASSERT_EQUAL(true, errorObserved);
		}
		{
			int s = 2;
			Courier::SEQUENCE<Courier::CARDINAL> a{s};
			CPPUNIT_ASSERT_EQUAL(s, a.maxSize);

			Courier::CARDINAL v1 = 0x1111;
			Courier::CARDINAL v2 = 0x2222;
			a[0] = v1;
			a[1] = v2;

			CPPUNIT_ASSERT_EQUAL(v1, a[0]);
			CPPUNIT_ASSERT_EQUAL(v2, a[1]);

			{
				bool errorObserved = false;
				try {
					Courier::CARDINAL t = 0x3333;
					a[s + 1] = t;
				} catch (Courier::CourierError& e) {
					errorObserved = true;
				}
				CPPUNIT_ASSERT_EQUAL(true, errorObserved);
			}
			{
				bool errorObserved = false;
				try {
					Courier::CARDINAL t;
					t = a[s + 1];
				} catch (Courier::CourierError& e) {
					errorObserved = true;
				}
				CPPUNIT_ASSERT_EQUAL(true, errorObserved);
			}
		}
		{
			Courier::SEQUENCE<Courier::CARDINAL> a{4};
			CPPUNIT_ASSERT_EQUAL(4, a.maxSize);

			Courier::CARDINAL v1 = 0x1122;
			Courier::CARDINAL v2 = 0x3344;
			a.append(v1);
			a.append(v2);

			CPPUNIT_ASSERT_EQUAL(4, a.maxSize);
			CPPUNIT_ASSERT_EQUAL(2, a.getSize());
			CPPUNIT_ASSERT_EQUAL(v1, a[0]);
			CPPUNIT_ASSERT_EQUAL(v2, a[1]);

			quint8 data[10];
			Courier::BLOCK block(data, sizeof(data));

			Courier::serialize(block, a);
			CPPUNIT_ASSERT_EQUAL((quint16)6, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)6, block.getLimit());
			CPPUNIT_ASSERT_EQUAL((quint8)0x00, data[0]);
			CPPUNIT_ASSERT_EQUAL((quint8)0x02, data[1]);
			CPPUNIT_ASSERT_EQUAL((quint8)0x11, data[2]);
			CPPUNIT_ASSERT_EQUAL((quint8)0x22, data[3]);
			CPPUNIT_ASSERT_EQUAL((quint8)0x33, data[4]);
			CPPUNIT_ASSERT_EQUAL((quint8)0x44, data[5]);
			CPPUNIT_ASSERT_EQUAL((quint8)0x00, data[6]);

			Courier::SEQUENCE<Courier::CARDINAL> b{2};
			block.rewind();
			Courier::deserialize(block, b);
//			CPPUNIT_ASSERT_EQUAL(a.maxSize,   b.maxSize);
			CPPUNIT_ASSERT_EQUAL(a.getSize(), b.getSize());
			CPPUNIT_ASSERT_EQUAL(a[0], b[0]);
			CPPUNIT_ASSERT_EQUAL(a[1], b[1]);
		}
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(testCourier);
