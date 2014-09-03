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
// Run_decl.h
//


RUN_DECL(@no@, FLAG_ARG_0, MOP_OPCODE_TRAP)
RUN_DECL(@no@, FLAG_ARG_0, ESC_OPCODE_TRAP)
RUN_DECL(@no@, FLAG_ARG_0, ESC)
RUN_DECL(@no@, FLAG_ARG_0, RESCHEDULE)


//
// Opcode_simple.cpp
//
// MOP
RUN_DECL(@no@, FLAG_ARG_0, NOOP)
RUN_DECL(@no@, FLAG_ARG_1, LL)
RUN_DECL(@no@, FLAG_ARG_1, LLD)
RUN_DECL(@no@, FLAG_ARG_1, SL)
RUN_DECL(@no@, FLAG_ARG_1, SLD)
RUN_DECL(@no@, FLAG_ARG_1, PL)
RUN_DECL(@no@, FLAG_ARG_1, PLD)
RUN_DECL(@no@, FLAG_ARG_1, LG)
RUN_DECL(@no@, FLAG_ARG_1, LGD)
RUN_DECL(@no@, FLAG_ARG_1, SG)
RUN_DECL(@no@, FLAG_ARG_0, BNDCK)
RUN_DECL(@no@, FLAG_ARG_0, BRK)
RUN_DECL(@no@, FLAG_ARG_1, R)
RUN_DECL(@no@, FLAG_ARG_1, RL)
RUN_DECL(@no@, FLAG_ARG_1, RD)
RUN_DECL(@no@, FLAG_ARG_1, RDL)
RUN_DECL(@no@, FLAG_ARG_1, W)
RUN_DECL(@no@, FLAG_ARG_1, PS)
RUN_DECL(@no@, FLAG_ARG_1, WL)
RUN_DECL(@no@, FLAG_ARG_1, PSL)
RUN_DECL(@no@, FLAG_ARG_1, WD)
RUN_DECL(@no@, FLAG_ARG_1, PSD)
RUN_DECL(@no@, FLAG_ARG_1, WDL)
RUN_DECL(@no@, FLAG_ARG_1, PSDL)
RUN_DECL(@no@, FLAG_ARG_2, RLI)
RUN_DECL(@no@, FLAG_ARG_2, RLIL)
RUN_DECL(@no@, FLAG_ARG_2, RLDI)
RUN_DECL(@no@, FLAG_ARG_2, RLDIL)
RUN_DECL(@no@, FLAG_ARG_2, RGI)
RUN_DECL(@no@, FLAG_ARG_2, RGIL)
RUN_DECL(@no@, FLAG_ARG_2, WLI)
RUN_DECL(@no@, FLAG_ARG_2, WLIL)
RUN_DECL(@no@, FLAG_ARG_2, WLDIL)
RUN_DECL(@no@, FLAG_ARG_1, RS)
RUN_DECL(@no@, FLAG_ARG_1, RLS)
RUN_DECL(@no@, FLAG_ARG_1, WS)
RUN_DECL(@no@, FLAG_ARG_1, WLS)
RUN_DECL(@no@, FLAG_ARG_2, RF)
RUN_DECL(@no@, FLAG_ARG_2, RLF)
RUN_DECL(@no@, FLAG_ARG_0, RLFS)
RUN_DECL(@no@, FLAG_ARG_2, RLIPF)
RUN_DECL(@no@, FLAG_ARG_2, RLILPF)
RUN_DECL(@no@, FLAG_ARG_2, WF)
RUN_DECL(@no@, FLAG_ARG_2, PSF)
RUN_DECL(@no@, FLAG_ARG_2, WSF)
RUN_DECL(@no@, FLAG_ARG_2, WLF)
RUN_DECL(@no@, FLAG_ARG_2, PSLF)
RUN_DECL(@no@, FLAG_ARG_0, WLFS)
RUN_DECL(@no@, FLAG_ARG_1, SGD)
RUN_DECL(@no@, FLAG_ARG_1, LLK)
RUN_DECL(@no@, FLAG_ARG_1, RKI)
RUN_DECL(@no@, FLAG_ARG_1, RKDI)
RUN_DECL(@no@, FLAG_ARG_1, LK)
RUN_DECL(@no@, FLAG_ARG_0, SHIFT)
RUN_DECL(@no@, FLAG_ARG_1, SHIFTI)
RUN_DECL(@no@, FLAG_ARG_1, CATCH)
RUN_DECL(@no@, FLAG_ARG_1, J)
RUN_DECL(@no@, FLAG_ARG_2, JUE)
RUN_DECL(@no@, FLAG_ARG_1, JEB)
RUN_DECL(@no@, FLAG_ARG_2, JUNE)
RUN_DECL(@no@, FLAG_ARG_1, JNEB)
RUN_DECL(@no@, FLAG_ARG_1, JLB)
RUN_DECL(@no@, FLAG_ARG_1, JGEB)
RUN_DECL(@no@, FLAG_ARG_1, JGB)
RUN_DECL(@no@, FLAG_ARG_1, JLEB)
RUN_DECL(@no@, FLAG_ARG_1, JULB)
RUN_DECL(@no@, FLAG_ARG_1, JUGEB)
RUN_DECL(@no@, FLAG_ARG_1, JUGB)
RUN_DECL(@no@, FLAG_ARG_1, JULEB)
RUN_DECL(@no@, FLAG_ARG_1, JZ)
RUN_DECL(@no@, FLAG_ARG_1, JNZ)
RUN_DECL(@no@, FLAG_ARG_1, JDE)
RUN_DECL(@no@, FLAG_ARG_1, JDNE)
RUN_DECL(@no@, FLAG_ARG_1, JIB)
RUN_DECL(@no@, FLAG_ARG_1, JIW)
RUN_DECL(@no@, FLAG_ARG_0, REC)
RUN_DECL(@no@, FLAG_ARG_0, REC2)
RUN_DECL(@no@, FLAG_ARG_0, DIS)
RUN_DECL(@no@, FLAG_ARG_0, DIS2)
RUN_DECL(@no@, FLAG_ARG_0, EXCH)
RUN_DECL(@no@, FLAG_ARG_0, DEXCH)
RUN_DECL(@no@, FLAG_ARG_0, DUP)
RUN_DECL(@no@, FLAG_ARG_0, DDUP)
RUN_DECL(@no@, FLAG_ARG_0, EXDIS)
RUN_DECL(@no@, FLAG_ARG_0, NEG)
RUN_DECL(@no@, FLAG_ARG_0, INC)
RUN_DECL(@no@, FLAG_ARG_0, DEC)
RUN_DECL(@no@, FLAG_ARG_0, DINC)
RUN_DECL(@no@, FLAG_ARG_0, DBL)
RUN_DECL(@no@, FLAG_ARG_0, DDBL)
RUN_DECL(@no@, FLAG_ARG_0, TRPL)
RUN_DECL(@no@, FLAG_ARG_0, AND)
RUN_DECL(@no@, FLAG_ARG_0, IOR)
RUN_DECL(@no@, FLAG_ARG_1, ADDI)
RUN_DECL(@no@, FLAG_ARG_0, ADD)
RUN_DECL(@no@, FLAG_ARG_0, SUB)
RUN_DECL(@no@, FLAG_ARG_0, DADD)
RUN_DECL(@no@, FLAG_ARG_0, DSUB)
RUN_DECL(@no@, FLAG_ARG_0, ADC)
RUN_DECL(@no@, FLAG_ARG_0, ACD)
RUN_DECL(@no@, FLAG_ARG_1, AL0I)
RUN_DECL(@no@, FLAG_ARG_0, MUL)
RUN_DECL(@no@, FLAG_ARG_0, DCMP)
RUN_DECL(@no@, FLAG_ARG_0, UDCMP)
RUN_DECL(@no@, FLAG_ARG_0, VMFIND)
RUN_DECL(@no@, FLAG_ARG_1, LI)
RUN_DECL(@no@, FLAG_ARG_1, LID)
RUN_DECL(@no@, FLAG_ARG_1, LA)
RUN_DECL(@no@, FLAG_ARG_1, GA)
RUN_DECL(@no@, FLAG_ARG_0, LP)
RUN_DECL(@no@, FLAG_ARG_1, LGA)
RUN_DECL(@no@, FLAG_ARG_1, DESC)
RUN_DECL(@no@, FLAG_ARG_0, RESRVD)
// ESC
RUN_DECL(@no@, FLAG_ARG_0, SM)
RUN_DECL(@no@, FLAG_ARG_0, GMF)
RUN_DECL(@no@, FLAG_ARG_0, SMF)
RUN_DECL(@no@, FLAG_ARG_0, DI)
RUN_DECL(@no@, FLAG_ARG_0, EI)
RUN_DECL(@no@, FLAG_ARG_0, XOR)
RUN_DECL(@no@, FLAG_ARG_0, DAND)
RUN_DECL(@no@, FLAG_ARG_0, DIOR)
RUN_DECL(@no@, FLAG_ARG_0, DXOR)
RUN_DECL(@no@, FLAG_ARG_0, ROTATE)
RUN_DECL(@no@, FLAG_ARG_0, DSHIFT)
RUN_DECL(@no@, FLAG_ARG_0, LINT)
RUN_DECL(@no@, FLAG_ARG_0, JS)
RUN_DECL(@no@, FLAG_ARG_0, RCFS)
RUN_DECL(@no@, FLAG_ARG_1, RC)
RUN_DECL(@no@, FLAG_ARG_0, UDIV)
RUN_DECL(@no@, FLAG_ARG_0, LUDIV)
RUN_DECL(@no@, FLAG_ARG_1, ROB)
RUN_DECL(@no@, FLAG_ARG_1, WOB)
RUN_DECL(@no@, FLAG_ARG_1, DSK)
RUN_DECL(@no@, FLAG_ARG_1, LSK)
RUN_DECL(@no@, FLAG_ARG_0, BNDCKL)
RUN_DECL(@no@, FLAG_ARG_0, NILCK)
RUN_DECL(@no@, FLAG_ARG_0, NILCKL)
RUN_DECL(@no@, FLAG_ARG_0, DMUL)
RUN_DECL(@no@, FLAG_ARG_0, SDIV)
RUN_DECL(@no@, FLAG_ARG_0, SDDIV)
RUN_DECL(@no@, FLAG_ARG_0, UDDIV)
RUN_DECL(@no@, FLAG_ARG_0, WRPSB)
RUN_DECL(@no@, FLAG_ARG_0, WRMDS)
RUN_DECL(@no@, FLAG_ARG_0, WRWP)
RUN_DECL(@no@, FLAG_ARG_0, WRWDC)
RUN_DECL(@no@, FLAG_ARG_0, WRPTC)
RUN_DECL(@no@, FLAG_ARG_0, WRIT)
RUN_DECL(@no@, FLAG_ARG_0, WRXTS)
RUN_DECL(@no@, FLAG_ARG_0, WRMP)
RUN_DECL(@no@, FLAG_ARG_0, RRPSB)
RUN_DECL(@no@, FLAG_ARG_0, RRMDS)
RUN_DECL(@no@, FLAG_ARG_0, RRWP)
RUN_DECL(@no@, FLAG_ARG_0, RRWDC)
RUN_DECL(@no@, FLAG_ARG_0, RRPTC)
RUN_DECL(@no@, FLAG_ARG_0, RRIT)
RUN_DECL(@no@, FLAG_ARG_0, RRXTS)


//
// Ocode_bitblt.cpp
//
RUN_DECL(@no@, FLAG_ARG_0, COLORBLT)
RUN_DECL(@no@, FLAG_ARG_0, BITBLT)


//
// Opcode_block.cpp
//
RUN_DECL(@no@, FLAG_ARG_0, BLT)
RUN_DECL(@no@, FLAG_ARG_0, BLTL)
RUN_DECL(@no@, FLAG_ARG_0, BLTC)
RUN_DECL(@no@, FLAG_ARG_0, BLTCL)
RUN_DECL(@no@, FLAG_ARG_0, BLTLR)
RUN_DECL(@no@, FLAG_ARG_0, BLEL)
RUN_DECL(@no@, FLAG_ARG_0, BLECL)
RUN_DECL(@no@, FLAG_ARG_0, CKSUM)
RUN_DECL(@no@, FLAG_ARG_0, BYTBLT)
RUN_DECL(@no@, FLAG_ARG_0, BYTBLTR)


//
// Opcode_control.cpp
//
RUN_DECL(@no@, FLAG_ARG_1, EFC)
RUN_DECL(@no@, FLAG_ARG_1, LFC)
RUN_DECL(@no@, FLAG_ARG_0, SFC)
RUN_DECL(@no@, FLAG_ARG_0, RET)
RUN_DECL(@no@, FLAG_ARG_1, KFC)
RUN_DECL(@no@, FLAG_ARG_0, AF)
RUN_DECL(@no@, FLAG_ARG_0, FF)
RUN_DECL(@no@, FLAG_ARG_0, PI)
RUN_DECL(@no@, FLAG_ARG_0, PO)
RUN_DECL(@no@, FLAG_ARG_1, XE)
RUN_DECL(@no@, FLAG_ARG_1, XF)


//
// Opcode_special.cpp
//
RUN_DECL(@no@, FLAG_ARG_0, CALLAGENT)
RUN_DECL(@no@, FLAG_ARG_0, MAPDISPLAY)
RUN_DECL(@no@, FLAG_ARG_0, STOPEMULATOR)
RUN_DECL(@no@, FLAG_ARG_0, VERSION)
RUN_DECL(@no@, FLAG_ARG_0, 214)
RUN_DECL(@no@, FLAG_ARG_0, 305)
RUN_DECL(@no@, FLAG_ARG_0, 306)


//
// Opcode_process.cpp
//
RUN_DECL(@no@, FLAG_ARG_0, ME)
RUN_DECL(@no@, FLAG_ARG_0, MX)
RUN_DECL(@no@, FLAG_ARG_0, MW)
RUN_DECL(@no@, FLAG_ARG_0, MR)
RUN_DECL(@no@, FLAG_ARG_0, NC)
RUN_DECL(@no@, FLAG_ARG_0, BC)
RUN_DECL(@no@, FLAG_ARG_0, REQ)
RUN_DECL(@no@, FLAG_ARG_0, SPP)


//
// Combined opcodes
//

// Pass 1
RUN_DECL(@no@, FLAG_ARG_4,  RECPLLIMULRECLGDDADDRL)
RUN_DECL(@no@, FLAG_ARG_3J, LLINCLLJULB)
RUN_DECL(@no@, FLAG_ARG_4,  LLDLLRLSSL)
RUN_DECL(@no@, FLAG_ARG_3,  LLDLLRLS)
RUN_DECL(@no@, FLAG_ARG_3,  LLLLADDSL)
RUN_DECL(@no@, FLAG_ARG_3,  LLDBLRLDIACD)
RUN_DECL(@no@, FLAG_ARG_3A, RLLLDACDSLD)
RUN_DECL(@no@, FLAG_ARG_3,  RLRLDIACDSLD)
RUN_DECL(@no@, FLAG_ARG_3A, LLLLDACDRL)
RUN_DECL(@no@, FLAG_ARG_3J, LLLLJLB)
RUN_DECL(@no@, FLAG_ARG_3C, RLILSL)
RUN_DECL(@no@, FLAG_ARG_4,  SLLLLLADDSL)
RUN_DECL(@no@, FLAG_ARG_4,  LLDECDBLRLDIACDWDL)
RUN_DECL(@no@, FLAG_ARG_4,  LLDECDBLRLDIACDRDL)
RUN_DECL(@no@, FLAG_ARG_2,  LLJNEB)
RUN_DECL(@no@, FLAG_ARG_3J, PLLLJNEB)
RUN_DECL(@no@, FLAG_ARG_3,  LKSLSL)
RUN_DECL(@no@, FLAG_ARG_3,  LKSLPLDBL)
RUN_DECL(@no@, FLAG_ARG_4,  RLDIACDRLSL)
RUN_DECL(@no@, FLAG_ARG_4,  RLSLRLDI)
RUN_DECL(@no@, FLAG_ARG_3,  LLDECLLRD)
RUN_DECL(@no@, FLAG_ARG_4,  LALLPSF)
RUN_DECL(@no@, FLAG_ARG_3,  LLDUPDECLLRD)
RUN_DECL(@no@, FLAG_ARG_3,  PLDBLPLLL)
RUN_DECL(@no@, FLAG_ARG_2,  LLJULB)
RUN_DECL(@no@, FLAG_ARG_3,  SLDRLDIL)
RUN_DECL(@no@, FLAG_ARG_1,  MULRECLLDDADD)
RUN_DECL(@no@, FLAG_ARG_2,  LLJUGB)
RUN_DECL(@no@, FLAG_ARG_2,  IORJZ)
RUN_DECL(@no@, FLAG_ARG_1,  LLDUDCMP)
RUN_DECL(@no@, FLAG_ARG_2,  LLJGB)
RUN_DECL(@no@, FLAG_ARG_2,  LLSHIFTI)
RUN_DECL(@no@, FLAG_ARG_2,  LLJEB)
RUN_DECL(@no@, FLAG_ARG_2,  IORJNZ)


// Pass 2
RUN_DECL(@no@, FLAG_ARG_3,  SLLLLL)
RUN_DECL(@no@, FLAG_ARG_2,  LLLL)
RUN_DECL(@no@, FLAG_ARG_2,  LKSL)
RUN_DECL(@no@, FLAG_ARG_2,  LLLI)
RUN_DECL(@no@, FLAG_ARG_2,  SLDSLD)
RUN_DECL(@no@, FLAG_ARG_3,  SLDSLDSLD)
RUN_DECL(@no@, FLAG_ARG_2,  LIJ)
RUN_DECL(@no@, FLAG_ARG_2,  LLJ)
RUN_DECL(@no@, FLAG_ARG_1,  LLINC)
RUN_DECL(@no@, FLAG_ARG_2,  LLDLLD)
RUN_DECL(@no@, FLAG_ARG_1,  RECSL)
RUN_DECL(@no@, FLAG_ARG_1,  LLDACD)
RUN_DECL(@no@, FLAG_ARG_2,  LLDLL)
RUN_DECL(@no@, FLAG_ARG_2,  LLDLI)
RUN_DECL(@no@, FLAG_ARG_2,  PLLI)
RUN_DECL(@no@, FLAG_ARG_1,  LALP)
RUN_DECL(@no@, FLAG_ARG_2,  LLWSF)
RUN_DECL(@no@, FLAG_ARG_2,  LISL)
RUN_DECL(@no@, FLAG_ARG_2,  LALI)
RUN_DECL(@no@, FLAG_ARG_2,  SLDLLD)
RUN_DECL(@no@, FLAG_ARG_2,  SLLL)
