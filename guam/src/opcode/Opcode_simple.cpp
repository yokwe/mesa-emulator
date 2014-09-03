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
// Opcode_simple.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("simple");

#include "../util/Debug.h"
#include "../util/GuiOp.h"
#include "../util/Perf.h"

#include "../mesa/MesaThread.h"

#include "Opcode.h"
#include "Interpreter.h"
#include "HotSpot.h"


__attribute__((always_inline)) static inline void R_NOOP_(Run /*run*/) {
	logger.fatal("TRACE %6o  NOOP", savedPC);
	ERROR();
}
DEF_R(NOOP)

__attribute__((always_inline)) static inline void R_LL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LL %3d", savedPC, run.getArg());
	Push(*FetchLF(run.getArg()));
}
DEF_R(LL)

__attribute__((always_inline)) static inline void R_LLD_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLD %3d", savedPC, run.getArg());
	POINTER ptr = run.getArg();
	CARD16 *p0 = FetchLF(ptr + 0);
	CARD16 *p1 = FetchLF(ptr + 1);
	Push(*p0);
	Push(*p1);
}
DEF_R(LLD)

__attribute__((always_inline)) static inline void R_SL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  SL %3d", savedPC, run.getArg());
	*StoreLF(run.getArg()) = Pop();
}
DEF_R(SL)

__attribute__((always_inline)) static inline void R_SLD_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  SLD %3d", savedPC, run.getArg());
	POINTER ptr = run.getArg();
	CARD16 *p0 = StoreLF(ptr + 0);
	CARD16 *p1 = StoreLF(ptr + 1);
	*p1 = Pop();
	*p0 = Pop();
}
DEF_R(SLD)

__attribute__((always_inline)) static inline void R_PL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  PL %3d", savedPC, run.getArg());
	*StoreLF(run.getArg()) = Pop();
	//Recover();
	SP++;
}
DEF_R(PL)

__attribute__((always_inline)) static inline void R_PLD_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  PLD %3d", savedPC, run.getArg());
	POINTER ptr = run.getArg();
	CARD16 *p0 = StoreLF(ptr + 0);
	CARD16 *p1 = StoreLF(ptr + 1);
	*p1 = Pop();
	*p0 = Pop();
	//Recover();
	SP++;
	//Recover();
	SP++;
}
DEF_R(PLD)

__attribute__((always_inline)) static inline void R_LG_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LG %3d", savedPC, run.getArg());
	Push(*Fetch(GF + run.getArg()));
}
DEF_R(LG)

__attribute__((always_inline)) static inline void R_LGD_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LGD %3d", savedPC, run.getArg());
	LONG_POINTER ptr = GF + run.getArg();
	CARD16 *p0 = Fetch(ptr);
	CARD16 *p1 = ((ptr & (PageSize - 1)) == (PageSize - 1)) ? Fetch(ptr + 1) : p0 + 1;
	Push(*p0);
	Push(*p1);
}
DEF_R(LGD)

__attribute__((always_inline)) static inline void R_SG_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  SG %3d", savedPC, run.getArg());
	*Store(GF + run.getArg()) = Pop();
}
DEF_R(SG)

__attribute__((always_inline)) static inline void R_BNDCK_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  BNDCK", savedPC);
	CARDINAL range = Pop();
	CARDINAL index = Pop();
	Push(index);
	if (range <= index) BoundsTrap();
}
DEF_R(BNDCK)

__attribute__((always_inline)) static inline void R_BRK_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  BRK", savedPC);
	if (breakByte == 0) BreakTrap();
	Interpreter::setOpcode(breakByte);
	Interpreter::dispatchMop();
	breakByte = 0;
}
DEF_R(BRK)

__attribute__((always_inline)) static inline void R_R_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  R %3d", savedPC, run.getArg());
	POINTER ptr = Pop();
	Push(*FetchMds(ptr + run.getArg()));
}
DEF_R(R)

__attribute__((always_inline)) static inline void R_RL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RL %3d", savedPC, run.getArg());
	LONG_POINTER ptr = PopLong();
	Push(*Fetch(ptr + run.getArg()));
}
DEF_R(RL)

__attribute__((always_inline)) static inline void R_RD_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RD %3d", savedPC, run.getArg());
	POINTER ptr = Pop() + run.getArg();
	CARD16 *p0 = FetchMds(ptr);
	CARD16 *p1 = ((ptr & (PageSize - 1)) == (PageSize - 1)) ? FetchMds(ptr + 1) : p0 + 1;
	UNSPEC u = *p0;
	UNSPEC v = *p1;
	Push(u);
	Push(v);
}
DEF_R(RD)

__attribute__((always_inline)) static inline void R_RDL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  PDL %3d", savedPC, run.getArg());
	LONG_POINTER ptr = PopLong() + run.getArg();
	CARD16 *p0 = Fetch(ptr);
	CARD16 *p1 = ((ptr & (PageSize - 1)) == (PageSize - 1)) ? Fetch(ptr + 1) : p0 + 1;
	UNSPEC u = *p0;
	UNSPEC v = *p1;
	Push(u);
	Push(v);
}
DEF_R(RDL)

__attribute__((always_inline)) static inline void R_W_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  W %3d", savedPC, run.getArg());
	POINTER ptr = Pop();
	*StoreMds(ptr + run.getArg()) = Pop();
}
DEF_R(W)

__attribute__((always_inline)) static inline void R_PS_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  PS %3d", savedPC, run.getArg());
	UNSPEC u = Pop();
	POINTER ptr = Pop();
	*StoreMds(ptr + run.getArg()) = u;
	//Recover();
	SP++;
}
DEF_R(PS)

__attribute__((always_inline)) static inline void R_WL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  WL %3d", savedPC, run.getArg());
	LONG_POINTER ptr = PopLong();
	*Store(ptr + run.getArg()) = Pop();
}
DEF_R(WL)

__attribute__((always_inline)) static inline void R_PSL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  PSL %3d", savedPC, run.getArg());
	UNSPEC u = Pop();
	LONG_POINTER ptr = PopLong();
	*Store(ptr + run.getArg()) = u;
	//Recover();
	SP++;
	//Recover();
	SP++;
}
DEF_R(PSL)

__attribute__((always_inline)) static inline void R_WD_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  WD %3d", savedPC, run.getArg());
	POINTER ptr = Pop() + run.getArg();
	CARD16 *p0 = StoreMds(ptr);
	CARD16 *p1 = ((ptr & (PageSize - 1)) == (PageSize - 1)) ? StoreMds(ptr + 1) : p0 + 1;
	*p1 = Pop();
	*p0 = Pop();
}
DEF_R(WD)

__attribute__((always_inline)) static inline void R_PSD_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  PSD %3d", savedPC, run.getArg());
	UNSPEC v = Pop();
	UNSPEC u = Pop();
	POINTER ptr = Pop() + run.getArg();
	CARD16 *p0 = StoreMds(ptr);
	CARD16 *p1 = ((ptr & (PageSize - 1)) == (PageSize - 1)) ? StoreMds(ptr + 1) : p0 + 1;
	*p1 = v;
	*p0 = u;
	//Recover();
	SP++;
}
DEF_R(PSD)

__attribute__((always_inline)) static inline void R_WDL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  WDL %3d", savedPC, run.getArg());
	LONG_POINTER ptr = PopLong() + run.getArg();
	CARD16 *p0 = Store(ptr);
	CARD16 *p1 = ((ptr & (PageSize - 1)) == (PageSize - 1)) ? Store(ptr + 1) : p0 + 1;
	*p1 = Pop();
	*p0 = Pop();
}
DEF_R(WDL)

__attribute__((always_inline)) static inline void R_PSDL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  PSDL %3d", savedPC, run.getArg());
	UNSPEC v = Pop();
	UNSPEC u = Pop();
	LONG_POINTER ptr = PopLong() + run.getArg();
	CARD16 *p0 = Store(ptr);
	CARD16 *p1 = ((ptr & (PageSize - 1)) == (PageSize - 1)) ? Store(ptr + 1) : p0 + 1;
	*p1 = v;
	*p0 = u;
	//Recover();
	SP++;
	//Recover();
	SP++;
}
DEF_R(PSDL)

__attribute__((always_inline)) static inline void R_RLIL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RLIL %3d %3d", savedPC, run.getArg0(), run.getArg1());
	LONG_POINTER ptr = ReadDblLF(run.getArg0()) + run.getArg1();
	Push(*Fetch(ptr));
}
DEF_R(RLIL)

__attribute__((always_inline)) static inline void R_RLDI_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RLDI %3d %3d", savedPC, run.getArg0(), run.getArg1());
	POINTER ptr = *FetchLF(run.getArg0()) + run.getArg1();
	CARD16 *p0 = FetchMds(ptr);
	CARD16 *p1 = ((ptr & (PageSize - 1)) == (PageSize - 1)) ? FetchMds(ptr + 1) : p0 + 1;
	UNSPEC u = *p0;
	UNSPEC v = *p1;
	Push(u);
	Push(v);
}
DEF_R(RLDI)

__attribute__((always_inline)) static inline void R_RLI_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RLI %3d %3d", savedPC, run.getArg0(), run.getArg1());
	POINTER ptr = *FetchLF(run.getArg0());
	Push(*FetchMds(ptr + run.getArg1()));
}
DEF_R(RLI)

__attribute__((always_inline)) static inline void R_RLDIL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RLDIL %3d %3d", savedPC, run.getArg0(), run.getArg1());
	LONG_POINTER ptr = ReadDblLF(run.getArg0()) + run.getArg1();
	CARD16 *p0 = Fetch(ptr);
	CARD16 *p1 = ((ptr & (PageSize - 1)) == (PageSize - 1)) ? Fetch(ptr + 1) : p0 + 1;
	UNSPEC u = *p0;
	UNSPEC v = *p1;
	Push(u);
	Push(v);
}
DEF_R(RLDIL)

__attribute__((always_inline)) static inline void R_RGI_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RGI %3d %3d", savedPC, run.getArg0(), run.getArg1());
	POINTER ptr = *Fetch(GF + run.getArg0()) + run.getArg1();
	Push(*FetchMds(ptr));
}
DEF_R(RGI)

__attribute__((always_inline)) static inline void R_RGIL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RGIL %3d %3d", savedPC, run.getArg0(), run.getArg1());
	LONG_POINTER ptr = ReadDbl(GF + run.getArg0()) + run.getArg1();
	Push(*Fetch(ptr));
}
DEF_R(RGIL)

__attribute__((always_inline)) static inline void R_WLI_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  WLI %3d %3d", savedPC, run.getArg0(), run.getArg1());
	POINTER ptr = *FetchLF(run.getArg0()) + run.getArg1();
	*StoreMds(ptr) = Pop();
}
DEF_R(WLI)

__attribute__((always_inline)) static inline void R_WLIL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  WLIL %3d %3d", savedPC, run.getArg0(), run.getArg1());
	LONG_POINTER ptr = ReadDblLF(run.getArg0()) + run.getArg1();
	*Store(ptr) = Pop();
}
DEF_R(WLIL)

__attribute__((always_inline)) static inline void R_WLDIL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  WLDIL %3d %3d", savedPC, run.getArg0(), run.getArg1());
	LONG_POINTER ptr = ReadDblLF(run.getArg0()) + run.getArg1();
	CARD16 *p0 = Store(ptr);
	CARD16 *p1 = ((ptr & (PageSize - 1)) == (PageSize - 1)) ? Store(ptr + 1) : p0 + 1;
	*p1 = Pop();
	*p0 = Pop();
}
DEF_R(WLDIL)

__attribute__((always_inline)) static inline void R_RS_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  zRS %3d", savedPC, run.getArg());
	CARDINAL index = Pop();
	LONG_POINTER ptr = LengthenPointer(Pop());
	Push(FetchByte(ptr, run.getArg() + index));
}
DEF_R(RS)

__attribute__((always_inline)) static inline void R_RLS_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  zRLS %3d", savedPC, run.getArg());
	CARDINAL index = Pop();
	LONG_POINTER ptr = PopLong();
	Push(FetchByte(ptr, run.getArg() + index));
}
DEF_R(RLS)

__attribute__((always_inline)) static inline void R_WS_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  zWS %3d", savedPC, run.getArg());
	CARDINAL index = Pop();
	LONG_POINTER ptr = LengthenPointer(Pop());
	BYTE data = LowByte(Pop());
	StoreByte(ptr, run.getArg() + index, data);
}
DEF_R(WS)

__attribute__((always_inline)) static inline void R_WLS_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  zWLS %3d", savedPC, run.getArg());
	CARDINAL index = Pop();
	LONG_POINTER ptr = PopLong();
	BYTE data = LowByte(Pop());
	StoreByte(ptr, run.getArg() + index, data);
}
DEF_R(WLS)

__attribute__((always_inline)) static inline void RLFnm(CARD8 offset, CARD8 field) {
	LONG_POINTER ptr = PopLong();
	Push(ReadField(*Fetch(ptr + offset), field));
}
__attribute__((always_inline)) static inline void R_RLF_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RLF %3d %3d", savedPC, run.getArg0(), run.getArg1());
	RLFnm(run.getArg0(), run.getArg1());
}
DEF_R(RLF)

__attribute__((always_inline)) static inline void R_RF_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RF %3d %3d", savedPC, run.getArg0(), run.getArg1());
	POINTER ptr = Pop();
	Push(ReadField(*FetchMds(ptr + run.getArg0()), run.getArg1()));
}
DEF_R(RF)

__attribute__((always_inline)) static inline void R_RLFS_(Run /*run*/) {
	FieldDesc desc = {Pop()};
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RLFS %3d %3d", savedPC, desc.offset, desc.field);
	RLFnm(desc.offset, desc.field);
}
DEF_R(RLFS)

__attribute__((always_inline)) static inline void R_RLIPF_(Run run) {
	NibblePair pair = {run.getArg0()};
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RLIPF %2d %2d %3d", savedPC, pair.left, pair.right, run.getArg1());
	POINTER ptr = *FetchLF(pair.left);
	Push(ReadField(*FetchMds(ptr + pair.right), run.getArg1()));
}
DEF_R(RLIPF)

__attribute__((always_inline)) static inline void R_RLILPF_(Run run) {
	NibblePair pair = {run.getArg0()};
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RLILPF %2d %2d %3d", savedPC, pair.left, pair.right, run.getArg1());
	LONG_POINTER ptr = ReadDblLF(pair.left);
	Push(ReadField(*Fetch(ptr + pair.right), run.getArg1()));
}
DEF_R(RLILPF)

__attribute__((always_inline)) static inline void R_WF_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  WF %3d %3d", savedPC, run.getArg0(), run.getArg1());
	POINTER ptr = Pop() + run.getArg0();
	UNSPEC data = Pop();
	CARD16 *p = StoreMds(ptr);
	*p = WriteField(*p, run.getArg1(), data);
}
DEF_R(WF)

__attribute__((always_inline)) static inline void R_PSF_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  PSF %3d %3d", savedPC, run.getArg0(), run.getArg1());
	UNSPEC data = Pop();
	POINTER ptr = Pop() + run.getArg0();
	CARD16 *p = StoreMds(ptr);
	*p = WriteField(*p, run.getArg1(), data);
	Recover();
}
DEF_R(PSF)

__attribute__((always_inline)) static inline void R_WSF_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  WSF %3d %3d", savedPC, run.getArg0(), run.getArg1());
	UNSPEC data = Pop();
	POINTER ptr = Pop() + run.getArg0();
	CARD16 *p = StoreMds(ptr);
	*p = WriteField(*p, (CARD8)run.getArg1(), data);
}
DEF_R(WSF)

__attribute__((always_inline)) static inline void WLFnm(CARD16 offset, CARD8 field) {
	LONG_POINTER ptr = PopLong() + offset;
	UNSPEC data = Pop();
	CARD16 *p = Store(ptr);
	*p = WriteField(*p, field, data);
}
__attribute__((always_inline)) static inline void R_WLF_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  WLF %3d %3d", savedPC, run.getArg0(), run.getArg1());
	WLFnm(run.getArg0(), run.getArg1());
}
DEF_R(WLF)

__attribute__((always_inline)) static inline void R_PSLF_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  PSLF %3d %3d", savedPC, run.getArg0(), run.getArg1());
	UNSPEC data = Pop();
	LONG_POINTER ptr = PopLong() + run.getArg0();
	CARD16 *p = Store(ptr);
	*p = WriteField(*p, run.getArg1(), data);
	Recover();
	Recover();
}
DEF_R(PSLF)

__attribute__((always_inline)) static inline void R_WLFS_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  WLFS", savedPC);
	FieldDesc desc = {Pop()};
	WLFnm(desc.offset, desc.field);
}
DEF_R(WLFS)

__attribute__((always_inline)) static inline void R_SGD_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  SGD %3d", savedPC, run.getArg());
	LONG_POINTER ptr = GF + run.getArg();
	CARD16 *p0 = Fetch(ptr);
	CARD16 *p1 = ((ptr & (PageSize - 1)) == (PageSize - 1)) ? Fetch(ptr + 1) : p0 + 1;
	*p1 = Pop();
	*p0 = Pop();
}
DEF_R(SGD)

__attribute__((always_inline)) static inline void R_LLK_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLK %3d", savedPC, run.getArg());
	PushLong(FetchLink(run.getArg()));
}
DEF_R(LLK)

__attribute__((always_inline)) static inline void R_RKI_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RKI %02X", savedPC, run.getArg());
	LONG_POINTER ptr = FetchLink(run.getArg());
	Push(*Fetch(ptr));
}
DEF_R(RKI)

__attribute__((always_inline)) static inline void R_RKDI_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RKDI %02X", savedPC, run.getArg());
	LONG_POINTER ptr = FetchLink(run.getArg());
	CARD16 *p0 = Fetch(ptr);
	CARD16 *p1 = ((ptr & (PageSize - 1)) == (PageSize - 1)) ? Fetch(ptr + 1) : p0 + 1;
	Push(*p0);
	Push(*p1);
}
DEF_R(RKDI)

__attribute__((always_inline)) static inline void R_LK_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LK %02X", savedPC, run.getArg());
	Recover();
	ShortControlLink link = Pop();
	*StoreLF(0) = link - run.getArg();
}
DEF_R(LK)

__attribute__((always_inline)) static inline void R_SHIFT_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  SHIFT", savedPC);
	INT16 shift = Pop();
	UNSPEC u = Pop();
	Push(Shift(u, shift));
}
DEF_R(SHIFT)

__attribute__((always_inline)) static inline void R_SHIFTI_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  SHIFTI %3d", savedPC, (INT16)run.getArg());
	UNSPEC u = Pop();
	Push(Shift(u, (INT16)run.getArg()));
}
DEF_R(SHIFTI)

__attribute__((always_inline)) static inline void R_CATCH_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  CATCH %3d", savedPC, run.getArg());
}
DEF_R(CATCH)

__attribute__((always_inline)) static inline void R_J_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  J %5d", savedPC, (INT16)run.getArg());
	PC = savedPC + (INT16)run.getArg();
}
DEF_R(J)

__attribute__((always_inline)) static inline void R_JUE_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  zJUE %3d %3d", savedPC, run.getArg0(), run.getArg1());
	UNSPEC data = Pop();
	if (data == run.getArg0()) PC = savedPC + SignExtend(run.getArg1());
}
DEF_R(JUE)

__attribute__((always_inline)) static inline void R_JEB_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JEB %3d", savedPC, (INT16)run.getArg());
	UNSPEC v = Pop();
	UNSPEC u = Pop();
	if (u == v) PC = savedPC + run.getArg();
}
DEF_R(JEB)

__attribute__((always_inline)) static inline void R_JUNE_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JUNE %3d %3d", savedPC, run.getArg0(), run.getArg1());
	UNSPEC data = Pop();
	if (data != run.getArg0()) PC = savedPC + SignExtend(run.getArg1());
}
DEF_R(JUNE)

__attribute__((always_inline)) static inline void R_JNEB_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JNEB %3d", savedPC, (INT16)run.getArg());
	UNSPEC v = Pop();
	UNSPEC u = Pop();
	if (u != v) PC = savedPC + run.getArg();
}
DEF_R(JNEB)

__attribute__((always_inline)) static inline void R_JLB_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JLB %3d", savedPC, (INT16)run.getArg());
	INT16 k = Pop();
	INT16 j = Pop();
	if (j < k) PC = savedPC + run.getArg();
}
DEF_R(JLB)

__attribute__((always_inline)) static inline void R_JGEB_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JGEB %3d", savedPC, (INT16)run.getArg());
	INT16 k = Pop();
	INT16 j = Pop();
	if (j >= k) PC = savedPC + run.getArg();
}
DEF_R(JGEB)

__attribute__((always_inline)) static inline void R_JGB_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JGB %3d", savedPC, (INT16)run.getArg());
	INT16 k = Pop();
	INT16 j = Pop();
	if (j > k) PC = savedPC + run.getArg();
}
DEF_R(JGB)

__attribute__((always_inline)) static inline void R_JLEB_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JLEB %3d", savedPC, (INT16)run.getArg());
	INT16 k = Pop();
	INT16 j = Pop();
	if (j <= k) PC = savedPC + run.getArg();
}
DEF_R(JLEB)

__attribute__((always_inline)) static inline void R_JULB_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  zJULB %3d", savedPC, (INT16)run.getArg());
	CARDINAL v = Pop();
	CARDINAL u = Pop();
	if (u < v) PC = savedPC + run.getArg();
}
DEF_R(JULB)

__attribute__((always_inline)) static inline void R_JUGEB_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JUGEB %3d", savedPC, (INT16)run.getArg());
	CARDINAL v = Pop();
	CARDINAL u = Pop();
	if (u >= v) PC = savedPC + run.getArg();
}
DEF_R(JUGEB)

__attribute__((always_inline)) static inline void R_JUGB_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JUGB %3d", savedPC, (INT16)run.getArg());
	CARDINAL v = Pop();
	CARDINAL u = Pop();
	if (u > v) PC = savedPC + run.getArg();
}
DEF_R(JUGB)

__attribute__((always_inline)) static inline void R_JULEB_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JULEB %3d", savedPC, (INT16)run.getArg());
	CARDINAL v = Pop();
	CARDINAL u = Pop();
	if (u <= v) PC = savedPC + run.getArg();
}
DEF_R(JULEB)

__attribute__((always_inline)) static inline void JZn(CARD16 n) {
	UNSPEC u = Pop();
	if (u == 0) PC = savedPC + n;
}
__attribute__((always_inline)) static inline void R_JZ_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JZ %5d", savedPC, (INT16)run.getArg());
	JZn(run.getArg());
}
DEF_R(JZ)

__attribute__((always_inline)) static inline void JNZn(CARD16 n) {
	UNSPEC u = Pop();
	if (u != 0) PC = savedPC + n;
}
__attribute__((always_inline)) static inline void R_JNZ_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JZ %5d", savedPC, (INT16)run.getArg());
	JNZn(run.getArg());
}
DEF_R(JNZ)

__attribute__((always_inline)) static inline void R_JDE_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JDE %5d", savedPC, (INT16)run.getArg());
	LONG_UNSPEC v = PopLong();
	LONG_UNSPEC u = PopLong();
	if (u == v) PC = savedPC + run.getArg();
}
DEF_R(JDE)

__attribute__((always_inline)) static inline void R_JDNE_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JDNE %5d", savedPC, (INT16)run.getArg());
	LONG_UNSPEC v = PopLong();
	LONG_UNSPEC u = PopLong();
	if (u != v) PC = savedPC + run.getArg();
}
DEF_R(JDNE)

__attribute__((always_inline)) static inline void R_JIB_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JIB %5d", savedPC, run.getArg());
	CARDINAL limit = Pop();
	CARDINAL index = Pop();
	if (index < limit) {
		BytePair disp = {ReadCode(run.getArg() + index / 2)};
		PC = savedPC + (((index % 2) == 0) ? disp.left : disp.right);
	}
}
DEF_R(JIB)

__attribute__((always_inline)) static inline void R_JIW_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JIW %5d", savedPC, run.getArg());
	CARDINAL limit = Pop();
	CARDINAL index = Pop();
	if (index < limit) {
		CARDINAL disp = ReadCode(run.getArg() + index);
		PC = savedPC + disp;
	}
}
DEF_R(JIW)

__attribute__((always_inline)) static inline void R_REC_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  REC", savedPC);
	Recover();
}
DEF_R(REC)

__attribute__((always_inline)) static inline void R_REC2_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  REC2", savedPC);
	Recover();
	Recover();
}
DEF_R(REC2)

__attribute__((always_inline)) static inline void R_DIS_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DIS", savedPC);
	Discard();
}
DEF_R(DIS)

__attribute__((always_inline)) static inline void R_DIS2_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DIS2", savedPC);
	Discard();
	Discard();
}
DEF_R(DIS2)

__attribute__((always_inline)) static inline void R_EXCH_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  EXCH", savedPC);
	UNSPEC v = Pop();
	UNSPEC u = Pop();
	Push(v);
	Push(u);
}
DEF_R(EXCH)

__attribute__((always_inline)) static inline void R_DEXCH_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DEXCH", savedPC);
	LONG_UNSPEC v = PopLong();
	LONG_UNSPEC u = PopLong();
	PushLong(v);
	PushLong(u);
}
DEF_R(DEXCH)

__attribute__((always_inline)) static inline void R_DUP_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DUP", savedPC);
	UNSPEC u = Pop();
	Push(u);
	Push(u);
}
DEF_R(DUP)

__attribute__((always_inline)) static inline void R_DDUP_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DDUP", savedPC);
	LONG_UNSPEC u = PopLong();
	PushLong(u);
	PushLong(u);
}
DEF_R(DDUP)

__attribute__((always_inline)) static inline void R_EXDIS_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  EXDIS", savedPC);
	UNSPEC u = Pop();
	/*UNSPEC v = */ Pop();
	Push(u);
}
DEF_R(EXDIS)

__attribute__((always_inline)) static inline void R_NEG_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  NEG", savedPC);
	if (SP == 0) STACK_ERROR();
	stack[SP - 1] = -stack[SP - 1];
	//INT16 i = Pop();
	//Push(-i);
}
DEF_R(NEG)

__attribute__((always_inline)) static inline void R_INC_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  INC", savedPC);
	if (SP == 0) STACK_ERROR();
	stack[SP - 1]++;
	//CARDINAL s = Pop();
	//Push(s + 1);
}
DEF_R(INC)

__attribute__((always_inline)) static inline void R_DEC_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DEC", savedPC);
	if (SP == 0) STACK_ERROR();
	stack[SP - 1]--;
	//CARDINAL s = Pop();
	//Push(s - 1);
}
DEF_R(DEC)

__attribute__((always_inline)) static inline void R_DINC_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DINC", savedPC);
	if (SP < 2) STACK_ERROR();
	stack[SP - 2] += 1;
	if (stack[SP - 2] == 0) stack[SP - 1] += 1;
	//LONG_CARDINAL s = PopLong();
	//PushLong(s + 1);
}
DEF_R(DINC)

__attribute__((always_inline)) static inline void R_DBL_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DBL", savedPC);
	if (SP == 0) STACK_ERROR();
	stack[SP - 1] *= 2;
	//UNSPEC u = Pop();
	//Push(u << 1);
}
DEF_R(DBL)

__attribute__((always_inline)) static inline void R_DDBL_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DDBL", savedPC);
	LONG_UNSPEC u = PopLong();
	PushLong(u << 1);
}
DEF_R(DDBL)

__attribute__((always_inline)) static inline void R_TRPL_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  TRPL", savedPC);
	if (SP == 0) STACK_ERROR();
	stack[SP - 1] *= 3;
	//CARDINAL s = Pop();
	//Push(s * 3);
}
DEF_R(TRPL)

__attribute__((always_inline)) static inline void R_AND_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  AND", savedPC);
	UNSPEC v = Pop();
	UNSPEC u = Pop();
	Push(u & v);
}
DEF_R(AND)

__attribute__((always_inline)) static inline void R_IOR_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  IOR", savedPC);
	UNSPEC v = Pop();
	UNSPEC u = Pop();
	Push(u | v);
}
DEF_R(IOR)

__attribute__((always_inline)) static inline void R_ADDI_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  ADDI %02X", savedPC, (INT16)run.getArg());
	CARD16 i = Pop();
	Push(i + run.getArg());
}
DEF_R(ADDI)

__attribute__((always_inline)) static inline void R_ADD_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  ADD", savedPC);
	CARDINAL t = Pop();
	CARDINAL s = Pop();
	Push(s + t);
}
DEF_R(ADD)

__attribute__((always_inline)) static inline void R_SUB_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  SUB", savedPC);
	CARDINAL t = Pop();
	CARDINAL s = Pop();
	Push(s - t);
}
DEF_R(SUB)

__attribute__((always_inline)) static inline void R_DADD_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DADD", savedPC);
	LONG_CARDINAL t = PopLong();
	LONG_CARDINAL s = PopLong();
	PushLong(s + t);
}
DEF_R(DADD)

__attribute__((always_inline)) static inline void R_DSUB_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DSUB", savedPC);
	LONG_CARDINAL t = PopLong();
	LONG_CARDINAL s = PopLong();
	PushLong(s - t);
}
DEF_R(DSUB)

__attribute__((always_inline)) static inline void R_ADC_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  ADC", savedPC);
	CARDINAL t = Pop();
	LONG_CARDINAL s = PopLong();
	PushLong(s + t);
}
DEF_R(ADC)

__attribute__((always_inline)) static inline void R_ACD_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  ACD", savedPC);
	LONG_CARDINAL t = PopLong();
	CARDINAL s = Pop();
	PushLong(s + t);
}
DEF_R(ACD)

__attribute__((always_inline)) static inline void R_AL0I_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  AL0IB %02X", savedPC, run.getArg());
	Push(*FetchLF(0) + run.getArg());
}
DEF_R(AL0I)

__attribute__((always_inline)) static inline void R_MUL_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  MUL", savedPC);
	CARDINAL t = Pop();
	CARDINAL s = Pop();
	PushLong((LONG_CARDINAL)s * t);
	//Discard();
	SP--;
}
DEF_R(MUL)

__attribute__((always_inline)) static inline void R_DCMP_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DCMP", savedPC);
	INT32 k = PopLong();
	INT32 j = PopLong();
	Push((k == j) ? 0 : ((j > k) ? 1 : -1));
}
DEF_R(DCMP)

__attribute__((always_inline)) static inline void R_UDCMP_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  UDCMP", savedPC);
	LONG_CARDINAL t = PopLong();
	LONG_CARDINAL s = PopLong();
	Push((s == t) ? 0 : ((s > t) ? 1 : -1));
}
DEF_R(UDCMP)


//RunBase: TYPE = LONG BASE POINTER TO ARRAY IndexRun OF Run;
//PRun: TYPE = RunBase RELATIVE ORDERED POINTER [0..LAST[CARDINAL]] TO Run;
//Find: PROCEDURE [page: VM.PageNumber, rBase: RunBase, pRunTop: PRun]
//  RETURNS [found: BOOLEAN, pRun: PRun] = MACHINE CODE {MopcodesExtras2.zVMFIND};
__attribute__((always_inline)) static inline void R_VMFIND_(Run /*run*/) {
	CARD16 pRunTop = Pop();
	CARD32 rBase   = PopLong();
	CARD32 page    = PopLong();
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  VMFIND  %06X  %08X  %04X", savedPC, page, rBase, pRunTop);

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
DEF_R(VMFIND)

__attribute__((always_inline)) static inline void R_LI_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LI %5d", savedPC, run.getArg());
	Push(run.getArg());
}
DEF_R(LI)

__attribute__((always_inline)) static inline void R_LID_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LID %5d", savedPC, run.getArg());
	PushLong(run.getArg());
}
DEF_R(LID)

__attribute__((always_inline)) static inline void R_LA_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LA %5d", savedPC, run.getArg());
	Push(LFCache::LF() + run.getArg());
}
DEF_R(LA)

__attribute__((always_inline)) static inline void R_GA_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LA %5d", savedPC, run.getArg());
	Push((CARD16)(GF + run.getArg()));
}
DEF_R(GA)

__attribute__((always_inline)) static inline void R_LP_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LP", savedPC);
	POINTER ptr = Pop();
	PushLong((ptr == 0) ? 0 : LengthenPointer(ptr));
}
DEF_R(LP)

__attribute__((always_inline)) static inline void R_ESC_(Run /*run*/) {
	Interpreter::setOpcode(GetCodeByte());
	Interpreter::dispatchEsc();
}
DEF_R(ESC)

__attribute__((always_inline)) static inline void R_LGA_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LGA %5d", savedPC, run.getArg());
	PushLong(GF + run.getArg());
}
DEF_R(LGA)

__attribute__((always_inline)) static inline void R_DESC_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DESC %02X", savedPC, run.getArg());
	Push(GFI | 0x0003);
	Push(run.getArg());
}
DEF_R(DESC)

__attribute__((always_inline)) static inline void R_RESRVD_(Run /*run*/) {
	logger.fatal("TRACE %6o  RESRVD", savedPC);
	ERROR();
}
DEF_R(RESRVD)

//
// ESC
//

__attribute__((always_inline)) static inline void R_SM_(Run /*run*/) {
	Memory::Map map;
	map.mf.u = Pop();
	map.rp = PopLong();
	CARD32 vp = PopLong();
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  SM   vp = %08X  mf = %04X  rp = %08X", savedPC, vp, map.mf.u, map.rp);
	if (Vacant(map.mf)) map.rp = 0;
	Memory::WriteMap(vp, map);
}
DEF_R(SM)

__attribute__((always_inline)) static inline void R_GMF_(Run /*run*/) {
	CARD32 vp = PopLong();
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  GMF  vp = %08X", savedPC, vp);
	Memory::Map map = Memory::ReadMap(vp);
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  GMF  rp = %08X  mf = %04X", savedPC, map.rp, map.mf.u);
	if (Vacant(map.mf)) map.rp = 0;
	Push(map.mf.u);
	PushLong(map.rp);
}
DEF_R(GMF)

__attribute__((always_inline)) static inline void R_SMF_(Run /*run*/) {
	MapFlags newMF = {Pop()};
	CARD32 vp = PopLong();
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  SMF  vp = %08X  mf = %04X", savedPC, vp, newMF.u);
	Memory::Map map = Memory::ReadMap(vp);
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  SMF  rp = %08X  mf = %04X", savedPC, map.rp, map.mf.u);
	Push(map.mf.u);
	PushLong(map.rp);
	if (!Vacant(map.mf)) {
		map.mf = newMF;
		Memory::WriteMap(vp, map);
	}
}
DEF_R(SMF)

__attribute__((always_inline)) static inline void R_DI_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DI  %3d", savedPC, InterruptThread::getWDC());
	if (InterruptThread::getWDC() == cWDC) InterruptError();
	InterruptThread::disable();
}
DEF_R(DI)

__attribute__((always_inline)) static inline void R_EI_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  EI  %3d", savedPC, InterruptThread::getWDC());
	if (InterruptThread::getWDC() == 0) InterruptError();
	InterruptThread::enable();
}
DEF_R(EI)

__attribute__((always_inline)) static inline void R_XOR_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  XOR", savedPC);
	UNSPEC v = Pop();
	UNSPEC u = Pop();
	Push(u ^ v);
}
DEF_R(XOR)

__attribute__((always_inline)) static inline void R_DAND_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DAND", savedPC);
	LONG_UNSPEC v = PopLong();
	LONG_UNSPEC u = PopLong();
	PushLong(u & v);
}
DEF_R(DAND)

__attribute__((always_inline)) static inline void R_DIOR_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DIOR", savedPC);
	LONG_UNSPEC v = PopLong();
	LONG_UNSPEC u = PopLong();
	PushLong(u | v);
}
DEF_R(DIOR)

__attribute__((always_inline)) static inline void R_DXOR_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DXOR", savedPC);
	LONG_UNSPEC v = PopLong();
	LONG_UNSPEC u = PopLong();
	PushLong(u ^ v);
}
DEF_R(DXOR)

__attribute__((always_inline)) static inline void R_ROTATE_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  ROTATE", savedPC);
	INT16 rotate = Pop();
	UNSPEC u = Pop();
	Push(Rotate(u, rotate));
}
DEF_R(ROTATE)

__attribute__((always_inline)) static inline void R_DSHIFT_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DSHIFT", savedPC);
	INT16 rotate = Pop();
	LONG_UNSPEC u = PopLong();
	PushLong(LongShift(u, rotate));
}
DEF_R(DSHIFT)

__attribute__((always_inline)) static inline void R_LINT_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LINT", savedPC);
	INT16 i = Pop();
	Push(i);
	Push((i < 0) ? (CARD16)0xffff : 0);
}
DEF_R(LINT)

__attribute__((always_inline)) static inline void R_JS_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  JS", savedPC);
	PC = Pop();
}
DEF_R(JS)

__attribute__((always_inline)) static inline void R_RCFS_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RCFS", savedPC);
	FieldDesc desc = {Pop()};
	CARDINAL offset = Pop();
	CARD8 spec = desc.field;
	Push(ReadField(ReadCode(offset + desc.offset), spec));
}
DEF_R(RCFS)

__attribute__((always_inline)) static inline void R_RC_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RC %02X", savedPC, run.getArg());
	CARDINAL offset = Pop();
	Push(ReadCode(offset + run.getArg()));
}
DEF_R(RC)

__attribute__((always_inline)) static inline void R_UDIV_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  UDIV", savedPC);
	CARDINAL t = Pop();
	CARDINAL s = Pop();
	if (t == 0) DivZeroTrap();
	Push(s / t);
	Push(s % t);
	Discard();
}
DEF_R(UDIV)

__attribute__((always_inline)) static inline void R_LUDIV_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LUDIV", savedPC);
	CARDINAL t = Pop();
	LONG_CARDINAL s = PopLong();
	if (t == 0) DivZeroTrap();
	if (t <= (CARDINAL)(s >> 16)) DivCheckTrap();
	Push(LowHalf(s / t));
	Push(LowHalf(s % t));
	Discard();
}
DEF_R(LUDIV)

__attribute__((always_inline)) static inline void R_ROB_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  ROB %02X", savedPC, run.getArg());
	POINTER ptr = Pop();
	Push(*FetchMds(ptr - run.getArg()));
}
DEF_R(ROB)

__attribute__((always_inline)) static inline void R_WOB_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  WOB %02X", savedPC, run.getArg());
	POINTER ptr = Pop();
	*StoreMds(ptr - run.getArg()) = Pop();
}
DEF_R(WOB)

__attribute__((always_inline)) static inline void R_DSK_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DSK %02X", savedPC, run.getArg());
	POINTER state = LFCache::LF() + run.getArg();
	SaveStack(LengthenPointer(state));
}
DEF_R(DSK)

__attribute__((always_inline)) static inline void R_LSK_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LSK %02X", savedPC, run.getArg());
	POINTER state = LFCache::LF() + run.getArg();
	LoadStack(LengthenPointer(state));
}
DEF_R(LSK)

__attribute__((always_inline)) static inline void R_BNDCKL_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  BNDCKL", savedPC);
	LONG_CARDINAL range = PopLong();
	LONG_CARDINAL index = PopLong();
	PushLong(index);
	if (range <= index) BoundsTrap();
}
DEF_R(BNDCKL)

__attribute__((always_inline)) static inline void R_NILCK_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  NILCK", savedPC);
	POINTER ptr = Pop();
	Push(ptr);
	if (ptr == 0) PointerTrap();
}
DEF_R(NILCK)

__attribute__((always_inline)) static inline void R_NILCKL_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  NILCKL", savedPC);
	LONG_POINTER ptr = PopLong();
	PushLong(ptr);
	if (ptr == 0) PointerTrap();
}
DEF_R(NILCKL)

__attribute__((always_inline)) static inline void R_DMUL_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  DMUL", savedPC);
	LONG_CARDINAL t = PopLong();
	LONG_CARDINAL s = PopLong();
	PushLong((LONG_CARDINAL)(s * t));
}
DEF_R(DMUL)

__attribute__((always_inline)) static inline void R_SDIV_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  SDIV", savedPC);
	INT16 k = Pop();
	INT16 j = Pop();
	if (k == 0) DivZeroTrap();
	Push(j / k);
	Push(j % k);
	Discard();
}
DEF_R(SDIV)

__attribute__((always_inline)) static inline void R_SDDIV_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  SDDIV", savedPC);
	INT32 k = PopLong();
	INT32 j = PopLong();
	if (k == 0) DivZeroTrap();
	PushLong(j / k);
	PushLong(j % k);
	Discard();
	Discard();
}
DEF_R(SDDIV)

__attribute__((always_inline)) static inline void R_UDDIV_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  UDDIV", savedPC);
	LONG_CARDINAL t = PopLong();
	LONG_CARDINAL s = PopLong();
	if (t == 0) DivZeroTrap();
	PushLong((CARD32)(s / t));
	PushLong((CARD32)(s % t));
	Discard();
	Discard();
}
DEF_R(UDDIV)

__attribute__((always_inline)) static inline void R_WRPSB_(Run /*run*/) {
	PSB = Index(Pop());
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  WRPSB  %4d", savedPC, PSB);
}
DEF_R(WRPSB)

__attribute__((always_inline)) static inline void R_WRMDS_(Run /*run*/) {
	CARD32 mds = Pop() << WordSize;
	MDSCache::setMDS(mds);
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  WRMDS  %04X", savedPC, (MDSCache::MDS() >> WordSize));
}
DEF_R(WRMDS)

__attribute__((always_inline)) static inline void R_WRWP_(Run /*run*/) {
	CARD16 newValue = Pop();
	InterruptThread::setWP(newValue);
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  WRWP   %04X", savedPC, newValue);
}
DEF_R(WRWP)

__attribute__((always_inline)) static inline void R_WRWDC_(Run /*run*/) {
	InterruptThread::setWDC(Pop());
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  WRWDC  %04X", savedPC, InterruptThread::getWDC());
}
DEF_R(WRWDC)

__attribute__((always_inline)) static inline void R_WRPTC_(Run /*run*/) {
	CARD16 newValue = Pop();
	TimerThread::setPTC(newValue);
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  WRPTC  %04X", savedPC, newValue);
//	lastTimeoutTime = Util::getMicroTime();
}
DEF_R(WRPTC)

__attribute__((always_inline)) static inline void R_WRIT_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  WRIT", savedPC);
	ERROR();
}
DEF_R(WRIT)

__attribute__((always_inline)) static inline void R_WRXTS_(Run /*run*/) {
	XTS = Pop();
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  WRXTS  %04X", savedPC, XTS);
}
DEF_R(WRXTS)

__attribute__((always_inline)) static inline void R_WRMP_(Run /*run*/) {
	MP = Pop();
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  WRMP   %4d", savedPC, MP);
	GuiOp::setMP(MP);

	if (DEBUG_STOP_MESSAGE_UNTIL_MP == MP) {
		Logger::popPriority();
	}
	switch(MP) {
	case 900:
		//  cGerm: Code = 900;  -- Germ entered
		logger.info("MP  900 cGerm");
		break;
	case 910:
		//  cGermAction: Code = 910;  -- Germ action running (e.g. inLoad, outLoad)
		logger.info("MP  910 cGermAction");
		break;
	case 917:
		// cRespondingToEtherDebugger: Code = 917;  -- talking to ethernet debugger
		logger.info("MP  917 cRespondingToEtherDebugger");
		break;
	case 915:
		// cWaitingForEtherDebugger: Code = 915;  -- waiting for ethernet debugger to begin debugging me
		logger.info("MP  915 cWaitingForEtherDebugger");
		if (DEBUG_STOP_AT_MP_915) ERROR();
		break;
	case 919:
		//  cGermFinished: Code = 919;  -- Germ transferred control back to caller (who has hung)
		logger.info("MP  919 cGermFinished");
		break;
	case 920:
		//  cGermDriver: Code = 920;  -- Germ driver running (e.g. disk, ether, floppy)
		logger.info("MP  920 cGermDriver");
		break;
	case 921:
		//  cGermDeviceError: Code = 921;  -- hard error on device being booted
		logger.info("MP  921 cGermDeviceError");
		break;
	case 927:
		//  cGermShortBootFile: Code = 927;  -- boot file ends before it should (try reinstalling)
		logger.info("MP  927 cGermShortBootFile");
		break;
	case 930:
		//  cControl: Code = 930;  -- Pilot Control and MesaRuntime components being initialized
		logger.info("MP  930 cControl");
		break;
	case 937:
		// cTimeNotAvailable: Code = 937;  -- trying to get the time from either hardware clock or ethernet
		logger.info("MP  937 cTimeNotAvailable");
		break;
	case 938:
		// cCleanup: Code = 938;  -- running cleanup procedures, e.g. before going to debugger
		logger.info("MP  938 cCleanup");
		break;
	case 939:
		// cPowerOff: Code = 939;  -- ProcessorFace.PowerOff called but no power control relay
		logger.info("MP  939 cPowerOff");
		break;
	case 940:
		// cStorage: Code = 940;  -- Pilot Store component being initialized
		logger.info("MP  940 cStorage");
		break;
	case 960:
		// cDeleteTemps: Code = 960;  -- temporary files from previous run being deleted
		logger.info("MP  960 cDeleteTemps");
		break;
	case 970:
		// cMap: Code = 970;  -- client and other non-bootloaded code being mapped
		logger.info("MP  970 cMap");
		break;
	case 980:
		// cCommunication: Code = 980;  -- Pilot Communication component being initialized
		logger.info("MP  980 cCommunication");
		break;
	case 990:
		// cClient: Code = 990;  -- PilotClient.Run called
		logger.info("MP  990 cClient");
		break;
	default:
		logger.info("MP %04d", MP);
		if (perf_stop_at_mp_8000 && MP == 8000) ProcessorThread::stop();
		break;
	}
}
DEF_R(WRMP)

// aRRPSB - 0170
__attribute__((always_inline)) static inline void R_RRPSB_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RRPSB  %4d", savedPC, PSB);
	Push(Handle(PSB));
}
DEF_R(RRPSB)

__attribute__((always_inline)) static inline void R_RRMDS_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RRMDS  %04X", savedPC, (MDSCache::MDS() >> WordSize));
	Push((CARD16)(MDSCache::MDS() >> WordSize));
}
DEF_R(RRMDS)

__attribute__((always_inline)) static inline void R_RRWP_(Run /*run*/) {
	CARD16 value = InterruptThread::getWP();
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RRWP   %04X", savedPC, value);
	Push(value);
}
DEF_R(RRWP)

__attribute__((always_inline)) static inline void R_RRWDC_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RRWDC  %04X", savedPC, InterruptThread::getWDC());
	Push(InterruptThread::getWDC());
}
DEF_R(RRWDC)

__attribute__((always_inline)) static inline void R_RRPTC_(Run /*run*/) {
	CARD16 value = TimerThread::getPTC();
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RRPTC  %04X", savedPC, value);
	Push(value);
}
DEF_R(RRPTC)

__attribute__((always_inline)) static inline void R_RRIT_(Run /*run*/) {
	CARD32 time = Util::getMicroTime();
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RRIT   %08X", savedPC, time);
	PushLong(time);
}
DEF_R(RRIT)

__attribute__((always_inline)) static inline void R_RRXTS_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RRXTS  %04X", savedPC, XTS);
	Push(XTS);
}
DEF_R(RRXTS)

/////////////////////////////////////////////////////////////////////////////////

#define DEF_CI_BREAK(name) \
Run C_##name(Opcode* opcode_) { \
	return C_##name##_(opcode_); \
} \
void I_##name(Opcode* opcode) { \
	Run run = C_##name##_(opcode); \
	PC = savedPC + L_##name; \
	R_##name##_(run); \
	if (DEBUG_ENABLE_HOTSPOT) HotSpot::update_BREAK(); \
}

#define DEF_C_0_BREAK(name) \
__attribute__((always_inline)) static inline Run C_##name##_(Opcode *opcode) { \
	Run ret(r##name, L_##name); \
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s", savedPC, opcode->getName()); \
	return ret; \
} \
DEF_CI_BREAK(name)


#define DEF_CI_JUMP(name) \
Run C_##name(Opcode* opcode_) { \
	return C_##name##_(opcode_); \
} \
void I_##name(Opcode* opcode) { \
	Run run = C_##name##_(opcode); \
	PC = savedPC + L_##name; \
	R_##name##_(run); \
	if (DEBUG_ENABLE_HOTSPOT) HotSpot::update_JUMP(); \
}

#define DEF_CI_n_JUMP(name, n) \
Run C_##name##n(Opcode* opcode_) { \
	return C_##name##n##_(opcode_); \
} \
void I_##name##n(Opcode* opcode) { \
	Run run = C_##name##n##_(opcode); \
	PC = savedPC + L_##name##n; \
	R_##name##_(run); \
	if (DEBUG_ENABLE_HOTSPOT) HotSpot::update_JUMP(); \
}

#define DEF_CI_r_JUMP(name, r) \
Run C_##name(Opcode* opcode_) { \
	return C_##name##_(opcode_); \
} \
void I_##name(Opcode* opcode) { \
	Run run = C_##name##_(opcode); \
	PC = savedPC + L_##name; \
	R_##r##_(run); \
	if (DEBUG_ENABLE_HOTSPOT) HotSpot::update_JUMP(); \
}

#define DEF_C_0_n_JUMP(name, n) \
__attribute__((always_inline)) static inline Run C_##name##n##_(Opcode *opcode) { \
	Run ret(r##name, L_##name##n, (CARD16)(n)); \
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s", savedPC, opcode->getName()); \
	return ret; \
} \
DEF_CI_n_JUMP(name, n)

#define DEF_C_0_JUMP(name) \
__attribute__((always_inline)) static inline Run C_##name##_(Opcode *opcode) { \
	Run ret(r##name, L_##name); \
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s", savedPC, opcode->getName()); \
	return ret; \
} \
DEF_CI_JUMP(name)

#define DEF_C_SB_JUMP(name) \
__attribute__((always_inline)) static inline Run C_##name##B##_(Opcode *opcode) { \
	Run ret(r##name, L_##name##B, (CARD16)SignExtend(GetCodeByte())); \
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s %3d", savedPC, opcode->getName(), ret.getArg()); \
	return ret; \
} \
DEF_CI_n_JUMP(name, B)


#define DEF_C_W_JUMP(name) \
__attribute__((always_inline)) static inline Run C_##name##W##_(Opcode *opcode) { \
	Run ret(r##name, L_##name##W, (CARD16)GetCodeWord()); \
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s %5d", savedPC, opcode->getName(), ret.getArg()); \
	return ret; \
} \
DEF_CI_n_JUMP(name, W)


#define DEF_C_SB_N_JUMP(name) \
__attribute__((always_inline)) static inline Run C_##name##_(Opcode *opcode) { \
	Run ret(r##name, L_##name, (CARD16)SignExtend(GetCodeByte())); \
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s %3d", savedPC, opcode->getName(), ret.getArg()); \
	return ret; \
} \
DEF_CI_JUMP(name)


#define DEF_C_W_N_JUMP(name) \
__attribute__((always_inline)) static inline Run C_##name##_(Opcode *opcode) { \
	Run ret(r##name, L_##name, (CARD16)GetCodeWord()); \
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s %5d", savedPC, opcode->getName(), ret.getArg()); \
	return ret; \
} \
DEF_CI_JUMP(name)



// zNOOP - 00
DEF_C_0(NOOP)
// zLL0 - 01
DEF_C_0_n(LL, 0)
// zLL1 - 02
DEF_C_0_n(LL, 1)
// zLL2 - 03
DEF_C_0_n(LL, 2)
// zLL3 - 04
DEF_C_0_n(LL, 3)
// zLL4 - 05
DEF_C_0_n(LL, 4)
// zLL5 - 06
DEF_C_0_n(LL, 5)
// zLL6 - 07
DEF_C_0_n(LL, 6)


// zLL7 - 010
DEF_C_0_n(LL, 7)
// zLL8 - 011
DEF_C_0_n(LL, 8)
// zLL9 - 012
DEF_C_0_n(LL, 9)
// zLL10 - 013
DEF_C_0_n(LL, 10)
// zLL11 - 014
DEF_C_0_n(LL, 11)
// zLLB - 015
DEF_C_B(LL)
// zLLD0 - 016
DEF_C_0_n(LLD, 0)
// zLLD1 - 017
DEF_C_0_n(LLD, 1)


// zLLD2 - 020
DEF_C_0_n(LLD, 2)
// zLLD3 - 021
DEF_C_0_n(LLD, 3)
// zLLD4 - 022
DEF_C_0_n(LLD, 4)
// zLLD5 - 023
DEF_C_0_n(LLD, 5)
// zLLD6 - 024
DEF_C_0_n(LLD, 6)
// zLLD7 - 025
DEF_C_0_n(LLD, 7)
// zLLD8 - 026
DEF_C_0_n(LLD, 8)
// zLLD10 - 027
DEF_C_0_n(LLD, 10)


// zLLDB - 030
DEF_C_B(LLD)
// zSL0 - 031
DEF_C_0_n(SL, 0)
// zSL1 - 032
DEF_C_0_n(SL, 1)
// zSL2 - 033
DEF_C_0_n(SL, 2)
// zSL3 - 034
DEF_C_0_n(SL, 3)
// zSL4 - 035
DEF_C_0_n(SL, 4)
// zSL5 - 036
DEF_C_0_n(SL, 5)
// zSL6 - 037
DEF_C_0_n(SL, 6)


// zSL7 - 040
DEF_C_0_n(SL, 7)
// zSL8 - 041
DEF_C_0_n(SL, 8)
// zSL9 - 042
DEF_C_0_n(SL, 9)
// zSL10 - 043
DEF_C_0_n(SL, 10)
// zSLB - 044
DEF_C_B(SL)
// zSLD0 - 045
DEF_C_0_n(SLD, 0)
// zSLD1 - 046
DEF_C_0_n(SLD, 1)
// zSLD2 - 047
DEF_C_0_n(SLD, 2)


// zSLD3 - 050
DEF_C_0_n(SLD, 3)
// zSLD4 - 051
DEF_C_0_n(SLD, 4)
// zSLD5 - 052
DEF_C_0_n(SLD, 5)
// zSLD6 - 053
DEF_C_0_n(SLD, 6)
// zSLD8 - 054
DEF_C_0_n(SLD, 8)
// zPL0 - 055
DEF_C_0_n(PL, 0)
// zPL1 - 056
DEF_C_0_n(PL, 1)
// zPL2 - 057
DEF_C_0_n(PL, 2)


// zPL3 - 060
DEF_C_0_n(PL, 3)
// zPLB - 061
DEF_C_B(PL)
// zPLD0 - 062
DEF_C_0_n(PLD, 0)
// zPLDB - 063
DEF_C_B(PLD)
// zLG0 - 064
DEF_C_0_n(LG, 0)
// zLG1 - 065
DEF_C_0_n(LG, 1)
// zLG2 - 066
DEF_C_0_n(LG, 2)
// zLGB - 067
DEF_C_B(LG)


// zLGD0 - 070
DEF_C_0_n(LGD, 0)
// zLGD2 - 071
DEF_C_0_n(LGD, 2)
// zLGDB - 072
DEF_C_B(LGD)
// zSGB - 073
DEF_C_B(SG)
// zBNDCK - 074
DEF_C_0(BNDCK)
// zBRK - 075
DEF_C_0_BREAK(BRK)
//
// zSTC - 077
// void OP_STC() {} // MopcodesExtras.mesa


// zR0 - 0100
DEF_C_0_n(R, 0)
// zR1 - 0101
DEF_C_0_n(R, 1)
// zRB - 0102
DEF_C_B(R)
// zRL0 - 0103
DEF_C_0_n(RL, 0)
// zRLB - 0104
DEF_C_B(RL)
// zRD0 - 0105
DEF_C_0_n(RD, 0)
// zRDB - 0106
DEF_C_B(RD)
// zRDL0 - 0107
DEF_C_0_n(RDL, 0)


// zRDLB - 0110
DEF_C_B(RDL)
// zW0 - 0111
DEF_C_0_n(W, 0)
// zWB - 0112
DEF_C_B(W)
// zPSB - 0113
DEF_C_B(PS)
// zWLB - 0114
DEF_C_B(WL)
// zPSLB - 0115
DEF_C_B(PSL)
// zWDB - 0116
DEF_C_B(WD)
// zPSD0 - 0117
DEF_C_0_n(PSD, 0)


// zPSDB - 0120
DEF_C_B(PSD)
// zWDLB - 0121
DEF_C_B(WDL)
// zPSDLB - 0122
DEF_C_B(PSDL)
// zRLI00 - 0123
DEF_C_P_0_n(RLI, 0)
// zRLI01 - 0124
DEF_C_P_0_n(RLI, 1)
// zRLI02 - 0125
DEF_C_P_0_n(RLI, 2)
// zRLI03 - 0126
DEF_C_P_0_n(RLI, 3)
// zRLIP - 0127
DEF_C_P(RLI)


// zRLILP - 0130
DEF_C_P(RLIL)
// zRLDI00 - 0131
DEF_C_P_0_n(RLDI, 0)
// zRLDIP - 0132
DEF_C_P(RLDI)
// zRLDILP - 0133
DEF_C_P(RLDIL)
// zRGIP - 0134
DEF_C_P(RGI)
// zRGILP - 0135
DEF_C_P(RGIL)
// zWLIP - 0136
DEF_C_P(WLI)
// zWLILP - 0137
DEF_C_P(WLIL)


// zWLDILP - 0140
DEF_C_P(WLDIL)
// zRS - 0141
DEF_C_B_N(RS)
// zRLS - 0142
DEF_C_B_N(RLS)
// zWS - 0143
DEF_C_B_N(WS)
// zWLS - 0144
DEF_C_B_N(WLS)
// zR0F - 0145
__attribute__((always_inline)) static inline Run C_R0F_(Opcode *opcode) {
	Run ret(rRF, L_R0F, (CARD8)0, (CARD8)GetCodeByte());
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s %3d", savedPC, opcode->getName(), ret.getArg1());
	return ret;
}
DEF_CI_r(R0F, RF)


// zRF - 0146
DEF_C_F(RF)
// zRL0F - 0147
__attribute__((always_inline)) static inline Run C_RL0F_(Opcode *opcode) {
	Run ret(rRLF, L_RL0F, (CARD8)0, (CARD8)GetCodeByte());
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s %3d", savedPC, opcode->getName(), ret.getArg1());
	return ret;
}
DEF_CI_r(RL0F, RLF)

// zRLF - 0150
DEF_C_F(RLF)
// zRLFS - 0151
DEF_C_0(RLFS);
// zRLIPF - 0152
DEF_C_PF(RLIPF)
// zRLILPF - 0153
DEF_C_PF(RLILPF)
// zW0F - 0154
__attribute__((always_inline)) static inline Run C_W0F_(Opcode *opcode) {
	Run ret(rWF, L_W0F, (CARD8)0, (CARD8)GetCodeByte());
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s %3d", savedPC, opcode->getName(), ret.getArg1());
	return ret;
}
DEF_CI_r(W0F, WF)
// zWF - 0155
DEF_C_F(WF)
// zPSF - 0156
DEF_C_F(PSF)
// zPS0F - 0157
__attribute__((always_inline)) static inline Run C_PS0F_(Opcode *opcode) {
	Run ret(rPSF, L_PS0F, (CARD8)0, (CARD8)GetCodeByte());
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s %3d", savedPC, opcode->getName(), ret.getArg1());
	return ret;
}
DEF_CI_r(PS0F, PSF)


// zWS0F - 0160
__attribute__((always_inline)) static inline Run C_WS0F_(Opcode *opcode) {
	Run ret(rWSF, L_WS0F, (CARD8)0, (CARD8)GetCodeByte());
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s %3d", savedPC, opcode->getName(), ret.getArg1());
	return ret;
}
DEF_CI_r(WS0F, WSF)
// zWL0F - 0161
__attribute__((always_inline)) static inline Run C_WL0F_(Opcode *opcode) {
	Run ret(rWLF, L_WL0F, (CARD8)0, (CARD8)GetCodeByte());
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s %3d", savedPC, opcode->getName(), ret.getArg1());
	return ret;
}
DEF_CI_r(WL0F, WLF)
// zWLF - 0162
DEF_C_F(WLF)
// zPSLF - 0163
DEF_C_F(PSLF)
// zWLFS - 0164
DEF_C_0(WLFS)
// zSLDB - 0165
DEF_C_B(SLD)
// zSGDB - 0166
DEF_C_B(SGD)
// zLLKB - 0167
DEF_C_B(LLK)


// zRKIB - 0170
DEF_C_B(RKI)
// zRKDIB - 0171
DEF_C_B(RKDI)
// zLKB - 0172
DEF_C_B(LK)
// zSHIFT - 0173
DEF_C_0(SHIFT)
// zSHIFTSB - 0174
__attribute__((always_inline)) static inline Run C_SHIFTSB_(Opcode *opcode) {
	int shift = SignExtend(GetCodeByte());
	Run ret(rSHIFTI, L_SHIFTSB, (CARD16)shift);
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s %3d", savedPC, opcode->getName(), (INT16)ret.getArg());
	if (shift < -15 || 15 < shift) ERROR();
	return ret;
}
DEF_CI_r(SHIFTSB, SHIFTI)
// zMBP - 0175
// void OP_MBP() {} // MopcodesExtras.mesa
// zRBP - 0176
// void OP_RBP() {} // MopcodesExtras.mesa
// zWBP - 0177
// void OP_WBP() {} // MopcodesExtras.mesa


// zCATCH - 0200
DEF_C_B_N(CATCH)
// zJ2 - 0201
DEF_C_0_n_JUMP(J, 2)
// zJ3 - 0202
DEF_C_0_n_JUMP(J, 3)
// zJ4 - 0203
DEF_C_0_n_JUMP(J, 4)
// zJ5 - 0204
DEF_C_0_n_JUMP(J, 5)
// zJ6 - 0205
DEF_C_0_n_JUMP(J, 6)
// zJ7 - 0206
DEF_C_0_n_JUMP(J, 7)
// zJ8 - 0207
DEF_C_0_n_JUMP(J, 8)


// zJB - 0210
//void OP_JB() {
//	BYTE disp = GetCodeByte();
//	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  zJB %02X", savedPC, disp);
//	PC = savedPC + SignExtend(disp);
//}
DEF_C_SB_JUMP(J)
// zJW - 0211
//void OP_JW() {
//	INT16 disp = GetCodeWord();
//	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  zJW %3d", savedPC, disp);
//	PC = savedPC + disp;
//}
DEF_C_W_JUMP(J)
// zJEP - 0212
__attribute__((always_inline)) static inline Run C_JEP_(Opcode* opcode) {
	NibblePair pair = {GetCodeByte()};
	Run ret(rJUE, L_JEP, (CARD8)pair.left, (CARD8)pair.right + 4);
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s %3d %3d", savedPC, opcode->getName(), ret.getArg0(), ret.getArg1());
	return ret;
}
DEF_CI_r_JUMP(JEP, JUE)

// zJEB - 0213
//void OP_JEB() {
//	BYTE disp = GetCodeByte();
//	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  zJEB %02X", savedPC, disp);
//	UNSPEC v = Pop();
//	UNSPEC u = Pop();
//	if (u == v) {
//		PC = savedPC + SignExtend(disp);
//	}
//}
DEF_C_SB_N_JUMP(JEB)
// zJEBB - 0214
//void OP_JEBB() {
//	BYTE byte = GetCodeByte();
//	BYTE disp = GetCodeByte();
//	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  zJEBB %02X %02X", savedPC, byte, disp);
//	UNSPEC data = Pop();
//	if (data == byte) {
//		PC = savedPC + SignExtend(disp);
//	}
//}
__attribute__((always_inline)) static inline Run C_JEBB_(Opcode* opcode) {
	CARD8 arg0 = GetCodeByte();
	CARD8 arg1 = GetCodeByte();
	Run ret(rJUE, L_JEBB, arg0, arg1);
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s %3d %3d", savedPC, opcode->getName(), ret.getArg0(), ret.getArg1());
	return ret;
}
DEF_CI_r_JUMP(JEBB, JUE)
// zJNEP - 0215
__attribute__((always_inline)) static inline Run C_JNEP_(Opcode* opcode) {
	NibblePair pair = {GetCodeByte()};
	Run ret(rJUNE, L_JNEP, (CARD8)pair.left, (CARD8)pair.right + 4);
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s %3d %3d", savedPC, opcode->getName(), ret.getArg0(), ret.getArg1());
	return ret;
}
DEF_CI_r_JUMP(JNEP, JUNE)
// zJNEB - 0216
DEF_C_SB_N_JUMP(JNEB)
// zJNEBB - 0217
//void OP_JNEBB() {
//	BYTE byte = GetCodeByte();
//	BYTE disp = GetCodeByte();
//	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  zJNEBB %02X %02X", savedPC, byte, disp);
//	UNSPEC data = Pop();
//	if (data != byte) {
//		PC = savedPC + SignExtend(disp);
//	}
//}
__attribute__((always_inline)) static inline Run C_JNEBB_(Opcode* opcode) {
	CARD8 arg0 = GetCodeByte();
	CARD8 arg1 = GetCodeByte();
	Run ret(rJUNE, L_JNEBB, arg0, arg1);
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s %3d %3d", savedPC, opcode->getName(), ret.getArg0(), ret.getArg1());
	return ret;
}
DEF_CI_r_JUMP(JNEBB, JUNE)


// zJLB - 0220
DEF_C_SB_N_JUMP(JLB)
// zJGEB - 0221
DEF_C_SB_N_JUMP(JGEB)
// zJGB - 0222
DEF_C_SB_N_JUMP(JGB)
// zJLEB - 0223
DEF_C_SB_N_JUMP(JLEB)
// zJULB - 0224
DEF_C_SB_N_JUMP(JULB)
// zJUGEB - 0225
DEF_C_SB_N_JUMP(JUGEB)
// zJUGB - 0226
DEF_C_SB_N_JUMP(JUGB)
// zJULEB - 0227
DEF_C_SB_N_JUMP(JULEB)


// zJZ3 - 0230
DEF_C_0_n_JUMP(JZ, 3)
// zJZ4 - 0231
DEF_C_0_n_JUMP(JZ, 4)
// zJZB - 0232
DEF_C_SB_JUMP(JZ)
// zJNZ3 - 0233
DEF_C_0_n_JUMP(JNZ, 3)
// zJNZ4 - 0234
DEF_C_0_n_JUMP(JNZ, 4)
// zJNZB - 0235
DEF_C_SB_JUMP(JNZ)
// zJDEB - 0236
DEF_C_SB_JUMP(JDE)
// zJDNEB - 0237
DEF_C_SB_JUMP(JDNE)


// zJIB - 0240
DEF_C_W_N_JUMP(JIB)
// zJIW - 0241
DEF_C_W_N_JUMP(JIW)
// zREC - 0242
DEF_C_0(REC)
// zREC2 - 0243
DEF_C_0(REC2)
// zDIS - 0244
DEF_C_0(DIS)
// zDIS2 - 0245
DEF_C_0(DIS2)
// zEXCH - 0246
DEF_C_0(EXCH)
// zDEXCH - 0247
DEF_C_0(DEXCH)


// zDUP - 0250
DEF_C_0(DUP)
// zDDUP - 0251
DEF_C_0(DDUP)
// zEXDIS - 0252
DEF_C_0(EXDIS)
// zNEG - 0253
DEF_C_0(NEG)
// zINC - 0254
DEF_C_0(INC)
// zDEC - 0255
DEF_C_0(DEC)
// zDINC - 0256
DEF_C_0(DINC)
// zDBL - 0257
DEF_C_0(DBL)


// zDDBL - 0260
DEF_C_0(DDBL)
// zTRPL - 0261
DEF_C_0(TRPL)
// zAND - 0262
DEF_C_0(AND)
// zIOR - 0263
DEF_C_0(IOR)
// zADDSB - 0264
__attribute__((always_inline)) static inline Run C_ADDSB_(Opcode* opcode) {
	Run ret(rADDI, L_ADDSB, (CARD16)SignExtend(GetCodeByte()));
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s", savedPC, opcode->getName());
	return ret;
}
DEF_CI_r(ADDSB, ADDI)

// zADD - 0265
DEF_C_0(ADD)
// zSUB - 0266
DEF_C_0(SUB)
// zDADD - 0267
DEF_C_0(DADD)

// zDSUB - 0270
DEF_C_0(DSUB)
// zADC - 0271
DEF_C_0(ADC)
// zACD - 0272
DEF_C_0(ACD)
// zAL0IB - 0273
DEF_C_B(AL0I)
// zMUL - 0274
DEF_C_0(MUL)
// zDCMP - 0275
DEF_C_0(DCMP)
// zUDCMP - 0276
DEF_C_0(UDCMP)
// zVMFIND - 0277
DEF_C_0(VMFIND)


// zLI0 - 0300
DEF_C_0_n(LI, 0)
// zLI1 - 0301
DEF_C_0_n(LI, 1)
// zLI2 - 0302
DEF_C_0_n(LI, 2)
// zLI3 - 0303
DEF_C_0_n(LI, 3)
// zLI4 - 0304
DEF_C_0_n(LI, 4)
// zLI5 - 0305
DEF_C_0_n(LI, 5)
// zLI6 - 0306
DEF_C_0_n(LI, 6)
// zLI7 - 0307
DEF_C_0_n(LI, 7)


// zLI8 - 0310
DEF_C_0_n(LI, 8)
// zLI9 - 0311
DEF_C_0_n(LI, 9)
// zLI10 - 0312
DEF_C_0_n(LI, 10)
// zLIN1 - 0313
__attribute__((always_inline)) static inline Run C_LIN1_(Opcode* opcode) {
	Run ret(rLI, L_LIN1, (CARD16)0xffff);
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s", savedPC, opcode->getName());
	return ret;
}
DEF_CI_r(LIN1, LI)

// zLINI - 0314
__attribute__((always_inline)) static inline Run C_LINI_(Opcode* opcode) {
	Run ret(rLI, L_LINI, (CARD16)0x8000);
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s", savedPC, opcode->getName());
	return ret;
}
DEF_CI_r(LINI, LI)
// zLIB - 0315
DEF_C_B(LI)
// zLIW - 0316
DEF_C_W(LI)
// zLINB - 0317
__attribute__((always_inline)) static inline Run C_LINB_(Opcode* opcode) {
	Run ret(rLI, L_LINB, (CARD16)(0xff00 | GetCodeByte()));
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s", savedPC, opcode->getName());
	return ret;
}
DEF_CI_r(LINB, LI)


// zLIHB - 0320
__attribute__((always_inline)) static inline Run C_LIHB_(Opcode* opcode) {
	Run ret(rLI, L_LIHB, (CARD16)(GetCodeByte() << 8));
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s", savedPC, opcode->getName());
	return ret;
}
DEF_CI_r(LIHB, LI)
// zLID0 - 0321
DEF_C_0_n(LID, 0)
// zLA0 - 0322
DEF_C_0_n(LA, 0)
// zLA1 - 0323
DEF_C_0_n(LA, 1)
// zLA2 - 0324
DEF_C_0_n(LA, 2)
// zLA3 - 0325
DEF_C_0_n(LA, 3)
// zLA6 - 0326
DEF_C_0_n(LA, 6)
// zLA8 - 0327
DEF_C_0_n(LA, 8)


// zLAB - 0330
DEF_C_B(LA)
// zLAW - 0331
DEF_C_W(LA)
// zGA0 - 0332
DEF_C_0_n(GA, 0)
// zGA1 - 0333
DEF_C_0_n(GA, 1)
// zGAB - 0334
DEF_C_B(GA)
// zGAW - 0335
DEF_C_W(GA)
// zCAW - 0336
// void OP_CAW() {} // Mopcodes.mesa
// zLP - 0367
DEF_C_0(LP)

// zESC - 0370
DEF_C_0(ESC)
// zESCL - 0371
__attribute__((always_inline)) static inline Run C_ESCL_(Opcode* opcode) {
	Run ret(rESC, L_ESCL);
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s", savedPC, opcode->getName());
	return ret;
}
DEF_CI_r(ESCL, ESC)
// zLGA0 - 0372
DEF_C_0_n(LGA, 0)
// zLGAB - 0373
DEF_C_B(LGA)
// zLGAW - 0374
DEF_C_W(LGA)
// zDESC - 0375
DEF_C_W_N(DESC)
// zRESRVD - 0377
DEF_C_0(RESRVD)



// ASSIGNESC(a00,        00);        // 00
// ASSIGNESC(a01,        01);        // 01

// aSM - 07
DEF_C_0(SM)
// aGMF - 010
DEF_C_0(GMF)
// aGMF - 011
DEF_C_0(SMF)


// aDI - 020
DEF_C_0(DI)
// aEI - 021
DEF_C_0(EI)
// aXOR - 022
DEF_C_0(XOR)
// aDAND - 023
DEF_C_0(DAND)
// aDIOR - 024
DEF_C_0(DIOR)
// aDXOR - 025
DEF_C_0(DXOR)
// aROTATE - 026
DEF_C_0(ROTATE)
// aDSHIFT - 027
DEF_C_0(DSHIFT)


// aLINT - 030
DEF_C_0(LINT)
// aJS - 031
DEF_C_0_JUMP(JS)
// aRCFS - 032
DEF_C_0(RCFS)
// bRC - 033
DEF_C_B_N(RC)
// aUDIV - 034
DEF_C_0(UDIV)
// aLUDIV - 035
DEF_C_0(LUDIV)
// bROB - 036
__attribute__((always_inline)) static inline Run C_ROB_(Opcode* opcode) {
	Run ret(rROB, L_ROB, (CARD16)GetCodeByte());
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s %3d", savedPC, opcode->getName(), ret.getArg());
	if (!(1 <= ret.getArg() && ret.getArg() <= 4)) ERROR();
	return ret;
}
DEF_CI(ROB)
// bWOB - 037
__attribute__((always_inline)) static inline Run C_WOB_(Opcode* opcode) {
	Run ret(rWOB, L_WOB, (CARD16)GetCodeByte());
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s %3d", savedPC, opcode->getName(), ret.getArg());
	if (!(1 <= ret.getArg() && ret.getArg() <= 4)) ERROR();
	return ret;
}
DEF_CI(WOB)


// bDSK - 040
DEF_C_B_N(DSK)
// bLSK - 043
DEF_C_B_N(LSK)
// aBNDCKL - 044
DEF_C_0(BNDCKL)
// aNILCK - 045
DEF_C_0(NILCK)
// aNILCKL - 046
DEF_C_0(NILCKL)
// aVERSION - 057
// void OP_VERSION() {} // EscAlpha.mesa


// aDMUL - 060
DEF_C_0(DMUL)
// aSDIV - 061
DEF_C_0(SDIV)
// aSDDIV - 062
DEF_C_0(SDDIV)
// aUDDIV - 063
DEF_C_0(UDDIV)
// ASSIGNESC(a64,        64);        // 064
// ASSIGNESC(a65,        65);        // 065
// ASSIGNESC(a66,        66);        // 066
// ASSIGNESC(a67,        67);        // 067
// ASSIGNESC(a70,        70);        // 070
// ASSIGNESC(a71,        71);        // 071
// ASSIGNESC(a72,        72);        // 072
// ASSIGNESC(a73,        73);        // 073
// ASSIGNESC(a74,        74);        // 074
// ASSIGNESC(a75,        75);        // 075
// ASSIGNESC(a76,        76);        // 076
// ASSIGNESC(a77,        77);        // 077

// Floating Point (100B-137B are reserved)
// ASSIGNESC(aFADD,      FADD);      // 0100
// ASSIGNESC(aFSUB,      FSUB);      // 0101
// ASSIGNESC(aFMUL,      FMUL);      // 0102
// ASSIGNESC(aFDIV,      FDIV);      // 0103
// ASSIGNESC(aFCOMP,     FCOMP);     // 0104
// ASSIGNESC(aFIX,       FIX);       // 0105
// ASSIGNESC(aFLOAT,     FLOAT);     // 0106
// ASSIGNESC(aFIXI,      FIXI);      // 0107
// ASSIGNESC(aFIXC,      FIXC);      // 0110
// ASSIGNESC(aFSTICKY,   FSTICKY);   // 0111
// ASSIGNESC(aFREM,      FREM);      // 0112
// ASSIGNESC(aROUND,     ROUND);     // 0113
// ASSIGNESC(aROUNDI,    ROUNDI);    // 0114
// ASSIGNESC(aROUNDC,    ROUNDC);    // 0115
// ASSIGNESC(aFSQRT,     FSQRT);     // 0116
// ASSIGNESC(aFSC,       FSC);       // 0117


// Cedar collector and allocator (140B-157B are reserved)
// ASSIGNESC(aRECLAIMREF,           RECLAIMREF);           // 0140
// ASSIGNESC(aALTERCOUNT,           ALTERCOUNT);           // 0141
// ASSIGNESC(aRESETSTKBITS,         RESETSTKBITS);         // 0142
// ASSIGNESC(aGCSETUP,              GCSETUP);              // 0143
// ASSIGNESC(a144,                  144);                  // 0144
// ASSIGNESC(aENUMERATERECLAIMABLE, ENUMERATERECLAIMABLE); // 0145
// ASSIGNESC(a146,                  146);                  // 0146
// ASSIGNESC(aCREATEREF,            CREATEREF);            // 0147
// ASSIGNESC(a150,                  150);                  // 0150
// ASSIGNESC(aREFTYPE,              REFTYPE);              // 0151
// ASSIGNESC(aCANONICALREFTYPE,     CANONICALREFTYPE);     // 0152
// ASSIGNESC(aALLOCQUANTIZED,       ALLOCQUANTIZED);       // 0153
// ASSIGNESC(aALLOCHEAP,            ALLOCHEAP);            // 0154
// ASSIGNESC(aFREEOBJECT,           FREEOBJECT);           // 0155
// ASSIGNESC(aFREEQUANTIZED,        FREEQUANTIZED);        // 0156
// ASSIGNESC(aFREEPREFIXED,         FREEPREFIXED);         // 0157


//  Read / Write Registers
// aWRPSB - 0160
DEF_C_0(WRPSB)
// aWRMDS - 0161
DEF_C_0(WRMDS)
// aWRWP - 0162
DEF_C_0(WRWP)
// aWRWDC - 0163
DEF_C_0(WRWDC)
// aWRPTC - 0164
DEF_C_0(WRPTC)
// aWRIT - 0165
DEF_C_0(WRIT)
// aWRXTS - 0166
DEF_C_0(WRXTS)
// aWRMP - 0167
DEF_C_0(WRMP)


// aRRPSB - 0170
DEF_C_0(RRPSB)
// aRRMDS - 0171
DEF_C_0(RRMDS)
// aRRWP - 0172
DEF_C_0(RRWP)
// aRRWDC - 0173
DEF_C_0(RRWDC)

// aRRPTC - 0174
DEF_C_0(RRPTC)
// aRRIT - 0175
DEF_C_0(RRIT)
// aRRXTS - 0176
DEF_C_0(RRXTS)
// ASSIGNESC(a177,       OP_177);       // 0177

// Processor Dependent (200B-237B are reserved)
// ASSIGNESC(aINPUT,     OP_INPUT);     // 0200
// ASSIGNESC(aOUTPUT,    OP_OUTPUT);    // 0201
// ASSIGNESC(aLOADRAMJ,  OP_LOADRAMJ);  // 0202

// Dandelion
// ASSIGNESC(aBANDBLT,   OP_BANDBLT);   // 0203

// ESCAlphaExtra3.mesa
// ASSIGNESC(aLOCKQUEUE,  OP_LOCKQUEUE);// 0206

// Dolphin
// ASSIGNESC(aRPRINTER,   OP_RPRINTER); // 0203
// ASSIGNESC(aWPRINTER,   OP_WPRINTER); // 0204
// ASSIGNESC(aREADRAM,    OP_READRAM);  // 0205
// ASSIGNESC(aJRAM,       OP_JRAM);     // 0206
// ASSIGNESC(aCCOPR,      OP_CCOPR);    // 0207
// ASSIGNESC(aFPOPR,      OP_FPOPR);    // 0210

// Guam
// ASSIGNESC(aCALLAGENT,    OP_CALLAGENT);    // 0211
// ASSIGNESC(aMAPDISPLAY,   OP_MAPDISPLAY);   // 0212
// ASSIGNESC(aSTOPEMULATOR, OP_STOPEMULATOR); // 0213



//
// Pass 1
//

// RECPLLIMULRECLGDDADDRL
// REC / PL m / LI n / MUL / REC / LGD o / DADD / RL p
__attribute__((always_inline)) static inline void R_RECPLLIMULRECLGDDADDRL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RECPLLIMULRECLGDDADDRL %3d %3d %3d %3d", savedPC, run.getArg40(), run.getArg41(), run.getArg42(), run.getArg43());
	const CARD16 m = run.getArg40();
	const CARD16 n = run.getArg41();
	const CARD16 o = run.getArg42();
	const CARD16 p = run.getArg43();
	// 0 REC
	if (StackDepth <= SP) STACK_ERROR();
	// 1 PL m
	const CARD16 t1 = stack[SP];
	*StoreLF(m) = t1;
	// 2 LI n
	const CARD16 t2 = n;
	//Push(t2);
	// 3 MUL
	const CARD32 t3 = (CARD32)t1 * (CARD32)t2;
	// 4 REC
	// 5 LGD o
	const CARD32 t5 = ReadDbl(GF + o);
	// 6 DADD
	const CARD32 t6 = t3 + t5;
	// 7 RL p
	Push(*Fetch(t6 + p));
}
DEF_R(RECPLLIMULRECLGDDADDRL)

// LLINCLLJULB
// LL m / INC / LL n / JULB o
__attribute__((always_inline)) static inline void R_LLINCLLJULB_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLINCLLJULB %3d %3d %3d", savedPC, run.getArg3j0(), run.getArg3j1(), run.getArg3j2());
	const CARD16 m = run.getArg3j0();
	const CARD16 n = run.getArg3j1();
	const CARD16 o = run.getArg3j2();
	// 0 LL m
	// 1 INC
	const CARD16 u = *FetchLF(m) + 1;
	// 2 LL n
	const CARD16 v = *FetchLF(n);
	// 3 JULB o
	// need to leave values in stack
	Push(u);
	Push(v);
	SP -= 2;
	//
	if (u < v) PC = savedPC + o;
}
DEF_R(LLINCLLJULB)

// LLDLLRLSSL
// LLD m / LL n / RLS o / SL p
__attribute__((always_inline)) static inline void R_LLDLLRLSSL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLDLRLSSL %3d %3d %3d %3d", savedPC, run.getArg40(), run.getArg41(), run.getArg42(), run.getArg43());
	const CARD16 m = run.getArg40();
	const CARD16 n = run.getArg41();
	const CARD16 o = run.getArg42();
	const CARD16 p = run.getArg43();
	// 0 LLD m
	CARD32 ptr = ReadDblLF(m);
	// 1 LL n
	CARD16 index = *FetchLF(n);
	// 2 RLS o
	const CARD16 t2 = FetchByte(ptr, o + index);
	// 3 SL p
	*StoreLF(p) = t2;
}
DEF_R(LLDLLRLSSL)

// LLDLRLS
// LLD m / LL n / RLS o
__attribute__((always_inline)) static inline void R_LLDLLRLS_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLDLLRLS %3d %3d %3d", savedPC, run.getArg30(), run.getArg31(), run.getArg32());
	const CARD16 m = run.getArg30();
	const CARD16 n = run.getArg31();
	const CARD16 o = run.getArg32();
	// 0 LLD m
	CARD32 ptr = ReadDblLF(m);
	// 1 LL n
	CARD16 index = *FetchLF(n);
	// 2 RLS o
	const CARD16 t2 = FetchByte(ptr, o + index);
	Push(t2);
}
DEF_R(LLDLLRLS)

// LLLLADDSL
// LL m / LL n / ADD / SL o
__attribute__((always_inline)) static inline void R_LLLLADDSL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLLLADDSL %3d %3d %3d", savedPC, run.getArg30(), run.getArg31(), run.getArg32());
	const CARD16 m = run.getArg30();
	const CARD16 n = run.getArg31();
	const CARD16 o = run.getArg32();
	// 0 LL m
	const CARD16 t0 = *FetchLF(m);
	// 1 LL n
	const CARD16 t1 = *FetchLF(n);
	// 2 ADD
	const CARD16 t2 = t0 + t1;
	// 3 SL o
	*StoreLF(o) = t2;
}
DEF_R(LLLLADDSL)

// LLDBLRLDIACD
// LL m / DBL / RLDI n o / ACD
__attribute__((always_inline)) static inline void R_LLDBLRLDIACD_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLDBLRLDIACD %3d %3d %3d", savedPC, run.getArg30(), run.getArg31(), run.getArg32());
	const CARD16 m = run.getArg30();
	const CARD16 n = run.getArg31();
	const CARD16 o = run.getArg32();
	// 0 LL m
	const CARD16 t0 = *FetchLF(m);
	// 1 DBL
	const CARD16 t2 = t0 + t0;
	// 2 RLDI n o
	const CARD16 ptr = *FetchLF(n) + o;
	const CARD32 t3 = ReadDblMds(ptr);
	// 3 ACD
	PushLong(t2 + t3);
}
DEF_R(LLDBLRLDIACD)

// RLLLDACDSLD
// RL m / LLD n / ACD / SLD o
__attribute__((always_inline)) static inline void R_RLLLDACDSLD_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RLLLDACDSLD %3d %3d %3d", savedPC, run.getArg3a0(), run.getArg3a1(), run.getArg3a2());
	const CARD16 m = run.getArg3a0();
	const CARD16 n = run.getArg3a1();
	const CARD16 o = run.getArg3a2();
	// 0 RL m
	const CARD32 ptr = PopLong();
	const CARD16 t0 = *Fetch(ptr + m);
	// 1 LLD n
	const CARD32 t1 = ReadDblLF(n);
	// 2 ACD
	const CARD32 t2 = t0 + t1;
	// 3 SLD o
	CARD16 *p0 = StoreLF(o + 0);
	CARD16 *p1 = StoreLF(o + 1);
	*p1 = (CARD16)(t2 >> WordSize);
	*p0 = (CARD16)t2;
}
DEF_R(RLLLDACDSLD)

// RLRLDIACDSLD
// RL m / RLDI 0 n / ACD / SLD o
__attribute__((always_inline)) static inline void R_RLRLDIACDSLD_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RLRLDIACDSLD %3d %3d %3d", savedPC, run.getArg30(), run.getArg31(), run.getArg32());
	const CARD16 m = run.getArg30();
	const CARD16 n = run.getArg31();
	const CARD16 o = run.getArg32();
	// 0 RL m
	const CARD32 ptr0 = PopLong();
	const CARD16 t0 = *Fetch(ptr0 + m);
	// 1 RLDI 0 n
	CARD16 ptr1 = *FetchLF(0) + n;
	CARD32 t1 = ReadDblMds(ptr1);
	// 2 ACD
	const CARD32 t2 = t0 + t1;
	// 3 SLD o
	CARD16 *p0 = StoreLF(o + 0);
	CARD16 *p1 = StoreLF(o + 1);
	*p1 = (CARD16)(t2 >> WordSize);
	*p0 = (CARD16)t2;
}
DEF_R(RLRLDIACDSLD)

// LLLLDACDRL
// LL m / LLD n / ACD / RL o
__attribute__((always_inline)) static inline void R_LLLLDACDRL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLLLDACDRL %3d %3d %3d", savedPC, run.getArg3a0(), run.getArg3a1(), run.getArg3a2());
	const CARD16 m = run.getArg3a0();
	const CARD16 n = run.getArg3a1();
	const CARD16 o = run.getArg3a2();
	// 0 LL m
	const CARD16 t0 = *FetchLF(m);
	// 1 LLD n
	const CARD32 t1 = ReadDblLF(n);
	// 2 ACD
	const CARD32 t2 = t0 + t1;
	// 3 RL o
	Push(*Fetch(t2 + o));
}
DEF_R(LLLLDACDRL)

// LLLLJLB
// LL m / LL n / JLB o
__attribute__((always_inline)) static inline void R_LLLLJLB_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLLLJLB %3d %3d %3d", savedPC, run.getArg3j0(), run.getArg3j1(), run.getArg3j2());
	const CARD16 m = run.getArg3j0();
	const CARD16 n = run.getArg3j1();
	const CARD16 o = run.getArg3j2();
	// 0 LL m
	const INT16 j = *FetchLF(m);
	// 1 LL n
	const INT16 k = *FetchLF(n);
	// 2 JLB o
	Push(j);
	Push(k);
	SP -= 2;
	//
	if (j < k) PC = savedPC + o;
}
DEF_R(LLLLJLB)

// RLILSL
// RLIL m n / SL o
__attribute__((always_inline)) static inline void R_RLILSL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RLILSL %3d %3d %3d", savedPC, run.getArg3c0(), run.getArg3c1(), run.getArg3c2());
	const CARD16 m = run.getArg3c0();
	const CARD16 n = run.getArg3c1();
	const CARD16 o = run.getArg3c2();
	// 0 RLIL m n
	LONG_POINTER ptr = ReadDblLF(m) + n;
	CARD16 t0 = *Fetch(ptr);
	//
	Push(*Fetch(ptr));
	SP -= 1;
	// 1 SL o
	*StoreLF(o) = t0;
}
DEF_R(RLILSL)

// SLLLLLADDSL
// SL m / LL n / LL o / ADD / SL p
__attribute__((always_inline)) static inline void R_SLLLLLADDSL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  SLLLLLADDSL %3d %3d %3d %3d", savedPC, run.getArg40(), run.getArg41(), run.getArg42(), run.getArg43());
	const CARD16 m = run.getArg40();
	const CARD16 n = run.getArg41();
	const CARD16 o = run.getArg42();
	const CARD16 p = run.getArg43();
	// 0 SL m
	*StoreLF(m) = Pop();
	// 1 LL n
	const CARD16 t0 = *FetchLF(n);
	// 2 LL o
	const CARD16 t1 = *FetchLF(o);
	// 3 ADD
	const CARD16 t2 = t0 + t1;
	// 4 SL p
	*StoreLF(p) = t2;
}
DEF_R(SLLLLLADDSL)

// LLDECDBLRLDIACDWDL
// LL m / DEC / DBL / RLDI m n / ACD / WDL p
__attribute__((always_inline)) static inline void R_LLDECDBLRLDIACDWDL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLDECDBLRLDIACDWDL %3d %3d %3d %3d", savedPC, run.getArg40(), run.getArg41(), run.getArg42(), run.getArg43());
	const CARD16 m = run.getArg40();
	const CARD16 n = run.getArg41();
	const CARD16 o = run.getArg42();
	const CARD16 p = run.getArg43();
	// 0 LL m
	const CARD16 t0 = *FetchLF(m);
	// 1 DEC
	const CARD16 t1 = t0 - 1;
	// 2 DBL
	const CARD16 t2 = t1 + t1;
	// 3 RLDI n o
	const CARD16 p3 = *FetchLF(n) + o;
	const CARD32 t3 = ReadDblMds(p3);
	// 4 ACD
	const CARD32 t4 = t2 + t3;
	// 5 WDL p
	const CARD32 p5 = t4 + p;
	CARD16 *p0 = Store(p5);
	CARD16 *p1 = ((p5 & (PageSize - 1)) == (PageSize - 1)) ? Store(p5 + 1) : p0 + 1;
	*p1 = Pop();
	*p0 = Pop();
}
DEF_R(LLDECDBLRLDIACDWDL)

// LLDECDBLRLDIACDRDL
// LL m / DEC / DBL / RLDI m n / ACD / RDL p
__attribute__((always_inline)) static inline void R_LLDECDBLRLDIACDRDL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLDECDBLRLDIACDRDL %3d %3d %3d %3d", savedPC, run.getArg40(), run.getArg41(), run.getArg42(), run.getArg43());
	const CARD16 m = run.getArg40();
	const CARD16 n = run.getArg41();
	const CARD16 o = run.getArg42();
	const CARD16 p = run.getArg43();
	// 0 LL m
	const CARD16 t0 = *FetchLF(m);
	// 1 DEC
	const CARD16 t1 = t0 - 1;
	// 2 DBL
	const CARD16 t2 = t1 + t1;
	// 3 RLDI n o
	const CARD16 p3 = *FetchLF(n) + o;
	const CARD32 t3 = ReadDblMds(p3);
	// 4 ACD
	const CARD32 t4 = t2 + t3;
	// 5 RDL p
	LONG_POINTER p5 = t4 + p;
	CARD16 *p0 = Store(p5);
	CARD16 *p1 = ((p5 & (PageSize - 1)) == (PageSize - 1)) ? Store(p5 + 1) : p0 + 1;
	UNSPEC u = *p0;
	UNSPEC v = *p1;
	Push(u);
	Push(v);
}
DEF_R(LLDECDBLRLDIACDRDL)

// LLDLILALP
__attribute__((always_inline)) static inline void R_LLDLILALP_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLDLILALP %3d %3d %3d", savedPC, run.getArg30(), run.getArg31(), run.getArg32());
	const CARD16 m = run.getArg30();
	const CARD16 n = run.getArg31();
	const CARD16 o = run.getArg32();
	// 0 LLD m
	CARD16 *p0 = FetchLF(m + 0);
	CARD16 *p1 = FetchLF(m + 1);
	Push(*p0);
	Push(*p1);
	// 1 LI n
	Push(n);
	// 2 LA o
	const CARD32 t2 = LFCache::LF() + o;
	// 3 LP
	PushLong(LengthenPointer(t2));
}
DEF_R(LLDLILALP)

//LALILA
__attribute__((always_inline)) static inline void R_LALILA_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LALILA %3d %3d %3d", savedPC, run.getArg30(), run.getArg31(), run.getArg32());
	const CARD16 m = run.getArg30();
	const CARD16 n = run.getArg31();
	const CARD16 o = run.getArg32();
	// 0 LA m
	Push(LFCache::LF() + m);
	// 1 LI n
	Push(n);
	// 2 LA o
	Push(LFCache::LF() + o);
}
DEF_R(LALILA)

// LLJNEB
// LL m / JNEB n
__attribute__((always_inline)) static inline void R_LLJNEB_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLJNEB %3d %3d", savedPC, run.getArg0(), run.getArg1());
	const CARD16 m = run.getArg0();
	const CARD16 n = (CARD16)SignExtend((CARD8)run.getArg1());
	// LL m
	const CARD16 t0 = *FetchLF(m);
	// JNEB n
	const UNSPEC u = Pop();
	if (u != t0) PC = savedPC + n;
}
DEF_R(LLJNEB)

// PLLLJNEB
// PL m / LL n / JNEB o
__attribute__((always_inline)) static inline void R_PLLLJNEB_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  PLLLJNEB %3d %3d", savedPC, run.getArg3j0(), run.getArg3j1(), run.getArg3j2());
	const CARD16 m = run.getArg3j0();
	const CARD16 n = run.getArg3j1();
	const CARD16 o = run.getArg3j2();
	// 0 PL m
	const CARD16 t0 = Pop();
	*StoreLF(m) = t0;
	// 1 LL n
	const CARD16 t1 = *FetchLF(n);
	// 2 JNEB o
	if (t1 != t0) PC = savedPC + o;
}
DEF_R(PLLLJNEB)

// LKSLSL
// LK m / SL n / SL o
__attribute__((always_inline)) static inline void R_LKSLSL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LKSLSL %3d %3d", savedPC, run.getArg30(), run.getArg31(), run.getArg32());
	const CARD16 m = run.getArg30();
	const CARD16 n = run.getArg31();
	const CARD16 o = run.getArg32();
	// 0 LK m
	if (SP == StackDepth) STACK_ERROR();
	const ShortControlLink link = stack[SP];
	*StoreLF(0) = link - m;
	// 1 SL n
	*StoreLF(n) = Pop();
	// 2 SL o
	*StoreLF(o) = Pop();
}
DEF_R(LKSLSL)

// LKSLPLDBL
// LK m / SL n / PL o / DBL
__attribute__((always_inline)) static inline void R_LKSLPLDBL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LKSLPLDBL %3d %3d", savedPC, run.getArg30(), run.getArg31(), run.getArg32());
	const CARD16 m = run.getArg30();
	const CARD16 n = run.getArg31();
	const CARD16 o = run.getArg32();
	// 0 LK m
	if (SP == StackDepth) STACK_ERROR();
	const ShortControlLink link = stack[SP];
	*StoreLF(0) = link - m;
	// 1 SL n
	*StoreLF(n) = Pop();
	// 2 PL o
	*StoreLF(o) = Pop();
	SP++;
	// 3 DBL
	stack[SP - 1] *= 2;
}
DEF_R(LKSLPLDBL)

// RLDIACDRLSL
// RLDI m n / ACD / RL o / SL p
__attribute__((always_inline)) static inline void R_RLDIACDRLSL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RLDIACDRLSL %3d %3d %3d %3d", savedPC, run.getArg40(), run.getArg41(), run.getArg42(), run.getArg43());
	const CARD16 m = run.getArg40();
	const CARD16 n = run.getArg41();
	const CARD16 o = run.getArg42();
	const CARD16 p = run.getArg43();
	// 0 RLDI m n
	const CARD16 p0 = *FetchLF(m) + n;
	const CARD32 t0 = ReadDblMds(p0);
	// 1 ACD
	const CARD32 t1 = t0 + Pop();
	// 2 RL o
	Push(*Fetch(t1 + o));
	// 3 SL p
	*StoreLF(p) = Pop();
}
DEF_R(RLDIACDRLSL)

// RLSLRLDI
// RL m / SL n / RLDI o p
__attribute__((always_inline)) static inline void R_RLSLRLDI_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RLSLRLDI %3d %3d %3d %3d", savedPC, run.getArg40(), run.getArg41(), run.getArg42(), run.getArg43());
	const CARD16 m = run.getArg40();
	const CARD16 n = run.getArg41();
	const CARD16 o = run.getArg42();
	const CARD16 p = run.getArg43();
	// 0 RL m
	const CARD32 p0 = PopLong();
	const CARD16 t0 = *Fetch(p0 + m);
	// 1 SL n
	*StoreLF(n) = t0;
	// 2 RLDI o p
	const CARD16 p2 = *FetchLF(o) + p;
	const CARD32 t2 = ReadDblMds(p2);
	PushLong(t2);
}
DEF_R(RLSLRLDI)

// LLDECLLRD
// LL m / DEC / LL n / RD o
__attribute__((always_inline)) static inline void R_LLDECLLRD_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLDECLLRD %3d %3d %3d", savedPC, run.getArg30(), run.getArg31(), run.getArg32());
	const CARD16 m = run.getArg30();
	const CARD16 n = run.getArg31();
	const CARD16 o = run.getArg32();
	// 0 LL m
	const CARD16 t0 = *FetchLF(m);
	// 1 DEC
	const CARD16 t1 = t0 - 1;
	Push(t1);
	// 2 LL n
	const CARD16 t2 = *FetchLF(n);
	// 3 RD o
	CARD16 p3 = t2 + o;
	CARD32 t3 = ReadDblMds(p3);
	PushLong(t3);
}
DEF_R(LLDECLLRD)

// LALLPSF
// LA m / LL n / PSF 0 o
__attribute__((always_inline)) static inline void R_LALLPSF_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LALLPSF %3d %3d %3d %3d", savedPC, run.getArg40(), run.getArg41(), run.getArg42(), run.getArg43());
	const CARD16 m = run.getArg3d0();
	const CARD16 n = run.getArg3d1();
	const CARD16 o = run.getArg3d2();
	// 0 LA m
	const CARD16 t0 = LFCache::LF() + m;
	Push(t0);
	// 1 LL n
	const CARD16 t1 = *FetchLF(n);
	// 2 PSF o p
	CARD16 *p2 = StoreMds(t0);
	*p2 = WriteField(*p2, o, t1);
}
DEF_R(LALLPSF)

// LLDUPDECLLRD
// LL m / DUP / DEC / LL n / RD o
__attribute__((always_inline)) static inline void R_LLDUPDECLLRD_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLDUPDECLLRD %3d %3d", savedPC, run.getArg30(), run.getArg31(), run.getArg32());
	const CARD16 m = run.getArg30();
	const CARD16 n = run.getArg31();
	const CARD16 o = run.getArg32();
	// 0 LL m
	const CARD16 t0 = *FetchLF(m);
	Push(t0);
	// 1 DUP
	// 2 DEC
	const CARD16 t2 = t0 - 1;
	Push(t2);
	// 3 LL n
	const CARD16 t3 = *FetchLF(n);
	// 3 RD o
	const CARD16 p3 = t3 + o;
	CARD16 *p0 = FetchMds(p3);
	CARD16 *p1 = ((p3 & (PageSize - 1)) == (PageSize - 1)) ? FetchMds(p3 + 1) : p0 + 1;
	UNSPEC u = *p0;
	UNSPEC v = *p1;
	Push(u);
	Push(v);
}
DEF_R(LLDUPDECLLRD)

// PL m / DBL / PL n / LL o
__attribute__((always_inline)) static inline void R_PLDBLPLLL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  PLDBLPLLL %3d %3d", savedPC, run.getArg30(), run.getArg31(), run.getArg32());
	const CARD16 m = run.getArg30();
	const CARD16 n = run.getArg31();
	const CARD16 o = run.getArg32();
	// 0 PL m
	if (SP == 0) STACK_ERROR();
	const CARD16 t0 = stack[SP - 1];
	*StoreLF(m) = t0;
	// 1 DBL
	const CARD16 t1 = t0 * 2;
	stack[SP - 1] = t1;
	// 2 PL n
	*StoreLF(n) = t1;
	// 3 LL o
	Push(*FetchLF(o));
}
DEF_R(PLDBLPLLL)

// LLJULB
// LL m / JULB n
__attribute__((always_inline)) static inline void R_LLJULB_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLJULB %3d %3d", savedPC, run.getArg0(), run.getArg1());
	const CARD16 m = run.getArg0();
	const CARD16 n = (CARD16)SignExtend((CARD8)run.getArg1());
	// LL m
	const CARD16 t0 = *FetchLF(m);
	Push(t0); // Need to leave value on stack
	// JULB n
	const CARD16 v = stack[--SP]; // Pop
	const CARD16 u = Pop();
	if (u < v) PC = savedPC + n;
}
DEF_R(LLJULB)

// SLDRLDIL
// SLD m / RLDIL n o
__attribute__((always_inline)) static inline void R_SLDRLDIL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  SLDRLDIL %3d %3d", savedPC, run.getArg30(), run.getArg31(), run.getArg32());
	const CARD16 m = run.getArg30();
	const CARD16 n = run.getArg31();
	const CARD16 o = run.getArg32();
	// 0 SLD m
	{
		CARD16 *p0 = StoreLF(m + 0);
		CARD16 *p1 = StoreLF(m + 1);
		*p1 = Pop();
		*p0 = Pop();
	}
	// 1 RLDIL n o
	{
		const CARD32 p = ReadDblLF(n) + o;
		const CARD16 *p0 = Fetch(p);
		const CARD16 *p1 = ((p & (PageSize - 1)) == (PageSize - 1)) ? Fetch(p + 1) : p0 + 1;
		const CARD16 u = *p0;
		const CARD16 v = *p1;
		Push(u);
		Push(v);
	}
}
DEF_R(SLDRLDIL)

// MULRECLLDDADD
// MUL / REC / LLD m / DADD
__attribute__((always_inline)) static inline void R_MULRECLLDDADD_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  MULRECLLDDADD %3d", savedPC, run.getArg());
	const CARD16 m = run.getArg();
	// 0 MUL
	const CARD16 t = Pop();
	const CARD16 s = Pop();
	const CARD32 t0 = s * t;
	// 1 REC
	// 2 LLD m
	const CARD32 t2 = ReadDblLF(m);
	// 3 DADD
	PushLong(t0 + t2);
}
DEF_R(MULRECLLDDADD)

// LLJUGB
// LL m / JUGB n
__attribute__((always_inline)) static inline void R_LLJUGB_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLJUGB %3d %3d", savedPC, run.getArg0(), run.getArg1());
	const CARD16 m = run.getArg0();
	const CARD16 n = (CARD16)SignExtend((CARD8)run.getArg1());
	// LL m
	const CARD16 t0 = *FetchLF(m);
	Push(t0); // Need to leave value on stack
	// JULB n
	const CARD16 v = stack[--SP]; // Pop
	const CARD16 u = Pop();
	if (u > v) PC = savedPC + n;
}
DEF_R(LLJUGB)

// IORJZ
// IOR / JZ m
__attribute__((always_inline)) static inline void R_IORJZ_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  IORJZ %3d %3d", savedPC, (INT16)run.getArg());
	const CARD16 m = run.getArg();
	// 0 IOR
	const CARD16 v = Pop();
	const CARD16 u = Pop();
	const CARD16 t0 = u | v;
	stack[SP] = t0;
	// 1 JZ m
	if (t0 == 0) PC = savedPC + m;
}
DEF_R(IORJZ)

// LLDUDCMP
// LLD m / UDCMP
__attribute__((always_inline)) static inline void R_LLDUDCMP_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLDUDCMP %3d %3d", savedPC, run.getArg());
	const CARD16 m = run.getArg();
	// 0 LLD m
	const CARD32 t0 = ReadDblLF(m);
	// 1 UDCMP
	const CARD32 s = PopLong();
	Push((s == t0) ? 0 : ((s > t0) ? 1 : -1));
}
DEF_R(LLDUDCMP)

// LLJGB
// LL m / JGB n
__attribute__((always_inline)) static inline void R_LLJGB_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLJGB %3d %3d", savedPC, run.getArg0(), run.getArg1());
	const CARD16 m = run.getArg0();
	const CARD16 n = (CARD16)SignExtend((CARD8)run.getArg1());
	// LL m
	const CARD16 t0 = *FetchLF(m);
	Push(t0); // Need to leave value on stack
	// JULB n
	const INT16 v = (INT16)stack[--SP]; // Pop
	const INT16 u = (INT16)Pop();
	if (u > v) PC = savedPC + n;
}
DEF_R(LLJGB)

// LLSHIFTI
// LL m / SHIFTI n
__attribute__((always_inline)) static inline void R_LLSHIFTI_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLSHIFTI %3d %3d", savedPC, run.getArg0(), run.getArg1());
	const CARD16 m = run.getArg0();
	const INT16  n = SignExtend((CARD8)run.getArg1());
	// 0 LL m
	const CARD16 t0 = *FetchLF(m);
	// 1 SHIFTI n
	Push(Shift(t0, n));
}
DEF_R(LLSHIFTI)

// LLJEB
// LL m / JNEB n
__attribute__((always_inline)) static inline void R_LLJEB_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLJEB %3d %3d", savedPC, run.getArg0(), run.getArg1());
	const CARD16 m = run.getArg0();
	const CARD16 n = (CARD16)SignExtend((CARD8)run.getArg1());
	// LL m
	const CARD16 t0 = *FetchLF(m);
	// JNEB n
	const UNSPEC u = Pop();
	if (u == t0) PC = savedPC + n;
}
DEF_R(LLJEB)

// IORJNZ
// IOR / JNZ m
__attribute__((always_inline)) static inline void R_IORJNZ_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  IORJNZ %3d %3d", savedPC, (INT16)run.getArg());
	const CARD16 m = run.getArg();
	// 0 IOR
	const CARD16 v = Pop();
	const CARD16 u = Pop();
	const CARD16 t0 = u | v;
	stack[SP] = t0;
	// 1 JNZ m
	if (t0) PC = savedPC + m;
}
DEF_R(IORJNZ)


//
// Pass 2
//

// SLLLLL
// SL m / LL n / LL o
__attribute__((always_inline)) static inline void R_SLLLLL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  SLLLLL %3d %3d %3d", savedPC, run.getArg30(), run.getArg31(), run.getArg32());
	const CARD16 m = run.getArg30();
	const CARD16 n = run.getArg31();
	const CARD16 o = run.getArg32();
	// 0 SL m
	*StoreLF(m) = Pop();
	// 0 LL n
	Push(*FetchLF(n));
	// 1 LL o
	Push(*FetchLF(o));
}
DEF_R(SLLLLL)

// LLLL
// LL m / LL n
__attribute__((always_inline)) static inline void R_LLLL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLLL %3d %3d", savedPC, run.getArg0(), run.getArg1());
	const CARD16 m = run.getArg0();
	const CARD16 n = run.getArg1();
	// 0 LL m
	Push(*FetchLF(m));
	// 1 LL n
	Push(*FetchLF(n));
}
DEF_R(LLLL)


// LKSL
// LK m / SL n
__attribute__((always_inline)) static inline void R_LKSL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LKSL %3d %3d", savedPC, run.getArg0(), run.getArg1());
	const CARD16 m = run.getArg0();
	const CARD16 n = run.getArg1();
	// 0 LK m
	if (SP == StackDepth) STACK_ERROR();
	const ShortControlLink link = stack[SP];
	*StoreLF(0) = link - m;
	// 1 SL n
	*StoreLF(n) = Pop();
}
DEF_R(LKSL)

// LLLI
// LL m / LI n
__attribute__((always_inline)) static inline void R_LLLI_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLLI %3d %3d", savedPC, run.getArg0(), run.getArg1());
	const CARD16 m = run.getArg0();
	const CARD16 n = run.getArg1();
	// 0 LL m
	Push(*FetchLF(m));
	// 1 SL n
	Push(n);
}
DEF_R(LLLI)

// SLDSLD
// SLD m / SLD n
__attribute__((always_inline)) static inline void R_SLDSLD_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  SLDSLD %3d %3d", savedPC, run.getArg0(), run.getArg1());
	const CARD16 m = run.getArg0();
	const CARD16 n = run.getArg1();
	// 0 SLD m
	{
		CARD16 *p0 = StoreLF(m + 0);
		CARD16 *p1 = StoreLF(m + 1);
		*p1 = Pop();
		*p0 = Pop();
	}

	// 1 SLD n
	{
		CARD16 *p0 = StoreLF(n + 0);
		CARD16 *p1 = StoreLF(n + 1);
		*p1 = Pop();
		*p0 = Pop();
	}
}
DEF_R(SLDSLD)

// SLDSLDSLD
// SLD m / SLD n / SLD o
__attribute__((always_inline)) static inline void R_SLDSLDSLD_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  SLDSLDSLD %3d %3d %3d", savedPC, run.getArg30(), run.getArg31(), run.getArg32());
	const CARD16 m = run.getArg30();
	const CARD16 n = run.getArg31();
	const CARD16 o = run.getArg32();
	// 0 SLD m
	{
		CARD16 *p0 = StoreLF(m + 0);
		CARD16 *p1 = StoreLF(m + 1);
		*p1 = Pop();
		*p0 = Pop();
	}
	// 1 SLD n
	{
		CARD16 *p0 = StoreLF(n + 0);
		CARD16 *p1 = StoreLF(n + 1);
		*p1 = Pop();
		*p0 = Pop();
	}
	// 2 SLD o
	{
		CARD16 *p0 = StoreLF(o + 0);
		CARD16 *p1 = StoreLF(o + 1);
		*p1 = Pop();
		*p0 = Pop();
	}
}
DEF_R(SLDSLDSLD)

// LIJ
// LI m / J n
__attribute__((always_inline)) static inline void R_LIJ_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LIJ %3d %3d %3d", savedPC, run.getArg0(), (CARD16)SignExtend(run.getArg1()));
	const CARD16 m = run.getArg0();
	const CARD16 n = (CARD16)SignExtend(run.getArg1());
	// 0 LI m
	Push(m);
	// 1 J n
	PC = savedPC + n;
}
DEF_R(LIJ)

// LLJ
// LL m / J n
__attribute__((always_inline)) static inline void R_LLJ_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLJ %3d %3d", savedPC, run.getArg0(), (CARD16)SignExtend(run.getArg1()));
	const CARD16 m = run.getArg0();
	const CARD16 n = (CARD16)SignExtend(run.getArg1());
	// 0 LI m
	Push(*FetchLF(m));
	// 1 J n
	PC = savedPC + n;
}
DEF_R(LLJ)

// LLINC
// LL m / INC
__attribute__((always_inline)) static inline void R_LLINC_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLINC %3d", savedPC, run.getArg());
	const CARD16 m = run.getArg();
	// 0 LL m
	const CARD16 t0 = *FetchLF(m);
	// 1 INC
	Push(t0 + 1);
}
DEF_R(LLINC)

// LLDLLD
// LLD m / LLD n
__attribute__((always_inline)) static inline void R_LLDLLD_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLDLLD %3d %3d", savedPC, run.getArg0(), run.getArg1());
	const CARD16 m = run.getArg0();
	const CARD16 n = run.getArg1();
	// 0 LLD m
	{
		CARD16 *p0 = FetchLF(m + 0);
		CARD16 *p1 = FetchLF(m + 1);
		Push(*p0);
		Push(*p1);
	}
	// 1 LLD n
	{
		CARD16 *p0 = FetchLF(n + 0);
		CARD16 *p1 = FetchLF(n + 1);
		Push(*p0);
		Push(*p1);
	}
}
DEF_R(LLDLLD)

// RECSL
// REC / SL m
__attribute__((always_inline)) static inline void R_RECSL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RECSL %3d %3d", savedPC, run.getArg());
	const CARD16 m = run.getArg();
	// REC
	if (SP == StackDepth) STACK_ERROR();
	// SL m
	*StoreLF(m) = stack[SP];
}
DEF_R(RECSL)

// LLDACD
// LLD m / ACD
__attribute__((always_inline)) static inline void R_LLDACD_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLDACD %3d %3d", savedPC, run.getArg());
	const CARD16 m = run.getArg();
	// 0 LLD m
	const CARD32 t0 = ReadDblLF(m);
	// 1 ACD
	const CARD16 t1 = Pop();
	PushLong(t0 + t1);
}
DEF_R(LLDACD)

// LLDLL
// LLD m / LL n
__attribute__((always_inline)) static inline void R_LLDLL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLDLL %3d %3d", savedPC, run.getArg0(), run.getArg1());
	const CARD16 m = run.getArg0();
	const CARD16 n = run.getArg1();
	// 0 LLD m
	{
		CARD16 *p0 = FetchLF(m + 0);
		CARD16 *p1 = FetchLF(m + 1);
		Push(*p0);
		Push(*p1);
	}
	// 1 LL n
	const CARD16 t1 = *FetchLF(n);
	Push(t1);
}
DEF_R(LLDLL)

// LLDLI
// LLD m / LI n
__attribute__((always_inline)) static inline void R_LLDLI_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLDLI %3d %3d", savedPC, run.getArg0(), run.getArg1());
	const CARD16 m = run.getArg0();
	const CARD16 n = run.getArg1();
	// 0 LLD m
	{
		CARD16 *p0 = FetchLF(m + 0);
		CARD16 *p1 = FetchLF(m + 1);
		Push(*p0);
		Push(*p1);
	}
	// 1 LI n
	Push((CARD16)SignExtend((CARD8)n));
}
DEF_R(LLDLI)

// PLLI
// PL m / LI n
__attribute__((always_inline)) static inline void R_PLLI_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  PLLI %3d %3d", savedPC, run.getArg0(), run.getArg1());
	const CARD16 m = run.getArg0();
	const CARD16 n = run.getArg1();
	// 0 PL m
	if (SP == 0) STACK_ERROR();
	*StoreLF(m) = stack[SP - 1];
	// 1 LI n
	Push((CARD16)SignExtend((CARD8)n));
}
DEF_R(PLLI)

// LALP
// LA m / LP
__attribute__((always_inline)) static inline void R_LALP_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LALP %3d %3d", savedPC, run.getArg());
	const CARD16 m = run.getArg();
	// LA m
	const CARD16 p01 = LFCache::LF() + m;
	// LP
	PushLong(MDSCache::lengthenPointer(p01));
}
DEF_R(LALP)

// LLWSF
// LL m / WSF 0 n
__attribute__((always_inline)) static inline void R_LLWSF_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LLWSF %3d %3d", savedPC, run.getArg0(), run.getArg1());
	const CARD16 m = run.getArg0();
	const CARD16 n = run.getArg1();
	// 0 LL m
	const CARD16 t0 = *FetchLF(m);
	// 1 WSF 0 n
	CARD16 p1 = Pop();
	CARD16 *p = StoreMds(p1);
	*p = WriteField(*p, (CARD8)n, t0);
}
DEF_R(LLWSF)

// LISL
// LI m / SL n
__attribute__((always_inline)) static inline void R_LISL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LISL %3d %3d", savedPC, run.getArg0(), run.getArg1());
	const CARD16 m = run.getArg0();
	const CARD16 n = run.getArg1();
	// 0 LI m
	const CARD16 t0 = (CARD16)SignExtend((CARD8)m);
	// 1 SL n
	*StoreLF(n) = t0;
}
DEF_R(LISL)

// LALI
// LA m / LI n
__attribute__((always_inline)) static inline void R_LALI_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  LALI %3d %3d", savedPC, run.getArg0(), run.getArg1());
	const CARD16 m = run.getArg0();
	const CARD16 n = run.getArg1();
	// 0 LA m
	const CARD16 t0 = LFCache::LF() + m;
	Push(t0);
	// 1 LI n
	const CARD16 t1 = (CARD16)SignExtend((CARD8)n);
	Push(t1);
}
DEF_R(LALI)

// SLDLLD
// SLD m / LLD n
__attribute__((always_inline)) static inline void R_SLDLLD_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  SLDLLD %3d %3d", savedPC, run.getArg0(), run.getArg1());
	const CARD16 m = run.getArg0();
	const CARD16 n = run.getArg1();
	// 0 SLD m
	{
		CARD16 *p0 = StoreLF(m + 0);
		CARD16 *p1 = StoreLF(m + 1);
		*p1 = Pop();
		*p0 = Pop();
	}
	// 1 LLD n
	{
		const CARD16 *p0 = FetchLF(n + 0);
		const CARD16 *p1 = FetchLF(n + 1);
		Push(*p0);
		Push(*p1);
	}
}
DEF_R(SLDLLD)

// SLLL
// SL m / LL n
__attribute__((always_inline)) static inline void R_SLLL_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  SLLL %3d %3d", savedPC, run.getArg0(), run.getArg1());
	const CARD16 m = run.getArg0();
	const CARD16 n = run.getArg1();
	// 0 SLD m
	{
		CARD16 *p0 = StoreLF(m + 0);
		*p0 = Pop();
	}
	// 1 LLD n
	{
		const CARD16 *p0 = FetchLF(n + 0);
		Push(*p0);
	}
}
DEF_R(SLLL)
