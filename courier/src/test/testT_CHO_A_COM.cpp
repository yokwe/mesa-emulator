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
// testT_CHO_A_COM.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("testT_CHO_A_COM");

#include "testBase.h"

#include "../stub/T_CHO.h"

#include "../courier/Last.h"

class testT_CHO_A_COM : public testBase {
	CPPUNIT_TEST_SUITE(testT_CHO_A_COM);

    CPPUNIT_TEST(testT_CHO_A_COM01);
    CPPUNIT_TEST(testT_CHO_A_COM02);
    CPPUNIT_TEST(testT_CHO_A_COM03);
    CPPUNIT_TEST(testT_CHO_A_COM04);

	CPPUNIT_TEST_SUITE_END();

public:
    void testT_CHO_A_COM01() {
    	using T = Courier::T_CHO::CHO_A_COM01;

    	// toString
    	{
        	T d;
        	d.choiceTag = T::CHOICE_TAG::a;
        	d.a() = {11111, 22222, 33333, 44444};

        	QString a = Courier::toString(d);
        	QString e = "[a (4)[11111 22222 33333 44444]]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Serialize
    	{
        	T d;
        	d.choiceTag = T::CHOICE_TAG::a;
        	d.a() = {0x1111, 0x2222, 0x3333, 0x4444};

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(10)00001111222233334444";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Deserialize
    	{
        	T d;
        	d.choiceTag = T::CHOICE_TAG::a;
        	d.a() = {0x1111, 0x2222, 0x3333, 0x4444};

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
    void testT_CHO_A_COM02() {
    	using T = Courier::T_CHO::CHO_A_COM02;

    	// toString
    	{
        	T d;
        	d.choiceTag = T::CHOICE_TAG::a;
        	d.a() = {11111, 22222, 33333, 44444};

        	QString a = Courier::toString(d);
        	QString e = "[a (4)[11111 22222 33333 44444]]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}
    	{
        	T d;
        	d.choiceTag = T::CHOICE_TAG::a;
        	d.a() = {11111, 22222, 33333};

        	QString a = Courier::toString(d);
        	QString e = "[a (3)[11111 22222 33333]]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Serialize
    	{
        	T d;
        	d.choiceTag = T::CHOICE_TAG::a;
        	d.a() = {0x1111, 0x2222, 0x3333, 0x4444};

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(12)000000041111222233334444";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}
    	{
        	T d;
        	d.choiceTag = T::CHOICE_TAG::a;
        	d.a() = {0x1111, 0x2222, 0x3333};

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(10)00000003111122223333";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Deserialize
    	{
        	T d;
        	d.choiceTag = T::CHOICE_TAG::a;
        	d.a() = {0x1111, 0x2222, 0x3333, 0x4444};

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
    void testT_CHO_A_COM03() {
    	using T = Courier::T_CHO::CHO_A_COM03;

    	// toString
    	{
        	T d;
        	d.choiceTag = T::CHOICE_TAG::a;
        	d.a() = {11111};

        	QString a = Courier::toString(d);
        	QString e = "[a [[a 11111]]]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Serialize
    	{
        	T d;
        	d.choiceTag = T::CHOICE_TAG::a;
        	d.a() = {0x1111};

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(4)00001111";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Deserialize
    	{
        	T d;
        	d.choiceTag = T::CHOICE_TAG::a;
        	d.a() = {0x1111};

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
    void testT_CHO_A_COM04() {
    	using T = Courier::T_CHO::CHO_A_COM04;

    	// toString
    	{
        	T d;
        	d.choiceTag = T::CHOICE_TAG::a;

        	auto& da = d.a();
        	da.choiceTag = decltype(da.choiceTag)::a;
        	da.a() = 11111;

        	QString a = Courier::toString(d);
        	QString e = "[a [a 11111]]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Serialize
    	{
        	T d;
        	d.choiceTag = T::CHOICE_TAG::a;

        	auto& da = d.a();
        	da.choiceTag = decltype(da.choiceTag)::a;
        	da.a() = 0x1111;

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(6)000000001111";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Deserialize
    	{
        	T d;
        	d.choiceTag = T::CHOICE_TAG::a;

        	auto& da = d.a();
        	da.choiceTag = decltype(da.choiceTag)::a;
        	da.a() = 0x1111;

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

CPPUNIT_TEST_SUITE_REGISTRATION(testT_CHO_A_COM);
