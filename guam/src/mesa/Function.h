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
// Function
//

#ifndef FUNCTIONE_H_
#define FUNCTIONE_H_ 1

#include "Type.h"
#include "Variable.h"


// Block Transfer

// Control Transfer

// 9.3 Control Transfer Primitives
extern void XFER(ControlLink dst, ShortControlLink src, XferType type, int freeFlag);

// 9.5.1 Trap Routines
extern void BoundsTrap();
extern void BreakTrap();
extern void CodeTrap(GFTHandle gfi);
extern void ControlTrap(ShortControlLink src);
extern void DivCheckTrap();
extern void DivZeroTrap();
extern void EscOpcodeTrap(BYTE opcode);
extern void InterruptError();
extern void OpcodeTrap(BYTE opcode);
extern void PointerTrap();
extern void ProcessTrap();
extern void RescheduleError();
extern void StackError();
extern void UnboundTrap(ControlLink dst);
extern void HardwareError();

// Process

// 10.4.1 Scheduler
extern void Reschedule(int preemption = 0);

// 10.4.2.1 Saving and Loading Process State
extern void SaveProcess(int preemption);
extern LocalFrameHandle LoadProcess();

// 10.4.2.2 State Vector Allocation
extern int EmptyState(CARD16 pri);

// 10.4.3 Faults
extern void FrameFault(FSIndex fsi);
extern void PageFault(LONG_POINTER ptr);
extern void WriteProtectFault(LONG_POINTER ptr);

// 10.4.4.2 Interrupt Processing
extern int ProcessInterrupt();
extern int CheckForInterrupt();

// 10.4.5 Timeouts
extern int TimeoutScan();
extern int CheckForTimeouts();

#endif
