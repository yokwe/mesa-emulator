/*
Copyright (c) 2014, Yasuhiro Hasegawa
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
// testMemory.h
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("testByteBuffer");

#include "testBase.h"

#include "../util/ByteBuffer.h"

class testByteBuffer : public testBase {
	CPPUNIT_TEST_SUITE(testByteBuffer);
	CPPUNIT_TEST(testCapacity);
	CPPUNIT_TEST(testGet);
	CPPUNIT_TEST(testPut);
	CPPUNIT_TEST_SUITE_END();

public:
	void testCapacity() {
		quint8 data[100];
		quint32 size = sizeof(data);
		for(quint32 i = 0; i < size; i++) data[i] = (quint8)i;

		{
			LittleEndianByteBuffer bb(data, size);
			CPPUNIT_ASSERT_EQUAL(size, bb.getCapacity());
			{
				bb.setPos(0);
				for(quint32 i = 0; i < size; i++) {
					CPPUNIT_ASSERT_EQUAL((quint8)i, bb.get8());
				}
			}
			{
				bb.setPos(2);
				quint16 v = 0x0203;
				CPPUNIT_ASSERT_EQUAL(v, bb.get16());
			}
			{
				bb.setPos(4);
				quint32 v = 0x04050607;
				CPPUNIT_ASSERT_EQUAL(v, bb.get32());
			}
		}

		{
			BigEndianByteBuffer bb(data, size);
			CPPUNIT_ASSERT_EQUAL(size, bb.getCapacity());
			{
				bb.setPos(0);
				for(quint32 i = 0; i < size; i++) {
					CPPUNIT_ASSERT_EQUAL((i ^ 1U), (quint32)(bb.get8()));
				}
			}
			{
				bb.setPos(2);
				quint16 v = 0x0302;

				CPPUNIT_ASSERT_EQUAL(v, bb.get16());
			}
			{
				bb.setPos(4);
				quint32 v = 0x05040706;
				CPPUNIT_ASSERT_EQUAL(v, bb.get32());
			}
		}

	}

	void testGet() {
		quint8 data[100];
		quint32 size = sizeof(data);
		for(quint32 i = 0; i < size; i++) data[i] = (quint8)i;

		{
			LittleEndianByteBuffer bb(data, size);

			bb.setPos(0);
			CPPUNIT_ASSERT_EQUAL((quint32)0, bb.getPos());
			bb.get8();
			CPPUNIT_ASSERT_EQUAL((quint32)1, bb.getPos());

			bb.setPos(0);
			CPPUNIT_ASSERT_EQUAL((quint32)0, bb.getPos());
			bb.get16();
			CPPUNIT_ASSERT_EQUAL((quint32)2, bb.getPos());

			bb.setPos(0);
			CPPUNIT_ASSERT_EQUAL((quint32)0, bb.getPos());
			bb.get32();
			CPPUNIT_ASSERT_EQUAL((quint32)4, bb.getPos());
		}

		{
			BigEndianByteBuffer bb(data, size);

			bb.setPos(0);
			CPPUNIT_ASSERT_EQUAL((quint32)0, bb.getPos());
			bb.get8();
			CPPUNIT_ASSERT_EQUAL((quint32)1, bb.getPos());

			bb.setPos(0);
			CPPUNIT_ASSERT_EQUAL((quint32)0, bb.getPos());
			bb.get16();
			CPPUNIT_ASSERT_EQUAL((quint32)2, bb.getPos());

			bb.setPos(0);
			CPPUNIT_ASSERT_EQUAL((quint32)0, bb.getPos());
			bb.get32();
			CPPUNIT_ASSERT_EQUAL((quint32)4, bb.getPos());
		}
	}

	void testPut() {
		quint8 data[100];
		quint32 size = sizeof(data);
		for(quint32 i = 0; i < size; i++) data[i] = (quint8)i;

		{
			LittleEndianByteBuffer bb(data, size);

			bb.setPos(0);
			bb.setLimit(0);
			CPPUNIT_ASSERT_EQUAL((quint32)0, bb.getPos());
			CPPUNIT_ASSERT_EQUAL((quint32)0, bb.getLimit());
			bb.put8((quint8)0x12);
			CPPUNIT_ASSERT_EQUAL((quint32)1, bb.getPos());
			CPPUNIT_ASSERT_EQUAL((quint32)1, bb.getLimit());
			CPPUNIT_ASSERT_EQUAL((quint8)0x12, data[0]);


			bb.setPos(0);
			bb.setLimit(0);
			CPPUNIT_ASSERT_EQUAL((quint32)0, bb.getPos());
			CPPUNIT_ASSERT_EQUAL((quint32)0, bb.getLimit());
			bb.put16((quint16)0x1234);

			logger.info("data00 = %02X", data[0]);
			logger.info("data01 = %02X", data[1]);

			CPPUNIT_ASSERT_EQUAL((quint32)2, bb.getPos());
			CPPUNIT_ASSERT_EQUAL((quint32)2, bb.getLimit());
			CPPUNIT_ASSERT_EQUAL((quint8)0x34, data[0]);
			CPPUNIT_ASSERT_EQUAL((quint8)0x12, data[1]);
		}
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(testByteBuffer);
