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
// Opcode.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("Opcode");

#include "../util/Debug.h"
#include "../util/Perf.h"

#include "../mesa/MesaThread.h"

#include "Opcode.h"
#include "HotSpot.h"
#include "Interpreter.h"

__attribute__((always_inline)) static inline void R_MOP_OPCODE_TRAP_(Run run) {
	if (PERF_ENABLE) perf_OpcodeTrap++;
	OpcodeTrap((CARD8)run.getArg());
}
DEF_R(MOP_OPCODE_TRAP)

__attribute__((always_inline)) static inline Run C_MOP_OPCODE_TRAP_(Opcode* opcode_) {
	Run ret(rMOP_OPCODE_TRAP, L_MOP_OPCODE_TRAP, (CARD16)opcode_->getCode());
	return ret;
}
DEF_CI(MOP_OPCODE_TRAP)


__attribute__((always_inline)) static inline void R_ESC_OPCODE_TRAP_(Run run) {
	if (PERF_ENABLE) perf_EscOpcodeTrap++;
	EscOpcodeTrap((CARD8)run.getArg());
}
DEF_R(ESC_OPCODE_TRAP)

__attribute__((always_inline)) static inline Run C_ESC_OPCODE_TRAP_(Opcode* opcode_) {
	Run ret(rESC_OPCODE_TRAP, L_ESC_OPCODE_TRAP, (CARD16)opcode_->getCode());
	return ret;
}
DEF_CI(ESC_OPCODE_TRAP)


__attribute__((always_inline)) static inline void R_RESCHEDULE_(Run /*run*/) {
	if (!InterruptThread::isEnabled()) return;
	if (!ProcessorThread::getRequestReschedule()) return;
	// If there is reschedule request, throw exception.
	ERROR_RequestReschedule();
}
DEF_R(RESCHEDULE)

int Opcode::isOpcodeTrap() {
	return (compileProc == C_ESC_OPCODE_TRAP) || (compileProc == C_MOP_OPCODE_TRAP);
}
