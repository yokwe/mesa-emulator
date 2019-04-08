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
// testT_CHO_A_PRE.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("testT_CHO_A_PRE");

#include "testBase.h"

#include "../stub/T_CHO.h"

class testT_CHO_A_PRE : public testBase {
	CPPUNIT_TEST_SUITE(testT_CHO_A_PRE);

    CPPUNIT_TEST(testT_CHO_A_PRE01);
    CPPUNIT_TEST(testT_CHO_A_PRE02);
    CPPUNIT_TEST(testT_CHO_A_PRE03);
    CPPUNIT_TEST(testT_CHO_A_PRE04);
    CPPUNIT_TEST(testT_CHO_A_PRE05);
    CPPUNIT_TEST(testT_CHO_A_PRE06);
    CPPUNIT_TEST(testT_CHO_A_PRE07);
    CPPUNIT_TEST(testT_CHO_A_PRE08);

	CPPUNIT_TEST_SUITE_END();

public:
    void testT_CHO_A_PRE01() {
    	using T = Courier::T_CHO::CHO_A_PRE01;

    	// toString
    	{
        	T d;
        	d.choiceTag = T::CHOICE_TAG::a;
        	d.a() = true;

        	QString a = Courier::toString(d);
        	QString e = "[a T]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}
    	{
        	T d;
        	d.choiceTag = T::CHOICE_TAG::a;
        	d.a() = false;

        	QString a = Courier::toString(d);
        	QString e = "[a F]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Serialize
    	{
        	T d;
        	d.choiceTag = T::CHOICE_TAG::a;
        	d.a() = true;

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(4)00000001";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}
    	{
        	T d;
        	d.choiceTag = T::CHOICE_TAG::a;
        	d.a() = false;

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, d);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(4)00000000";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}

    	// Deserialize
    	{
        	T d;
        	d.choiceTag = T::CHOICE_TAG::a;
        	d.a() = true;

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
    	{
        	T d;
        	d.choiceTag = T::CHOICE_TAG::a;
        	d.a() = false;

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
    void testT_CHO_A_PRE02() {}
    void testT_CHO_A_PRE03() {}
    void testT_CHO_A_PRE04() {}
    void testT_CHO_A_PRE05() {}
    void testT_CHO_A_PRE06() {}
    void testT_CHO_A_PRE07() {}
    void testT_CHO_A_PRE08() {}

};

CPPUNIT_TEST_SUITE_REGISTRATION(testT_CHO_A_PRE);
