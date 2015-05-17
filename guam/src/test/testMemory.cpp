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
static log4cpp::Category& logger = Logger::getLogger("testMemory");

#include "testBase.h"

class testMemory : public testBase {

	CPPUNIT_TEST_SUITE(testMemory);
	CPPUNIT_TEST(testPageSize);
	CPPUNIT_TEST(testStack);
	CPPUNIT_TEST(testReadDbl);
	CPPUNIT_TEST(testGetCodeByte);
	CPPUNIT_TEST_SUITE_END();


public:
	void testPageSize() {
		CPPUNIT_ASSERT_EQUAL((PageSize * sizeof(CARD16)), sizeof(Memory::Page));
	}
    void testStack() {
    	SP = 0;
		for(int i = 0; i < StackDepth; i++) {
			stack[i] = 0;
		}

    	CPPUNIT_ASSERT_EQUAL((CARD16)0, SP);

    	Push(0xabcd);
    	CPPUNIT_ASSERT_EQUAL((CARD16)1, SP);
    	CPPUNIT_ASSERT_EQUAL((CARD16)0xabcd, stack[0]);
    	CPPUNIT_ASSERT_EQUAL((CARD16)0, stack[1]);

    	CARD16 t = Pop();
    	CPPUNIT_ASSERT_EQUAL((CARD16)0xabcd, t);
    	CPPUNIT_ASSERT_EQUAL((CARD16)0, SP);
    	CPPUNIT_ASSERT_EQUAL((CARD16)0xabcd, stack[0]);
    	CPPUNIT_ASSERT_EQUAL((CARD16)0, stack[1]);

    	Recover();
    	CPPUNIT_ASSERT_EQUAL((CARD16)1, SP);
    	CPPUNIT_ASSERT_EQUAL((CARD16)0xabcd, stack[0]);
    	CPPUNIT_ASSERT_EQUAL((CARD16)0, stack[1]);

    	Discard();
    	CPPUNIT_ASSERT_EQUAL((CARD16)0, SP);
    	CPPUNIT_ASSERT_EQUAL((CARD16)0xabcd, stack[0]);
    	CPPUNIT_ASSERT_EQUAL((CARD16)0, stack[1]);

    	PushLong(0xcafebabe);
    	CPPUNIT_ASSERT_EQUAL((CARD16)2, SP);
    	CPPUNIT_ASSERT_EQUAL((CARD16)0xbabe, stack[0]); // LowHalf
    	CPPUNIT_ASSERT_EQUAL((CARD16)0xcafe, stack[1]); // HighHalf

    	CARD32 t2 = PopLong();
    	CPPUNIT_ASSERT_EQUAL((CARD32)0xcafebabe, t2);
    	CPPUNIT_ASSERT_EQUAL((CARD16)0, SP);
    }

    void testReadDbl() {
    	CARD32 t1 = 0x12345678;
    	page_MDS[0x1000 + 0] = LowHalf(t1);
    	page_MDS[0x1000 + 1] = HighHalf(t1);
    	CARD32 t2 = ReadDbl(Memory::MDS() + 0x1000);

    	CPPUNIT_ASSERT_EQUAL(t1, t2);
    	CPPUNIT_ASSERT_EQUAL((CARD16)0x5678, page_MDS[0x1000 + 0]);
    	CPPUNIT_ASSERT_EQUAL((CARD16)0x1234, page_MDS[0x1000 + 1]);
    }

    void testGetCodeByte() {
    	//logger.debug("testGetCodeByte   CB = %8X  PC = %04X", CodeCache::CB(), PC);
		CARD8* p = (CARD8*)page_CB;
		p += PC;
    	for(CARD16 i = 0; i < 1024; i++) {
    		//CARD16 pc = PC;
    		CARD8 expect = p[i ^1];
    		CARD8 actual = GetCodeByte();
     		//logger.debug("XXX %04X  PC = %04X   %02X  %02X", (CodeCache::CB() + (pc / 2)), pc, expect, actual);
     		CPPUNIT_ASSERT_EQUAL(expect, actual);
    	}
    }
};


CPPUNIT_TEST_SUITE_REGISTRATION(testMemory);
