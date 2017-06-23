/*
Copyright (c) 2017, Yasuhiro Hasegawa
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
static log4cpp::Category& logger = Logger::getLogger("testESV");

#include "testBase.h"

#include "../mesa/Type.h"
#include "../mesa/ExternalStateVector.h"


class testESV : public testBase {

	CPPUNIT_TEST_SUITE(testESV);

	// LoadStateFormat
	CPPUNIT_TEST(testModuleInfo);
	CPPUNIT_TEST(testBcdInfo);
	CPPUNIT_TEST(testObject);
	CPPUNIT_TEST(testSwapInfo);
	CPPUNIT_TEST(testExternalStateVector);

	CPPUNIT_TEST_SUITE_END();


public:
	void testModuleInfo() {
		//ModuleInfo: TYPE = MACHINE DEPENDENT RECORD [
		//  resolved(0:0..0): BOOLEAN,
		//  cgfi(0:1..15): NATURAL,
		//  index(1): Index,
		//  globalFrame(2): SELECT OVERLAID * FROM
		//    oldWorld => [gf(2): PrincOps.GlobalFrameHandle],
		//    newWorld => [gfi(2): PrincOpsExtras2.GFTHandle],
		//    ENDCASE];
		CPPUNIT_ASSERT_EQUAL((CARD32) 3, SIZE(LoadStateFormat::ModuleInfo));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(LoadStateFormat::ModuleInfo, u0));
		CPPUNIT_ASSERT_EQUAL((CARD32) 1, OFFSET(LoadStateFormat::ModuleInfo, index));
		CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(LoadStateFormat::ModuleInfo, globalFrame));

		LoadStateFormat::ModuleInfo t;

		//  resolved(0:0..0): BOOLEAN,
		t.u0 = 0;
		t.resolved = ~t.resolved;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x8000, t.u0);

		//  cgfi(0:1..15): NATURAL,
		t.u0 = 0;
		t.cgfi = ~t.cgfi;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x7FFF, t.u0);
	}
	void testBcdInfo() {
		//BcdInfo: TYPE = MACHINE DEPENDENT RECORD [
		//  exports(0:0..0), typeExported(0:1..1): BOOLEAN,
		//  pages(0:2..15): [1..256],
		//  base(1): LONG BASE POINTER TO BcdDefs.BCD,
		//  id(3): ID];
		CPPUNIT_ASSERT_EQUAL((CARD32) 4, SIZE(LoadStateFormat::BcdInfo));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(LoadStateFormat::BcdInfo, u0));
		CPPUNIT_ASSERT_EQUAL((CARD32) 1, OFFSET(LoadStateFormat::BcdInfo, base));
		CPPUNIT_ASSERT_EQUAL((CARD32) 3, OFFSET(LoadStateFormat::BcdInfo, id));

		LoadStateFormat::BcdInfo t;

		//  exports(0:0..0),
		t.u0 = 0;
		t.exports = ~t.exports;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x8000, t.u0);

		//  typeExported(0:1..1),
		t.u0 = 0;
		t.typeExported = ~t.typeExported;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x4000, t.u0);

		//  pages(0:2..15): [1..256]
		t.u0 = 0;
		t.pages = ~t.pages;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x3FFF, t.u0);
	}
	void testObject() {
		//Object: TYPE = MACHINE DEPENDENT RECORD [
		//  versionident: CARDINAL,
		//  nModules: CARDINAL,
		//  maxModules: CARDINAL,
		//  nBcds: CARDINAL,
		//  maxBcds: CARDINAL,
		//  nextID: ID,
		//  moduleInfo: ModuleInfoIndex,
		//  bcdInfo: BcdInfoIndex];
		CPPUNIT_ASSERT_EQUAL((CARD32) 8, SIZE(LoadStateFormat::Object));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(LoadStateFormat::Object, versionident));
		CPPUNIT_ASSERT_EQUAL((CARD32) 1, OFFSET(LoadStateFormat::Object, nModules));
		CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(LoadStateFormat::Object, maxModules));
		CPPUNIT_ASSERT_EQUAL((CARD32) 3, OFFSET(LoadStateFormat::Object, nBcds));
		CPPUNIT_ASSERT_EQUAL((CARD32) 4, OFFSET(LoadStateFormat::Object, maxBcds));
		CPPUNIT_ASSERT_EQUAL((CARD32) 5, OFFSET(LoadStateFormat::Object, nextID));
		CPPUNIT_ASSERT_EQUAL((CARD32) 6, OFFSET(LoadStateFormat::Object, moduleInfo));
		CPPUNIT_ASSERT_EQUAL((CARD32) 7, OFFSET(LoadStateFormat::Object, bcdInfo));
	}
	void testSwapInfo() {
		//SwapInfo: TYPE = MACHINE DEPENDENT RECORD [
		//  -- This is the actual usage of PSB.PDA.available.
		//  externalStateVector(0): LONG POINTER TO ExternalStateVector,
		//  availableC(2), availableD(3), availableE(4): WORD];
		CPPUNIT_ASSERT_EQUAL((CARD32) 5, SIZE(CPSwapDefs::SwapInfo));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(CPSwapDefs::SwapInfo, externalStateVector));
		CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(CPSwapDefs::SwapInfo, availableC));
		CPPUNIT_ASSERT_EQUAL((CARD32) 3, OFFSET(CPSwapDefs::SwapInfo, availableD));
		CPPUNIT_ASSERT_EQUAL((CARD32) 4, OFFSET(CPSwapDefs::SwapInfo, availableE));
	}
	void testExternalStateVector() {
		//ExternalStateVector: TYPE = MACHINE DEPENDENT RECORD [
		//	-- Items describing the boot session in general:
		//	version(0): CARDINAL,  -- version of CPSwapDefs. MUST BE FIRST.
		//	bootSession(1): LONG CARDINAL,  -- random number identifying boot session.
		//	    loadState(3): LONG POINTER TO LoadStateFormat.Object,
		//	    mapLog(5): LONG POINTER TO VMMapLog.Descriptor,
		//	    patchTable(7): LONG POINTER TO PatchTable,
		//	    breakBlocks(9): LONG POINTER TO BBArray,
		//	    breakpointHandlers(11): LONG POINTER TO BreakpointHandlerItem,
		//	    mds(13): CARDINAL,  -- MDS of *all* processes.
		//	    faultsBeingProcessed(14): ARRAY FaultType OF FaultQueuePtr,
		//	    systemVolumeID(20): Volume.ID,  -- (nullID if none or not known yet)
		//	    virtualMemoryCount(25): Environment.PageCount,
		//	    patchTableInUse(27:0..12): BOOLEAN,  -- (in use by debuggee)
		//	    breakBlocksInUse(27:13..13): BOOLEAN,  -- (in use by debuggee)
		//	    loadStateChanging(27:14..14): BOOLEAN,
		//	    loadStateDirty(27:15..15): BOOLEAN,  -- set by debuggee, reset by debugger
		//	    -- Items describing the current trip to or from the debugger:
		//	    swapData(28): SwapData,
		//	    spareA(48): WORD,
		//	    spareB(49): WORD];
		CPPUNIT_ASSERT_EQUAL((CARD32) 50, SIZE(CPSwapDefs::ExternalStateVector));
		CPPUNIT_ASSERT_EQUAL((CARD32)  0, OFFSET(CPSwapDefs::ExternalStateVector, version));
		CPPUNIT_ASSERT_EQUAL((CARD32)  1, OFFSET(CPSwapDefs::ExternalStateVector, bootSession));
		CPPUNIT_ASSERT_EQUAL((CARD32)  3, OFFSET(CPSwapDefs::ExternalStateVector, loadState));
		CPPUNIT_ASSERT_EQUAL((CARD32)  5, OFFSET(CPSwapDefs::ExternalStateVector, mapLog));
		CPPUNIT_ASSERT_EQUAL((CARD32)  7, OFFSET(CPSwapDefs::ExternalStateVector, patchTable));
		CPPUNIT_ASSERT_EQUAL((CARD32)  9, OFFSET(CPSwapDefs::ExternalStateVector, breakBlocks));
		CPPUNIT_ASSERT_EQUAL((CARD32) 11, OFFSET(CPSwapDefs::ExternalStateVector, breakpointHandlers));
		CPPUNIT_ASSERT_EQUAL((CARD32) 13, OFFSET(CPSwapDefs::ExternalStateVector, mds));
		CPPUNIT_ASSERT_EQUAL((CARD32) 14, OFFSET(CPSwapDefs::ExternalStateVector, faultsBeingProcessed));
		CPPUNIT_ASSERT_EQUAL((CARD32) 20, OFFSET(CPSwapDefs::ExternalStateVector, systemVolumeID));
		CPPUNIT_ASSERT_EQUAL((CARD32) 25, OFFSET(CPSwapDefs::ExternalStateVector, virtualMemoryCount));
		CPPUNIT_ASSERT_EQUAL((CARD32) 27, OFFSET(CPSwapDefs::ExternalStateVector, u27));
		CPPUNIT_ASSERT_EQUAL((CARD32) 28, OFFSET(CPSwapDefs::ExternalStateVector, swapData));
		CPPUNIT_ASSERT_EQUAL((CARD32) 48, OFFSET(CPSwapDefs::ExternalStateVector, spareA));
		CPPUNIT_ASSERT_EQUAL((CARD32) 49, OFFSET(CPSwapDefs::ExternalStateVector, spareB));

		CPSwapDefs::ExternalStateVector t;

		//  patchTableInUse(27:0..12): BOOLEAN
		t.u27 = 0;
		t.patchTableInUse = ~t.patchTableInUse;
		CPPUNIT_ASSERT_EQUAL((CARD16)0xFFF8, t.u27);

		//  breakBlocksInUse(27:13..13): BOOLEAN
		t.u27 = 0;
		t.breakBlocksInUse = ~t.breakBlocksInUse;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0004, t.u27);

		//  loadStateChanging(27:14..14): BOOLEAN,
		t.u27 = 0;
		t.loadStateChanging = ~t.loadStateChanging;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0002, t.u27);

		//  loadStateDirty(27:15..15): BOOLEAN
		t.u27 = 0;
		t.loadStateDirty = ~t.loadStateDirty;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0001, t.u27);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(testESV);
