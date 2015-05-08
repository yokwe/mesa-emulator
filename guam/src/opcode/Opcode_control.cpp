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
// Opcode_control.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("control");

#include "../util/Debug.h"

#include "../mesa/Constant.h"
#include "../mesa/Type.h"
#include "../mesa/Memory.h"
#include "../mesa/Function.h"
#include "../mesa/MesaThread.h"

#include "Opcode.h"

// 9.5.2 Trap Processing
// Trap: PROC[ptr: POINTER TO ControlLink]
static inline void Trap(POINTER ptr) {
	ControlLink handler = ReadDblMds(ptr);
	PC = savedPC;
	SP = savedSP;
	if (ValidContext()) *StoreLF(LO_OFFSET(0, pc)) = PC;
	XFER(handler, LFCache::LF(), XT_trap, 0);
}

// TrapZero: PROC[ptr: POINTER TO ControlLink]
static inline void TrapZero(POINTER ptr) {
	Trap(ptr);
	ERROR_Abort();
}

// TrapOne: PROC[ptr POINTER TO ControlLink, parameter: UNSPEC]
static inline void TrapOne(POINTER ptr, UNSPEC parameter) {
	Trap(ptr);
	*StoreLF(0) = parameter;
	ERROR_Abort();
}

// TrapTwo: PROC[ptr POINTER TO ControlLink, parameter: LONG UNSPEC]
static inline void TrapTwo(POINTER ptr, UNSPEC parameter) {
	Trap(ptr);
	*StoreLF(0) = LowHalf(parameter);
	*StoreLF(1) = HighHalf(parameter);
	ERROR_Abort();
}

// 9.5.1 Trap Routines
void BoundsTrap() {
	if (DEBUG_SHOW_BOUNDS_TRAP) logger.debug("%s", __FUNCTION__);
	TrapZero(SD_OFFSET(sBoundsTrap));
}
void BreakTrap() {
	if (DEBUG_SHOW_BREAK_TRAP) logger.debug("%s", __FUNCTION__);
	TrapZero(SD_OFFSET(sBreakTrap));
}
void CodeTrap(GFTHandle gfi) {
	if (DEBUG_SHOW_CODE_TRAP) logger.debug("%s %04X", __FUNCTION__, gfi);
	TrapOne(SD_OFFSET(sCodeTrap), gfi);
}
void ControlTrap(ShortControlLink src) {
	if (DEBUG_SHOW_CONTROL_TRAP) logger.debug("%s %04X", __FUNCTION__, src);
	if (DEBUG_STOP_AT_CONTROL_TRAP) ERROR();
	TrapOne(SD_OFFSET(sControlTrap), src);
}
void DivCheckTrap() {
	if (DEBUG_SHOW_DIV_CHECK_TRAP) logger.debug("%s", __FUNCTION__);
	TrapZero(SD_OFFSET(sDivCheckTrap));
}
void DivZeroTrap() {
	if (DEBUG_SHOW_DIV_ZERO_TRAP) logger.debug("%s", __FUNCTION__);
	TrapZero(SD_OFFSET(sDivZeroTrap));
}
void EscOpcodeTrap(BYTE opcode) {
	if (DEBUG_SHOW_ESC_OPCODE_TRAP) logger.debug("%s %03o", __FUNCTION__, opcode);
	if (DEBUG_STOP_AT_OPCODE_TRAP) ERROR();
	TrapOne(ETT_OFFSET(opcode), opcode);
}
void InterruptError() {
	if (DEBUG_SHOW_INTERRUPT_ERROR) logger.debug("%s", __FUNCTION__);
	TrapZero(SD_OFFSET(sInterruptError));
}
void OpcodeTrap(BYTE opcode) {
	if (DEBUG_SHOW_OPCODE_TRAP) logger.debug("%s %03o", __FUNCTION__, opcode);
	if (DEBUG_STOP_AT_OPCODE_TRAP) ERROR();
	TrapOne(SD_OFFSET(sOpcodeTrap), opcode);
}
void PointerTrap() {
	if (DEBUG_SHOW_POINTER_TRAP) logger.debug("%s", __FUNCTION__);
	TrapZero(SD_OFFSET(sPointerTrap));
}
void ProcessTrap() {
	if (DEBUG_SHOW_PROCESS_TRAP) logger.debug("%s", __FUNCTION__);
	TrapZero(SD_OFFSET(sProcessTrap));
}
void RescheduleError() {
	if (DEBUG_SHOW_RESCHEDULE_TRAP) logger.debug("%s", __FUNCTION__);
	TrapZero(SD_OFFSET(sRescheduleError));
}
void StackError() {
	if (DEBUG_SHOW_STACK_ERROR) logger.debug("%s", __FUNCTION__);
	if (DEBUG_STOP_AT_STACK_ERROR) {
		logger.fatal("StackError  SP = %d savedSP = %d", SP, savedSP);
		logger.fatal("GFI = %04X  CB = %8X  PC = %4X  savedPC = %4X", GFI, CodeCache::CB(), PC, savedPC);
		ERROR();
	}
	TrapZero(SD_OFFSET(sStackError));
}
void UnboundTrap(ControlLink dst) {
	if (DEBUG_SHOW_UNBOUND_TRAP) logger.debug("%s %08X", __FUNCTION__, dst);
	if (DEBUG_STOP_AT_UNBOUND_TRAP) ERROR();
	TrapTwo(SD_OFFSET(sUnboundTrap), dst);
}
void HardwareError() {
	if (DEBUG_SHOW_HARDWARE_ERROR) logger.debug("%s", __FUNCTION__);
	TrapZero(SD_OFFSET(sHardwareError));
}

// 9.5.5 Xfer Traps
// CheckForXferTraps: PROC[dst: ConrolLink, type: XferType]
static inline void CheckForXferTraps(ControlLink dst, int type) {
	if (Odd(XTS)) {
		GlobalWord word = {*Fetch(GO_OFFSET(GF, word))};
		if (word.trapxfers) {
			XTS = XTS >> 1;
			if (DEBUG_SHOW_XFER_TRAP) logger.debug("XferTrap %08X %d", dst, type);
			Trap(SD_OFFSET(sXferTrap));
			*StoreLF(0) = LowHalf(dst);
			*StoreLF(1) = HighHalf(dst);
			*StoreLF(2) = type;
			ERROR_Abort();
		}
	} else {
		XTS = XTS >> 1;
	}
}


// 9.2.2 Frame Allocation Primitives

// Alloc: PROC[fsi: FSIndex] RETURNS[LocalFrameHandle]
static inline LocalFrameHandle Alloc(FSIndex fsi) {
	AVItem item;
	FSIndex slot = fsi;
	for(;;) {
		item.u = *FetchMds(AV_OFFSET(slot));
		if (item.tag != AT_indirect) break;
		if (FSIndex_SIZE <= item.data) ERROR();
		slot = item.data;
	}
	if (item.tag == AT_empty) FrameFault(fsi);
	*StoreMds(AV_OFFSET(slot)) = *FetchMds(AVLink(item.u));
	return AVFrame(item.u);
}

// Free: PROC[frame: LocalFrameHandle]
static inline void Free(LocalFrameHandle frame) {
	LocalWord word = {*FetchMds(LO_OFFSET(frame, word))};
	AVItem item = {*FetchMds(AV_OFFSET(word.fsi))};
	*StoreMds(frame) = item.u;
	*StoreMds(AV_OFFSET(word.fsi)) = frame;
}

// 9.3 Control Transfer Primitives

// XFER: PROC[dst: ControlLink, src: ShortControlLink, type: XferType, free: BOOLEAN = FALSE]
void XFER(ControlLink dst, ShortControlLink src, XferType type, int freeFlag = 0) {
	if (DEBUG_TRACE_XFER) logger.debug("XFER  dst = %08X  src = %04X  type = %d  freeFlag = %d", dst, src, type, freeFlag);

	CARDINAL nPC;
	CARDINAL nLF;
	int push = 0;
	ControlLink nDst = dst;

	if (type == XT_trap && freeFlag) ERROR();
	while (ControlLinkType(nDst) == LT_indirect ) {
		IndirectLink link = MakeIndirectLink(nDst);
		if (type == XT_trap) ERROR();
		nDst = ReadDblMds(link);
		push = 1;
	}

	switch (ControlLinkType(nDst)) {
	case LT_oldProcedure : {
		if (DEBUG_TRACE_XFER) logger.debug("XFER  LT_oldProcedure");
		ProcDesc proc = {MakeProcDesc(nDst)};
		CARD16 gf = proc.taggedGF & 0xfffc; // 177774
		if (DEBUG_TRACE_XFER) logger.debug("XFER  gf  = %04X", gf);
		if (gf == 0) UnboundTrap(dst);
		GFI = *FetchMds(GO_OFFSET(gf, word)) & 0xfffc; // 177774
		if (DEBUG_TRACE_XFER) logger.debug("XFER  GFI = %04X", GFI);
		if (GFI == 0) UnboundTrap(dst);
		GF = ReadDbl(GFT_OFFSET(GFI, globalFrame));
		if (GF != LengthenPointer(gf)) ERROR(); // Sanity check
		CodeCache::setCB(ReadDbl(GFT_OFFSET(GFI, codebase)));
		if (DEBUG_TRACE_XFER) logger.debug("XFER  GF  = %08X  CB = %08X", GF, CodeCache::CB());
		if (CodeCache::CB() & 1) CodeTrap(GFI);
		nPC = proc.pc;
		if (DEBUG_TRACE_XFER) logger.debug("XFER  nPC = %6o", nPC);
		if (nPC == 0) UnboundTrap(dst);
		BytePair word = { ReadCode(nPC / 2) };
		FSIndex fsi = ((nPC % 2) == 0) ? word.left : word.right;
		if (DEBUG_TRACE_XFER) logger.debug("XFER  fsi = %2d", fsi);
		nLF = Alloc(fsi);
		if (DEBUG_TRACE_XFER) logger.debug("XFER  nLF = %04X", nLF);
		nPC = nPC + 1;
		*StoreMds(LO_OFFSET(nLF, globallink)) = GFI;
		*StoreMds(LO_OFFSET(nLF, returnlink)) = src;
	}
		break;
	case LT_frame : {
		if (DEBUG_TRACE_XFER) logger.debug("XFER  LT_frame");
		FrameLink frame = {MakeFrameLink(nDst)};
		if (DEBUG_TRACE_XFER) logger.debug("XFER  frame = %04X", frame);
		if (frame == 0) ControlTrap(src);
		nLF = frame;
		if (DEBUG_TRACE_XFER) logger.debug("XFER  nLF = %04X", nLF);
		GFI = *FetchMds(LO_OFFSET(nLF, globallink));
		if (DEBUG_TRACE_XFER) logger.debug("XFER  GFI = %04X", GFI);
		if (GFI == 0) UnboundTrap(dst);
		GF = ReadDbl(GFT_OFFSET(GFI, globalFrame));
		CodeCache::setCB(ReadDbl(GFT_OFFSET(GFI, codebase)));
		if (DEBUG_TRACE_XFER) logger.debug("XFER  GF  = %08X  CB = %08X", GF, CodeCache::CB());
		if (CodeCache::CB() & 1) CodeTrap(GFI);
		nPC = *FetchMds(LO_OFFSET(nLF, pc));
		if (DEBUG_TRACE_XFER) logger.debug("XFER  nPC = %6o", nPC);

		if (nPC == 0) UnboundTrap(dst);
		if (type == XT_trap) {
			*StoreMds(LO_OFFSET(nLF, returnlink)) = src;
			InterruptThread::disable();
		}
	}
		break;
	case LT_newProcedure : {
		if (DEBUG_TRACE_XFER) logger.debug("XFER  LT_newProcedure");
		NewProcDesc proc = {MakeNewProcDesc(nDst)};
		GFI = proc.taggedGFI & 0xfffc; // 177774
		if (DEBUG_TRACE_XFER) logger.debug("XFER  GFI = %04X", GFI);
		if (GFI == 0) UnboundTrap(dst);
		GF = ReadDbl(GFT_OFFSET(GFI, globalFrame));
		CodeCache::setCB(ReadDbl(GFT_OFFSET(GFI, codebase)));
		if (DEBUG_TRACE_XFER) logger.debug("XFER  GF  = %08X  CB = %08X", GF, CodeCache::CB());
		if (CodeCache::CB() & 1) CodeTrap(GFI);
		nPC = proc.pc;
		if (DEBUG_TRACE_XFER) logger.debug("XFER  nPC = %6o", nPC);
		if (nPC == 0) UnboundTrap(dst);
		BytePair word = { ReadCode(nPC / 2) };
		FSIndex fsi = ((nPC % 2) == 0) ? word.left : word.right;
		if (DEBUG_TRACE_XFER) logger.debug("XFER  fsi = %2d", fsi);
		nLF = Alloc(fsi);
		if (DEBUG_TRACE_XFER) logger.debug("XFER  nLF = %04X", nLF);
		nPC = nPC + 1;
		*StoreMds(LO_OFFSET(nLF, globallink)) = GFI;
		*StoreMds(LO_OFFSET(nLF, returnlink)) = src;
	}
		break;
	default:
		ERROR();
		nPC = 0;
		nLF = 0;
		break;
	}

	if (push) {
		Push((CARD16)dst);
		Push(src);
		Discard();
		Discard();
	}
	if (freeFlag)
		Free(LFCache::LF());
	LFCache::setLF(nLF);
	PC = nPC;
	CheckForXferTraps(dst, type);
}

// 9.4.2 External Function Calls
// Call: PROC[dst: ControlLink]
static inline void Call(ControlLink dst) {
	*StoreLF(LO_OFFSET(0, pc)) = PC;
	XFER(dst, LFCache::LF(), XT_call, 0);
}
///////////////////////////////////////////////////////////////////////

#define DEF_CI_XFER(name) \
Run C_##name(Opcode* opcode_) { \
	return C_##name##_(opcode_); \
} \
void I_##name(Opcode* opcode) { \
	Run run = C_##name##_(opcode); \
	PC = savedPC + L_##name; \
	R_##name##_(run); \
}

#define DEF_CI_n_XFER(name, n) \
Run C_##name##n(Opcode* opcode_) { \
	return C_##name##n##_(opcode_); \
} \
void I_##name##n(Opcode* opcode) { \
	Run run = C_##name##n##_(opcode); \
	PC = savedPC + L_##name##n; \
	R_##name##_(run); \
}

#define DEF_C_0_XFER(name) \
__attribute__((always_inline)) static inline Run C_##name##_(Opcode *opcode) { \
	Run ret(r##name, L_##name); \
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s", savedPC, opcode->getName()); \
	return ret; \
} \
DEF_CI_XFER(name)

#define DEF_C_0_n_XFER(name, n) \
__attribute__((always_inline)) static inline Run C_##name##n##_(Opcode *opcode) { \
	Run ret(r##name, L_##name##n, (CARD16)(n)); \
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s", savedPC, opcode->getName()); \
	return ret; \
} \
DEF_CI_n_XFER(name, n)

#define DEF_C_B_XFER(name) \
__attribute__((always_inline)) static inline Run C_##name##B##_(Opcode *opcode) { \
	Run ret(r##name, L_##name##B, (CARD16)GetCodeByte()); \
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s %3d", savedPC, opcode->getName(), ret.getArg()); \
	return ret; \
} \
DEF_CI_n_XFER(name, B)

#define DEF_C_B_N_XFER(name) \
__attribute__((always_inline)) static inline Run C_##name##_(Opcode *opcode) { \
	Run ret(r##name, L_##name, (CARD16)GetCodeByte()); \
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s %3d", savedPC, opcode->getName(), ret.getArg()); \
	return ret; \
} \
DEF_CI_XFER(name)


#define DEF_C_W_N_XFER(name) \
__attribute__((always_inline)) static inline Run C_##name##_(Opcode *opcode) { \
	Run ret(r##name, L_##name, (CARD16)GetCodeWord()); \
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s %5d", savedPC, opcode->getName(), ret.getArg()); \
	return ret; \
} \
DEF_CI_XFER(name)

#define DEF_CI_r_XFER(name, r) \
Run C_##name(Opcode* opcode_) { \
	return C_##name##_(opcode_); \
} \
void I_##name(Opcode* opcode) { \
	Run run = C_##name##_(opcode); \
	PC = savedPC + L_##name; \
	R_##r##_(run); \
}


// zEFC0 - 0337
__attribute__((always_inline)) static inline void R_EFC_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  EFC %5d", savedPC, run.getArg());
	Call(FetchLink(run.getArg()));
}
DEF_R(EFC)
DEF_C_0_n_XFER(EFC, 0)


// zEFC1 - 0340
DEF_C_0_n_XFER(EFC, 1)
// zEFC2 - 0341
DEF_C_0_n_XFER(EFC, 2)
// zEFC3 - 0342
DEF_C_0_n_XFER(EFC, 3)
// zEFC4 - 0343
DEF_C_0_n_XFER(EFC, 4)
// zEFC5 - 0344
DEF_C_0_n_XFER(EFC, 5)
// zEFC6 - 0345
DEF_C_0_n_XFER(EFC, 6)
// zEFC7 - 0346
DEF_C_0_n_XFER(EFC, 7)
// zEFC8 - 0347
DEF_C_0_n_XFER(EFC, 8)


// zEFC9 - 0350
DEF_C_0_n_XFER(EFC, 9)
// zEFC10 - 0351
DEF_C_0_n_XFER(EFC, 10)
// zEFC11 - 0352
DEF_C_0_n_XFER(EFC, 11)
// zEFC12 - 0353
DEF_C_0_n_XFER(EFC, 12)
// zEFCB - 0354
DEF_C_B_XFER(EFC)
// zLFC - 0355
__attribute__((always_inline)) static inline void R_LFC_(Run run) {
	//CARDINAL nPC = GetCodeWord();
	CARDINAL nPC = run.getArg();
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  zLFC %04X", savedPC, nPC);
	if (DEBUG_TRACE_XFER) logger.debug("LFC   nPC = %6o", nPC);
	*StoreLF(LO_OFFSET(0, pc)) = PC;
	if (nPC == 0) UnboundTrap(0);
	BytePair word = { ReadCode(nPC / 2) };
	FSIndex fsi = ((nPC % 2) == 0) ? word.left : word.right;
	if (DEBUG_TRACE_XFER) logger.debug("LFC   fsi = %2d", fsi);
	CARD16 nLF = Alloc(fsi);
	if (DEBUG_TRACE_XFER) logger.debug("LFC   nLF = %04X", nLF);
	nPC = nPC + 1;
	*StoreMds(LO_OFFSET(nLF, globallink)) = GFI;
	*StoreMds(LO_OFFSET(nLF, returnlink)) = LFCache::LF();
	LFCache::setLF(nLF);
	PC = nPC;

	ProcDesc dst;
	dst.taggedGF = GFI | 1;
	dst.pc = PC - 1;
	CheckForXferTraps(dst.u, XT_localCall);
}
DEF_R(LFC)
DEF_C_W_N_XFER(LFC)
// zSFC - 0356
__attribute__((always_inline)) static inline void R_SFC_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  SFC", savedPC);
	ControlLink link = PopLong();
	Call(link);
}
DEF_R(SFC)
DEF_C_0_XFER(SFC)
// zRET - 0357
__attribute__((always_inline)) static inline void R_RET_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  RET", savedPC);
	ControlLink dst = {*FetchLF(LO_OFFSET(0, returnlink))};
	XFER(dst, 0, XT_return, 1);
}
DEF_R(RET)
DEF_C_0_XFER(RET)


// zKFCB - 0360
__attribute__((always_inline)) static inline void R_KFC_(Run run) {
	//SDIndex alpha = GetCodeByte();
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  KFCB %02X", savedPC, run.getArg());
	Call(ReadDblMds(SD_OFFSET(run.getArg())));
}
DEF_R(KFC)
DEF_C_B_XFER(KFC)
// aAF - 012
__attribute__((always_inline)) static inline void R_AF_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  AF", savedPC);
	FSIndex fsi = Pop();
	Push(Alloc(fsi));
}
DEF_R(AF)
DEF_C_0(AF)
// aFF - 013
__attribute__((always_inline)) static inline void R_FF_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  FF", savedPC);
	LocalFrameHandle frame = Pop();
	Free(frame);
}
DEF_R(FF)
DEF_C_0(FF)
// aPI - 014
__attribute__((always_inline)) static inline void R_PI_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  PI", savedPC);
	Recover();
	Recover();
	ShortControlLink src = Pop();
	PortLink port = Pop();
	*StoreMds(PORT_OFFSET(port, inport)) = 0;
	if (src) *StoreMds(PORT_OFFSET(port, outport)) = src;
}
DEF_R(PI)
DEF_C_0(PI)
// aPO - 015
__attribute__((always_inline)) static inline void R_PO_(Run /*run*/) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  PO", savedPC);
	/* UNSPEC reserved = */ Pop();
	PortLink port = Pop();
	*StoreLF(LO_OFFSET(0, pc)) = PC;
	*StoreMds(PORT_OFFSET(port, inport)) = LFCache::LF();
	XFER(ReadDblMds(PORT_OFFSET(port, outport)), port, XT_port, 0);
}
DEF_R(PO)
DEF_C_0_XFER(PO)
// aPOR - 016
__attribute__((always_inline)) static inline Run C_POR_(Opcode *opcode) {
	Run ret(rPO, L_POR);
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s", savedPC, opcode->getName());
	return ret;
}
DEF_CI_r_XFER(POR, PO)


// bXE - 041
__attribute__((always_inline)) static inline void R_XE_(Run run) {
	if (InterruptThread::getWDC() == 0) InterruptError();
	InterruptThread::enable();
	//*StoreMds(LO_OFFSET(LF, pc)) = PC + 1; // plus 1 for GetCodeByte. PC + 1 point to next instruction.
	*StoreLF(LO_OFFSET(0, pc)) = PC; // Store location of next instruction
	try {
		//POINTER ptr = LF + GetCodeByte();
		POINTER ptr = run.getArg();
		if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  XE %02X", savedPC, ptr);
		ControlLink      dst = ReadDblLF(ptr + OFFSET(TransferDescriptor, dst));
		ShortControlLink src = *FetchLF (ptr + OFFSET(TransferDescriptor, src));
		XFER(dst, src, XT_xfer, 0);
	} catch(Abort &abort) {
		ERROR();
	}
}
DEF_R(XE)
DEF_C_B_N_XFER(XE)
// bXF - 042
__attribute__((always_inline)) static inline void R_XF_(Run run) {
	//*StoreMds(LO_OFFSET(LF, pc)) = PC + 1; // plus 1 for GetCodeByte. PC + 1 point to next instruction.
	*StoreLF(LO_OFFSET(0, pc)) = PC;  // Store location of next instruction
	//POINTER ptr = LF + GetCodeByte();
	POINTER ptr = run.getArg();
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  XF %02X", savedPC, ptr);
	ControlLink      dst = ReadDblLF(ptr + OFFSET(TransferDescriptor, dst));
	ShortControlLink src = *FetchLF (ptr + OFFSET(TransferDescriptor, src));
	XFER(dst, src, XT_xfer, 1);
}
DEF_R(XF)
DEF_C_B_N_XFER(XF)


__attribute__((always_inline)) static inline void R_AFQ_(Run run) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  AFQ %d", savedPC, run.getArg());
	Push(Alloc((CARD8)run.getArg()));
}
DEF_R(AFQ)
