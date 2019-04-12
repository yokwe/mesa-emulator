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
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONARRUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/


//
// testT_PRE_REC_ARR.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("testT_PRE_REC_ARR");

#include "testBase.h"

#include "../stub/T_PRE.h"

#include "../courier/Last.h"

class testT_PRE_REC_ARR : public testBase {
	CPPUNIT_TEST_SUITE(testT_PRE_REC_ARR);

    CPPUNIT_TEST(testT_PRE_REC_ARR01);
    CPPUNIT_TEST(testT_PRE_REC_ARR02);
    CPPUNIT_TEST(testT_PRE_REC_ARR03);
    CPPUNIT_TEST(testT_PRE_REC_ARR04);
    CPPUNIT_TEST(testT_PRE_REC_ARR05);
    CPPUNIT_TEST(testT_PRE_REC_ARR06);
    CPPUNIT_TEST(testT_PRE_REC_ARR07);
    CPPUNIT_TEST(testT_PRE_REC_ARR08);

	CPPUNIT_TEST_SUITE_END();

public:
    void testT_PRE_REC_ARR01() {
    	using T = Courier::T_PRE::REC_ARR01;

    	// toString
    	{
        	T d;
        	d.v = {true, false, true, false};

        	QString a = Courier::toString(d);
        	QString e = "[[v (4)[T F T F]]]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Serialize
    	{
        	T d;
        	d.v = {true, false, true, false};

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d.v);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(8)0001000000010000";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Deserialize
    	{
        	T d;
        	d.v = {true, false, true, false};

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	T s;
        	blockData.block.rewind();
        	Courier::deserialize(blockData.block, s);

			CPPUNIT_ASSERT_EQUAL(d.v[0], s.v[0]);
			CPPUNIT_ASSERT_EQUAL(d.v[1], s.v[1]);
			CPPUNIT_ASSERT_EQUAL(d.v[2], s.v[2]);
			CPPUNIT_ASSERT_EQUAL(d.v[3], s.v[3]);
    	}
    }
    void testT_PRE_REC_ARR02() {
    	using T = Courier::T_PRE::REC_ARR02;

    	// toString
    	{
        	T d;
        	d.v = {11, 22, 33, 44};

        	QString a = Courier::toString(d);
        	QString e = "[[v (4)[11 22 33 44]]]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Serialize
    	{
        	T d;
        	d.v = {0x11, 0x12, 0x13, 0x14};

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d.v);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(4)11121314";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Deserialize
    	{
        	T d;
        	d.v = {0x11, 0x12, 0x13, 0x14};

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	T s;
        	blockData.block.rewind();
        	Courier::deserialize(blockData.block, s);

        	QString a = Courier::toString(s);
        	QString e = Courier::toString(d);

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);

			CPPUNIT_ASSERT_EQUAL(d.v[0], s.v[0]);
			CPPUNIT_ASSERT_EQUAL(d.v[1], s.v[1]);
			CPPUNIT_ASSERT_EQUAL(d.v[2], s.v[2]);
			CPPUNIT_ASSERT_EQUAL(d.v[3], s.v[3]);
    	}
    }
    void testT_PRE_REC_ARR03() {
    	using T = Courier::T_PRE::REC_ARR03;

    	// toString
    	{
        	T d;
        	d.v = {1111, 2222, 3333, 4444};

        	QString a = Courier::toString(d);
        	QString e = "[[v (4)[1111 2222 3333 4444]]]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Serialize
    	{
        	T d;
        	d.v = {0x1111, 0x1212, 0x1313, 0x1414};

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d.v);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(8)1111121213131414";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Deserialize
    	{
        	T d;
        	d.v = {0x1111, 0x1212, 0x1313, 0x1414};

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	T s;
        	blockData.block.rewind();
        	Courier::deserialize(blockData.block, s);

        	QString a = Courier::toString(s);
        	QString e = Courier::toString(d);

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);

			CPPUNIT_ASSERT_EQUAL(d.v[0], s.v[0]);
			CPPUNIT_ASSERT_EQUAL(d.v[1], s.v[1]);
			CPPUNIT_ASSERT_EQUAL(d.v[2], s.v[2]);
			CPPUNIT_ASSERT_EQUAL(d.v[3], s.v[3]);
    	}
    }
    void testT_PRE_REC_ARR04() {
    	using T = Courier::T_PRE::REC_ARR04;

    	// toString
    	{
        	T d;
        	d.v = {11111111, 22222222, 33333333, 44444444};

        	QString a = Courier::toString(d);
        	QString e = "[[v (4)[11111111 22222222 33333333 44444444]]]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Serialize
    	{
        	T d;
        	d.v = {0x11112222, 0x33334444, 0x55556666, 0x77778888};

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d.v);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(16)11112222333344445555666677778888";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Deserialize
    	{
        	T d;
        	d.v = {0x11111111, 0x12121212, 0x13131313, 0x14141414};

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	T s;
        	blockData.block.rewind();
        	Courier::deserialize(blockData.block, s);

        	QString a = Courier::toString(s);
        	QString e = Courier::toString(d);

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}
    }
    void testT_PRE_REC_ARR05() {
   	using T = Courier::T_PRE::REC_ARR05;

    	// toString
    	{
       	T d;
        	d.v = {QString("11"), QString("12"), QString("13"), QString("14")};

        	QString a = Courier::toString(d);
        	QString e = "[[v (4)[\"11\" \"12\" \"13\" \"14\"]]]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Serialize
    	{
        	T d;
        	d.v = {"11", "12", "13", "14"};

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d.v);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(16)00023131000231320002313300023134";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Deserialize
    	{
        	T d;
        	d.v = {"11", "12", "13", "14"};

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	T s;
        	blockData.block.rewind();
        	Courier::deserialize(blockData.block, s);

        	QString a = Courier::toString(s);
        	QString e = Courier::toString(d);

        	logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}
    }
    void testT_PRE_REC_ARR06() {
    	using T = Courier::T_PRE::REC_ARR06;

    	// toString
    	{
        	T d;
        	d.v = {1111, 2222, 3333, 4444};

        	QString a = Courier::toString(d);
        	QString e = "[[v (4)[1111 2222 3333 4444]]]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Serialize
    	{
        	T d;
        	d.v = {0x1111, 0x1212, 0x1313, 0x1414};

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d.v);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(8)1111121213131414";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Deserialize
    	{
        	T d;
        	d.v = {0x1111, 0x1212, 0x1313, 0x1414};

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	T s;
        	blockData.block.rewind();
        	Courier::deserialize(blockData.block, s);

        	QString a = Courier::toString(s);
        	QString e = Courier::toString(d);

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);

			CPPUNIT_ASSERT_EQUAL(d.v[0], s.v[0]);
			CPPUNIT_ASSERT_EQUAL(d.v[1], s.v[1]);
			CPPUNIT_ASSERT_EQUAL(d.v[2], s.v[2]);
			CPPUNIT_ASSERT_EQUAL(d.v[3], s.v[3]);
    	}
    }
    void testT_PRE_REC_ARR07() {
    	using T = Courier::T_PRE::REC_ARR07;

    	// toString
    	{
        	T d;
        	d.v = {11111111, 22222222, 33333333, 44444444};

        	QString a = Courier::toString(d);
        	QString e = "[[v (4)[11111111 22222222 33333333 44444444]]]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Serialize
    	{
        	T d;
        	d.v = {0x11112222, 0x33334444, 0x55556666, 0x77778888};

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d.v);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(16)11112222333344445555666677778888";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Deserialize
    	{
        	T d;
        	d.v = {0x11111111, 0x12121212, 0x13131313, 0x14141414};

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	T s;
        	blockData.block.rewind();
        	Courier::deserialize(blockData.block, s);

        	QString a = Courier::toString(s);
        	QString e = Courier::toString(d);

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}
    }
    void testT_PRE_REC_ARR08() {
    	using T = Courier::T_PRE::REC_ARR08;

    	// toString
    	{
        	T d;
        	d.v = {11111111111, 22222222222, 33333333333, 44444444444};

        	QString a = Courier::toString(d);
        	QString e = "[[v (4)[11111111111 22222222222 33333333333 44444444444]]]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Serialize
    	{
        	T d;
        	d.v = {0x111122223333U, 0x333344445555U, 0x555566667777U, 0x777788889999U};

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d.v);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(24)111122223333333344445555555566667777777788889999";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Deserialize
    	{
        	T d;
        	d.v = {0x111122223333U, 0x333344445555U, 0x555566667777U, 0x777788889999U};

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	T s;
        	blockData.block.rewind();
        	Courier::deserialize(blockData.block, s);

        	QString a = Courier::toString(s);
        	QString e = Courier::toString(d);

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}
    }

};

CPPUNIT_TEST_SUITE_REGISTRATION(testT_PRE_REC_ARR);
