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

#include "../stub/Ethernet.h"
#include "../stub/Time.h"

#include "../courier/Last.h"

class testCourier : public testBase {
	CPPUNIT_TEST_SUITE(testCourier);

	CPPUNIT_TEST(testBlock);
	CPPUNIT_TEST(testQuint8);
	CPPUNIT_TEST(testBool);
	CPPUNIT_TEST(testQString);
	CPPUNIT_TEST(testQuint16);
	CPPUNIT_TEST(testQuint32);
	CPPUNIT_TEST(testQuint64);
	CPPUNIT_TEST(testSEQUENCE);
	CPPUNIT_TEST(testARRAY);

	CPPUNIT_TEST_SUITE_END();

public:
#define CPPUNIT_ASSERT_EQUAL_QSTRING(a,b) CPPUNIT_ASSERT_EQUAL((a).toStdString(), ((QString)b).toStdString())

#define BLOCK_DATA(n,s) quint8 data_##n[s]; Courier::Block n(&data_##n[0], (quint16)(sizeof(data_##n)));
	void testBlock() {
		// toString
		{
			BLOCK_DATA(b,100)
			b.serialize8(0x11);
			b.serialize8(0x22);
			
			QString a = b.toString();
			QString e = "(2)1122";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}
		// serialize
		{
			BLOCK_DATA(v,100)
			v.serialize8(0x11);
			v.serialize8(0x22);
			v.rewind();
			quint8 c;
			v.deserialize8(c);

			BLOCK_DATA(t,100)
			Courier::serialize(t, (quint8)0x99);
			Courier::serialize(t, v);
			
			QString a = t.toString();
			QString e = "(3)991122";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}
		// deserialize
		{
			BLOCK_DATA(v,100)
			v.serialize8(0x11);
			v.serialize8(0x22);
			v.serialize8(0x33);
			v.rewind();

			quint8 c;
			Courier::deserialize(v, c);
			CPPUNIT_ASSERT_EQUAL((quint8)0x11, c);
			BLOCK_DATA(t,100)
			Courier::deserialize(v, t);
			
			QString a = t.toString();
			QString e = "(2)2233";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}
	}

	void testQuint8() {
		// toString
		{
			{
				quint8 t = 123;
				
				QString a = Courier::toString(t);
				QString e = "123";

				logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
				logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
				CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
			}
			{
				quint8 t = 255;
				
				QString a = Courier::toString(t);
				QString e = "0xFF";

				logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
				logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
				CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
			}
			{
				quint8 t = 254;
				
				QString a = Courier::toString(t);
				QString e = "0xFE";

				logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
				logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
				CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
			}
		}
		// serialize
		{
			BLOCK_DATA(t,100)
			Courier::serialize(t, (quint8)0x12);
			Courier::serialize(t, (quint8)0x34);
			
			QString a = t.toString();
			QString e = "(2)1234";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}
		// deserialize
		{
			BLOCK_DATA(t,100)
			Courier::serialize(t, (quint8)0x12);
			Courier::serialize(t, (quint8)0x34);
			t.rewind();

			quint8 c;
			Courier::deserialize(t, c);
			CPPUNIT_ASSERT_EQUAL((quint8)0x12, c);

			Courier::deserialize(t, c);
			CPPUNIT_ASSERT_EQUAL((quint8)0x34, c);
		}
	}

	void testBool() {
		// toString
		{
			{
				bool t = true;
				
				QString a = Courier::toString(t);
				QString e = "T";

				logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
				logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
				CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
			}
			{
				bool t = false;
				
				QString a = Courier::toString(t);
				QString e = "F";

				logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
				logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
				CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
			}
		}
		// serialize
		{
			bool v1 = false;
			bool v2 = true;
			BLOCK_DATA(t,100)
			Courier::serialize(t, v1);
			Courier::serialize(t, v2);
			
			QString a = t.toString();
			QString e = "(4)00000001";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}
		// deserialize
		{
			bool v1 = false;
			bool v2 = true;
			BLOCK_DATA(t,100)
			Courier::serialize(t, v1);
			Courier::serialize(t, v2);
			t.rewind();

			bool c;
			Courier::deserialize(t, c);
			CPPUNIT_ASSERT_EQUAL(v1, c);

			Courier::deserialize(t, c);
			CPPUNIT_ASSERT_EQUAL(v2, c);
		}
	}

	void testQString() {
		// toString
		{
			QString t = "abc";
			
			QString a = Courier::toString(t);
			QString e("\"abc\"");

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}
		// serialize
		{
			QString v1 = "ab";
			QString v2 = "cde";
			BLOCK_DATA(t,100)
			CPPUNIT_ASSERT_EQUAL((quint16)0, t.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)0, t.getLimit());

			Courier::serialize(t, v1);
			CPPUNIT_ASSERT_EQUAL((quint16)4, t.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)4, t.getLimit());

			Courier::serialize(t, v2);
			CPPUNIT_ASSERT_EQUAL((quint16)10, t.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)10, t.getLimit());

			QString a = t.toString();
			QString e = "(10)00026162000363646500";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}
		// deserialize
		{
			QString v1 = "ab";
			QString v2 = "def";
			BLOCK_DATA(t,100)
			CPPUNIT_ASSERT_EQUAL((quint16)0, t.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)0, t.getLimit());

			Courier::serialize(t, v1);
			CPPUNIT_ASSERT_EQUAL((quint16)4, t.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)4, t.getLimit());

			Courier::serialize(t, v2);
			CPPUNIT_ASSERT_EQUAL((quint16)10, t.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)10, t.getLimit());
			t.rewind();

			QString a;
			QString e;
			Courier::deserialize(t, a);
			e = v1;
			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);

			Courier::deserialize(t, a);
			e = v2;
			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}
	}

	void testQuint16() {
		// toString
		{
			{
				quint16 t = 12345;
				
				QString a = Courier::toString(t);
				QString e = "12345";

				logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
				logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
				CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
			}
			{
				quint16 t = 0xFFFFU;
				
				QString a = Courier::toString(t);
				QString e = "0xFFFF";

				logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
				logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
				CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
			}
			{
				quint16 t = 0xFFFEU;
				
				QString a = Courier::toString(t);
				QString e = "0xFFFE";

				logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
				logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
				CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
			}
		}
		// serialize
		{
			BLOCK_DATA(t,100)
			Courier::serialize(t, (quint16)0x1234);
			Courier::serialize(t, (quint16)0x5678);
			
			QString a = t.toString();
			QString e("(4)12345678");

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}
		// deserialize
		{
			BLOCK_DATA(t,100)
			Courier::serialize(t, (quint16)0x1234);
			Courier::serialize(t, (quint16)0x5678);
			t.rewind();

			quint16 c;
			Courier::deserialize(t, c);
			CPPUNIT_ASSERT_EQUAL((quint16)0x1234, c);

			Courier::deserialize(t, c);
			CPPUNIT_ASSERT_EQUAL((quint16)0x5678, c);
		}
	}

	void testQuint32() {
		// toString
		{
			{
				quint32 t = 12345678;
				
				QString a = Courier::toString(t);
				QString e = "12345678";

				logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
				logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
				CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
			}
			{
				quint32 t = 0xFFFFFFFFU;
				
				QString a = Courier::toString(t);
				QString e = "0xFFFFFFFF";

				logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
				logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
				CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
			}
			{
				quint32 t = 0xFFFFFFFEU;
				
				QString a = Courier::toString(t);
				QString e = "0xFFFFFFFE";

				logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
				logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
				CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
			}
		}
		// serialize
		{
			BLOCK_DATA(t,100)
			Courier::serialize(t, (quint32)0x12345678);
			Courier::serialize(t, (quint32)0x9ABCDEF0);
			
			QString a = t.toString();
			QString e = "(8)123456789ABCDEF0";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}
		// deserialize
		{
			BLOCK_DATA(t,100)
			Courier::serialize(t, (quint32)0x12345678);
			Courier::serialize(t, (quint32)0x9ABCDEF0);
			t.rewind();

			quint32 c;
			Courier::deserialize(t, c);
			CPPUNIT_ASSERT_EQUAL((quint32)0x12345678, c);

			Courier::deserialize(t, c);
			CPPUNIT_ASSERT_EQUAL((quint32)0x9ABCDEF0, c);
		}
	}

	void testQuint64() {
		// toString
		{
			{
				quint64 t = 1234567890123;
				
				QString a = Courier::toString(t);
				QString e = "1234567890123";

				logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
				logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
				CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
			}
			{
				quint64 t = 0xFFFFFFFFFFFFU;
				
				QString a = Courier::toString(t);
				QString e = "0xFFFFFFFFFFFF";

				logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
				logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
				CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
			}
			{
				quint64 t = 0xFFFFFFFFFFFEU;
				
				QString a = Courier::toString(t);
				QString e = "0xFFFFFFFFFFFE";

				logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
				logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
				CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
			}
		}
		// serialize
		{
			quint64 v1 = 0x123456789ABC;
			quint64 v2 = 0xDEF012345678;

			BLOCK_DATA(t,100)
			Courier::serialize(t, v1);
			Courier::serialize(t, v2);
			
			QString a = t.toString();
			QString e = "(12)123456789ABCDEF012345678";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}
		// deserialize
		{
			quint64 v1 = 0x123456789ABC;
			quint64 v2 = 0xDEF012345678;

			BLOCK_DATA(t,100)
			Courier::serialize(t, v1);
			Courier::serialize(t, v2);
			t.rewind();

			quint64 c;
			Courier::deserialize(t, c);
			CPPUNIT_ASSERT_EQUAL(v1, c);

			Courier::deserialize(t, c);
			CPPUNIT_ASSERT_EQUAL(v2, c);
		}
	}

	void testSEQUENCE() {
		{
			Courier::SEQUENCE_N<quint16, 10> t;
			CPPUNIT_ASSERT_EQUAL((quint16)10, t.getCapacity());
			CPPUNIT_ASSERT_EQUAL((quint16)10, t.getSize());

			t.setSize(2);

			CPPUNIT_ASSERT_EQUAL((quint16)10, t.getCapacity());
			CPPUNIT_ASSERT_EQUAL((quint16)2, t.getSize());
		}
		{
			Courier::SEQUENCE_N<quint16> t;
			t.setSize(2);

			CPPUNIT_ASSERT_EQUAL((quint16)65535, t.getCapacity());
			CPPUNIT_ASSERT_EQUAL((quint16)2, t.getSize());
		}

		// toString
		{
			quint16 v1 = 1234;
			quint16 v2 = 5678;
			Courier::SEQUENCE_N<quint16, 10> t;
			t.setSize(2);
			t[0] = v1;
			t[1] = v2;

			QString a = Courier::toString(t);
			QString e = "(2)[1234 5678]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}
		// toString
		{
			quint16 v1 = 1234;
			quint16 v2 = 5678;
			Courier::SEQUENCE_N<quint16> t;
			t.setSize(2);
			t[0] = v1;
			t[1] = v2;

			QString a = Courier::toString(t);
			QString e = "(2)[1234 5678]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}
		// serialize
		{
			quint16 v1 = 0x1234;
			quint16 v2 = 0x5678;
			Courier::SEQUENCE_N<quint16, 10> t;
			t.setSize(2);
			t[0] = v1;
			t[1] = v2;

			BLOCK_DATA(b,100)
			CPPUNIT_ASSERT_EQUAL((quint16)0, b.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)0, b.getLimit());

			Courier::serialize(b, t);
			CPPUNIT_ASSERT_EQUAL((quint16)6, b.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)6, b.getLimit());

			QString a = b.toString();
			QString e = "(6)000212345678";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}

		// deserialize
		{
			quint16 v1 = 1234;
			quint16 v2 = 5678;
			Courier::SEQUENCE_N<quint16, 10> t;
			t.setSize(2);
			t[0] = v1;
			t[1] = v2;

			BLOCK_DATA(b,100)
			Courier::serialize(b, t);
			b.rewind();

			Courier::SEQUENCE_N<quint16, 10> u;
			Courier::deserialize(b, u);

			QString a = Courier::toString(u);
			QString e = Courier::toString(t);

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}

		// initializer list
		{
			using A = Courier::SEQUENCE_N<quint16>;
			A t = {1, 2, 3};
			CPPUNIT_ASSERT_EQUAL((quint16)65535, t.getCapacity());
			CPPUNIT_ASSERT_EQUAL((quint16)3, t.getSize());

			QString a = Courier::toString(t);
			QString e = "(3)[1 2 3]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}

		// initializer list
		{
			using A = Courier::SEQUENCE_N<quint16, 10>;
			A t = {1, 2, 3};
			CPPUNIT_ASSERT_EQUAL((quint16)10, t.getCapacity());
			CPPUNIT_ASSERT_EQUAL((quint16)3,  t.getSize());

			QString a = Courier::toString(t);
			QString e = "(3)[1 2 3]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}

		// FIXME
		// each method of ARRAY
	}

	void testARRAY() {
		{
			Courier::ARRAY_N<quint16, 10> t;

			CPPUNIT_ASSERT_EQUAL((quint16)10, t.getCapacity());
			CPPUNIT_ASSERT_EQUAL((quint16)10, t.getSize());
		}

		// toString
		{
			quint16 v1 = 1234;
			quint16 v2 = 5678;
			Courier::ARRAY_N<quint16, 2> t;
			t[0] = v1;
			t[1] = v2;

			QString a = Courier::toString(t);
			QString e = "(2)[1234 5678]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}
		// serialize
		{
			quint16 v1 = 0x1234;
			quint16 v2 = 0x5678;
			Courier::ARRAY_N<quint16, 2> t;
			t[0] = v1;
			t[1] = v2;

			BLOCK_DATA(b,100)
			CPPUNIT_ASSERT_EQUAL((quint16)0, b.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)0, b.getLimit());

			Courier::serialize(b, t);
			CPPUNIT_ASSERT_EQUAL((quint16)4, b.getPos());
			CPPUNIT_ASSERT_EQUAL((quint16)4, b.getLimit());

			QString a = b.toString();
			QString e("(4)12345678");

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}

		// deserialize
		{
			quint16 v1 = 1234;
			quint16 v2 = 5678;
			Courier::ARRAY_N<quint16, 2> t;
			t[0] = v1;
			t[1] = v2;

			BLOCK_DATA(b,100)
			Courier::serialize(b, t);
			b.rewind();

			Courier::ARRAY_N<quint16, 2> u;
			Courier::deserialize(b, u);

			QString a = Courier::toString(u);
			QString e = Courier::toString(t);

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}

		// initializer list
		{
			using A = Courier::ARRAY_N<quint16, 10>;
			A t = {1, 2, 3};
			CPPUNIT_ASSERT_EQUAL((quint16)10, t.getCapacity());

			QString a = Courier::toString(t);
			QString e = "(10)[1 2 3 0 0 0 0 0 0 0]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}

		// FIXME
		// each method of ARRAY
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION(testCourier);
