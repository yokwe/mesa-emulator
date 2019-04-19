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
static log4cpp::Category& logger = Logger::getLogger("testStreamOf");

#include "testBase.h"

#include "../stub/T_PRE.h"
#include "../courier/StreamOf.h"
#include "../courier/Last.h"

class testStreamOf : public testBase {
	CPPUNIT_TEST_SUITE(testStreamOf);

	CPPUNIT_TEST(testStreamOfSegmentTypeNextSegment);
	CPPUNIT_TEST(testStreamOfSegmentTypeLastSegment);
	CPPUNIT_TEST(testStreamOfSegmentCARDINAL);
	CPPUNIT_TEST(testStreamOfSegmentREC_PRE03);
	CPPUNIT_TEST(testStreamOfCARDINAL);

	CPPUNIT_TEST_SUITE_END();

public:
	void testStreamOfSegmentTypeNextSegment() {
		using T = Courier::StreamOfSegmentType;
		T value = T::nextSegment;

		{
			QString a = Courier::toString(value);
			QString e = "nextSegment";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}
		{
        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, value);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(2)0000";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}
		{
        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, value);

        	T result;
        	blockData.block.rewind();
        	Courier::deserialize(blockData.block, result);

        	QString a = Courier::toString(result);
        	QString e = Courier::toString(value);

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}
	}
	void testStreamOfSegmentTypeLastSegment() {
		using T = Courier::StreamOfSegmentType;
		T value = T::lastSegment;

		{
			QString a = Courier::toString(value);
			QString e = "lastSegment";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}
		{
        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, value);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(2)0001";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}
		{
        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, value);

        	T result;
        	blockData.block.rewind();
        	Courier::deserialize(blockData.block, result);

        	QString a = Courier::toString(result);
        	QString e = Courier::toString(value);

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}
	}

	void testStreamOfSegmentCARDINAL() {
		using T = Courier::StreamOfSegment<Courier::CARDINAL>;

		{
			T value;
			value.segmentType = Courier::StreamOfSegmentType::lastSegment;
			value.segment = {1, 2, 3};

			QString a = Courier::toString(value);
			QString e = "[lastSegment (3)[1 2 3]]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}
		{
			T value;
			value.segmentType = Courier::StreamOfSegmentType::lastSegment;
			value.segment = {1, 2, 3};

			Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, value);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(10)00010003000100020003";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}
		{
			T value;
			value.segmentType = Courier::StreamOfSegmentType::lastSegment;
			value.segment = {1, 2, 3};

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, value);

        	T result;
        	blockData.block.rewind();
        	Courier::deserialize(blockData.block, result);

        	QString a = Courier::toString(result);
        	QString e = Courier::toString(value);

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}
	}
	void testStreamOfSegmentREC_PRE03() {
		using T = Courier::StreamOfSegment<Courier::T_PRE::REC_PRE03>;

		{
			T value;
			value.segmentType = Courier::StreamOfSegmentType::lastSegment;
			value.segment = {{1}, {2}, {3}};

			QString a = Courier::toString(value);
			QString e = "[lastSegment (3)[[[v 1]] [[v 2]] [[v 3]]]]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}
		{
			T value;
			value.segmentType = Courier::StreamOfSegmentType::lastSegment;
			value.segment = {{1}, {2}, {3}};

			Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, value);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(10)00010003000100020003";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}
		{
			T value;
			value.segmentType = Courier::StreamOfSegmentType::lastSegment;
			value.segment = {{1}, {2}, {3}};

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, value);

        	T result;
        	blockData.block.rewind();
        	Courier::deserialize(blockData.block, result);

        	QString a = Courier::toString(result);
        	QString e = Courier::toString(value);

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}
	}
	void testStreamOfCARDINAL() {
		using T = Courier::StreamOf<Courier::CARDINAL>;

		{
			T value;

			Courier::StreamOfSegment<T::TYPE> e1;
			e1.segmentType = Courier::StreamOfSegmentType::nextSegment;
			e1.segment = {1, 2, 3};
			value.segments.append(e1);

			Courier::StreamOfSegment<T::TYPE> e2;
			e2.segmentType = Courier::StreamOfSegmentType::lastSegment;
			e2.segment = {4, 5, 6};
			value.segments.append(e2);


			QString a = Courier::toString(value);
			QString e = "(2)[[nextSegment (3)[1 2 3]] [lastSegment (3)[4 5 6]]]";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}
		{
			T value;

			Courier::StreamOfSegment<T::TYPE> e1;
			e1.segmentType = Courier::StreamOfSegmentType::nextSegment;
			e1.segment = {1, 2, 3};
			value.segments.append(e1);

			Courier::StreamOfSegment<T::TYPE> e2;
			e2.segmentType = Courier::StreamOfSegmentType::lastSegment;
			e2.segment = {4, 5, 6};
			value.segments.append(e2);

			Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, value);

        	QString a = Courier::toString(blockData.block);
        	QString e = "(20)0000000300010002000300010003000400050006";

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
    	}
		{
			T value;

			Courier::StreamOfSegment<T::TYPE> e1;
			e1.segmentType = Courier::StreamOfSegmentType::nextSegment;
			e1.segment = {1, 2, 3};
			value.segments.append(e1);

			Courier::StreamOfSegment<T::TYPE> e2;
			e2.segmentType = Courier::StreamOfSegmentType::lastSegment;
			e2.segment = {4, 5, 6};
			value.segments.append(e2);

        	Courier::BlockData<100> blockData;
        	Courier::serialize(blockData.block, value);

        	T result;
        	blockData.block.rewind();
        	Courier::deserialize(blockData.block, result);

        	QString a = Courier::toString(result);
        	QString e = Courier::toString(value);

			logger.info("%s %d e = %s", __FILE__, __LINE__, e.toLocal8Bit().constData());
			logger.info("%s %d a = %s", __FILE__, __LINE__, a.toLocal8Bit().constData());
			CPPUNIT_ASSERT_EQUAL(true, QString::compare(e, a) == 0);
		}
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(testStreamOf);
