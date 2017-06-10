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
static log4cpp::Category& logger = Logger::getLogger("1xx");

#include "../util/Debug.h"
#include "../util/Perf.h"

#include "../mesa/MesaThread.h"

#include "Opcode.h"
#include "Interpreter.h"


__attribute__((always_inline)) static inline void E_R_(CARD16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  R %3d", savedPC, arg);
	POINTER ptr = Pop();
	CARD16 *p = FetchMds(ptr + arg);
	// NO PAGE FAULT AFTER HERE
	Push(*p);
}
#define Rn(n) \
void E_R##n (Opcode*) { \
	E_R_(n); \
}


__attribute__((always_inline)) static inline void E_RL_(CARD16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RL %3d", savedPC, arg);
	LONG_POINTER ptr = PopLong();
	CARD16 *p = Fetch(ptr + arg);
	// NO PAGE FAULT AFTER HERE
	Push(*p);
}
#define RLn(n) \
void E_RL##n (Opcode*) { \
	E_RL_(n); \
}


__attribute__((always_inline)) static inline void E_RD_(CARD16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RD %3d", savedPC, arg);
	POINTER ptr = Pop() + arg;
	CARD16 *p0 = FetchMds(ptr + 0);
	CARD16 *p1 = FetchMds(ptr + 1);
	// NO PAGE FAULT AFTER HERE
	UNSPEC u = *p0;
	UNSPEC v = *p1;
	Push(u);
	Push(v);
}
#define RDn(n) \
void E_RD##n (Opcode*) { \
	E_RD_(n); \
}


__attribute__((always_inline)) static inline void E_RDL_(CARD16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  PDL %3d", savedPC, arg);
	LONG_POINTER ptr = PopLong() + arg;
	CARD16 *p0 = Fetch(ptr + 0);
	CARD16 *p1 = Fetch(ptr + 1);
	// NO PAGE FAULT AFTER HERE
	UNSPEC u = *p0;
	UNSPEC v = *p1;
	Push(u);
	Push(v);
}
#define RDLn(n) \
void E_RDL##n (Opcode*) { \
	E_RDL_(n); \
}


__attribute__((always_inline)) static inline void E_W_(CARD16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  W %3d", savedPC, arg);
	POINTER ptr = Pop();
	CARD16* p = StoreMds(ptr + arg);
	// NO PAGE FAULT AFTER HERE
	*p = Pop();
}
#define Wn(n) \
void E_W##n (Opcode*) { \
	E_W_(n); \
}


__attribute__((always_inline)) static inline void E_PS_(CARD16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  PS %3d", savedPC, arg);
	UNSPEC u = Pop();
	POINTER ptr = Pop();
	CARD16* p = StoreMds(ptr + arg);
	// NO PAGE FAULT AFTER HERE
	*p = u;
	SP++; //Recover();
}


__attribute__((always_inline)) static inline void E_WL_(CARD16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  WL %3d", savedPC, arg);
	LONG_POINTER ptr = PopLong();
	CARD16* p = Store(ptr + arg);
	// NO PAGE FAULT AFTER HERE
	*p = Pop();
}


__attribute__((always_inline)) static inline void E_PSL_(CARD16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  PSL %3d", savedPC, arg);
	UNSPEC u = Pop();
	LONG_POINTER ptr = PopLong();
	CARD16* p = Store(ptr + arg);
	// NO PAGE FAULT AFTER HERE
	*p = u;
	SP++; //Recover();
	SP++; //Recover();
}


__attribute__((always_inline)) static inline void E_WD_(CARD16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  WD %3d", savedPC, arg);
	POINTER ptr = Pop() + arg;
	CARD16 *p0 = StoreMds(ptr + 0);
	CARD16 *p1 = StoreMds(ptr + 1);
	// NO PAGE FAULT AFTER HERE
	*p1 = Pop();
	*p0 = Pop();
}


__attribute__((always_inline)) static inline void E_PSD_(CARD16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  PSD %3d", savedPC, arg);
	UNSPEC v = Pop();
	UNSPEC u = Pop();
	POINTER ptr = Pop() + arg;
	CARD16 *p0 = StoreMds(ptr + 0);
	CARD16 *p1 = StoreMds(ptr + 1);
	// NO PAGE FAULT AFTER HERE
	*p1 = v;
	*p0 = u;
	SP++; //Recover();
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// 0100  ASSIGN_MOP(z, R0)
Rn(0)
// 0101  ASSIGN_MOP(z, R1)
Rn(1)
// 0102  ASSIGN_MOP(z, RB)
void E_RB (Opcode*) {
	E_R_(GetCodeByte());
}
// 0103  ASSIGN_MOP(z, RL0)
RLn(0)
// 0104  ASSIGN_MOP(z, RLB)
void E_RLB (Opcode*) {
	E_RL_(GetCodeByte());
}
// 0105  ASSIGN_MOP(z, RD0)
RDn(0)
// 0106  ASSIGN_MOP(z, RDB)
void E_RDB (Opcode*) {
	E_RD_(GetCodeByte());
}
// 0107  ASSIGN_MOP(z, RDL0)
RDLn(0)



// 0110  ASSIGN_MOP(z, RDLB)
void E_RDLB (Opcode*) {
	E_RDL_(GetCodeByte());
}
// 0111  ASSIGN_MOP(z, W0)
Wn(0)
// 0112  ASSIGN_MOP(z, WB)
void E_WB (Opcode*) {
	E_W_(GetCodeByte());
}
// 0113  ASSIGN_MOP(z, PSB)
void E_PSB (Opcode*) {
	E_PS_(GetCodeByte());
}
// 0114  ASSIGN_MOP(z, WLB)
void E_WLB (Opcode*) {
	E_WL_(GetCodeByte());
}
// 0115  ASSIGN_MOP(z, PSLB)
void E_PSLB (Opcode*) {
	E_PSL_(GetCodeByte());
}
// 0116  ASSIGN_MOP(z, WDB)
void E_WDB (Opcode*) {
	E_WD_(GetCodeByte());
}
// 0117  ASSIGN_MOP(z, PSD0)
void E_PSD0 (Opcode*) {
	E_PSD_(0);
}

// 0120  ASSIGN_MOP(z, PSDB)
void E_PSDB (Opcode*) {
	E_PSD_(GetCodeByte());
}
// 0121  ASSIGN_MOP(z, WDLB)
// 0122  ASSIGN_MOP(z, PSDLB)
// 0123  ASSIGN_MOP(z, RLI00)
// 0124  ASSIGN_MOP(z, RLI01)
// 0125  ASSIGN_MOP(z, RLI02)
// 0126  ASSIGN_MOP(z, RLI03)
// 0127  ASSIGN_MOP(z, RLIP)

// 0130  ASSIGN_MOP(z, RLILP)
// 0131  ASSIGN_MOP(z, RLDI00)
// 0132  ASSIGN_MOP(z, RLDIP)
// 0133  ASSIGN_MOP(z, RLDILP)
// 0134  ASSIGN_MOP(z, RGIP)
// 0135  ASSIGN_MOP(z, RGILP)
// 0136  ASSIGN_MOP(z, WLIP)
// 0137  ASSIGN_MOP(z, WLILP)

// 0140  ASSIGN_MOP(z, WLDILP)
// 0141  ASSIGN_MOP(z, RS)
// 0142  ASSIGN_MOP(z, RLS)
// 0143  ASSIGN_MOP(z, WS)
// 0144  ASSIGN_MOP(z, WLS)
// 0145  ASSIGN_MOP(z, R0F)
// 0146  ASSIGN_MOP(z, RF)
// 0147  ASSIGN_MOP(z, RL0F)

// 0150  ASSIGN_MOP(z, RLF)
// 0151  ASSIGN_MOP(z, RLFS)
// 0152  ASSIGN_MOP(z, RLIPF)
// 0153  ASSIGN_MOP(z, RLILPF)
// 0154  ASSIGN_MOP(z, W0F)
// 0155  ASSIGN_MOP(z, WF)
// 0156  ASSIGN_MOP(z, PSF)
// 0157  ASSIGN_MOP(z, PS0F)

// 0160  ASSIGN_MOP(z, WS0F)
// 0161  ASSIGN_MOP(z, WL0F)
// 0162  ASSIGN_MOP(z, WLF)
// 0163  ASSIGN_MOP(z, PSLF)
// 0164  ASSIGN_MOP(z, WLFS)
// 0165  ASSIGN_MOP(z, SLDB)
// 0166  ASSIGN_MOP(z, SGDB)
// 0167  ASSIGN_MOP(z, LLKB)

// 0170  ASSIGN_MOP(z, RKIB)
// 0171  ASSIGN_MOP(z, RKDIB)
// 0172  ASSIGN_MOP(z, LKB)
// 0173  ASSIGN_MOP(z, SHIFT)
// 0174  ASSIGN_MOP(z, SHIFTSB)
// 0175  //ASSIGN_MOP(z, MBP)
// 0176  //ASSIGN_MOP(z, RBP)
// 0177  //ASSIGN_MOP(z, WBP)
