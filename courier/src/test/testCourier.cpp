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
	CPPUNIT_TEST(testSTRING);
	CPPUNIT_TEST(testENUM);

	CPPUNIT_TEST(testARRAY);
	CPPUNIT_TEST(testSEQUENCE);

	CPPUNIT_TEST_SUITE_END();

public:
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
			block.serialize(a);
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
			block.deserialize(b);
			CPPUNIT_ASSERT_EQUAL(v.toStdString(), ((QString)a).toStdString());
			CPPUNIT_ASSERT_EQUAL((quint16)4, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)4, block.getLimit());
		}
	}
	void testENUM() {
		quint8 data[100];
		Courier::BLOCK block(data, sizeof(data));

		enum class ABC : quint16 {A = 0x11, B = 0x22, C = 0x33};
		ABC v = ABC::A;
		quint8 v1 = (quint8)(((quint16)v) << 8);
		quint8 v2 = (quint8)(((quint16)v) << 0);

		{
			block.zero();
			block.clear();

			ABC a(v);
			CPPUNIT_ASSERT_EQUAL(v, a);
			CPPUNIT_ASSERT_EQUAL((quint8)0, data[0]);
			CPPUNIT_ASSERT_EQUAL((quint16)0, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)0, block.getLimit());
			{
				quint16 t = (quint16)a;
				block.serialize16(t);
			}
			CPPUNIT_ASSERT_EQUAL(v1, data[0]);
			CPPUNIT_ASSERT_EQUAL(v2, data[1]);
			CPPUNIT_ASSERT_EQUAL((quint16)2, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)2, block.getLimit());

			block.rewind();
			ABC b;
			CPPUNIT_ASSERT_EQUAL((quint16)0, (quint16)b);
			CPPUNIT_ASSERT_EQUAL((quint16)0, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)2, block.getLimit());
			{
				quint16 t;
				block.deserialize16(t);
				b = (ABC)t;
			}

			CPPUNIT_ASSERT_EQUAL(v, b);
			CPPUNIT_ASSERT_EQUAL((quint16)2, block.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)2, block.getLimit());
		}
	}
	void testARRAY() {
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
					if (t == 0) RUNTIME_ERROR();
				} catch (Courier::CourierError& e) {
					errorObserved = true;
				}
				CPPUNIT_ASSERT_EQUAL(true, errorObserved);
			}
		}
//		{
//			Courier::ARRAY<Courier::CARDINAL> a{2};
//			CPPUNIT_ASSERT_EQUAL(2, a.maxSize);
//
//			Courier::CARDINAL v1 = 0x1122;
//			Courier::CARDINAL v2 = 0x3344;
//			a[0] = v1;
//			a[1] = v2;
//
//			CPPUNIT_ASSERT_EQUAL(v1, a[0]);
//			CPPUNIT_ASSERT_EQUAL(v2, a[1]);
//
//			quint8 data[6];
//			Courier::BLOCK block(data, sizeof(data));
//
//			a.serialize(block);
//
//			CPPUNIT_ASSERT_EQUAL((quint16)4, block.getPos());
//			CPPUNIT_ASSERT_EQUAL((quint16)4, block.getLimit());
//			CPPUNIT_ASSERT_EQUAL((quint8)0x11, data[0]);
//			CPPUNIT_ASSERT_EQUAL((quint8)0x22, data[1]);
//			CPPUNIT_ASSERT_EQUAL((quint8)0x33, data[2]);
//			CPPUNIT_ASSERT_EQUAL((quint8)0x44, data[3]);
//			CPPUNIT_ASSERT_EQUAL((quint8)0x00, data[4]);
//
//			Courier::ARRAY<Courier::CARDINAL> b{2};
//			block.rewind();
//			b.deserialize(block);
//			CPPUNIT_ASSERT_EQUAL(a.maxSize, b.maxSize);
//			CPPUNIT_ASSERT_EQUAL(a[0], b[0]);
//			CPPUNIT_ASSERT_EQUAL(a[1], b[1]);
//		}
	}
	void testSEQUENCE() {
		{
			int s = 2;
			Courier::SEQUENCE<Courier::CARDINAL> a{s};
			CPPUNIT_ASSERT_EQUAL(s, a.maxSize);

			{
				quint16 newValue = 1;
				a.setSize(newValue);
				CPPUNIT_ASSERT_EQUAL(newValue, a.getSize());
			}
			{
				bool errorObserved = false;
				try {
					a.setSize(99);
				} catch (Courier::CourierError& e) {
					errorObserved = true;
				}
				CPPUNIT_ASSERT_EQUAL(true, errorObserved);
			}

			Courier::CARDINAL v1 = 0x1111;
			Courier::CARDINAL v2 = 0x2222;
			a[0] = v1;
			a[1] = v2;
			a.setSize(s);

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
					if (t == 0) {
						logger.error("Unexpected then");
					} else {
						logger.error("Unexpected else");
					}
				} catch (Courier::CourierError& e) {
					errorObserved = true;
				}
				CPPUNIT_ASSERT_EQUAL(true, errorObserved);
			}
		}
//		{
//			Courier::SEQUENCE<Courier::CARDINAL> a{4};
//			CPPUNIT_ASSERT_EQUAL(4, a.maxSize);
//
//			Courier::CARDINAL v1 = 0x1122;
//			Courier::CARDINAL v2 = 0x3344;
//			a.append(v1);
//			a.append(v2);
//
//			CPPUNIT_ASSERT_EQUAL(4, a.maxSize);
//			CPPUNIT_ASSERT_EQUAL(2, a.getSize());
//			CPPUNIT_ASSERT_EQUAL(v1, a[0]);
//			CPPUNIT_ASSERT_EQUAL(v2, a[1]);
//
//			quint8 data[10];
//			Courier::BLOCK block(data, sizeof(data));
//
//			a.serialize(block);
//			CPPUNIT_ASSERT_EQUAL((quint16)6, block.getPos());
//			CPPUNIT_ASSERT_EQUAL((quint16)6, block.getLimit());
//			CPPUNIT_ASSERT_EQUAL((quint8)0x00, data[0]);
//			CPPUNIT_ASSERT_EQUAL((quint8)0x02, data[1]);
//			CPPUNIT_ASSERT_EQUAL((quint8)0x11, data[2]);
//			CPPUNIT_ASSERT_EQUAL((quint8)0x22, data[3]);
//			CPPUNIT_ASSERT_EQUAL((quint8)0x33, data[4]);
//			CPPUNIT_ASSERT_EQUAL((quint8)0x44, data[5]);
//			CPPUNIT_ASSERT_EQUAL((quint8)0x00, data[6]);
//
//			Courier::SEQUENCE<Courier::CARDINAL> b{2};
//			block.rewind();
//			b.deserialize(block);
////			CPPUNIT_ASSERT_EQUAL(a.maxSize,   b.maxSize);
//			CPPUNIT_ASSERT_EQUAL(a.getSize(), b.getSize());
//			CPPUNIT_ASSERT_EQUAL(a[0], b[0]);
//			CPPUNIT_ASSERT_EQUAL(a[1], b[1]);
//		}
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(testCourier);
