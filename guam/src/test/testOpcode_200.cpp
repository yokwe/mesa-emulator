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
// testOpcode_200.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("test200");

#include "testBase.h"

class testOpcode_200 : public testBase {
	CPPUNIT_TEST_SUITE(testOpcode_200);

	CPPUNIT_TEST(testCATCH); // 0200
	CPPUNIT_TEST(testJ2);    // 0201
	CPPUNIT_TEST(testJ3);    // 0202
	CPPUNIT_TEST(testJ4);    // 0203
	CPPUNIT_TEST(testJ5);    // 0204
	CPPUNIT_TEST(testJ6);    // 0205
	CPPUNIT_TEST(testJ7);    // 0206
	CPPUNIT_TEST(testJ8);    // 0207

	CPPUNIT_TEST(testJB_p);     // 0210
	CPPUNIT_TEST(testJB_n);     // 0210
	CPPUNIT_TEST(testJW_p);     // 0211
	CPPUNIT_TEST(testJW_n);     // 0211
	CPPUNIT_TEST(testJEP_j);    // 0212
	CPPUNIT_TEST(testJEP_nj);   // 0212
	CPPUNIT_TEST(testJEB_jp);   // 0213
	CPPUNIT_TEST(testJEB_jn);   // 0213
	CPPUNIT_TEST(testJEB_nj);   // 0213
	CPPUNIT_TEST(testJEBB_jp);  // 0214
	CPPUNIT_TEST(testJEBB_jn);  // 0214
	CPPUNIT_TEST(testJEBB_nj);  // 0214
	CPPUNIT_TEST(testJNEP_j);   // 0215
	CPPUNIT_TEST(testJNEP_nj);  // 0215
	CPPUNIT_TEST(testJNEB_jp);  // 0216
	CPPUNIT_TEST(testJNEB_jn);  // 0216
	CPPUNIT_TEST(testJNEB_nj);  // 0216
	CPPUNIT_TEST(testJNEBB_jp); // 0217
	CPPUNIT_TEST(testJNEBB_jn); // 0217
	CPPUNIT_TEST(testJNEBB_nj); // 0217

	CPPUNIT_TEST(testJLB_jp);   // 0220
	CPPUNIT_TEST(testJLB_jn);   // 0220
	CPPUNIT_TEST(testJLB_nj);   // 0220
	CPPUNIT_TEST(testJLB_eq);   // 0220
	CPPUNIT_TEST(testJGEB_jp);  // 0221
	CPPUNIT_TEST(testJGEB_jn);  // 0221
	CPPUNIT_TEST(testJGEB_nj);  // 0221
	CPPUNIT_TEST(testJGEB_eq);  // 0221
	CPPUNIT_TEST(testJGB_jp);   // 0222
	CPPUNIT_TEST(testJGB_jn);   // 0222
	CPPUNIT_TEST(testJGB_nj);   // 0222
	CPPUNIT_TEST(testJGB_eq);   // 0222
	CPPUNIT_TEST(testJLEB_jp);  // 0223
	CPPUNIT_TEST(testJLEB_jn);  // 0223
	CPPUNIT_TEST(testJLEB_nj);  // 0223
	CPPUNIT_TEST(testJLEB_eq);  // 0223
	CPPUNIT_TEST(testJULB_jp);  // 0224
	CPPUNIT_TEST(testJULB_jn);  // 0224
	CPPUNIT_TEST(testJULB_nj);  // 0224
	CPPUNIT_TEST(testJULB_eq);  // 0224
	CPPUNIT_TEST(testJUGEB_jp); // 0225
	CPPUNIT_TEST(testJUGEB_jn); // 0225
	CPPUNIT_TEST(testJUGEB_nj); // 0225
	CPPUNIT_TEST(testJUGEB_eq); // 0225
	CPPUNIT_TEST(testJUGB_jp);  // 0226
	CPPUNIT_TEST(testJUGB_jn);  // 0226
	CPPUNIT_TEST(testJUGB_nj);  // 0226
	CPPUNIT_TEST(testJUGB_eq);  // 0226
	CPPUNIT_TEST(testJULEB_jp); // 0227
	CPPUNIT_TEST(testJULEB_jn); // 0227
	CPPUNIT_TEST(testJULEB_nj); // 0227
	CPPUNIT_TEST(testJULEB_eq); // 0227

	CPPUNIT_TEST(testJZ3_j);    // 0230
	CPPUNIT_TEST(testJZ3_nj);   // 0230
	CPPUNIT_TEST(testJZ4_j);    // 0231
	CPPUNIT_TEST(testJZ4_nj);   // 0231
	CPPUNIT_TEST(testJZB_jp);   // 0232
	CPPUNIT_TEST(testJZB_jn);   // 0232
	CPPUNIT_TEST(testJZB_nj);   // 0232
	CPPUNIT_TEST(testJNZ3_j);   // 0233
	CPPUNIT_TEST(testJNZ3_nj);  // 0233
	CPPUNIT_TEST(testJNZ4_j);   // 0234
	CPPUNIT_TEST(testJNZ4_nj);  // 0234
	CPPUNIT_TEST(testJNZB_jp);  // 0235
	CPPUNIT_TEST(testJNZB_jn);  // 0235
	CPPUNIT_TEST(testJNZB_nj);  // 0235
	CPPUNIT_TEST(testJDEB_jp);  // 0236
	CPPUNIT_TEST(testJDEB_jn);  // 0236
	CPPUNIT_TEST(testJDEB_nj);  // 0236
	CPPUNIT_TEST(testJDNEB_jp); // 0237
	CPPUNIT_TEST(testJDNEB_jn); // 0237
	CPPUNIT_TEST(testJDNEB_nj); // 0237

	CPPUNIT_TEST(testJIB_j);  // 0240
	CPPUNIT_TEST(testJIB_nj); // 0240
	CPPUNIT_TEST(testJIW_j);  // 0241
	CPPUNIT_TEST(testJIW_nj); // 0241
	CPPUNIT_TEST(testREC);    // 0242
	CPPUNIT_TEST(testREC2);   // 0243
	CPPUNIT_TEST(testDIS);    // 0244
	CPPUNIT_TEST(testDIS2);   // 0245
	CPPUNIT_TEST(testEXCH);   // 0246
	CPPUNIT_TEST(testDEXCH);  // 0247

	CPPUNIT_TEST(testDUP);   // 0250
	CPPUNIT_TEST(testDDUP);  // 0251
	CPPUNIT_TEST(testEXDIS); // 0252
	CPPUNIT_TEST(testNEG);   // 0253
	CPPUNIT_TEST(testINC);   // 0254
	CPPUNIT_TEST(testDEC);   // 0255
	CPPUNIT_TEST(testDINC);  // 0256
	CPPUNIT_TEST(testDBL);   // 0257

	CPPUNIT_TEST(testDDBL);    // 0260
	CPPUNIT_TEST(testTRPL);    // 0261
	CPPUNIT_TEST(testAND);     // 0262
	CPPUNIT_TEST(testIOR);     // 0263
	CPPUNIT_TEST(testADDSB_p); // 0264
	CPPUNIT_TEST(testADDSB_n); // 0264
	CPPUNIT_TEST(testADD);     // 0265
	CPPUNIT_TEST(testSUB);     // 0266
	CPPUNIT_TEST(testDADD);    // 0267

	CPPUNIT_TEST(testDSUB);     // 0270
	CPPUNIT_TEST(testADC);      // 0271
	CPPUNIT_TEST(testACD);      // 0272
	CPPUNIT_TEST(testAL0IB);    // 0273
	CPPUNIT_TEST(testMUL);      // 0274
	CPPUNIT_TEST(testDCMP_gt);  // 0275
	CPPUNIT_TEST(testDCMP_lt);  // 0275
	CPPUNIT_TEST(testDCMP_eq);  // 0275
	CPPUNIT_TEST(testUDCMP_gt); // 0276
	CPPUNIT_TEST(testUDCMP_lt); // 0276
	CPPUNIT_TEST(testUDCMP_eq); // 0276
//  CPPUNIT_TEST(testVMFIND);   // 0277

	CPPUNIT_TEST_SUITE_END();

	///////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////

	void testCATCH() {
		page_CB[PC / 2] = zCATCH << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}

	void testJ2() {
		page_CB[PC / 2] = zJ2 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJ3() {
		page_CB[PC / 2] = zJ3 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 3, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJ4() {
		page_CB[PC / 2] = zJ4 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 4, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJ5() {
		page_CB[PC / 2] = zJ5 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 5, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJ6() {
		page_CB[PC / 2] = zJ6 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 6, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJ7() {
		page_CB[PC / 2] = zJ7 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 7, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJ8() {
		page_CB[PC / 2] = zJ8 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 8, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}


	void testJB_p() {
		INT8 alpha = 4;
		page_CB[PC / 2] = zJB << 8 | (CARD8)alpha;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJB_n() {
		INT8 alpha = -4;
		page_CB[PC / 2] = zJB << 8 | (CARD8)alpha;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}


	void testJW_p() {
		BytePair alpha = {16};
		page_CB[PC / 2] = zJW << 8 | (CARD8)alpha.left;
		page_CB[(PC / 2) + 1] = (CARD8)(alpha.right) << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + (INT16)alpha.u, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJW_n() {
		BytePair alpha = {(CARD16)-16};
		page_CB[PC / 2] = zJW << 8 | (CARD8)alpha.left;
		page_CB[(PC / 2) + 1] = (CARD8)(alpha.right) << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL((INT16)savedPC + (INT16)alpha.u, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}


	void testJEP_j() {
		NibblePair alpha;
		alpha.left = 4;
		alpha.right = 10;
		page_CB[PC / 2] = zJEP << 8 | (CARD8)alpha.u;
		CARD16 data = 4;
		stack[SP++] = data;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha.right + 4, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJEP_nj() {
		NibblePair alpha;
		alpha.left = 4;
		alpha.right = 10;
		page_CB[PC / 2] = zJEP << 8 | (CARD8)alpha.u;
		CARD16 data = 5;
		stack[SP++] = data;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}


	void testJEB_jp() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJEB << 8 | (CARD8)alpha;
		CARD16 u = 0x1111;
		CARD16 v = 0x1111;
		stack[SP++] = u;
		stack[SP++] = v;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJEB_jn() {
		INT8 alpha = -16;
		page_CB[PC / 2] = zJEB << 8 | (CARD8)alpha;
		CARD16 u = 0x1111;
		CARD16 v = 0x1111;
		stack[SP++] = u;
		stack[SP++] = v;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJEB_nj() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJEB << 8 | (CARD8)alpha;
		CARD16 u = 0x1111;
		CARD16 v = 0x2222;
		stack[SP++] = u;
		stack[SP++] = v;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}


	void testJEBB_jp() {
		CARD8 byte = 0x12;
		INT8 disp = 16;
		page_CB[PC / 2] = zJEBB << 8 | byte;
		page_CB[(PC / 2) + 1] = (CARD8)(disp) << 8 | 0x00;
		CARD16 data = 0x12;
		stack[SP++] = data;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + disp, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJEBB_jn() {
		CARD8 byte = 0x12;
		INT8 disp = -16;
		page_CB[PC / 2] = zJEBB << 8 | byte;
		page_CB[(PC / 2) + 1] = (CARD8)(disp) << 8 | 0x00;
		CARD16 data = 0x12;
		stack[SP++] = data;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + disp, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJEBB_nj() {
		CARD8 byte = 0x12;
		INT8 disp = -16;
		page_CB[PC / 2] = zJEBB << 8 | byte;
		page_CB[(PC / 2) + 1] = (CARD8)(disp) << 8 | 0x00;
		CARD16 data = 0x22;
		stack[SP++] = data;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 3, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}


	void testJNEP_j() {
		NibblePair alpha;
		alpha.left = 4;
		alpha.right = 10;
		page_CB[PC / 2] = zJNEP << 8 | (CARD8)alpha.u;
		CARD16 data = 5;
		stack[SP++] = data;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha.right + 4, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJNEP_nj() {
		NibblePair alpha;
		alpha.left = 4;
		alpha.right = 10;
		page_CB[PC / 2] = zJNEP << 8 | (CARD8)alpha.u;
		CARD16 data = 4;
		stack[SP++] = data;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}


	void testJNEB_jp() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJNEB << 8 | (CARD8)alpha;
		CARD16 u = 0x1111;
		CARD16 v = 0x2222;
		stack[SP++] = u;
		stack[SP++] = v;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJNEB_jn() {
		INT8 alpha = -16;
		page_CB[PC / 2] = zJNEB << 8 | (CARD8)alpha;
		CARD16 u = 0x1111;
		CARD16 v = 0x2222;
		stack[SP++] = u;
		stack[SP++] = v;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJNEB_nj() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJNEB << 8 | (CARD8)alpha;
		CARD16 u = 0x1111;
		CARD16 v = 0x1111;
		stack[SP++] = u;
		stack[SP++] = v;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}


	void testJNEBB_jp() {
		CARD8 byte = 0x22;
		INT8 disp = 16;
		page_CB[PC / 2] = zJNEBB << 8 | byte;
		page_CB[(PC / 2) + 1] = (CARD8)(disp) << 8 | 0x00;
		CARD16 data = 0x12;
		stack[SP++] = data;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + disp, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJNEBB_jn() {
		CARD8 byte = 0x22;
		INT8 disp = -16;
		page_CB[PC / 2] = zJNEBB << 8 | byte;
		page_CB[(PC / 2) + 1] = (CARD8)(disp) << 8 | 0x00;
		CARD16 data = 0x12;
		stack[SP++] = data;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + disp, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJNEBB_nj() {
		CARD8 byte = 0x12;
		INT8 disp = -16;
		page_CB[PC / 2] = zJNEBB << 8 | byte;
		page_CB[(PC / 2) + 1] = (CARD8)(disp) << 8 | 0x00;
		CARD16 data = 0x12;
		stack[SP++] = data;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 3, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}


	void testJLB_jp() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJLB << 8 | (CARD8)alpha;
		INT16 k = 0x2222;
		INT16 j = 0x1111;
		stack[SP++] = j;
		stack[SP++] = k;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJLB_jn() {
		INT8 alpha = -16;
		page_CB[PC / 2] = zJLB << 8 | (CARD8)alpha;
		INT16 k = 0x2222;
		INT16 j = 0x1111;
		stack[SP++] = j;
		stack[SP++] = k;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJLB_nj() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJLB << 8 | (CARD8)alpha;
		INT16 k = 0x1111;
		INT16 j = 0x2222;
		stack[SP++] = j;
		stack[SP++] = k;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJLB_eq() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJLB << 8 | (CARD8)alpha;
		INT16 k = 0x1111;
		INT16 j = 0x1111;
		stack[SP++] = j;
		stack[SP++] = k;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}


	void testJGEB_jp() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJGEB << 8 | (CARD8)alpha;
		INT16 k = 0x1111;
		INT16 j = 0x2222;
		stack[SP++] = j;
		stack[SP++] = k;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJGEB_jn() {
		INT8 alpha = -16;
		page_CB[PC / 2] = zJGEB << 8 | (CARD8)alpha;
		INT16 k = 0x1111;
		INT16 j = 0x2222;
		stack[SP++] = j;
		stack[SP++] = k;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJGEB_nj() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJGEB << 8 | (CARD8)alpha;
		INT16 k = 0x2222;
		INT16 j = 0x1111;
		stack[SP++] = j;
		stack[SP++] = k;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJGEB_eq() {
		INT8 alpha = -16;
		page_CB[PC / 2] = zJGEB << 8 | (CARD8)alpha;
		INT16 k = 0x1111;
		INT16 j = 0x1111;
		stack[SP++] = j;
		stack[SP++] = k;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}


	void testJGB_jp() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJGB << 8 | (CARD8)alpha;
		INT16 k = 0x1111;
		INT16 j = 0x2222;
		stack[SP++] = j;
		stack[SP++] = k;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJGB_jn() {
		INT8 alpha = -16;
		page_CB[PC / 2] = zJGB << 8 | (CARD8)alpha;
		INT16 k = 0x1111;
		INT16 j = 0x2222;
		stack[SP++] = j;
		stack[SP++] = k;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJGB_nj() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJGB << 8 | (CARD8)alpha;
		INT16 k = 0x2222;
		INT16 j = 0x1111;
		stack[SP++] = j;
		stack[SP++] = k;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJGB_eq() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJGB << 8 | (CARD8)alpha;
		INT16 k = 0x1111;
		INT16 j = 0x1111;
		stack[SP++] = j;
		stack[SP++] = k;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}


	void testJLEB_jp() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJLEB << 8 | (CARD8)alpha;
		INT16 k = 0x2222;
		INT16 j = 0x1111;
		stack[SP++] = j;
		stack[SP++] = k;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJLEB_jn() {
		INT8 alpha = -16;
		page_CB[PC / 2] = zJLEB << 8 | (CARD8)alpha;
		INT16 k = 0x2222;
		INT16 j = 0x1111;
		stack[SP++] = j;
		stack[SP++] = k;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJLEB_nj() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJLEB << 8 | (CARD8)alpha;
		INT16 k = 0x1111;
		INT16 j = 0x2222;
		stack[SP++] = j;
		stack[SP++] = k;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJLEB_eq() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJLEB << 8 | (CARD8)alpha;
		INT16 k = 0x1111;
		INT16 j = 0x1111;
		stack[SP++] = j;
		stack[SP++] = k;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}


	void testJULB_jp() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJULB << 8 | (CARD8)alpha;
		CARDINAL u = 0x1111;
		CARDINAL v = 0x2222;
		stack[SP++] = u;
		stack[SP++] = v;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJULB_jn() {
		INT8 alpha = -16;
		page_CB[PC / 2] = zJULB << 8 | (CARD8)alpha;
		CARDINAL u = 0x1111;
		CARDINAL v = 0x2222;
		stack[SP++] = u;
		stack[SP++] = v;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJULB_nj() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJULB << 8 | (CARD8)alpha;
		CARDINAL u = 0x2222;
		CARDINAL v = 0x1111;
		stack[SP++] = u;
		stack[SP++] = v;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJULB_eq() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJULB << 8 | (CARD8)alpha;
		CARDINAL u = 0x1111;
		CARDINAL v = 0x1111;
		stack[SP++] = u;
		stack[SP++] = v;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}


	void testJUGEB_jp() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJUGEB << 8 | (CARD8)alpha;
		CARDINAL u = 0x2222;
		CARDINAL v = 0x1111;
		stack[SP++] = u;
		stack[SP++] = v;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJUGEB_jn() {
		INT8 alpha = -16;
		page_CB[PC / 2] = zJUGEB << 8 | (CARD8)alpha;
		CARDINAL u = 0x2222;
		CARDINAL v = 0x1111;
		stack[SP++] = u;
		stack[SP++] = v;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJUGEB_nj() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJUGEB << 8 | (CARD8)alpha;
		CARDINAL u = 0x1111;
		CARDINAL v = 0x2222;
		stack[SP++] = u;
		stack[SP++] = v;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJUGEB_eq() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJUGEB << 8 | (CARD8)alpha;
		CARDINAL u = 0x1111;
		CARDINAL v = 0x1111;
		stack[SP++] = u;
		stack[SP++] = v;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}


	void testJUGB_jp() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJUGB << 8 | (CARD8)alpha;
		CARDINAL u = 0x2222;
		CARDINAL v = 0x1111;
		stack[SP++] = u;
		stack[SP++] = v;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJUGB_jn() {
		INT8 alpha = -16;
		page_CB[PC / 2] = zJUGB << 8 | (CARD8)alpha;
		CARDINAL u = 0x2222;
		CARDINAL v = 0x1111;
		stack[SP++] = u;
		stack[SP++] = v;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJUGB_nj() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJUGB << 8 | (CARD8)alpha;
		CARDINAL u = 0x1111;
		CARDINAL v = 0x2222;
		stack[SP++] = u;
		stack[SP++] = v;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJUGB_eq() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJUGB << 8 | (CARD8)alpha;
		CARDINAL u = 0x1111;
		CARDINAL v = 0x1111;
		stack[SP++] = u;
		stack[SP++] = v;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}


	void testJULEB_jp() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJULEB << 8 | (CARD8)alpha;
		CARDINAL u = 0x1111;
		CARDINAL v = 0x2222;
		stack[SP++] = u;
		stack[SP++] = v;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJULEB_jn() {
		INT8 alpha = -16;
		page_CB[PC / 2] = zJULEB << 8 | (CARD8)alpha;
		CARDINAL u = 0x1111;
		CARDINAL v = 0x2222;
		stack[SP++] = u;
		stack[SP++] = v;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJULEB_nj() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJULEB << 8 | (CARD8)alpha;
		CARDINAL u = 0x2222;
		CARDINAL v = 0x1111;
		stack[SP++] = u;
		stack[SP++] = v;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJULEB_eq() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJULEB << 8 | (CARD8)alpha;
		CARDINAL u = 0x1111;
		CARDINAL v = 0x1111;
		stack[SP++] = u;
		stack[SP++] = v;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}


	void testJZ3_j() {
		page_CB[PC / 2] = zJZ3 << 8 | 0x00;
		CARD16 data = 0;
		stack[SP++] = data;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 3, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJZ3_nj() {
		page_CB[PC / 2] = zJZ3 << 8 | 0x00;
		CARD16 data = 4;
		stack[SP++] = data;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}


	void testJZ4_j() {
		page_CB[PC / 2] = zJZ4 << 8 | 0x00;
		CARD16 data = 0;
		stack[SP++] = data;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 4, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJZ4_nj() {
		page_CB[PC / 2] = zJZ4 << 8 | 0x00;
		CARD16 data = 4;
		stack[SP++] = data;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}


	void testJZB_jp() {
		INT8 disp = 16;
		page_CB[PC / 2] = zJZB << 8 | (CARD8)disp;
		CARD16 data = 0;
		stack[SP++] = data;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + disp, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJZB_jn() {
		INT8 disp = -16;
		page_CB[PC / 2] = zJZB << 8 | (CARD8)disp;
		CARD16 data = 0;
		stack[SP++] = data;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + disp, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJZB_nj() {
		INT8 disp = 16;
		page_CB[PC / 2] = zJZB << 8 | (CARD8)disp;
		CARD16 data = 4;
		stack[SP++] = data;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}


	void testJNZ3_j() {
		page_CB[PC / 2] = zJNZ3 << 8 | 0x00;
		CARD16 data = 4;
		stack[SP++] = data;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 3, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJNZ3_nj() {
		page_CB[PC / 2] = zJNZ3 << 8 | 0x00;
		CARD16 data = 0;
		stack[SP++] = data;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}


	void testJNZ4_j() {
		page_CB[PC / 2] = zJNZ4 << 8 | 0x00;
		CARD16 data = 4;
		stack[SP++] = data;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 4, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJNZ4_nj() {
		page_CB[PC / 2] = zJNZ4 << 8 | 0x00;
		CARD16 data = 0;
		stack[SP++] = data;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}


	void testJNZB_jp() {
		INT8 disp = 16;
		page_CB[PC / 2] = zJNZB << 8 | (CARD8)disp;
		CARD16 data = 4;
		stack[SP++] = data;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + disp, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJNZB_jn() {
		INT8 disp = -16;
		page_CB[PC / 2] = zJNZB << 8 | (CARD8)disp;
		CARD16 data = 4;
		stack[SP++] = data;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + disp, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJNZB_nj() {
		INT8 disp = 16;
		page_CB[PC / 2] = zJNZB << 8 | (CARD8)disp;
		CARD16 data = 0;
		stack[SP++] = data;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}


	void testJDEB_jp() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJDEB << 8 | (CARD8)alpha;
		CARD32 u = 0x11111111;
		CARD32 v = 0x11111111;
		stack[SP++] = LowHalf(u);
		stack[SP++] = HighHalf(u);
		stack[SP++] = LowHalf(v);
		stack[SP++] = HighHalf(v);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJDEB_jn() {
		INT8 alpha = -16;
		page_CB[PC / 2] = zJDEB << 8 | (CARD8)alpha;
		CARD32 u = 0x11111111;
		CARD32 v = 0x11111111;
		stack[SP++] = LowHalf(u);
		stack[SP++] = HighHalf(u);
		stack[SP++] = LowHalf(v);
		stack[SP++] = HighHalf(v);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJDEB_nj() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJDEB << 8 | (CARD8)alpha;
		CARD32 u = 0x11111111;
		CARD32 v = 0x22222222;
		stack[SP++] = LowHalf(u);
		stack[SP++] = HighHalf(u);
		stack[SP++] = LowHalf(v);
		stack[SP++] = HighHalf(v);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}


	void testJDNEB_jp() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJDNEB << 8 | (CARD8)alpha;
		CARD32 u = 0x11111111;
		CARD32 v = 0x22222222;
		stack[SP++] = LowHalf(u);
		stack[SP++] = HighHalf(u);
		stack[SP++] = LowHalf(v);
		stack[SP++] = HighHalf(v);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJDNEB_jn() {
		INT8 alpha = -16;
		page_CB[PC / 2] = zJDNEB << 8 | (CARD8)alpha;
		CARD32 u = 0x11111111;
		CARD32 v = 0x22222222;
		stack[SP++] = LowHalf(u);
		stack[SP++] = HighHalf(u);
		stack[SP++] = LowHalf(v);
		stack[SP++] = HighHalf(v);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + alpha, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJDNEB_nj() {
		INT8 alpha = 16;
		page_CB[PC / 2] = zJDNEB << 8 | (CARD8)alpha;
		CARD32 u = 0x11111111;
		CARD32 v = 0x11111111;
		stack[SP++] = LowHalf(u);
		stack[SP++] = HighHalf(u);
		stack[SP++] = LowHalf(v);
		stack[SP++] = HighHalf(v);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}


	void testJIB_j() {
		BytePair base = {0x80};
		page_CB[(PC / 2) + 0] = zJIB << 8 | (CARD8)base.left;
		page_CB[(PC / 2) + 1] = (CARD8)(base.right) << 8 | 0x20;
		page_CB[(base.u + 0)] = 0x00 << 8 | 0x10;
		page_CB[(base.u + 1)] = 0x20 << 8 | 0x30;
		CARDINAL limit = 4;
		CARDINAL index = 3;
		stack[SP++] = index;
		stack[SP++] = limit;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 0x30, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJIB_nj() {
		BytePair base = {0x80};
		page_CB[(PC / 2) + 0] = zJIB << 8 | (CARD8)base.left;
		page_CB[(PC / 2) + 1] = (CARD8)(base.right) << 8 | 0x20;
		page_CB[(base.u + 0)] = 0x00 << 8 | 0x10;
		page_CB[(base.u + 1)] = 0x20 << 8 | 0x30;
		CARDINAL limit = 4;
		CARDINAL index = 10;
		stack[SP++] = index;
		stack[SP++] = limit;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 3, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}


	void testJIW_j() {
		BytePair base = {0x80};
		page_CB[(PC / 2) + 0] = zJIW << 8 | (CARD8)base.left;
		page_CB[(PC / 2) + 1] = (CARD8)(base.right) << 8 | 0x20;
		page_CB[(base.u + 0)] = 0x00;
		page_CB[(base.u + 1)] = 0x10;
		page_CB[(base.u + 2)] = 0x20;
		page_CB[(base.u + 3)] = 0x30;
		CARDINAL limit = 4;
		CARDINAL index = 3;
		stack[SP++] = index;
		stack[SP++] = limit;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 0x30, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testJIW_nj() {
		BytePair base = {0x80};
		page_CB[(PC / 2) + 0] = zJIW << 8 | (CARD8)base.left;
		page_CB[(PC / 2) + 1] = (CARD8)(base.right) << 8 | 0x20;
		page_CB[(base.u + 0)] = 0x00;
		page_CB[(base.u + 1)] = 0x10;
		page_CB[(base.u + 2)] = 0x20;
		page_CB[(base.u + 3)] = 0x30;
		CARDINAL limit = 4;
		CARDINAL index = 10;
		stack[SP++] = index;
		stack[SP++] = limit;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 3, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}

	void testREC() {
		page_CB[(PC / 2) + 0] = zREC << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
	}
	void testREC2() {
		page_CB[(PC / 2) + 0] = zREC2 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
	}
	void testDIS() {
		page_CB[(PC / 2) + 0] = zDIS << 8 | 0x00;
		SP++;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}
	void testDIS2() {
		page_CB[(PC / 2) + 0] = zDIS2 << 8 | 0x00;
		SP++;
		SP++;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}


	void testEXCH() {
		page_CB[(PC / 2) + 0] = zEXCH << 8 | 0x00;
		CARD16 u = 0x1111;
		CARD16 v = 0x2222;
		stack[SP++] = u;
		stack[SP++] = v;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(v, stack[0]);
		CPPUNIT_ASSERT_EQUAL(u, stack[1]);
	}

	void testDEXCH() {
		page_CB[(PC / 2) + 0] = zDEXCH << 8 | 0x00;
		CARD32 u = 0x12345678;
		CARD32 v = 0x90abcdef;
		stack[SP++] = LowHalf(u);
		stack[SP++] = HighHalf(u);
		stack[SP++] = LowHalf(v);
		stack[SP++] = HighHalf(v);

		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(4, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(v),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(v), stack[1]);
		CPPUNIT_ASSERT_EQUAL(LowHalf(u),  stack[2]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(u), stack[3]);
	}

	void testDUP() {
		page_CB[(PC / 2) + 0] = zDUP << 8 | 0x00;
		CARD16 u = 0x1111;
		stack[SP++] = u;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(u, stack[0]);
		CPPUNIT_ASSERT_EQUAL(u, stack[1]);
	}
	void testDDUP() {
		page_CB[(PC / 2) + 0] = zDDUP << 8 | 0x00;
		CARD32 u = 0x12345678;
		stack[SP++] = LowHalf(u);
		stack[SP++] = HighHalf(u);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(4, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(u),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(u), stack[1]);
		CPPUNIT_ASSERT_EQUAL(LowHalf(u),  stack[2]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(u), stack[3]);
	}
	void testEXDIS() {
		page_CB[(PC / 2) + 0] = zEXDIS << 8 | 0x00;
		CARD16 u = 0x1111;
		CARD16 v = 0x2222;
		stack[SP++] = u;
		stack[SP++] = v;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(v, stack[0]);
	}


	void testNEG() {
		page_CB[(PC / 2) + 0] = zNEG << 8 | 0x00;
		INT16 u = 1000;
		stack[SP++] = u;
		Interpreter::execute();

		u = -u;
		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)u, stack[0]);
	}
	void testINC() {
		page_CB[(PC / 2) + 0] = zINC << 8 | 0x00;
		CARDINAL u = 1000;
		stack[SP++] = u;
		Interpreter::execute();

		u = u + 1;
		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(u, stack[0]);
	}
	void testDEC() {
		page_CB[(PC / 2) + 0] = zDEC << 8 | 0x00;
		CARDINAL u = 1000;
		stack[SP++] = u;
		Interpreter::execute();

		u = u - 1;
		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(u, stack[0]);
	}
	void testDINC() {
		page_CB[(PC / 2) + 0] = zDINC << 8 | 0x00;
		CARD32 u = 0x12345678;
		stack[SP++] = LowHalf(u);
		stack[SP++] = HighHalf(u);
		Interpreter::execute();

		u = u + 1;
		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(u),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(u), stack[1]);
	}
	void testDBL() {
		page_CB[(PC / 2) + 0] = zDBL << 8 | 0x00;
		CARDINAL u = 1000;
		stack[SP++] = u;
		Interpreter::execute();

		u = u + u;
		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(u, stack[0]);
	}
	void testDDBL() {
		page_CB[(PC / 2) + 0] = zDDBL << 8 | 0x00;
		CARD32 u = 0x12345678;
		stack[SP++] = LowHalf(u);
		stack[SP++] = HighHalf(u);
		Interpreter::execute();

		u = u + u;
		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(u),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(u), stack[1]);
	}
	void testTRPL() {
		page_CB[(PC / 2) + 0] = zTRPL << 8 | 0x00;
		CARDINAL u = 1000;
		stack[SP++] = u;
		Interpreter::execute();

		u = u + u + u;
		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(u, stack[0]);
	}


	void testAND() {
		page_CB[(PC / 2) + 0] = zAND << 8 | 0x00;
		CARD16 u = 0x1111;
		CARD16 v = 0x2222;
		stack[SP++] = u;
		stack[SP++] = v;
		Interpreter::execute();

		u = u & v;
		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(u, stack[0]);
	}
	void testIOR() {
		page_CB[(PC / 2) + 0] = zIOR << 8 | 0x00;
		CARD16 u = 0x1111;
		CARD16 v = 0x2222;
		stack[SP++] = u;
		stack[SP++] = v;
		Interpreter::execute();

		u = u | v;
		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(u, stack[0]);
	}
	void testADDSB_p() {
		INT8 alpha = 10;
		page_CB[(PC / 2) + 0] = zADDSB << 8 | (CARD8)alpha;
		INT16 u = 1000;
		stack[SP++] = u;
		Interpreter::execute();

		u = u + alpha;
		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)u, stack[0]);
	}
	void testADDSB_n() {
		INT8 alpha = -10;
		page_CB[(PC / 2) + 0] = zADDSB << 8 | (CARD8)alpha;
		INT16 u = 1000;
		stack[SP++] = u;
		Interpreter::execute();

		u = u + alpha;
		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)u, stack[0]);
	}
	void testADD() {
		page_CB[(PC / 2) + 0] = zADD << 8 | 0x00;
		CARD16 s = 0x1111;
		CARD16 t = 0x2222;
		stack[SP++] = s;
		stack[SP++] = t;
		Interpreter::execute();

		s = s + t;
		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(s, stack[0]);
	}
	void testSUB() {
		page_CB[(PC / 2) + 0] = zSUB << 8 | 0x00;
		CARD16 s = 0x1111;
		CARD16 t = 0x2222;
		stack[SP++] = s;
		stack[SP++] = t;
		Interpreter::execute();

		s = s - t;
		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(s, stack[0]);
	}
	void testDADD() {
		page_CB[(PC / 2) + 0] = zDADD << 8 | 0x00;
		CARD32 t = 0x12345678;
		CARD32 s = 0x90abcdef;
		stack[SP++] = LowHalf(s);
		stack[SP++] = HighHalf(s);
		stack[SP++] = LowHalf(t);
		stack[SP++] = HighHalf(t);
		Interpreter::execute();

		s = s + t;
		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(s),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(s), stack[1]);
	}
	void testDSUB() {
		page_CB[(PC / 2) + 0] = zDSUB << 8 | 0x00;
		CARD32 t = 0x12345678;
		CARD32 s = 0x90abcdef;
		stack[SP++] = LowHalf(s);
		stack[SP++] = HighHalf(s);
		stack[SP++] = LowHalf(t);
		stack[SP++] = HighHalf(t);
		Interpreter::execute();

		s = s - t;
		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(s),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(s), stack[1]);
	}
	void testADC() {
		page_CB[(PC / 2) + 0] = zADC << 8 | 0x00;
		CARD32 s = 0x90abcdef;
		CARD16 t = 0x5678;
		stack[SP++] = LowHalf(s);
		stack[SP++] = HighHalf(s);
		stack[SP++] = t;
		Interpreter::execute();

		s = s + t;
		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(s),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(s), stack[1]);
	}
	void testACD() {
		page_CB[(PC / 2) + 0] = zACD << 8 | 0x00;
		CARD16 s = 0x5678;
		CARD32 t = 0x90abcdef;
		stack[SP++] = s;
		stack[SP++] = LowHalf(t);
		stack[SP++] = HighHalf(t);
		Interpreter::execute();

		t = s + t;
		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(t),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(t), stack[1]);
	}
	void testAL0IB() {
		CARD8 alpha = 0x10;
		page_CB[(PC / 2) + 0] = zAL0IB << 8 | alpha;
		CARD16 value = 0x1234;
		page_LF[0] = value;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)(value + alpha),  stack[0]);
	}
	void testMUL() {
		page_CB[(PC / 2) + 0] = zMUL << 8 | 0x00;
		CARD16 s = 0x1111;
		CARD16 t = 0x2222;
		stack[SP++] = s;
		stack[SP++] = t;
		Interpreter::execute();

		CARD32 a = (CARD32)s * (CARD32)t;
		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(a),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(a), stack[1]);
	}
	void testDCMP_gt() {
		page_CB[(PC / 2) + 0] = zDCMP << 8 | 0x00;
		INT32 j = 0x22222222;
		INT32 k = 0x99999999;
		stack[SP++] = LowHalf(j);
		stack[SP++] = HighHalf(j);
		stack[SP++] = LowHalf(k);
		stack[SP++] = HighHalf(k);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((INT16)1,  (INT16)stack[0]);
	}
	void testDCMP_lt() {
		page_CB[(PC / 2) + 0] = zDCMP << 8 | 0x00;
		INT32 j = 0x99999999;
		INT32 k = 0x22222222;
		stack[SP++] = LowHalf(j);
		stack[SP++] = HighHalf(j);
		stack[SP++] = LowHalf(k);
		stack[SP++] = HighHalf(k);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((INT16)-1,  (INT16)stack[0]);
	}
	void testDCMP_eq() {
		page_CB[(PC / 2) + 0] = zDCMP << 8 | 0x00;
		INT32 j = 0x11111111;
		INT32 k = 0x11111111;
		stack[SP++] = LowHalf(j);
		stack[SP++] = HighHalf(j);
		stack[SP++] = LowHalf(k);
		stack[SP++] = HighHalf(k);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((INT16)0,  (INT16)stack[0]);
	}
	void testUDCMP_gt() {
		page_CB[(PC / 2) + 0] = zUDCMP << 8 | 0x00;
		CARD32 s = 0x22222222;
		CARD32 t = 0x11111111;
		stack[SP++] = LowHalf(s);
		stack[SP++] = HighHalf(s);
		stack[SP++] = LowHalf(t);
		stack[SP++] = HighHalf(t);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((INT16)1,  (INT16)stack[0]);
	}
	void testUDCMP_lt() {
		page_CB[(PC / 2) + 0] = zUDCMP << 8 | 0x00;
		CARD32 s = 0x11111111;
		CARD32 t = 0x22222222;
		stack[SP++] = LowHalf(s);
		stack[SP++] = HighHalf(s);
		stack[SP++] = LowHalf(t);
		stack[SP++] = HighHalf(t);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((INT16)-1,  (INT16)stack[0]);
	}
	void testUDCMP_eq() {
		page_CB[(PC / 2) + 0] = zUDCMP << 8 | 0x00;
		CARD32 s = 0x11111111;
		CARD32 t = 0x11111111;
		stack[SP++] = LowHalf(s);
		stack[SP++] = HighHalf(s);
		stack[SP++] = LowHalf(t);
		stack[SP++] = HighHalf(t);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((INT16)0,  (INT16)stack[0]);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(testOpcode_200);
