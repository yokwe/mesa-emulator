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
// Opcode.h
//

#ifndef OPCODE_H_
#define OPCODE_H_ 1

#include "../util/Debug.h"

#include "../mesa/MesaBasic.h"
#include "../mesa/Memory.h"

#include "Run.h"

class Opcode {
public:
	typedef Run  (*COMPILE)(Opcode* opcode);
	typedef void (*INTERPRET)(Opcode* opcode);

	Opcode() : flag(0), name(0), compileProc(0), interpretProc(0) {}
	Opcode(const Opcode& that) :
		flag(that.flag), name(that.name), compileProc(that.compileProc), interpretProc(that.interpretProc) {}
	Opcode(CARD8 code_, CARD32 length_, const char* name_, COMPILE compileProc_, INTERPRET interpretProc_) :
		code(code_), length(length_), filler(0), name(name_), compileProc(compileProc_), interpretProc(interpretProc_) {}

	Opcode& operator=(const Opcode& that) {
		flag          = that.flag;
		name          = that.name;
		compileProc   = that.compileProc;
		interpretProc = that.interpretProc;
		//
		return *this;
	}

	const char* getName() const {
		return name;
	}
	CARD32 getCode() const {
		return code;
	}
	CARD32 getLength() const {
		return length;
	}
	int isEmpty() const {
		return compileProc == 0;
	}
	void empty() {
		flag          = 0;
		name          = 0;
		compileProc   = 0;
		interpretProc = 0;
	}

	Run compile() {
		return compileProc(this);
	}
	void interpret() {
		if (DEBUG_USE_COMPILE_RUN) compileRun(this);
		else interpretProc(this);
	}

	int isOpcodeTrap();

protected:
	union {
		CARD32 flag;
		struct {
			CARD32 code   :  8;
			CARD32 length :  8;
			CARD32 filler : 16;
		};
	};
	const char* name;
	COMPILE     compileProc;
	INTERPRET   interpretProc;

	__attribute__((always_inline)) static inline void compileRun(Opcode* opcode) {
		Run run = opcode->compileProc(opcode);
		PC = savedPC + opcode->getLength();
		Run::execute(run);
	}
};


#define DECL_C(len, name) \
extern Run  C_##name(Opcode* opcode); \
extern void I_##name(Opcode* opcode); \
static const CARD32 L_##name = (len);

DECL_C(0, MOP_OPCODE_TRAP)
DECL_C(0, ESC_OPCODE_TRAP)


// MOP
/* 0000 */ DECL_C(1, NOOP)
/* 0001 */ DECL_C(1, LL0)
/* 0002 */ DECL_C(1, LL1)
/* 0003 */ DECL_C(1, LL2)
/* 0004 */ DECL_C(1, LL3)
/* 0005 */ DECL_C(1, LL4)
/* 0006 */ DECL_C(1, LL5)
/* 0007 */ DECL_C(1, LL6)
/* 0010 */ DECL_C(1, LL7)

/* 0011 */ DECL_C(1, LL8)
/* 0012 */ DECL_C(1, LL9)
/* 0013 */ DECL_C(1, LL10)
/* 0014 */ DECL_C(1, LL11)
/* 0015 */ DECL_C(2, LLB)
/* 0016 */ DECL_C(1, LLD0)
/* 0017 */ DECL_C(1, LLD1)

/* 0020 */ DECL_C(1, LLD2)
/* 0021 */ DECL_C(1, LLD3)
/* 0022 */ DECL_C(1, LLD4)
/* 0023 */ DECL_C(1, LLD5)
/* 0024 */ DECL_C(1, LLD6)
/* 0025 */ DECL_C(1, LLD7)
/* 0026 */ DECL_C(1, LLD8)
/* 0027 */ DECL_C(1, LLD10)

/* 0030 */ DECL_C(2, LLDB)
/* 0031 */ DECL_C(1, SL0)
/* 0032 */ DECL_C(1, SL1)
/* 0033 */ DECL_C(1, SL2)
/* 0034 */ DECL_C(1, SL3)
/* 0035 */ DECL_C(1, SL4)
/* 0036 */ DECL_C(1, SL5)
/* 0037 */ DECL_C(1, SL6)

/* 0040 */ DECL_C(1, SL7)
/* 0041 */ DECL_C(1, SL8)
/* 0042 */ DECL_C(1, SL9)
/* 0043 */ DECL_C(1, SL10)
/* 0044 */ DECL_C(2, SLB)
/* 0045 */ DECL_C(1, SLD0)
/* 0046 */ DECL_C(1, SLD1)
/* 0047 */ DECL_C(1, SLD2)

/* 0050 */ DECL_C(1, SLD3)
/* 0051 */ DECL_C(1, SLD4)
/* 0052 */ DECL_C(1, SLD5)
/* 0053 */ DECL_C(1, SLD6)
/* 0054 */ DECL_C(1, SLD8)
/* 0055 */ DECL_C(1, PL0)
/* 0056 */ DECL_C(1, PL1)
/* 0057 */ DECL_C(1, PL2)

/* 0060 */ DECL_C(1, PL3)
/* 0061 */ DECL_C(2, PLB)
/* 0062 */ DECL_C(1, PLD0)
/* 0063 */ DECL_C(2, PLDB)
/* 0064 */ DECL_C(1, LG0)
/* 0065 */ DECL_C(1, LG1)
/* 0066 */ DECL_C(1, LG2)
/* 0067 */ DECL_C(2, LGB)

/* 0070 */ DECL_C(1, LGD0)
/* 0071 */ DECL_C(1, LGD2)
/* 0072 */ DECL_C(2, LGDB)
/* 0073 */ DECL_C(2, SGB)
/* 0074 */ DECL_C(1, BNDCK)
/* 0075 */ DECL_C(1, BRK)

/* 0100 */ DECL_C(1, R0)
/* 0101 */ DECL_C(1, R1)
/* 0102 */ DECL_C(2, RB)
/* 0103 */ DECL_C(1, RL0)
/* 0104 */ DECL_C(2, RLB)
/* 0105 */ DECL_C(1, RD0)
/* 0106 */ DECL_C(2, RDB)
/* 0107 */ DECL_C(1, RDL0)

/* 0110 */ DECL_C(2, RDLB)
/* 0111 */ DECL_C(1, W0)
/* 0112 */ DECL_C(2, WB)
/* 0113 */ DECL_C(2, PSB)
/* 0114 */ DECL_C(2, WLB)
/* 0115 */ DECL_C(2, PSLB)
/* 0116 */ DECL_C(2, WDB)
/* 0117 */ DECL_C(1, PSD0)

/* 0120 */ DECL_C(2, PSDB)
/* 0121 */ DECL_C(2, WDLB)
/* 0122 */ DECL_C(2, PSDLB)
/* 0123 */ DECL_C(1, RLI00)
/* 0124 */ DECL_C(1, RLI01)
/* 0125 */ DECL_C(1, RLI02)
/* 0126 */ DECL_C(1, RLI03)
/* 0127 */ DECL_C(2, RLIP)

/* 0130 */ DECL_C(2, RLILP)
/* 0131 */ DECL_C(1, RLDI00)
/* 0132 */ DECL_C(2, RLDIP)
/* 0133 */ DECL_C(2, RLDILP)
/* 0134 */ DECL_C(2, RGIP)
/* 0135 */ DECL_C(2, RGILP)
/* 0136 */ DECL_C(2, WLIP)
/* 0137 */ DECL_C(2, WLILP)

/* 0140 */ DECL_C(2, WLDILP)
/* 0141 */ DECL_C(2, RS)
/* 0142 */ DECL_C(2, RLS)
/* 0143 */ DECL_C(2, WS)
/* 0144 */ DECL_C(2, WLS)
/* 0145 */ DECL_C(2, R0F)
/* 0146 */ DECL_C(3, RF)
/* 0147 */ DECL_C(2, RL0F)

/* 0150 */ DECL_C(3, RLF)
/* 0151 */ DECL_C(1, RLFS)
/* 0152 */ DECL_C(3, RLIPF)
/* 0153 */ DECL_C(3, RLILPF)
/* 0154 */ DECL_C(2, W0F)
/* 0155 */ DECL_C(3, WF)
/* 0156 */ DECL_C(3, PSF)
/* 0157 */ DECL_C(2, PS0F)

/* 0160 */ DECL_C(2, WS0F)
/* 0161 */ DECL_C(2, WL0F)
/* 0162 */ DECL_C(3, WLF)
/* 0163 */ DECL_C(3, PSLF)
/* 0164 */ DECL_C(1, WLFS)
/* 0165 */ DECL_C(2, SLDB)
/* 0166 */ DECL_C(2, SGDB)
/* 0167 */ DECL_C(2, LLKB)

/* 0170 */ DECL_C(2, RKIB)
/* 0171 */ DECL_C(2, RKDIB)
/* 0172 */ DECL_C(2, LKB)
/* 0173 */ DECL_C(1, SHIFT)
/* 0174 */ DECL_C(2, SHIFTSB)

/* 0200 */ DECL_C(2, CATCH)
/* 0201 */ DECL_C(1, J2)
/* 0202 */ DECL_C(1, J3)
/* 0203 */ DECL_C(1, J4)
/* 0204 */ DECL_C(1, J5)
/* 0205 */ DECL_C(1, J6)
/* 0206 */ DECL_C(1, J7)
/* 0207 */ DECL_C(1, J8)

/* 0210 */ DECL_C(2, JB)
/* 0211 */ DECL_C(3, JW)
/* 0212 */ DECL_C(2, JEP)
/* 0213 */ DECL_C(2, JEB)
/* 0214 */ DECL_C(3, JEBB)
/* 0215 */ DECL_C(2, JNEP)
/* 0216 */ DECL_C(2, JNEB)
/* 0217 */ DECL_C(3, JNEBB)

/* 0220 */ DECL_C(2, JLB)
/* 0221 */ DECL_C(2, JGEB)
/* 0222 */ DECL_C(2, JGB)
/* 0223 */ DECL_C(2, JLEB)
/* 0224 */ DECL_C(2, JULB)
/* 0225 */ DECL_C(2, JUGEB)
/* 0226 */ DECL_C(2, JUGB)
/* 0227 */ DECL_C(2, JULEB)

/* 0230 */ DECL_C(1, JZ3)
/* 0231 */ DECL_C(1, JZ4)
/* 0232 */ DECL_C(2, JZB)
/* 0233 */ DECL_C(1, JNZ3)
/* 0234 */ DECL_C(1, JNZ4)
/* 0235 */ DECL_C(2, JNZB)
/* 0236 */ DECL_C(2, JDEB)
/* 0237 */ DECL_C(2, JDNEB)

/* 0240 */ DECL_C(3, JIB)
/* 0241 */ DECL_C(3, JIW)
/* 0242 */ DECL_C(1, REC)
/* 0243 */ DECL_C(1, REC2)
/* 0244 */ DECL_C(1, DIS)
/* 0245 */ DECL_C(1, DIS2)
/* 0246 */ DECL_C(1, EXCH)
/* 0247 */ DECL_C(1, DEXCH)

/* 0250 */ DECL_C(1, DUP)
/* 0251 */ DECL_C(1, DDUP)
/* 0252 */ DECL_C(1, EXDIS)
/* 0253 */ DECL_C(1, NEG)
/* 0254 */ DECL_C(1, INC)
/* 0255 */ DECL_C(1, DEC)
/* 0256 */ DECL_C(1, DINC)
/* 0257 */ DECL_C(1, DBL)

/* 0260 */ DECL_C(1, DDBL)
/* 0261 */ DECL_C(1, TRPL)
/* 0262 */ DECL_C(1, AND)
/* 0263 */ DECL_C(1, IOR)
/* 0264 */ DECL_C(2, ADDSB)
/* 0265 */ DECL_C(1, ADD)
/* 0266 */ DECL_C(1, SUB)
/* 0267 */ DECL_C(1, DADD)

/* 0270 */ DECL_C(1, DSUB)
/* 0271 */ DECL_C(1, ADC)
/* 0272 */ DECL_C(1, ACD)
/* 0273 */ DECL_C(2, AL0IB)
/* 0274 */ DECL_C(1, MUL)
/* 0275 */ DECL_C(1, DCMP)
/* 0276 */ DECL_C(1, UDCMP)
/* 0277 */ DECL_C(1, VMFIND)

/* 0300 */ DECL_C(1, LI0)
/* 0301 */ DECL_C(1, LI1)
/* 0302 */ DECL_C(1, LI2)
/* 0303 */ DECL_C(1, LI3)
/* 0304 */ DECL_C(1, LI4)
/* 0305 */ DECL_C(1, LI5)
/* 0306 */ DECL_C(1, LI6)
/* 0307 */ DECL_C(1, LI7)

/* 0310 */ DECL_C(1, LI8)
/* 0311 */ DECL_C(1, LI9)
/* 0312 */ DECL_C(1, LI10)
/* 0313 */ DECL_C(1, LIN1)
/* 0314 */ DECL_C(1, LINI)
/* 0315 */ DECL_C(2, LIB)
/* 0316 */ DECL_C(3, LIW)
/* 0317 */ DECL_C(2, LINB)

/* 0320 */ DECL_C(2, LIHB)
/* 0321 */ DECL_C(1, LID0)
/* 0322 */ DECL_C(1, LA0)
/* 0323 */ DECL_C(1, LA1)
/* 0324 */ DECL_C(1, LA2)
/* 0325 */ DECL_C(1, LA3)
/* 0326 */ DECL_C(1, LA6)
/* 0327 */ DECL_C(1, LA8)

/* 0330 */ DECL_C(2, LAB)
/* 0331 */ DECL_C(3, LAW)
/* 0332 */ DECL_C(1, GA0)
/* 0333 */ DECL_C(1, GA1)
/* 0334 */ DECL_C(2, GAB)
/* 0335 */ DECL_C(3, GAW)
/* 0337 */ DECL_C(1, EFC0)

/* 0340 */ DECL_C(1, EFC1)
/* 0341 */ DECL_C(1, EFC2)
/* 0342 */ DECL_C(1, EFC3)
/* 0343 */ DECL_C(1, EFC4)
/* 0344 */ DECL_C(1, EFC5)
/* 0345 */ DECL_C(1, EFC6)
/* 0346 */ DECL_C(1, EFC7)
/* 0347 */ DECL_C(1, EFC8)

/* 0350 */ DECL_C(1, EFC9)
/* 0351 */ DECL_C(1, EFC10)
/* 0352 */ DECL_C(1, EFC11)
/* 0353 */ DECL_C(1, EFC12)
/* 0354 */ DECL_C(2, EFCB)
/* 0355 */ DECL_C(3, LFC)
/* 0356 */ DECL_C(1, SFC)
/* 0357 */ DECL_C(1, RET)

/* 0360 */ DECL_C(2, KFCB)
/* 0361 */ DECL_C(1, ME)
/* 0362 */ DECL_C(1, MX)
/* 0363 */ DECL_C(1, BLT)
/* 0364 */ DECL_C(1, BLTL)
/* 0365 */ DECL_C(1, BLTC)
/* 0366 */ DECL_C(1, BLTCL)
/* 0367 */ DECL_C(1, LP)

/* 0370 */ DECL_C(1, ESC)
/* 0371 */ DECL_C(1, ESCL)
/* 0372 */ DECL_C(1, LGA0)
/* 0373 */ DECL_C(2, LGAB)
/* 0374 */ DECL_C(3, LGAW)
/* 0375 */ DECL_C(3, DESC)
/* 0377 */ DECL_C(1, RESRVD)

//
// ESC
//
/* 0002 */ DECL_C(2, MW)
/* 0003 */ DECL_C(2, MR)
/* 0004 */ DECL_C(2, NC)
/* 0005 */ DECL_C(2, BC)
/* 0006 */ DECL_C(2, REQ)
/* 0007 */ DECL_C(2, SM)

/* 0010 */ DECL_C(2, SMF)
/* 0011 */ DECL_C(2, GMF)
/* 0012 */ DECL_C(2, AF)
/* 0013 */ DECL_C(2, FF)
/* 0014 */ DECL_C(2, PI)
/* 0015 */ DECL_C(2, PO)
/* 0016 */ DECL_C(2, POR)
/* 0017 */ DECL_C(2, SPP)

/* 0020 */ DECL_C(2, DI)
/* 0021 */ DECL_C(2, EI)
/* 0022 */ DECL_C(2, XOR)
/* 0023 */ DECL_C(2, DAND)
/* 0024 */ DECL_C(2, DIOR)
/* 0025 */ DECL_C(2, DXOR)
/* 0026 */ DECL_C(2, ROTATE)
/* 0027 */ DECL_C(2, DSHIFT)

/* 0030 */ DECL_C(2, LINT)
/* 0031 */ DECL_C(2, JS)
/* 0032 */ DECL_C(2, RCFS)
/* 0033 */ DECL_C(3, RC)
/* 0034 */ DECL_C(2, UDIV)
/* 0035 */ DECL_C(2, LUDIV)
/* 0036 */ DECL_C(3, ROB)
/* 0037 */ DECL_C(3, WOB)

/* 0040 */ DECL_C(3, DSK)
/* 0041 */ DECL_C(3, XE)
/* 0042 */ DECL_C(3, XF)
/* 0043 */ DECL_C(3, LSK)
/* 0044 */ DECL_C(2, BNDCKL)
/* 0045 */ DECL_C(2, NILCK)
/* 0046 */ DECL_C(2, NILCKL)
/* 0047 */ DECL_C(2, BLTLR)

/* 0050 */ DECL_C(2, BLEL)
/* 0051 */ DECL_C(2, BLECL)
/* 0052 */ DECL_C(2, CKSUM)
/* 0053 */ DECL_C(2, BITBLT)
/* 0054 */ DECL_C(2, TXTBLT)
/* 0055 */ DECL_C(2, BYTBLT)
/* 0056 */ DECL_C(2, BYTBLTR)
/* 0057 */ DECL_C(2, VERSION)

/* 0060 */ DECL_C(2, DMUL)
/* 0061 */ DECL_C(2, SDIV)
/* 0062 */ DECL_C(2, SDDIV)
/* 0063 */ DECL_C(2, UDDIV)

/* 0160 */ DECL_C(2, WRPSB)
/* 0161 */ DECL_C(2, WRMDS)
/* 0162 */ DECL_C(2, WRWP)
/* 0163 */ DECL_C(2, WRWDC)
/* 0164 */ DECL_C(2, WRPTC)
/* 0165 */ DECL_C(2, WRIT)
/* 0166 */ DECL_C(2, WRXTS)
/* 0167 */ DECL_C(2, WRMP)

/* 0170 */ DECL_C(2, RRPSB)
/* 0171 */ DECL_C(2, RRMDS)
/* 0172 */ DECL_C(2, RRWP)
/* 0173 */ DECL_C(2, RRWDC)
/* 0174 */ DECL_C(2, RRPTC)
/* 0175 */ DECL_C(2, RRIT)
/* 0176 */ DECL_C(2, RRXTS)

/* 0211 */ DECL_C(2, CALLAGENT)
/* 0212 */ DECL_C(2, MAPDISPLAY)
/* 0213 */ DECL_C(2, STOPEMULATOR)
/* 0214 */ DECL_C(2, 214)

/* 0300 */ DECL_C(2, COLORBLT)
/* 0304 */ DECL_C(2, 305)
/* 0306 */ DECL_C(2, 306)


#define DEF_R(name) \
void R_##name(Run run) { \
	R_##name##_(run); \
}

#define DEF_CI(name) \
Run C_##name(Opcode* opcode_) { \
	return C_##name##_(opcode_); \
} \
void I_##name(Opcode* opcode) { \
	Run run = C_##name##_(opcode); \
	PC = savedPC + L_##name; \
	R_##name##_(run); \
}

#define DEF_CI_n(name, n) \
Run C_##name##n(Opcode* opcode_) { \
	return C_##name##n##_(opcode_); \
} \
void I_##name##n(Opcode* opcode) { \
	Run run = C_##name##n##_(opcode); \
	PC = savedPC + L_##name##n; \
	R_##name##_(run); \
}

#define DEF_CI_r(name, r) \
Run C_##name(Opcode* opcode_) { \
	return C_##name##_(opcode_); \
} \
void I_##name(Opcode* opcode) { \
	Run run = C_##name##_(opcode); \
	PC = savedPC + L_##name; \
	R_##r##_(run); \
}



#define DEF_C_0(name) \
__attribute__((always_inline)) static inline Run C_##name##_(Opcode *opcode) { \
	Run ret(r##name, L_##name); \
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s", savedPC, opcode->getName()); \
	return ret; \
} \
DEF_CI(name)

#define DEF_C_0_n(name, n) \
__attribute__((always_inline)) static inline Run C_##name##n##_(Opcode *opcode) { \
	Run ret(r##name, L_##name##n, (CARD16)(n)); \
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s", savedPC, opcode->getName()); \
	return ret; \
} \
DEF_CI_n(name, n)

#define DEF_C_B(name) \
__attribute__((always_inline)) static inline Run C_##name##B##_(Opcode *opcode) { \
	Run ret(r##name, L_##name##B, (CARD16)GetCodeByte()); \
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s %3d", savedPC, opcode->getName(), ret.getArg()); \
	return ret; \
} \
DEF_CI_n(name, B)

#define DEF_C_P_0_n(name, n) \
__attribute__((always_inline)) static inline Run C_##name##0##n##_(Opcode *opcode) { \
	Run ret(r##name, L_##name##0##n, (CARD8)0, (CARD8)(n)); \
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s %3d", savedPC, opcode->getName(), ret.getArg1()); \
	return ret; \
} \
DEF_CI_n(name, 0##n)

#define DEF_C_P(name) \
__attribute__((always_inline)) static inline Run C_##name##P##_(Opcode *opcode) { \
	NibblePair pair = {GetCodeByte()}; \
	Run ret(r##name, L_##name##P, (CARD8)pair.left, (CARD8)pair.right); \
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s %3d %3d", savedPC, opcode->getName(), ret.getArg0(), ret.getArg1()); \
	return ret; \
} \
DEF_CI_n(name, P)

#define DEF_C_B_N(name) \
__attribute__((always_inline)) static inline Run C_##name##_(Opcode *opcode) { \
	Run ret(r##name, L_##name, (CARD16)GetCodeByte()); \
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s %3d", savedPC, opcode->getName(), ret.getArg()); \
	return ret; \
} \
DEF_CI(name)

#define DEF_C_F(name) \
__attribute__((always_inline)) static inline Run C_##name##_(Opcode *opcode) { \
	FieldDesc desc = {GetCodeWord()}; \
	Run ret(r##name, L_##name, (CARD8)desc.offset, (CARD8)desc.field); \
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s %3d %3d", savedPC, opcode->getName(), ret.getArg0(), ret.getArg1()); \
	return ret; \
} \
DEF_CI(name)

#define DEF_C_PF(name) \
__attribute__((always_inline)) static inline Run C_##name##_(Opcode *opcode) { \
	CARD8 arg0 = GetCodeByte(); \
	CARD8 arg1 = GetCodeByte(); \
	Run ret(r##name, L_##name, arg0, arg1); \
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s %3d %3d", savedPC, opcode->getName(), ret.getArg0(), ret.getArg1()); \
	return ret; \
} \
DEF_CI(name)

#define DEF_C_SB(name) \
__attribute__((always_inline)) static inline Run C_##name##B##_(Opcode *opcode) { \
	Run ret(r##name, L_##name##B, (CARD16)SignExtend(GetCodeByte())); \
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s %3d", savedPC, opcode->getName(), ret.getArg()); \
	return ret; \
} \
DEF_CI_n(name, B)

#define DEF_C_W(name) \
__attribute__((always_inline)) static inline Run C_##name##W##_(Opcode *opcode) { \
	Run ret(r##name, L_##name##W, (CARD16)GetCodeWord()); \
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s %5d", savedPC, opcode->getName(), ret.getArg()); \
	return ret; \
} \
DEF_CI_n(name, W)

#define DEF_C_SB_N(name) \
__attribute__((always_inline)) static inline Run C_##name##_(Opcode *opcode) { \
	Run ret(r##name, L_##name, (CARD16)SignExtend(GetCodeByte())); \
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s %3d", savedPC, opcode->getName(), ret.getArg()); \
	return ret; \
} \
DEF_CI(name)

#define DEF_C_W_N(name) \
__attribute__((always_inline)) static inline Run C_##name##_(Opcode *opcode) { \
	Run ret(r##name, L_##name, (CARD16)GetCodeWord()); \
	if (DEBUG_TRACE_COMPILE) logger.debug("TRACE %6o  %s %5d", savedPC, opcode->getName(), ret.getArg()); \
	return ret; \
} \
DEF_CI(name)

#endif /* OPCODETABLE_H_ */
