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
// Interpreter.h
//

#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include "Opcode.h"
#include "Run.h"

class Interpreter {
public:
	static const int TABLE_SIZE = 256;

	__attribute__((always_inline)) static void dispatchEsc(CARD32 opcode) {
		// ESC and ESCL
		//logger.debug("dispatch ESC  %04X opcode = %03o", savedPC, opcode);
		tableEsc[opcode].interpret();
		// increment stat counter after execution. We don't count ABORTED instruction.
		if (DEBUG_SHOW_STATS_OPCODE) statEsc[opcode]++;
	}

	__attribute__((always_inline)) static inline void dispatchMop(CARD32 opcode) {
		if (PERF_ENABLE) perf_Dispatch++;
		tableMop[opcode].interpret();
		// increment stat counter after execution. We don't count ABORTED instruction.
		if (DEBUG_SHOW_STATS_OPCODE) statMop[opcode]++;
	}

	static inline void execute() {
		savedPC = PC;
		savedSP = SP;
		dispatchMop(GetCodeByte());
	}

	// Implementation Specific
	static void initialize() {
		initRegisters();

		for(int i = 0; i < TABLE_SIZE; i++) {
			tableMop[i].empty();
			tableEsc[i].empty();
			statMop[i] = 0;
			statEsc[i] = 0;
		}
		initTable();
		for(CARD32 i = 0; i < TABLE_SIZE; i++) {
			if (tableMop[i].isEmpty()) assignMop(Opcode(i, 1, "__MOP_OPCODE_TRAP__", C_MOP_OPCODE_TRAP, I_MOP_OPCODE_TRAP));
			if (tableEsc[i].isEmpty()) assignEsc(Opcode(i, 2, "__ESC_OPCODE_TRAP__", C_ESC_OPCODE_TRAP, I_ESC_OPCODE_TRAP));
		}

		Run::initialize();
	}
	static void assignMop(const Opcode& op);
	static void assignEsc(const Opcode& op);

	static Opcode* getMop(CARD8 code) {
		return tableMop + code;
	}
	static Opcode* getEsc(CARD8 code) {
		return tableEsc + code;
	}

	static void stats();

private:
	static Opcode    tableMop[TABLE_SIZE];
	static Opcode    tableEsc[TABLE_SIZE];
	static long long statMop [TABLE_SIZE];
	static long long statEsc [TABLE_SIZE];

	static void initTable();
	static void initRegisters();
};

#endif
