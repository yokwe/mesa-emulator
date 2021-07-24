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
static log4cpp::Category& logger = Logger::getLogger("esc");

#include "../util/Debug.h"
#include "../util/GuiOp.h"
#include "../util/Perf.h"

#include "../mesa/MesaThread.h"

#include "Opcode.h"
#include "Interpreter.h"


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// 00  //ASSIGN_ESC(a, 00)
// 01  //ASSIGN_ESC(a, 01)
// 02  ASSIGN_ESC(a, MW)
// 03  ASSIGN_ESC(a, MR)
// 04  ASSIGN_ESC(a, NC)
// 05  ASSIGN_ESC(a, BC)
// 06  ASSIGN_ESC(a, REQ)
// 07  ASSIGN_ESC(a, SM)
void E_SM() {
	Memory::Map map;
	map.mf.u = Pop();
	map.rp = PopLong();
	CARD32 vp = PopLong();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  SM   vp = %08X  mf = %04X  rp = %08X", savedPC, vp, map.mf.u, map.rp);
	if (Vacant(map.mf)) map.rp = 0;
	Memory::WriteMap(vp, map);
}
// 010  ASSIGN_ESC(a, SMF)
void E_SMF() {
	MapFlags newMF = {Pop()};
	CARD32 vp = PopLong();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  SMF  vp = %08X  mf = %04X", savedPC, vp, newMF.u);
	Memory::Map map = Memory::ReadMap(vp);
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  SMF  rp = %08X  mf = %04X", savedPC, map.rp, map.mf.u);
	Push(map.mf.u);
	PushLong(map.rp);
	if (!Vacant(map.mf)) {
		map.mf = newMF;
		Memory::WriteMap(vp, map);
	}
}
// 011  ASSIGN_ESC(a, GMF)
void E_GMF() {
	CARD32 vp = PopLong();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  GMF  vp = %08X", savedPC, vp);
	Memory::Map map = Memory::ReadMap(vp);
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  GMF  rp = %08X  mf = %04X", savedPC, map.rp, map.mf.u);
	if (Vacant(map.mf)) map.rp = 0;
	Push(map.mf.u);
	PushLong(map.rp);
}
// 012  ASSIGN_ESC(a, AF)
// 013  ASSIGN_ESC(a, FF)
// 014  ASSIGN_ESC(a, PI)
// 015  ASSIGN_ESC(a, PO)
// 016  ASSIGN_ESC(a, POR)
// 017  ASSIGN_ESC(a, SPP)
// 020  ASSIGN_ESC(a, DI)
void E_DI() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  DI  %3d", savedPC, InterruptThread::getWDC());
	if (InterruptThread::getWDC() == cWDC) InterruptError();
	InterruptThread::disable();
}
// 021  ASSIGN_ESC(a, EI)
void E_EI() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  EI  %3d", savedPC, InterruptThread::getWDC());
	if (InterruptThread::getWDC() == 0) InterruptError();
	InterruptThread::enable();
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}
// 022  ASSIGN_ESC(a, XOR)
void E_XOR() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  XOR", savedPC);
	UNSPEC v = Pop();
	UNSPEC u = Pop();
	Push(u ^ v);
}
// 023  ASSIGN_ESC(a, DAND)
void E_DAND() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  DAND", savedPC);
	LONG_UNSPEC v = PopLong();
	LONG_UNSPEC u = PopLong();
	PushLong(u & v);
}
// 024  ASSIGN_ESC(a, DIOR)
void E_DIOR() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  DIOR", savedPC);
	LONG_UNSPEC v = PopLong();
	LONG_UNSPEC u = PopLong();
	PushLong(u | v);
}
// 025  ASSIGN_ESC(a, DXOR)
void E_DXOR() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  DXOR", savedPC);
	LONG_UNSPEC v = PopLong();
	LONG_UNSPEC u = PopLong();
	PushLong(u ^ v);
}
// 026  ASSIGN_ESC(a, ROTATE)
void E_ROTATE() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  ROTATE", savedPC);
	INT16 rotate = Pop();
	UNSPEC u = Pop();
	Push(Rotate(u, rotate));
}
// 027  ASSIGN_ESC(a, DSHIFT)
void E_DSHIFT() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  DSHIFT", savedPC);
	INT16 rotate = Pop();
	LONG_UNSPEC u = PopLong();
	PushLong(LongShift(u, rotate));
}
// 030  ASSIGN_ESC(a, LINT)
void E_LINT() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  LINT", savedPC);
	INT16 i = Pop();
	Push(i);
	Push((i < 0) ? (CARD16)0xffff : 0);
}
// 031  ASSIGN_ESC(a, JS)
void E_JS() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  JS", savedPC);
	PC = Pop();
	// ProcessorThread::checkRequestReschedule must be placed at very end of implementation of opcode.
	ProcessorThread::checkRequestReschedule();
}
// 032  ASSIGN_ESC(a, RCFS)
void E_RCFS() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  RCFS", savedPC);
	FieldDesc desc = {Pop()};
	CARDINAL offset = Pop();
	CARD8 spec = desc.field;
	Push(ReadField(ReadCode(offset + desc.offset), spec));
}
// 033  ASSIGN_ESC(b, RC)
void E_RC() {
	CARD16 arg = GetCodeByte();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  RC %02X", savedPC, arg);
	CARDINAL offset = Pop();
	CARD16 t = ReadCode(offset + arg);
	// NO PAGE FAULT AFTER HERE
	Push(t);
}
// 034  ASSIGN_ESC(a, UDIV)
void E_UDIV() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  UDIV", savedPC);
	CARDINAL t = Pop();
	CARDINAL s = Pop();
	if (t == 0) DivZeroTrap();
	Push(s / t);
	Push(s % t);
	Discard();
}
// 035  ASSIGN_ESC(a, LUDIV)
void E_LUDIV() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  LUDIV", savedPC);
	CARDINAL t = Pop();
	LONG_CARDINAL s = PopLong();
	if (t == 0) DivZeroTrap();
	if (t <= (CARDINAL)(s >> 16)) DivCheckTrap();
	Push(LowHalf(s / t));
	Push(LowHalf(s % t));
	Discard();
}
// 036  ASSIGN_ESC(b, ROB)
void E_ROB() {
	CARD16 arg = GetCodeByte();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  ROB %02X", savedPC, arg);
	POINTER ptr = Pop();
	CARD16* p = FetchMds(ptr - arg);
	// NO PAGE FAULT AFTER HERE
	Push(*p);
}
// 037  ASSIGN_ESC(b, WOB)
void E_WOB() {
	CARD16 arg = GetCodeByte();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  WOB %02X", savedPC, arg);
	POINTER ptr = Pop();
	CARD16* p = FetchMds(ptr - arg);
	// NO PAGE FAULT AFTER HERE
	*p = Pop();
}
// 040  ASSIGN_ESC(b, DSK)
void E_DSK() {
	CARD16 arg = GetCodeByte();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  DSK %02X", savedPC, arg);
	POINTER state = LFCache::LF() + arg;
	SaveStack(LengthenPointer(state));
}
// 041  ASSIGN_ESC(b, XE)
// 042  ASSIGN_ESC(b, XF)
// 043  ASSIGN_ESC(b, LSK)
void E_LSK() {
	CARD16 arg = GetCodeByte();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  LSK %02X", savedPC, arg);
	POINTER state = LFCache::LF() + arg;
	LoadStack(LengthenPointer(state));
}
// 044  ASSIGN_ESC(a, BNDCKL)
void E_BNDCKL() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  BNDCKL", savedPC);
	LONG_CARDINAL range = PopLong();
	LONG_CARDINAL index = PopLong();
	SP += 2; // PushLong(index);
	if (range <= index) BoundsTrap();
}
// 045  ASSIGN_ESC(a, NILCK)
void E_NILCK() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  NILCK", savedPC);
	POINTER ptr = Pop();
	SP++; // Push(ptr);
	if (ptr == 0) PointerTrap();
}
// 046  ASSIGN_ESC(a, NILCKL)
void E_NILCKL() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  NILCKL", savedPC);
	LONG_POINTER ptr = PopLong();
	SP += 2; // PushLong(ptr);
	if (ptr == 0) PointerTrap();
}
// 047  ASSIGN_ESC(a, BLTLR)
// 050  ASSIGN_ESC(a, BLEL)
// 051  ASSIGN_ESC(a, BLECL)
// 052  ASSIGN_ESC(a, CKSUM)
// 053  ASSIGN_ESC(a, BITBLT)
// 054  //ASSIGN_ESC(a, TXTBLT)
// 055  ASSIGN_ESC(a, BYTBLT)
// 056  ASSIGN_ESC(a, BYTBLTR)
// 057  ASSIGN_ESC(a, VERSION)
// 060  ASSIGN_ESC(a, DMUL)
void E_DMUL() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  DMUL", savedPC);
	LONG_CARDINAL t = PopLong();
	LONG_CARDINAL s = PopLong();
	PushLong((LONG_CARDINAL)(s * t));
}
// 061  ASSIGN_ESC(a, SDIV)
void E_SDIV() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  SDIV", savedPC);
	INT16 k = Pop();
	INT16 j = Pop();
	if (k == 0) DivZeroTrap();
	Push(j / k);
	Push(j % k);
	Discard();
}
// 062  ASSIGN_ESC(a, SDDIV)
void E_SDDIV() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  SDDIV", savedPC);
	INT32 k = PopLong();
	INT32 j = PopLong();
	if (k == 0) DivZeroTrap();
	PushLong(j / k);
	PushLong(j % k);
	Discard();
	Discard();
}
// 063  ASSIGN_ESC(a, UDDIV)
void E_UDDIV() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  UDDIV", savedPC);
	LONG_CARDINAL t = PopLong();
	LONG_CARDINAL s = PopLong();
	if (t == 0) DivZeroTrap();
	PushLong((CARD32)(s / t));
	PushLong((CARD32)(s % t));
	Discard();
	Discard();
}
// 064  //ASSIGN_ESC(a, 64)
// 065  //ASSIGN_ESC(a, 65)
// 066  //ASSIGN_ESC(a, 66)
// 067  //ASSIGN_ESC(a, 67)

// 070  //ASSIGN_ESC(a, 70)
// 071  //ASSIGN_ESC(a, 71)
// 072  //ASSIGN_ESC(a, 72)
// 073  //ASSIGN_ESC(a, 73)
// 074  //ASSIGN_ESC(a, 74)
// 075  //ASSIGN_ESC(a, 75)
// 076  //ASSIGN_ESC(a, 76)
// 077  //ASSIGN_ESC(a, 77)

// Floating Point (100B-137B are reserved)
// 0100  //ASSIGN_ESC(a, FADD)
// 0101  //ASSIGN_ESC(a, FSUB)
// 0102  //ASSIGN_ESC(a, FMUL)
// 0103  //ASSIGN_ESC(a, FDIV)
// 0104  //ASSIGN_ESC(a, FCOMP)
// 0105  //ASSIGN_ESC(a, FIX)
// 0106  //ASSIGN_ESC(a, FLOAT)
// 0107  //ASSIGN_ESC(a, FIXI)

// 0110  //ASSIGN_ESC(a, FIXC)
// 0111  //ASSIGN_ESC(a, FSTICKY)
// 0112  //ASSIGN_ESC(a, FREM)
// 0113  //ASSIGN_ESC(a, ROUND)
// 0114  //ASSIGN_ESC(a, ROUNDI)
// 0115  //ASSIGN_ESC(a, ROUNDC)
// 0116  //ASSIGN_ESC(a, FSQRT)
// 0117  //ASSIGN_ESC(a, FSC)

//  Read / Write Registers
// 0160  ASSIGN_ESC(a, WRPSB)
void E_WRPSB() {
	PSB = Index(Pop());
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  WRPSB  %4d", savedPC, PSB);
}
// 0161  ASSIGN_ESC(a, WRMDS)
void E_WRMDS() {
	CARD32 mds = Pop() << WordSize;
	Memory::setMDS(mds);
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  WRMDS  %04X", savedPC, (Memory::MDS() >> WordSize));
}
// 0162  ASSIGN_ESC(a, WRWP)
void E_WRWP() {
	CARD16 newValue = Pop();
	InterruptThread::setWP(newValue);
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  WRWP   %04X", savedPC, newValue);
}
// 0163  ASSIGN_ESC(a, WRWDC)
void E_WRWDC() {
	InterruptThread::setWDC(Pop());
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  WRWDC  %04X", savedPC, InterruptThread::getWDC());
}
// 0164  ASSIGN_ESC(a, WRPTC)
void E_WRPTC() {
	CARD16 newValue = Pop();
	TimerThread::setPTC(newValue);
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  WRPTC  %04X", savedPC, newValue);
//	lastTimeoutTime = Util::getMicroTime();
}
// 0165  ASSIGN_ESC(a, WRIT)
void E_WRIT() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  WRIT", savedPC);
	ERROR();
}
// 0166  ASSIGN_ESC(a, WRXTS)
void E_WRXTS() {
	XTS = Pop();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  WRXTS  %04X", savedPC, XTS);
}
// 0167  ASSIGN_ESC(a, WRMP)
void E_WRMP() {
	CARD16 newValue = Pop();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  WRMP   %4d", savedPC, newValue);
	GuiOp::setMP(newValue);
	ProcessorThread::setMP(newValue);

	switch(newValue) {
	case 900:
		//  cGerm: Code = 900;  -- Germ entered
		logger.info("MP  900 cGerm");
		break;
	case 910:
		//  cGermAction: Code = 910;  -- Germ action running (e.g. inLoad, outLoad)
		logger.info("MP  910 cGermAction");
		break;
	case 911:
		// cGermBadPhysicalVolume: Code = 911;  -- Germ and physical volume have incompatible version numbers
		logger.info("MP  911 cGermBadPhysicalVolume");
		break;
	case 912:
		// cGermBadBootFileVersion: Code = 912;  -- Germ and boot file have incompatible version numbers
		logger.info("MP  912 cGermBadBootFileVersion");
		break;
	case 917:
		// cRespondingToEtherDebugger: Code = 917;  -- talking to ethernet debugger
		logger.info("MP  917 cRespondingToEtherDebugger");
		break;
	case 915:
		// cWaitingForEtherDebugger: Code = 915;  -- waiting for ethernet debugger to begin debugging me
		logger.info("MP  915 cWaitingForEtherDebugger");
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
	case 922:
		//  cGermTimeout: Code = 922;  -- operation on boot device no completed in expected time
		logger.info("MP  922 cGermTimeout");
		break;
	case 924:
		//  cGermNoServer: Code = 924;  -- no response to Germ's request for ether boot file
		logger.info("MP  924 cGermNoServer");
		break;
	case 925:
		//  cGermFunnyPacket: Code = 925;  -- e.g. unexpected sequence number or size
		logger.info("MP  925 cGermFunnyPacket");
		break;
	case 927:
		//  cGermShortBootFile: Code = 927;  -- boot file ends before it should (try reinstalling)
		logger.info("MP  927 cGermShortBootFile");
		break;
	case 928:
		//  cWaitingForBootServer: Code = 928;  -- waiting for any boot server to respond
		logger.info("MP  928 cWaitingForBootServer");
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
		logger.info("MP %04d", newValue);
		break;
	}
}
// 0170  ASSIGN_ESC(a, RRPSB)
void E_RRPSB() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  RRPSB  %4d", savedPC, PSB);
	Push(Handle(PSB));
}
// 0171  ASSIGN_ESC(a, RRMDS)
void E_RRMDS() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  RRMDS  %04X", savedPC, (Memory::MDS() >> WordSize));
	Push((CARD16)(Memory::MDS() >> WordSize));
}
// 0172  ASSIGN_ESC(a, RRWP)
void E_RRWP() {
	CARD16 value = InterruptThread::getWP();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  RRWP   %04X", savedPC, value);
	Push(value);
}
// 0173  ASSIGN_ESC(a, RRWDC)
void E_RRWDC() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  RRWDC  %04X", savedPC, InterruptThread::getWDC());
	Push(InterruptThread::getWDC());
}
// 0174  ASSIGN_ESC(a, RRPTC)
void E_RRPTC() {
	CARD16 value = TimerThread::getPTC();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  RRPTC  %04X", savedPC, value);
	Push(value);
}
// 0175  ASSIGN_ESC(a, RRIT)
void E_RRIT() {
	CARD32 time = Util::getMicroTime();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  RRIT   %08X", savedPC, time);
	PushLong(time);
}
// 0176  ASSIGN_ESC(a, RRXTS)
void E_RRXTS() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  RRXTS  %04X", savedPC, XTS);
	Push(XTS);
}
// 0177  //ASSIGN_ESC(a, 177)

// Guam
//   GuamInputOutput.mesa
// 0211  ASSIGN_ESC(a, CALLAGENT)
// 0212  ASSIGN_ESC(a, MAPDISPLAY)
// 0213  ASSIGN_ESC(a, STOPEMULATOR)
// 0214  ASSIGN_ESC(a, 214)

// BitBltX.mesa
// 0302  //ASSIGN_ESC(a, BITBLTX)

// ColorBlt.mesa
// 0300  ASSIGN_ESC(a, COLORBLT)
// 0301  //ASSIGN_ESC(a, WRITEPIXEL)
// 0305  ASSIGN_ESC(a, 305)
// 0306  ASSIGN_ESC(a, 306)
