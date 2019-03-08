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


#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("testType");

#include "testBase.h"

class testType : public testBase {

	CPPUNIT_TEST_SUITE(testType);

	CPPUNIT_TEST(testBasicType);
	CPPUNIT_TEST(testNibblePair);
	CPPUNIT_TEST(testBytePair);
	CPPUNIT_TEST(testBasicLogical);
	CPPUNIT_TEST(testShift);
	CPPUNIT_TEST(testRotate);
	CPPUNIT_TEST(testArithShift);
	CPPUNIT_TEST(testLong);
	CPPUNIT_TEST(testLongShift);
	CPPUNIT_TEST(testLongArithShift);
	CPPUNIT_TEST(testSignExtend);

	CPPUNIT_TEST(testMapFlags);
	CPPUNIT_TEST(testCodeSegment);
	CPPUNIT_TEST(testGlobalWord);
	CPPUNIT_TEST(testGlobalOverhead);
	CPPUNIT_TEST(testOldGlobalOverhead);
	CPPUNIT_TEST(testGFTItem);
	CPPUNIT_TEST(testGlobalFrameTable);
	CPPUNIT_TEST(testLocalWord);
	CPPUNIT_TEST(testLocalOverhead);

	CPPUNIT_TEST(testFieldSpec);
	CPPUNIT_TEST(testFieldDesc);
	CPPUNIT_TEST(testBitAddress);
	CPPUNIT_TEST(testBitBltArg);
	CPPUNIT_TEST(testBitBltFlags);
	CPPUNIT_TEST(testGrayParm);

	CPPUNIT_TEST(testTaggedControlLink);
	CPPUNIT_TEST(testMakeFrameLink);
	CPPUNIT_TEST(testMakeIndirectLink);
	CPPUNIT_TEST(testProcDesc);
	CPPUNIT_TEST(testMakeProcDesc);
	CPPUNIT_TEST(testNewProcDesc);
	CPPUNIT_TEST(testMakeNewProcDesc);
	CPPUNIT_TEST(testAVItem);

	CPPUNIT_TEST(testPort);
	CPPUNIT_TEST(testSystemData);
	CPPUNIT_TEST(testEscTrapTable);
	CPPUNIT_TEST(testStateWord);
	CPPUNIT_TEST(testStateVector);
	CPPUNIT_TEST(testTransferDescriptor);

	CPPUNIT_TEST(testPsbLink);
	CPPUNIT_TEST(testPsbFlags);
	CPPUNIT_TEST(testMonitor);
	CPPUNIT_TEST(testCondition);
	CPPUNIT_TEST(testQueue);

	CPPUNIT_TEST(testStateAllocationTable);
	CPPUNIT_TEST(testFaultVector);
	CPPUNIT_TEST(testInterruptVector);

	CPPUNIT_TEST(testProcessStateBlock);
	CPPUNIT_TEST(testProcessDataArea);

	CPPUNIT_TEST(testFontRecord);
	CPPUNIT_TEST(testSpacingWidths);
	CPPUNIT_TEST(testPrinterWidths);
	CPPUNIT_TEST(testFlagsArray);
	CPPUNIT_TEST(testRasterInfo);
	CPPUNIT_TEST(testTxtBltArg);

	CPPUNIT_TEST_SUITE_END();


public:
	void testBasicType() {
		CPPUNIT_ASSERT_EQUAL((CARD32)1, (CARD32)sizeof(CARD8));
		CPPUNIT_ASSERT_EQUAL((CARD32)2, (CARD32)sizeof(CARD16));
		CPPUNIT_ASSERT_EQUAL((CARD32)4, (CARD32)sizeof(CARD32));

		CPPUNIT_ASSERT_EQUAL((CARD32)1, (CARD32)sizeof(INT8));
		CPPUNIT_ASSERT_EQUAL((CARD32)2, (CARD32)sizeof(INT16));
		CPPUNIT_ASSERT_EQUAL((CARD32)4, (CARD32)sizeof(INT32));
	}
	void testNibblePair() {
		CPPUNIT_ASSERT_EQUAL((CARD32)1, (CARD32)sizeof(NibblePair));

		NibblePair t = {0xab};
		CPPUNIT_ASSERT_EQUAL((CARD8)0xa, t.left);
		CPPUNIT_ASSERT_EQUAL((CARD8)0xb, t.right);
	}
	void testBytePair() {
		CPPUNIT_ASSERT_EQUAL((CARD32)1, SIZE(BytePair));
		CPPUNIT_ASSERT_EQUAL((CARD8)0xab, HighByte(0xabcd));
		CPPUNIT_ASSERT_EQUAL((CARD8)0xcd, LowByte(0xabcd));
	}
	void testBasicLogical() {
		CPPUNIT_ASSERT_EQUAL((CARD16)0xEDCB, Not((CARD16)0x1234));
		CPPUNIT_ASSERT(!Odd((CARD16)0x1234));
		CPPUNIT_ASSERT(Odd((CARD16)0x4321));
	}
	void testShift() {
		CPPUNIT_ASSERT_EQUAL((CARD16)0xbcd0, Shift((CARD16)0xabcd,  4));
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0abc, Shift((CARD16)0xabcd, -4));
		CPPUNIT_ASSERT_EQUAL((CARD16)0xabcd, Shift((CARD16)0xabcd,  0));
	}
	void testRotate() {
		CPPUNIT_ASSERT_EQUAL((CARD16)0xbcda, Rotate((CARD16)0xabcd,  4));
		CPPUNIT_ASSERT_EQUAL((CARD16)0xdabc, Rotate((CARD16)0xabcd, -4));
		CPPUNIT_ASSERT_EQUAL((CARD16)0xabcd, Rotate((CARD16)0xabcd,  0));
	}
	void testArithShift() {
		CPPUNIT_ASSERT_EQUAL((INT16)0xacd0, ArithShift((INT16)0xa2cd,  4));
		CPPUNIT_ASSERT_EQUAL((INT16)0xfabc, ArithShift((INT16)0xabcd, -4));

		CPPUNIT_ASSERT_EQUAL((INT16)0x3cd0, ArithShift((INT16)0x7bcd,  4));
		CPPUNIT_ASSERT_EQUAL((INT16)0x07bc, ArithShift((INT16)0x7bcd, -4));

		CPPUNIT_ASSERT_EQUAL((INT16)0xabcd, ArithShift((INT16)0xabcd,  0));
	}

	// Long: TYPE = MACHINE DEPENDENT RECORD[low(0), high(1): UNSPECIFIED];
	void testLong() {
		CPPUNIT_ASSERT_EQUAL((CARD32)2, SIZE(Long));
		CPPUNIT_ASSERT_EQUAL((CARD16)0xcafe, HighHalf((CARD32)0xcafebabe));
		CPPUNIT_ASSERT_EQUAL((CARD16)0xbabe, LowHalf((CARD32)0xcafebabe));

		Long t;
		t.low = 0x1234;
		t.high = 0xabcd;
		CPPUNIT_ASSERT_EQUAL((CARD32)0xabcd1234, t.u);
	}
	void testLongShift() {
		CPPUNIT_ASSERT_EQUAL((CARD32)0xafebabe0, LongShift((INT32)0xcafebabe,  4));
		CPPUNIT_ASSERT_EQUAL((CARD32)0x0cafebab, LongShift((INT32)0xcafebabe, -4));
		CPPUNIT_ASSERT_EQUAL((CARD32)0xcafebabe, LongShift((INT32)0xcafebabe,  0));
	}
	void testLongArithShift() {
		CPPUNIT_ASSERT_EQUAL((INT32)0xafebabe0, LongArithShift((INT32)0xcafebabe,  4));
		CPPUNIT_ASSERT_EQUAL((INT32)0xfcafebab, LongArithShift((INT32)0xcafebabe, -4));

		CPPUNIT_ASSERT_EQUAL((INT32)0x2febabe0, LongArithShift((INT32)0x4afebabe,  4));
		CPPUNIT_ASSERT_EQUAL((INT32)0x04afebab, LongArithShift((INT32)0x4afebabe, -4));

		CPPUNIT_ASSERT_EQUAL((INT32)0xcafebabe, LongArithShift((INT32)0xcafebabe,  0));
	}
	void testSignExtend() {
		CPPUNIT_ASSERT_EQUAL((INT16)0x12, SignExtend((CARD8)0x12));
		CPPUNIT_ASSERT_EQUAL((INT16)0xff92, SignExtend((CARD8)0x92));
	}

	//MapFlags: TYPE = MACHINE DEPENDENT RECORD (
	//  reserved (0:0..12) : UNSPEClFIED[0..17777B],
	//  protected (0:13..13) : BOOLEAN,
	//  dirty (0:14..14): BOOLEAN,
	//  referenced (0:15..15): BOOLEAN];
	void testMapFlags() {
		MapFlags mf;

		CPPUNIT_ASSERT_EQUAL((CARD32)1, SIZE(mf));

		mf.u = 0;
		mf.reserved = ~mf.reserved;
		CPPUNIT_ASSERT_EQUAL((CARD16)0xfff8, mf.u);

		mf.u = 0;
		mf.protect = 1;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0004, mf.u);

		mf.u = 0;
		mf.dirty = 1;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0002, mf.u);

		mf.u = 0;
		mf.referenced = 1;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0001, mf.u);

		mf.u = (CARD16)0x006;
		CPPUNIT_ASSERT(Vacant(mf)); // 0110 => 6
		mf.u = 0xfff6;
		CPPUNIT_ASSERT(Vacant(mf)); // 0110 => 6
		mf.u = 0xfffe;
		CPPUNIT_ASSERT(Vacant(mf)); // 1110 => E
	}

	//	struct CodeSegment {
	//		CARD16 available[4];
	//		CARD16 block[0];
	//	};
	void testCodeSegment() {
		CPPUNIT_ASSERT_EQUAL((CARD32)4, SIZE(CodeSegment));
		CPPUNIT_ASSERT_EQUAL((CARD32)0, OFFSET(CodeSegment, available[0]));
		CPPUNIT_ASSERT_EQUAL((CARD32)1, OFFSET(CodeSegment, available[1]));
		CPPUNIT_ASSERT_EQUAL((CARD32)2, OFFSET(CodeSegment, available[2]));
		CPPUNIT_ASSERT_EQUAL((CARD32)3, OFFSET(CodeSegment, available[3]));
		CPPUNIT_ASSERT_EQUAL((CARD32)4, OFFSET(CodeSegment, block));
	}

	// 3.2.2.2 Local and Global Frames
	// GlobalWord: TYPE = MACHINE DEPENDENT RECORD [
	//   gfi      (0: 0..13): GFTIndex,
	//   trapxfers(0:14..14): BOOLEAN,
	//   codelinks(0:15..15): BOOLEAN];
	void testGlobalWord() {
		CPPUNIT_ASSERT_EQUAL((CARD32)1, SIZE(GlobalWord));

		GlobalWord t;

		t.u = 0;
		t.gfi = ~t.gfi;
		CPPUNIT_ASSERT_EQUAL((CARD16)0xfffc, t.u);

		t.u = 0;
		t.trapxfers = ~t.trapxfers;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0002, t.u);

		t.u = 0;
		t.codelinks = ~t.codelinks;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0001, t.u);
	}

	//GlobalOverhead : TYPE  =  MACHINE DEPENDENT RECORD (
	//  available (0): UNSPECIFIED,
	//  word      (1): GlobalWord,
	//  global    (2): GlobaiVariables];
    void testGlobalOverhead() {
    	CPPUNIT_ASSERT_EQUAL((CARD32)2, SIZE(GlobalOverhead));
    	CPPUNIT_ASSERT_EQUAL((CARD32)4, SIZE(GlobalOverhead[2]));

    	CPPUNIT_ASSERT_EQUAL((CARD32)0, OFFSET(GlobalOverhead, available));
    	CPPUNIT_ASSERT_EQUAL((CARD32)1, OFFSET(GlobalOverhead, word));
    	CPPUNIT_ASSERT_EQUAL((CARD32)2, OFFSET(GlobalOverhead, global));

    	CPPUNIT_ASSERT_EQUAL((CARD32)0x12340010, GlobalBase(0x12340012));
    }


    // In PrincOps 4.0
    // GlobalOverhead : TYPE =  MACHINE DEPENDENT RECORD (
    //   available (0): UNSPECIFIED,
    //   word      (1): GlobalWord,
    //   codebase  (2): LONG_POINTER TO COdeSegment
    //   global    (4): GlobaiVariables];
    void testOldGlobalOverhead() {
    	CPPUNIT_ASSERT_EQUAL((CARD32)4, SIZE(OldGlobalOverhead));
    	CPPUNIT_ASSERT_EQUAL((CARD32)8, SIZE(OldGlobalOverhead[2]));

    	CPPUNIT_ASSERT_EQUAL((CARD32)0, OFFSET(OldGlobalOverhead, available));
    	CPPUNIT_ASSERT_EQUAL((CARD32)1, OFFSET(OldGlobalOverhead, word));
    	CPPUNIT_ASSERT_EQUAL((CARD32)2, OFFSET(OldGlobalOverhead, codebase));
    	CPPUNIT_ASSERT_EQUAL((CARD32)4, OFFSET(OldGlobalOverhead, global));

    	CPPUNIT_ASSERT_EQUAL((CARD32)0x12340010, OldBlobaseBase(0x12340014));
    }


    // GFTItem: TYPE = MACHINE DEPENDENT RECORD[
    //   globalFrame(0): GlobalFrameHandle,
    //   codebase(2): LONG POINTER TO CodeSegment];
    void testGFTItem() {
    	CPPUNIT_ASSERT_EQUAL(4U, SIZE(GFTItem));
    	CPPUNIT_ASSERT_EQUAL(8U, SIZE(GFTItem[2]));

    	CPPUNIT_ASSERT_EQUAL(0U, OFFSET(GFTItem, globalFrame));
       	CPPUNIT_ASSERT_EQUAL(2U, OFFSET(GFTItem, codebase));
    }


   // GlobalFrameTable: TYPE = LONG BASE POINTER TO ARRAY GFTIndex OF GFTItem;
   // GFTIndex: TYPE = [0..16384);
   // #define GFT_OFFSET(n,m) (GFT + OFFSET4(GlobalFrameTable, item, (n / SIZE(GFTItem)), m))

   void testGlobalFrameTable() {
	   	CPPUNIT_ASSERT_EQUAL(65536U, SIZE(GlobalFrameTable));

	   	CARD16 index = 8;
	   	CPPUNIT_ASSERT_EQUAL((GFT + index + 0), (CARD32)(GFT_OFFSET(index, globalFrame)));
	   	CPPUNIT_ASSERT_EQUAL((GFT + index + 2), ((CARD32)GFT_OFFSET(index, codebase)));
   }


	//LocalWord: TYPE = MACHINE DEPENDENT RECORD[
	//  available(0:0..7): BYTE,
	//  fsi(0:8..15): FSIndex];
	void testLocalWord() {
		CPPUNIT_ASSERT_EQUAL((CARD32)1, SIZE(LocalWord));

		LocalWord t;

		t.u = 0;
		t.available = ~t.available;
		CPPUNIT_ASSERT_EQUAL((CARD16)0xff00, t.u);

		t.u = 0;
		t.fsi = ~t.fsi;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x00ff, t.u);
	}

	//LocalOverhead : TYPE = MACHINE DEPENDENT RECORD []
	//  word (0):  LocalWord.
	//  returnlink(1) : ShortControlLink,
	//  globallink(2) : GlobalFrameHandle,
	//  pc(3): CARDINAL,
	//  local(4):  LocaiVariables];
	// #define LO_OFFSET(p,m) (LocalBase(p) + OFFSET(LocalOverhead, m))
    void testLocalOverhead() {
    	CPPUNIT_ASSERT_EQUAL((CARD32)4, SIZE(LocalOverhead));
    	CPPUNIT_ASSERT_EQUAL((CARD32)8, SIZE(LocalOverhead[2]));

    	CPPUNIT_ASSERT_EQUAL((CARD32)0, OFFSET(LocalOverhead, word));
    	CPPUNIT_ASSERT_EQUAL((CARD32)1, OFFSET(LocalOverhead, returnlink));
    	CPPUNIT_ASSERT_EQUAL((CARD32)2, OFFSET(LocalOverhead, globallink));
    	CPPUNIT_ASSERT_EQUAL((CARD32)3, OFFSET(LocalOverhead, pc));
    	CPPUNIT_ASSERT_EQUAL((CARD32)4, OFFSET(LocalOverhead, local));

    	CPPUNIT_ASSERT_EQUAL((CARD16)96, LocalBase(100));

    	CPPUNIT_ASSERT_EQUAL((CARD16)98, LO_OFFSET(100U, globallink));
    }

	//// 7.5 Field Instruction
	//FieldSpec: TYPE = MACHINE DEPENDENT RECORD [
	// pos (0:0..3):  NIBBLE,
	// size (0:4..7): NIBBLE];
	void testFieldSpec() {
		CPPUNIT_ASSERT_EQUAL((CARD32)1, (CARD32)sizeof(FieldSpec));

		FieldSpec t;

		t.u = 0;
		t.pos = ~t.pos;
		CPPUNIT_ASSERT_EQUAL((CARD8)0xf0, t.u);

		t.u = 0;
		t.size = ~t.size;
		CPPUNIT_ASSERT_EQUAL((CARD8)0x0f, t.u);
	}

	//FieldDesc: TYPE = MACHINE DEPENDENT RECORD [
	//  offset(0:0..7) : BYTE,
	//  field(0:8..15) : FieldSpec];
	void testFieldDesc() {
		CPPUNIT_ASSERT_EQUAL((CARD32)1, SIZE(FieldDesc));

		FieldDesc t;

		t.u = 0;
		t.offset = ~t.offset;
		CPPUNIT_ASSERT_EQUAL((CARD16)0xff00, t.u);

		t.u = 0;
		t.field = ~t.field;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x00ff, t.u);
	}

	//BitAddress: TYPE = MACHINE DEPENDENT RECORD[
	//  word(0): LONG POINTER,
	//  reserved(2:0..11): [0..777B] = 0,
	//  bit(2:12:15): [0..WordSize)];
	void testBitAddress() {
		CPPUNIT_ASSERT_EQUAL((CARD32)3, SIZE(BitAddress)); // need to be 3
		CPPUNIT_ASSERT_EQUAL((CARD32)6, SIZE(BitAddress[2]));
		CPPUNIT_ASSERT_EQUAL((CARD32)0, OFFSET(BitAddress, word));
		CPPUNIT_ASSERT_EQUAL((CARD32)2, OFFSET(BitAddress, u));

		BitAddress t;

		t.word = 0;
		t.u = 0;

		t.reserved = ~t.reserved;
		CPPUNIT_ASSERT_EQUAL((CARD16)0xfff0, t.u);

		t.u = 0;
		t.bit = ~t.bit;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x000f, t.u);
	}


	//BitBltFlags: TYPE = MACHINE DEPENDENT RECORD[
	//  direction(0:0..0): Direction,
	//  disjoint(0:1..1): BOOLEAN,
	//  disjointItems(0:2..2) BOOLEAN,
	//  gray(0:3..3) BOOLEAN,
	//  srcFunc(0:4..4) SrcFunc,
	//  dstFunc(0:5..6) DstFunc,
	//  reserved(0:7..15]: [0..777B] = 0];
	void testBitBltFlags() {
		CPPUNIT_ASSERT_EQUAL((CARD32)1, SIZE(BitBltFlags));

		BitBltFlags t;

		t.u = 0;
		t.direction = ~t.direction;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x8000, t.u);

		t.u = 0;
		t.disjoint = ~t.disjoint;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x4000, t.u);

		t.u = 0;
		t.disjointItems = ~t.disjointItems;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x2000, t.u);

		t.u = 0;
		t.gray = ~t.gray;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x1000, t.u);

		t.u = 0;
		t.srcFunc = ~t.srcFunc;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0800, t.u);

		t.u = 0;
		t.dstFunc = ~t.dstFunc;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0600, t.u);

		t.u = 0;
		t.reserved = ~t.reserved;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x01ff, t.u);
	}


	//BitBltArg: TYPE = MACHINE DPENEDENT RECORD[
	//  dst(0): BitAddress,
	//  dstBpl(3): INTEGER,
	//  src(4) BitAddress,
	//  srcBpl(7): INTEGER,
	//  width(8): CARDINAL,
	//  height(9): CARDINAL,
	//  flags(10): BitBltFlags,
	//  reserved(11): UNSPEC = 0];
	void testBitBltArg() {
		CPPUNIT_ASSERT_EQUAL((CARD32)12, SIZE(BitBltArg));
		CPPUNIT_ASSERT_EQUAL((CARD32)24, SIZE(BitBltArg[2]));
		
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(BitBltArg, dst));
		CPPUNIT_ASSERT_EQUAL((CARD32) 3, OFFSET(BitBltArg, dstBpl));
		CPPUNIT_ASSERT_EQUAL((CARD32) 4, OFFSET(BitBltArg, src));
		CPPUNIT_ASSERT_EQUAL((CARD32) 7, OFFSET(BitBltArg, srcBpl));
		CPPUNIT_ASSERT_EQUAL((CARD32) 8, OFFSET(BitBltArg, width));
		CPPUNIT_ASSERT_EQUAL((CARD32) 9, OFFSET(BitBltArg, height));
		CPPUNIT_ASSERT_EQUAL((CARD32)10, OFFSET(BitBltArg, flags));
		CPPUNIT_ASSERT_EQUAL((CARD32)11, OFFSET(BitBltArg, reserved));
	}


	//GrayParam: TYPE = MACHINE DEPENDENT RECORD[
	//  reserved(0:0..3): NIBBLE = 0,
	//  yOffset(0:4..7): NIBBLE,
	//  widthMinusOne(0:8..11): NIBBLE,
	//  heightMinusOne(0:12..15): NIBBLE];
	void testGrayParm() {
		CPPUNIT_ASSERT_EQUAL((CARD32)1, SIZE(GrayParm));

		GrayParm t;

		t.u = 0;
		t.reserved = ~t.reserved;
		CPPUNIT_ASSERT_EQUAL((CARD16)0xf000, t.u);

		t.u = 0;
		t.yOffset = ~t.yOffset;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0f00, t.u);

		t.u = 0;
		t.widthMinusOne = ~t.widthMinusOne;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x00f0, t.u);

		t.u = 0;
		t.heightMinusOne = ~t.heightMinusOne;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x000f, t.u);
	}


	// 9.1 Control Links

	//TaggedControlLink: TYPE =  MACHINE DEPENDENT RECORD [
	//  data (0:  0..13): [0 .. 377778];
	//  tag  (0: 14..15): [0 .. 3],
	//  fill (1) :  UNSPECIFIED);
	void testTaggedControlLink() {
		CPPUNIT_ASSERT_EQUAL((CARD32)2, SIZE(TaggedControlLink));

		TaggedControlLink t;

		t.u = 0;
		t.data = ~t.data;
		CPPUNIT_ASSERT_EQUAL((CARD32)0x0000fffc, t.u);

		t.u = 0;
		t.tag = ~t.tag;
		CPPUNIT_ASSERT_EQUAL((CARD32)0x00000003, t.u);

		t.u = 0;
		t.fill = ~t.fill;
		CPPUNIT_ASSERT_EQUAL((CARD32)0xffff0000, t.u);

		t.u = 0;
		t.tag = 0;
		CPPUNIT_ASSERT_EQUAL(LT_frame, ControlLinkType(t.u));
		t.tag = 1;
		CPPUNIT_ASSERT_EQUAL(LT_oldProcedure, ControlLinkType(t.u));
		t.tag = 2;
		CPPUNIT_ASSERT_EQUAL(LT_indirect, ControlLinkType(t.u));
		t.tag = 3;
		CPPUNIT_ASSERT_EQUAL(LT_newProcedure, ControlLinkType(t.u));
	}

	// 9.1.1 Frame Control Links
	void testMakeFrameLink() {
		TaggedControlLink t;
		t.u = 0x12345678;

		FrameLink fl = MakeFrameLink(t.u);
		CPPUNIT_ASSERT_EQUAL((CARD16)0x5678, fl);
	}

	// 9.1.2 Indirect Control Links
	void testMakeIndirectLink() {
		TaggedControlLink t;
		t.u = 0x12345676;

		IndirectLink il = MakeIndirectLink(t.u);
		CPPUNIT_ASSERT_EQUAL((CARD16)0x5676, il);
	}

	// 9.1.3 Procedure Descriptor
	//ProcDesc: TYPE =  MACHINE DEPENDENT RECORD [
	//  taggedGF(0) :UNSPECIFIED,
	//  pc(1}: CARDINAL];
	void testProcDesc() {
		CPPUNIT_ASSERT_EQUAL((CARD32)2, SIZE(ProcDesc));

		ProcDesc t;

		t.u = 0;
		t.taggedGF = ~t.taggedGF;
		CPPUNIT_ASSERT_EQUAL((CARD32)0x0000ffff, t.u);

		t.u = 0;
		t.pc = ~t.pc;
		CPPUNIT_ASSERT_EQUAL((CARD32)0xffff0000, t.u);
	}
	void testMakeProcDesc() {
		TaggedControlLink t;
		t.u = 0x12345679;

		ProcDesc pd = {MakeProcDesc(t.u)};
		CPPUNIT_ASSERT_EQUAL(t.u, pd.u);
	}


	// 9.1.4 new Procedure Descriptor
	// NewProcDesc: TYPE = MACHINE DEPENDENT RECORD[
	//   taggedGFI(0): UNSPEC,
	//   pc(1): CARDINAL];
	void testNewProcDesc() {
		CPPUNIT_ASSERT_EQUAL((CARD32)2, SIZE(NewProcDesc));

		NewProcDesc t;

		t.u = 0;
		t.taggedGFI = ~t.taggedGFI;
		CPPUNIT_ASSERT_EQUAL((CARD32)0x0000ffff, t.u);

		t.u = 0;
		t.pc = ~t.pc;
		CPPUNIT_ASSERT_EQUAL((CARD32)0xffff0000, t.u);
	}
	void testMakeNewProcDesc() {
		TaggedControlLink t;
		t.u = 0x1237567b;

		NewProcDesc pd = {MakeNewProcDesc(t.u)};
		CPPUNIT_ASSERT_EQUAL(t.u, pd.u);
	}

	// 9.2.1 Frame Allocation Vector
	//AVItem: TYPE = MACHINE DEPENDENT RECORD [
	//  data (0: 0 .. 13): [0 .. 377778],
	//  tag (0: 14 .. 15) :  MACHINE DEPENDENT {frame, empty, indirect, unused}];
	void testAVItem() {
		CPPUNIT_ASSERT_EQUAL((CARD32)1, SIZE(AVItem));

		AVItem t;

		t.u = 0;
		t.data = ~t.data;
		CPPUNIT_ASSERT_EQUAL((CARD16)0xfffc, t.u);

		t.u = 0;
		t.tag = ~t.tag;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0003, t.u);

		CPPUNIT_ASSERT_EQUAL(256U, SIZE(AllocationVector));
	}

	// Port: TYPE = MACHINE DEPENDENT RECORD[
	//   inport(0):  FrameLink,
	//   unused(1):  UNSPECIFIED,
	//   outport(2): ControlLink];
	void testPort() {
		CPPUNIT_ASSERT_EQUAL((CARD32)4, SIZE(Port));
		CPPUNIT_ASSERT_EQUAL((CARD32)0, OFFSET(Port, inport));
		CPPUNIT_ASSERT_EQUAL((CARD32)1, OFFSET(Port, unused));
		CPPUNIT_ASSERT_EQUAL((CARD32)2, OFFSET(Port, outport));
	}

	void testSystemData() {
		CPPUNIT_ASSERT_EQUAL((CARD32)512, SIZE(SystemData));
	}

	void testEscTrapTable() {
		CPPUNIT_ASSERT_EQUAL((CARD32)512, SIZE(EscTrapTable));
	}


	//// 9.5.3 Trap Handler
	//StateWord: TYPE = MACHINE DEPENDENT RECORD[
	//  breakByte(0:0..7), stkptr(0:8..15): BYTE];
	void testStateWord() {
		CPPUNIT_ASSERT_EQUAL((CARD32)1, SIZE(StateWord));

		StateWord t;

		t.u = 0;
		t.breakByte = ~t.breakByte;
		CPPUNIT_ASSERT_EQUAL((CARD16)0xff00, t.u);

		t.u = 0;
		t.stkptr = ~t.stkptr;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x00ff, t.u);
	}

	// StateVector: TYPE = MACH I N E D E P E N D ENT R ECORD [
	//   stack  (0) : ARRAY [0.. StackDepth) OF UNSPECIFIED,
	//   word  (14) : StateWord,
	//   frame (15) : LocalFrameHandle,
	//   data  (16) : BLOCK];
	void testStateVector() {
		CPPUNIT_ASSERT_EQUAL((CARD32)cSV, SIZE(StateVector));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(StateVector, stack));
		CPPUNIT_ASSERT_EQUAL((CARD32)14, OFFSET(StateVector, word));
		CPPUNIT_ASSERT_EQUAL((CARD32)15, OFFSET(StateVector, frame));
		CPPUNIT_ASSERT_EQUAL((CARD32)16, OFFSET(StateVector, data));
	}

	// TransferDescriptor: TYPE = MACHINE DEPENDENT RECORD[
	//   src(0): ShortControlLink,
	//   reserved(1): UNSPECIFIED = 0,
	//   dst(2): CoontrolLink(2)];
	void testTransferDescriptor() {
		CPPUNIT_ASSERT_EQUAL((CARD32)4, SIZE(TransferDescriptor));
		CPPUNIT_ASSERT_EQUAL((CARD32)0, OFFSET(TransferDescriptor, src));
		CPPUNIT_ASSERT_EQUAL((CARD32)1, OFFSET(TransferDescriptor, reserved));
		CPPUNIT_ASSERT_EQUAL((CARD32)2, OFFSET(TransferDescriptor, dst));
	}

	//PsbLink: TYPE = MACHINE DEPENDENT RECORD[
	//  priority(0:0..2): Priority,
	//  next(0:3..12): PsbIndex,
	//  failed(0:13..13): BOOLEAN,
	//  permanent(0:14..14): BOOLEAN,
	//  preempted(0:15..15): BOOLEAN];
	void testPsbLink() {
		CPPUNIT_ASSERT_EQUAL((CARD32)1, SIZE(PsbLink));

		PsbLink t;

		t.u = 0;
		t.priority = ~t.priority;
		CPPUNIT_ASSERT_EQUAL((CARD16)0xe000, t.u);

		t.u = 0;
		t.next = ~t.next;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x1ff8, t.u);

		t.u = 0;
		t.failed = ~t.failed;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0004, t.u);

		t.u = 0;
		t.permanent = ~t.permanent;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0002, t.u);

		t.u = 0;
		t.preempted = ~t.preempted;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0001, t.u);
	}

	//PsbFlags: TYPE = MACHINE DEPENDENT RECORD[
	//  available(0:0..2): [0..7],
	//  cleanup(0:3..12): PsbIndex,
	//  reserved(0:13..13): BIT = 0,
	//  waiting(0:14..14): BOOLEAN,
	//  abort(0:15..15): BOOLEAN];
	void testPsbFlags() {
		CPPUNIT_ASSERT_EQUAL((CARD32)1, SIZE(PsbFlags));

		PsbFlags t;

		t.u = 0;
		t.available = ~t.available;
		CPPUNIT_ASSERT_EQUAL((CARD16)0xe000, t.u);

		t.u = 0;
		t.cleanup = ~t.cleanup;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x1ff8, t.u);

		t.u = 0;
		t.reserved = ~t.reserved;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0004, t.u);

		t.u = 0;
		t.waiting = ~t.waiting;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0002, t.u);

		t.u = 0;
		t.abort = ~t.abort;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0001, t.u);
	}

	//// 10.1.3 Monitor Locks
	//Monitor: TYPE = MACHINE DEPENDENT RECORD[
	//  reserved(0:0..2): [0..7] = 0,
	//  tail(0:3..12): PsbIndex,
	//  available(0:13..14): [0..3],
	//  locked(0:15..15): BOOLEAN];
	void testMonitor() {
		CPPUNIT_ASSERT_EQUAL((CARD32)1, SIZE(Monitor));

		Monitor t;

		t.u = 0;
		t.reserved = ~t.reserved;
		CPPUNIT_ASSERT_EQUAL((CARD16)0xe000, t.u);

		t.u = 0;
		t.tail = ~t.tail;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x1ff8, t.u);

		t.u = 0;
		t.available = ~t.available;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0006, t.u);

		t.u = 0;
		t.locked = ~t.locked;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0001, t.u);
	}

	//// 10.1.4 Condition Variables
	//Condition: TYPE = MACHIND DEPENDENT RECORD[
	//  reserved(0:0..2): [0..7] = 0,
	//  tail(0:3..12): PsbIndex,
	//  available(0:13..13): BIT,
	//  abortable(0:14..14): BOOLEAN,
	//  wakeup(0:15..15): BOOLEAN];
	void testCondition() {
		CPPUNIT_ASSERT_EQUAL((CARD32)1, SIZE(Condition));

		Condition t;

		t.u = 0;
		t.reserved = ~t.reserved;
		CPPUNIT_ASSERT_EQUAL((CARD16)0xe000, t.u);

		t.u = 0;
		t.tail = ~t.tail;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x1ff8, t.u);

		t.u = 0;
		t.available = ~t.available;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0004, t.u);

		t.u = 0;
		t.abortable = ~t.abortable;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0002, t.u);

		t.u = 0;
		t.wakeup = ~t.wakeup;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0001, t.u);
	}

	//// 10.1.5 Process Queue
	//Queue: TYPE = MACHINE DEPENDENT RECORD[
	//  reserved1(0:0..2): [0..7] = 0,
	//  tail(0:3..12): PsbIndex,
	//  reserved2(0:13..15): [0..7]];
	void testQueue() {
		CPPUNIT_ASSERT_EQUAL((CARD32)1, SIZE(Queue));

		Queue t;

		t.u = 0;
		t.reserved1 = ~t.reserved1;
		CPPUNIT_ASSERT_EQUAL((CARD16)0xe000, t.u);

		t.u = 0;
		t.tail = ~t.tail;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x1ff8, t.u);

		t.u = 0;
		t.reserved2 = ~t.reserved2;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0007, t.u);
	}

	// StateAllocationTable: TYPE = ARRAY Priority OF POINTER TO StateVector
	void testStateAllocationTable() {
		CPPUNIT_ASSERT_EQUAL((CARD16)(8), (CARD16)SIZE(StateAllocationTable));
	}

	// FaultVector: TYPE = ARRAY FaultIndex OF FaultQueue;
	// FaultIndex: TYPE = [0..8);
	// FaultQueue: TYPE = MACHINE DEPENDENT RECORD[
	//   queue(0): Queue,
	//   condition(1): Condition];
	void testFaultVector() {
		CPPUNIT_ASSERT_EQUAL((CARD32)2, SIZE(FaultQueue));
		CPPUNIT_ASSERT_EQUAL((CARD32)0, OFFSET(FaultQueue, queue));
		CPPUNIT_ASSERT_EQUAL((CARD32)1, OFFSET(FaultQueue, condition));

		CPPUNIT_ASSERT_EQUAL((CARD32)16, SIZE(FaultVector));
	}

	// InterruptVector: TYPE = ARRAY InterruptLevel OF InterruptItem;
	// InterruptLvel: TYPE = [0..WordSize);
	// InterruptItem: TYPE = MACHINE DPENEDENT RECORD[
	//   condition(0): Condition,
	//   available(1): UNSPECIFIED];
	void testInterruptVector() {
		CPPUNIT_ASSERT_EQUAL((CARD32)2, SIZE(InterruptItem));
		CPPUNIT_ASSERT_EQUAL((CARD32)0, OFFSET(InterruptItem, condition));
		CPPUNIT_ASSERT_EQUAL((CARD32)1, OFFSET(InterruptItem, available));

		CPPUNIT_ASSERT_EQUAL((CARD32)32, SIZE(InterruptVector));
	}

	// ProcessStateBlock: TYPE = MACHINE DPENDENT RECORD[
	//   link(0): PsbLink,
	//   flags(1): PsbFlags,
	//   context(2): POINTER,
	//   timeout(3): Ticks,
	//   mds(4): CARDINAL,
	//   available(5): UNSPECIFIED,
	//   stickty(6): LONG UNSPECIFIED];
	void testProcessStateBlock() {
		CPPUNIT_ASSERT_EQUAL((CARD32)8, SIZE(ProcessStateBlock));
		CPPUNIT_ASSERT_EQUAL((CARD32)0, OFFSET(ProcessStateBlock, link));
		CPPUNIT_ASSERT_EQUAL((CARD32)1, OFFSET(ProcessStateBlock, flags));
		CPPUNIT_ASSERT_EQUAL((CARD32)2, OFFSET(ProcessStateBlock, context));
		CPPUNIT_ASSERT_EQUAL((CARD32)3, OFFSET(ProcessStateBlock, timeout));
		CPPUNIT_ASSERT_EQUAL((CARD32)4, OFFSET(ProcessStateBlock, mds));
		CPPUNIT_ASSERT_EQUAL((CARD32)5, OFFSET(ProcessStateBlock, available));
		CPPUNIT_ASSERT_EQUAL((CARD32)6, OFFSET(ProcessStateBlock, sticky));
	}

	// ProcessDataArea: TYPE = MACHINE DEPENDENT RECORD[
	//   SELECT OVERLAID * FROM
	//     header => [
	//       ready: Queue,
	//       count: CARDINAL,
	//       unused: UNSPECIFIED,
	//       available: ARRAY [0..5) OF UNSPECIFIED,
	//       state: StateAllocationTable,
	//       interrupt: InterruptVector,
	//       fault: FaultVector],
	//     blocks => [
	//       block: ARRAY [0..0) OF ProcessStateBlock],
	//     ENDCASE];
	void testProcessDataArea() {
		CPPUNIT_ASSERT_EQUAL((CARD32)64, SIZE(ProcessDataArea)); // (8 + 8 + 32 + 16)
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(ProcessDataArea, ready));
		CPPUNIT_ASSERT_EQUAL((CARD32) 1, OFFSET(ProcessDataArea, count));
		CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(ProcessDataArea, unused));
		CPPUNIT_ASSERT_EQUAL((CARD32) 3, OFFSET(ProcessDataArea, available));
		CPPUNIT_ASSERT_EQUAL((CARD32) 8, OFFSET(ProcessDataArea, state));
		CPPUNIT_ASSERT_EQUAL((CARD32)16, OFFSET(ProcessDataArea, interrupt));
		CPPUNIT_ASSERT_EQUAL((CARD32)48, OFFSET(ProcessDataArea, fault));

		CPPUNIT_ASSERT_EQUAL(8U, StartPsb);
	}


	// FontRecord: TYPE = MACHINE DEPENDENT RECORD [
	//   rasters(0): FontRasters,
	//   spacingWidths(2): SpacingWidths,
	//   printerWidths(4): PrinterWidths,
	//   flags(6): FlagsArray,
	//   rasterInfos(8): RasterInfos,
	//   height(10): CARDINAL];
	void testFontRecord() {
		CPPUNIT_ASSERT_EQUAL((CARD32)11, SIZE(FontRecord));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(FontRecord, rasters));
		CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(FontRecord, spacingWidths));
		CPPUNIT_ASSERT_EQUAL((CARD32) 4, OFFSET(FontRecord, printerWidths));
		CPPUNIT_ASSERT_EQUAL((CARD32) 6, OFFSET(FontRecord, flags));
		CPPUNIT_ASSERT_EQUAL((CARD32) 8, OFFSET(FontRecord, rasterInfos));
		CPPUNIT_ASSERT_EQUAL((CARD32)10, OFFSET(FontRecord, height));
	}

	// Byte: TYPE = [0..255)
	// SpacingWidths: TYPE = LONG POINTER TO PACKED ARRAY Byte OF SpacingWidth;
	// SpacingWidth: TYPE = Byte;
	void testSpacingWidths() {
		CPPUNIT_ASSERT_EQUAL((CARD32)128, SIZE(SpacingWidthsRecord));
	}

	// PrinterWidths: TYPE = LONG POINTER TO ARRAY Byte OF PrinterWidth;
	// PrinterWidth: TYPE = CARDINAL;
	void testPrinterWidths() {
		CPPUNIT_ASSERT_EQUAL((CARD32)256, SIZE(PrinterWidthsRecord));
	}

	// FlagsArray: TYPE = LONG POINTER TO PACKED ARRAY Byte OF Flags;
	// Flags: TYPE = MACHINE DEPENDENT RECORD [
	//   pad(0:0..0): BOOLEAN,
	//   stop(0:1..1): BOOLEAN];
	void testFlagsArray() {
		CPPUNIT_ASSERT_EQUAL((CARD32)(256 * 2 / 16), SIZE(FlagsArrayRecord));
	}

	// RasterInfos: TYPE = LONG POINTER TO ARRAY Byte OF RasterInfo;
	// RasterInfo: TYPE = MACHINE DEPENDENT RECORD [
	//   leftKern: BOOLEAN,
	//   rightKern: BOOLEAN,
	//   offset: RasterOffset];
	void testRasterInfo() {
		CPPUNIT_ASSERT_EQUAL((CARD32) 1, SIZE(RasterInfo));

		RasterInfo t;

		t.u = 0;
		t.leftKern = ~t.leftKern;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x8000, t.u);

		t.u = 0;
		t.rightKern = ~t.rightKern;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x4000, t.u);

		t.u = 0;
		t.offset = ~t.offset;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x3fff, t.u);
	}

	// TxtBltArg: TYPE = MACHINE DEPENDENT RECORD [
	//   reserved(0:0..13): [0.37777B] = 0,
	//   function(0:14..15): Function,
	//   last(1): CARDINAL,
	//   text(2): LONG POINTER TO ARRAY CARDIANL Of BytePair,
	//   font(4): FontHandle,
	//   dst(6): LONG POINTER,
	//   dstBpl(8): CARDINAL,
	//   margin(9): CARDINAL,
	//   space(10): INTEGER,
	//   coord(11): LONG POINTER TO ARRAY CARDINAL OF CARDINAL];
	void testTxtBltArg() {
		CPPUNIT_ASSERT_EQUAL((CARD32)13, SIZE(TxtBltArg));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(TxtBltArg, u0));
		CPPUNIT_ASSERT_EQUAL((CARD32) 1, OFFSET(TxtBltArg, last));
		CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(TxtBltArg, text));
		CPPUNIT_ASSERT_EQUAL((CARD32) 4, OFFSET(TxtBltArg, font));
		CPPUNIT_ASSERT_EQUAL((CARD32) 6, OFFSET(TxtBltArg, dst));
		CPPUNIT_ASSERT_EQUAL((CARD32) 8, OFFSET(TxtBltArg, dstBpl));
		CPPUNIT_ASSERT_EQUAL((CARD32) 9, OFFSET(TxtBltArg, margin));
		CPPUNIT_ASSERT_EQUAL((CARD32)10, OFFSET(TxtBltArg, space));
		CPPUNIT_ASSERT_EQUAL((CARD32)11, OFFSET(TxtBltArg, coord));

		TxtBltArg t;

		t.u0 = 0;
		t.reserved = ~t.reserved;
		CPPUNIT_ASSERT_EQUAL((CARD16)0xfffc, t.u0);

		t.u0 = 0;
		t.function = ~t.function;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0003, t.u0);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(testType);
