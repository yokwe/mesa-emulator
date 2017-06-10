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

// 0300  ASSIGN_MOP(z, LI0)
// 0301  ASSIGN_MOP(z, LI1)
// 0302  ASSIGN_MOP(z, LI2)
// 0303  ASSIGN_MOP(z, LI3)
// 0304  ASSIGN_MOP(z, LI4)
// 0305  ASSIGN_MOP(z, LI5)
// 0306  ASSIGN_MOP(z, LI6)
// 0307  ASSIGN_MOP(z, LI7)

// 0310  ASSIGN_MOP(z, LI8)
// 0311  ASSIGN_MOP(z, LI9)
// 0312  ASSIGN_MOP(z, LI10)
// 0313  ASSIGN_MOP(z, LIN1)
// 0314  ASSIGN_MOP(z, LINI)
// 0315  ASSIGN_MOP(z, LIB)
// 0316  ASSIGN_MOP(z, LIW)
// 0317  ASSIGN_MOP(z, LINB)

// 0320  ASSIGN_MOP(z, LIHB)
// 0321  ASSIGN_MOP(z, LID0)
// 0322  ASSIGN_MOP(z, LA0)
// 0323  ASSIGN_MOP(z, LA1)
// 0324  ASSIGN_MOP(z, LA2)
// 0325  ASSIGN_MOP(z, LA3)
// 0326  ASSIGN_MOP(z, LA6)
// 0327  ASSIGN_MOP(z, LA8)

// 0330  ASSIGN_MOP(z, LAB)
// 0331  ASSIGN_MOP(z, LAW)
// 0332  ASSIGN_MOP(z, GA0)
// 0333  ASSIGN_MOP(z, GA1)
// 0334  ASSIGN_MOP(z, GAB)
// 0335  ASSIGN_MOP(z, GAW)
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

// 0370  ASSIGN_MOP(z, ESC)
// 0371  ASSIGN_MOP(z, ESCL)
// 0372  ASSIGN_MOP(z, LGA0)
// 0373  ASSIGN_MOP(z, LGAB)
// 0374  ASSIGN_MOP(z, LGAW)
// 0375  ASSIGN_MOP(z, DESC)
// 0376
// 0377  ASSIGN_MOP(z, RESRVD)
