/*
Copyright (c) 2014, 2017 Yasuhiro Hasegawa
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
// OpcodeMop1xx.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("2xx");

#include "../util/Debug.h"
#include "../util/Perf.h"

#include "../mesa/MesaThread.h"

#include "Opcode.h"
#include "Interpreter.h"


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_CATCH_(CARD16 arg) {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  CATCH %3d", savedPC, arg);
}
// 0200  ASSIGN_MOP(z, CATCH)
void E_CATCH() {
	E_CATCH_(GetCodeByte());
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_J_(INT16 arg) {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  J %5d", savedPC, arg);
	PC = savedPC + arg;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}
#define Jn(n) \
void E_J##n () { \
	E_J_(n); \
}
// 0201  ASSIGN_MOP(z, J2)
Jn(2)
// 0202  ASSIGN_MOP(z, J3)
Jn(3)
// 0203  ASSIGN_MOP(z, J4)
Jn(4)
// 0204  ASSIGN_MOP(z, J5)
Jn(5)
// 0205  ASSIGN_MOP(z, J6)
Jn(6)
// 0206  ASSIGN_MOP(z, J7)
Jn(7)
// 0207  ASSIGN_MOP(z, J8)
Jn(8)
// 0210  ASSIGN_MOP(z, JB)
void E_JB() {
	E_J_(SignExtend(GetCodeByte()));
}
// 0211  ASSIGN_MOP(z, JW)
void E_JW() {
	E_J_((INT16)GetCodeWord());
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_JUE_(CARD16 arg0, INT16 arg1) {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  JUE %3d %3d", savedPC, arg0, arg1);
	UNSPEC data = Pop();
	if (data == arg0) PC = savedPC + arg1;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}
// 0212  ASSIGN_MOP(z, JEP)
void E_JEP() {
	NibblePair pair = {GetCodeByte()};
	E_JUE_(pair.left, SignExtend(pair.right + 4));
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_JEB_(INT16 arg) {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  JEB %3d", savedPC, arg);
	UNSPEC v = Pop();
	UNSPEC u = Pop();
	if (u == v) PC = savedPC + arg;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}
// 0213  ASSIGN_MOP(z, JEB)
void E_JEB() {
	E_JEB_(SignExtend(GetCodeByte()));
}
// 0214  ASSIGN_MOP(z, JEBB)
void E_JEBB() {
	CARD8 arg0 = GetCodeByte();
	CARD8 arg1 = GetCodeByte();
	E_JUE_(arg0, SignExtend(arg1));
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_JUNE_(CARD16 arg0, INT16 arg1) {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  JUNE %3d %3d", savedPC, arg0, arg1);
	UNSPEC data = Pop();
	if (data != arg0) PC = savedPC + arg1;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}
// 0215  ASSIGN_MOP(z, JNEP)
void E_JNEP() {
	NibblePair pair = {GetCodeByte()};
	E_JUNE_(pair.left, SignExtend(pair.right + 4));
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_JNE_(INT16 arg) {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  JNE %3d", savedPC, arg);
	UNSPEC v = Pop();
	UNSPEC u = Pop();
	if (u != v) PC = savedPC + arg;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}
// 0216  ASSIGN_MOP(z, JNEB)
void E_JNEB() {
	E_JNE_(SignExtend(GetCodeByte()));
}
// 0217  ASSIGN_MOP(z, JNEBB)
void E_JNEBB() {
	CARD8 arg0 = GetCodeByte();
	CARD8 arg1 = GetCodeByte();
	E_JUNE_(arg0, SignExtend(arg1));
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// 0220  ASSIGN_MOP(z, JLB)
__attribute__((always_inline)) static inline void E_JL_(INT16 arg) {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  JL %3d", savedPC, arg);
	INT16 k = Pop();
	INT16 j = Pop();
	if (j < k) PC = savedPC + arg;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}
void E_JLB() {
	E_JL_(SignExtend(GetCodeByte()));
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// 0221  ASSIGN_MOP(z, JGEB)
__attribute__((always_inline)) static inline void E_JGE_(INT16 arg) {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  JGE %3d", savedPC, arg);
	INT16 k = Pop();
	INT16 j = Pop();
	if (j >= k) PC = savedPC + arg;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}
void E_JGEB() {
	E_JGE_(SignExtend(GetCodeByte()));
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_JG_(INT16 arg) {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  JG %3d", savedPC, arg);
	INT16 k = Pop();
	INT16 j = Pop();
	if (j > k) PC = savedPC + arg;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}
// 0222  ASSIGN_MOP(z, JGB)
void E_JGB() {
	E_JG_(SignExtend(GetCodeByte()));
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_JLE_(INT16 arg) {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  JLE %3d", savedPC, arg);
	INT16 k = Pop();
	INT16 j = Pop();
	if (j <= k) PC = savedPC + arg;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}
// 0223  ASSIGN_MOP(z, JLEB)
void E_JLEB() {
	E_JLE_(SignExtend(GetCodeByte()));
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_JUL_(INT16 arg) {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  JUL %3d", savedPC, arg);
	CARDINAL v = Pop();
	CARDINAL u = Pop();
	if (u < v) PC = savedPC + arg;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}
// 0224  ASSIGN_MOP(z, JULB)
void E_JULB() {
	E_JUL_(SignExtend(GetCodeByte()));
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_JUGE_(INT16 arg) {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  JUGE %3d", savedPC, arg);
	CARDINAL v = Pop();
	CARDINAL u = Pop();
	if (u >= v) PC = savedPC + arg;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}
// 0225  ASSIGN_MOP(z, JUGEB)
void E_JUGEB() {
	E_JUGE_(SignExtend(GetCodeByte()));
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_JUG_(INT16 arg) {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  JUG %3d", savedPC, arg);
	CARDINAL v = Pop();
	CARDINAL u = Pop();
	if (u > v) PC = savedPC + arg;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}
// 0226  ASSIGN_MOP(z, JUGB)
void E_JUGB() {
	E_JUG_(SignExtend(GetCodeByte()));
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_JULE_(INT16 arg) {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  JULE %3d", savedPC, arg);
	CARDINAL v = Pop();
	CARDINAL u = Pop();
	if (u <= v) PC = savedPC + arg;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}
// 0227  ASSIGN_MOP(z, JULEB)
void E_JULEB() {
	E_JULE_(SignExtend(GetCodeByte()));
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_JZ_(INT16 arg) {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  JZ %3d", savedPC, arg);
	UNSPEC u = Pop();
	if (u == 0) PC = savedPC + arg;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}
#define JZn(n) \
void E_JZ##n () { \
	E_JZ_(n); \
}
// 0230  ASSIGN_MOP(z, JZ3)
JZn(3)
// 0231  ASSIGN_MOP(z, JZ4)
JZn(4)
// 0232  ASSIGN_MOP(z, JZB)
void E_JZB() {
	E_JZ_(SignExtend(GetCodeByte()));
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_JNZ_(INT16 arg) {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  JNZ %3d", savedPC, arg);
	UNSPEC u = Pop();
	if (u != 0) PC = savedPC + arg;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}
#define JNZn(n) \
void E_JNZ##n () { \
	E_JNZ_(n); \
}
// 0233  ASSIGN_MOP(z, JNZ3)
JNZn(3)
// 0234  ASSIGN_MOP(z, JNZ4)
JNZn(4)
// 0235  ASSIGN_MOP(z, JNZB)
void E_JNZB() {
	E_JNZ_(SignExtend(GetCodeByte()));
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_JDE_(INT16 arg) {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  JDE %5d", savedPC, arg);
	LONG_UNSPEC v = PopLong();
	LONG_UNSPEC u = PopLong();
	if (u == v) PC = savedPC + arg;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}
// 0236  ASSIGN_MOP(z, JDEB)
void E_JDEB() {
	E_JDE_(SignExtend(GetCodeByte()));
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_JDNE_(INT16 arg) {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  JDNE %5d", savedPC, arg);
	LONG_UNSPEC v = PopLong();
	LONG_UNSPEC u = PopLong();
	if (u != v) PC = savedPC + arg;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}
// 0237  ASSIGN_MOP(z, JDNEB)
void E_JDNEB() {
	E_JDNE_(SignExtend(GetCodeByte()));
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_JIB_(CARD16 arg) {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  JIB %5d", savedPC, arg);
	CARDINAL limit = Pop();
	CARDINAL index = Pop();
	if (index < limit) {
		BytePair disp = {ReadCode(arg + index / 2)};
		PC = savedPC + (((index % 2) == 0) ? disp.left : disp.right);
	}
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}
// 0240  ASSIGN_MOP(z, JIB)
void E_JIB() {
	E_JIB_(GetCodeWord());
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_JIW_(CARD16 arg) {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  JIW %5d", savedPC, arg);
	CARDINAL limit = Pop();
	CARDINAL index = Pop();
	if (index < limit) {
		CARDINAL disp = ReadCode(arg + index);
		PC = savedPC + disp;
	}
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}
// 0241  ASSIGN_MOP(z, JIW)
void E_JIW() {
	E_JIW_(GetCodeWord());
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// 0242  ASSIGN_MOP(z, REC)
void E_REC() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  REC", savedPC);
	Recover();
}
// 0243  ASSIGN_MOP(z, REC2)
void E_REC2() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  REC2", savedPC);
	Recover();
	Recover();
}
// 0244  ASSIGN_MOP(z, DIS)
void E_DIS() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  DIS", savedPC);
	Discard();
}
// 0245  ASSIGN_MOP(z, DIS2)
void E_DIS2() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  DIS2", savedPC);
	Discard();
	Discard();
}
// 0246  ASSIGN_MOP(z, EXCH)
void E_EXCH() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  EXCH", savedPC);
	UNSPEC v = Pop();
	UNSPEC u = Pop();
	Push(v);
	Push(u);
}
// 0247  ASSIGN_MOP(z, DEXCH)
void E_DEXCH() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  DEXCH", savedPC);
	LONG_UNSPEC v = PopLong();
	LONG_UNSPEC u = PopLong();
	PushLong(v);
	PushLong(u);
}
// 0250  ASSIGN_MOP(z, DUP)
void E_DUP() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  DUP", savedPC);
	UNSPEC u = Pop();
	Push(u);
	Push(u);
}
// 0251  ASSIGN_MOP(z, DDUP)
void E_DDUP() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  DDUP", savedPC);
	LONG_UNSPEC u = PopLong();
	PushLong(u);
	PushLong(u);
}
// 0252  ASSIGN_MOP(z, EXDIS)
void E_EXDIS() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  EXDIS", savedPC);
	UNSPEC u = Pop();
	/*UNSPEC v = */ Pop();
	Push(u);
}
// 0253  ASSIGN_MOP(z, NEG)
void E_NEG() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  NEG", savedPC);
	if (SP == 0) STACK_ERROR();
	stack[SP - 1] = -stack[SP - 1];
	//INT16 i = Pop();
	//Push(-i);
}
// 0254  ASSIGN_MOP(z, INC)
void E_INC() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  INC", savedPC);
	if (SP == 0) STACK_ERROR();
	stack[SP - 1]++;
	//CARDINAL s = Pop();
	//Push(s + 1);
}
// 0255  ASSIGN_MOP(z, DEC)
void E_DEC() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  DEC", savedPC);
	if (SP == 0) STACK_ERROR();
	stack[SP - 1]--;
	//CARDINAL s = Pop();
	//Push(s - 1);
}
// 0256  ASSIGN_MOP(z, DINC)
void E_DINC() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  DINC", savedPC);
	if (SP < 2) STACK_ERROR();
	stack[SP - 2] += 1;
	if (stack[SP - 2] == 0) stack[SP - 1] += 1;
	//LONG_CARDINAL s = PopLong();
	//PushLong(s + 1);
}
// 0257  ASSIGN_MOP(z, DBL)
void E_DBL() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  DBL", savedPC);
	if (SP == 0) STACK_ERROR();
	stack[SP - 1] *= 2;
	//UNSPEC u = Pop();
	//Push(u << 1);
}
// 0260  ASSIGN_MOP(z, DDBL)
void E_DDBL() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  DDBL", savedPC);
	LONG_UNSPEC u = PopLong();
	PushLong(u << 1);
}
// 0261  ASSIGN_MOP(z, TRPL)
void E_TRPL() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  TRPL", savedPC);
	if (SP == 0) STACK_ERROR();
	stack[SP - 1] *= 3;
	//CARDINAL s = Pop();
	//Push(s * 3);
}
// 0262  ASSIGN_MOP(z, AND)
void E_AND() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  AND", savedPC);
	UNSPEC v = Pop();
	UNSPEC u = Pop();
	Push(u & v);
}
// 0263  ASSIGN_MOP(z, IOR)
void E_IOR() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  IOR", savedPC);
	UNSPEC v = Pop();
	UNSPEC u = Pop();
	Push(u | v);
}
// 0264  ASSIGN_MOP(z, ADDSB)
__attribute__((always_inline)) static inline void E_ADDS_(INT16 arg) {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  ADDS %02X", savedPC, arg);
	CARD16 i = Pop();
	Push(i + arg);
}
void E_ADDSB() {
	E_ADDS_(SignExtend(GetCodeByte()));
}
// 0265  ASSIGN_MOP(z, ADD)
void E_ADD() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  ADD", savedPC);
	CARDINAL t = Pop();
	CARDINAL s = Pop();
	Push(s + t);
}
// 0266  ASSIGN_MOP(z, SUB)
void E_SUB() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  SUB", savedPC);
	CARDINAL t = Pop();
	CARDINAL s = Pop();
	Push(s - t);
}
// 0267  ASSIGN_MOP(z, DADD)
void E_DADD() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  DADD", savedPC);
	LONG_CARDINAL t = PopLong();
	LONG_CARDINAL s = PopLong();
	PushLong(s + t);
}
// 0270  ASSIGN_MOP(z, DSUB)
void E_DSUB() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  DSUB", savedPC);
	LONG_CARDINAL t = PopLong();
	LONG_CARDINAL s = PopLong();
	PushLong(s - t);
}
// 0271  ASSIGN_MOP(z, ADC)
void E_ADC() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  ADC", savedPC);
	CARDINAL t = Pop();
	LONG_CARDINAL s = PopLong();
	PushLong(s + t);
}
// 0272  ASSIGN_MOP(z, ACD)
void E_ACD() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  ACD", savedPC);
	LONG_CARDINAL t = PopLong();
	CARDINAL s = Pop();
	PushLong(s + t);
}
// 0273  ASSIGN_MOP(z, AL0IB)
__attribute__((always_inline)) static inline void E_AL0I_(CARD16 arg) {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  AL0I %02X", savedPC, arg);
	CARD16* p = FetchLF(0);
	// NO PAGE FAULT AFTER HERE
	Push(*p + arg);
}
void E_AL0IB() {
	E_AL0I_(GetCodeByte());
}
// 0274  ASSIGN_MOP(z, MUL)
void E_MUL() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  MUL", savedPC);
	CARDINAL t = Pop();
	CARDINAL s = Pop();
	PushLong((LONG_CARDINAL)s * t);
	//Discard();
	SP--;
}
// 0275  ASSIGN_MOP(z, DCMP)
void E_DCMP() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  DCMP", savedPC);
	INT32 k = PopLong();
	INT32 j = PopLong();
	Push((k == j) ? 0 : ((j > k) ? 1 : -1));
}
// 0276  ASSIGN_MOP(z, UDCMP)
void E_UDCMP() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  UDCMP", savedPC);
	LONG_CARDINAL t = PopLong();
	LONG_CARDINAL s = PopLong();
	Push((s == t) ? 0 : ((s > t) ? 1 : -1));
}
// 0277  ASSIGN_MOP(z, VMFIND)
//RunBase: TYPE = LONG BASE POINTER TO ARRAY IndexRun OF Run;
//PRun: TYPE = RunBase RELATIVE ORDERED POINTER [0..LAST[CARDINAL]] TO Run;
//Find: PROCEDURE [page: VM.PageNumber, rBase: RunBase, pRunTop: PRun]
//  RETURNS [found: BOOLEAN, pRun: PRun] = MACHINE CODE {MopcodesExtras2.zVMFIND};
void E_VMFIND() {
	CARD16 pRunTop = Pop();
	CARD32 rBase   = PopLong();
	CARD32 page    = PopLong();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  VMFIND  %06X  %08X  %04X", savedPC, page, rBase, pRunTop);

	CARD16 found = 0;
	CARD16 pRun  = 0;

//	{
//		logger.debug("%s  zVMFIND  %06X  %08X  %04X", __FUNCTION__, page, rBase, pRunTop);
//		for(CARD32 p = VMDataInternal::pRunFirst; p <= pRunTop; p += SIZE(VMDataInternal::Run)) {
//			CARD32 pPage  = ReadDbl(rBase + p + OFFSET(VMDataInternal::Run, interval.page));
//			CARD32 pCount = ReadDbl(rBase + p + OFFSET(VMDataInternal::Run, interval.count));
//			logger.debug("%s  pRun %08X  %08X %5d", __FUNCTION__, p, pPage, pCount);
//		}
//	}

	//pageTop: VM.PageNumber = FIRST[VM.PageNumber] + StoragePrograms.countVM;  --end of VM
	CARD32 pageTop = 0 + Memory::getVPSize();
	//CARD32 pageTop = ReadDbl(rBase + pRunTop + OFFSET(VMDataInternal::Run, interval.page));

	//indexRunFirst: CARDINAL = (pRunFirst - FIRST[PRun])/SIZE[Run];
	const CARD16 indexRunFirst = (VMDataInternal::pRunFirst - 0) / SIZE(VMDataInternal::Run);

	//IF page >= pageTop THEN Bug[beyondVM];
	if (pageTop <= page) ERROR();

	{
		//vmDatabaseFullSearches = vmDatabaseFullSearches.SUCC;
		//indexRunLow = indexRunFirst;
		CARD16 indexRunLow = indexRunFirst;
//		logger.debug("%s  indexRunLow  = %d", __FUNCTION__, indexRunLow);

		//indexRunHigh = (pRunTop - FIRST[PRun])/SIZE[Run];
		CARD16 indexRunHigh = (pRunTop - 0) / SIZE(VMDataInternal::Run);
//		logger.debug("%s  indexRunHigh = %d", __FUNCTION__, indexRunHigh);

		//DO  --UNTIL search terminates--
		for(;;) {
			//  indexRun = (indexRunLow + indexRunHigh)/2;
			CARD16 indexRun = (indexRunLow + indexRunHigh) / 2;
//			logger.debug("%s  indexRun = %d", __FUNCTION__, indexRun);

			//pageComp = rBase[FIRST[PRun] + indexRun*SIZE[Run]].interval.page;
			CARD32 pageComp  = ReadDbl(rBase + 0 + (indexRun * SIZE(VMDataInternal::Run)) + OFFSET(VMDataInternal::Run, interval.page));
//			CARD32 countComp = ReadDbl(rBase + 0 + (indexRun * SIZE(VMDataInternal::Run)) + OFFSET(VMDataInternal::Run, interval.count));
//			logger.debug("%s  pageComp  = %08X", __FUNCTION__, pageComp);
//			logger.debug("%s  countComp = %d",   __FUNCTION__, countComp);

			//IF pageComp > page
			//  THEN indexRunHigh = indexRun - 1
			//    -- note that indexRunHigh, a CARDINAL, might be indexDescLow-1 here.
			//  ELSE
			//    IF page > pageComp
			//      THEN indexRunLow = indexRun + 1
			//      ELSE GO TO Exact;
			if (page < pageComp)
				indexRunHigh = indexRun - 1;
			else
				if (pageComp < page) indexRunLow = indexRun + 1;
				else {
					// Exact => {pRun = FIRST[PRun] + indexRun*SIZE[Run]; found = TRUE};
//					logger.debug("%s  Exact", __FUNCTION__);
					pRun = 0 + indexRun * SIZE(VMDataInternal::Run);
					found = 1;
					break;
				}

//			logger.debug("%s  indexRunHigh = %d", __FUNCTION__, indexRunHigh);
//			logger.debug("%s  indexRunLow  = %d", __FUNCTION__, indexRunLow);
			//  IF indexRunHigh < indexRunLow THEN GO TO NotExact;
			if (indexRunHigh < indexRunLow) {
				//  NotExact =>
				//    -- Assert: page>"indexRunHigh".page AND page<"indexRunHigh+1".page AND indexRunHigh+1 = indexRunLow.
				//    IF indexRunLow = indexRunFirst THEN {pRun = pRunFirst; found = FALSE}
				//    ELSE
				//      BEGIN
				//      pRun = FIRST[PRun] + indexRunHigh*SIZE[Run];
				//      IF page < rBase[pRun].interval.page + rBase[pRun].interval.count THEN
				//        found = TRUE
				//      ELSE {pRun = pRun + SIZE[Run]; found = FALSE};
				//      END;
				if (indexRunLow == indexRunFirst) {
					pRun = VMDataInternal::pRunFirst;
					found = 0;
				} else {
					pRun = 0 + indexRunHigh * SIZE(VMDataInternal::Run);
					const CARD32 intervalPage  = ReadDbl(rBase + pRun + OFFSET(VMDataInternal::Run, interval.page));
					const CARD32 intervalCount = ReadDbl(rBase + pRun + OFFSET(VMDataInternal::Run, interval.count));
//					logger.debug("%s  inervalPage = %08X  intervalCount = %d", __FUNCTION__, intervalPage, intervalCount);
					if (page < (intervalPage + intervalCount)) {
						found = 1;
					} else {
						pRun = pRun + SIZE(VMDataInternal::Run);
						found = 0;
					}
				}
				break;
			}
		}
	}
	//ENDLOOP;  --DO UNTIL search terminates--
	//EXITS
	//  Exact => {pRun = FIRST[PRun] + indexRun*SIZE[Run]; found = TRUE};
	//  NotExact =>
	//    -- Assert: page>"indexRunHigh".page AND page<"indexRunHigh+1".page AND indexRunHigh+1 = indexRunLow.
	//    IF indexRunLow = indexRunFirst THEN {pRun = pRunFirst; found = FALSE}
	//    ELSE
	//      BEGIN
	//      pRun = FIRST[PRun] + indexRunHigh*SIZE[Run];
	//      IF page < rBase[pRun].interval.page + rBase[pRun].interval.count THEN
	//        found = TRUE
	//      ELSE {pRun = pRun + SIZE[Run]; found = FALSE};
	//      END;
	//END;  --scope of SameAsLastTime--

	Push(found);
	Push(pRun);
//	logger.debug("%s  found %d  pRun %04X", __FUNCTION__, found, pRun);
}

