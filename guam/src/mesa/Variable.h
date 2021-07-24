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
// Variable.h
//

#ifndef VARIABLE_H__
#define VARIABLE_H__

#include "MesaBasic.h"
#include "Type.h"

// 3.3.2 Evaluation Stack
extern CARD16 stack[StackDepth];
extern CARD16 SP;

// 3.3.3 Data and Status Registers
extern CARD16 PID[4]; // Processor ID
//extern CARD16 MP;     // Maintenance Panel
//extern CARD32 IT;     // Interval Timer
//extern CARD16 WM;     // Wakeup mask register - 10.4.4
//extern CARD16 WP;     // Wakeup pending register - 10.4.4.1
//extern CARD16 WDC;    // Wakeup disable counter - 10.4.4.3
//extern CARD16 PTC;    // Process timeout counter - 10.4.5
extern CARD16 XTS;    // Xfer trap status - 9.5.5

// 3.3.1 Control Registers
extern CARD16            PSB; // PsbIndex - 10.1.1
//extern MdsHandle         MDS;
//extern LocalFrameHandle  LF;  // POINTER TO LocalVariables
extern GlobalFrameHandle GF;  // LONG POINTER TO GlobalVarables
//extern CARD32            CB;  // LONG POINTER TO CodeSegment
extern CARD16            PC;
extern GFTHandle         GFI;

// 4.5 Instruction Execution
extern CARD8 breakByte;
extern CARD16 savedPC;
extern CARD16 savedSP;

// 10.4.1 Scheduler
//extern int running;

// 10.4.5 Timeouts
// TimeOutInterval:LONG CARDINAL;
// One tick = 40 milliseconds
//const LONG_CARDINAL TimeOutInterval = 40 * 1000;

// time: LONG CARDINAL
// Due to name conflict with time, rename to time_CheckForTimeouts
//extern LONG_CARDINAL lastTimeoutTime;

#endif
