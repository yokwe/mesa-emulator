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
static log4cpp::Category& logger = Logger::getLogger("3xx");

#include "../util/Debug.h"
#include "../util/Perf.h"

#include "../mesa/MesaThread.h"

#include "Opcode.h"
#include "Interpreter.h"



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_LI_(CARD16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LI %5d", savedPC, arg);
	Push(arg);
}
#define LIn(n) \
void E_LI##n (Opcode*) { \
	E_LI_(n); \
}
// 0300  ASSIGN_MOP(z, LI0)
LIn(0)
// 0301  ASSIGN_MOP(z, LI1)
LIn(1)
// 0302  ASSIGN_MOP(z, LI2)
LIn(2)
// 0303  ASSIGN_MOP(z, LI3)
LIn(3)
// 0304  ASSIGN_MOP(z, LI4)
LIn(4)
// 0305  ASSIGN_MOP(z, LI5)
LIn(5)
// 0306  ASSIGN_MOP(z, LI6)
LIn(6)
// 0307  ASSIGN_MOP(z, LI7)
LIn(7)
// 0310  ASSIGN_MOP(z, LI8)
LIn(8)
// 0311  ASSIGN_MOP(z, LI9)
LIn(9)
// 0312  ASSIGN_MOP(z, LI10)
LIn(10)
// 0313  ASSIGN_MOP(z, LIN1)
void E_LIN1(Opcode*) {
	E_LI_((CARD16)0xffff);
}
// 0314  ASSIGN_MOP(z, LINI)
void E_LINI(Opcode*) {
	E_LI_((CARD16)0x8000);
}
// 0315  ASSIGN_MOP(z, LIB)
void E_LIB(Opcode*) {
	E_LI_(GetCodeByte());
}
// 0316  ASSIGN_MOP(z, LIW)
void E_LIW(Opcode*) {
	E_LI_(GetCodeWord());
}
// 0317  ASSIGN_MOP(z, LINB)
void E_LINB(Opcode*) {
	E_LI_((CARD16)(0xff00 | GetCodeByte()));
}
// 0320  ASSIGN_MOP(z, LIHB)
void E_LIHB(Opcode*) {
	E_LI_((CARD16)(GetCodeByte() << 8));
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_LID_(CARD32 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LID %5d", savedPC, arg);
	PushLong(arg);
}
// 0321  ASSIGN_MOP(z, LID0)
void E_LID0(Opcode*) {
	E_LID_(0);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_LA_(CARD16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LA %5d", savedPC, arg);
	Push(LFCache::LF() + arg);
}
#define LAn(n) \
void E_LA##n (Opcode*) { \
	E_LA_(n); \
}
// 0322  ASSIGN_MOP(z, LA0)
LAn(0)
// 0323  ASSIGN_MOP(z, LA1)
LAn(1)
// 0324  ASSIGN_MOP(z, LA2)
LAn(2)
// 0325  ASSIGN_MOP(z, LA3)
LAn(3)
// 0326  ASSIGN_MOP(z, LA6)
LAn(6)
// 0327  ASSIGN_MOP(z, LA8)
LAn(8)
// 0330  ASSIGN_MOP(z, LAB)
void E_LAB(Opcode*) {
	E_LA_(GetCodeByte());
}
// 0331  ASSIGN_MOP(z, LAW)
void E_LAW(Opcode*) {
	E_LA_(GetCodeWord());
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_GA_(CARD16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LA %5d", savedPC, arg);
	Push((CARD16)(GF + arg));
}
#define GAn(n) \
void E_GA##n (Opcode*) { \
	E_GA_(n); \
}
// 0332  ASSIGN_MOP(z, GA0)
GAn(0)
// 0333  ASSIGN_MOP(z, GA1)
GAn(1)
// 0334  ASSIGN_MOP(z, GAB)
void E_GAB(Opcode*) {
	E_GA_(GetCodeByte());
}
// 0335  ASSIGN_MOP(z, GAW)
void E_GAW(Opcode*) {
	E_GA_(GetCodeWord());
}
// 0336  //ASSIGN_MOP(z, CAW)
// 0337  ASSIGN_MOP(z, EFC0)

// 0340  ASSIGN_MOP(z, EFC1)
// 0341  ASSIGN_MOP(z, EFC2)
// 0342  ASSIGN_MOP(z, EFC3)
// 0343  ASSIGN_MOP(z, EFC4)
// 0344  ASSIGN_MOP(z, EFC5)
// 0345  ASSIGN_MOP(z, EFC6)
// 0346  ASSIGN_MOP(z, EFC7)
// 0347  ASSIGN_MOP(z, EFC8)

// 0350  ASSIGN_MOP(z, EFC9)
// 0351  ASSIGN_MOP(z, EFC10)
// 0352  ASSIGN_MOP(z, EFC11)
// 0353  ASSIGN_MOP(z, EFC12)
// 0354  ASSIGN_MOP(z, EFCB)
// 0355  ASSIGN_MOP(z, LFC)
// 0356  ASSIGN_MOP(z, SFC)
// 0357  ASSIGN_MOP(z, RET)

// 0360  ASSIGN_MOP(z, KFCB)
// 0361  ASSIGN_MOP(z, ME)
// 0362  ASSIGN_MOP(z, MX)
// 0363  ASSIGN_MOP(z, BLT)
// 0364  ASSIGN_MOP(z, BLTL)
// 0365  ASSIGN_MOP(z, BLTC)
// 0366  ASSIGN_MOP(z, BLTCL)
// 0367  ASSIGN_MOP(z, LP)
void E_LP(Opcode*) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LP", savedPC);
	POINTER ptr = Pop();
	PushLong((ptr == 0) ? 0 : LengthenPointer(ptr));
}
// 0370  ASSIGN_MOP(z, ESC)
void E_ESC(Opcode*) {
	Interpreter::dispatchEsc(GetCodeByte());
}
// 0371  ASSIGN_MOP(z, ESCL)
void E_ESCL(Opcode*) {
	Interpreter::dispatchEsc(GetCodeByte());
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_LGA_(CARD16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LGA %5d", savedPC, arg);
	PushLong(GF + arg);
}
// 0372  ASSIGN_MOP(z, LGA0)
void E_LGA0(Opcode*) {
	E_LGA_(0);
}
// 0373  ASSIGN_MOP(z, LGAB)
void E_LGAB(Opcode*) {
	E_LGA_(GetCodeByte());
}
// 0374  ASSIGN_MOP(z, LGAW)
void E_LGAW(Opcode*) {
	E_LGA_(GetCodeWord());
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__attribute__((always_inline)) static inline void E_DESC_(CARD16 arg) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DESC %02X", savedPC, arg);
	Push(GFI | 0x0003);
	Push(arg);
}
// 0375  ASSIGN_MOP(z, DESC)
void E_DESC(Opcode*) {
	E_DESC_(GetCodeWord());
}
// 0376
// 0377  ASSIGN_MOP(z, RESRVD)
void E_RESRVD(Opcode*) {
	logger.fatal("TRACE %6o  RESRVD", savedPC);
	ERROR();
}
