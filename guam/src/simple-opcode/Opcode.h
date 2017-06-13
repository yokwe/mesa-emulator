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
// Opcode.h
//

#ifndef OPCODE_H_
#define OPCODE_H_ 1

#include "../util/Debug.h"

#include "../mesa/MesaBasic.h"
#include "../mesa/Memory.h"

class Opcode {
public:
	typedef void (*EXEC)(Opcode* opcode);

	Opcode() : exec(0), name(0), flag(0) {}
	Opcode(const Opcode& that) :
		exec(that.exec), name(that.name), flag(that.flag) {}
	Opcode(EXEC exec_, const char* name_, CARD32 code_, CARD32 size_) :
		exec(exec_), name(name_), code(code_), size(size_) {}

	Opcode& operator=(const Opcode& that) {
		exec = that.exec;
		name = that.name;
		flag = that.flag;
		//
		return *this;
	}

	void empty() {
		exec = 0;
		name = 0;
		flag = 0;
	}
	int isEmpty() {
		return exec == 0;
	}
	int isEqual(EXEC exec_) {
		return exec == exec_;
	}

	const char* getName() const {
		return name;
	}
	CARD32 getCode() const {
		return code;
	}
	CARD32 getSize() const {
		return size;
	}

	void execute() {
		exec(this);
	}

protected:
	EXEC        exec;
	const char* name;
	union {
		CARD32 flag;
		struct {
			CARD32 code : 16;
			CARD32 size : 16;
		};
	};
};

#define DECL_E(len, name) \
extern void E_##name(Opcode* opcode); \
static const CARD32 L_##name = (len);

// MOP
/* 0000 */ DECL_E(1, NOOP)
/* 0001 */ DECL_E(1, LL0)
/* 0002 */ DECL_E(1, LL1)
/* 0003 */ DECL_E(1, LL2)
/* 0004 */ DECL_E(1, LL3)
/* 0005 */ DECL_E(1, LL4)
/* 0006 */ DECL_E(1, LL5)
/* 0007 */ DECL_E(1, LL6)
/* 0010 */ DECL_E(1, LL7)

/* 0011 */ DECL_E(1, LL8)
/* 0012 */ DECL_E(1, LL9)
/* 0013 */ DECL_E(1, LL10)
/* 0014 */ DECL_E(1, LL11)
/* 0015 */ DECL_E(2, LLB)
/* 0016 */ DECL_E(1, LLD0)
/* 0017 */ DECL_E(1, LLD1)

/* 0020 */ DECL_E(1, LLD2)
/* 0021 */ DECL_E(1, LLD3)
/* 0022 */ DECL_E(1, LLD4)
/* 0023 */ DECL_E(1, LLD5)
/* 0024 */ DECL_E(1, LLD6)
/* 0025 */ DECL_E(1, LLD7)
/* 0026 */ DECL_E(1, LLD8)
/* 0027 */ DECL_E(1, LLD10)

/* 0030 */ DECL_E(2, LLDB)
/* 0031 */ DECL_E(1, SL0)
/* 0032 */ DECL_E(1, SL1)
/* 0033 */ DECL_E(1, SL2)
/* 0034 */ DECL_E(1, SL3)
/* 0035 */ DECL_E(1, SL4)
/* 0036 */ DECL_E(1, SL5)
/* 0037 */ DECL_E(1, SL6)

/* 0040 */ DECL_E(1, SL7)
/* 0041 */ DECL_E(1, SL8)
/* 0042 */ DECL_E(1, SL9)
/* 0043 */ DECL_E(1, SL10)
/* 0044 */ DECL_E(2, SLB)
/* 0045 */ DECL_E(1, SLD0)
/* 0046 */ DECL_E(1, SLD1)
/* 0047 */ DECL_E(1, SLD2)

/* 0050 */ DECL_E(1, SLD3)
/* 0051 */ DECL_E(1, SLD4)
/* 0052 */ DECL_E(1, SLD5)
/* 0053 */ DECL_E(1, SLD6)
/* 0054 */ DECL_E(1, SLD8)
/* 0055 */ DECL_E(1, PL0)
/* 0056 */ DECL_E(1, PL1)
/* 0057 */ DECL_E(1, PL2)

/* 0060 */ DECL_E(1, PL3)
/* 0061 */ DECL_E(2, PLB)
/* 0062 */ DECL_E(1, PLD0)
/* 0063 */ DECL_E(2, PLDB)
/* 0064 */ DECL_E(1, LG0)
/* 0065 */ DECL_E(1, LG1)
/* 0066 */ DECL_E(1, LG2)
/* 0067 */ DECL_E(2, LGB)

/* 0070 */ DECL_E(1, LGD0)
/* 0071 */ DECL_E(1, LGD2)
/* 0072 */ DECL_E(2, LGDB)
/* 0073 */ DECL_E(2, SGB)
/* 0074 */ DECL_E(1, BNDCK)
/* 0075 */ DECL_E(1, BRK)

/* 0100 */ DECL_E(1, R0)
/* 0101 */ DECL_E(1, R1)
/* 0102 */ DECL_E(2, RB)
/* 0103 */ DECL_E(1, RL0)
/* 0104 */ DECL_E(2, RLB)
/* 0105 */ DECL_E(1, RD0)
/* 0106 */ DECL_E(2, RDB)
/* 0107 */ DECL_E(1, RDL0)

/* 0110 */ DECL_E(2, RDLB)
/* 0111 */ DECL_E(1, W0)
/* 0112 */ DECL_E(2, WB)
/* 0113 */ DECL_E(2, PSB)
/* 0114 */ DECL_E(2, WLB)
/* 0115 */ DECL_E(2, PSLB)
/* 0116 */ DECL_E(2, WDB)
/* 0117 */ DECL_E(1, PSD0)

/* 0120 */ DECL_E(2, PSDB)
/* 0121 */ DECL_E(2, WDLB)
/* 0122 */ DECL_E(2, PSDLB)
/* 0123 */ DECL_E(1, RLI00)
/* 0124 */ DECL_E(1, RLI01)
/* 0125 */ DECL_E(1, RLI02)
/* 0126 */ DECL_E(1, RLI03)
/* 0127 */ DECL_E(2, RLIP)

/* 0130 */ DECL_E(2, RLILP)
/* 0131 */ DECL_E(1, RLDI00)
/* 0132 */ DECL_E(2, RLDIP)
/* 0133 */ DECL_E(2, RLDILP)
/* 0134 */ DECL_E(2, RGIP)
/* 0135 */ DECL_E(2, RGILP)
/* 0136 */ DECL_E(2, WLIP)
/* 0137 */ DECL_E(2, WLILP)

/* 0140 */ DECL_E(2, WLDILP)
/* 0141 */ DECL_E(2, RS)
/* 0142 */ DECL_E(2, RLS)
/* 0143 */ DECL_E(2, WS)
/* 0144 */ DECL_E(2, WLS)
/* 0145 */ DECL_E(2, R0F)
/* 0146 */ DECL_E(3, RF)
/* 0147 */ DECL_E(2, RL0F)

/* 0150 */ DECL_E(3, RLF)
/* 0151 */ DECL_E(1, RLFS)
/* 0152 */ DECL_E(3, RLIPF)
/* 0153 */ DECL_E(3, RLILPF)
/* 0154 */ DECL_E(2, W0F)
/* 0155 */ DECL_E(3, WF)
/* 0156 */ DECL_E(3, PSF)
/* 0157 */ DECL_E(2, PS0F)

/* 0160 */ DECL_E(2, WS0F)
/* 0161 */ DECL_E(2, WL0F)
/* 0162 */ DECL_E(3, WLF)
/* 0163 */ DECL_E(3, PSLF)
/* 0164 */ DECL_E(1, WLFS)
/* 0165 */ DECL_E(2, SLDB)
/* 0166 */ DECL_E(2, SGDB)
/* 0167 */ DECL_E(2, LLKB)

/* 0170 */ DECL_E(2, RKIB)
/* 0171 */ DECL_E(2, RKDIB)
/* 0172 */ DECL_E(2, LKB)
/* 0173 */ DECL_E(1, SHIFT)
/* 0174 */ DECL_E(2, SHIFTSB)

/* 0200 */ DECL_E(2, CATCH)
/* 0201 */ DECL_E(1, J2)
/* 0202 */ DECL_E(1, J3)
/* 0203 */ DECL_E(1, J4)
/* 0204 */ DECL_E(1, J5)
/* 0205 */ DECL_E(1, J6)
/* 0206 */ DECL_E(1, J7)
/* 0207 */ DECL_E(1, J8)

/* 0210 */ DECL_E(2, JB)
/* 0211 */ DECL_E(3, JW)
/* 0212 */ DECL_E(2, JEP)
/* 0213 */ DECL_E(2, JEB)
/* 0214 */ DECL_E(3, JEBB)
/* 0215 */ DECL_E(2, JNEP)
/* 0216 */ DECL_E(2, JNEB)
/* 0217 */ DECL_E(3, JNEBB)

/* 0220 */ DECL_E(2, JLB)
/* 0221 */ DECL_E(2, JGEB)
/* 0222 */ DECL_E(2, JGB)
/* 0223 */ DECL_E(2, JLEB)
/* 0224 */ DECL_E(2, JULB)
/* 0225 */ DECL_E(2, JUGEB)
/* 0226 */ DECL_E(2, JUGB)
/* 0227 */ DECL_E(2, JULEB)

/* 0230 */ DECL_E(1, JZ3)
/* 0231 */ DECL_E(1, JZ4)
/* 0232 */ DECL_E(2, JZB)
/* 0233 */ DECL_E(1, JNZ3)
/* 0234 */ DECL_E(1, JNZ4)
/* 0235 */ DECL_E(2, JNZB)
/* 0236 */ DECL_E(2, JDEB)
/* 0237 */ DECL_E(2, JDNEB)

/* 0240 */ DECL_E(3, JIB)
/* 0241 */ DECL_E(3, JIW)
/* 0242 */ DECL_E(1, REC)
/* 0243 */ DECL_E(1, REC2)
/* 0244 */ DECL_E(1, DIS)
/* 0245 */ DECL_E(1, DIS2)
/* 0246 */ DECL_E(1, EXCH)
/* 0247 */ DECL_E(1, DEXCH)

/* 0250 */ DECL_E(1, DUP)
/* 0251 */ DECL_E(1, DDUP)
/* 0252 */ DECL_E(1, EXDIS)
/* 0253 */ DECL_E(1, NEG)
/* 0254 */ DECL_E(1, INC)
/* 0255 */ DECL_E(1, DEC)
/* 0256 */ DECL_E(1, DINC)
/* 0257 */ DECL_E(1, DBL)

/* 0260 */ DECL_E(1, DDBL)
/* 0261 */ DECL_E(1, TRPL)
/* 0262 */ DECL_E(1, AND)
/* 0263 */ DECL_E(1, IOR)
/* 0264 */ DECL_E(2, ADDSB)
/* 0265 */ DECL_E(1, ADD)
/* 0266 */ DECL_E(1, SUB)
/* 0267 */ DECL_E(1, DADD)

/* 0270 */ DECL_E(1, DSUB)
/* 0271 */ DECL_E(1, ADC)
/* 0272 */ DECL_E(1, ACD)
/* 0273 */ DECL_E(2, AL0IB)
/* 0274 */ DECL_E(1, MUL)
/* 0275 */ DECL_E(1, DCMP)
/* 0276 */ DECL_E(1, UDCMP)
/* 0277 */ DECL_E(1, VMFIND)

/* 0300 */ DECL_E(1, LI0)
/* 0301 */ DECL_E(1, LI1)
/* 0302 */ DECL_E(1, LI2)
/* 0303 */ DECL_E(1, LI3)
/* 0304 */ DECL_E(1, LI4)
/* 0305 */ DECL_E(1, LI5)
/* 0306 */ DECL_E(1, LI6)
/* 0307 */ DECL_E(1, LI7)

/* 0310 */ DECL_E(1, LI8)
/* 0311 */ DECL_E(1, LI9)
/* 0312 */ DECL_E(1, LI10)
/* 0313 */ DECL_E(1, LIN1)
/* 0314 */ DECL_E(1, LINI)
/* 0315 */ DECL_E(2, LIB)
/* 0316 */ DECL_E(3, LIW)
/* 0317 */ DECL_E(2, LINB)

/* 0320 */ DECL_E(2, LIHB)
/* 0321 */ DECL_E(1, LID0)
/* 0322 */ DECL_E(1, LA0)
/* 0323 */ DECL_E(1, LA1)
/* 0324 */ DECL_E(1, LA2)
/* 0325 */ DECL_E(1, LA3)
/* 0326 */ DECL_E(1, LA6)
/* 0327 */ DECL_E(1, LA8)

/* 0330 */ DECL_E(2, LAB)
/* 0331 */ DECL_E(3, LAW)
/* 0332 */ DECL_E(1, GA0)
/* 0333 */ DECL_E(1, GA1)
/* 0334 */ DECL_E(2, GAB)
/* 0335 */ DECL_E(3, GAW)
/* 0337 */ DECL_E(1, EFC0)

/* 0340 */ DECL_E(1, EFC1)
/* 0341 */ DECL_E(1, EFC2)
/* 0342 */ DECL_E(1, EFC3)
/* 0343 */ DECL_E(1, EFC4)
/* 0344 */ DECL_E(1, EFC5)
/* 0345 */ DECL_E(1, EFC6)
/* 0346 */ DECL_E(1, EFC7)
/* 0347 */ DECL_E(1, EFC8)

/* 0350 */ DECL_E(1, EFC9)
/* 0351 */ DECL_E(1, EFC10)
/* 0352 */ DECL_E(1, EFC11)
/* 0353 */ DECL_E(1, EFC12)
/* 0354 */ DECL_E(2, EFCB)
/* 0355 */ DECL_E(3, LFC)
/* 0356 */ DECL_E(1, SFC)
/* 0357 */ DECL_E(1, RET)

/* 0360 */ DECL_E(2, KFCB)
/* 0361 */ DECL_E(1, ME)
/* 0362 */ DECL_E(1, MX)
/* 0363 */ DECL_E(1, BLT)
/* 0364 */ DECL_E(1, BLTL)
/* 0365 */ DECL_E(1, BLTC)
/* 0366 */ DECL_E(1, BLTCL)
/* 0367 */ DECL_E(1, LP)

/* 0370 */ DECL_E(1, ESC)
/* 0371 */ DECL_E(1, ESCL)
/* 0372 */ DECL_E(1, LGA0)
/* 0373 */ DECL_E(2, LGAB)
/* 0374 */ DECL_E(3, LGAW)
/* 0375 */ DECL_E(3, DESC)
/* 0377 */ DECL_E(1, RESRVD)

//
// ESC
//
/* 0002 */ DECL_E(2, MW)
/* 0003 */ DECL_E(2, MR)
/* 0004 */ DECL_E(2, NC)
/* 0005 */ DECL_E(2, BC)
/* 0006 */ DECL_E(2, REQ)
/* 0007 */ DECL_E(2, SM)

/* 0010 */ DECL_E(2, SMF)
/* 0011 */ DECL_E(2, GMF)
/* 0012 */ DECL_E(2, AF)
/* 0013 */ DECL_E(2, FF)
/* 0014 */ DECL_E(2, PI)
/* 0015 */ DECL_E(2, PO)
/* 0016 */ DECL_E(2, POR)
/* 0017 */ DECL_E(2, SPP)

/* 0020 */ DECL_E(2, DI)
/* 0021 */ DECL_E(2, EI)
/* 0022 */ DECL_E(2, XOR)
/* 0023 */ DECL_E(2, DAND)
/* 0024 */ DECL_E(2, DIOR)
/* 0025 */ DECL_E(2, DXOR)
/* 0026 */ DECL_E(2, ROTATE)
/* 0027 */ DECL_E(2, DSHIFT)

/* 0030 */ DECL_E(2, LINT)
/* 0031 */ DECL_E(2, JS)
/* 0032 */ DECL_E(2, RCFS)
/* 0033 */ DECL_E(3, RC)
/* 0034 */ DECL_E(2, UDIV)
/* 0035 */ DECL_E(2, LUDIV)
/* 0036 */ DECL_E(3, ROB)
/* 0037 */ DECL_E(3, WOB)

/* 0040 */ DECL_E(3, DSK)
/* 0041 */ DECL_E(3, XE)
/* 0042 */ DECL_E(3, XF)
/* 0043 */ DECL_E(3, LSK)
/* 0044 */ DECL_E(2, BNDCKL)
/* 0045 */ DECL_E(2, NILCK)
/* 0046 */ DECL_E(2, NILCKL)
/* 0047 */ DECL_E(2, BLTLR)

/* 0050 */ DECL_E(2, BLEL)
/* 0051 */ DECL_E(2, BLECL)
/* 0052 */ DECL_E(2, CKSUM)
/* 0053 */ DECL_E(2, BITBLT)
/* 0054 */ DECL_E(2, TXTBLT)
/* 0055 */ DECL_E(2, BYTBLT)
/* 0056 */ DECL_E(2, BYTBLTR)
/* 0057 */ DECL_E(2, VERSION)

/* 0060 */ DECL_E(2, DMUL)
/* 0061 */ DECL_E(2, SDIV)
/* 0062 */ DECL_E(2, SDDIV)
/* 0063 */ DECL_E(2, UDDIV)

/* 0160 */ DECL_E(2, WRPSB)
/* 0161 */ DECL_E(2, WRMDS)
/* 0162 */ DECL_E(2, WRWP)
/* 0163 */ DECL_E(2, WRWDC)
/* 0164 */ DECL_E(2, WRPTC)
/* 0165 */ DECL_E(2, WRIT)
/* 0166 */ DECL_E(2, WRXTS)
/* 0167 */ DECL_E(2, WRMP)

/* 0170 */ DECL_E(2, RRPSB)
/* 0171 */ DECL_E(2, RRMDS)
/* 0172 */ DECL_E(2, RRWP)
/* 0173 */ DECL_E(2, RRWDC)
/* 0174 */ DECL_E(2, RRPTC)
/* 0175 */ DECL_E(2, RRIT)
/* 0176 */ DECL_E(2, RRXTS)

/* 0211 */ DECL_E(2, CALLAGENT)
/* 0212 */ DECL_E(2, MAPDISPLAY)
/* 0213 */ DECL_E(2, STOPEMULATOR)
/* 0214 */ DECL_E(2, 214)

/* 0300 */ DECL_E(2, COLORBLT)
/* 0304 */ DECL_E(2, 305)
/* 0306 */ DECL_E(2, 306)

#endif /* OPCODE_H_ */
