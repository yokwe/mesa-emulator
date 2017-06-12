/*
Copyright (c) 2014, 2017, Yasuhiro Hasegawa
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
// Opcode_special.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("special");

#include "../util/Debug.h"

#include "../mesa/Constant.h"
#include "../mesa/Type.h"
#include "../mesa/Memory.h"
#include "../mesa/Function.h"
#include "../mesa/MesaThread.h"

#include "../agent/Agent.h"

#include "Opcode.h"

// CallAgent: PROC [devIndex: AgentDeviceIndex] = MACHINE CODE
//	  {Mopcodes.zESC, aCALLAGENT};
void E_CALLAGENT(Opcode*) {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  CALLAGENT %2d", savedPC, stack[SP - 1]);
	CARD16 index = Pop();
	Agent::CallAgent(index);
}



// MapDisplay: PROC
//	  [startingVirtualPage: Environment.PageCount, startingRealAddress: LONG UNSPECIFIED,
//	   totalPageCount: CARDINAL, pageCountInEachBlock: CARDINAL] = MACHINE CODE
//	  {Mopcodes.zESC, aMAPDISPLAY};
void E_MAPDISPLAY(Opcode*) {
	CARD16 pageCountInEachBlock = Pop();
	CARD16 totalPageCount       = Pop();
	CARD32 startingRealPage     = PopLong();
	CARD32 startingVirtualPage  = PopLong();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  MAPDISPLAY  %6X  %6X %2d %2d", savedPC, startingVirtualPage, startingRealPage, totalPageCount, pageCountInEachBlock);
	if (totalPageCount != pageCountInEachBlock) ERROR();

	if (startingRealPage != Memory::getDisplayRealPage()) ERROR(); // sanity check

	CARD32 displayPageSize = Memory::getDisplayPageSize();
	if (totalPageCount != displayPageSize) ERROR(); // sanity check

	Memory::mapDisplay(startingVirtualPage, startingRealPage, totalPageCount);
}




// StopEmulator: PROC [secondsTillRestart: LONG CARDINAL = noRestart] = MACHINE CODE
//	  {Mopcodes.zESC, aSTOPEMULATOR};
void E_STOPEMULATOR(Opcode*) {
	logger.fatal("TRACE %6o  STOPEMULATOR %04X%04X", savedPC, stack[SP - 1], stack[SP - 2]);
	PopLong(); // pop long parameter
	ProcessorThread::stop();
}



//Version: PROCEDURE RETURNS [VersionResult] = MACHINE CODE {
//  Mopcodes.zESC, ESCAlpha.aVERSION};
void E_VERSION(Opcode*) {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  VERSION", savedPC);
	ProcessorFaceExtras::VersionResult result;

	result.machineType   = ProcessorFaceExtras::MT_daybreak;
	result.majorVersion  = 0;
	result.unused        = 0;
	result.floatingPoint = 1;
	result.cedar         = 0;
	result.releaseDate   = 40908; // 2013-01-01

	Push(result.u0);
	Push(result.releaseDate);
}



// a214
void E_214(Opcode*) {
	if (DEBUG_SHOW_DUMMY_IMPL_OPCODE || DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  A214", savedPC);
	// TODO Implements OP_A214
	PopLong();
	PopLong();
	PopLong();
	Push(0);
}
//14:59:24.02 DEBUG block        TRACE   1426  zBLTL     307E7    AB63F     4
//14:59:24.02 DEBUG Opcode       TRACE   1427  zLLD1
//14:59:24.02 DEBUG Opcode       TRACE   1430  zLLD7
//14:59:24.02 DEBUG Opcode       TRACE   1431  zLLDB 09
//14:59:24.02 DEBUG guam         TRACE   1433  a214  <= ##
//14:59:24.02 DEBUG Opcode       TRACE   1435  zDIS
//14:59:24.02 DEBUG control      TRACE   1436  zRET




// a305
void E_305(Opcode*) {
	if (DEBUG_SHOW_DUMMY_IMPL_OPCODE || DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  A305", savedPC);
	// TODO Implements OP_A305
}



// a306
void E_306(Opcode*) {
	CARD16 n = Pop();
	if (DEBUG_SHOW_DUMMY_IMPL_OPCODE || DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  A306  %d", savedPC, n);
	// TODO Implements OP_A306
	Push(0);
}
//13:02:26.01 DEBUG control      TRACE    222  zEFC2
//13:02:26.01 DEBUG Opcode       TRACE  13635  zSL0
//13:02:26.01 DEBUG Opcode       TRACE  13636  zSLD1
//13:02:26.01 DEBUG Opcode       TRACE  13637  zLI2
//13:02:26.01 DEBUG bootguam     TRACE  13640  a306  2
//13:02:26.01 DEBUG Opcode       TRACE  13642  zSL3     <= #1
//13:02:26.01 DEBUG Opcode       TRACE  13643  zLLD1
//13:02:26.01 DEBUG Opcode       TRACE  13644  zPLDB 04
//13:02:26.01 DEBUG Opcode       TRACE  13646  zIOR
