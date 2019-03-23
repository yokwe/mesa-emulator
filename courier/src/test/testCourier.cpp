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
static log4cpp::Category& logger = Logger::getLogger("testCourier");

#include "testBase.h"

class testCourier : public testBase {
	CPPUNIT_TEST_SUITE(testCourier);
	CPPUNIT_TEST(testBlock);
	CPPUNIT_TEST(testBlock8);
	CPPUNIT_TEST_SUITE_END();

public:
	void testBlock() {
		const quint16 capacity = 256;
		quint8 data[capacity];
		for(quint16 i = 0; i < capacity; i++) {
			data[i] = i;
		}
		Courier::BLOCK block(data, sizeof(data));

		CPPUNIT_ASSERT_EQUAL(capacity, block.capacity);
		CPPUNIT_ASSERT_EQUAL((quint16)0, block.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)0, block.getLimit());
	}
	void testBlock8() {
		const quint16 capacity = 2;
		quint8 data[capacity];
		for(quint16 i = 0; i < capacity; i++) {
			data[i] = i;
		}
		Courier::BLOCK block(data, sizeof(data));

		quint8 a0 = 0x11;
		quint8 a1 = 0x22;
		block.serialize8(a0);
		block.serialize8(a1);

		CPPUNIT_ASSERT_EQUAL((quint16)2, block.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)0, block.getLimit());

		block.rewind();

		CPPUNIT_ASSERT_EQUAL((quint16)0, block.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)2, block.getLimit());

		quint8 b0, b1;
		block.deserialize8(b0);
		block.deserialize8(b1);

		CPPUNIT_ASSERT_EQUAL((quint16)2, block.getPos());
		CPPUNIT_ASSERT_EQUAL((quint16)2, block.getLimit());

		CPPUNIT_ASSERT_EQUAL(a0, b0);
		CPPUNIT_ASSERT_EQUAL(a1, b1);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(testCourier);
