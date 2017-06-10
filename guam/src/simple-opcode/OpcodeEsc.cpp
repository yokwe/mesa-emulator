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

// 010  ASSIGN_ESC(a, SMF)
// 011  ASSIGN_ESC(a, GMF)
// 012  ASSIGN_ESC(a, AF)
// 013  ASSIGN_ESC(a, FF)
// 014  ASSIGN_ESC(a, PI)
// 015  ASSIGN_ESC(a, PO)
// 016  ASSIGN_ESC(a, POR)
// 017  ASSIGN_ESC(a, SPP)

// 020  ASSIGN_ESC(a, DI)
// 021  ASSIGN_ESC(a, EI)
// 022  ASSIGN_ESC(a, XOR)
// 023  ASSIGN_ESC(a, DAND)
// 024  ASSIGN_ESC(a, DIOR)
// 025  ASSIGN_ESC(a, DXOR)
// 026  ASSIGN_ESC(a, ROTATE)
// 027  ASSIGN_ESC(a, DSHIFT)

// 030  ASSIGN_ESC(a, LINT)
// 031  ASSIGN_ESC(a, JS)
// 032  ASSIGN_ESC(a, RCFS)
// 033  ASSIGN_ESC(b, RC)
// 034  ASSIGN_ESC(a, UDIV)
// 035  ASSIGN_ESC(a, LUDIV)
// 036  ASSIGN_ESC(b, ROB)
// 037  ASSIGN_ESC(b, WOB)

// 040  ASSIGN_ESC(b, DSK)
// 041  ASSIGN_ESC(b, XE)
// 042  ASSIGN_ESC(b, XF)
// 043  ASSIGN_ESC(b, LSK)
// 044  ASSIGN_ESC(a, BNDCKL)
// 045  ASSIGN_ESC(a, NILCK)
// 046  ASSIGN_ESC(a, NILCKL)
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
// 061  ASSIGN_ESC(a, SDIV)
// 062  ASSIGN_ESC(a, SDDIV)
// 063  ASSIGN_ESC(a, UDDIV)
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
// 0161  ASSIGN_ESC(a, WRMDS)
// 0162  ASSIGN_ESC(a, WRWP)
// 0163  ASSIGN_ESC(a, WRWDC)
// 0164  ASSIGN_ESC(a, WRPTC)
// 0165  ASSIGN_ESC(a, WRIT)
// 0166  ASSIGN_ESC(a, WRXTS)
// 0167  ASSIGN_ESC(a, WRMP)

// 0170  ASSIGN_ESC(a, RRPSB)
// 0171  ASSIGN_ESC(a, RRMDS)
// 0172  ASSIGN_ESC(a, RRWP)
// 0173  ASSIGN_ESC(a, RRWDC)
// 0174  ASSIGN_ESC(a, RRPTC)
// 0175  ASSIGN_ESC(a, RRIT)
// 0176  ASSIGN_ESC(a, RRXTS)
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
