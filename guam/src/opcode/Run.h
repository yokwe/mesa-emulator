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
// Run.h
//

#ifndef RUN_H_
#define RUN_H_

#include "../mesa/MesaBasic.h"

class Run {
public:
	static const CARD32 FLAG_ARG_0  = 0;
	static const CARD32 FLAG_ARG_1  = 1;
	static const CARD32 FLAG_ARG_2  = 2;
	static const CARD32 FLAG_ARG_3  = 3;
	static const CARD32 FLAG_ARG_3A = 4;
	static const CARD32 FLAG_ARG_3C = 5;
	static const CARD32 FLAG_ARG_3J = 6;
	static const CARD32 FLAG_ARG_4  = 7;

	typedef void (*FUNC)(Run run);

	Run() : u(0) {}
	Run(CARD32 code_, CARD32 length_) :
		code(code_), length(length_), arg(0) {}
	Run(CARD32 code_, CARD32 length_, CARD16 arg_) :
		code(code_), length(length_), arg(arg_) {}
	Run(CARD32 code_, CARD32 length_, CARD8 arg0_, CARD8 arg1_) :
		code(code_), length(length_), arg0(arg0_), arg1(arg1_) {}
	Run(CARD32 code_, CARD32 length_, CARD8 arg0_, CARD8 arg1_, CARD8 arg2_) :
		code(code_), length(length_), arg30(arg0_), arg31(arg1_) , arg32(arg2_) {}

	void set3a(CARD32 code_, CARD32 length_, CARD8 arg0_, CARD8 arg1_, CARD8 arg2_) {
		code   = code_;
		length = length_;
		arg3a0 = arg0_;
		arg3a1 = arg1_;
		arg3a2 = arg2_;
	}

	void set3c(CARD32 code_, CARD32 length_, CARD8 arg0_, CARD8 arg1_, CARD8 arg2_) {
		code   = code_;
		length = length_;
		arg3c0 = arg0_;
		arg3c1 = arg1_;
		arg3c2 = arg2_;
	}

	void set3d(CARD32 code_, CARD32 length_, CARD8 arg0_, CARD8 arg1_, CARD8 arg2_) {
		code   = code_;
		length = length_;
		arg3d0 = arg0_;
		arg3d1 = arg1_;
		arg3d2 = arg2_;
	}

	void set3j(CARD32 code_, CARD32 length_, CARD8 arg0_, CARD8 arg1_, CARD8 arg2_) {
		code   = code_;
		length = length_;
		arg3j0 = arg0_;
		arg3j1 = arg1_;
		arg3j2 = arg2_;
	}

	Run(CARD32 code_, CARD32 length_, CARD8 arg0_, CARD8 arg1_, CARD8 arg2_, CARD8 arg3_) :
		code(code_), length(length_), arg40(arg0_), arg41(arg1_), arg42(arg2_), arg43(arg3_) {}


	CARD32 getCode() const {
		return code;
	}
	CARD32 getLength() const {
		return length;
	}
	CARD16 getArg() const {
		return (CARD16)arg;
	}
	CARD8 getArg0() const {
		return (CARD8)arg0;
	}
	CARD8 getArg1() const {
		return (CARD8)arg1;
	}
	CARD8 getArg30() const {
		return (CARD8)arg30;
	}
	CARD8 getArg31() const {
		return (CARD8)arg31;
	}
	CARD8 getArg32() const {
		return (CARD8)arg32;
	}
	CARD8 getArg3a0() const {
		return (CARD8)arg3a0;
	}
	CARD8 getArg3a1() const {
		return (CARD8)arg3a1;
	}
	CARD8 getArg3a2() const {
		return (CARD8)arg3a2;
	}
	CARD8 getArg3c0() const {
		return (CARD8)arg3c0;
	}
	CARD8 getArg3c1() const {
		return (CARD8)arg3c1;
	}
	CARD8 getArg3c2() const {
		return (CARD8)arg3c2;
	}
	CARD8 getArg3d0() const {
		return (CARD8)arg3d0;
	}
	CARD8 getArg3d1() const {
		return (CARD8)arg3d1;
	}
	CARD8 getArg3d2() const {
		return (CARD8)arg3d2;
	}
	CARD8 getArg3j0() const {
		return (CARD8)arg3j0;
	}
	CARD8 getArg3j1() const {
		return (CARD8)arg3j1;
	}
	CARD16 getArg3j2() const {
		return (CARD16)SignExtend((CARD8)arg3j2);
	}
	CARD8 getArg40() const {
		return (CARD8)arg40;
	}
	CARD8 getArg41() const {
		return (CARD8)arg41;
	}
	CARD8 getArg42() const {
		return (CARD8)arg42;
	}
	CARD8 getArg43() const {
		return (CARD8)arg43;
	}

	int isNull() const {
		return u == 0;
	}

	const char* getName() const {
		return nameTable[code];
	}

	static void execute(Run run_) {
		if (DEBUG_SHOW_STATS_RUN) statTable[run_.code]++;
		funcTable[run_.code](run_);
	}

	static void stats();

	static const char* trace(Run run_);

	static void initialize();

protected:
	union {
		CARD32 u;
		struct {
			CARD32 code   : 8;
			CARD32 length : 8;
			union {
				CARD32 arg : 16;
				struct {
					CARD32 arg0 : 8;
					CARD32 arg1 : 8;
				};
				struct {
					CARD32 arg30 : 5;
					CARD32 arg31 : 5;
					CARD32 arg32 : 5;
				};
				struct {
					CARD32 arg3a0 : 8;
					CARD32 arg3a1 : 4;
					CARD32 arg3a2 : 4;
				};
				struct {
					CARD32 arg3c0 : 4;
					CARD32 arg3c1 : 4;
					CARD32 arg3c2 : 8;
				};
				struct {
					CARD32 arg3d0 : 6;
					CARD32 arg3d1 : 6;
					CARD32 arg3d2 : 4;
				};
				struct {
					CARD32 arg3j0 : 4;
					CARD32 arg3j1 : 4;
					CARD32 arg3j2 : 8;
				};
				struct {
					CARD32 arg40 : 4;
					CARD32 arg41 : 4;
					CARD32 arg42 : 4;
					CARD32 arg43 : 4;
				};
			};
		};
	};

	static const int TABLE_SIZE = 1 << 8;
	static FUNC        funcTable[TABLE_SIZE];
	static const char* nameTable[TABLE_SIZE];
	static CARD32      flagTable[TABLE_SIZE];
	static long long   statTable[TABLE_SIZE];

	static void assign(CARD32 code_, FUNC func_, const char* name_, const CARD32 flag_);
};

#undef RUN_DECL
#define RUN_DECL(code, flag, name) \
static const CARD32 r##name = code; \
extern void R_##name(Run run_);

#include "Run_decl.h"

#endif
