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
// testOpcde_100.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("test100");

#include "testBase.h"

class testOpcode_100 : public testBase {
	CPPUNIT_TEST_SUITE(testOpcode_100);

	CPPUNIT_TEST(testR0);   // 0100
	CPPUNIT_TEST(testR1);   // 0101
	CPPUNIT_TEST(testRB);   // 0102
	CPPUNIT_TEST(testRL0);  // 0103
	CPPUNIT_TEST(testRLB);  // 0104
	CPPUNIT_TEST(testRD0);  // 0105
	CPPUNIT_TEST(testRDB);  // 0106
	CPPUNIT_TEST(testRDL0); // 0107

	CPPUNIT_TEST(testRDLB); // 0110
	CPPUNIT_TEST(testW0);   // 0111
	CPPUNIT_TEST(testWB);   // 0112
	CPPUNIT_TEST(testPSB);  // 0113
	CPPUNIT_TEST(testWLB);  // 0114
	CPPUNIT_TEST(testPSLB); // 0115
	CPPUNIT_TEST(testWDB);  // 0116
	CPPUNIT_TEST(testPSD0); // 0117

	CPPUNIT_TEST(testPSDB);  // 0120
	CPPUNIT_TEST(testWDLB);  // 0121
	CPPUNIT_TEST(testPSDLB); // 0122
	CPPUNIT_TEST(testRLI00); // 0123
	CPPUNIT_TEST(testRLI01); // 0124
	CPPUNIT_TEST(testRLI02); // 0125
	CPPUNIT_TEST(testRLI03); // 0126
	CPPUNIT_TEST(testRLIP);  // 0127

	CPPUNIT_TEST(testRLILP);  // 0130
	CPPUNIT_TEST(testRLDI00); // 0131
	CPPUNIT_TEST(testRLDIP);  // 0132
	CPPUNIT_TEST(testRLDILP); // 0133
	CPPUNIT_TEST(testRGIP);   // 0134
	CPPUNIT_TEST(testRGILP);  // 0135
	CPPUNIT_TEST(testWLIP);   // 0136
	CPPUNIT_TEST(testWLILP);  // 0137

	CPPUNIT_TEST(testWLDILP);    // 0140
	CPPUNIT_TEST(testRS_left);   // 0141
	CPPUNIT_TEST(testRS_right);  // 0141
	CPPUNIT_TEST(testRLS_left);  // 0142
	CPPUNIT_TEST(testRLS_right); // 0142
	CPPUNIT_TEST(testWS_left);   // 0143
	CPPUNIT_TEST(testWS_right);  // 0143
	CPPUNIT_TEST(testWLS_left);  // 0144
	CPPUNIT_TEST(testWLS_right); // 0144
	CPPUNIT_TEST(testR0F_0);     // 0145
	CPPUNIT_TEST(testR0F_4);     // 0145
	CPPUNIT_TEST(testR0F_8);     // 0145
	CPPUNIT_TEST(testR0F_12);    // 0145
	CPPUNIT_TEST(testRF);        // 0146
	CPPUNIT_TEST(testRL0F);      // 0147

	CPPUNIT_TEST(testRLF);    // 0150
	CPPUNIT_TEST(testRLFS);   // 0151
	CPPUNIT_TEST(testRLIPF);  // 0152
	CPPUNIT_TEST(testRLILPF); // 0153
	CPPUNIT_TEST(testW0F);    // 0154
	CPPUNIT_TEST(testWF);     // 0155
	CPPUNIT_TEST(testPSF);    // 0156
	CPPUNIT_TEST(testPS0F);   // 0157

	CPPUNIT_TEST(testWS0F);    // 0160
	CPPUNIT_TEST(testWL0F);    // 0161
	CPPUNIT_TEST(testWLF);     // 0162
	CPPUNIT_TEST(testPSLF);    // 0163
	CPPUNIT_TEST(testWLFS);    // 0164
	CPPUNIT_TEST(testSLDB);    // 0165
	CPPUNIT_TEST(testSGDB);    // 0166
	CPPUNIT_TEST(testLLKB_cl); // 0167
	CPPUNIT_TEST(testLLKB_fl); // 0167

	CPPUNIT_TEST(testRKIB);      // 0170
	CPPUNIT_TEST(testRKDIB);     // 0171
	CPPUNIT_TEST(testLKB);       // 0172
	CPPUNIT_TEST(testSHIFT_p);   // 0173
	CPPUNIT_TEST(testSHIFT_n);   // 0173
	CPPUNIT_TEST(testSHIFTSB_p); // 0174
	CPPUNIT_TEST(testSHIFTSB_n); // 0174
//	CPPUNIT_TEST(testMBP);       // 0175
//	CPPUNIT_TEST(testRBP);       // 0176
//	CPPUNIT_TEST(testWBP);       // 0177


	CPPUNIT_TEST_SUITE_END();

	///////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////

	void testR0() {
		page_CB[PC / 2] = zR0 << 8 | 0x00;
		CARD16 ptr = 0x40;
		stack[SP++] = ptr;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_MDS[ptr + 0], stack[0]);
	}
	void testR1() {
		page_CB[PC / 2] = zR1 << 8 | 0x00;
		CARD16 ptr = 0x40;
		stack[SP++] = ptr;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_MDS[ptr + 1], stack[0]);
	}
	void testRB() {
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zRB << 8 | alpha;
		CARD16 ptr = 0x40;
		stack[SP++] = ptr;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_MDS[ptr + alpha], stack[0]);
	}


	void testRL0() {
		page_CB[PC / 2] = zRL0 << 8 | 0x00;
		CARD32 ptr = Memory::MDS() + 0x10;
		stack[SP++] = LowHalf(ptr);
		stack[SP++] = HighHalf(ptr);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_MDS[(ptr - Memory::MDS()) + 0], stack[0]);
	}
	void testRLB() {
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zRLB << 8 | alpha;
		CARD32 ptr = Memory::MDS() + 0x10;
		stack[SP++] = LowHalf(ptr);
		stack[SP++] = HighHalf(ptr);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_MDS[(ptr - Memory::MDS()) + alpha], stack[0]);
	}


	void testRD0() {
		page_CB[PC / 2] = zRD0 << 8 | 0x00;
		CARD16 ptr = 0x40;
		stack[SP++] = ptr;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_MDS[ptr + 0 + 0], stack[0]);
		CPPUNIT_ASSERT_EQUAL(page_MDS[ptr + 0 + 1], stack[1]);
	}
	void testRDB() {
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zRDB << 8 | alpha;
		CARD16 ptr = 0x40;
		stack[SP++] = ptr;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_MDS[ptr + alpha + 0], stack[0]);
		CPPUNIT_ASSERT_EQUAL(page_MDS[ptr + alpha + 1], stack[1]);
	}


	void testRDL0() {
		page_CB[PC / 2] = zRDL0 << 8 | 0x00;
		CARD32 ptr = Memory::MDS() + 0x10;
		stack[SP++] = LowHalf(ptr);
		stack[SP++] = HighHalf(ptr);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_MDS[(ptr - Memory::MDS() + 0 + 0)], stack[0]);
		CPPUNIT_ASSERT_EQUAL(page_MDS[(ptr - Memory::MDS() + 0 + 1)], stack[1]);
	}
	void testRDLB() {
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zRDLB << 8 | alpha;
		CARD32 ptr = Memory::MDS() + 0x10;
		stack[SP++] = LowHalf(ptr);
		stack[SP++] = HighHalf(ptr);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_MDS[(ptr - Memory::MDS() + alpha + 0)], stack[0]);
		CPPUNIT_ASSERT_EQUAL(page_MDS[(ptr - Memory::MDS() + alpha + 1)], stack[1]);
	}


	void testW0() {
		page_CB[PC / 2] = zW0 << 8 | 0x00;
		const CARD16 value = 0x1234;
		const CARD16 ptr = 0x40;
		stack[SP++] = value;
		stack[SP++] = ptr;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value, page_MDS[(ptr + 0)]);
	}
	void testWB() {
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zWB << 8 | alpha;
		const CARD16 value = 0x1234;
		const CARD16 ptr = 0x40;
		stack[SP++] = value;
		stack[SP++] = ptr;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value, page_MDS[(ptr + alpha)]);
	}


	void testPSB() {
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zPSB << 8 | alpha;
		const CARD16 ptr = 0x40;
		const CARD16 value = 0x1234;
		stack[SP++] = ptr;
		stack[SP++] = value;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(ptr, stack[0]);
		CPPUNIT_ASSERT_EQUAL(value, page_MDS[(ptr + alpha)]);
	}


	void testWLB() {
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zWLB << 8 | alpha;
		const CARD16 value = 0x1234;
		const CARD32 ptr = Memory::MDS() + 0x40;
		stack[SP++] = value;
		stack[SP++] = LowHalf(ptr);
		stack[SP++] = HighHalf(ptr);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value, page_MDS[(ptr - Memory::MDS() + alpha)]);
	}


	void testPSLB() {
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zPSLB << 8 | alpha;
		const CARD32 ptr = Memory::MDS() + 0x40;
		const CARD16 value = 0x1234;
		stack[SP++] = LowHalf(ptr);
		stack[SP++] = HighHalf(ptr);
		stack[SP++] = value;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(ptr),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(ptr), stack[1]);
		CPPUNIT_ASSERT_EQUAL(value, page_MDS[(ptr - Memory::MDS() + alpha)]);
	}


	void testWDB() {
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zWDB << 8 | alpha;
		const CARD32 value = 0x12345678;
		const CARD16 ptr = 0x40;
		stack[SP++] = LowHalf(value);
		stack[SP++] = HighHalf(value);
		stack[SP++] = ptr;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(value),  page_MDS[(ptr + alpha + 0)]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(value), page_MDS[(ptr + alpha + 1)]);
	}


	void testPSD0() {
		page_CB[PC / 2] = zPSD0 << 8 | 0x00;
		const CARD16 ptr = 0x40;
		const CARD32 value = 0x12345678;
		stack[SP++] = ptr;
		stack[SP++] = LowHalf(value);
		stack[SP++] = HighHalf(value);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(value),  page_MDS[(ptr + 0 + 0)]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(value), page_MDS[(ptr + 0 + 1)]);
	}
	void testPSDB() {
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zPSDB << 8 | alpha;
		const CARD16 ptr = 0x40;
		const CARD32 value = 0x12345678;
		stack[SP++] = ptr;
		stack[SP++] = LowHalf(value);
		stack[SP++] = HighHalf(value);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(value),  page_MDS[(ptr + alpha + 0)]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(value), page_MDS[(ptr + alpha + 1)]);
	}


	void testWDLB() {
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zWDLB << 8 | alpha;
		const CARD32 value = 0x12345678;
		const CARD32 ptr = Memory::MDS() + 0x40;
		stack[SP++] = LowHalf(value);
		stack[SP++] = HighHalf(value);
		stack[SP++] = LowHalf(ptr);
		stack[SP++] = HighHalf(ptr);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(value),  page_MDS[(ptr - Memory::MDS() + alpha + 0)]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(value), page_MDS[(ptr - Memory::MDS() + alpha + 1)]);
	}


	void testPSDLB() {
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zPSDLB << 8 | alpha;
		const CARD32 ptr = Memory::MDS() + 0x40;
		const CARD32 value = 0x12345678;
		stack[SP++] = LowHalf(ptr);
		stack[SP++] = HighHalf(ptr);
		stack[SP++] = LowHalf(value);
		stack[SP++] = HighHalf(value);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(ptr),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(ptr), stack[1]);
		CPPUNIT_ASSERT_EQUAL(LowHalf(value),  page_MDS[(ptr - Memory::MDS() + alpha + 0)]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(value), page_MDS[(ptr - Memory::MDS() + alpha + 1)]);
	}


	void testRLI00() {
		page_CB[PC / 2] = zRLI00 << 8 | 0x00;
		CARD16 ptr = 0x40;
		page_LF[0] = ptr;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_MDS[(ptr + 0)], stack[0]);
	}
	void testRLI01() {
		page_CB[PC / 2] = zRLI01 << 8 | 0x00;
		CARD16 ptr = 0x40;
		page_LF[0] = ptr;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_MDS[(ptr + 1)], stack[0]);
	}
	void testRLI02() {
		page_CB[PC / 2] = zRLI02 << 8 | 0x00;
		CARD16 ptr = 0x40;
		page_LF[0] = ptr;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_MDS[(ptr + 2)], stack[0]);
	}
	void testRLI03() {
		page_CB[PC / 2] = zRLI03 << 8 | 0x00;
		CARD16 ptr = 0x40;
		page_LF[0] = ptr;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_MDS[(ptr + 3)], stack[0]);
	}


	void testRLIP() {
		NibblePair pair;
		pair.left  = 0x02;
		pair.right = 0x04;
		page_CB[PC / 2] = zRLIP << 8 | pair.u;
		CARD16 ptr = 0x40;
		page_LF[(pair.left)] = ptr;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_MDS[(ptr + pair.right)], stack[0]);
	}


	void testRLILP() {
		NibblePair pair;
		pair.left  = 0x02;
		pair.right = 0x04;
		page_CB[PC / 2] = zRLILP << 8 | pair.u;
		CARD32 ptr = Memory::MDS() + 0x40;
		page_LF[(pair.left + 0)] = LowHalf(ptr);
		page_LF[(pair.left + 1)] = HighHalf(ptr);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_MDS[(ptr - Memory::MDS() + pair.right)], stack[0]);
	}


	void testRLDI00() {
		page_CB[PC / 2] = zRLDI00 << 8 | 0x00;
		CARD16 ptr = 0x40;
		page_LF[(0)] = ptr;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_MDS[(ptr + 0)], stack[0]);
		CPPUNIT_ASSERT_EQUAL(page_MDS[(ptr + 1)], stack[1]);
	}


	void testRLDIP() {
		NibblePair pair;
		pair.left  = 0x02;
		pair.right = 0x04;
		page_CB[PC / 2] = zRLDIP << 8 | pair.u;
		CARD16 ptr = 0x40;
		page_LF[(pair.left)] = ptr;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_MDS[(ptr + pair.right + 0)], stack[0]);
		CPPUNIT_ASSERT_EQUAL(page_MDS[(ptr + pair.right + 1)], stack[1]);
	}


	void testRLDILP() {
		NibblePair pair;
		pair.left  = 0x02;
		pair.right = 0x04;
		page_CB[PC / 2] = zRLDILP << 8 | pair.u;
		CARD32 ptr = Memory::MDS() + 0x40;
		page_LF[(pair.left + 0)] = LowHalf(ptr);
		page_LF[(pair.left + 1)] = HighHalf(ptr);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_MDS[(ptr - Memory::MDS() + pair.right + 0)], stack[0]);
		CPPUNIT_ASSERT_EQUAL(page_MDS[(ptr - Memory::MDS() + pair.right + 1)], stack[1]);
	}


	void testRGIP() {
		NibblePair pair;
		pair.left  = 0x02;
		pair.right = 0x04;
		page_CB[PC / 2] = zRGIP << 8 | pair.u;
		CARD16 ptr = 0x40;
		page_GF[(pair.left)] = ptr;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_MDS[(ptr + pair.right)], stack[0]);
	}


	void testRGILP() {
		NibblePair pair;
		pair.left  = 0x02;
		pair.right = 0x04;
		page_CB[PC / 2] = zRGILP << 8 | pair.u;
		CARD32 ptr = Memory::MDS() + 0x40;
		page_GF[(pair.left + 0)] = LowHalf(ptr);
		page_GF[(pair.left + 1)] = HighHalf(ptr);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_MDS[(ptr - Memory::MDS() + pair.right)], stack[0]);
	}


	void testWLIP() {
		NibblePair pair;
		pair.left  = 0x02;
		pair.right = 0x04;
		page_CB[PC / 2] = zWLIP << 8 | pair.u;
		CARD16 ptr = 0x40;
		page_LF[(pair.left)] = ptr;
		CARD16 value = 0x1234;
		stack[SP++] = value;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value, page_MDS[(ptr + pair.right)]);
	}


	void testWLILP() {
		NibblePair pair;
		pair.left  = 0x02;
		pair.right = 0x04;
		page_CB[PC / 2] = zWLILP << 8 | pair.u;
		CARD32 ptr = Memory::MDS() + 0x40;
		page_LF[(pair.left + 0)] = LowHalf(ptr);
		page_LF[(pair.left + 1)] = HighHalf(ptr);
		CARD16 value = 0x1234;
		stack[SP++] = value;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value, page_MDS[(ptr - Memory::MDS() + pair.right)]);
	}


	void testWLDILP() {
		NibblePair pair;
		pair.left  = 0x02;
		pair.right = 0x04;
		page_CB[PC / 2] = zWLDILP << 8 | pair.u;
		CARD32 ptr = Memory::MDS() + 0x40;
		page_LF[(pair.left + 0)] = LowHalf(ptr);
		page_LF[(pair.left + 1)] = HighHalf(ptr);
		CARD32 value = 0x12345678;
		stack[SP++] = LowHalf(value);
		stack[SP++] = HighHalf(value);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(value),  page_MDS[(ptr - Memory::MDS() + pair.right + 0)]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(value), page_MDS[(ptr - Memory::MDS() + pair.right + 1)]);
	}


	void testRS_left() {
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zRS << 8 | alpha;
		CARD16 index = 0x30;
		POINTER ptr = 0x40;
		BytePair value;
		value.left = 0x12;
		value.right = 0x34;
		page_MDS[(ptr + (index + alpha) / 2)] = value.u;
		stack[SP++] = ptr;
		stack[SP++] = index;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value.left,  stack[0]);
	}
	void testRS_right() {
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zRS << 8 | alpha;
		CARD16 index = 0x31;
		POINTER ptr = 0x40;
		BytePair value;
		value.left = 0x12;
		value.right = 0x34;
		page_MDS[(ptr + (index + alpha) / 2)] = value.u;
		stack[SP++] = ptr;
		stack[SP++] = index;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value.right,  stack[0]);
	}


	void testRLS_left() {
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zRLS << 8 | alpha;
		CARD16 index = 0x30;
		LONG_POINTER ptr = Memory::MDS() + 0x40;
		BytePair value;
		value.left = 0x12;
		value.right = 0x34;
		page_MDS[(ptr - Memory::MDS() + (index + alpha) / 2)] = value.u;
		stack[SP++] = LowHalf(ptr);
		stack[SP++] = HighHalf(ptr);
		stack[SP++] = index;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value.left,  stack[0]);
	}
	void testRLS_right() {
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zRLS << 8 | alpha;
		CARD16 index = 0x31;
		LONG_POINTER ptr = Memory::MDS() + 0x40;
		BytePair value;
		value.left = 0x12;
		value.right = 0x34;
		page_MDS[(ptr - Memory::MDS() + (index + alpha) / 2)] = value.u;
		stack[SP++] = LowHalf(ptr);
		stack[SP++] = HighHalf(ptr);
		stack[SP++] = index;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value.right,  stack[0]);
	}


	void testWS_left() {
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zWS << 8 | alpha;
		CARD16 index = 0x30;
		POINTER ptr = 0x40;
		BytePair value;
		value.left = 0x12;
		value.right = 0x34;
		page_MDS[(ptr + (index + alpha) / 2)] = value.u;
		CARD16 data = 0xab;
		stack[SP++] = data;
		stack[SP++] = ptr;
		stack[SP++] = index;
		Interpreter::execute();

		value.left = data;
		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value.u,  page_MDS[(ptr + (index + alpha) / 2)]);
	}
	void testWS_right() {
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zWS << 8 | alpha;
		CARD16 index = 0x31;
		POINTER ptr = 0x40;
		BytePair value;
		value.left = 0x12;
		value.right = 0x34;
		page_MDS[(ptr + (index + alpha) / 2)] = value.u;
		CARD16 data = 0xab;
		stack[SP++] = data;
		stack[SP++] = ptr;
		stack[SP++] = index;
		Interpreter::execute();

		value.right = data;
		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value.u,  page_MDS[(ptr + (index + alpha) / 2)]);
	}


	void testWLS_left() {
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zWLS << 8 | alpha;
		CARD16 index = 0x30;
		LONG_POINTER ptr = Memory::MDS() + 0x40;
		BytePair value;
		value.left = 0x12;
		value.right = 0x34;
		page_MDS[(ptr - Memory::MDS() + (index + alpha) / 2)] = value.u;
		CARD16 data = 0xab;
		stack[SP++] = data;
		stack[SP++] = LowHalf(ptr);
		stack[SP++] = HighHalf(ptr);
		stack[SP++] = index;
		Interpreter::execute();

		value.left = data;
		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value.u,  page_MDS[(ptr - Memory::MDS() + (index + alpha) / 2)]);
	}
	void testWLS_right() {
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zWLS << 8 | alpha;
		CARD16 index = 0x31;
		LONG_POINTER ptr = Memory::MDS() + 0x40;
		BytePair value;
		value.left = 0x12;
		value.right = 0x34;
		page_MDS[(ptr - Memory::MDS() + (index + alpha) / 2)] = value.u;
		CARD16 data = 0xab;
		stack[SP++] = data;
		stack[SP++] = LowHalf(ptr);
		stack[SP++] = HighHalf(ptr);
		stack[SP++] = index;
		Interpreter::execute();

		value.right = data;
		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value.u,  page_MDS[(ptr - Memory::MDS() + (index + alpha) / 2)]);
	}


	void testR0F_0() {
		FieldSpec spec;
		spec.pos = 0;
		spec.size = 3;
		page_CB[PC / 2] = zR0F << 8 | spec.u;
		CARD16 ptr = 0x40;
		page_MDS[(ptr + 0)] = 0xABCD;
		stack[SP++] = ptr;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(0x0A, (int)stack[0]);
	}
	void testR0F_4() {
		FieldSpec spec;
		spec.pos = 4;
		spec.size = 3;
		page_CB[PC / 2] = zR0F << 8 | spec.u;
		CARD16 ptr = 0x40;
		page_MDS[(ptr + 0)] = 0xABCD;
		stack[SP++] = ptr;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(0x0B, (int)stack[0]);
	}
	void testR0F_8() {
		FieldSpec spec;
		spec.pos = 8;
		spec.size = 3;
		page_CB[PC / 2] = zR0F << 8 | spec.u;
		CARD16 ptr = 0x40;
		page_MDS[(ptr + 0)] = 0xABCD;
		stack[SP++] = ptr;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(0x0C, (int)stack[0]);
	}
	void testR0F_12() {
		FieldSpec spec;
		spec.pos = 12;
		spec.size = 3;
		page_CB[PC / 2] = zR0F << 8 | spec.u;
		CARD16 ptr = 0x40;
		page_MDS[(ptr + 0)] = 0xABCD;
		stack[SP++] = ptr;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(0x0D, (int)stack[0]);
	}


	void testRF() {
		FieldSpec spec;
		spec.pos = 8;
		spec.size = 3;
		FieldDesc desc;
		desc.field = spec.u;
		desc.offset = 0x10;
		page_CB[PC / 2] = zRF << 8 | desc.offset;
		page_CB[(PC / 2) + 1] = desc.field << 8 | 0x00;
		CARD16 ptr = 0x40;
		page_MDS[(ptr + desc.offset)] = 0xABCD;
		stack[SP++] = ptr;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 3, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(0x0C, (int)stack[0]);
	}


	void testRL0F() {
		FieldSpec spec;
		spec.pos = 0;
		spec.size = 3;
		page_CB[PC / 2] = zRL0F << 8 | spec.u;
		CARD32 ptr = Memory::MDS() + 0x40;
		page_MDS[(ptr - Memory::MDS() + 0)] = 0xABCD;
		stack[SP++] = LowHalf(ptr);
		stack[SP++] = HighHalf(ptr);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(0x0A, (int)stack[0]);
	}


	void testRLF() {
		FieldSpec spec;
		spec.pos = 8;
		spec.size = 3;
		FieldDesc desc;
		desc.field = spec.u;
		desc.offset = 0x10;
		page_CB[PC / 2] = zRLF << 8 | desc.offset;
		page_CB[(PC / 2) + 1] = desc.field << 8 | 0x00;
		CARD32 ptr = Memory::MDS() + 0x40;
		page_MDS[(ptr - Memory::MDS() + desc.offset)] = 0xABCD;
		stack[SP++] = LowHalf(ptr);
		stack[SP++] = HighHalf(ptr);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 3, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(0x0C, (int)stack[0]);
	}


	void testRLFS() {
		FieldSpec spec;
		spec.pos = 8;
		spec.size = 3;
		FieldDesc desc;
		desc.field = spec.u;
		desc.offset = 0x10;
		page_CB[PC / 2] = zRLFS << 8 | 0x00;
		CARD32 ptr = Memory::MDS() + 0x40;
		page_MDS[(ptr - Memory::MDS() + desc.offset)] = 0xABCD;
		stack[SP++] = LowHalf(ptr);
		stack[SP++] = HighHalf(ptr);
		stack[SP++] = desc.u;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(0x0C, (int)stack[0]);
	}


	void testRLIPF() {
		NibblePair pair;
		pair.left = 0x04;
		pair.right = 0x06;
		FieldSpec spec;
		spec.pos = 8;
		spec.size = 3;
		page_CB[PC / 2] = zRLIPF << 8 | pair.u;
		page_CB[(PC / 2) + 1] = spec.u << 8 | 0x00;
		CARD16 ptr = 0x40;
		page_LF[(pair.left)] = ptr;
		page_MDS[ptr + pair.right] = 0xABCD;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 3, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(0x0C, (int)stack[0]);
	}


	void testRLILPF() {
		NibblePair pair;
		pair.left = 0x04;
		pair.right = 0x06;
		FieldSpec spec;
		spec.pos = 8;
		spec.size = 3;
		page_CB[PC / 2] = zRLILPF << 8 | pair.u;
		page_CB[(PC / 2) + 1] = spec.u << 8 | 0x00;
		CARD32 ptr = Memory::MDS() + 0x40;
		page_LF[(pair.left + 0)] = LowHalf(ptr);
		page_LF[(pair.left + 1)] = HighHalf(ptr);
		page_MDS[(ptr - Memory::MDS() + pair.right)] = 0xABCD;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 3, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(0x0C, (int)stack[0]);
	}


	void testW0F() {
		FieldSpec spec;
		spec.pos = 8;
		spec.size = 3;
		page_CB[PC / 2] = zW0F << 8 | spec.u;
		UNSPEC data = 0x0A;
		POINTER ptr = 0x40;
		page_MDS[(ptr + 0)] = 0x1234;
		stack[SP++] = data;
		stack[SP++] = ptr;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)0x12A4, page_MDS[(ptr + 0)]);
	}


	void testWF() {
		FieldSpec spec;
		spec.pos = 8;
		spec.size = 3;
		FieldDesc desc;
		desc.field = spec.u;
		desc.offset = 0x10;
		page_CB[PC / 2] = zWF << 8 | desc.offset;
		page_CB[(PC / 2) + 1] = desc.field << 8 | 0x00;
		UNSPEC data = 0x0A;
		POINTER ptr = 0x40;
		page_MDS[(ptr + desc.offset)] = 0x1234;
		stack[SP++] = data;
		stack[SP++] = ptr;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 3, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)0x12A4, page_MDS[(ptr + desc.offset)]);
	}


	void testPSF() {
		FieldSpec spec;
		spec.pos = 8;
		spec.size = 3;
		FieldDesc desc;
		desc.field = spec.u;
		desc.offset = 0x10;
		page_CB[PC / 2] = zPSF << 8 | desc.offset;
		page_CB[(PC / 2) + 1] = desc.field << 8 | 0x00;
		POINTER ptr = 0x40;
		UNSPEC data = 0x0A;
		page_MDS[(ptr + desc.offset)] = 0x1234;
		stack[SP++] = ptr;
		stack[SP++] = data;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 3, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(ptr, stack[0]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0x12A4, page_MDS[(ptr + desc.offset)]);
	}


	void testPS0F() {
		FieldSpec spec;
		spec.pos = 8;
		spec.size = 3;
		page_CB[PC / 2] = zPS0F << 8 | spec.u;
		POINTER ptr = 0x40;
		UNSPEC data = 0x0A;
		page_MDS[(ptr + 0)] = 0x1234;
		stack[SP++] = ptr;
		stack[SP++] = data;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(ptr, stack[0]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0x12A4, page_MDS[(ptr + 0)]);
	}

	void testWS0F() {
		FieldSpec spec;
		spec.pos = 8;
		spec.size = 3;
		page_CB[PC / 2] = zWS0F << 8 | spec.u;
		POINTER ptr = 0x40;
		UNSPEC data = 0x0A;
		page_MDS[(ptr + 0)] = 0x1234;
		stack[SP++] = ptr;
		stack[SP++] = data;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)0x12A4, page_MDS[(ptr + 0)]);
	}


	void testWL0F() {
		FieldSpec spec;
		spec.pos = 8;
		spec.size = 3;
		page_CB[PC / 2] = zWL0F << 8 | spec.u;
		LONG_POINTER ptr = Memory::MDS() + 0x40;
		UNSPEC data = 0x0A;
		page_MDS[(ptr - Memory::MDS() + 0)] = 0x1234;
		stack[SP++] = data;
		stack[SP++] = LowHalf(ptr);
		stack[SP++] = HighHalf(ptr);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)0x12A4, page_MDS[(ptr - Memory::MDS() + 0)]);
	}


	void testWLF() {
		FieldSpec spec;
		spec.pos = 8;
		spec.size = 3;
		FieldDesc desc;
		desc.field = spec.u;
		desc.offset = 0x10;
		page_CB[PC / 2] = zWLF << 8 | desc.offset;
		page_CB[(PC / 2) + 1] = desc.field << 8 | 0x00;
		LONG_POINTER ptr = Memory::MDS() + 0x40;
		UNSPEC data = 0x0A;
		page_MDS[(ptr - Memory::MDS() + desc.offset)] = 0x1234;
		stack[SP++] = data;
		stack[SP++] = LowHalf(ptr);
		stack[SP++] = HighHalf(ptr);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 3, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)0x12A4, page_MDS[(ptr - Memory::MDS() + desc.offset)]);
	}


	void testPSLF() {
		FieldSpec spec;
		spec.pos = 8;
		spec.size = 3;
		FieldDesc desc;
		desc.field = spec.u;
		desc.offset = 0x10;
		page_CB[PC / 2] = zPSLF << 8 | desc.offset;
		page_CB[(PC / 2) + 1] = desc.field << 8 | 0x00;
		LONG_POINTER ptr = Memory::MDS() + 0x40;
		UNSPEC data = 0x0A;
		page_MDS[(ptr - Memory::MDS() + desc.offset)] = 0x1234;
		stack[SP++] = LowHalf(ptr);
		stack[SP++] = HighHalf(ptr);
		stack[SP++] = data;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 3, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(ptr),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(ptr), stack[1]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0x12A4, page_MDS[(ptr - Memory::MDS() + desc.offset)]);
	}


	void testWLFS() {
		FieldSpec spec;
		spec.pos = 8;
		spec.size = 3;
		FieldDesc desc;
		desc.field = spec.u;
		desc.offset = 0x10;
		page_CB[PC / 2] = zWLFS << 8 | 0x00;
		LONG_POINTER ptr = Memory::MDS() + 0x40;
		UNSPEC data = 0x0A;
		page_MDS[(ptr - Memory::MDS() + desc.offset)] = 0x1234;
		stack[SP++] = data;
		stack[SP++] = LowHalf(ptr);
		stack[SP++] = HighHalf(ptr);
		stack[SP++] = desc.u;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)0x12A4, page_MDS[(ptr - Memory::MDS() + desc.offset)]);
	}


	void testSLDB() {
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zSLDB << 8 | alpha;
		const CARD32 value = 0x12345678;
		stack[SP++] = LowHalf(value);
		stack[SP++] = HighHalf(value);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(value),  page_LF[(alpha + 0)]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(value), page_LF[(alpha + 1)]);
	}


	void testSGDB() {
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zSGDB << 8 | alpha;
		const CARD32 value = 0x12345678;
		stack[SP++] = LowHalf(value);
		stack[SP++] = HighHalf(value);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(value),  page_GF[(alpha + 0)]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(value), page_GF[(alpha + 1)]);
	}


	void testLLKB_cl() {
		// test for code link
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zLLKB << 8 | alpha;
		CARD32 link = 0xcafebabe;
		page_GF[-1] = 0x0001; // [word] = [codelink: 1];
		page_CB[-(alpha + 1) * 2 + 0] = LowHalf(link); // CB - (CARD32)(offset + 1) * 2
		page_CB[-(alpha + 1) * 2 + 1] = HighHalf(link);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(link),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(link), stack[1]);
	}
	void testLLKB_fl() {
		// test for frame link
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zLLKB << 8 | alpha;
		CARD32 link = 0xcafebabe;
		page_GF[-1] = 0x0000; // [word] = [codelink: 0];
		page_GF[-2 -(alpha + 1) * 2 + 0] = LowHalf(link); // GlobalBase(GF) - (offset + 1) * 2
		page_GF[-2 -(alpha + 1) * 2 + 1] = HighHalf(link);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(link),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(link), stack[1]);
	}


	void testRKIB() {
		// test for frame link
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zRKIB << 8 | alpha;
		CARD32 link = Memory::MDS() + 0x60;
		CARD16 value = 0xcafe;
		page_GF[-1] = 0x0000; // [word] = [codelink: 0];
		page_GF[-2 -(alpha + 1) * 2 + 0] = LowHalf(link); // GlobalBase(GF) - (offset + 1) * 2
		page_GF[-2 -(alpha + 1) * 2 + 1] = HighHalf(link);
		page_MDS[link - Memory::MDS() + 0] = value;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value,  stack[0]);
	}


	void testRKDIB() {
		// test for frame link
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zRKDIB << 8 | alpha;
		CARD32 link = Memory::MDS() + 0x60;
		CARD32 value = 0xcafebabe;
		page_GF[-1] = 0x0000; // [word] = [codelink: 0];
		page_GF[-2 -(alpha + 1) * 2 + 0] = LowHalf(link); // GlobalBase(GF) - (offset + 1) * 2
		page_GF[-2 -(alpha + 1) * 2 + 1] = HighHalf(link);
		page_MDS[link - Memory::MDS() + 0] = LowHalf(value);
		page_MDS[link - Memory::MDS() + 1] = HighHalf(value);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(value),   stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(value),  stack[1]);
	}


	void testLKB() {
		CARD8 alpha = 4;
		page_CB[PC / 2] = zLKB << 8 | alpha;
		CARD16 link = 0x1004;
		stack[SP] = link;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)(link - alpha), page_LF[(0)]);
	}


	void testSHIFT_p() {
		page_CB[PC / 2] = zSHIFT << 8 | 0x00;
		INT16 shift = 4;
		CARD16 u = 0x1234;
		stack[SP++] = u;
		stack[SP++] = shift;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)(u << shift), stack[0]);
	}
	void testSHIFT_n() {
		page_CB[PC / 2] = zSHIFT << 8 | 0x00;
		INT16 shift = -4;
		CARD16 u = 0x1234;
		stack[SP++] = u;
		stack[SP++] = shift;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)(u >> -shift), stack[0]);
	}


	void testSHIFTSB_p() {
		INT8 alpha = 4;
		page_CB[PC / 2] = zSHIFTSB << 8 | (CARD8)alpha;
		CARD16 u = 0x1234;
		stack[SP++] = u;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)(u << alpha), stack[0]);
	}
	void testSHIFTSB_n() {
		INT8 alpha = -4;
		page_CB[PC / 2] = zSHIFTSB << 8 | (CARD8)alpha;
		CARD16 u = 0x1234;
		stack[SP++] = u;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)(u >> -alpha), stack[0]);
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION(testOpcode_100);
