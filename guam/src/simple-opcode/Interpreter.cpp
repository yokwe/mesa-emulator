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
// Interpreter.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("Interpreter");

#include "../util/Debug.h"
#include "../util/Perf.h"

#include "../mesa/MesaThread.h"

#include "Interpreter.h"

Opcode    Interpreter::tableMop[Interpreter::TABLE_SIZE];
Opcode    Interpreter::tableEsc[Interpreter::TABLE_SIZE];
long long Interpreter::statMop [Interpreter::TABLE_SIZE];
long long Interpreter::statEsc [Interpreter::TABLE_SIZE];


void Interpreter::assignMop(Opcode::EXEC exec_, const char* name_, CARD32 code_, CARD32 size_) {
	if (exec_ == 0) {
		logger.fatal("assignMop exec_ == 0  code = %d", code_);
		ERROR();
	}
	if (name_ == 0) {
		logger.fatal("assignMop name_ == 0  code = %d", code_);
		ERROR();
	}

	if (!tableMop[code_].isEmpty()) {
		logger.fatal("assignMop code = %3o  old name = %s  new name = %s", code_, tableMop[code_].getName(), name_);
		ERROR();
	}

	Opcode opcode (exec_, name_, code_, size_);
	tableMop[code_] = opcode;
}
void Interpreter::assignEsc(Opcode::EXEC exec_, const char* name_, CARD32 code_, CARD32 size_) {
	if (exec_ == 0) {
		logger.fatal("assignEsc exec_ == 0  code = %d", code_);
		ERROR();
	}
	if (name_ == 0) {
		logger.fatal("assignEsc name_ == 0  code = %d", code_);
		ERROR();
	}

	if (!tableEsc[code_].isEmpty()) {
		logger.fatal("assignEsc code = %3o  old name = %s  new name = %s", code_, tableEsc[code_].getName(), name_);
		ERROR();
	}

	Opcode opcode (exec_, name_, code_, size_);
	tableEsc[code_] = opcode;
}

static void mopOpcodeTrap(Opcode* opcode) {
	OpcodeTrap((CARD8)opcode->getCode());
}
static void escOpcodeTrap(Opcode* opcode) {
	EscOpcodeTrap((CARD8)opcode->getCode());
}

void Interpreter::fillOpcodeTrap() {
	for(CARD32 i = 0; i < TABLE_SIZE; i++) {
		if (tableMop[i].isEmpty()) assignMop(mopOpcodeTrap, "__MOP_OPCODE_TRAP__", i, 1); // can be 1, 2 or 3
		if (tableEsc[i].isEmpty()) assignEsc(escOpcodeTrap, "__ESC_OPCODE_TRAP__", i, 2); // can bw 2 or 3
	}
}


void Interpreter::stats() {
	if (DEBUG_SHOW_OPCODE_STATS) {
		long long total = 0;
		logger.info("==== Interpreter stats  START");
		for(int i = 0; i < TABLE_SIZE; i++) {
			Opcode *op = tableMop + i;
			if (statMop[i] == 0 && op->isEqual(mopOpcodeTrap)) continue;
			logger.info("stats mop  %3o  %-16s  %10lld", op->getCode(), op->getName(), statMop[i]);
			total += statMop[i];
		}
		for(int i = 0; i < TABLE_SIZE; i++) {
			Opcode *op = tableEsc + i;
			if (statEsc[i] == 0 && op->isEqual(escOpcodeTrap)) continue;
			logger.info("stats esc  %3o  %-16s  %10lld", op->getCode(), op->getName(), statEsc[i]);
			total += statEsc[i];
		}
		logger.info("total = %lld", total);
		logger.info("==== Interpreter stats  STOP");
	}
}


void Interpreter::initRegisters() {
	// Processor ID
	PID[0] = 0x0000;
	PID[1] = 0x0000;
	PID[2] = 0x0000;
	PID[3] = 0x0000;
	MP  = 0;    // Maintenance Panel
//	IT  = 0;    // Interval Timer
//	WM  = 0;    // Wakeup mask register - 10.4.4
//	WP  = 0;    // Wakeup pending register - 10.4.4.1
	InterruptThread::setWP(0);
//	WDC = 1;    // Wakeup disable counter - 10.4.4.3
	InterruptThread::setWDC(1);
//	PTC = 0;    // Process timeout counter - 10.4.5
	TimerThread::setPTC(0);
	XTS = 0;    // Xfer trap status - 9.5.5

	// 3.3.1 Control Registers
	PSB = 0; // PsbIndex - 10.1.1
//	MDS = 0;
	Memory::setMDS(0);
//	LF  = 0;  // POINTER TO LocalVariables
	LFCache::setLF(0);
	GF  = 0;  // LONG POINTER TO GlobalVarables
	CodeCache::setCB(0);  // LONG POINTER TO CodeSegment
	PC  = 0;
	GFI = 0;

	for(int i = 0; i < StackDepth; i++) stack[i] = 0;
	SP = 0;

	// 4.5 Instruction Execution
	breakByte = 0;
	savedPC   = 0;
	savedSP   = 0;

	// 10.4.1 Scheduler
	//running = 1;
	ProcessorThread::startRunning();

	// 10.4.5 Timeouts
    //lastTimeoutTime = 0;
}

#define ASSIGN_MOP(prefix, name) Interpreter::assignMop(E_##name, #name, prefix##name, L_##name);
#define ASSIGN_ESC(prefix, name) Interpreter::assignEsc(E_##name, #name, prefix##name, L_##name);

//#define ASSIGN_MOP(prefix, name) Interpreter::assignMop(Opcode(prefix##name, L_##name, #name, C_##name, I_##name));
//#define ASSIGN_ESC(prefix, name) Interpreter::assignEsc(Opcode(prefix##name, L_##name, #name, C_##name, I_##name));

void Interpreter::initTable() {
	/* 00 */ ASSIGN_MOP(z, NOOP)
	/* 01 */ ASSIGN_MOP(z, LL0)
	/* 02 */ ASSIGN_MOP(z, LL1)
	/* 03 */ ASSIGN_MOP(z, LL2)
	/* 04 */ ASSIGN_MOP(z, LL3)
	/* 05 */ ASSIGN_MOP(z, LL4)
	/* 06 */ ASSIGN_MOP(z, LL5)
	/* 07 */ ASSIGN_MOP(z, LL6)

	/* 010 */ ASSIGN_MOP(z, LL7)
	/* 011 */ ASSIGN_MOP(z, LL8)
	/* 012 */ ASSIGN_MOP(z, LL9)
	/* 013 */ ASSIGN_MOP(z, LL10)
	/* 014 */ ASSIGN_MOP(z, LL11)
	/* 015 */ ASSIGN_MOP(z, LLB)
	/* 016 */ ASSIGN_MOP(z, LLD0)
	/* 017 */ ASSIGN_MOP(z, LLD1)

	/* 020 */ ASSIGN_MOP(z, LLD2)
	/* 021 */ ASSIGN_MOP(z, LLD3)
	/* 022 */ ASSIGN_MOP(z, LLD4)
	/* 023 */ ASSIGN_MOP(z, LLD5)
	/* 024 */ ASSIGN_MOP(z, LLD6)
	/* 025 */ ASSIGN_MOP(z, LLD7)
	/* 026 */ ASSIGN_MOP(z, LLD8)
	/* 027 */ ASSIGN_MOP(z, LLD10)

	/* 030 */ ASSIGN_MOP(z, LLDB)
	/* 031 */ ASSIGN_MOP(z, SL0)
	/* 032 */ ASSIGN_MOP(z, SL1)
	/* 033 */ ASSIGN_MOP(z, SL2)
	/* 034 */ ASSIGN_MOP(z, SL3)
	/* 035 */ ASSIGN_MOP(z, SL4)
	/* 036 */ ASSIGN_MOP(z, SL5)
	/* 037 */ ASSIGN_MOP(z, SL6)

	/* 040 */ ASSIGN_MOP(z, SL7)
	/* 041 */ ASSIGN_MOP(z, SL8)
	/* 042 */ ASSIGN_MOP(z, SL9)
	/* 043 */ ASSIGN_MOP(z, SL10)
	/* 044 */ ASSIGN_MOP(z, SLB)
	/* 045 */ ASSIGN_MOP(z, SLD0)
	/* 046 */ ASSIGN_MOP(z, SLD1)
	/* 047 */ ASSIGN_MOP(z, SLD2)

	/* 050 */ ASSIGN_MOP(z, SLD3)
	/* 051 */ ASSIGN_MOP(z, SLD4)
	/* 052 */ ASSIGN_MOP(z, SLD5)
	/* 053 */ ASSIGN_MOP(z, SLD6)
	/* 054 */ ASSIGN_MOP(z, SLD8)
	/* 055 */ ASSIGN_MOP(z, PL0)
	/* 056 */ ASSIGN_MOP(z, PL1)
	/* 057 */ ASSIGN_MOP(z, PL2)

	/* 060 */ ASSIGN_MOP(z, PL3)
	/* 061 */ ASSIGN_MOP(z, PLB)
	/* 062 */ ASSIGN_MOP(z, PLD0)
	/* 063 */ ASSIGN_MOP(z, PLDB)
	/* 064 */ ASSIGN_MOP(z, LG0)
	/* 065 */ ASSIGN_MOP(z, LG1)
	/* 066 */ ASSIGN_MOP(z, LG2)
	/* 067 */ ASSIGN_MOP(z, LGB)

	/* 070 */ ASSIGN_MOP(z, LGD0)
	/* 071 */ ASSIGN_MOP(z, LGD2)
	/* 072 */ ASSIGN_MOP(z, LGDB)
	/* 073 */ ASSIGN_MOP(z, SGB)
	/* 074 */ ASSIGN_MOP(z, BNDCK)
	/* 075 */ ASSIGN_MOP(z, BRK)

	/* 077 */ //ASSIGN_MOP(z, STC)

	/* 0100 */ ASSIGN_MOP(z, R0)
	/* 0101 */ ASSIGN_MOP(z, R1)
	/* 0102 */ ASSIGN_MOP(z, RB)
	/* 0103 */ ASSIGN_MOP(z, RL0)
	/* 0104 */ ASSIGN_MOP(z, RLB)
	/* 0105 */ ASSIGN_MOP(z, RD0)
	/* 0106 */ ASSIGN_MOP(z, RDB)
	/* 0107 */ ASSIGN_MOP(z, RDL0)

	/* 0110 */ ASSIGN_MOP(z, RDLB)
	/* 0111 */ ASSIGN_MOP(z, W0)
	/* 0112 */ ASSIGN_MOP(z, WB)
	/* 0113 */ ASSIGN_MOP(z, PSB)
	/* 0114 */ ASSIGN_MOP(z, WLB)
	/* 0115 */ ASSIGN_MOP(z, PSLB)
	/* 0116 */ ASSIGN_MOP(z, WDB)
	/* 0117 */ ASSIGN_MOP(z, PSD0)

	/* 0120 */ ASSIGN_MOP(z, PSDB)
	/* 0121 */ ASSIGN_MOP(z, WDLB)
	/* 0122 */ ASSIGN_MOP(z, PSDLB)
	/* 0123 */ ASSIGN_MOP(z, RLI00)
	/* 0124 */ ASSIGN_MOP(z, RLI01)
	/* 0125 */ ASSIGN_MOP(z, RLI02)
	/* 0126 */ ASSIGN_MOP(z, RLI03)
	/* 0127 */ ASSIGN_MOP(z, RLIP)

	/* 0130 */ ASSIGN_MOP(z, RLILP)
	/* 0131 */ ASSIGN_MOP(z, RLDI00)
	/* 0132 */ ASSIGN_MOP(z, RLDIP)
	/* 0133 */ ASSIGN_MOP(z, RLDILP)
	/* 0134 */ ASSIGN_MOP(z, RGIP)
	/* 0135 */ ASSIGN_MOP(z, RGILP)
	/* 0136 */ ASSIGN_MOP(z, WLIP)
	/* 0137 */ ASSIGN_MOP(z, WLILP)

	/* 0140 */ ASSIGN_MOP(z, WLDILP)
	/* 0141 */ ASSIGN_MOP(z, RS)
	/* 0142 */ ASSIGN_MOP(z, RLS)
	/* 0143 */ ASSIGN_MOP(z, WS)
	/* 0144 */ ASSIGN_MOP(z, WLS)
	/* 0145 */ ASSIGN_MOP(z, R0F)
	/* 0146 */ ASSIGN_MOP(z, RF)
	/* 0147 */ ASSIGN_MOP(z, RL0F)

	/* 0150 */ ASSIGN_MOP(z, RLF)
	/* 0151 */ ASSIGN_MOP(z, RLFS)
	/* 0152 */ ASSIGN_MOP(z, RLIPF)
	/* 0153 */ ASSIGN_MOP(z, RLILPF)
	/* 0154 */ ASSIGN_MOP(z, W0F)
	/* 0155 */ ASSIGN_MOP(z, WF)
	/* 0156 */ ASSIGN_MOP(z, PSF)
	/* 0157 */ ASSIGN_MOP(z, PS0F)

	/* 0160 */ ASSIGN_MOP(z, WS0F)
	/* 0161 */ ASSIGN_MOP(z, WL0F)
	/* 0162 */ ASSIGN_MOP(z, WLF)
	/* 0163 */ ASSIGN_MOP(z, PSLF)
	/* 0164 */ ASSIGN_MOP(z, WLFS)
	/* 0165 */ ASSIGN_MOP(z, SLDB)
	/* 0166 */ ASSIGN_MOP(z, SGDB)
	/* 0167 */ ASSIGN_MOP(z, LLKB)

	/* 0170 */ ASSIGN_MOP(z, RKIB)
	/* 0171 */ ASSIGN_MOP(z, RKDIB)
	/* 0172 */ ASSIGN_MOP(z, LKB)
	/* 0173 */ ASSIGN_MOP(z, SHIFT)
	/* 0174 */ ASSIGN_MOP(z, SHIFTSB)
	/* 0175 */ //ASSIGN_MOP(z, MBP)
	/* 0176 */ //ASSIGN_MOP(z, RBP)
	/* 0177 */ //ASSIGN_MOP(z, WBP)


	/* 0200 */ ASSIGN_MOP(z, CATCH)
	/* 0201 */ ASSIGN_MOP(z, J2)
	/* 0202 */ ASSIGN_MOP(z, J3)
	/* 0203 */ ASSIGN_MOP(z, J4)
	/* 0204 */ ASSIGN_MOP(z, J5)
	/* 0205 */ ASSIGN_MOP(z, J6)
	/* 0206 */ ASSIGN_MOP(z, J7)
	/* 0207 */ ASSIGN_MOP(z, J8)

	/* 0210 */ ASSIGN_MOP(z, JB)
	/* 0211 */ ASSIGN_MOP(z, JW)
	/* 0212 */ ASSIGN_MOP(z, JEP)
	/* 0213 */ ASSIGN_MOP(z, JEB)
	/* 0214 */ ASSIGN_MOP(z, JEBB)
	/* 0215 */ ASSIGN_MOP(z, JNEP)
	/* 0216 */ ASSIGN_MOP(z, JNEB)
	/* 0217 */ ASSIGN_MOP(z, JNEBB)

	/* 0220 */ ASSIGN_MOP(z, JLB)
	/* 0221 */ ASSIGN_MOP(z, JGEB)
	/* 0222 */ ASSIGN_MOP(z, JGB)
	/* 0223 */ ASSIGN_MOP(z, JLEB)
	/* 0224 */ ASSIGN_MOP(z, JULB)
	/* 0225 */ ASSIGN_MOP(z, JUGEB)
	/* 0226 */ ASSIGN_MOP(z, JUGB)
	/* 0227 */ ASSIGN_MOP(z, JULEB)

	/* 0230 */ ASSIGN_MOP(z, JZ3)
	/* 0231 */ ASSIGN_MOP(z, JZ4)
	/* 0232 */ ASSIGN_MOP(z, JZB)
	/* 0233 */ ASSIGN_MOP(z, JNZ3)
	/* 0234 */ ASSIGN_MOP(z, JNZ4)
	/* 0235 */ ASSIGN_MOP(z, JNZB)
	/* 0236 */ ASSIGN_MOP(z, JDEB)
	/* 0237 */ ASSIGN_MOP(z, JDNEB)

	/* 0240 */ ASSIGN_MOP(z, JIB)
	/* 0241 */ ASSIGN_MOP(z, JIW)
	/* 0242 */ ASSIGN_MOP(z, REC)
	/* 0243 */ ASSIGN_MOP(z, REC2)
	/* 0244 */ ASSIGN_MOP(z, DIS)
	/* 0245 */ ASSIGN_MOP(z, DIS2)
	/* 0246 */ ASSIGN_MOP(z, EXCH)
	/* 0247 */ ASSIGN_MOP(z, DEXCH)

	/* 0250 */ ASSIGN_MOP(z, DUP)
	/* 0251 */ ASSIGN_MOP(z, DDUP)
	/* 0252 */ ASSIGN_MOP(z, EXDIS)
	/* 0253 */ ASSIGN_MOP(z, NEG)
	/* 0254 */ ASSIGN_MOP(z, INC)
	/* 0255 */ ASSIGN_MOP(z, DEC)
	/* 0256 */ ASSIGN_MOP(z, DINC)
	/* 0257 */ ASSIGN_MOP(z, DBL)

	/* 0260 */ ASSIGN_MOP(z, DDBL)
	/* 0261 */ ASSIGN_MOP(z, TRPL)
	/* 0262 */ ASSIGN_MOP(z, AND)
	/* 0263 */ ASSIGN_MOP(z, IOR)
	/* 0264 */ ASSIGN_MOP(z, ADDSB)
	/* 0265 */ ASSIGN_MOP(z, ADD)
	/* 0266 */ ASSIGN_MOP(z, SUB)
	/* 0267 */ ASSIGN_MOP(z, DADD)

	/* 0270 */ ASSIGN_MOP(z, DSUB)
	/* 0271 */ ASSIGN_MOP(z, ADC)
	/* 0272 */ ASSIGN_MOP(z, ACD)
	/* 0273 */ ASSIGN_MOP(z, AL0IB)
	/* 0274 */ ASSIGN_MOP(z, MUL)
	/* 0275 */ ASSIGN_MOP(z, DCMP)
	/* 0276 */ ASSIGN_MOP(z, UDCMP)
	/* 0277 */ ASSIGN_MOP(z, VMFIND)

	/* 0300 */ ASSIGN_MOP(z, LI0)
	/* 0301 */ ASSIGN_MOP(z, LI1)
	/* 0302 */ ASSIGN_MOP(z, LI2)
	/* 0303 */ ASSIGN_MOP(z, LI3)
	/* 0304 */ ASSIGN_MOP(z, LI4)
	/* 0305 */ ASSIGN_MOP(z, LI5)
	/* 0306 */ ASSIGN_MOP(z, LI6)
	/* 0307 */ ASSIGN_MOP(z, LI7)

	/* 0310 */ ASSIGN_MOP(z, LI8)
	/* 0311 */ ASSIGN_MOP(z, LI9)
	/* 0312 */ ASSIGN_MOP(z, LI10)
	/* 0313 */ ASSIGN_MOP(z, LIN1)
	/* 0314 */ ASSIGN_MOP(z, LINI)
	/* 0315 */ ASSIGN_MOP(z, LIB)
	/* 0316 */ ASSIGN_MOP(z, LIW)
	/* 0317 */ ASSIGN_MOP(z, LINB)

	/* 0320 */ ASSIGN_MOP(z, LIHB)
	/* 0321 */ ASSIGN_MOP(z, LID0)
	/* 0322 */ ASSIGN_MOP(z, LA0)
	/* 0323 */ ASSIGN_MOP(z, LA1)
	/* 0324 */ ASSIGN_MOP(z, LA2)
	/* 0325 */ ASSIGN_MOP(z, LA3)
	/* 0326 */ ASSIGN_MOP(z, LA6)
	/* 0327 */ ASSIGN_MOP(z, LA8)

	/* 0330 */ ASSIGN_MOP(z, LAB)
	/* 0331 */ ASSIGN_MOP(z, LAW)
	/* 0332 */ ASSIGN_MOP(z, GA0)
	/* 0333 */ ASSIGN_MOP(z, GA1)
	/* 0334 */ ASSIGN_MOP(z, GAB)
	/* 0335 */ ASSIGN_MOP(z, GAW)
	/* 0336 */ //ASSIGN_MOP(z, CAW)
	/* 0337 */ ASSIGN_MOP(z, EFC0)

	/* 0340 */ ASSIGN_MOP(z, EFC1)
	/* 0341 */ ASSIGN_MOP(z, EFC2)
	/* 0342 */ ASSIGN_MOP(z, EFC3)
	/* 0343 */ ASSIGN_MOP(z, EFC4)
	/* 0344 */ ASSIGN_MOP(z, EFC5)
	/* 0345 */ ASSIGN_MOP(z, EFC6)
	/* 0346 */ ASSIGN_MOP(z, EFC7)
	/* 0347 */ ASSIGN_MOP(z, EFC8)

	/* 0350 */ ASSIGN_MOP(z, EFC9)
	/* 0351 */ ASSIGN_MOP(z, EFC10)
	/* 0352 */ ASSIGN_MOP(z, EFC11)
	/* 0353 */ ASSIGN_MOP(z, EFC12)
	/* 0354 */ ASSIGN_MOP(z, EFCB)
	/* 0355 */ ASSIGN_MOP(z, LFC)
	/* 0356 */ ASSIGN_MOP(z, SFC)
	/* 0357 */ ASSIGN_MOP(z, RET)

	/* 0360 */ ASSIGN_MOP(z, KFCB)
	/* 0361 */ ASSIGN_MOP(z, ME)
	/* 0362 */ ASSIGN_MOP(z, MX)
	/* 0363 */ ASSIGN_MOP(z, BLT)
	/* 0364 */ ASSIGN_MOP(z, BLTL)
	/* 0365 */ ASSIGN_MOP(z, BLTC)
	/* 0366 */ ASSIGN_MOP(z, BLTCL)
	/* 0367 */ ASSIGN_MOP(z, LP)

	/* 0370 */ ASSIGN_MOP(z, ESC)
	/* 0371 */ ASSIGN_MOP(z, ESCL)
	/* 0372 */ ASSIGN_MOP(z, LGA0)
	/* 0373 */ ASSIGN_MOP(z, LGAB)
	/* 0374 */ ASSIGN_MOP(z, LGAW)
	/* 0375 */ ASSIGN_MOP(z, DESC)
	// 0376
	/* 0377 */ ASSIGN_MOP(z, RESRVD)

	/* 00 */ //ASSIGN_ESC(a, 00)
	/* 01 */ //ASSIGN_ESC(a, 01)
	/* 02 */ ASSIGN_ESC(a, MW)
	/* 03 */ ASSIGN_ESC(a, MR)
	/* 04 */ ASSIGN_ESC(a, NC)
	/* 05 */ ASSIGN_ESC(a, BC)
	/* 06 */ ASSIGN_ESC(a, REQ)
	/* 07 */ ASSIGN_ESC(a, SM)

	/* 010 */ ASSIGN_ESC(a, SMF)
	/* 011 */ ASSIGN_ESC(a, GMF)
	/* 012 */ ASSIGN_ESC(a, AF)
	/* 013 */ ASSIGN_ESC(a, FF)
	/* 014 */ ASSIGN_ESC(a, PI)
	/* 015 */ ASSIGN_ESC(a, PO)
	/* 016 */ ASSIGN_ESC(a, POR)
	/* 017 */ ASSIGN_ESC(a, SPP)

	/* 020 */ ASSIGN_ESC(a, DI)
	/* 021 */ ASSIGN_ESC(a, EI)
	/* 022 */ ASSIGN_ESC(a, XOR)
	/* 023 */ ASSIGN_ESC(a, DAND)
	/* 024 */ ASSIGN_ESC(a, DIOR)
	/* 025 */ ASSIGN_ESC(a, DXOR)
	/* 026 */ ASSIGN_ESC(a, ROTATE)
	/* 027 */ ASSIGN_ESC(a, DSHIFT)

	/* 030 */ ASSIGN_ESC(a, LINT)
	/* 031 */ ASSIGN_ESC(a, JS)
	/* 032 */ ASSIGN_ESC(a, RCFS)
	/* 033 */ ASSIGN_ESC(b, RC)
	/* 034 */ ASSIGN_ESC(a, UDIV)
	/* 035 */ ASSIGN_ESC(a, LUDIV)
	/* 036 */ ASSIGN_ESC(b, ROB)
	/* 037 */ ASSIGN_ESC(b, WOB)

	/* 040 */ ASSIGN_ESC(b, DSK)
	/* 041 */ ASSIGN_ESC(b, XE)
	/* 042 */ ASSIGN_ESC(b, XF)
	/* 043 */ ASSIGN_ESC(b, LSK)
	/* 044 */ ASSIGN_ESC(a, BNDCKL)
	/* 045 */ ASSIGN_ESC(a, NILCK)
	/* 046 */ ASSIGN_ESC(a, NILCKL)
	/* 047 */ ASSIGN_ESC(a, BLTLR)

	/* 050 */ ASSIGN_ESC(a, BLEL)
	/* 051 */ ASSIGN_ESC(a, BLECL)
	/* 052 */ ASSIGN_ESC(a, CKSUM)
	/* 053 */ ASSIGN_ESC(a, BITBLT)
	/* 054 */ //ASSIGN_ESC(a, TXTBLT)
	/* 055 */ ASSIGN_ESC(a, BYTBLT)
	/* 056 */ ASSIGN_ESC(a, BYTBLTR)
	/* 057 */ ASSIGN_ESC(a, VERSION)

	/* 060 */ ASSIGN_ESC(a, DMUL)
	/* 061 */ ASSIGN_ESC(a, SDIV)
	/* 062 */ ASSIGN_ESC(a, SDDIV)
	/* 063 */ ASSIGN_ESC(a, UDDIV)
	/* 064 */ //ASSIGN_ESC(a, 64)
	/* 065 */ //ASSIGN_ESC(a, 65)
	/* 066 */ //ASSIGN_ESC(a, 66)
	/* 067 */ //ASSIGN_ESC(a, 67)

	/* 070 */ //ASSIGN_ESC(a, 70)
	/* 071 */ //ASSIGN_ESC(a, 71)
	/* 072 */ //ASSIGN_ESC(a, 72)
	/* 073 */ //ASSIGN_ESC(a, 73)
	/* 074 */ //ASSIGN_ESC(a, 74)
	/* 075 */ //ASSIGN_ESC(a, 75)
	/* 076 */ //ASSIGN_ESC(a, 76)
	/* 077 */ //ASSIGN_ESC(a, 77)

	// Floating Point (100B-137B are reserved)
	/* 0100 */ //ASSIGN_ESC(a, FADD)
	/* 0101 */ //ASSIGN_ESC(a, FSUB)
	/* 0102 */ //ASSIGN_ESC(a, FMUL)
	/* 0103 */ //ASSIGN_ESC(a, FDIV)
	/* 0104 */ //ASSIGN_ESC(a, FCOMP)
	/* 0105 */ //ASSIGN_ESC(a, FIX)
	/* 0106 */ //ASSIGN_ESC(a, FLOAT)
	/* 0107 */ //ASSIGN_ESC(a, FIXI)

	/* 0110 */ //ASSIGN_ESC(a, FIXC)
	/* 0111 */ //ASSIGN_ESC(a, FSTICKY)
	/* 0112 */ //ASSIGN_ESC(a, FREM)
	/* 0113 */ //ASSIGN_ESC(a, ROUND)
	/* 0114 */ //ASSIGN_ESC(a, ROUNDI)
	/* 0115 */ //ASSIGN_ESC(a, ROUNDC)
	/* 0116 */ //ASSIGN_ESC(a, FSQRT)
	/* 0117 */ //ASSIGN_ESC(a, FSC)

	//  Read / Write Registers
	/* 0160 */ ASSIGN_ESC(a, WRPSB)
	/* 0161 */ ASSIGN_ESC(a, WRMDS)
	/* 0162 */ ASSIGN_ESC(a, WRWP)
	/* 0163 */ ASSIGN_ESC(a, WRWDC)
	/* 0164 */ ASSIGN_ESC(a, WRPTC)
	/* 0165 */ ASSIGN_ESC(a, WRIT)
	/* 0166 */ ASSIGN_ESC(a, WRXTS)
	/* 0167 */ ASSIGN_ESC(a, WRMP)

	/* 0170 */ ASSIGN_ESC(a, RRPSB)
	/* 0171 */ ASSIGN_ESC(a, RRMDS)
	/* 0172 */ ASSIGN_ESC(a, RRWP)
	/* 0173 */ ASSIGN_ESC(a, RRWDC)
	/* 0174 */ ASSIGN_ESC(a, RRPTC)
	/* 0175 */ ASSIGN_ESC(a, RRIT)
	/* 0176 */ ASSIGN_ESC(a, RRXTS)
	/* 0177 */ //ASSIGN_ESC(a, 177)

	// Guam
	//   GuamInputOutput.mesa
	/* 0211 */ ASSIGN_ESC(a, CALLAGENT)
	/* 0212 */ ASSIGN_ESC(a, MAPDISPLAY)
	/* 0213 */ ASSIGN_ESC(a, STOPEMULATOR)
	/* 0214 */ ASSIGN_ESC(a, 214)

	// BitBltX.mesa
	/* 0302 */ //ASSIGN_ESC(a, BITBLTX)

	// ColorBlt.mesa
	/* 0300 */ ASSIGN_ESC(a, COLORBLT)
	/* 0301 */ //ASSIGN_ESC(a, WRITEPIXEL)
	/* 0305 */ ASSIGN_ESC(a, 305)
	/* 0306 */ ASSIGN_ESC(a, 306)
}
