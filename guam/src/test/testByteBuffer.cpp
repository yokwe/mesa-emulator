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

		{
			BigEndianByteBuffer bb(data, size);
			CPPUNIT_ASSERT_EQUAL(size, bb.getCapacity());
			CPPUNIT_ASSERT_EQUAL(size, bb.getLimit());
			CPPUNIT_ASSERT_EQUAL(size, bb.remaining());
		}

		{
			LittleEndianByteBuffer bb(data, size);
			CPPUNIT_ASSERT_EQUAL(size, bb.getCapacity());
			CPPUNIT_ASSERT_EQUAL(size, bb.getLimit());
			CPPUNIT_ASSERT_EQUAL(size, bb.remaining());
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
				CPPUNIT_ASSERT_EQUAL((quint32)0, bb.getPos());

				bb.setPos(pos);
				CPPUNIT_ASSERT_EQUAL(val, bb.get8());
				CPPUNIT_ASSERT_EQUAL(quint32(pos + sizeof(val)), bb.getPos());
			}

			{
				bb.setPos(0);
				for(quint32 i = 0; i < size; i++) data[i] = 0;

				quint32 pos = 16;
				quint16 val = 0x1122;
				data[16] = 0x11;
				data[17] = 0x22;

				CPPUNIT_ASSERT_EQUAL(val, bb.get16(pos));
				CPPUNIT_ASSERT_EQUAL((quint32)0, bb.getPos());

				bb.setPos(pos);
				CPPUNIT_ASSERT_EQUAL(val, bb.get16());
				CPPUNIT_ASSERT_EQUAL(quint32(pos + sizeof(val)), bb.getPos());
			}

			{
				bb.setPos(0);
				for(quint32 i = 0; i < size; i++) data[i] = 0;

				quint32 pos = 16;
				quint32 val = 0x11223344;
				data[16] = 0x33;
				data[17] = 0x44;
				data[18] = 0x11;
				data[19] = 0x22;

				CPPUNIT_ASSERT_EQUAL(val, bb.get32(pos));
				CPPUNIT_ASSERT_EQUAL((quint32)0, bb.getPos());

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
				quint8 val = 0x11;
				data[17] = 0x11;

				CPPUNIT_ASSERT_EQUAL(val, bb.get8(pos));
				CPPUNIT_ASSERT_EQUAL((quint32)0, bb.getPos());

				bb.setPos(pos);
				CPPUNIT_ASSERT_EQUAL(val, bb.get8());
				CPPUNIT_ASSERT_EQUAL(quint32(pos + sizeof(val)), bb.getPos());
			}

			{
				bb.setPos(0);
				for(quint32 i = 0; i < size; i++) data[i] = 0;

				quint32 pos = 16;
				quint16 val = 0x1122;
				data[17] = 0x11;
				data[16] = 0x22;

				CPPUNIT_ASSERT_EQUAL(val, bb.get16(pos));
				CPPUNIT_ASSERT_EQUAL((quint32)0, bb.getPos());

				bb.setPos(pos);
				CPPUNIT_ASSERT_EQUAL(val, bb.get16());
				CPPUNIT_ASSERT_EQUAL(quint32(pos + sizeof(val)), bb.getPos());
			}

			{
				bb.setPos(0);
				for(quint32 i = 0; i < size; i++) data[i] = 0;

				quint32 pos = 16;
				quint32 val = 0x11223344;
				data[17] = 0x33;
				data[16] = 0x44;
				data[19] = 0x11;
				data[18] = 0x22;

				CPPUNIT_ASSERT_EQUAL(val, bb.get32(pos));
				CPPUNIT_ASSERT_EQUAL((quint32)0, bb.getPos());

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

		{
			BigEndianByteBuffer bb(data, size);
			quint32 pos = 70;
			bb.setPos(pos);
			{
				for(quint32 i = 0; i < size; i++) data[i] = (quint8)0;

				quint32 setPos = 16;
				quint8  setVal = 0x11;
				bb.set8(setPos, setVal);
				CPPUNIT_ASSERT_EQUAL((quint32)(0x11), (quint32)(data[16]));
				CPPUNIT_ASSERT_EQUAL(pos, bb.getPos());
			}
			{
				for(quint32 i = 0; i < size; i++) data[i] = (quint8)0;

				quint32 setPos = 16;
				quint16 setVal = 0x1122;
				bb.set16(setPos, setVal);
				// Big Endian => 11 22
				CPPUNIT_ASSERT_EQUAL((quint32)(0x11), (quint32)(data[16]));
				CPPUNIT_ASSERT_EQUAL((quint32)(0x22), (quint32)(data[17]));
				//
				CPPUNIT_ASSERT_EQUAL(pos, bb.getPos());
			}
			{
				for(quint32 i = 0; i < size; i++) data[i] = (quint8)0;

				quint32 setPos = 16;
				quint32 setVal = 0x11223344;
				bb.set32(setPos, setVal);
				// Big Endian => 33 44 11 22
				CPPUNIT_ASSERT_EQUAL((quint8)(0x33), data[16]);
				CPPUNIT_ASSERT_EQUAL((quint8)(0x44), data[17]);
				CPPUNIT_ASSERT_EQUAL((quint8)(0x11), data[18]);
				CPPUNIT_ASSERT_EQUAL((quint8)(0x22), data[19]);
				//
				CPPUNIT_ASSERT_EQUAL(pos, bb.getPos());
			}
		}

		{
			LittleEndianByteBuffer bb(data, size);
			quint32 pos = 70;
			bb.setPos(pos);
			{
				for(quint32 i = 0; i < size; i++) data[i] = (quint8)0;

				quint32 setPos = 16;
				quint8  setVal = 0x11;
				bb.set8(setPos, setVal);
				CPPUNIT_ASSERT_EQUAL((quint32)(0x11), (quint32)(data[17]));
				CPPUNIT_ASSERT_EQUAL(pos, bb.getPos());
			}
			{
				for(quint32 i = 0; i < size; i++) data[i] = (quint8)0;

				quint32 setPos = 16;
				quint16 setVal = 0x1122;
				bb.set16(setPos, setVal);
				// Little Endian => 22 11
				CPPUNIT_ASSERT_EQUAL((quint32)(0x11), (quint32)(data[17]));
				CPPUNIT_ASSERT_EQUAL((quint32)(0x22), (quint32)(data[16]));
				//
				CPPUNIT_ASSERT_EQUAL(pos, bb.getPos());
			}
			{
				for(quint32 i = 0; i < size; i++) data[i] = (quint8)0;

				quint32 setPos = 16;
				quint32 setVal = 0x11223344;
				bb.set32(setPos, setVal);
				// Little Endian => 44 33 22 11
				CPPUNIT_ASSERT_EQUAL((quint8)(0x33), data[17]);
				CPPUNIT_ASSERT_EQUAL((quint8)(0x44), data[16]);
				CPPUNIT_ASSERT_EQUAL((quint8)(0x11), data[19]);
				CPPUNIT_ASSERT_EQUAL((quint8)(0x22), data[18]);
				//
				CPPUNIT_ASSERT_EQUAL(pos, bb.getPos());
			}
		}
	}

	void testPut() {
		quint8 data[100];
		quint32 size = sizeof(data);

		{
			BigEndianByteBuffer bb(data, size);
			{
				for(quint32 i = 0; i < size; i++) data[i] = (quint8)0;

				quint32 setPos = 16;
				quint8  setVal = 0x11;
				bb.setPos(setPos);
				bb.put8(setVal);
				CPPUNIT_ASSERT_EQUAL((quint32)(0x11), (quint32)(data[16]));
				CPPUNIT_ASSERT_EQUAL((quint32)(setPos + sizeof(setVal)), bb.getPos());
			}
			{
				for(quint32 i = 0; i < size; i++) data[i] = (quint8)0;

				quint32 setPos = 16;
				quint16 setVal = 0x1122;
				bb.setPos(setPos);
				bb.put16(setVal);
				// Big Endian => 11 22
				CPPUNIT_ASSERT_EQUAL((quint32)(0x11), (quint32)(data[16]));
				CPPUNIT_ASSERT_EQUAL((quint32)(0x22), (quint32)(data[17]));
				CPPUNIT_ASSERT_EQUAL((quint32)(setPos + sizeof(setVal)), bb.getPos());
			}
			{
				for(quint32 i = 0; i < size; i++) data[i] = (quint8)0;

				quint32 setPos = 16;
				quint32 setVal = 0x11223344;
				bb.setPos(setPos);
				bb.put32(setVal);
				// Big Endian => 33 44 11 22
				CPPUNIT_ASSERT_EQUAL((quint8)(0x33), data[16]);
				CPPUNIT_ASSERT_EQUAL((quint8)(0x44), data[17]);
				CPPUNIT_ASSERT_EQUAL((quint8)(0x11), data[18]);
				CPPUNIT_ASSERT_EQUAL((quint8)(0x22), data[19]);
				CPPUNIT_ASSERT_EQUAL((quint32)(setPos + sizeof(setVal)), bb.getPos());
			}
		}

		{
			LittleEndianByteBuffer bb(data, size);
			quint32 pos = 70;
			bb.setPos(pos);
			{
				for(quint32 i = 0; i < size; i++) data[i] = (quint8)0;

				quint32 setPos = 16;
				quint8  setVal = 0x11;
				bb.setPos(setPos);
				bb.put8(setVal);
				CPPUNIT_ASSERT_EQUAL((quint32)(0x11), (quint32)(data[17]));
				CPPUNIT_ASSERT_EQUAL((quint32)(setPos + sizeof(setVal)), bb.getPos());
			}
			{
				for(quint32 i = 0; i < size; i++) data[i] = (quint8)0;

				quint32 setPos = 16;
				quint16 setVal = 0x1122;
				bb.setPos(setPos);
				bb.put16(setVal);
				// Little Endian => 22 11
				CPPUNIT_ASSERT_EQUAL((quint32)(0x11), (quint32)(data[17]));
				CPPUNIT_ASSERT_EQUAL((quint32)(0x22), (quint32)(data[16]));
				CPPUNIT_ASSERT_EQUAL((quint32)(setPos + sizeof(setVal)), bb.getPos());
			}
			{
				for(quint32 i = 0; i < size; i++) data[i] = (quint8)0;

				quint32 setPos = 16;
				quint32 setVal = 0x11223344;
				bb.setPos(setPos);
				bb.put32(setVal);
				// Little Endian => 44 33 22 11
				CPPUNIT_ASSERT_EQUAL((quint8)(0x33), data[17]);
				CPPUNIT_ASSERT_EQUAL((quint8)(0x44), data[16]);
				CPPUNIT_ASSERT_EQUAL((quint8)(0x11), data[19]);
				CPPUNIT_ASSERT_EQUAL((quint8)(0x22), data[18]);
				CPPUNIT_ASSERT_EQUAL((quint32)(setPos + sizeof(setVal)), bb.getPos());
			}
		}
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(testByteBuffer);
