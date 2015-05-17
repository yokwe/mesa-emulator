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
// testOpcode_esc.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("testesc");

#include "testBase.h"

class testOpcode_esc : public testBase {
	CPPUNIT_TEST_SUITE(testOpcode_esc);

//	CPPUNIT_TEST(testA00);  // 0000
//	CPPUNIT_TEST(testA01);  // 0001
	CPPUNIT_TEST(testMW);   // 0002
	CPPUNIT_TEST(testMR);   // 0003
	CPPUNIT_TEST(testNC);   // 0004
	CPPUNIT_TEST(testBC);   // 0005
	CPPUNIT_TEST(testREQ);  // 0006
	CPPUNIT_TEST(testSM_n); // 0007
	CPPUNIT_TEST(testSM_v); // 0007

	CPPUNIT_TEST(testGMF_n); // 0010
	CPPUNIT_TEST(testGMF_v); // 0010
	CPPUNIT_TEST(testSMF_n); // 0011
	CPPUNIT_TEST(testSMF_v); // 0011
	CPPUNIT_TEST(testAF);    // 0012
	CPPUNIT_TEST(testFF);    // 0013
	CPPUNIT_TEST(testPI_nz); // 0014
	CPPUNIT_TEST(testPI_z);  // 0014
	CPPUNIT_TEST(testPO);    // 0015
	CPPUNIT_TEST(testPOR);   // 0016
	CPPUNIT_TEST(testSPP);   // 0017

	CPPUNIT_TEST(testDI);       // 0020
	CPPUNIT_TEST(testEI);       // 0021
	CPPUNIT_TEST(testXOR);      // 0022
	CPPUNIT_TEST(testDAND);     // 0023
	CPPUNIT_TEST(testDIOR);     // 0024
	CPPUNIT_TEST(testDXOR);     // 0025
	CPPUNIT_TEST(testROTATE_p); // 0026
	CPPUNIT_TEST(testROTATE_n); // 0026
	CPPUNIT_TEST(testDSHIFT);   // 0027

	CPPUNIT_TEST(testLINT_p);    // 0030
	CPPUNIT_TEST(testLINT_n);    // 0030
	CPPUNIT_TEST(testJS);        // 0031
	CPPUNIT_TEST(testRCFS);      // 0032
	CPPUNIT_TEST(testRC);        // 0033
	CPPUNIT_TEST(testUDIV_n);    // 0034
	CPPUNIT_TEST(testUDIV_t);    // 0034
	CPPUNIT_TEST(testLUDIV_n);   // 0035
	CPPUNIT_TEST(testLUDIV_dzt); // 0035
	CPPUNIT_TEST(testLUDIV_dct); // 0035
	CPPUNIT_TEST(testROB);       // 0036
	CPPUNIT_TEST(testWOB);       // 0037

	CPPUNIT_TEST(testDSK);      // 0040
	CPPUNIT_TEST(testXE);       // 0041
	CPPUNIT_TEST(testXF);       // 0042
	CPPUNIT_TEST(testLSK);      // 0043
	CPPUNIT_TEST(testBNDCKL_n); // 0044
	CPPUNIT_TEST(testBNDCKL_t); // 0044
	CPPUNIT_TEST(testNILCK_n);  // 0045
	CPPUNIT_TEST(testNILCK_t);  // 0045
	CPPUNIT_TEST(testNILCKL_n); // 0046
	CPPUNIT_TEST(testNILCKL_t); // 0046
	CPPUNIT_TEST(testBLTLR_n);  // 0047
	CPPUNIT_TEST(testBLTLR_o);  // 0047
	CPPUNIT_TEST(testBLTLR_l);  // 0047

	CPPUNIT_TEST(testBLEL_t);    // 0050
	CPPUNIT_TEST(testBLEL_f);    // 0050
	CPPUNIT_TEST(testBLEL_l);    // 0050
	CPPUNIT_TEST(testBLECL_t);   // 0051
	CPPUNIT_TEST(testBLECL_f);   // 0051
	CPPUNIT_TEST(testBLECL_l);   // 0051
	CPPUNIT_TEST(testCKSUM);     // 0052
	CPPUNIT_TEST(testBITBLT);    // 0053
	CPPUNIT_TEST(testTXTBLT);    // 0054
	CPPUNIT_TEST(testBYTBLT_n);  // 0055
	CPPUNIT_TEST(testBYTBLT_o);  // 0055
	CPPUNIT_TEST(testBYTBLT_l);  // 0055
	CPPUNIT_TEST(testBYTBLTR_n); // 0056
	CPPUNIT_TEST(testBYTBLTR_o); // 0056
	CPPUNIT_TEST(testBYTBLTR_l); // 0056
	CPPUNIT_TEST(testVERSION);   // 0057

	CPPUNIT_TEST(testDMUL);    // 0060
	CPPUNIT_TEST(testSDIV_n);  // 0061
	CPPUNIT_TEST(testSDIV_t);  // 0061
	CPPUNIT_TEST(testSDDIV_n); // 0062
	CPPUNIT_TEST(testSDDIV_t); // 0062
	CPPUNIT_TEST(testUDDIV_n); // 0063
	CPPUNIT_TEST(testUDDIV_t); // 0063
//	CPPUNIT_TEST(testA64);     // 0064
//	CPPUNIT_TEST(testA65);     // 0065
//	CPPUNIT_TEST(testA66);     // 0066
//	CPPUNIT_TEST(testA67);     // 0067

//	CPPUNIT_TEST(testA70);   // 0070
//	CPPUNIT_TEST(testA71);   // 0071
//	CPPUNIT_TEST(testA72);   // 0072
//	CPPUNIT_TEST(testA73);   // 0073
//	CPPUNIT_TEST(testA74);   // 0074
//	CPPUNIT_TEST(testA75);   // 0075
//	CPPUNIT_TEST(testA76);   // 0076
//	CPPUNIT_TEST(testA77);   // 0077

//  CPPUNIT_TEST(testFADD);  // 0100
//  CPPUNIT_TEST(testFSUB);  // 0101
//  CPPUNIT_TEST(testFMUL);  // 0102
//  CPPUNIT_TEST(testFDIV);  // 0103
//  CPPUNIT_TEST(testFCOMP); // 0104
//  CPPUNIT_TEST(testFIX);   // 0105
//  CPPUNIT_TEST(testFLOAT); // 0106
//  CPPUNIT_TEST(testFIXI);  // 0107

//  CPPUNIT_TEST(testFIXC);    // 0110
//  CPPUNIT_TEST(testFSTICKY); // 0111
//  CPPUNIT_TEST(testFREM);    // 0112
//  CPPUNIT_TEST(testROUND);   // 0113
//  CPPUNIT_TEST(testROUNDI);  // 0114
//  CPPUNIT_TEST(testROUNDC);  // 0115
//  CPPUNIT_TEST(testFSQRT);   // 0116
//  CPPUNIT_TEST(testFSC);     // 0117



	CPPUNIT_TEST(testWRPSB); // 0160
	CPPUNIT_TEST(testWRMDS); // 0161
	CPPUNIT_TEST(testWRWP);  // 0162
	CPPUNIT_TEST(testWRWDC); // 0163
//	CPPUNIT_TEST(testWRPTC); // 0164
//	CPPUNIT_TEST(testWRIT);  // 0165
	CPPUNIT_TEST(testWRXTS); // 0166
	CPPUNIT_TEST(testWRMP);  // 0167

	CPPUNIT_TEST(testRRPSB); // 0170
	CPPUNIT_TEST(testRRMDS); // 0171
	CPPUNIT_TEST(testRRWP);  // 0172
	CPPUNIT_TEST(testRRWDC); // 0173
//	CPPUNIT_TEST(testRRPTC); // 0174
//	CPPUNIT_TEST(testRRIT);  // 0175
	CPPUNIT_TEST(testRRXTS); // 0176
//  CPPUNIT_TEST(testA117);  // 0117

//	CPPUNIT_TEST(testOpcodeTrap);
//	CPPUNIT_TEST(testEscOpcodeTrap);

	CPPUNIT_TEST(testCOLORBLT_bit); // 0330
	CPPUNIT_TEST(testCOLORBLT_pat); // 0330

	CPPUNIT_TEST_SUITE_END();

	///////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////

	void testMW() {} // TODO MW
	void testMR() {} // TODO MR
	void testNC() {} // TODO NC
	void testBC() {} // TODO BC
	void testREQ() {} // TODO REQ
	void testSM_n() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aSM;
		MapFlags mf;
		mf.u = 0;
		mf.dirty = 1;
		mf.protect = 0;
		mf.referenced = 1;
		CARD16 rp = 0x123;
		CARD32 vp = 0x234;
		stack[SP++] = LowHalf(vp);
		stack[SP++] = HighHalf(vp);
		stack[SP++] = LowHalf(rp);
		stack[SP++] = HighHalf(rp);
		stack[SP++] = mf.u;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL((CARD16)0, SP);
		Memory::Map map = Memory::ReadMap(vp);
		CPPUNIT_ASSERT_EQUAL(mf.u, map.mf.u);
		CPPUNIT_ASSERT_EQUAL(rp, map.rp);
	}
	void testSM_v() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aSM;
		MapFlags mf;
		mf.u = 0;
		mf.dirty = 1;
		mf.protect = 1;
		mf.referenced = 0;
		CARD16 rp = 0x123;
		CARD32 vp = 0x234;
		stack[SP++] = LowHalf(vp);
		stack[SP++] = HighHalf(vp);
		stack[SP++] = LowHalf(rp);
		stack[SP++] = HighHalf(rp);
		stack[SP++] = mf.u;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL((CARD16)0, SP);
		Memory::Map map = Memory::ReadMap(vp);
		CPPUNIT_ASSERT_EQUAL(mf.u, map.mf.u);
		CPPUNIT_ASSERT_EQUAL((CARD16)0, map.rp);
	}
	void testGMF_n() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aGMF;
		CARD32 vp = Memory::MDS() / PageSize;
		stack[SP++] = LowHalf(vp);
		stack[SP++] = HighHalf(vp);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL((CARD16)3, SP);
		MapFlags mf = {0};
		CPPUNIT_ASSERT_EQUAL(mf.u, stack[0]);
		CPPUNIT_ASSERT_EQUAL(LowHalf(vp), stack[1]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(vp), stack[2]);
	}
	void testGMF_v() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aGMF;
		CARD32 vp = 0x00200000 / PageSize;
		stack[SP++] = LowHalf(vp);
		stack[SP++] = HighHalf(vp);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL((CARD16)3, SP);
		MapFlags mf = {0};
		mf.dirty = 1;
		mf.protect = 1;
		mf.referenced = 0;
		CPPUNIT_ASSERT_EQUAL(mf.u, stack[0]);
		CARD16 rp = 0;
		CPPUNIT_ASSERT_EQUAL(LowHalf(rp), stack[1]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(rp), stack[2]);
	}
	void testSMF_n() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aSMF;
		CARD32 vp = Memory::MDS() / PageSize;
		MapFlags mf;
		mf.u = 0;
		mf.dirty = 1;
		mf.protect = 0;
		mf.referenced = 1;
		stack[SP++] = LowHalf(vp);
		stack[SP++] = HighHalf(vp);
		stack[SP++] = mf.u;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL((CARD16)3, SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)0, stack[0]);
		CPPUNIT_ASSERT_EQUAL(LowHalf(vp), stack[1]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(vp), stack[2]);
		Memory::Map map = Memory::ReadMap(vp);
		CPPUNIT_ASSERT_EQUAL(mf.u, map.mf.u);
		CPPUNIT_ASSERT_EQUAL((CARD16)vp, map.rp);
	}
	void testSMF_v() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aSMF;
		CARD32 vp = 0x00200000 / PageSize;
		MapFlags mf;
		mf.u = 0;
		mf.dirty = 1;
		mf.protect = 0;
		mf.referenced = 1;
		stack[SP++] = LowHalf(vp);
		stack[SP++] = HighHalf(vp);
		stack[SP++] = mf.u;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL((CARD16)3, SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)6, stack[0]);
		CARD16 rp = 0;
		CPPUNIT_ASSERT_EQUAL(LowHalf(rp), stack[1]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(rp), stack[2]);
		Memory::Map map = Memory::ReadMap(vp);
		CPPUNIT_ASSERT_EQUAL((CARD16)6, map.mf.u);
		CPPUNIT_ASSERT_EQUAL((CARD16)0, map.rp);
	}

	void testAF() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aAF;
		FSIndex fsi = 10;
		stack[SP++] = fsi;
		CARD16 first = page_AV[fsi];
		CARD16 next = page_MDS[first];
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL((CARD16)1, SP);
		CPPUNIT_ASSERT_EQUAL(first, stack[0]);
		CPPUNIT_ASSERT_EQUAL(next, page_AV[fsi]);
	}
	void testFF() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aFF;
		FSIndex fsi = 10;
		LocalFrameHandle frame = 0x2010;
		page_MDS[frame - SIZE(LocalOverhead)] = fsi;
		stack[SP++] = frame;
		CARD16 first = page_AV[fsi];
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL((CARD16)0, SP);
		CPPUNIT_ASSERT_EQUAL(frame, page_AV[fsi]);
		CPPUNIT_ASSERT_EQUAL(first, page_MDS[frame]);
	}


	void testPI_nz() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aPI;
		ShortControlLink src = 0x1234;
		PortLink port = 0x0040;
		CARD16 vlaue = 0x1234;
		page_MDS[(port + OFFSET(Port, outport))] = vlaue;
		stack[0] = port;
		stack[1] = src;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL((CARD16)0, SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)0, page_MDS[(port + OFFSET(Port, inport))]);
		CPPUNIT_ASSERT_EQUAL(src, page_MDS[(port + OFFSET(Port, outport))]);
	}
	void testPI_z() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aPI;
		ShortControlLink src = 0x0000;
		PortLink port = 0x0040;
		CARD16 vlaue = 0x1234;
		page_MDS[(port + OFFSET(Port, outport))] = vlaue;
		stack[0] = port;
		stack[1] = src;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL((CARD16)0, SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)0, page_MDS[(port + OFFSET(Port, inport))]);
		CPPUNIT_ASSERT_EQUAL(vlaue, page_MDS[(port + OFFSET(Port, outport))]);
	}


	void testPO() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aPO;
		CARD16 port = 0x0040;
		CARD16 inport = 0x1234;
		//If the inport is non-zero, it contains a pointer to a frame
		NewProcDesc outport = {0};
		outport.pc = 0x22;
		outport.taggedGFI = GFI_EFC | 0x0003;
		// The outport may contain any type of control link, including a frame, a procedure, or the address of another port.
		page_MDS[port + 0] = inport;               // inport
		page_MDS[port + 1] = 0;                    // unused
		page_MDS[port + 2] = LowHalf(outport.u);   // outport
		page_MDS[port + 3] = HighHalf(outport.u);  // outport
		page_CB[outport.pc / 2] = 0; // set fsi = 0
		stack[SP++] = port;
		stack[SP++] = 0;
		CARD16 oLF = LFCache::LF();
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL((CARD16)0, SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)(savedPC + 2), page_MDS[oLF - 1]);
		CPPUNIT_ASSERT_EQUAL(oLF, page_MDS[port + 0]);
		//
		CPPUNIT_ASSERT_EQUAL(GFI_EFC, GFI);
		CPPUNIT_ASSERT_EQUAL((CARD16)(outport.pc + 1), PC);
		CPPUNIT_ASSERT_EQUAL(port, page_MDS[LFCache::LF() - 3]); // returnlink
		CPPUNIT_ASSERT_EQUAL(GFI,  page_MDS[LFCache::LF() - 2]); // globallink
	}
	void testPOR() {
		testPO();
	}


	void testSPP() {} // TODO SPP

	void testDI() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aDI;
		InterruptThread::setWDC(0);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL((CARD16)1, InterruptThread::getWDC());
		CPPUNIT_ASSERT_EQUAL((CARD16)0, SP);
	}
	void testEI() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aEI;
		InterruptThread::setWDC(1);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL((CARD16)0, InterruptThread::getWDC());
		CPPUNIT_ASSERT_EQUAL((CARD16)0, SP);
	}


	void testXOR() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aXOR;
		CARD16 u = 0x1234;
		CARD16 v = 0xcdef;
		stack[SP++] = u;
		stack[SP++] = v;
		Interpreter::execute();

		u = u ^ v;
		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(u, stack[0]);
	}


	void testDAND() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aDAND;
		CARD32 t = 0x12345678;
		CARD32 s = 0x90abcdef;
		stack[SP++] = LowHalf(s);
		stack[SP++] = HighHalf(s);
		stack[SP++] = LowHalf(t);
		stack[SP++] = HighHalf(t);
		Interpreter::execute();

		s = s & t;
		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(s),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(s), stack[1]);
	}
	void testDIOR() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aDIOR;
		CARD32 t = 0x12345678;
		CARD32 s = 0x90abcdef;
		stack[SP++] = LowHalf(s);
		stack[SP++] = HighHalf(s);
		stack[SP++] = LowHalf(t);
		stack[SP++] = HighHalf(t);
		Interpreter::execute();

		s = s | t;
		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(s),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(s), stack[1]);
	}
	void testDXOR() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aDXOR;
		CARD32 t = 0x12345678;
		CARD32 s = 0x90abcdef;
		stack[SP++] = LowHalf(s);
		stack[SP++] = HighHalf(s);
		stack[SP++] = LowHalf(t);
		stack[SP++] = HighHalf(t);
		Interpreter::execute();

		s = s ^ t;
		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(s),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(s), stack[1]);
	}


	void testROTATE_p() {
		page_CB[PC / 2] = zESC << 8 | aROTATE;
		INT16 shift = 4;
		CARD16 u = 0x1234;
		stack[SP++] = u;
		stack[SP++] = shift;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)0x2341, stack[0]);
	}
	void testROTATE_n() {
		page_CB[PC / 2] = zESC << 8 | aROTATE;
		INT16 shift = -4;
		CARD16 u = 0x1234;
		stack[SP++] = u;
		stack[SP++] = shift;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)0x4123, stack[0]);
	}


	void testDSHIFT() {
		page_CB[PC / 2] = zESC << 8 | aDSHIFT;
		INT16 shift = 12;
		CARD32 u = 0x12345678;
		stack[SP++] = LowHalf(u);
		stack[SP++] = HighHalf(u);
		stack[SP++] = shift;
		Interpreter::execute();

		CARD32 a = u << shift;
		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(a),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(a), stack[1]);
	}


	void testLINT_p() {
		page_CB[PC / 2] = zESC << 8 | aLINT;
		INT16 i = 5;
		stack[SP++] = i;
		Interpreter::execute();

		INT32 a = i;
		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(a),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(a), stack[1]);
	}
	void testLINT_n() {
		page_CB[PC / 2] = zESC << 8 | aLINT;
		INT16 i = -5;
		stack[SP++] = i;
		Interpreter::execute();

		INT32 a = i;
		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(a),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(a), stack[1]);
	}


	void testJS() {
		page_CB[PC / 2] = zESC << 8 | aJS;
		CARD16 u = 0x1234;
		stack[SP++] = u;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(u, PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}


	void testRCFS() {
		FieldSpec spec;
		spec.pos = 8;
		spec.size = 3;
		FieldDesc desc;
		desc.field = spec.u;
		desc.offset = 0x40;
		CARDINAL offset = 0x50;
		page_CB[PC / 2] = zESCL << 8 | aRCFS;
		page_CB[offset + desc.offset] = 0xABCD;
		stack[SP++] = offset;
		stack[SP++] = desc.u;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(0x0C, (int)stack[0]);
	}


	void testRC() {
		CARD8 alpha = 0x20;
		page_CB[(PC / 2) + 0] = zESCL << 8 | bRC;
		page_CB[(PC / 2) + 1] = alpha << 8 | 0x00;
		CARDINAL offset = 0x50;
		CARDINAL value = 0x1234;
		page_CB[offset + alpha] = value;
		stack[SP++] = offset;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 3, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value, stack[0]);
	}

	void testUDIV_n() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aUDIV;
		CARD16 s = 54321;
		CARD16 t = 12345;
		stack[SP++] = s;
		stack[SP++] = t;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)(s / t), stack[0]);
		CPPUNIT_ASSERT_EQUAL((CARD16)(s % t), stack[1]);
	}
	void testUDIV_t() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aUDIV;
		CARD16 s = 54321;
		CARD16 t = 00000;
		stack[SP++] = s;
		stack[SP++] = t;

		int catchException = 0;
		try {
			Interpreter::execute();
		} catch (Abort &info) {
			catchException = 1;
		}

		CPPUNIT_ASSERT_EQUAL(1, catchException);
		CPPUNIT_ASSERT_EQUAL(pc_SD + sDivZeroTrap + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(GFI_SD, GFI);
	}


	void testLUDIV_n() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aLUDIV;
		CARD32 s = 123456789;
		CARD16 t = 56789;
		stack[SP++] = LowHalf(s);
		stack[SP++] = HighHalf(s);
		stack[SP++] = t;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)LowHalf(s / t), stack[0]);
		CPPUNIT_ASSERT_EQUAL((CARD16)LowHalf(s % t), stack[1]);
	}
	void testLUDIV_dzt() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aLUDIV;
		CARD32 s = 123456789;
		CARD16 t = 00000;
		stack[SP++] = LowHalf(s);
		stack[SP++] = HighHalf(s);
		stack[SP++] = t;

		int catchException = 0;
		try {
			Interpreter::execute();
		} catch (Abort &info) {
			catchException = 1;
		}

		CPPUNIT_ASSERT_EQUAL(1, catchException);
		CPPUNIT_ASSERT_EQUAL(pc_SD + sDivZeroTrap + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(GFI_SD, GFI);
	}
	void testLUDIV_dct() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aLUDIV;
		CARD32 s = 0x44442222;
		CARD16 t = 0x1111;
		stack[SP++] = LowHalf(s);
		stack[SP++] = HighHalf(s);
		stack[SP++] = t;

		int catchException = 0;
		try {
			Interpreter::execute();
		} catch (Abort &info) {
			catchException = 1;
		}

		CPPUNIT_ASSERT_EQUAL(1, catchException);
		CPPUNIT_ASSERT_EQUAL(pc_SD + sDivCheckTrap + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(GFI_SD, GFI);
	}


	void testROB() {
		CARD8 alpha = 2;
		page_CB[(PC / 2) + 0] = zESCL << 8 | bROB;
		page_CB[(PC / 2) + 1] = alpha << 8 | 0x00;
		POINTER ptr = 0x60;
		stack[SP++] = ptr;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 3, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(page_MDS[(ptr - alpha + 0)], stack[0]);
	}
	void testWOB() {
		CARD8 alpha = 2;
		page_CB[(PC / 2) + 0] = zESCL << 8 | bWOB;
		page_CB[(PC / 2) + 1] = alpha << 8 | 0x00;
		POINTER ptr = 0x60;
		CARD16 value = 0x1234;
		stack[SP++] = value;
		stack[SP++] = ptr;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 3, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value, page_MDS[(ptr - alpha + 0)]);
	}


	void testXE() {
		CARD8 alpha = 0x10;
		page_CB[(PC / 2) + 0] = zESCL << 8 | bXE;
		page_CB[(PC / 2) + 1] = alpha << 8 | 0x00;
		ShortControlLink src = 0;
		NewProcDesc dst;
		dst.pc = 0x40;
		dst.taggedGFI = GFI_EFC | 0x0003;
		page_LF[alpha + 0] = src;
		page_LF[alpha + 1] = 0;
		page_LF[alpha + 2] = LowHalf(dst.u);
		page_LF[alpha + 3] = HighHalf(dst.u);
		page_CB[dst.pc / 2] = 0; // set fsi = 0
		InterruptThread::setWDC(1);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL((CARD16)(savedPC + 3), page_LF[-1]);
		CPPUNIT_ASSERT_EQUAL(GFI_EFC, GFI);
		CPPUNIT_ASSERT_EQUAL((CARD16)(dst.pc + 1), PC);
		CPPUNIT_ASSERT_EQUAL((CARD16)0, InterruptThread::getWDC());
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}


	void testXF() {
		CARD8 alpha = 0x10;
		page_CB[(PC / 2) + 0] = zESCL << 8 | bXF;
		page_CB[(PC / 2) + 1] = alpha << 8 | 0x00;
		ShortControlLink src = 0;
		NewProcDesc dst;
		dst.pc = 0x40;
		dst.taggedGFI = GFI_EFC | 0x0003;
		// ProcDesc pd;
		page_LF[alpha + 0] = src;             // src
		page_LF[alpha + 1] = 0;               // reserved
		page_LF[alpha + 2] = LowHalf(dst.u);  // dst
		page_LF[alpha + 3] = HighHalf(dst.u); // dst
		page_CB[dst.pc / 2] = 0; // set fsi = 0
		page_LF[-4] = 0; // fsi in LocalWord;
		CARD16 oLF = LFCache::LF();
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(GFI_EFC, GFI);
		CPPUNIT_ASSERT_EQUAL((CARD16)(dst.pc + 1), PC);
		CPPUNIT_ASSERT_EQUAL((CARD16)1, InterruptThread::getWDC());
		CPPUNIT_ASSERT_EQUAL(page_AV[0], oLF);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
	}

	void testDSK() {
		CARD8 alpha = 0x10;
		page_CB[(PC / 2) + 0] = zESCL << 8 | bDSK;
		page_CB[(PC / 2) + 1] = alpha << 8 | 0x00;
		for(int i = 0; i < StackDepth; i++) stack[i] = 0xaa00 | i;
		SP = StackDepth;
		breakByte = 0xaa;
		Interpreter::execute();

		StateWord sw;
		sw.breakByte = 0xaa;
		sw.stkptr = StackDepth;
		CPPUNIT_ASSERT_EQUAL(savedPC + 3, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(0, (int)savedSP);
		CPPUNIT_ASSERT_EQUAL(0, (int)breakByte);
		for(int i = 0; i < StackDepth; i++)
			CPPUNIT_ASSERT_EQUAL((CARD16)(0xaa00 | i), page_LF[(alpha + i)]);
		CPPUNIT_ASSERT_EQUAL(sw.u, page_LF[(alpha + StackDepth)]);
	}
	void testLSK() {
		CARD8 alpha = 0x10;
		page_CB[(PC / 2) + 0] = zESCL << 8 | bLSK;
		page_CB[(PC / 2) + 1] = alpha << 8 | 0x00;
		for(int i = 0; i < StackDepth; i++)
			page_LF[(alpha + i)] = (CARD16)(0xaa00 | i);
		StateWord sw;
		sw.breakByte = 0xaa;
		sw.stkptr = StackDepth;
		page_LF[(alpha + StackDepth)] = sw.u;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 3, (int)PC);
		CPPUNIT_ASSERT_EQUAL(sw.stkptr, SP);
		CPPUNIT_ASSERT_EQUAL(sw.stkptr, savedSP);
		CPPUNIT_ASSERT_EQUAL(sw.breakByte, (CARD16)breakByte);
		for(int i = 0; i < StackDepth; i++)
			CPPUNIT_ASSERT_EQUAL((CARD16)(0xaa00 | i), stack[i]);
	}

	void testBNDCKL_n() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aBNDCKL;
		CARD32 range = 0x12345678;
		CARD32 index = 0x11111111;
		stack[SP++] = LowHalf(index);
		stack[SP++] = HighHalf(index);
		stack[SP++] = LowHalf(range);
		stack[SP++] = HighHalf(range);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL((CARD16)(savedPC + 2), PC);
		CPPUNIT_ASSERT_EQUAL((CARD16)2, SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(index),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(index), stack[1]);
	}
	void testBNDCKL_t() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aBNDCKL;
		CARD32 range = 0x11111111;
		CARD32 index = 0x12345678;
		stack[SP++] = LowHalf(index);
		stack[SP++] = HighHalf(index);
		stack[SP++] = LowHalf(range);
		stack[SP++] = HighHalf(range);

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


	void testNILCK_n() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aNILCK;
		CARD16 ptr = 0x1234;
		stack[SP++] = ptr;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL((CARD16)(savedPC + 2), PC);
		CPPUNIT_ASSERT_EQUAL((CARD16)1, SP);
		CPPUNIT_ASSERT_EQUAL(ptr,  stack[0]);
	}
	void testNILCK_t() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aNILCK;
		CARD16 ptr = 0x0000;
		stack[SP++] = ptr;

		int catchException = 0;
		try {
			Interpreter::execute();
		} catch (Abort &info) {
			catchException = 1;
		}

		CPPUNIT_ASSERT_EQUAL(1, catchException);
		CPPUNIT_ASSERT_EQUAL(pc_SD + sPointerTrap + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(GFI_SD, GFI);
	}

	void testNILCKL_n() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aNILCKL;
		CARD32 ptr = 0x12345678;
		stack[SP++] = LowHalf(ptr);
		stack[SP++] = HighHalf(ptr);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL((CARD16)(savedPC + 2), PC);
		CPPUNIT_ASSERT_EQUAL((CARD16)2, SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(ptr),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(ptr), stack[1]);
	}
	void testNILCKL_t() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aNILCKL;
		CARD32 ptr = 0x00000000;
		stack[SP++] = LowHalf(ptr);
		stack[SP++] = HighHalf(ptr);

		int catchException = 0;
		try {
			Interpreter::execute();
		} catch (Abort &info) {
			catchException = 1;
		}

		CPPUNIT_ASSERT_EQUAL(1, catchException);
		CPPUNIT_ASSERT_EQUAL(pc_SD + sPointerTrap + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(GFI_SD, GFI);
	}

	void testBLTLR_n() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aBLTLR;
		LONG_POINTER dest   = Memory::MDS() + 0x0020;
		CARDINAL     count  = 0x0010;
		LONG_POINTER source = Memory::MDS() + 0x0070;
		for(int i = count - 1; 0 <= i; i--) page_MDS[source - Memory::MDS() + i] = (0xB000 | i);
		stack[SP++] = LowHalf(source);
		stack[SP++] = HighHalf(source);
		stack[SP++] = count;
		stack[SP++] = LowHalf(dest);
		stack[SP++] = HighHalf(dest);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		for(int i = count - 1; 0 <= i; i--)
			CPPUNIT_ASSERT_EQUAL((CARD16)(0xB000 | i), page_MDS[dest - Memory::MDS() + i]);
    }
	void testBLTLR_o() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aBLTLR;
		LONG_POINTER dest   = Memory::MDS() + 0x006f;
		CARDINAL     count  = 0x0010;
		LONG_POINTER source = Memory::MDS() + 0x0070;
		page_MDS[source - Memory::MDS() + count - 1] = 0x1234;
		stack[SP++] = LowHalf(source);
		stack[SP++] = HighHalf(source);
		stack[SP++] = count;
		stack[SP++] = LowHalf(dest);
		stack[SP++] = HighHalf(dest);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		for(int i = count - 1; 0 <= i; i--)
			CPPUNIT_ASSERT_EQUAL((CARD16)0x1234, page_MDS[dest - Memory::MDS() + i]);
    }
	void testBLTLR_l() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aBLTLR;
		LONG_POINTER dest   = Memory::MDS() + 0x0630;
		CARDINAL     count  = 0x0340;
		LONG_POINTER source = Memory::MDS() + 0x0070;
		for(int i = count - 1; 0 <= i; i--) page_MDS[source - Memory::MDS() + i] = (0xB000 | i);
		stack[SP++] = LowHalf(source);
		stack[SP++] = HighHalf(source);
		stack[SP++] = count;
		stack[SP++] = LowHalf(dest);
		stack[SP++] = HighHalf(dest);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		for(int i = count - 1; 0 <= i; i--)
			CPPUNIT_ASSERT_EQUAL((CARD16)(0xB000 | i), page_MDS[dest - Memory::MDS() + i]);
    }

	void testBLEL_t() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aBLEL;
		LONG_POINTER dest   = Memory::MDS() + 0x0020;
		CARDINAL     count  = 0x0010;
		LONG_POINTER source = Memory::MDS() + 0x0080;
		for(CARDINAL i = 0; i < count; i++) page_MDS[source - Memory::MDS() + i] = (0xB000 | i);
		for(CARDINAL i = 0; i < count; i++) page_MDS[dest   - Memory::MDS() + i] = (0xB000 | i);
		stack[SP++] = LowHalf(source);
		stack[SP++] = HighHalf(source);
		stack[SP++] = count;
		stack[SP++] = LowHalf(dest);
		stack[SP++] = HighHalf(dest);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)1, stack[0]);
    }
	void testBLEL_f() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aBLEL;
		LONG_POINTER dest   = Memory::MDS() + 0x0020;
		CARDINAL     count  = 0x0010;
		LONG_POINTER source = Memory::MDS() + 0x0080;
		for(CARDINAL i = 0; i < count; i++) page_MDS[source - Memory::MDS() + i] = (0xB000 | i);
		for(CARDINAL i = 1; i < count; i++) page_MDS[dest   - Memory::MDS() + i] = (0xB000 | i);
		stack[SP++] = LowHalf(source);
		stack[SP++] = HighHalf(source);
		stack[SP++] = count;
		stack[SP++] = LowHalf(dest);
		stack[SP++] = HighHalf(dest);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)0, stack[0]);
    }
	void testBLEL_l() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aBLEL;
		LONG_POINTER dest   = Memory::MDS() + 0x0020;
		CARDINAL     count  = 0x0340;
		LONG_POINTER source = Memory::MDS() + 0x0680;
		for(CARDINAL i = 0; i < count; i++) page_MDS[source - Memory::MDS() + i] = (0xB000 | i);
		for(CARDINAL i = 0; i < count; i++) page_MDS[dest   - Memory::MDS() + i] = (0xB000 | i);
		stack[SP++] = LowHalf(source);
		stack[SP++] = HighHalf(source);
		stack[SP++] = count;
		stack[SP++] = LowHalf(dest);
		stack[SP++] = HighHalf(dest);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)1, stack[0]);
    }

	void testBLECL_t() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aBLECL;
		LONG_POINTER ptr    = Memory::MDS() + 0x0020;
		CARDINAL     count  = 0x0010;
		CARDINAL     offset = 0x0080;
		for(CARDINAL i = 0; i < count; i++) page_MDS[ptr - Memory::MDS() + i] = (0xB000 | i);
		for(CARDINAL i = 0; i < count; i++) page_CB [offset    + i] = (0xB000 | i);
		stack[SP++] = offset;
		stack[SP++] = count;
		stack[SP++] = LowHalf(ptr);
		stack[SP++] = HighHalf(ptr);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)1, stack[0]);
    }
	void testBLECL_f() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aBLECL;
		LONG_POINTER ptr    = Memory::MDS() + 0x0020;
		CARDINAL     count  = 0x0010;
		CARDINAL     offset = 0x0080;
		for(CARDINAL i = 0; i < count; i++) page_MDS[ptr - Memory::MDS() + i] = (0xB000 | i);
		for(CARDINAL i = 0; i < count; i++) page_CB [offset    + i] = (0xBB01 | i);
		stack[SP++] = offset;
		stack[SP++] = count;
		stack[SP++] = LowHalf(ptr);
		stack[SP++] = HighHalf(ptr);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)0, stack[0]);
    }
	void testBLECL_l() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aBLECL;
		LONG_POINTER ptr   = Memory::MDS() + 0x0620;
		CARDINAL     count = 0x0340;
		CARDINAL    offset = 0x0080;
		for(CARDINAL i = 0; i < count; i++) page_MDS[ptr - Memory::MDS() + i] = (0xB000 | i);
		for(CARDINAL i = 0; i < count; i++) page_CB [offset    + i] = (0xB000 | i);
		stack[SP++] = offset;
		stack[SP++] = count;
		stack[SP++] = LowHalf(ptr);
		stack[SP++] = HighHalf(ptr);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD16)1, stack[0]);
    }

	void testCKSUM() {} // TODO CKSUM
	void testBITBLT() {} // TODO BITBLT
	void testTXTBLT() {} // TODO TXTBLT

	void testBYTBLT_n() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aBYTBLT;
		CARDINAL     sourceOffset = 0x20;
		LONG_POINTER sourceBase   = Memory::MDS() + 0x40;
		CARDINAL     count        = 16;
		CARDINAL     destOffset   = 0x50;
		LONG_POINTER destBase     = Memory::MDS() + 0x80;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 0] = 0x0011;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 1] = 0x2233;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 2] = 0x4455;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 3] = 0x6677;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 4] = 0x8899;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 5] = 0xAABB;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 6] = 0xCCDD;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 7] = 0xEEFF;

		stack[SP++] = LowHalf(destBase);
		stack[SP++] = HighHalf(destBase);
		stack[SP++] = destOffset;
		stack[SP++] = count;
		stack[SP++] = LowHalf(sourceBase);
		stack[SP++] = HighHalf(sourceBase);
		stack[SP++] = sourceOffset;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);

		CPPUNIT_ASSERT_EQUAL((CARD16)0x0011, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 0]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0x2233, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 1]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0x4455, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 2]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0x6677, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 3]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0x8899, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 4]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0xAABB, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 5]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0xCCDD, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 6]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0xEEFF, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 7]);
	}
	void testBYTBLT_o() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aBYTBLT;
		CARDINAL     sourceOffset = 0x20;
		LONG_POINTER sourceBase   = Memory::MDS() + 0x40;
		CARDINAL     count        = 16;
		CARDINAL     destOffset   = 0x22;
		LONG_POINTER destBase     = Memory::MDS() + 0x40;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 0] = 0x0011;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 1] = 0x2233;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 2] = 0x4455;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 3] = 0x6677;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 4] = 0x8899;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 5] = 0xAABB;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 6] = 0xCCDD;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 7] = 0xEEFF;

		stack[SP++] = LowHalf(destBase);
		stack[SP++] = HighHalf(destBase);
		stack[SP++] = destOffset;
		stack[SP++] = count;
		stack[SP++] = LowHalf(sourceBase);
		stack[SP++] = HighHalf(sourceBase);
		stack[SP++] = sourceOffset;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);

		CPPUNIT_ASSERT_EQUAL((CARD16)0x0011, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 0]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0011, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 1]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0011, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 2]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0011, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 3]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0011, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 4]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0011, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 5]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0011, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 6]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0011, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 7]);
	}
	void testBYTBLT_l() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aBYTBLT;
		CARDINAL     sourceOffset = 0x20;
		LONG_POINTER sourceBase   = Memory::MDS() + 0x0640;
		CARDINAL     count        = 0x0240;
		CARDINAL     destOffset   = 0x50;
		LONG_POINTER destBase     = Memory::MDS() + 0x0080;
		for(CARDINAL i = 0; i < (count / 2); i++) page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + i] = (0xB000 | i);
		stack[SP++] = LowHalf(destBase);
		stack[SP++] = HighHalf(destBase);
		stack[SP++] = destOffset;
		stack[SP++] = count;
		stack[SP++] = LowHalf(sourceBase);
		stack[SP++] = HighHalf(sourceBase);
		stack[SP++] = sourceOffset;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		for(CARDINAL i = 0; i < (count / 2); i++)
			CPPUNIT_ASSERT_EQUAL((CARD16)(0xB000 | i), page_MDS[destBase - Memory::MDS() + (destOffset / 2) + i]);
	}
	void testBYTBLTR_n() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aBYTBLTR;
		CARDINAL     sourceOffset = 0x20;
		LONG_POINTER sourceBase   = Memory::MDS() + 0x40;
		CARDINAL     count        = 16;
		CARDINAL     destOffset   = 0x40;
		LONG_POINTER destBase     = Memory::MDS() + 0x80;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 0] = 0x0011;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 1] = 0x2233;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 2] = 0x4455;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 3] = 0x6677;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 4] = 0x8899;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 5] = 0xAABB;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 6] = 0xCCDD;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 7] = 0xEEFF;

		stack[SP++] = LowHalf(destBase);
		stack[SP++] = HighHalf(destBase);
		stack[SP++] = destOffset;
		stack[SP++] = count;
		stack[SP++] = LowHalf(sourceBase);
		stack[SP++] = HighHalf(sourceBase);
		stack[SP++] = sourceOffset;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);

		CPPUNIT_ASSERT_EQUAL((CARD16)0x0011, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 0]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0x2233, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 1]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0x4455, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 2]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0x6677, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 3]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0x8899, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 4]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0xAABB, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 5]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0xCCDD, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 6]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0xEEFF, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 7]);
	}
	void testBYTBLTR_o() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aBYTBLTR;
		CARDINAL     sourceOffset = 0x20;
		LONG_POINTER sourceBase   = Memory::MDS() + 0x40;
		CARDINAL     count        = 16;
		CARDINAL     destOffset   = 0x1E;
		LONG_POINTER destBase     = Memory::MDS() + 0x40;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 0] = 0x0011;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 1] = 0x2233;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 2] = 0x4455;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 3] = 0x6677;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 4] = 0x8899;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 5] = 0xAABB;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 6] = 0xCCDD;
		page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + 7] = 0xEEFF;

		stack[SP++] = LowHalf(destBase);
		stack[SP++] = HighHalf(destBase);
		stack[SP++] = destOffset;
		stack[SP++] = count;
		stack[SP++] = LowHalf(sourceBase);
		stack[SP++] = HighHalf(sourceBase);
		stack[SP++] = sourceOffset;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);

		CPPUNIT_ASSERT_EQUAL((CARD16)0xEEFF, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 0]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0xEEFF, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 1]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0xEEFF, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 2]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0xEEFF, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 3]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0xEEFF, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 4]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0xEEFF, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 5]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0xEEFF, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 6]);
		CPPUNIT_ASSERT_EQUAL((CARD16)0xEEFF, page_MDS[destBase - Memory::MDS() + (destOffset / 2) + 7]);
	}
	void testBYTBLTR_l() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aBYTBLTR;
		CARDINAL     sourceOffset = 0x20;
		LONG_POINTER sourceBase   = Memory::MDS() + 0x40;
		CARDINAL     count        = 260;
		CARDINAL     destOffset   = 0x400;
		LONG_POINTER destBase     = Memory::MDS() + 0x80;

		for(int i = 0; i < (count / 2); i++) {
			page_MDS[sourceBase - Memory::MDS() + (sourceOffset / 2) + i] = (CARD16)(0xB000 | i);
		}

		stack[SP++] = LowHalf(destBase);
		stack[SP++] = HighHalf(destBase);
		stack[SP++] = destOffset;
		stack[SP++] = count;
		stack[SP++] = LowHalf(sourceBase);
		stack[SP++] = HighHalf(sourceBase);
		stack[SP++] = sourceOffset;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);

		for(int i = 0; i < (count / 2); i++) {
			CPPUNIT_ASSERT_EQUAL((CARD16)(0xB000 | i), page_MDS[destBase - Memory::MDS() + (destOffset / 2) + i]);
		}
	}
	void testVERSION() {} // TODO VERSION


	void testDMUL() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aDMUL;
		CARD32 s = 0x11111111;
		CARD32 t = 0x12345678;
		stack[SP++] = LowHalf(s);
		stack[SP++] = HighHalf(s);
		stack[SP++] = LowHalf(t);
		stack[SP++] = HighHalf(t);
		Interpreter::execute();

		CARD32 a = s * t;
		CPPUNIT_ASSERT_EQUAL((CARD16)(savedPC + 2), PC);
		CPPUNIT_ASSERT_EQUAL((CARD16)2, SP);
		CPPUNIT_ASSERT_EQUAL(LowHalf(a),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(a), stack[1]);
	}


	void testSDIV_n() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aSDIV;
		INT16 j = 54321;
		INT16 k = 12345;
		stack[SP++] = j;
		stack[SP++] = k;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL((INT16)(j / k), (INT16)stack[0]);
		CPPUNIT_ASSERT_EQUAL((INT16)(j % k), (INT16)stack[1]);
	}
	void testSDIV_t() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aSDIV;
		INT16 j = 54321;
		INT16 k = 00000;
		stack[SP++] = j;
		stack[SP++] = k;

		int catchException = 0;
		try {
			Interpreter::execute();
		} catch (Abort &info) {
			catchException = 1;
		}

		CPPUNIT_ASSERT_EQUAL(1, catchException);
		CPPUNIT_ASSERT_EQUAL(pc_SD + sDivZeroTrap + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(GFI_SD, GFI);
	}


	void testSDDIV_n() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aSDDIV;
		INT32 j = 87654321;
		INT32 k = 12345678;
		stack[SP++] = LowHalf(j);
		stack[SP++] = HighHalf(j);
		stack[SP++] = LowHalf(k);
		stack[SP++] = HighHalf(k);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		INT32 a = j / k;
		CPPUNIT_ASSERT_EQUAL(LowHalf(a),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(a), stack[1]);
		INT32 b = j % k;
		CPPUNIT_ASSERT_EQUAL(LowHalf(b),  stack[2]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(b), stack[3]);
	}
	void testSDDIV_t() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aSDDIV;
		INT32 j = 87654321;
		INT32 k = 00000000;
		stack[SP++] = LowHalf(j);
		stack[SP++] = HighHalf(j);
		stack[SP++] = LowHalf(k);
		stack[SP++] = HighHalf(k);

		int catchException = 0;
		try {
			Interpreter::execute();
		} catch (Abort &info) {
			catchException = 1;
		}

		CPPUNIT_ASSERT_EQUAL(1, catchException);
		CPPUNIT_ASSERT_EQUAL(pc_SD + sDivZeroTrap + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(GFI_SD, GFI);
	}


	void testUDDIV_n() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aUDDIV;
		CARD32 s = 87654321;
		CARD32 t = 12345678;
		stack[SP++] = LowHalf(s);
		stack[SP++] = HighHalf(s);
		stack[SP++] = LowHalf(t);
		stack[SP++] = HighHalf(t);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
		CARD32 a = s / t;
		CPPUNIT_ASSERT_EQUAL(LowHalf(a),  stack[0]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(a), stack[1]);
		CARD32 b = s % t;
		CPPUNIT_ASSERT_EQUAL(LowHalf(b),  stack[2]);
		CPPUNIT_ASSERT_EQUAL(HighHalf(b), stack[3]);
	}
	void testUDDIV_t() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aUDDIV;
		CARD32 s = 87654321;
		CARD32 t = 00000000;
		stack[SP++] = LowHalf(s);
		stack[SP++] = HighHalf(s);
		stack[SP++] = LowHalf(t);
		stack[SP++] = HighHalf(t);

		int catchException = 0;
		try {
			Interpreter::execute();
		} catch (Abort &info) {
			catchException = 1;
		}

		CPPUNIT_ASSERT_EQUAL(1, catchException);
		CPPUNIT_ASSERT_EQUAL(pc_SD + sDivZeroTrap + 1, (int)PC);
		CPPUNIT_ASSERT_EQUAL(GFI_SD, GFI);
	}


	void testWRPSB() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aWRPSB;
		CARD16 n = 128;
		stack[SP++] = n;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(Index(n), PSB);
	}
	void testWRMDS() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aWRMDS;
		CARD16 n = 0x1234;
		stack[SP++] = n;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL((CARD32)(n << WordSize), Memory::MDS());
	}
	void testWRWP() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aWRWP;
		CARD16 n = 0x1234;
		stack[SP++] = n;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(n, InterruptThread::getWP());
	}
	void testWRWDC() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aWRWDC;
		CARD16 n = 0x1234;
		stack[SP++] = n;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(n, InterruptThread::getWDC());
	}
//	void testWRPTC() {
//		page_CB[(PC / 2) + 0] = zESC << 8 | aWRPTC;
//		CARD16 n = 0x1234;
//		stack[SP++] = n;
//		CARD32 value = 0x12345678;
//		IT = value;
//		Interpreter::execute();
//
//		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
//		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
//		CPPUNIT_ASSERT_EQUAL(n, PTC);
//		CPPUNIT_ASSERT_EQUAL(value, time_CheckForTimeouts);
//	}
//	void testWRIT() {
//		page_CB[(PC / 2) + 0] = zESC << 8 | aWRIT;
//		CARD32 n = 0x12345678;
//		stack[SP++] = LowHalf(n);
//		stack[SP++] = HighHalf(n);
//		Interpreter::execute();
//
//		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
//		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
//		CPPUNIT_ASSERT_EQUAL(n, IT);
//	}
	void testWRXTS() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aWRXTS;
		CARD16 n = 0x1234;
		stack[SP++] = n;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(n, XTS);
	}
	void testWRMP() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aWRMP;
		CARD16 n = 900;
		stack[SP++] = n;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(0, (int)SP);
		CPPUNIT_ASSERT_EQUAL(n, MP);
	}
	void testRRPSB() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aRRPSB;
		PSB = 55;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(Handle(PSB), stack[0]);
	}
	void testRRMDS() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aRRMDS;
		CARD16 value = 0x1234;
		Memory::setMDS(value << 16);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(value, stack[0]);
	}
	void testRRWP() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aRRWP;
		InterruptThread::setWP(0x1234);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(InterruptThread::getWP(), stack[0]);
	}
	void testRRWDC() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aRRWDC;
		InterruptThread::setWDC(0x1234);
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(InterruptThread::getWDC(), stack[0]);
	}
//	void testRRPTC() {
//		page_CB[(PC / 2) + 0] = zESC << 8 | aRRPTC;
//		PTC = 0x1234;
//		Interpreter::execute();
//
//		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
//		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
//		CPPUNIT_ASSERT_EQUAL(PTC, stack[0]);
//	}
//	void testRRIT() {
//		page_CB[(PC / 2) + 0] = zESC << 8 | aRRIT;
//		IT = 0x12345678;
//		Interpreter::execute();
//
//		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
//		CPPUNIT_ASSERT_EQUAL(2, (int)SP);
//		CPPUNIT_ASSERT_EQUAL(LowHalf(IT),  stack[0]);
//		CPPUNIT_ASSERT_EQUAL(HighHalf(IT), stack[1]);
//	}
	void testRRXTS() {
		page_CB[(PC / 2) + 0] = zESC << 8 | aRRXTS;
		XTS = 0x1234;
		Interpreter::execute();

		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
		CPPUNIT_ASSERT_EQUAL(XTS, stack[0]);
	}

//	void testOpcodeTrap() {
//		page_CB[(PC / 2) + 0] = (CARD8)0376 << 8 | 0x00;
//		int catchException = 0;
//		try {
//			Interpreter::execute();
//		} catch (Abort &info) {
//			catchException = 1;
//		}
//
//		CPPUNIT_ASSERT_EQUAL(1, catchException);
//		CPPUNIT_ASSERT_EQUAL(pc_SD + sOpcodeTrap + 1, (int)PC);
//		CPPUNIT_ASSERT_EQUAL(GFI_SD, GFI);
//	}
//
//	void testEscOpcodeTrap() {
//		CARD8 escOpcode = 0x40;
//		page_CB[(PC / 2) + 0] = zESC << 8 | escOpcode;
//		int catchException = 0;
//		try {
//			Interpreter::execute();
//		} catch (Abort &info) {
//			catchException = 1;
//		}
//
//		CPPUNIT_ASSERT_EQUAL(1, catchException);
//		CPPUNIT_ASSERT_EQUAL(pc_ETT + escOpcode + 1, (int)PC);
//		CPPUNIT_ASSERT_EQUAL(GFI_ETT, GFI);
//	}


	void testCOLORBLT_bit() {
		// TODO write test case COLORBLT bit
		// case 1  3x3
//		CARD16 ptr = 0x1000;
//
//		CARD32 dstAddress = Memory::MDS() + 0x3000;
//		CARD16 dstPixel   = 0x0;
//		INT16  dstPpl     = 0x30;
//		CARD32 srcAddress = Memory::MDS() + 0x2000;
//		CARD16 srcPixel   = 0x0;
//		INT16  srcPpl     = 0x40;
//		CARD16 width      = 0x30;
//		CARD16 height     = 0x3;
//		CARD16 flags      = 0x0;
//		CARD16 color0     = 0x0;
//		CARD16 color1     = 0x0;
//
//		CARD16 direction  = ColorBlt::D_forward;
//		CARD16 srcType    = ColorBlt::PT_bit;
//		CARD16 dstType    = ColorBlt::PT_bit;
//		CARD16 pattern    = 0;
//		CARD16 srcFunc    = ColorBlt::SF_null;
//		CARD16 dstFunc    = ColorBlt::DF_src;
//		CARD16 reserved   = 0;
//
//
//		page_MDS[ptr +  0] = LowHalf(dstAddress);
//		page_MDS[ptr +  1] = HighHalf(dstAddress);
//		page_MDS[ptr +  2] = dstPixel;
//		page_MDS[ptr +  3] = dstPpl;
//		page_MDS[ptr +  4] = LowHalf(srcAddress);
//		page_MDS[ptr +  5] = HighHalf(srcAddress);
//		page_MDS[ptr +  6] = srcPixel;
//		page_MDS[ptr +  7] = srcPpl;
//		page_MDS[ptr +  8] = width;
//		page_MDS[ptr +  9] = height;
//		page_MDS[ptr + 10] = direction << 15 | srcType << 14 | dstType << 13 | pattern << 12 | srcFunc << 11 | dstFunc << 8;
//		page_MDS[ptr + 11] = color0;
//		page_MDS[ptr + 12] = color1;
//
//		page_CB[(PC / 2) + 0] = zESC   << 8 | aCOLORBLT;
//		page_CB[(PC / 2) + 1] = ptr;
//		Interpreter::execute();
//
//		CPPUNIT_ASSERT_EQUAL(savedPC + 2, (int)PC);
//		CPPUNIT_ASSERT_EQUAL(1, (int)SP);
	}
	void testCOLORBLT_pat() {
		// TODO write test case COLORBLT pat
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION(testOpcode_esc);
