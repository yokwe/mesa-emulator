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
// testT_PRE_REC_ALI.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("testT_PRE_REC_ALI");

#include "testBase.h"

#include "../stub/T_PRE.h"
#include "../courier/Last.h"


class testT_PRE_REC_ALI : public testBase {
	CPPUNIT_TEST_SUITE(testT_PRE_REC_ALI);

    CPPUNIT_TEST(testT_PRE_REC_ALI01);
    CPPUNIT_TEST(testT_PRE_REC_ALI02);
    CPPUNIT_TEST(testT_PRE_REC_ALI03);
    CPPUNIT_TEST(testT_PRE_REC_ALI04);
    CPPUNIT_TEST(testT_PRE_REC_ALI05);
    CPPUNIT_TEST(testT_PRE_REC_ALI06);
    CPPUNIT_TEST(testT_PRE_REC_ALI07);
    CPPUNIT_TEST(testT_PRE_REC_ALI08);

	CPPUNIT_TEST_SUITE_END();

public:
    void testT_PRE_REC_ALI01() {
    	using T = Courier::T_PRE::REC_ALI01;

    	// toString
    	{
        	T d;
        	d.v = true;

        	QString a = Courier::toString(d);
        	QString e = "[[v T]]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}
    	{
        	T d;
        	d.v = false;

        	QString a = Courier::toString(d);
        	QString e = "[[v F]]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Serialize
    	{
        	T d;
        	d.v = true;

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d.v);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(2)0001";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}
    	{
        	T d;
        	d.v = false;

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(2)0000";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Deserialize
    	{
        	T d;
        	d.v = false;

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	T s;
        	blockData.block.rewind();
        	Courier::deserialize(blockData.block, s);

			CPPUNIT_ASSERT_EQUAL(d.v, s.v);
    	}
    	{
        	T d;
        	d.v = true;

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	T s;
        	blockData.block.rewind();
        	Courier::deserialize(blockData.block, s);

			CPPUNIT_ASSERT_EQUAL(d.v, s.v);
    	}
    }
    void testT_PRE_REC_ALI02() {
    	using T = Courier::T_PRE::REC_ALI02;

    	// toString
    	{
        	T d;
        	d.v = 11;

        	QString a = Courier::toString(d);
        	QString e = "[[v 11]]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Serialize
    	{
        	T d;
        	d.v = 0x11;

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(1)11";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Deserialize
    	{
        	T d;
        	d.v = 0x11;

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	T s;
        	blockData.block.rewind();
        	Courier::deserialize(blockData.block, s);

			CPPUNIT_ASSERT_EQUAL(d.v, s.v);
    	}
    }
    void testT_PRE_REC_ALI03() {
    	using T = Courier::T_PRE::REC_ALI03;

    	// toString
    	{
        	T d;
        	d.v = 1111;

        	QString a = Courier::toString(d);
        	QString e = "[[v 1111]]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Serialize
    	{
        	T d;
        	d.v = 0x1122;

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(2)1122";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Deserialize
    	{
        	T d;
        	d.v = 0x1122;

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	T s;
        	blockData.block.rewind();
        	Courier::deserialize(blockData.block, s);

			CPPUNIT_ASSERT_EQUAL(d.v, s.v);
    	}
    }
    void testT_PRE_REC_ALI04() {
    	using T = Courier::T_PRE::REC_ALI04;

    	// toString
    	{
        	T d;
        	d.v = 11112222;

        	QString a = Courier::toString(d);
        	QString e = "[[v 11112222]]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Serialize
    	{
        	T d;
        	d.v = 0x11223344;

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(4)11223344";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Deserialize
    	{
        	T d;
        	d.v = 0x11223344;

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	T s;
        	blockData.block.rewind();
        	Courier::deserialize(blockData.block, s);

			CPPUNIT_ASSERT_EQUAL(d.v, s.v);
    	}
    }
    void testT_PRE_REC_ALI05() {
    	using T = Courier::T_PRE::REC_ALI05;

    	// toString
    	{
        	T d;
        	d.v = "ABCD";

        	QString a = Courier::toString(d);
        	QString e = QString("[[v \"ABCD\"]]");

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Serialize
    	{
        	T d;
        	d.v = QString("ABCD");

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(6)000441424344";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}
    	{
        	T d;
        	d.v = QString("ABC");

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(6)000341424300";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Deserialize
    	{
        	T d;
        	d.v = "ABCD";

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	T s;
        	blockData.block.rewind();
        	Courier::deserialize(blockData.block, s);

			logger.info("%s %d d.v = %s", __FILE__, __LINE__, d.v.toLocal8Bit().constData());
			logger.info("%s %d s.v = %s", __FILE__, __LINE__, s.v.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(d.v, s.v) == 0);
    	}
    }
    void testT_PRE_REC_ALI06() {
    	using T = Courier::T_PRE::REC_ALI06;

    	// toString
    	{
        	T d;
        	d.v = 1111;

        	QString a = Courier::toString(d);
        	QString e = "[[v 1111]]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Serialize
    	{
        	T d;
        	d.v = 0x1122;

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(2)1122";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Deserialize
    	{
        	T d;
        	d.v = 0x1122;

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	T s;
        	blockData.block.rewind();
        	Courier::deserialize(blockData.block, s);

			CPPUNIT_ASSERT_EQUAL(d.v, s.v);
    	}
    }
    void testT_PRE_REC_ALI07() {
    	using T = Courier::T_PRE::REC_ALI07;

    	// toString
    	{
        	T d;
        	d.v = 11112222;

        	QString a = Courier::toString(d);
        	QString e = "[[v 11112222]]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Serialize
    	{
        	T d;
        	d.v = 0x11223344;

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(4)11223344";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Deserialize
    	{
        	T d;
        	d.v = 0x11223344;

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	T s;
        	blockData.block.rewind();
        	Courier::deserialize(blockData.block, s);

			CPPUNIT_ASSERT_EQUAL(d.v, s.v);
    	}
    }
    void testT_PRE_REC_ALI08() {
    	using T = Courier::T_PRE::REC_ALI08;

    	// toString
    	{
        	T d;
        	d.v = 111122223333;

        	QString a = Courier::toString(d);
        	QString e = "[[v 111122223333]]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Serialize
    	{
        	T d;
        	d.v = 0x112233445566;

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(6)112233445566";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Deserialize
    	{
        	T d;
        	d.v = 0x112233445566;

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	T s;
        	blockData.block.rewind();
        	Courier::deserialize(blockData.block, s);

			CPPUNIT_ASSERT_EQUAL(d.v, s.v);
    	}
    }

};

CPPUNIT_TEST_SUITE_REGISTRATION(testT_PRE_REC_ALI);
