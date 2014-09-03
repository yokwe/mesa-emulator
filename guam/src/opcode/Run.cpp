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
// Run.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("Run");

#include "../util/Debug.h"
#include "../util/Perf.h"

#include "../mesa/Function.h"

#include "Interpreter.h"

#include "Run.h"

Run::FUNC   Run::funcTable[TABLE_SIZE];
const char* Run::nameTable[TABLE_SIZE];
CARD32      Run::flagTable[TABLE_SIZE];
long long   Run::statTable[TABLE_SIZE];

static const int TRACE_BUFFER_SIZE = 100;
static char traceBuffer[TRACE_BUFFER_SIZE];

const char* Run::trace(Run run_) {
	switch(flagTable[run_.code]) {
	case FLAG_ARG_0:
		sprintf(traceBuffer, "%8X  %4X  %10s", CodeCache::CB(), savedPC, nameTable[run_.code]);
		break;
	case FLAG_ARG_1:
		sprintf(traceBuffer, "%8X  %4X  %10s  %4X", CodeCache::CB(), savedPC, nameTable[run_.code], run_.arg);
		break;
	case FLAG_ARG_2:
		sprintf(traceBuffer, "%8X  %4X  %10s  %3d %3d", CodeCache::CB(), savedPC, nameTable[run_.code], run_.arg0, run_.arg1);
		break;
	case FLAG_ARG_3:
		sprintf(traceBuffer, "%8X  %4X  %10s  %3d %3d %3d", CodeCache::CB(), savedPC, nameTable[run_.code], run_.arg30, run_.arg31, run_.arg32);
		break;
	case FLAG_ARG_3A:
		sprintf(traceBuffer, "%8X  %4X  %10s  %3d %3d %3d", CodeCache::CB(), savedPC, nameTable[run_.code], run_.arg3a0, run_.arg3a1, run_.arg3a2);
		break;
	case FLAG_ARG_3C:
		sprintf(traceBuffer, "%8X  %4X  %10s  %3d %3d %3d", CodeCache::CB(), savedPC, nameTable[run_.code], run_.arg3c0, run_.arg3c1, run_.arg3c2);
		break;
	case FLAG_ARG_3J:
		sprintf(traceBuffer, "%8X  %4X  %10s  %3d %3d %3d", CodeCache::CB(), savedPC, nameTable[run_.code], run_.arg3j0, run_.arg3j1, SignExtend((CARD8)run_.arg3j2));
		break;
	case FLAG_ARG_4:
		sprintf(traceBuffer, "%8X  %4X  %10s  %3d %3d %3d %3d", CodeCache::CB(), savedPC, nameTable[run_.code], run_.arg40, run_.arg41, run_.arg42, run_.arg43);
		break;
	default:
		ERROR();
		break;
	}
	return traceBuffer;
}


void Run::assign(CARD32 code_, FUNC func_, const char* name_, const CARD32 flag_) {
	// sanity check
	if (code_ == 0) {
		logger.fatal("code_ == 0");
		ERROR();
	}
	if (TABLE_SIZE <= code_) {
		logger.fatal("TABLE_SIZE <= code_  %d", code_);
		ERROR();
	}
	if (func_ == 0) {
		logger.fatal("func_ == 0");
		ERROR();
	}
	if (funcTable[code_]) {
		logger.fatal("funcTable[%d]", code_);
		ERROR();
	}
	funcTable[code_] = func_;
	nameTable[code_] = name_;
	flagTable[code_] = flag_;
}

void Run::stats() {
	if (DEBUG_SHOW_STATS_RUN) {
		long long total = 0;
		logger.info("==== Run stats  START");
		for(int i = 0; i < TABLE_SIZE; i++) {
			if (funcTable[i] == 0) continue;
			logger.info("stats run  %3d  %-24s %10lld", i, nameTable[i], statTable[i]);
			total += statTable[i];
		}
		logger.info("total = %lld", total);
		logger.info("==== Run stats  STOP");
	}
}

#undef RUN_DECL
#define RUN_DECL(code, flag, name) Run::assign(r##name, R_##name, #name, flag);

void Run::initialize() {
	for(int i = 0; i < TABLE_SIZE; i++) {
		funcTable[i] = 0;
		nameTable[i] = 0;
		flagTable[i] = 0;
		statTable[i] = 0;
	}

#include "Run_decl.h"

}
