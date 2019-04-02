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
static log4cpp::Category& logger = Logger::getLogger("testBlock");

#include "testBase.h"

class testBlock : public testBase {
	CPPUNIT_TEST_SUITE(testBlock);
	CPPUNIT_TEST(testBlock_);
	CPPUNIT_TEST(testBlock8);
	CPPUNIT_TEST(testBlock16);
	CPPUNIT_TEST(testBlock32);
	CPPUNIT_TEST(testBlock48);
	CPPUNIT_TEST(testBlockQString);
	CPPUNIT_TEST(testBlockBlockSerialize);
	CPPUNIT_TEST(testBlockBlockDeserialize);

	CPPUNIT_TEST(testRemnant);

	CPPUNIT_TEST_SUITE_END();

public:
	void testBlock_() {
		const quint16 capacity = 256;
		quint8 data[capacity];
		for(quint16 i = 0; i < capacity; i++) {
			data[i] = i;
		}
		Courier::BLOCK block(data, sizeof(data));

		CPPUNIT_ASSERT_EQUAL(capacity, block.getCapacity());
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

	void testRemnant() {
		quint8 src_data[100];
		Courier::BLOCK src(src_data, sizeof(src_data));
		CPPUNIT_ASSERT_EQUAL((quint16)0,   src.getOffset());
		CPPUNIT_ASSERT_EQUAL((quint16)0,   src.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)0,   src.getLimit());
		CPPUNIT_ASSERT_EQUAL((quint16)100, src.getCapacity());

		src.serialize16((quint16)0x1111);
		src.serialize16((quint16)0x2222);
		src.serialize16((quint16)0x3333);
		src.serialize16((quint16)0x4444);
		CPPUNIT_ASSERT_EQUAL((quint16)0,   src.getOffset());
		CPPUNIT_ASSERT_EQUAL((quint16)8,   src.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)8,   src.getLimit());
		CPPUNIT_ASSERT_EQUAL((quint16)100, src.getCapacity());

		src.rewind();
		CPPUNIT_ASSERT_EQUAL((quint16)0,   src.getOffset());
		CPPUNIT_ASSERT_EQUAL((quint16)0,   src.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)8,   src.getLimit());
		CPPUNIT_ASSERT_EQUAL((quint16)100, src.getCapacity());

		quint16 t;
		src.deserialize16(t);
		CPPUNIT_ASSERT_EQUAL((quint16)0,   src.getOffset());
		CPPUNIT_ASSERT_EQUAL((quint16)2,   src.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)8,   src.getLimit());
		CPPUNIT_ASSERT_EQUAL((quint16)100, src.getCapacity());

		CPPUNIT_ASSERT_EQUAL((quint16)0x1111, t);

		src.deserialize16(t);
		CPPUNIT_ASSERT_EQUAL((quint16)0,   src.getOffset());
		CPPUNIT_ASSERT_EQUAL((quint16)4,   src.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)8,   src.getLimit());
		CPPUNIT_ASSERT_EQUAL((quint16)100, src.getCapacity());

		CPPUNIT_ASSERT_EQUAL((quint16)0x2222, t);

		Courier::Block rem = src.remnant();
		CPPUNIT_ASSERT_EQUAL((quint16)0,   src.getOffset());
		CPPUNIT_ASSERT_EQUAL((quint16)4,   src.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)8,   src.getLimit());
		CPPUNIT_ASSERT_EQUAL((quint16)100, src.getCapacity());
		CPPUNIT_ASSERT_EQUAL((quint16)4,   rem.getOffset());
		CPPUNIT_ASSERT_EQUAL((quint16)0,   rem.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)4,   rem.getLimit());
		CPPUNIT_ASSERT_EQUAL((quint16)96,  rem.getCapacity());

		rem.deserialize16(t);
		CPPUNIT_ASSERT_EQUAL((quint16)4,   rem.getOffset());
		CPPUNIT_ASSERT_EQUAL((quint16)2,   rem.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)4,   rem.getLimit());
		CPPUNIT_ASSERT_EQUAL((quint16)96,  rem.getCapacity());

		CPPUNIT_ASSERT_EQUAL((quint16)0x3333, t);

		rem.deserialize16(t);
		CPPUNIT_ASSERT_EQUAL((quint16)4,   rem.getOffset());
		CPPUNIT_ASSERT_EQUAL((quint16)4,   rem.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)4,   rem.getLimit());
		CPPUNIT_ASSERT_EQUAL((quint16)96,  rem.getCapacity());

		CPPUNIT_ASSERT_EQUAL((quint16)0x4444, t);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(testBlock);
