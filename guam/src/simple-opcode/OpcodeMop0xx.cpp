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
static log4cpp::Category& logger = Logger::getLogger("0xx");

#include "../util/Debug.h"
#include "../util/Perf.h"

#include "../mesa/MesaThread.h"

#include "Opcode.h"
#include "Interpreter.h"


// 00  ASSIGN_MOP(z, NOOP)
void E_NOOP(Opcode* /*opcode*/) {
	logger.fatal("TRACE %6o  NOOP", savedPC);
	ERROR();
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_LL_(CARD16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LL %3d", savedPC, arg);
	CARD16 *p = FetchLF(arg);
	// NO PAGE FAULT AFTER HERE
	Push(*p);
}
#define LLn(n) \
void E_LL##n (Opcode*) { \
	E_LL_(n); \
}
// 01  ASSIGN_MOP(z, LL0)
LLn(0)
// 02  ASSIGN_MOP(z, LL1)
LLn(1)
// 03  ASSIGN_MOP(z, LL2)
LLn(2)
// 04  ASSIGN_MOP(z, LL3)
LLn(3)
// 05  ASSIGN_MOP(z, LL4)
LLn(4)
// 06  ASSIGN_MOP(z, LL5)
LLn(5)
// 07  ASSIGN_MOP(z, LL6)
LLn(6)
// 010  ASSIGN_MOP(z, LL7)
LLn(7)
// 011  ASSIGN_MOP(z, LL8)
LLn(8)
// 012  ASSIGN_MOP(z, LL9)
LLn(9)
// 013  ASSIGN_MOP(z, LL10)
LLn(10)
// 014  ASSIGN_MOP(z, LL11)
LLn(11)
// 015  ASSIGN_MOP(z, LLB)
void E_LLB (Opcode*) {
	E_LL_(GetCodeByte());
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_LLD_(CARD16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLD %3d", savedPC, arg);
	POINTER ptr = arg;
	CARD16 *p0 = FetchLF(ptr + 0);
	CARD16 *p1 = FetchLF(ptr + 1);
	// NO PAGE FAULT AFTER HERE
	Push(*p0);
	Push(*p1);
}
#define LLDn(n) \
void E_LLD##n (Opcode*) { \
	E_LLD_(n); \
}
// 016  ASSIGN_MOP(z, LLD0)
LLDn(0)
// 017  ASSIGN_MOP(z, LLD1)
LLDn(1)
// 020  ASSIGN_MOP(z, LLD2)
LLDn(2)
// 021  ASSIGN_MOP(z, LLD3)
LLDn(3)
// 022  ASSIGN_MOP(z, LLD4)
LLDn(4)
// 023  ASSIGN_MOP(z, LLD5)
LLDn(5)
// 024  ASSIGN_MOP(z, LLD6)
LLDn(6)
// 025  ASSIGN_MOP(z, LLD7)
LLDn(7)
// 026  ASSIGN_MOP(z, LLD8)
LLDn(8)
// 027  ASSIGN_MOP(z, LLD10)
LLDn(10)
// 030  ASSIGN_MOP(z, LLDB)
void E_LLDB (Opcode*) {
	E_LLD_(GetCodeByte());
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_SL_(CARD16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  SL %3d", savedPC, arg);
	CARD16 *p = StoreLF(arg);
	// NO PAGE FAULT AFTER HERE
	*p = Pop();
}
#define SLn(n) \
void E_SL##n (Opcode*) { \
	E_SL_(n); \
}
// 031  ASSIGN_MOP(z, SL0)
SLn(0)
// 032  ASSIGN_MOP(z, SL1)
SLn(1)
// 033  ASSIGN_MOP(z, SL2)
SLn(2)
// 034  ASSIGN_MOP(z, SL3)
SLn(3)
// 035  ASSIGN_MOP(z, SL4)
SLn(4)
// 036  ASSIGN_MOP(z, SL5)
SLn(5)
// 037  ASSIGN_MOP(z, SL6)
SLn(6)
// 040  ASSIGN_MOP(z, SL7)
SLn(7)
// 041  ASSIGN_MOP(z, SL8)
SLn(8)
// 042  ASSIGN_MOP(z, SL9)
SLn(9)
// 043  ASSIGN_MOP(z, SL10)
SLn(10)
// 044  ASSIGN_MOP(z, SLB)
void E_SLB (Opcode*) {
	E_SL_(GetCodeByte());
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_SLD_(CARD16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  SLD %3d", savedPC, arg);
	POINTER ptr = arg;
	CARD16 *p0 = StoreLF(ptr + 0);
	CARD16 *p1 = StoreLF(ptr + 1);
	// NO PAGE FAULT AFTER HERE
	*p1 = Pop();
	*p0 = Pop();
}
#define SLDn(n) \
void E_SLD##n (Opcode*) { \
	E_SLD_(n); \
}
// 045  ASSIGN_MOP(z, SLD0)
SLDn(0)
// 046  ASSIGN_MOP(z, SLD1)
SLDn(1)
// 047  ASSIGN_MOP(z, SLD2)
SLDn(2)
// 050  ASSIGN_MOP(z, SLD3)
SLDn(3)
// 051  ASSIGN_MOP(z, SLD4)
SLDn(4)
// 052  ASSIGN_MOP(z, SLD5)
SLDn(5)
// 053  ASSIGN_MOP(z, SLD6)
SLDn(6)
// 054  ASSIGN_MOP(z, SLD8)
SLDn(8)
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_PL_(CARD16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  PL %3d", savedPC, arg);
	CARD16 *p = StoreLF(arg);
	// NO PAGE FAULT AFTER HERE
	*p = Pop();
	SP++; //Recover();
}
#define PLn(n) \
void E_PL##n (Opcode*) { \
	E_PL_(n); \
}
// 055  ASSIGN_MOP(z, PL0)
PLn(0)
// 056  ASSIGN_MOP(z, PL1)
PLn(1)
// 057  ASSIGN_MOP(z, PL2)
PLn(2)
// 060  ASSIGN_MOP(z, PL3)
PLn(3)
// 061  ASSIGN_MOP(z, PLB)
void E_PLB (Opcode*) {
	E_PL_(GetCodeByte());
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_PLD_(CARD16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  PLD %3d", savedPC, arg);
	POINTER ptr = arg;
	CARD16 *p0 = StoreLF(ptr + 0);
	CARD16 *p1 = StoreLF(ptr + 1);
	// NO PAGE FAULT AFTER HERE
	*p1 = Pop();
	*p0 = Pop();
	SP++; //Recover();
	SP++; //Recover();
}
// 062  ASSIGN_MOP(z, PLD0)
void E_PLD0 (Opcode*) {
	E_PLD_(0);
}
// 063  ASSIGN_MOP(z, PLDB)
void E_PLDB (Opcode*) {
	E_PLD_(GetCodeByte());
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_LG_(CARD16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LG %3d", savedPC, arg);
	CARD16 *p = Fetch(GF + arg);
	// NO PAGE FAULT AFTER HERE
	Push(*p);
}
#define LGn(n) \
void E_LG##n (Opcode*) { \
	E_LG_(n); \
}
// 064  ASSIGN_MOP(z, LG0)
LGn(0)
// 065  ASSIGN_MOP(z, LG1)
LGn(1)
// 066  ASSIGN_MOP(z, LG2)
LGn(2)
// 067  ASSIGN_MOP(z, LGB)
LGn(3)
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_LGD_(CARD16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LGD %3d", savedPC, arg);
	LONG_POINTER ptr = GF + arg;
	CARD16 *p0 = Fetch(ptr + 0);
	CARD16 *p1 = Fetch(ptr + 1);
	// NO PAGE FAULT AFTER HERE
	Push(*p0);
	Push(*p1);
}
#define LGDn(n) \
void E_LGD##n (Opcode*) { \
	E_LGD_(n); \
}
// 070  ASSIGN_MOP(z, LGD0)
LGDn(0)
// 071  ASSIGN_MOP(z, LGD2)
LGDn(2)
// 072  ASSIGN_MOP(z, LGDB)
void E_LGDB (Opcode*) {
	E_LGD_(GetCodeByte());
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// 073  ASSIGN_MOP(z, SGB)
__attribute__((always_inline)) static inline void E_SG_(CARD16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  SG %3d", savedPC, arg);
	CARD16 *p = Store(GF + arg);
	// NO PAGE FAULT AFTER HERE
	*p = Pop();
}
void E_SGB (Opcode*) {
	E_SG_(GetCodeByte());
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// 074  ASSIGN_MOP(z, BNDCK)
void E_BNDCK(Opcode*) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  BNDCK", savedPC);
	CARDINAL range = Pop();
	CARDINAL index = Pop();
	Push(index);
	if (range <= index) BoundsTrap();
}
// 075  ASSIGN_MOP(z, BRK)
void E_BRK(Opcode*) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  BRK", savedPC);
	if (breakByte == 0) BreakTrap();
	Interpreter::dispatchMop(breakByte);
	breakByte = 0;
}
// 077  ASSIGN_MOP(z, STC)
