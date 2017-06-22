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


//
// LoadState.h
//

#ifndef LOADSTATE_H__
#define LOADSTATE_H__ 1

#include "Type.h"

//
// Trace XFER with module name
//   Hook write access to esv.loadStateDirty and collect module info.

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.0.1/Mesa/Friends/LoadStateFormat.mesa
//
class LoadStateFormat {
public:
	//VersionID: CARDINAL = 06113;
	const CARD16 VersionID = 06113;

	//Index: TYPE = CARDINAL;
	//nullIndex: Index = LAST[Index];
	const CARD16 nullIndex = ~0U;

	//ModuleInfoIndex: TYPE = Handle RELATIVE POINTER TO ARRAY [0..0) OF ModuleInfo;
	//BcdInfoIndex:    TYPE = Handle RELATIVE POINTER TO ARRAY Index[0..0) OF BcdInfo;

	//ID: TYPE = CARDINAL;

	//From PrincOps
	//  GlobalFrameHandle: TYPE = POINTER TO GlobalVariables
	//From PrincOpsExtras2
	//  GFTHandle: TYPE = GlobalFrameTable RELATIVE ORDERED POINTER [0..LAST[CARDINAL]] TO GFTItem;

	//ModuleInfo: TYPE = MACHINE DEPENDENT RECORD [
	//  resolved(0:0..0): BOOLEAN,
	//  cgfi(0:1..15): NATURAL,
	//  index(1): Index,
	//  globalFrame(2): SELECT OVERLAID * FROM
	//    oldWorld => [gf(2): PrincOps.GlobalFrameHandle],
	//    newWorld => [gfi(2): PrincOpsExtras2.GFTHandle],
	//    ENDCASE];
	struct ModuleInfo {
		union {
			CARD16 u0;
			struct {
				CARD16 resolved :  1;
				CARD16 cgfi     : 15;
			};
		};
		CARD16 index;
		CARD16 globalFrame; // can be PrincOps.GlobalFrameHandle or PrincOpsExtras2.GFTHandle
	} __attribute__((packed));


	//BcdInfo: TYPE = MACHINE DEPENDENT RECORD [
	//  exports(0:0..0), typeExported(0:1..1): BOOLEAN,
	//  pages(0:2..15): [1..256],
	//  base(1): LONG BASE POINTER TO BcdDefs.BCD,
	//  id(3): ID];
	struct BcdInfo {
		union {
			CARD16 u0;
			struct {
				CARD16 exports      :  1;
				CARD16 typeExported :  1;
				CARD16 pages        : 14;
			};
			CARD32 base;
			CARD16 id;
		};
	} __attribute__((packed));


	//Object: TYPE = MACHINE DEPENDENT RECORD [
	//  versionident: CARDINAL,
	//  nModules: CARDINAL,
	//  maxModules: CARDINAL,
	//  nBcds: CARDINAL,
	//  maxBcds: CARDINAL,
	//  nextID: ID,
	//  moduleInfo: ModuleInfoIndex,
	//  bcdInfo: BcdInfoIndex];

	struct Object {
		CARD16 versionident;
		CARD16 nModules;
		CARD16 maxModules;
		CARD16 nBcds;
		CARD16 maxBcds;
		CARD16 nextID;
		CARD16 moduleInfo;
		CARD16 bcdInfo;
	} __attribute__((packed));
};

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.0.1/Pilot/Friends/CPSwapDefs.mesa
//
class CPSwapDefs {
public:
	//oldVersion: CARDINAL = 01054;
	//currentVersion: CARDINAL = 01055;
	static const CARD16 currentVersion = 01055;

	//PESV: PROCEDURE[] RETURNS [LONG POINTER TO ExternalStateVector] = INLINE {
	//  RETURN[LOOPHOLE[PSB.PDA.available, SwapInfo].externalStateVector]};
	CARD32 PESV() {
		CARD32 p = PDA + OFFSET(ProcessDataArea, available);
		return ReadDbl(p);
	}

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

	struct ExternalStateVector {
		CARD16 version;     // version of CPSwapDefs. It must be 01055
		CARD32 bootSession; // random number identifying boot session
		CARD32 loadState;   // LONG POITER TO LoadStateFormat.Object
		// Not every field is described
	} __attribute__((packed));

};
#endif
