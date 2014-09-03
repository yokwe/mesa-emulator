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
// testOpcode_300.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("test300");

#include "testBase.h"

class testOpcode_300 : public testBase {
	CPPUNIT_TEST_SUITE(testOpcode_300);

	CPPUNIT_TEST(testLI0); // 0300
	CPPUNIT_TEST(testLI1); // 0301
	CPPUNIT_TEST(testLI2); // 0302
	CPPUNIT_TEST(testLI3); // 0303
	CPPUNIT_TEST(testLI4); // 0304
	CPPUNIT_TEST(testLI5); // 0305
	CPPUNIT_TEST(testLI6); // 0306
	CPPUNIT_TEST(testLI7); // 0307

	CPPUNIT_TEST(testLI8);  // 0310
	CPPUNIT_TEST(testLI9);  // 0311
	CPPUNIT_TEST(testLI10); // 0312
	CPPUNIT_TEST(testLIN1); // 0313
	CPPUNIT_TEST(testLINI); // 0314
	CPPUNIT_TEST(testLIB);  // 0315
	CPPUNIT_TEST(testLIW);  // 0316
	CPPUNIT_TEST(testLINB); // 0317

	CPPUNIT_TEST(testLIHB); // 0320
	CPPUNIT_TEST(testLID0); // 0321
	CPPUNIT_TEST(testLA0);  // 0322
	CPPUNIT_TEST(testLA1);  // 0323
	CPPUNIT_TEST(testLA2);  // 0324
	CPPUNIT_TEST(testLA3);  // 0325
	CPPUNIT_TEST(testLA6);  // 0326
	CPPUNIT_TEST(testLA8);  // 0327

	CPPUNIT_TEST(testLAB);   // 0330
	CPPUNIT_TEST(testLAW);   // 0331
//	CPPUNIT_TEST(testGA0);   // 0332
//	CPPUNIT_TEST(testGA1);   // 0333
//	CPPUNIT_TEST(testGAB);   // 0334
//	CPPUNIT_TEST(testGAW);   // 0335
//  CPPUNIT_TEST(testCAW);   // 0336
    CPPUNIT_TEST(testEFC0);  // 0337

    CPPUNIT_TEST(testEFC1);  // 0340
    CPPUNIT_TEST(testEFC2);  // 0341
    CPPUNIT_TEST(testEFC3);  // 0342
    CPPUNIT_TEST(testEFC4);  // 0343
    CPPUNIT_TEST(testEFC5);  // 0344
    CPPUNIT_TEST(testEFC6);  // 0345
    CPPUNIT_TEST(testEFC7);  // 0346
    CPPUNIT_TEST(testEFC8);  // 0347

    CPPUNIT_TEST(testEFC9);  // 0350
    CPPUNIT_TEST(testEFC10); // 0351
    CPPUNIT_TEST(testEFC11); // 0352
    CPPUNIT_TEST(testEFC12); // 0353
    CPPUNIT_TEST(testEFCB);  // 0354
    CPPUNIT_TEST(testLFC);   // 0355
    CPPUNIT_TEST(testSFC);   // 0356
    CPPUNIT_TEST(testRET);   // 0357

    CPPUNIT_TEST(testKFCB);  // 0360
    CPPUNIT_TEST(testME);    // 0361
    CPPUNIT_TEST(testMX);    // 0362
    CPPUNIT_TEST(testBLT_n); // 0363
    CPPUNIT_TEST(testBLT_o); // 0363
    CPPUNIT_TEST(testBLT_l); // 0363
    CPPUNIT_TEST(testBLTL_n);  // 0364
    CPPUNIT_TEST(testBLTL_o);  // 0364
    CPPUNIT_TEST(testBLTL_l);  // 0364
    CPPUNIT_TEST(testBLTC);    // 0365
    CPPUNIT_TEST(testBLTC_l);  // 0365
    CPPUNIT_TEST(testBLTCL);   // 0366
    CPPUNIT_TEST(testBLTCL_l); // 0366
	CPPUNIT_TEST(testLP_z);  // 0367
	CPPUNIT_TEST(testLP_nz); // 0367

//	CPPUNIT_TEST(testESC);    // 0370
//	CPPUNIT_TEST(testESCL);   // 0371
	CPPUNIT_TEST(testLGA0);   // 0372
	CPPUNIT_TEST(testLGAB);   // 0373
	CPPUNIT_TEST(testLGAW);   // 0374
	CPPUNIT_TEST(testDESC);   // 0375

//	CPPUNIT_TEST(testRESRVD); // 0377

	CPPUNIT_TEST_SUITE_END();

	///////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////

	void testLI0() {
		page_CB[(PC / 2) + 0] = zLI0 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)0, stack[0]);
	}
	void testLI1() {
		page_CB[(PC / 2) + 0] = zLI1 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)1, stack[0]);
	}
	void testLI2() {
		page_CB[(PC / 2) + 0] = zLI2 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)2, stack[0]);
	}
	void testLI3() {
		page_CB[(PC / 2) + 0] = zLI3 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)3, stack[0]);
	}
	void testLI4() {
		page_CB[(PC / 2) + 0] = zLI4 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)4, stack[0]);
	}
	void testLI5() {
		page_CB[(PC / 2) + 0] = zLI5 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)5, stack[0]);
	}
	void testLI6() {
		page_CB[(PC / 2) + 0] = zLI6 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)6, stack[0]);
	}
	void testLI7() {
		page_CB[(PC / 2) + 0] = zLI7 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)7, stack[0]);
	}
	void testLI8() {
		page_CB[(PC / 2) + 0] = zLI8 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)8, stack[0]);
	}
	void testLI9() {
		page_CB[(PC / 2) + 0] = zLI9 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)9, stack[0]);
	}
	void testLI10() {
		page_CB[(PC / 2) + 0] = zLI10 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)10, stack[0]);
	}
	void testLIN1() {
		page_CB[(PC / 2) + 0] = zLIN1 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)0xFFFF, stack[0]);
	}
	void testLINI() {
		page_CB[(PC / 2) + 0] = zLINI << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)0x8000, stack[0]);
	}
	void testLIB() {
		CARD8 alpha = 0x45;
		page_CB[(PC / 2) + 0] = zLIB << 8 | alpha;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)alpha, stack[0]);
	}
	void testLIW() {
		BytePair alpha = {0x1234};
		page_CB[(PC / 2) + 0] = zLIW << 8 | alpha.left;
		page_CB[(PC / 2) + 1] = alpha.right << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 3, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)alpha.u, stack[0]);
	}
	void testLINB() {
		CARD8 alpha = 0x45;
		page_CB[(PC / 2) + 0] = zLINB << 8 | alpha;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)(0xff00 | alpha), stack[0]);
	}
	void testLIHB() {
		CARD8 alpha = 0x45;
		page_CB[(PC / 2) + 0] = zLIHB << 8 | alpha;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)(alpha << 8), stack[0]);
	}
	void testLID0() {
		page_CB[(PC / 2) + 0] = zLID0 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)0, stack[0]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0, stack[1]);
	}


	void testLA0() {
		page_CB[(PC / 2) + 0] = zLA0 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)(LFCache::LF() + 0), stack[0]);
	}
	void testLA1() {
		page_CB[(PC / 2) + 0] = zLA1 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)(LFCache::LF() + 1), stack[0]);
	}
	void testLA2() {
		page_CB[(PC / 2) + 0] = zLA2 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)(LFCache::LF() + 2), stack[0]);
	}
	void testLA3() {
		page_CB[(PC / 2) + 0] = zLA3 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)(LFCache::LF() + 3), stack[0]);
	}
	void testLA6() {
		page_CB[(PC / 2) + 0] = zLA6 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)(LFCache::LF() + 6), stack[0]);
	}
	void testLA8() {
		page_CB[(PC / 2) + 0] = zLA8 << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)(LFCache::LF() + 8), stack[0]);
	}
	void testLAB() {
		CARD8 alpha = 0xa0;
		page_CB[(PC / 2) + 0] = zLAB << 8 | alpha;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)(LFCache::LF() + alpha), stack[0]);
	}
	void testLAW() {
		BytePair alpha = {0x7000};
		page_CB[(PC / 2) + 0] = zLAW << 8 | (CARD8)alpha.left;
		page_CB[(PC / 2) + 1] = (CARD8)(alpha.right) << 8 | 0x00;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 3, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)(LFCache::LF() + alpha.u), stack[0]);
	}

	//	void testGA0() {}
	//	void testGA1() {}
	//	void testGAB() {}
	//	void testGAW() {}

    void testEFC0() {
    	CARD8 alpha = 0x00;
		page_CB[PC / 2] = zEFC0 << 8 | 0x00;
		NewProcDesc dst = {0};
		dst.pc = 0x22;
		dst.taggedGFI = GFI_EFC | 0x0003;
		page_GF[-1] = 0x0000; // codelink == 0 => frame link
		page_GF[-2 -(alpha + 1) * 2 + 0] = LowHalf(dst.u);
		page_GF[-2 -(alpha + 1) * 2 + 1] = HighHalf(dst.u);
		page_CB[dst.pc / 2] = 0; // set fsi = 0
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL((CARD16)(savedPC + 1), page_LF[-1]);
		CPPUNIT_ASSERT_EQUAL(GFI_EFC, GFI);
		CPPUNIT_ASSERT_EQUAL((CARD16)(dst.pc + 1), PC);
		CPPUNIT_ASSERT_EQUAL((CARD16)0, SP);
    }
    void testEFC1() {
    	CARD8 alpha = 1;
		page_CB[PC / 2] = zEFC1 << 8 | 0x00;
		NewProcDesc dst = {0};
		dst.pc = 0x22;
		dst.taggedGFI = GFI_EFC | 0x0003;
		page_GF[-1] = 0x0000; // codelink == 0 => frame link
		page_GF[-2 -(alpha + 1) * 2 + 0] = LowHalf(dst.u);
		page_GF[-2 -(alpha + 1) * 2 + 1] = HighHalf(dst.u);
		page_CB[dst.pc / 2] = 0; // set fsi = 0
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL((CARD16)(savedPC + 1), page_LF[-1]);
		CPPUNIT_ASSERT_EQUAL(GFI_EFC, GFI);
		CPPUNIT_ASSERT_EQUAL((CARD16)(dst.pc + 1), PC);
		CPPUNIT_ASSERT_EQUAL((CARD16)0, SP);
    }
    void testEFC2() {
    	CARD8 alpha = 2;
		page_CB[PC / 2] = zEFC2 << 8 | 0x00;
		NewProcDesc dst = {0};
		dst.pc = 0x22;
		dst.taggedGFI = GFI_EFC | 0x0003;
		page_GF[-1] = 0x0000; // codelink == 0 => frame link
		page_GF[-2 -(alpha + 1) * 2 + 0] = LowHalf(dst.u);
		page_GF[-2 -(alpha + 1) * 2 + 1] = HighHalf(dst.u);
		page_CB[dst.pc / 2] = 0; // set fsi = 0
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL((CARD16)(savedPC + 1), page_LF[-1]);
		CPPUNIT_ASSERT_EQUAL(GFI_EFC, GFI);
		CPPUNIT_ASSERT_EQUAL((CARD16)(dst.pc + 1), PC);
		CPPUNIT_ASSERT_EQUAL((CARD16)0, SP);
    }
    void testEFC3() {
    	CARD8 alpha = 3;
		page_CB[PC / 2] = zEFC3 << 8 | 0x00;
		NewProcDesc dst = {0};
		dst.pc = 0x22;
		dst.taggedGFI = GFI_EFC | 0x0003;
		page_GF[-1] = 0x0000; // codelink == 0 => frame link
		page_GF[-2 -(alpha + 1) * 2 + 0] = LowHalf(dst.u);
		page_GF[-2 -(alpha + 1) * 2 + 1] = HighHalf(dst.u);
		page_CB[dst.pc / 2] = 0; // set fsi = 0
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL((CARD16)(savedPC + 1), page_LF[-1]);
		CPPUNIT_ASSERT_EQUAL(GFI_EFC, GFI);
		CPPUNIT_ASSERT_EQUAL((CARD16)(dst.pc + 1), PC);
		CPPUNIT_ASSERT_EQUAL((CARD16)0, SP);
    }
    void testEFC4() {
    	CARD8 alpha = 4;
		page_CB[PC / 2] = zEFC4 << 8 | 0x00;
		NewProcDesc dst = {0};
		dst.pc = 0x22;
		dst.taggedGFI = GFI_EFC | 0x0003;
		page_GF[-1] = 0x0000; // codelink == 0 => frame link
		page_GF[-2 -(alpha + 1) * 2 + 0] = LowHalf(dst.u);
		page_GF[-2 -(alpha + 1) * 2 + 1] = HighHalf(dst.u);
		page_CB[dst.pc / 2] = 0; // set fsi = 0
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL((CARD16)(savedPC + 1), page_LF[-1]);
		CPPUNIT_ASSERT_EQUAL(GFI_EFC, GFI);
		CPPUNIT_ASSERT_EQUAL((CARD16)(dst.pc + 1), PC);
		CPPUNIT_ASSERT_EQUAL((CARD16)0, SP);
    }
    void testEFC5() {
    	CARD8 alpha = 5;
		page_CB[PC / 2] = zEFC5 << 8 | 0x00;
		NewProcDesc dst = {0};
		dst.pc = 0x22;
		dst.taggedGFI = GFI_EFC | 0x0003;
		page_GF[-1] = 0x0000; // codelink == 0 => frame link
		page_GF[-2 -(alpha + 1) * 2 + 0] = LowHalf(dst.u);
		page_GF[-2 -(alpha + 1) * 2 + 1] = HighHalf(dst.u);
		page_CB[dst.pc / 2] = 0; // set fsi = 0
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL((CARD16)(savedPC + 1), page_LF[-1]);
		CPPUNIT_ASSERT_EQUAL(GFI_EFC, GFI);
		CPPUNIT_ASSERT_EQUAL((CARD16)(dst.pc + 1), PC);
		CPPUNIT_ASSERT_EQUAL((CARD16)0, SP);
   }
    void testEFC6() {
    	CARD8 alpha = 6;
		page_CB[PC / 2] = zEFC6 << 8 | 0x00;
		NewProcDesc dst = {0};
		dst.pc = 0x22;
		dst.taggedGFI = GFI_EFC | 0x0003;
		page_GF[-1] = 0x0000; // codelink == 0 => frame link
		page_GF[-2 -(alpha + 1) * 2 + 0] = LowHalf(dst.u);
		page_GF[-2 -(alpha + 1) * 2 + 1] = HighHalf(dst.u);
		page_CB[dst.pc / 2] = 0; // set fsi = 0
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(GFI_EFC, GFI);
		CPPUNIT_ASSERT_EQUAL((CARD16)(dst.pc + 1), PC);
		CPPUNIT_ASSERT_EQUAL((CARD16)0, SP);
    }
    void testEFC7() {
    	CARD8 alpha = 7;
		page_CB[PC / 2] = zEFC7 << 8 | 0x00;
		NewProcDesc dst = {0};
		dst.pc = 0x22;
		dst.taggedGFI = GFI_EFC | 0x0003;
		page_GF[-1] = 0x0000; // codelink == 0 => frame link
		page_GF[-2 -(alpha + 1) * 2 + 0] = LowHalf(dst.u);
		page_GF[-2 -(alpha + 1) * 2 + 1] = HighHalf(dst.u);
		page_CB[dst.pc / 2] = 0; // set fsi = 0
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL((CARD16)(savedPC + 1), page_LF[-1]);
		CPPUNIT_ASSERT_EQUAL(GFI_EFC, GFI);
		CPPUNIT_ASSERT_EQUAL((CARD16)(dst.pc + 1), PC);
		CPPUNIT_ASSERT_EQUAL(savedSP, SP);
   }
    void testEFC8() {
    	CARD8 alpha = 8;
		page_CB[PC / 2] = zEFC8 << 8 | 0x00;
		NewProcDesc dst = {0};
		dst.pc = 0x22;
		dst.taggedGFI = GFI_EFC | 0x0003;
		page_GF[-1] = 0x0000; // codelink == 0 => frame link
		page_GF[-2 -(alpha + 1) * 2 + 0] = LowHalf(dst.u);
		page_GF[-2 -(alpha + 1) * 2 + 1] = HighHalf(dst.u);
		page_CB[dst.pc / 2] = 0; // set fsi = 0
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL((CARD16)(savedPC + 1), page_LF[-1]);
		CPPUNIT_ASSERT_EQUAL(GFI_EFC, GFI);
		CPPUNIT_ASSERT_EQUAL((CARD16)(dst.pc + 1), PC);
		CPPUNIT_ASSERT_EQUAL(savedSP, SP);
    }

    void testEFC9() {
    	CARD8 alpha = 9;
		page_CB[PC / 2] = zEFC9 << 8 | 0x00;
		NewProcDesc dst = {0};
		dst.pc = 0x22;
		dst.taggedGFI = GFI_EFC | 0x0003;
		page_GF[-1] = 0x0000; // codelink == 0 => frame link
		page_GF[-2 -(alpha + 1) * 2 + 0] = LowHalf(dst.u);
		page_GF[-2 -(alpha + 1) * 2 + 1] = HighHalf(dst.u);
		page_CB[dst.pc / 2] = 0; // set fsi = 0
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL((CARD16)(savedPC + 1), page_LF[-1]);
		CPPUNIT_ASSERT_EQUAL(GFI_EFC, GFI);
		CPPUNIT_ASSERT_EQUAL((CARD16)(dst.pc + 1), PC);
		CPPUNIT_ASSERT_EQUAL(savedSP, SP);
    }
    void testEFC10() {
    	CARD8 alpha = 10;
		page_CB[PC / 2] = zEFC10 << 8 | 0x00;
		NewProcDesc dst = {0};
		dst.pc = 0x22;
		dst.taggedGFI = GFI_EFC | 0x0003;
		page_GF[-1] = 0x0000; // codelink == 0 => frame link
		page_GF[-2 -(alpha + 1) * 2 + 0] = LowHalf(dst.u);
		page_GF[-2 -(alpha + 1) * 2 + 1] = HighHalf(dst.u);
		page_CB[dst.pc / 2] = 0; // set fsi = 0
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL((CARD16)(savedPC + 1), page_LF[-1]);
		CPPUNIT_ASSERT_EQUAL(GFI_EFC, GFI);
		CPPUNIT_ASSERT_EQUAL((CARD16)(dst.pc + 1), PC);
		CPPUNIT_ASSERT_EQUAL(savedSP, SP);
   }
    void testEFC11() {
    	CARD8 alpha = 11;
		page_CB[PC / 2] = zEFC11 << 8 | 0x00;
		NewProcDesc dst = {0};
		dst.pc = 0x22;
		dst.taggedGFI = GFI_EFC | 0x0003;
		page_GF[-1] = 0x0000; // codelink == 0 => frame link
		page_GF[-2 -(alpha + 1) * 2 + 0] = LowHalf(dst.u);
		page_GF[-2 -(alpha + 1) * 2 + 1] = HighHalf(dst.u);
		page_CB[dst.pc / 2] = 0; // set fsi = 0
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL((CARD16)(savedPC + 1), page_LF[-1]);
		CPPUNIT_ASSERT_EQUAL(GFI_EFC, GFI);
		CPPUNIT_ASSERT_EQUAL((CARD16)(dst.pc + 1), PC);
		CPPUNIT_ASSERT_EQUAL(savedSP, SP);
    }
    void testEFC12() {
    	CARD8 alpha = 12;
		page_CB[PC / 2] = zEFC12 << 8 | 0x00;
		NewProcDesc dst = {0};
		dst.pc = 0x22;
		dst.taggedGFI = GFI_EFC | 0x0003;
		page_GF[-1] = 0x0000; // codelink == 0 => frame link
		page_GF[-2 -(alpha + 1) * 2 + 0] = LowHalf(dst.u);
		page_GF[-2 -(alpha + 1) * 2 + 1] = HighHalf(dst.u);
		page_CB[dst.pc / 2] = (CARD16)0; // set fsi = 0
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL((CARD16)(savedPC + 1), page_LF[-1]);
		CPPUNIT_ASSERT_EQUAL(GFI_EFC, GFI);
		CPPUNIT_ASSERT_EQUAL((CARD16)(dst.pc + 1), PC);
		CPPUNIT_ASSERT_EQUAL(savedSP, SP);
   }
    void testEFCB() {
    	CARD8 alpha = 16;
		page_CB[PC / 2] = zEFCB << 8 | alpha;
		NewProcDesc dst = {0};
		dst.pc = 0x22;
		dst.taggedGFI = GFI_EFC | 0x0003;
		page_GF[-1] = 0x0000; // codelink == 0 => frame link
		page_GF[-2 -(alpha + 1) * 2 + 0] = LowHalf(dst.u);
		page_GF[-2 -(alpha + 1) * 2 + 1] = HighHalf(dst.u);
		page_CB[dst.pc / 2] = 0; // set fsi = 0
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL((CARD16)(savedPC + 2), page_LF[-1]);
		CPPUNIT_ASSERT_EQUAL(GFI_EFC, GFI);
		CPPUNIT_ASSERT_EQUAL((CARD16)(dst.pc + 1), PC);
		CPPUNIT_ASSERT_EQUAL(savedSP, SP);
   }


    void testLFC() {
		BytePair nPC = {16};
		page_CB[PC / 2] = zLFC << 8 | (CARD8)nPC.left;
		page_CB[(PC / 2) + 1] = (CARD8)(nPC.right) << 8 | 0x00;
		page_CB[nPC.u / 2] = 0; // set fsi = 0
		CARD16 nLF = page_AV[0];
		CARD16 oLF = LFCache::LF();
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL((CARD16)(savedPC + 3), page_LF[-1]); // PC of caller
		CPPUNIT_ASSERT_EQUAL(nLF, LFCache::LF());                // new LF
		CPPUNIT_ASSERT_EQUAL((CARD16)(nPC.u + 1), PC);            // new PC
		CPPUNIT_ASSERT_EQUAL(GFI, page_MDS[nLF - 2]);             // stored GFI in LocalOverhead.globallink
		CPPUNIT_ASSERT_EQUAL(oLF, page_MDS[nLF - 3]);             // stored LF in LocalOverhead.returnllink
		CPPUNIT_ASSERT_EQUAL(savedSP, SP);
    }


    void testSFC() {
		page_CB[PC / 2] = zSFC << 8 | 0x00;
		NewProcDesc dst = {0};
		dst.pc = 0x22;
		dst.taggedGFI = GFI_EFC | 0x0003;
		stack[SP++] = LowHalf(dst.u);
		stack[SP++] = HighHalf(dst.u);
		page_CB[dst.pc / 2] = 0; // set fsi = 0
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL((CARD16)(savedPC + 1), page_LF[-1]);
		CPPUNIT_ASSERT_EQUAL(GFI_EFC, GFI);
		CPPUNIT_ASSERT_EQUAL((CARD16)(dst.pc + 1), PC);
		CPPUNIT_ASSERT_EQUAL((CARD16)0, SP);
    }


    void testRET() {
		page_CB[(PC / 2) + 0] = zRET << 8 | 0x00;
		FrameLink dst; // dst contains LF of destination (return)
		dst = page_AV[0];
		page_AV[0] = page_MDS[dst];
		CARD16 nPC = 0x40;
		page_MDS[dst - 2] = GFI_EFC; // set globallink of dst as GFI_EFC
		page_MDS[dst - 1] = nPC;     // set pc of dst os nPC
		page_LF[-4] = 0;             // set fsi of current LF
		page_LF[-3] = dst;           // set returnlink of current LF
		CARD16 oLF = LFCache::LF();
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(GFI_EFC, GFI);
		CPPUNIT_ASSERT_EQUAL(nPC, PC);
		CPPUNIT_ASSERT_EQUAL(page_AV[0], oLF);
		CPPUNIT_ASSERT_EQUAL((CARD16)0, SP);
	}


    void testKFCB() {
    	CARD8 alpha = sDivZeroTrap;
		page_CB[PC / 2] = zKFCB << 8 | alpha;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL((CARD16)(savedPC + 2), page_LF[-1]);
		CPPUNIT_ASSERT_EQUAL(pc_SD + sDivZeroTrap + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(GFI_SD, GFI);
		CPPUNIT_ASSERT_EQUAL((CARD16)0, SP);
   }


    void testME() {} // TODO ME
    void testMX() {} // TODO MX

    void testBLT_n() {
		page_CB[(PC / 2) + 0] = zBLT << 8 | 0x00;
		POINTER dest   = 0x0020;
		CARDINAL count = 0x0010;
		POINTER source = 0x0080;
		for(CARDINAL i = 0; i < count; i++) page_MDS[source + i] = (0xB000 | i);
		stack[SP++] = source;
		stack[SP++] = count;
		stack[SP++] = dest;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		for(CARDINAL i = 0; i < count; i++)
			CPPUNIT_ASSERT_EQUAL((CARD16)(0xB000 | i), page_MDS[dest + i]);
    }
    void testBLT_o() {
		page_CB[(PC / 2) + 0] = zBLT << 8 | 0x00;
		POINTER dest   = 0x0021;
		CARDINAL count = 0x0010;
		POINTER source = 0x0020;
		CARD16 value = 0xCAFE;
		page_MDS[source] = value;
		stack[SP++] = source;
		stack[SP++] = count;
		stack[SP++] = dest;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		for(CARDINAL i = 0; i < count; i++)
			CPPUNIT_ASSERT_EQUAL((CARD16)value, page_MDS[dest + i]);
    }
    void testBLT_l() {
		page_CB[(PC / 2) + 0] = zBLT << 8 | 0x00;
		POINTER dest   = 0x0630;
		CARDINAL count = 0x0340;
		POINTER source = 0x0140;
		for(CARDINAL i = 0; i < count; i++) page_MDS[source + i] = (0xB000 | i);
		stack[SP++] = source;
		stack[SP++] = count;
		stack[SP++] = dest;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		for(CARDINAL i = 0; i < count; i++)
			CPPUNIT_ASSERT_EQUAL((CARD16)(0xB000 | i), page_MDS[dest + i]);
    }


    void testBLTL_n() {
		page_CB[(PC / 2) + 0] = zBLTL << 8 | 0x00;
		LONG_POINTER dest   = MDSCache::MDS() + 0x0020;
		CARDINAL count = 0x0010;
		LONG_POINTER source = MDSCache::MDS() + 0x0080;
		for(CARDINAL i = 0; i < count; i++) page_MDS[source - MDSCache::MDS() + i] = (0x00 | i);
		stack[SP++] = LowHalf(source);
		stack[SP++] = HighHalf(source);
		stack[SP++] = count;
		stack[SP++] = LowHalf(dest);
		stack[SP++] = HighHalf(dest);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		for(CARDINAL i = 0; i < count; i++)
			CPPUNIT_ASSERT_EQUAL((CARD16)(0x00 | i), page_MDS[dest - MDSCache::MDS() + i]);
    }
    void testBLTL_o() {
		page_CB[(PC / 2) + 0] = zBLTL << 8 | 0x00;
		LONG_POINTER dest   = MDSCache::MDS() + 0x0021;
		CARDINAL count = 0x0010;
		LONG_POINTER source = MDSCache::MDS() + 0x0020;
		CARD16 value = 0xCAFE;
		page_MDS[(source + 0) % PageSize] = value;
		stack[SP++] = LowHalf(source);
		stack[SP++] = HighHalf(source);
		stack[SP++] = count;
		stack[SP++] = LowHalf(dest);
		stack[SP++] = HighHalf(dest);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		for(CARDINAL i = 0; i < count; i++)
			CPPUNIT_ASSERT_EQUAL((CARD16)value, page_MDS[dest - MDSCache::MDS() + i]);
    }
    void testBLTL_l() {
		page_CB[(PC / 2) + 0] = zBLTL << 8 | 0x00;
		LONG_POINTER dest   = MDSCache::MDS() + 0x0620;
		CARDINAL count = 0x0340;
		LONG_POINTER source = MDSCache::MDS() + 0x0180;
		for(CARDINAL i = 0; i < count; i++) page_MDS[source - MDSCache::MDS() + i] = (0xB000 | i);
		stack[SP++] = LowHalf(source);
		stack[SP++] = HighHalf(source);
		stack[SP++] = count;
		stack[SP++] = LowHalf(dest);
		stack[SP++] = HighHalf(dest);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		for(CARDINAL i = 0; i < count; i++)
			CPPUNIT_ASSERT_EQUAL((CARD16)(0xB000 | i), page_MDS[dest - MDSCache::MDS() + i]);
    }


    void testBLTC() {
		page_CB[(PC / 2) + 0] = zBLTC << 8 | 0x00;
		POINTER dest   = 0x0020;
		CARDINAL count = 0x0010;
		POINTER source = 0x0080;
		for(CARDINAL i = 0; i < count; i++) page_CB[source + i] = (0xB000 | i);
		stack[SP++] = source;
		stack[SP++] = count;
		stack[SP++] = dest;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		for(CARDINAL i = 0; i < count; i++)
			CPPUNIT_ASSERT_EQUAL((CARD16)(0xB000 | i), page_MDS[dest + i]);
    }
    void testBLTC_l() {
		page_CB[(PC / 2) + 0] = zBLTC << 8 | 0x00;
		POINTER dest   = 0x0020;
		CARDINAL count = 0x0340;
		POINTER source = 0x0080;
		for(CARDINAL i = 0; i < count; i++) page_CB[source + i] = (0xB000 | i);
		stack[SP++] = source;
		stack[SP++] = count;
		stack[SP++] = dest;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		for(CARDINAL i = 0; i < count; i++)
			CPPUNIT_ASSERT_EQUAL((CARD16)(0xB000 | i), page_MDS[dest + i]);
    }
    void testBLTCL() {
		page_CB[(PC / 2) + 0] = zBLTCL << 8 | 0x00;
		LONG_POINTER dest = MDSCache::MDS() + 0x0020;
		CARDINAL count = 0x0010;
		POINTER source = 0x0080;
		for(CARDINAL i = 0; i < count; i++) page_CB[source + i] = (0xB000 | i);
		stack[SP++] = source;
		stack[SP++] = count;
		stack[SP++] = LowHalf(dest);
		stack[SP++] = HighHalf(dest);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		for(CARDINAL i = 0; i < count; i++)
			CPPUNIT_ASSERT_EQUAL((CARD16)(0xB000 | i), page_MDS[dest - MDSCache::MDS() + i]);
    }
    void testBLTCL_l() {
		page_CB[(PC / 2) + 0] = zBLTCL << 8 | 0x00;
		LONG_POINTER dest = MDSCache::MDS() + 0x0020;
		CARDINAL count = 0x0340;
		POINTER source = 0x0080;
		for(CARDINAL i = 0; i < count; i++) page_CB[source + i] = (0xB000 | i);
		stack[SP++] = source;
		stack[SP++] = count;
		stack[SP++] = LowHalf(dest);
		stack[SP++] = HighHalf(dest);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		for(CARDINAL i = 0; i < count; i++)
			CPPUNIT_ASSERT_EQUAL((CARD16)(0xB000 | i), page_MDS[dest - MDSCache::MDS() + i]);
    }


	void testLP_z() {
		page_CB[(PC / 2) + 0] = zLP << 8 | 0x00;
		POINTER ptr = 0x0000;
		stack[SP++] = ptr;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)0, stack[0]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0, stack[1]);
	}
	void testLP_nz() {
		page_CB[(PC / 2) + 0] = zLP << 8 | 0x00;
		CARD16 ptr = 0x1234;
		stack[SP++] = ptr;
		Interpreter::execute();

		CARD32 p = MDSCache::MDS() + ptr;
		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(p),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(p), stack[1]);
	}


	//	void testESC() {}
    //	void testESCL() {}
	void testLGA0() {
		page_CB[(PC / 2) + 0] = zLGA0 << 8 | 0x00;
		Interpreter::execute();

		CARD32 p = GF + 0;
		CPPUNIT_ASSERT_EQUAL(savedPC + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(p),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(p), stack[1]);
	}
	void testLGAB() {
		CARD8 alpha = 0x40;
		page_CB[(PC / 2) + 0] = zLGAB << 8 | alpha;
		Interpreter::execute();

		CARD32 p = GF + alpha;
		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(p),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(p), stack[1]);
	}
	void testLGAW() {
		BytePair alpha = {0x1234};
		page_CB[(PC / 2) + 0] = zLGAW << 8 | alpha.left;
		page_CB[(PC / 2) + 1] = alpha.right << 8 | 0x00;
		Interpreter::execute();

		CARD32 p = GF + alpha.u;
		CPPUNIT_ASSERT_EQUAL(savedPC + 3, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(p),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(p), stack[1]);
	}
	void testDESC() {
		BytePair alpha = {0x1234};
		page_CB[(PC / 2) + 0] = zDESC << 8 | alpha.left;
		page_CB[(PC / 2) + 1] = alpha.right << 8 | 0x00;
		GFI = 0x4567;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 3, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)(GFI | 3),  stack[0]);
		CPPUNIT_ASSERT_EQUAL((CARD16)alpha.u, stack[1]);
	}

	// void testRESRVD() {}
};

CPPUNIT_TEST_SUITE_REGISTRATION(testOpcode_300);
