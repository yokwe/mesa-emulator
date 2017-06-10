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
static log4cpp::Category& logger = Logger::getLogger("2xx");

#include "../util/Debug.h"
#include "../util/Perf.h"

#include "../mesa/MesaThread.h"

#include "Opcode.h"
#include "Interpreter.h"



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// 0200  ASSIGN_MOP(z, CATCH)
// 0201  ASSIGN_MOP(z, J2)
// 0202  ASSIGN_MOP(z, J3)
// 0203  ASSIGN_MOP(z, J4)
// 0204  ASSIGN_MOP(z, J5)
// 0205  ASSIGN_MOP(z, J6)
// 0206  ASSIGN_MOP(z, J7)
// 0207  ASSIGN_MOP(z, J8)

// 0210  ASSIGN_MOP(z, JB)
// 0211  ASSIGN_MOP(z, JW)
// 0212  ASSIGN_MOP(z, JEP)
// 0213  ASSIGN_MOP(z, JEB)
// 0214  ASSIGN_MOP(z, JEBB)
// 0215  ASSIGN_MOP(z, JNEP)
// 0216  ASSIGN_MOP(z, JNEB)
// 0217  ASSIGN_MOP(z, JNEBB)

// 0220  ASSIGN_MOP(z, JLB)
// 0221  ASSIGN_MOP(z, JGEB)
// 0222  ASSIGN_MOP(z, JGB)
// 0223  ASSIGN_MOP(z, JLEB)
// 0224  ASSIGN_MOP(z, JULB)
// 0225  ASSIGN_MOP(z, JUGEB)
// 0226  ASSIGN_MOP(z, JUGB)
// 0227  ASSIGN_MOP(z, JULEB)

// 0230  ASSIGN_MOP(z, JZ3)
// 0231  ASSIGN_MOP(z, JZ4)
// 0232  ASSIGN_MOP(z, JZB)
// 0233  ASSIGN_MOP(z, JNZ3)
// 0234  ASSIGN_MOP(z, JNZ4)
// 0235  ASSIGN_MOP(z, JNZB)
// 0236  ASSIGN_MOP(z, JDEB)
// 0237  ASSIGN_MOP(z, JDNEB)

// 0240  ASSIGN_MOP(z, JIB)
// 0241  ASSIGN_MOP(z, JIW)
// 0242  ASSIGN_MOP(z, REC)
// 0243  ASSIGN_MOP(z, REC2)
// 0244  ASSIGN_MOP(z, DIS)
// 0245  ASSIGN_MOP(z, DIS2)
// 0246  ASSIGN_MOP(z, EXCH)
// 0247  ASSIGN_MOP(z, DEXCH)

// 0250  ASSIGN_MOP(z, DUP)
// 0251  ASSIGN_MOP(z, DDUP)
// 0252  ASSIGN_MOP(z, EXDIS)
// 0253  ASSIGN_MOP(z, NEG)
// 0254  ASSIGN_MOP(z, INC)
// 0255  ASSIGN_MOP(z, DEC)
// 0256  ASSIGN_MOP(z, DINC)
// 0257  ASSIGN_MOP(z, DBL)

// 0260  ASSIGN_MOP(z, DDBL)
// 0261  ASSIGN_MOP(z, TRPL)
// 0262  ASSIGN_MOP(z, AND)
// 0263  ASSIGN_MOP(z, IOR)
// 0264  ASSIGN_MOP(z, ADDSB)
// 0265  ASSIGN_MOP(z, ADD)
// 0266  ASSIGN_MOP(z, SUB)
// 0267  ASSIGN_MOP(z, DADD)

// 0270  ASSIGN_MOP(z, DSUB)
// 0271  ASSIGN_MOP(z, ADC)
// 0272  ASSIGN_MOP(z, ACD)
// 0273  ASSIGN_MOP(z, AL0IB)
// 0274  ASSIGN_MOP(z, MUL)
// 0275  ASSIGN_MOP(z, DCMP)
// 0276  ASSIGN_MOP(z, UDCMP)
// 0277  ASSIGN_MOP(z, VMFIND)

