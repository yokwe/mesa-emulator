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
// testT_SEQ_ALI.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("testT_SEQ_ALI");

#include "testBase.h"

#include "../stub/T_SEQ.h"

#include "../courier/Last.h"

class testT_SEQ_ALI : public testBase {
	CPPUNIT_TEST_SUITE(testT_SEQ_ALI);

    CPPUNIT_TEST(testT_SEQ_ALI01);
    CPPUNIT_TEST(testT_SEQ_ALI02);
    CPPUNIT_TEST(testT_SEQ_ALI03);
    CPPUNIT_TEST(testT_SEQ_ALI04);
    CPPUNIT_TEST(testT_SEQ_ALI05);

	CPPUNIT_TEST_SUITE_END();

public:
    void testT_SEQ_ALI01() {
    	using T = Courier::Stub::T_SEQ::SEQ_ALI01;

    	// toString
    	{
        	T d;
        	d = {T::TYPE::a, T::TYPE::b, T::TYPE::c, T::TYPE::d};

        	QString a = Courier::toString(d);
        	QString e = "(4)[a b c d]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Serialize
    	{
        	T d;
        	d = {T::TYPE::a, T::TYPE::b, T::TYPE::c, T::TYPE::d};

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(10)00040001000200030004";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Deserialize
    	{
        	T d;
        	d = {T::TYPE::a, T::TYPE::b, T::TYPE::c, T::TYPE::d};

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
    void testT_SEQ_ALI02() {
    	using T = Courier::Stub::T_SEQ::SEQ_ALI02;

    	// toString
    	{
        	T d;
        	d = {{11, 12, 13, 14}, {21, 22, 23, 24}, {31, 32, 33, 34}, {41, 42, 43, 44}};

        	QString a = Courier::toString(d);
        	QString e = "(4)[(4)[11 12 13 14] (4)[21 22 23 24] (4)[31 32 33 34] (4)[41 42 43 44]]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Serialize
    	{
        	T d;
        	d = {{0x11, 0x12, 0x13, 0x14}, {0x21, 0x22, 0x23, 0x24}, {0x31, 0x32, 0x33, 0x34}, {0x41, 0x42, 0x43, 0x44}};

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(34)00040011001200130014002100220023002400310032003300340041004200430044";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Deserialize
    	{
        	T d;
        	d = {{0x11, 0x12, 0x13, 0x14}, {0x21, 0x22, 0x23, 0x24}, {0x31, 0x32, 0x33, 0x34}, {0x41, 0x42, 0x43, 0x44}};

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
    void testT_SEQ_ALI03() {
    	using T = Courier::Stub::T_SEQ::SEQ_ALI03;

    	// toString
    	{
        	T d;
        	d = {{11, 12, 13, 14}, {21, 22, 23, 24}, {31, 32, 33, 34}, {41, 42, 43, 44}};

        	QString a = Courier::toString(d);
        	QString e = "(4)[(4)[11 12 13 14] (4)[21 22 23 24] (4)[31 32 33 34] (4)[41 42 43 44]]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Serialize
    	{
        	T d;
        	d = {{0x11, 0x12, 0x13, 0x14}, {0x21, 0x22, 0x23, 0x24}, {0x31, 0x32, 0x33, 0x34}, {0x41, 0x42, 0x43, 0x44}};

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(42)000400040011001200130014000400210022002300240004003100320033003400040041004200430044";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Deserialize
    	{
        	T d;
        	d = {{0x11, 0x12, 0x13, 0x14}, {0x21, 0x22, 0x23, 0x24}, {0x31, 0x32, 0x33, 0x34}, {0x41, 0x42, 0x43, 0x44}};

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
    void testT_SEQ_ALI04() {
    	using T = Courier::Stub::T_SEQ::SEQ_ALI04;

    	// toString
    	{
        	T d;
        	d = {{1}, {2}, {3}, {4}};

        	QString a = Courier::toString(d);
        	QString e = "(4)[[[a 1]] [[a 2]] [[a 3]] [[a 4]]]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Serialize
    	{
        	T d;
        	d = {{1}, {2}, {3}, {4}};

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(10)00040001000200030004";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Deserialize
    	{
        	T d;
        	d = {{1}, {2}, {3}, {4}};

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
    void testT_SEQ_ALI05() {
    	using T = Courier::Stub::T_SEQ::SEQ_ALI05;

    	// toString
    	{
        	T d;
        	d[0].choiceTag = decltype(d[0].choiceTag)::a;
        	d[0].a() = {1};
        	d[1].choiceTag = decltype(d[0].choiceTag)::a;
        	d[1].a() = {2};
        	d[2].choiceTag = decltype(d[0].choiceTag)::a;
        	d[2].a() = {3};
        	d[3].choiceTag = decltype(d[0].choiceTag)::a;
        	d[3].a() = {4};


        	QString a = Courier::toString(d);
        	QString e = "(4)[[a 1] [a 2] [a 3] [a 4]]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Serialize
    	{
        	T d;
        	d[0].choiceTag = decltype(d[0].choiceTag)::a;
        	d[0].a() = {1};
        	d[1].choiceTag = decltype(d[0].choiceTag)::a;
        	d[1].a() = {2};
        	d[2].choiceTag = decltype(d[0].choiceTag)::a;
        	d[2].a() = {3};
        	d[3].choiceTag = decltype(d[0].choiceTag)::a;
        	d[3].a() = {4};

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(18)000400000001000000020000000300000004";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Deserialize
    	{
        	T d;
        	d[0].choiceTag = decltype(d[0].choiceTag)::a;
        	d[0].a() = {1};
        	d[1].choiceTag = decltype(d[0].choiceTag)::a;
        	d[1].a() = {2};
        	d[2].choiceTag = decltype(d[0].choiceTag)::a;
        	d[2].a() = {3};
        	d[3].choiceTag = decltype(d[0].choiceTag)::a;
        	d[3].a() = {4};

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

CPPUNIT_TEST_SUITE_REGISTRATION(testT_SEQ_ALI);
