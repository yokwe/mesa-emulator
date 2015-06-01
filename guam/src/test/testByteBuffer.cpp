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

		{
			BigEndianByteBuffer bb(data, size);

			// get with position
			// get with no position
			{
				bb.setPos(0);
				for(quint32 i = 0; i < size; i++) data[i] = 0;

				quint32 pos = 16;
				quint8 val = 0xA5;
				data[pos] = val;

				CPPUNIT_ASSERT_EQUAL(val, bb.get8(pos));

				bb.setPos(pos);
				CPPUNIT_ASSERT_EQUAL(val, bb.get8());
				CPPUNIT_ASSERT_EQUAL(quint32(pos + sizeof(val)), bb.getPos());
			}

			{
				bb.setPos(0);
				for(quint32 i = 0; i < size; i++) data[i] = 0;

				quint32 pos = 16;
				quint16 val = 0xA55A;
				data[pos + 0] = (quint8)(val >> 8);
				data[pos + 1] = (quint8)(val >> 0);

				CPPUNIT_ASSERT_EQUAL(val, bb.get16(pos));

				bb.setPos(pos);
				CPPUNIT_ASSERT_EQUAL(val, bb.get16());
				CPPUNIT_ASSERT_EQUAL(quint32(pos + sizeof(val)), bb.getPos());
			}

			{
				bb.setPos(0);
				for(quint32 i = 0; i < size; i++) data[i] = 0;

				quint32 pos = 16;
				quint32 val = 0xA55AB33B;
				data[pos + 0] = (quint8)(val >>  8);
				data[pos + 1] = (quint8)(val >>  0);
				data[pos + 2] = (quint8)(val >> 24);
				data[pos + 3] = (quint8)(val >> 16);

				CPPUNIT_ASSERT_EQUAL(val, bb.get32(pos));

				bb.setPos(pos);
				CPPUNIT_ASSERT_EQUAL(val, bb.get32());
				CPPUNIT_ASSERT_EQUAL(quint32(pos + sizeof(val)), bb.getPos());
			}
		}

		{
			LittleEndianByteBuffer bb(data, size);

			// get with position
			// get with no position
			{
				bb.setPos(0);
				for(quint32 i = 0; i < size; i++) data[i] = 0;

				quint32 pos = 16;
				quint8 val = 0xA5;
				data[pos ^ 1] = val;

				CPPUNIT_ASSERT_EQUAL(val, bb.get8(pos));

				bb.setPos(pos);
				CPPUNIT_ASSERT_EQUAL(val, bb.get8());
				CPPUNIT_ASSERT_EQUAL(quint32(pos + sizeof(val)), bb.getPos());
			}

			{
				bb.setPos(0);
				for(quint32 i = 0; i < size; i++) data[i] = 0;

				quint32 pos = 16;
				quint16 val = 0xA55A;
				data[(pos + 0) ^ 1] = (quint8)(val >> 8);
				data[(pos + 1) ^ 1] = (quint8)(val >> 0);

				CPPUNIT_ASSERT_EQUAL(val, bb.get16(pos));

				bb.setPos(pos);
				CPPUNIT_ASSERT_EQUAL(val, bb.get16());
				CPPUNIT_ASSERT_EQUAL(quint32(pos + sizeof(val)), bb.getPos());
			}

			{
				bb.setPos(0);
				for(quint32 i = 0; i < size; i++) data[i] = 0;

				quint32 pos = 16;
				quint32 val = 0xA55AB33B;
				data[(pos + 0) ^ 1] = (quint8)(val >>  8);
				data[(pos + 1) ^ 1] = (quint8)(val >>  0);
				data[(pos + 2) ^ 1] = (quint8)(val >> 24);
				data[(pos + 3) ^ 1] = (quint8)(val >> 16);

				CPPUNIT_ASSERT_EQUAL(val, bb.get32(pos));

				bb.setPos(pos);
				CPPUNIT_ASSERT_EQUAL(val, bb.get32());
				CPPUNIT_ASSERT_EQUAL(quint32(pos + sizeof(val)), bb.getPos());
			}
		}
	}

	// TODO write from here
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
				//
				CPPUNIT_ASSERT_EQUAL(pos, bb.getPos());
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
				//
				CPPUNIT_ASSERT_EQUAL(pos, bb.getPos());
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

			{
				quint32 putPos = 16;
				quint8  putVal = 100;
				bb.setPos(putPos);
				bb.put8(putVal);
				//
				CPPUNIT_ASSERT_EQUAL(putVal, data[putPos]);
				CPPUNIT_ASSERT_EQUAL((quint32)(putPos + sizeof(putVal)), bb.getPos());
			}

			{
				quint32 putPos = 32;
				quint16 putVal = 0x3456;
				bb.setPos(putPos);
				bb.put16(putVal);
				//
				CPPUNIT_ASSERT_EQUAL((quint32)((putVal >> 8) & 0xff), (quint32)(data[putPos + 0]));
				CPPUNIT_ASSERT_EQUAL((quint32)((putVal >> 0) & 0xff), (quint32)(data[putPos + 1]));
				CPPUNIT_ASSERT_EQUAL((quint32)(putPos + sizeof(putVal)), bb.getPos());
			}

			// TODO add test case of put32
		}

		{} // TODO add test case of LittleEndian
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(testByteBuffer);
