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
// tetOpcode_000.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("test000");

#include "testBase.h"

class testOpcode_000 : public testBase {
	CPPUNIT_TEST_SUITE(testOpcode_000);

	CPPUNIT_TEST(testNOOP); // 0000
	CPPUNIT_TEST(testLL0);  // 0001
	CPPUNIT_TEST(testLL1);  // 0002
	CPPUNIT_TEST(testLL2);  // 0003
	CPPUNIT_TEST(testLL3);  // 0004
	CPPUNIT_TEST(testLL4);  // 0005
	CPPUNIT_TEST(testLL5);  // 0006
	CPPUNIT_TEST(testLL6);  // 0007

	CPPUNIT_TEST(testLL7);  // 0010
	CPPUNIT_TEST(testLL8);  // 0011
	CPPUNIT_TEST(testLL9);  // 0012
	CPPUNIT_TEST(testLL10); // 0013
	CPPUNIT_TEST(testLL11); // 0014
	CPPUNIT_TEST(testLLB);  // 0015
	CPPUNIT_TEST(testLLD0); // 0016
	CPPUNIT_TEST(testLLD1); // 0017

	CPPUNIT_TEST(testLLD2);  // 0020
	CPPUNIT_TEST(testLLD3);  // 0021
	CPPUNIT_TEST(testLLD4);  // 0022
	CPPUNIT_TEST(testLLD5);  // 0023
	CPPUNIT_TEST(testLLD6);  // 0024
	CPPUNIT_TEST(testLLD7);  // 0025
	CPPUNIT_TEST(testLLD8);  // 0026
	CPPUNIT_TEST(testLLD10); // 0027

	CPPUNIT_TEST(testLLDB); // 0030
	CPPUNIT_TEST(testSL0);  // 0031
	CPPUNIT_TEST(testSL1);  // 0032
	CPPUNIT_TEST(testSL2);  // 0033
	CPPUNIT_TEST(testSL3);  // 0034
	CPPUNIT_TEST(testSL4);  // 0035
	CPPUNIT_TEST(testSL5);  // 0036
	CPPUNIT_TEST(testSL6);  // 0037

	CPPUNIT_TEST(testSL7);  // 0040
	CPPUNIT_TEST(testSL8);  // 0041
	CPPUNIT_TEST(testSL9);  // 0042
	CPPUNIT_TEST(testSL10); // 0043
	CPPUNIT_TEST(testSLB);  // 0044
	CPPUNIT_TEST(testSLD0); // 0045
	CPPUNIT_TEST(testSLD1); // 0046
	CPPUNIT_TEST(testSLD2); // 0047

	CPPUNIT_TEST(testSLD3); // 0050
	CPPUNIT_TEST(testSLD4); // 0051
	CPPUNIT_TEST(testSLD5); // 0052
	CPPUNIT_TEST(testSLD6); // 0053
	CPPUNIT_TEST(testSLD8); // 0054
	CPPUNIT_TEST(testPL0);  // 0055
	CPPUNIT_TEST(testPL1);  // 0056
	CPPUNIT_TEST(testPL2);  // 0057

	CPPUNIT_TEST(testPL3);  // 0060
	CPPUNIT_TEST(testPLB);  // 0061
	CPPUNIT_TEST(testPLD0); // 0062
	CPPUNIT_TEST(testPLDB); // 0063
	CPPUNIT_TEST(testLG0);  // 0064
	CPPUNIT_TEST(testLG1);  // 0065
	CPPUNIT_TEST(testLG2);  // 0066
	CPPUNIT_TEST(testLGB);  // 0067

	CPPUNIT_TEST(testLGD0);    // 0070
	CPPUNIT_TEST(testLGD2);    // 0071
	CPPUNIT_TEST(testLGDB);    // 0072
	CPPUNIT_TEST(testSGB);     // 0073
	CPPUNIT_TEST(testBNDCK_n); // 0074
	CPPUNIT_TEST(testBNDCK_t); // 0074
	CPPUNIT_TEST(testBRK_n);   // 0075
	CPPUNIT_TEST(testBRK_t);   // 0075
	//
//  CPPUNIT_TEST(testSTC);   // 0077

	CPPUNIT_TEST_SUITE_END();

	///////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////

	void testNOOP() {}

	void testLL0() {
		page_CB[PC / 2] = zLL0 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_LF[0], stack[0]);
	}
	void testLL1() {
		page_CB[PC / 2] = zLL1 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_LF[1], stack[0]);
	}
	void testLL2() {
		page_CB[PC / 2] = zLL2 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_LF[2], stack[0]);
	}
	void testLL3() {
		page_CB[PC / 2] = zLL3 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_LF[3], stack[0]);
	}
	void testLL4() {
		page_CB[PC / 2] = zLL4 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_LF[4], stack[0]);
	}
	void testLL5() {
		page_CB[PC / 2] = zLL5 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_LF[5], stack[0]);
	}
	void testLL6() {
		page_CB[PC / 2] = zLL6 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_LF[6], stack[0]);
	}
	void testLL7() {
		page_CB[PC / 2] = zLL7 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_LF[7], stack[0]);
	}
	void testLL8() {
		page_CB[PC / 2] = zLL8 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_LF[8], stack[0]);
	}
	void testLL9() {
		page_CB[PC / 2] = zLL9 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_LF[9], stack[0]);
	}
	void testLL10() {
		page_CB[PC / 2] = zLL10 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_LF[10], stack[0]);
	}
	void testLL11() {
		page_CB[PC / 2] = zLL11 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_LF[11], stack[0]);
	}
	void testLLB() {
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zLLB << 8 | alpha;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_LF[alpha], stack[0]);
	}


	void testLLD0() {
		page_CB[PC / 2] = zLLD0 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_LF[0], stack[0]);
		CPPUNIT_ASSERT_EQUAL(page_LF[1], stack[1]);
	}
	void testLLD1() {
		page_CB[PC / 2] = zLLD1 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_LF[1 + 0], stack[0]);
		CPPUNIT_ASSERT_EQUAL(page_LF[1 + 1], stack[1]);
	}
	void testLLD2() {
		page_CB[PC / 2] = zLLD2 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_LF[2 + 0], stack[0]);
		CPPUNIT_ASSERT_EQUAL(page_LF[2 + 1], stack[1]);
	}
	void testLLD3() {
		page_CB[PC / 2] = zLLD3 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_LF[3 + 0], stack[0]);
		CPPUNIT_ASSERT_EQUAL(page_LF[3 + 1], stack[1]);
	}
	void testLLD4() {
		page_CB[PC / 2] = zLLD4 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_LF[4 + 0], stack[0]);
		CPPUNIT_ASSERT_EQUAL(page_LF[4 + 1], stack[1]);
	}
	void testLLD5() {
		page_CB[PC / 2] = zLLD5 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_LF[5 + 0], stack[0]);
		CPPUNIT_ASSERT_EQUAL(page_LF[5 + 1], stack[1]);
	}
	void testLLD6() {
		page_CB[PC / 2] = zLLD6 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_LF[6 + 0], stack[0]);
		CPPUNIT_ASSERT_EQUAL(page_LF[6 + 1], stack[1]);
	}
	void testLLD7() {
		page_CB[PC / 2] = zLLD7 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_LF[7 + 0], stack[0]);
		CPPUNIT_ASSERT_EQUAL(page_LF[7 + 1], stack[1]);
	}
	void testLLD8() {
		page_CB[PC / 2] = zLLD8 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_LF[8 + 0], stack[0]);
		CPPUNIT_ASSERT_EQUAL(page_LF[8 + 1], stack[1]);
	}
	void testLLD10() {
		page_CB[PC / 2] = zLLD10 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_LF[10 + 0], stack[0]);
		CPPUNIT_ASSERT_EQUAL(page_LF[10 + 1], stack[1]);
	}
	void testLLDB() {
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zLLDB << 8 | alpha;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_LF[alpha + 0], stack[0]);
		CPPUNIT_ASSERT_EQUAL(page_LF[alpha + 1], stack[1]);
	}


	void testSL0() {
		page_CB[PC / 2] = zSL0 << 8 | 0x00;
		const CARD16 value = 0x1234;
		stack[SP++] = value;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value, page_LF[0]);
	}
	void testSL1() {
		page_CB[PC / 2] = zSL1 << 8 | 0x00;
		const CARD16 value = 0x1234;
		stack[SP++] = value;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value, page_LF[1]);
	}
	void testSL2() {
		page_CB[PC / 2] = zSL2 << 8 | 0x00;
		const CARD16 value = 0x1234;
		stack[SP++] = value;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value, page_LF[2]);
	}
	void testSL3() {
		page_CB[PC / 2] = zSL3 << 8 | 0x00;
		const CARD16 value = 0x1234;
		stack[SP++] = value;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value, page_LF[3]);
	}
	void testSL4() {
		page_CB[PC / 2] = zSL4 << 8 | 0x00;
		const CARD16 value = 0x1234;
		stack[SP++] = value;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value, page_LF[4]);
	}
	void testSL5() {
		page_CB[PC / 2] = zSL5 << 8 | 0x00;
		const CARD16 value = 0x1234;
		stack[SP++] = value;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value, page_LF[5]);
	}
	void testSL6() {
		page_CB[PC / 2] = zSL6 << 8 | 0x00;
		const CARD16 value = 0x1234;
		stack[SP++] = value;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value, page_LF[6]);
	}
	void testSL7() {
		page_CB[PC / 2] = zSL7 << 8 | 0x00;
		const CARD16 value = 0x1234;
		stack[SP++] = value;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value, page_LF[7]);
	}
	void testSL8() {
		page_CB[PC / 2] = zSL8 << 8 | 0x00;
		const CARD16 value = 0x1234;
		stack[SP++] = value;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value, page_LF[8]);
	}
	void testSL9() {
		page_CB[PC / 2] = zSL9 << 8 | 0x00;
		const CARD16 value = 0x1234;
		stack[SP++] = value;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value, page_LF[9]);
	}
	void testSL10() {
		page_CB[PC / 2] = zSL10 << 8 | 0x00;
		const CARD16 value = 0x1234;
		stack[SP++] = value;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value, page_LF[10]);
	}
	void testSLB() {
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zSLB << 8 | alpha;
		const CARD16 value = 0x1234;
		stack[SP++] = value;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value, page_LF[alpha]);
	}


	void testSLD0() {
		page_CB[PC / 2] = zSLD0 << 8 | 0x00;
		const CARD32 value = 0x12345678;
		stack[SP++] = LowHalf(value);
		stack[SP++] = HighHalf(value);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(value),  page_LF[0 + 0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(value), page_LF[0 + 1]);
	}
	void testSLD1() {
		page_CB[PC / 2] = zSLD1 << 8 | 0x00;
		const CARD32 value = 0x12345678;
		stack[SP++] = LowHalf(value);
		stack[SP++] = HighHalf(value);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(value),  page_LF[1 + 0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(value), page_LF[1 + 1]);
	}
	void testSLD2() {
		page_CB[PC / 2] = zSLD2 << 8 | 0x00;
		const CARD32 value = 0x12345678;
		stack[SP++] = LowHalf(value);
		stack[SP++] = HighHalf(value);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(value),  page_LF[2 + 0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(value), page_LF[2 + 1]);
	}
	void testSLD3() {
		page_CB[PC / 2] = zSLD3 << 8 | 0x00;
		const CARD32 value = 0x12345678;
		stack[SP++] = LowHalf(value);
		stack[SP++] = HighHalf(value);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(value),  page_LF[3 + 0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(value), page_LF[3 + 1]);
	}
	void testSLD4() {
		page_CB[PC / 2] = zSLD4 << 8 | 0x00;
		const CARD32 value = 0x12345678;
		stack[SP++] = LowHalf(value);
		stack[SP++] = HighHalf(value);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(value),  page_LF[4 + 0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(value), page_LF[4 + 1]);
	}
	void testSLD5() {
		page_CB[PC / 2] = zSLD5 << 8 | 0x00;
		const CARD32 value = 0x12345678;
		stack[SP++] = LowHalf(value);
		stack[SP++] = HighHalf(value);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(value),  page_LF[5 + 0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(value), page_LF[5 + 1]);
	}
	void testSLD6() {
		page_CB[PC / 2] = zSLD6 << 8 | 0x00;
		const CARD32 value = 0x12345678;
		stack[SP++] = LowHalf(value);
		stack[SP++] = HighHalf(value);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(value),  page_LF[6 + 0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(value), page_LF[6 + 1]);
	}
	void testSLD8() {
		page_CB[PC / 2] = zSLD8 << 8 | 0x00;
		const CARD32 value = 0x12345678;
		stack[SP++] = LowHalf(value);
		stack[SP++] = HighHalf(value);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(value),  page_LF[8 + 0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(value), page_LF[8 + 1]);
	}


	void testPL0() {
		page_CB[PC / 2] = zPL0 << 8 | 0x00;
		const CARD16 value = 0x1234;
		stack[SP++] = value;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value, stack[0]);
		CPPUNIT_ASSERT_EQUAL(value, page_LF[0]);
	}
	void testPL1() {
		page_CB[PC / 2] = zPL1 << 8 | 0x00;
		const CARD16 value = 0x1234;
		stack[SP++] = value;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value, stack[0]);
		CPPUNIT_ASSERT_EQUAL(value, page_LF[1]);
	}
	void testPL2() {
		page_CB[PC / 2] = zPL2 << 8 | 0x00;
		const CARD16 value = 0x1234;
		stack[SP++] = value;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value, stack[0]);
		CPPUNIT_ASSERT_EQUAL(value, page_LF[2]);
	}
	void testPL3() {
		page_CB[PC / 2] = zPL3 << 8 | 0x00;
		const CARD16 value = 0x1234;
		stack[SP++] = value;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value, stack[0]);
		CPPUNIT_ASSERT_EQUAL(value, page_LF[3]);
	}
	void testPLB() {
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zPLB << 8 | alpha;
		const CARD16 value = 0x1234;
		stack[SP++] = value;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value, stack[0]);
		CPPUNIT_ASSERT_EQUAL(value, page_LF[alpha]);
	}


	void testPLD0() {
		page_CB[PC / 2] = zPLD0 << 8 | 0x00;
		const CARD32 value = 0x12345678;
		stack[SP++] = LowHalf(value);
		stack[SP++] = HighHalf(value);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(value),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(value), stack[1]);
		CPPUNIT_ASSERT_EQUAL(LowHalf(value),  page_LF[0 + 0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(value), page_LF[0 + 1]);
	}
	void testPLDB() {
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zPLDB << 8 | alpha;
		const CARD32 value = 0x12345678;
		stack[SP++] = LowHalf(value);
		stack[SP++] = HighHalf(value);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(value),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(value), stack[1]);
		CPPUNIT_ASSERT_EQUAL(LowHalf(value),  page_LF[alpha + 0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(value), page_LF[alpha + 1]);
	}


	void testLG0() {
		page_CB[PC / 2] = zLG0 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_GF[0], stack[0]);
	}
	void testLG1() {
		page_CB[PC / 2] = zLG1 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_GF[1], stack[0]);
	}
	void testLG2() {
		page_CB[PC / 2] = zLG2 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_GF[2], stack[0]);
	}
	void testLGB() {
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zLGB << 8 | alpha;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_GF[alpha], stack[0]);
	}


	void testLGD0() {
		page_CB[PC / 2] = zLGD0 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_GF[0 + 0], stack[0]);
		CPPUNIT_ASSERT_EQUAL(page_GF[0 + 1], stack[1]);
	}
	void testLGD2() {
		page_CB[PC / 2] = zLGD2 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_GF[2 + 0], stack[0]);
		CPPUNIT_ASSERT_EQUAL(page_GF[2 + 1], stack[1]);
	}
	void testLGDB() {
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zLGDB << 8 | alpha;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_GF[alpha + 0], stack[0]);
		CPPUNIT_ASSERT_EQUAL(page_GF[alpha + 1], stack[1]);
	}


	void testSGB() {
		CARD8 alpha = 0x20;
		page_CB[PC / 2] = zSGB << 8 | alpha;
		const CARD16 value = 0x1234;
		stack[SP++] = value;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value, page_GF[alpha]);
	}


	void testBNDCK_n() {
		// normal case - don't generate BoundsTrap
		page_CB[PC / 2] = zBNDCK << 8 | 0x00;
		const CARD16 range = 0x2222;
		const CARD16 index = 0x1111;
		stack[SP++] = index;
		stack[SP++] = range;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(index, stack[0]);
	}
	void testBNDCK_t() {
		page_CB[PC / 2] = zBNDCK << 8 | 0x00;
		const CARD16 range = 0x1111;
		const CARD16 index = 0x2222;
		stack[SP++] = index;
		stack[SP++] = range;

		int catchException = 0;
		try {
			Interpreter::execute();
		} catch (Abort &info) {
			catchException = 1;
		}

		CPPUNIT_ASSERT_EQUAL(1, catchException);
		CPPUNIT_ASSERT_EQUAL(pc_SD + sBoundsTrap + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(GFI_SD, GFI);
	}


	void testBRK_n() {
		breakByte = zLL0;
		page_CB[PC / 2] = zBRK << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_LF[0], stack[0]);
		CPPUNIT_ASSERT_EQUAL(0, (int)breakByte);
	}
	void testBRK_t() {
		breakByte = 0;
		page_CB[PC / 2] = zBRK << 8 | 0x00;
		int catchException = 0;
		try {
			Interpreter::execute();
		} catch (Abort &info) {
			//logger.debug("Abort  func = %-20s line = %4d file = %s\n", info.func, info.line, info.file);
			catchException = 1;
		}

		CPPUNIT_ASSERT_EQUAL(1, catchException);
		CPPUNIT_ASSERT_EQUAL(pc_SD + sBreakTrap + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(GFI_SD, GFI);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(testOpcode_000);
