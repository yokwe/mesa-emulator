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
	CPPUNIT_TEST(testSet);
	CPPUNIT_TEST(testPut);
	CPPUNIT_TEST_SUITE_END();

public:
	void testCapacity() {
		quint8 data[100];
		quint32 size = sizeof(data);
		for(quint32 i = 0; i < size; i++) data[i] = (quint8)i;

		{
			BigEndianByteBuffer bb(data, size);
			CPPUNIT_ASSERT_EQUAL(size, bb.getCapacity());
			{
				for(quint32 i = 0; i < size; i++) {
					CPPUNIT_ASSERT_EQUAL((i), (quint32)(bb.get8(i)));
				}
			}
		}

		{
			LittleEndianByteBuffer bb(data, size);
			CPPUNIT_ASSERT_EQUAL(size, bb.getCapacity());
			{
				for(quint32 i = 0; i < size; i++) {
					CPPUNIT_ASSERT_EQUAL(i ^ 1U, (quint32)bb.get8(i));
				}
			}
		}
	}

	void testGet() {
		quint8 data[100];
		quint32 size = sizeof(data);
		for(quint32 i = 0; i < size; i++) data[i] = (quint8)i;

		{
			BigEndianByteBuffer bb(data, size);

			{
				bb.setPos(0);
				for(quint32 i = 0; i < size; i++) {
					CPPUNIT_ASSERT_EQUAL((quint8)i, bb.get8());
				}
			}

			{
				quint32 pos = 16;
				bb.setPos(pos);
				CPPUNIT_ASSERT_EQUAL(pos, bb.getPos());
				quint8 value = bb.get8();
				CPPUNIT_ASSERT_EQUAL((pos + 1), bb.getPos());
				CPPUNIT_ASSERT_EQUAL(pos, (quint32)value);
			}

			{
				quint32 pos = 32;
				bb.setPos(pos);
				CPPUNIT_ASSERT_EQUAL(pos, bb.getPos());
				quint16 value = bb.get16();
				CPPUNIT_ASSERT_EQUAL((pos + 2), bb.getPos());
				CPPUNIT_ASSERT_EQUAL((quint16)((pos << 8) | (pos + 1)), value);
			}

			{
				quint32 pos = 64;
				bb.setPos(pos);
				CPPUNIT_ASSERT_EQUAL(pos, bb.getPos());
				quint32 value = bb.get32();
				CPPUNIT_ASSERT_EQUAL((pos + 4), bb.getPos());
				CPPUNIT_ASSERT_EQUAL((quint32)((pos << 24) | ((pos + 1) << 16) | ((pos + 2) << 8) | ((pos + 3) << 0)), value);
			}
		}

		{
			LittleEndianByteBuffer bb(data, size);

			{
				bb.setPos(0);
				for(quint32 i = 0; i < size; i++) {
					CPPUNIT_ASSERT_EQUAL((quint32)(i ^ 1), (quint32)bb.get8());
				}
			}

			{
				quint32 pos = 16;
				bb.setPos(pos);
				CPPUNIT_ASSERT_EQUAL(pos, bb.getPos());
				quint8 value = bb.get8();
				CPPUNIT_ASSERT_EQUAL((pos ^ 1), bb.getPos());
				CPPUNIT_ASSERT_EQUAL((pos ^ 1), (quint32)value);
			}

			{
				quint32 pos = 32;
				bb.setPos(pos);
				CPPUNIT_ASSERT_EQUAL(pos, bb.getPos());
				quint16 value = bb.get16();
				CPPUNIT_ASSERT_EQUAL((pos + 2), bb.getPos());
				CPPUNIT_ASSERT_EQUAL((quint16)((((pos + 0) ^ 1) << 8) | ((pos + 1) ^ 1)), value);
			}

			{
				quint32 pos = 64;
				bb.setPos(pos);
				CPPUNIT_ASSERT_EQUAL(pos, bb.getPos());
				quint32 value = bb.get32();
				CPPUNIT_ASSERT_EQUAL((pos + 4), bb.getPos());
				CPPUNIT_ASSERT_EQUAL((quint32)((((pos + 0) ^ 1) << 24) | (((pos + 1) ^ 1) << 16) | (((pos + 2) ^ 1) << 8) | (((pos + 3) ^ 1) << 0)), value);
			}
		}
	}

	void testSet() {
		quint8 data[100];
		quint32 size = sizeof(data);
		for(quint32 i = 0; i < size; i++) data[i] = (quint8)i;

		{
			BigEndianByteBuffer bb(data, size);
			quint32 pos = 70;
			bb.setPos(pos);
			{
				quint32 setPos = 16;
				quint8  setVal = 100;
				bb.set8(setPos, setVal);
				CPPUNIT_ASSERT_EQUAL(setVal, data[setPos]);
			}
			{
				quint32 setPos = 32;
				quint16 setVal = 0x3456;
				bb.set16(setPos, setVal);
				// Big Endian => 34 56
				CPPUNIT_ASSERT_EQUAL((quint32)((setVal >> 8) & 0xff), (quint32)(data[setPos + 0]));
				CPPUNIT_ASSERT_EQUAL((quint32)((setVal >> 0) & 0xff), (quint32)(data[setPos + 1]));
			}
			{
				quint32 setPos = 64;
				quint32 setVal = 0x23456789;
				bb.set32(setPos, setVal);
				// Big Endian => 67 89 23 45
				CPPUNIT_ASSERT_EQUAL((quint8)(setVal >>  8), data[setPos + 0]);
				CPPUNIT_ASSERT_EQUAL((quint8)(setVal >>  0), data[setPos + 1]);
				CPPUNIT_ASSERT_EQUAL((quint8)(setVal >> 24), data[setPos + 2]);
				CPPUNIT_ASSERT_EQUAL((quint8)(setVal >> 16), data[setPos + 3]);
			}
		}

		// TODO write test code for little endian
		{}
	}

	void testPut() {
		quint8 data[100];
		quint32 size = sizeof(data);
		for(quint32 i = 0; i < size; i++) data[i] = (quint8)i;

		{
			BigEndianByteBuffer bb(data, size);

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
