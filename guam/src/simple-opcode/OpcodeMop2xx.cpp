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


__attribute__((always_inline)) static inline void E_CATCH_(CARD16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  CATCH %3d", savedPC, arg);
}


__attribute__((always_inline)) static inline void E_J_(INT16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  J %5d", savedPC, arg);
	PC = savedPC + arg;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}
#define Jn(n) \
void E_J##n (Opcode*) { \
	E_J_(n); \
}


__attribute__((always_inline)) static inline void E_JUE_(CARD16 arg0, INT16 arg1) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JUE %3d %3d", savedPC, arg0, arg1);
	UNSPEC data = Pop();
	if (data == arg0) PC = savedPC + arg1;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}


__attribute__((always_inline)) static inline void E_JEB_(INT16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JEB %3d", savedPC, arg);
	UNSPEC v = Pop();
	UNSPEC u = Pop();
	if (u == v) PC = savedPC + arg;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}


__attribute__((always_inline)) static inline void E_JUNE_(CARD16 arg0, INT16 arg1) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JUNE %3d %3d", savedPC, arg0, arg1);
	UNSPEC data = Pop();
	if (data != arg0) PC = savedPC + arg1;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}


__attribute__((always_inline)) static inline void E_JNEB_(INT16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JNEB %3d", savedPC, arg);
	UNSPEC v = Pop();
	UNSPEC u = Pop();
	if (u != v) PC = savedPC + arg;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}


__attribute__((always_inline)) static inline void E_JLB_(INT16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JLB %3d", savedPC, arg);
	INT16 k = Pop();
	INT16 j = Pop();
	if (j < k) PC = savedPC + arg;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}


__attribute__((always_inline)) static inline void E_JGEB_(INT16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JGEB %3d", savedPC, arg);
	INT16 k = Pop();
	INT16 j = Pop();
	if (j >= k) PC = savedPC + arg;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}


__attribute__((always_inline)) static inline void E_JGB_(INT16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JGB %3d", savedPC, arg);
	INT16 k = Pop();
	INT16 j = Pop();
	if (j > k) PC = savedPC + arg;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}


__attribute__((always_inline)) static inline void E_JLEB_(INT16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JLEB %3d", savedPC, arg);
	INT16 k = Pop();
	INT16 j = Pop();
	if (j <= k) PC = savedPC + arg;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}


__attribute__((always_inline)) static inline void E_JULB_(INT16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  zJULB %3d", savedPC, arg);
	CARDINAL v = Pop();
	CARDINAL u = Pop();
	if (u < v) PC = savedPC + arg;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}


__attribute__((always_inline)) static inline void E_JUGEB_(INT16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JUGEB %3d", savedPC, arg);
	CARDINAL v = Pop();
	CARDINAL u = Pop();
	if (u >= v) PC = savedPC + arg;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}


__attribute__((always_inline)) static inline void E_JUGB_(INT16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JUGB %3d", savedPC, arg);
	CARDINAL v = Pop();
	CARDINAL u = Pop();
	if (u > v) PC = savedPC + arg;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}


__attribute__((always_inline)) static inline void E_JULEB_(INT16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JULEB %3d", savedPC, arg);
	CARDINAL v = Pop();
	CARDINAL u = Pop();
	if (u <= v) PC = savedPC + arg;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}


__attribute__((always_inline)) static inline void E_JZ_(INT16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JZ %3d", savedPC, arg);
	UNSPEC u = Pop();
	if (u == 0) PC = savedPC + arg;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}
#define JZn(n) \
void E_JZ##n (Opcode*) { \
	E_JZ_(n); \
}


__attribute__((always_inline)) static inline void E_JNZ_(INT16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JNZ %3d", savedPC, arg);
	UNSPEC u = Pop();
	if (u != 0) PC = savedPC + arg;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}
#define JNZn(n) \
void E_JNZ##n (Opcode*) { \
	E_JNZ_(n); \
}


__attribute__((always_inline)) static inline void E_JDE_(INT16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JDE %5d", savedPC, arg);
	LONG_UNSPEC v = PopLong();
	LONG_UNSPEC u = PopLong();
	if (u == v) PC = savedPC + arg;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}


__attribute__((always_inline)) static inline void E_JDNE_(INT16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JDNE %5d", savedPC, arg);
	LONG_UNSPEC v = PopLong();
	LONG_UNSPEC u = PopLong();
	if (u != v) PC = savedPC + arg;
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}


__attribute__((always_inline)) static inline void E_JIB_(CARD16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JIB %5d", savedPC, arg);
	CARDINAL limit = Pop();
	CARDINAL index = Pop();
	if (index < limit) {
		BytePair disp = {ReadCode(arg + index / 2)};
		PC = savedPC + (((index % 2) == 0) ? disp.left : disp.right);
	}
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}


__attribute__((always_inline)) static inline void E_JIW_(CARD16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JIW %5d", savedPC, arg);
	CARDINAL limit = Pop();
	CARDINAL index = Pop();
	if (index < limit) {
		CARDINAL disp = ReadCode(arg + index);
		PC = savedPC + disp;
	}
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}


__attribute__((always_inline)) static inline void E_REC_() {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  REC", savedPC);
	Recover();
}


__attribute__((always_inline)) static inline void E_REC2_() {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  REC2", savedPC);
	Recover();
	Recover();
}


__attribute__((always_inline)) static inline void E_DIS_() {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DIS", savedPC);
	Discard();
}


__attribute__((always_inline)) static inline void E_DIS2_() {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DIS2", savedPC);
	Discard();
	Discard();
}


__attribute__((always_inline)) static inline void E_EXCH_() {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  EXCH", savedPC);
	UNSPEC v = Pop();
	UNSPEC u = Pop();
	Push(v);
	Push(u);
}

__attribute__((always_inline)) static inline void E_DEXCH_() {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DEXCH", savedPC);
	LONG_UNSPEC v = PopLong();
	LONG_UNSPEC u = PopLong();
	PushLong(v);
	PushLong(u);
}


__attribute__((always_inline)) static inline void E_DUP_() {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DUP", savedPC);
	UNSPEC u = Pop();
	Push(u);
	Push(u);
}


__attribute__((always_inline)) static inline void E_DDUP_() {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DDUP", savedPC);
	LONG_UNSPEC u = PopLong();
	PushLong(u);
	PushLong(u);
}


__attribute__((always_inline)) static inline void E_EXDIS_() {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  EXDIS", savedPC);
	UNSPEC u = Pop();
	/*UNSPEC v = */ Pop();
	Push(u);
}


__attribute__((always_inline)) static inline void E_NEG_() {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  NEG", savedPC);
	if (SP == 0) STACK_ERROR();
	stack[SP - 1] = -stack[SP - 1];
	//INT16 i = Pop();
	//Push(-i);
}

__attribute__((always_inline)) static inline void E_INC_() {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  INC", savedPC);
	if (SP == 0) STACK_ERROR();
	stack[SP - 1]++;
	//CARDINAL s = Pop();
	//Push(s + 1);
}


__attribute__((always_inline)) static inline void E_DEC_() {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DEC", savedPC);
	if (SP == 0) STACK_ERROR();
	stack[SP - 1]--;
	//CARDINAL s = Pop();
	//Push(s - 1);
}


__attribute__((always_inline)) static inline void E_DINC_() {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DINC", savedPC);
	if (SP < 2) STACK_ERROR();
	stack[SP - 2] += 1;
	if (stack[SP - 2] == 0) stack[SP - 1] += 1;
	//LONG_CARDINAL s = PopLong();
	//PushLong(s + 1);
}


__attribute__((always_inline)) static inline void E_DBL_() {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DBL", savedPC);
	if (SP == 0) STACK_ERROR();
	stack[SP - 1] *= 2;
	//UNSPEC u = Pop();
	//Push(u << 1);
}


__attribute__((always_inline)) static inline void E_DDBL_() {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DDBL", savedPC);
	LONG_UNSPEC u = PopLong();
	PushLong(u << 1);
}


__attribute__((always_inline)) static inline void E_TRPL_() {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  TRPL", savedPC);
	if (SP == 0) STACK_ERROR();
	stack[SP - 1] *= 3;
	//CARDINAL s = Pop();
	//Push(s * 3);
}


__attribute__((always_inline)) static inline void E_AND_() {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  AND", savedPC);
	UNSPEC v = Pop();
	UNSPEC u = Pop();
	Push(u & v);
}


__attribute__((always_inline)) static inline void E_IOR_() {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  IOR", savedPC);
	UNSPEC v = Pop();
	UNSPEC u = Pop();
	Push(u | v);
}


__attribute__((always_inline)) static inline void E_ADDSB_(INT16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  ADDSB %02X", savedPC, arg);
	CARD16 i = Pop();
	Push(i + arg);
}


__attribute__((always_inline)) static inline void E_ADD_() {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  ADD", savedPC);
	CARDINAL t = Pop();
	CARDINAL s = Pop();
	Push(s + t);
}


__attribute__((always_inline)) static inline void E_SUB_() {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  SUB", savedPC);
	CARDINAL t = Pop();
	CARDINAL s = Pop();
	Push(s - t);
}


__attribute__((always_inline)) static inline void E_DADD_() {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DADD", savedPC);
	LONG_CARDINAL t = PopLong();
	LONG_CARDINAL s = PopLong();
	PushLong(s + t);
}


__attribute__((always_inline)) static inline void E_DSUB_() {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DSUB", savedPC);
	LONG_CARDINAL t = PopLong();
	LONG_CARDINAL s = PopLong();
	PushLong(s - t);
}


__attribute__((always_inline)) static inline void E_ADC_() {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  ADC", savedPC);
	CARDINAL t = Pop();
	LONG_CARDINAL s = PopLong();
	PushLong(s + t);
}


__attribute__((always_inline)) static inline void E_ACD_() {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  ACD", savedPC);
	LONG_CARDINAL t = PopLong();
	CARDINAL s = Pop();
	PushLong(s + t);
}


__attribute__((always_inline)) static inline void E_AL0IB_(CARD16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  AL0IB %02X", savedPC, arg);
	CARD16* p = FetchLF(0);
	// NO PAGE FAULT AFTER HERE
	Push(*p + arg);
}


__attribute__((always_inline)) static inline void E_MUL_() {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  MUL", savedPC);
	CARDINAL t = Pop();
	CARDINAL s = Pop();
	PushLong((LONG_CARDINAL)s * t);
	//Discard();
	SP--;
}


__attribute__((always_inline)) static inline void E_DCMP_() {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DCMP", savedPC);
	INT32 k = PopLong();
	INT32 j = PopLong();
	Push((k == j) ? 0 : ((j > k) ? 1 : -1));
}


__attribute__((always_inline)) static inline void E_UDCMP_() {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  UDCMP", savedPC);
	LONG_CARDINAL t = PopLong();
	LONG_CARDINAL s = PopLong();
	Push((s == t) ? 0 : ((s > t) ? 1 : -1));
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// 0200  ASSIGN_MOP(z, CATCH)
void E_CATCH (Opcode*) {
	E_CATCH_(GetCodeByte());
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
void E_JB (Opcode*) {
	E_J_(SignExtend(GetCodeByte()));
}
// 0211  ASSIGN_MOP(z, JW)
void E_JW (Opcode*) {
	E_J_((INT16)GetCodeWord());
}
// 0212  ASSIGN_MOP(z, JEP)
void E_JEP (Opcode*) {
	NibblePair pair = {GetCodeByte()};
	E_JUE_(pair.left, SignExtend(pair.right + 4));
}
// 0213  ASSIGN_MOP(z, JEB)
void E_JEB (Opcode*) {
	E_JEB_(SignExtend(GetCodeByte()));
}
// 0214  ASSIGN_MOP(z, JEBB)
void E_JEBB (Opcode*) {
	CARD8 arg0 = GetCodeByte();
	CARD8 arg1 = GetCodeByte();
	E_JUE_(arg0, SignExtend(arg1));
}
// 0215  ASSIGN_MOP(z, JNEP)
void E_JNEP (Opcode*) {
	NibblePair pair = {GetCodeByte()};
	E_JUNE_(pair.left, SignExtend(pair.right + 4));
}
// 0216  ASSIGN_MOP(z, JNEB)
void E_JNEB (Opcode*) {
	E_JNEB_(SignExtend(GetCodeByte()));
}
// 0217  ASSIGN_MOP(z, JNEBB)
void E_JNEBB (Opcode*) {
	CARD8 arg0 = GetCodeByte();
	CARD8 arg1 = GetCodeByte();
	E_JUNE_(arg0, SignExtend(arg1));
}


// 0220  ASSIGN_MOP(z, JLB)
void E_JLB (Opcode*) {
	E_JLB_(SignExtend(GetCodeByte()));
}
// 0221  ASSIGN_MOP(z, JGEB)
void E_JGEB (Opcode*) {
	E_JGEB_(SignExtend(GetCodeByte()));
}
// 0222  ASSIGN_MOP(z, JGB)
void E_JGB (Opcode*) {
	E_JGB_(SignExtend(GetCodeByte()));
}
// 0223  ASSIGN_MOP(z, JLEB)
void E_JLEB (Opcode*) {
	E_JLEB_(SignExtend(GetCodeByte()));
}
// 0224  ASSIGN_MOP(z, JULB)
void E_JULB (Opcode*) {
	E_JULB_(SignExtend(GetCodeByte()));
}
// 0225  ASSIGN_MOP(z, JUGEB)
void E_JUGEB (Opcode*) {
	E_JUGEB_(SignExtend(GetCodeByte()));
}
// 0226  ASSIGN_MOP(z, JUGB)
void E_JUGB (Opcode*) {
	E_JUGB_(SignExtend(GetCodeByte()));
}
// 0227  ASSIGN_MOP(z, JULEB)
void E_JULEB (Opcode*) {
	E_JULEB_(SignExtend(GetCodeByte()));
}


// 0230  ASSIGN_MOP(z, JZ3)
JZn(3)
// 0231  ASSIGN_MOP(z, JZ4)
JZn(4)
// 0232  ASSIGN_MOP(z, JZB)
void E_JZB (Opcode*) {
	E_JZ_(SignExtend(GetCodeByte()));
}
// 0233  ASSIGN_MOP(z, JNZ3)
JNZn(3)
// 0234  ASSIGN_MOP(z, JNZ4)
JNZn(4)
// 0235  ASSIGN_MOP(z, JNZB)
void E_JNZB (Opcode*) {
	E_JNZ_(SignExtend(GetCodeByte()));
}
// 0236  ASSIGN_MOP(z, JDEB)
void E_JDEB (Opcode*) {
	E_JDE_(SignExtend(GetCodeByte()));
}
// 0237  ASSIGN_MOP(z, JDNEB)
void E_JMDEB (Opcode*) {
	E_JDNE_(SignExtend(GetCodeByte()));
}

// 0240  ASSIGN_MOP(z, JIB)
void E_JIB (Opcode*) {
	E_JIB_(GetCodeWord());
}
// 0241  ASSIGN_MOP(z, JIW)
void E_JIW (Opcode*) {
	E_JIW_(GetCodeWord());
}
// 0242  ASSIGN_MOP(z, REC)
void E_REC(Opcode*) {
	E_REC_();
}
// 0243  ASSIGN_MOP(z, REC2)
void E_REC2(Opcode*) {
	E_REC2_();
}
// 0244  ASSIGN_MOP(z, DIS)
void E_DIS(Opcode*) {
	E_DIS_();
}
// 0245  ASSIGN_MOP(z, DIS2)
void E_DIS2(Opcode*) {
	E_DIS2_();
}
// 0246  ASSIGN_MOP(z, EXCH)
void E_EXCH(Opcode*) {
	E_EXCH_();
}
// 0247  ASSIGN_MOP(z, DEXCH)
void E_DEXCH(Opcode*) {
	E_DEXCH_();
}



// 0250  ASSIGN_MOP(z, DUP)
void E_DUP(Opcode*) {
	E_DUP_();
}
// 0251  ASSIGN_MOP(z, DDUP)
void E_DDUP(Opcode*) {
	E_DDUP_();
}
// 0252  ASSIGN_MOP(z, EXDIS)
void E_EXDIS(Opcode*) {
	E_EXDIS_();
}
// 0253  ASSIGN_MOP(z, NEG)
void E_NEG(Opcode*) {
	E_NEG_();
}
// 0254  ASSIGN_MOP(z, INC)
void E_INC(Opcode*) {
	E_INC_();
}
// 0255  ASSIGN_MOP(z, DEC)
void E_DEC(Opcode*) {
	E_DEC_();
}
// 0256  ASSIGN_MOP(z, DINC)
void E_DINC(Opcode*) {
	E_DINC_();
}
// 0257  ASSIGN_MOP(z, DBL)
void E_DBL(Opcode*) {
	E_DBL_();
}



// 0260  ASSIGN_MOP(z, DDBL)
void E_DDBL(Opcode*) {
	E_DDBL_();
}
// 0261  ASSIGN_MOP(z, TRPL)
void E_TRPL(Opcode*) {
	E_TRPL_();
}
// 0262  ASSIGN_MOP(z, AND)
void E_AND(Opcode*) {
	E_AND_();
}
// 0263  ASSIGN_MOP(z, IOR)
void E_IOR(Opcode*) {
	E_IOR_();
}
// 0264  ASSIGN_MOP(z, ADDSB)
void E_ADDSB (Opcode*) {
	E_ADDSB_(SignExtend(GetCodeByte()));
}
// 0265  ASSIGN_MOP(z, ADD)
void E_ADD(Opcode*) {
	E_ADD_();
}
// 0266  ASSIGN_MOP(z, SUB)
void E_SUB(Opcode*) {
	E_SUB_();
}
// 0267  ASSIGN_MOP(z, DADD)
void E_DADD(Opcode*) {
	E_DADD_();
}



// 0270  ASSIGN_MOP(z, DSUB)
void E_DSUB(Opcode*) {
	E_DSUB_();
}
// 0271  ASSIGN_MOP(z, ADC)
void E_ADC(Opcode*) {
	E_ADC_();
}
// 0272  ASSIGN_MOP(z, ACD)
void E_ACD(Opcode*) {
	E_ACD_();
}
// 0273  ASSIGN_MOP(z, AL0IB)
void E_AL0IB(Opcode*) {
	E_AL0IB_(GetCodeByte());
}
// 0274  ASSIGN_MOP(z, MUL)
void E_MUL(Opcode*) {
	E_MUL_();
}
// 0275  ASSIGN_MOP(z, DCMP)
void E_DCMP(Opcode*) {
	E_DCMP_();
}
// 0276  ASSIGN_MOP(z, UDCMP)
void E_UDCMP(Opcode*) {
	E_UDCMP_();
}
// 0277  ASSIGN_MOP(z, VMFIND)

