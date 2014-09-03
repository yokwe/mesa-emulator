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
// HotSPot.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("HotSpot");

#include "../util/Debug.h"
#include "../util/Perf.h"

#include "../mesa/MesaThread.h"

#include "HotSpot.h"
#include "Opcode.h"
#include "Interpreter.h"

#include <QMap>

static int count_update  = 0;
static int count_create  = 0;
static int count_compile = 0;
static int count_run     = 0;
static int count_count   = 0;

static int count_cacheHit1    = 0;
static int count_cacheHit2    = 0;
static int count_cacheHotSpot = 0;


QHash<HotSpot::Address, HotSpot*> HotSpot::hotSpots;

__attribute__((always_inline)) static inline void R_RESCHEDULE_() {
	if (!InterruptThread::isEnabled()) return;
	if (!ProcessorThread::getRequestReschedule()) return;
	// If there is reschedule request, throw exception.
	ERROR_RequestReschedule();
}

void HotSpot::run(HotSpot* hotSpot) {
	if (PERF_ENABLE) count_run++;
	int count = 0;
	if (DEBUG_SHOW_HOTSPOT_RUN) logger.info("run     ----");
	for(;;) {
		if (PERF_ENABLE) hotSpot->countRun++;

		// Use c++ 11 style range base for loop with constant reference
		for(const auto& run: hotSpot->opcodes) {
			// Prepare for restart of opcode
			savedPC = PC;
			savedSP = SP;
			//
			if (DEBUG_SHOW_HOTSPOT_RUN) logger.info("run     %s", Run::trace(run));
			// Update PC (Do same as we do in I_XXX functions)
			PC += run.getLength();
			// Run
			Run::execute(run);
		}
		R_RESCHEDULE_();

		{
			Address address;
			// First check nextCache1
			{
				HotSpot* next = hotSpot->nextCache1;
				if (next && next->address == address) {
					if (DEBUG_SHOW_HOTSPOT_RUN) logger.info("chain 1  ---- %6d", ++count);
					if (PERF_ENABLE) hotSpot->countCacheHit++;
					hotSpot = next;
					if (PERF_ENABLE) count_cacheHit1++;
					continue;
				}
			}
			// Second check nextCache2
			{
				HotSpot* next = hotSpot->nextCache2;
				if (next && next->address == address) {
					if (DEBUG_SHOW_HOTSPOT_RUN) logger.info("chain 2  ---- %6d", ++count);
					if (PERF_ENABLE) hotSpot->countCacheHit++;
					hotSpot->nextCache2 = hotSpot->nextCache1;
					hotSpot->nextCache1 = next;
					hotSpot = next;
					if (PERF_ENABLE) count_cacheHit2++;
					continue;
				}
			}
			// Otherwise check hotSpots
			{
				HotSpot* next = hotSpots.value(address, 0);
				if (next && next->compiled) {
					if (DEBUG_SHOW_HOTSPOT_RUN) logger.info("chain H  ---- %6d", ++count);
					if (PERF_ENABLE) hotSpot->countCacheHit++;
					hotSpot->nextCache2 = hotSpot->nextCache1;
					hotSpot->nextCache1 = next;
					hotSpot = next;
					if (PERF_ENABLE) count_cacheHotSpot++;
					continue;
				}
				// Do special if last opcode is XFER.
				if (hotSpot->lastCodeIsXFER) {
					if (next) {
						if (next->countInvoke < (THREASHOLD_COUNT_XFER / 10)) {
							next->countInvoke++;
						} else {
							// Compile this block
							compile(next->address.cb, next->address.pc, MAX_COMPILE_LEVEL_XFER);
							if (DEBUG_SHOW_HOTSPOT_RUN) logger.info("chain C  ---- %6d", ++count);
							if (PERF_ENABLE) hotSpot->countCacheHit++;
							// Maintain nextCache
							hotSpot->nextCache2 = hotSpot->nextCache1;
							hotSpot->nextCache1 = next;
							hotSpot = next;
							if (PERF_ENABLE) count_cacheHotSpot++;
							continue;
						}
					} else {
						next = new HotSpot;
						hotSpots.insert(address, next);
						next->countInvoke++;
					}
				}
			}
		}
		break;
	}
	if (DEBUG_SHOW_HOTSPOT_RUN) logger.info("chain   ---- %6d", count);
}

HotSpot* HotSpot::compile(CARD32 cb_, CARD16 pc_, const int level) {
	if (level <= 0) return 0;
	const int nextLevel = level - 1;

	// Get hotSpot of new location
	HotSpot* hotSpot = hotSpots.value(Address(cb_, pc_), 0);
	// Nothing to do. Because hotSpot is already compiled.
	if (hotSpot && hotSpot->compiled) return hotSpot;

	if (PERF_ENABLE) count_compile++;

	// save running context
	CARD32 compile_save_CB = CodeCache::CB();
	CARD16 compile_save_PC = PC;
	CARD16 compile_save_savedPC = savedPC;

	// Initialize CB and PC for new location
	if (CodeCache::CB() != cb_) CodeCache::setCB(cb_);
	PC = pc_;

	if (hotSpot == 0) {
		hotSpot = new HotSpot;
		hotSpots.insert(hotSpot->address, hotSpot);
	}
	// To prevent recursive process, set compiled as 1
	hotSpot->compiled = 1;

	if (DEBUG_SHOW_HOTSPOT_COMPILE) logger.info("hotspot %8X  %4X  -----------", CodeCache::CB(), PC);
	for(;;) {
		// To emulate running context, set savedPC to point first byte of instruction.
		savedPC = PC;
		// Find opcode
		Opcode* opcode;
		CARD8   code = GetCodeByte();
		switch (code) {
		case zESC:
		case zESCL:
			code = GetCodeByte();
			opcode = Interpreter::getEsc(code);
			break;
		default:
			opcode = Interpreter::getMop(code);
			break;
		}
		// Compile opcode
		Run run = opcode->compile();
		// PC is already advanced to next opcode
#if 0
		// sanity check
		if (PC != savedPC + opcode->getLength()) {
			logger.fatal("opcode %3o %d  %s", opcode->getCode(), opcode->getLength(), opcode->getName());
			ERROR();
		}
#endif
		// Do special for opcode
		hotSpot->opcodes.append(run);
		switch(run.getCode()) {
		// Unconditional JUMP
		case rJ: {
			CARD16 nextPC = (CARD16)(savedPC + run.getArg());
			if (DEBUG_SHOW_HOTSPOT_COMPILE) logger.info("special %s  -> %4X", Run::trace(run), nextPC);
			// Compile destination of Jump
			hotSpot->nextCache1 = compile(cb_, nextPC, nextLevel);
		}
			break;
		// Conditional JUMP with getArg
		case rJZ:
		case rJNZ:
		case rJDE:
		case rJDNE:
		case rJEB:
		case rJNEB:
		case rJLB:
		case rJGEB:
		case rJGB:
		case rJLEB:
		case rJULB:
		case rJUGEB:
		case rJUGB:
		case rJULEB: {
			CARD16 nextPC = (CARD16)(savedPC + run.getArg());
			if (DEBUG_SHOW_HOTSPOT_COMPILE) logger.info("special %s  -> %4X", Run::trace(run), nextPC);
			// Compile from PC as another HotSpot
			hotSpot->nextCache1 = compile(cb_, PC, nextLevel);
			// Compile destination of Jump
			hotSpot->nextCache2 = compile(cb_, nextPC, nextLevel);
		}
			break;
		// Conditional JUMP with getArg1
		case rJUE:
		case rJUNE: {
			CARD16 nextPC = (CARD16)(savedPC + SignExtend(run.getArg1()));
			if (DEBUG_SHOW_HOTSPOT_COMPILE) logger.info("special %s  -> %4X", Run::trace(run), nextPC);
			// Compile from PC as another HotSpot
			hotSpot->nextCache1 = compile(cb_, PC, nextLevel);
			// Compile destination of Jump
			hotSpot->nextCache2 = compile(cb_, nextPC, nextLevel);
		}
			break;
		case rJIB: {
			if (DEBUG_SHOW_HOTSPOT_COMPILE) logger.info("special %s", Run::trace(run));
			// Assume last instruction is LIn
			Run last = hotSpot->opcodes.value(hotSpot->opcodes.size() - 2);
			if (last.getCode() != rLI) {
				logger.info("code = %d", last.getCode());
				ERROR();
			}
			// Compile from PC as another HotSpot
			hotSpot->nextCache1 = compile(cb_, PC, nextLevel);
			// Compile from each index
			CARD16 limit = last.getArg();
			for(CARD16 index = 0; index < limit; index++) {
				BytePair disp = {ReadCode(run.getArg() + index / 2)};
				CARD16 nextPC = savedPC + (((index % 2) == 0) ? disp.left : disp.right);
				hotSpot->nextCache2 = compile(cb_, nextPC, nextLevel);
			}
		}
			break;
		case rJIW: {
			if (DEBUG_SHOW_HOTSPOT_COMPILE) logger.info("special %s", Run::trace(run));
			// Assume last instruction is LIn
			Run last = hotSpot->opcodes.value(hotSpot->opcodes.size() - 2);
			if (last.getCode() != rLI) {
				logger.info("code = %d", last.getCode());
				ERROR();
			}
			// Compile from PC as another HotSpot
			hotSpot->nextCache1 = compile(cb_, PC, nextLevel);
			// Compile from each index
			CARD16 limit = last.getArg();
			for(CARD16 index = 0; index < limit; index++) {
				CARD16 disp = ReadCode(run.getArg() + index);
				CARD16 nextPC = savedPC + disp;
				hotSpot->nextCache2 = compile(cb_, nextPC, nextLevel);
			}
		}
			break;
		// This is the last opcode for current hotSpot.
		// Next opcode starts next hotSpot.
		case rEFC:
		case rLFC:
		case rSFC:
		case rKFC:
		case rPO:
		case rXE:
		case rXF:
		case rME:
		case rMX:
		case rMW:
		case rMR:
		case rNC:
		case rBC:
		case rREQ:
		case rSPP:
			if (DEBUG_SHOW_HOTSPOT_COMPILE) logger.info("special %s", Run::trace(run));
			// Compile from PC as another HotSpot
			hotSpot->nextCache1 = compile(cb_, PC, nextLevel);
			break;
		case rMOP_OPCODE_TRAP: {
			if (DEBUG_SHOW_HOTSPOT_COMPILE) logger.info("special %s", Run::trace(run));
			// OPCODE_TRAP will generate ABORT trap. So this is the last opcode in hotSpot.
			// There is no opcode length information. Figure out opcode length from opcode.
			CARD8 code = run.getArg();
			switch(code) {
			default:
				logger.fatal("mop opcode trap  %o", code);
				ERROR();
				break;
			}
		}
			break;
		case rESC_OPCODE_TRAP: {
			if (DEBUG_SHOW_HOTSPOT_COMPILE) logger.info("special %s", Run::trace(run));
			// OPCODE_TRAP will generate ABORT trap. So this is the last opcode in hotSpot.
			// There is no opcode length information. Figure out opcode length from opcode.
			CARD8 code = run.getArg();
			switch(code) {
			case aTXTBLT:
			case aFSUB:
			case aFMUL:
			case aFDIV:
			case aFCOMP:
			case aFLOAT:
				// Length of opcode is 2. So no need to bump PC.
				// Compile from PC as another HotSpot
				compile(cb_, PC, nextLevel);
				break;
			default:
				logger.fatal("esc opcode trap  %o", code);
				ERROR();
				break;
			}
		}
			break;
		case rRET:
			if (DEBUG_SHOW_HOTSPOT_COMPILE) logger.info("special %s", Run::trace(run));
			break;
		default:
			if (DEBUG_SHOW_HOTSPOT_COMPILE) logger.info("special %s", Run::trace(run));
			continue;
		}
		break;
	}

	// Peephole Optimization
	if (DEBUG_ENABLE_PEEPHOLE) peephole(hotSpot);

	switch(hotSpot->opcodes.last().getCode()) {
	case rEFC:
	case rLFC:
	case rSFC:
	case rKFC:
	case rPO:
	case rXE:
	case rXF:
//	case rME:
//	case rMX:
//	case rMW:
//	case rMR:
//	case rNC:
//	case rBC:
//	case rREQ:
//	case rSPP:
		hotSpot->lastCodeIsXFER = 1;
		break;
	}

	// restore running context
	if (CodeCache::CB() != compile_save_CB) CodeCache::setCB(compile_save_CB);
	PC = compile_save_PC;
	savedPC = compile_save_savedPC;

	return hotSpot;
}

void HotSpot::update(const int compileLevel, const int threasholdCount) {
	if (PERF_ENABLE) count_update++;

	const Address address;
	HotSpot* hotSpot = hotSpots.value(address, 0);
	if (hotSpot) {
		if (hotSpot->compiled) {
			run(hotSpot);
			return;
		}
		if (hotSpot->countInvoke < threasholdCount) {
			hotSpot->countInvoke++;
			if (PERF_ENABLE) count_count++;
			return;
		}
		if (DEBUG_SHOW_HOTSPOT_COMPILE) logger.info("===========");
		compile(address.cb, address.pc, compileLevel);
		run(hotSpot);
		return;
	} else {
		if (PERF_ENABLE) count_create++;
		hotSpot = new HotSpot;
		hotSpots.insert(address, hotSpot);
		hotSpot->countInvoke++;
		return;
	}
}

void HotSpot::stats() {
	int countHotSpot         = 0;
	int countCompiled        = 0;
	int countRun             = 0;
	int countCacheHit        = 0;
	int countCompiledUsed    = 0;

	int codeCompiled = 0;
	int codeCompiledNotUsed = 0;
	long long codeCompiledTotalRun = 0;

	if (!PERF_ENABLE) {
		for(auto& key: hotSpots.keys()) {
			countHotSpot++;
			const HotSpot* value = hotSpots.value(key);
			if (!value->compiled) continue;
			countCompiled++;
			codeCompiled += value->opcodes.size();
			if (DEBUG_SHOW_HOTSPOT_DETAIL) {
				logger.info("stats  %04X  %8X  %4X  %3d  %10d",
					value->gfi, value->address.cb, value->address.pc, value->opcodes.size(), value->countInvoke);

				CodeCache::setCB(value->address.cb);
				PC = value->address.pc;
				for(const Run& run_ : value->opcodes) {
					savedPC = PC;
					logger.info("list   %04X  %s", value->gfi, Run::trace(run_));
					PC += run_.getLength();
				}
				logger.info("chain  %6s  %6s  invoke = %8d  %s",
						(value->nextCache1 ? "CACHE1" : ""), (value->nextCache2 ? "CACHE2" : ""), value->countInvoke, value->opcodes.last().getName());
			}
		}
		logger.info("hotSpots  hotSpot = %d  compiled = %d  compiledCodeSize = %d", countHotSpot, countCompiled, codeCompiled);
	} else {
		for(auto& key: hotSpots.keys()) {
			countHotSpot++;
			const HotSpot* value = hotSpots.value(key);
			if (!value->compiled) continue;
			codeCompiled += value->opcodes.size();

			countCompiled++;
			countRun += value->countRun;
			countCacheHit += value->countCacheHit;
			if (value->countRun) {
				countCompiledUsed++;
				codeCompiledTotalRun += value->opcodes.size() * (long long)value->countRun;
			} else {
				codeCompiledNotUsed += value->opcodes.size();
			}

			if (DEBUG_SHOW_HOTSPOT_DETAIL) {
				logger.info("stats  %04X  %8X  %4X  %d  %3d  %10d  %10d  %10d",
					value->gfi, value->address.cb, value->address.pc, value->compiled, value->opcodes.size(), value->countInvoke, value->countRun, value->countCacheHit);
				if (value->countRun) {
					CodeCache::setCB(value->address.cb);
					PC = value->address.pc;
					for(const Run& run_ : value->opcodes) {
						savedPC = PC;
						logger.info("list   %04X  %s", value->gfi, Run::trace(run_));
						PC += run_.getLength();
					}
					logger.info("chain  %6s  %6s  invoke = %8d  run = %8d  hit = %8d  miss = %8d  %s",
							(value->nextCache1 ? "CACHE1" : ""), (value->nextCache2 ? "CACHE2" : ""), value->countInvoke, value->countRun, value->countCacheHit, (value->countRun - value->countCacheHit), value->opcodes.last().getName());
				}
			}
		}
		if (DEBUG_SHOW_HOTSPOT_USAGE) {
			QMap<QString, CARD32> t2;
			QMap<QString, CARD32> t3;
			QMap<QString, CARD32> t4;
			QMap<QString, CARD32> t5;
			QMap<QString, CARD32> t6;
			QMap<QString, CARD32> t7;
			QMap<QString, CARD32> t8;
			QMap<QString, CARD32> t0;

			for(auto& hotSpotsKey :hotSpots.keys()) {
				const HotSpot* hotSpot = hotSpots.value(hotSpotsKey);

				for(int i = 0; i < hotSpot->opcodes.size(); i++) {
					const Run& r0 = hotSpot->opcodes.value(i + 0);
					const Run& r1 = hotSpot->opcodes.value(i + 1);
					const Run& r2 = hotSpot->opcodes.value(i + 2);
					const Run& r3 = hotSpot->opcodes.value(i + 3);
					const Run& r4 = hotSpot->opcodes.value(i + 4);
					const Run& r5 = hotSpot->opcodes.value(i + 5);
					const Run& r6 = hotSpot->opcodes.value(i + 6);
					const Run& r7 = hotSpot->opcodes.value(i + 7);

#define ADD_T2 {                                          \
	QString key;                                          \
	key.sprintf("%10s %10s",                              \
		r0.getName(), r1.getName());                      \
	if (!t2.contains(key)) t2.insert(key, 0);             \
	t2[key] += hotSpot->countRun;                         \
}
#define ADD_T3 {                                          \
	QString key;                                          \
	key.sprintf("%10s %10s %10s",                         \
		r0.getName(), r1.getName(), r2.getName());        \
	if (!t3.contains(key)) t3.insert(key, 0);             \
	t3[key] += hotSpot->countRun;                         \
}
#define ADD_T4 {                                          \
	QString key;                                          \
	key.sprintf("%10s %10s %10s %10s",                    \
		r0.getName(), r1.getName(), r2.getName(),         \
		r3.getName());                                    \
	if (!t4.contains(key)) t4.insert(key, 0);             \
	t4[key] += hotSpot->countRun;                         \
}
#define ADD_T5 {                                          \
	QString key;                                          \
	key.sprintf("%10s %10s %10s %10s %10s",               \
		r0.getName(), r1.getName(), r2.getName(),         \
		r3.getName(), r4.getName());                      \
	if (!t5.contains(key)) t5.insert(key, 0);             \
	t5[key] += hotSpot->countRun;                         \
}
#define ADD_T6 {                                          \
	QString key;                                          \
	key.sprintf("%10s %10s %10s %10s %10s %10s",          \
		r0.getName(), r1.getName(), r2.getName(),         \
		r3.getName(), r4.getName(), r5.getName());        \
	if (!t6.contains(key)) t6.insert(key, 0);             \
	t6[key] += hotSpot->countRun;                         \
}
#define ADD_T7 {                                          \
	QString key;                                          \
	key.sprintf("%10s %10s %10s %10s %10s %10s %10s",     \
		r0.getName(), r1.getName(), r2.getName(),         \
		r3.getName(), r4.getName(), r5.getName(),         \
		r6.getName());                                    \
	if (!t7.contains(key)) t7.insert(key, 0);             \
	t7[key] += hotSpot->countRun;                         \
}
#define ADD_T8 {                                          \
	QString key;                                          \
	key.sprintf("%10s %10s %10s %10s %10s %10s %10s %10s",\
		r0.getName(), r1.getName(), r2.getName(),         \
		r3.getName(), r4.getName(), r5.getName(),         \
		r6.getName(), r7.getName());                      \
	if (!t8.contains(key)) t8.insert(key, 0);             \
	t8[key] += hotSpot->countRun;                         \
}


					if (r1.isNull()) continue;
					ADD_T2
					if (r2.isNull()) continue;
					ADD_T3
					if (r3.isNull()) continue;
					ADD_T4
					if (r4.isNull()) continue;
					ADD_T5
					if (r5.isNull()) continue;
					ADD_T6
					if (r6.isNull()) continue;
					ADD_T7
					if (r7.isNull()) continue;
					ADD_T8
				}
			}
			for(const auto& k: t2.keys()) {
//				logger.info("USAGE2 %10d %s", t2[k], k.toLocal8Bit().data());
				t0[k] = t2[k] * (2 - 1);
			}
			for(const auto& k: t3.keys()) {
//				logger.info("USAGE3 %10d %s", t3[k], k.toLocal8Bit().data());
				t0[k] = t3[k] * (3 - 1);
			}
			for(const auto& k: t4.keys()) {
//				logger.info("USAGE4 %10d %s", t4[k], k.toLocal8Bit().data());
				t0[k] = t4[k] * (4 - 1);
			}
			for(const auto& k: t5.keys()) {
//				logger.info("USAGE5 %10d %s", t5[k], k.toLocal8Bit().data());
				t0[k] = t5[k] * (5 - 1);
			}
			for(const auto& k: t6.keys()) {
//				logger.info("USAGE6 %10d %s", t6[k], k.toLocal8Bit().data());
				t0[k] = t6[k] * (6 - 1);
			}
			for(const auto& k: t7.keys()) {
//				logger.info("USAGE7 %10d %s", t7[k], k.toLocal8Bit().data());
				t0[k] = t7[k] * (7 - 1);
			}
			for(const auto& k: t8.keys()) {
//				logger.info("USAGE8 %10d %s", t8[k], k.toLocal8Bit().data());
				t0[k] = t8[k] * (8 - 1);
			}
			for(const auto& k: t0.keys()) {
				logger.info("USAGE0 %10d %s", t0[k], k.toLocal8Bit().data());
			}
		}
		logger.info("hotSpots  hotSpot = %d  compiled = %d  run = %d  cacheHit = %d  compiledUsed = %d",
				countHotSpot, countCompiled, countRun, countCacheHit, countCompiledUsed);
		logger.info("hotSpots  update = %u  create = %u  compile = %u  run = %u  count = %u  hit = %d + %d + %d",
				count_update, count_create, count_compile, count_run, count_count, count_cacheHit1, count_cacheHit2, count_cacheHotSpot);
		logger.info("hotSpots  codeCompiled = %d  codeCompiledNotUsed = %d  codeCompiledTotalRun = %lld", codeCompiled, codeCompiledNotUsed, codeCompiledTotalRun);
	}
	logger.info("XFER  MAX_COMPILE_LEVEL = %2d  THREASHOLD_COUNT  = %5d", MAX_COMPILE_LEVEL_XFER, THREASHOLD_COUNT_XFER);
	//logger.info("JUMP  MAX_COMPILE_LEVEL = %2d  THREASHOLD_COUNT  = %5d", MAX_COMPILE_LEVEL_JUMP, THREASHOLD_COUNT_JUMP);
	logger.info("PROC  MAX_COMPILE_LEVEL = %2d  THREASHOLD_COUNT  = %5d", MAX_COMPILE_LEVEL_PROC, THREASHOLD_COUNT_PROC);
}


#define EQUALS_2(c0, c1) \
	(r0.getCode() == r##c0 && r1.getCode() == r##c1)

#define EQUALS_3(c0, c1, c2) \
	(r0.getCode() == r##c0 && r1.getCode() == r##c1 && r2.getCode() == r##c2)

#define EQUALS_4(c0, c1, c2, c3) \
	(r0.getCode() == r##c0 && r1.getCode() == r##c1 && r2.getCode() == r##c2 && r3.getCode() == r##c3)

#define EQUALS_5(c0, c1, c2, c3, c4) \
	(r0.getCode() == r##c0 && r1.getCode() == r##c1 && r2.getCode() == r##c2 && r3.getCode() == r##c3 && r4.getCode() == r##c4)

#define EQUALS_6(c0, c1, c2, c3, c4, c5) \
	(r0.getCode() == r##c0 && r1.getCode() == r##c1 && r2.getCode() == r##c2 && r3.getCode() == r##c3 && r4.getCode() == r##c4 && r5.getCode() == r##c5)

#define EQUALS_8(c0, c1, c2, c3, c4, c5, c6, c7) \
	(r0.getCode() == r##c0 && r1.getCode() == r##c1 && r2.getCode() == r##c2 && r3.getCode() == r##c3 && r4.getCode() == r##c4 && r5.getCode() == r##c5 && r6.getCode() == r##c6 && r7.getCode() == r##c7)


#define CHECK_2(a, b) \
	(a.getArg() < 256 && b.getArg() < 256)

#define CHECK_3(a, b, c) \
	(a.getArg() < 32 && b.getArg() < 32 && c.getArg() < 32)

#define CHECK_3J(a, b, c) \
	(a.getArg() < 16 && b.getArg() < 16 && c.getArg() == (CARD16)SignExtend((CARD8)c.getArg()))

#define CHECK_4(a, b, c, d) \
	(a.getArg() < 16 && b.getArg() < 16 && c.getArg() < 16 && d.getArg() < 16)

#define REPLACE_OPCODE_2(newOpcode)                                              \
	hotSpot->opcodes.replace(i, newOpcode); /* replace r0 with newOpcode */      \
	hotSpot->opcodes.remove(i + 1);         /* remove r1 */                      \
	i -= 1;                                 /* minus one for remove of r1-2 */   \
	oneMore = 1;

#define REPLACE_OPCODE_3(newOpcode)                                              \
	hotSpot->opcodes.replace(i, newOpcode); /* replace r0 with newOpcode */      \
	hotSpot->opcodes.remove(i + 1);         /* remove r1 */                      \
	hotSpot->opcodes.remove(i + 1);         /* remove r2 */                      \
	i -= 2;                                 /* minus two for remove of r1-2 */   \
	oneMore = 1;

#define REPLACE_OPCODE_4(newOpcode)                                              \
	hotSpot->opcodes.replace(i, newOpcode); /* replace r0 with newOpcode */      \
	hotSpot->opcodes.remove(i + 1);         /* remove r1 */                      \
	hotSpot->opcodes.remove(i + 1);         /* remove r2 */                      \
	hotSpot->opcodes.remove(i + 1);         /* remove r3 */                      \
	i -= 3;                                 /* minus three for remove of r1-3 */ \
	oneMore = 1;

#define REPLACE_OPCODE_5(newOpcode)                                              \
	hotSpot->opcodes.replace(i, newOpcode); /* replace r0 with newOpcode */      \
	hotSpot->opcodes.remove(i + 1);         /* remove r1 */                      \
	hotSpot->opcodes.remove(i + 1);         /* remove r2 */                      \
	hotSpot->opcodes.remove(i + 1);         /* remove r3 */                      \
	hotSpot->opcodes.remove(i + 1);         /* remove r4 */                      \
	i -= 4;                                 /* minus four for remove of r1-4 */  \
	oneMore = 1;

#define REPLACE_OPCODE_6(newOpcode)                                              \
	hotSpot->opcodes.replace(i, newOpcode); /* replace r0 with newOpcode */      \
	hotSpot->opcodes.remove(i + 1);         /* remove r1 */                      \
	hotSpot->opcodes.remove(i + 1);         /* remove r2 */                      \
	hotSpot->opcodes.remove(i + 1);         /* remove r3 */                      \
	hotSpot->opcodes.remove(i + 1);         /* remove r4 */                      \
	hotSpot->opcodes.remove(i + 1);         /* remove r5 */                      \
	i -= 5;                                 /* minus five for remove of r1-5 */ \
	oneMore = 1;

#define REPLACE_OPCODE_8(newOpcode)                                              \
	hotSpot->opcodes.replace(i, newOpcode); /* replace r0 with newOpcode */      \
	hotSpot->opcodes.remove(i + 1);         /* remove r1 */                      \
	hotSpot->opcodes.remove(i + 1);         /* remove r2 */                      \
	hotSpot->opcodes.remove(i + 1);         /* remove r3 */                      \
	hotSpot->opcodes.remove(i + 1);         /* remove r4 */                      \
	hotSpot->opcodes.remove(i + 1);         /* remove r5 */                      \
	hotSpot->opcodes.remove(i + 1);         /* remove r6 */                      \
	hotSpot->opcodes.remove(i + 1);         /* remove r7 */                      \
	i -= 7;                                 /* minus seven for remove of r1-7 */ \
	oneMore = 1;

#define LENGTH_8 (r0.getLength() + r1.getLength() + r2.getLength() + r3.getLength() + r4.getLength() + r5.getLength() + r6.getLength() + r7.getLength())
#define LENGTH_6 (r0.getLength() + r1.getLength() + r2.getLength() + r3.getLength() + r4.getLength() + r5.getLength())
#define LENGTH_5 (r0.getLength() + r1.getLength() + r2.getLength() + r3.getLength() + r4.getLength())
#define LENGTH_4 (r0.getLength() + r1.getLength() + r2.getLength() + r3.getLength())
#define LENGTH_3 (r0.getLength() + r1.getLength() + r2.getLength())
#define LENGTH_2 (r0.getLength() + r1.getLength())

void HotSpot::peephole(HotSpot* hotSpot) {
	// Pass 1
	for(;;) {
		int oneMore = 0;

		for(int i = 0; i < hotSpot->opcodes.size(); i++) {
			const Run& r0 = hotSpot->opcodes.value(i + 0);
			const Run& r1 = hotSpot->opcodes.value(i + 1);
			const Run& r2 = hotSpot->opcodes.value(i + 2);
			const Run& r3 = hotSpot->opcodes.value(i + 3);
			const Run& r4 = hotSpot->opcodes.value(i + 4);
			const Run& r5 = hotSpot->opcodes.value(i + 5);
			const Run& r6 = hotSpot->opcodes.value(i + 6);
			const Run& r7 = hotSpot->opcodes.value(i + 7);

			// REC / PL m / LI n / MUL / REC / LGD o / DADD / RL p
			if (EQUALS_8(REC, PL, LI, MUL, REC, LGD, DADD, RL)) {
				if (CHECK_4(r1, r2, r5, r7)) {
					const CARD32 len = LENGTH_8;
					Run newOpcode(rRECPLLIMULRECLGDDADDRL, len, (CARD8)r1.getArg(), (CARD8)r2.getArg(), (CARD8)r5.getArg(), (CARD8)r7.getArg());
					REPLACE_OPCODE_8(newOpcode)
					continue;
				}
			}
			// LL m / INC / LL n / JULB o
			if (EQUALS_4(LL, INC, LL, JULB)) {
				const CARD16 offset = r0.getLength() + r1.getLength() + r2.getLength() + r3.getArg();
				if ((r0.getArg() < 16 && r2.getArg() < 16 && offset == (CARD16)SignExtend((CARD8)offset))) {
					const CARD32 len    = LENGTH_4;
					Run newOpcode;
					newOpcode.set3j(rLLINCLLJULB, len, (CARD8)r0.getArg(), (CARD8)r2.getArg(), (CARD8)offset);
					REPLACE_OPCODE_4(newOpcode)
					//logger.info("AAAA LLINCLLJULB");
					continue;
				} else {
//					const INT16 offset = (CARD16)(r0.getLength() + r1.getLength() + r2.getLength() + r3.getArg());
//					logger.info("BBBB LLINCLLJULB  %3d %3d %3d", r0.getArg(), r2.getArg(), offset);
				}
			}
			// LLD m / LL n / RLS o / SL p
			if (EQUALS_4(LLD, LL, RLS, SL)) {
				if (CHECK_4(r0, r1, r2, r3)) {
					const CARD32 len = LENGTH_4;
					Run newOpcode(rLLDLLRLSSL, len, (CARD8)r0.getArg(), (CARD8)r1.getArg(), (CARD8)r2.getArg(), (CARD8)r3.getArg());
					REPLACE_OPCODE_4(newOpcode)
					//logger.info("AAAA LLDLLRLSSL  %3d %3d %3d %3d", r0.getArg(), r1.getArg(), r2.getArg(), r3.getArg());
					continue;
				} else {
					//logger.info("BBBB LLDLLRLSSL  %3d %3d %3d %3d", r0.getArg(), r1.getArg(), r2.getArg(), r3.getArg());
				}
			}
			// LLD m / LL n / RLS o
			if (EQUALS_3(LLD, LL, RLS)) {
				if (CHECK_3(r0, r1, r2)) {
					const CARD32 len = LENGTH_3;
					Run newOpcode(rLLDLLRLS, len, (CARD8)r0.getArg(), (CARD8)r1.getArg(), (CARD8)r2.getArg());
					REPLACE_OPCODE_3(newOpcode)
					//logger.info("AAAA LLDLLRLS  %3d %3d %3d", r0.getArg(), r1.getArg(), r2.getArg());
					continue;
				} else {
					//logger.info("BBBB LLDLLRLS  %3d %3d %3d", r0.getArg(), r1.getArg(), r2.getArg());
				}
			}
			// LL m / LL n / ADD / SL o
			if (EQUALS_4(LL, LL, ADD, SL)) {
				if (CHECK_3(r0, r1, r3)) {
					const CARD32 len = LENGTH_4;
					Run newOpcode(rLLLLADDSL, len, (CARD8)r0.getArg(), (CARD8)r1.getArg(), (CARD8)r3.getArg());
					REPLACE_OPCODE_4(newOpcode)
					//logger.info("AAAA LLLLADDSL");
					continue;
				} else {
					//logger.info("BBBB LLLLADDSL %3d %3d %3d", r0.getArg(), r1.getArg(), r3.getArg());
				}
			}
			// SL m / LL n / LL o / ADD / SL p
			if (EQUALS_5(SL, LL, LL, ADD, SL)) {
				if (CHECK_4(r0, r1, r2, r4)) {
					const CARD32 len = LENGTH_5;
					Run newOpcode(rSLLLLLADDSL, len, (CARD8)r0.getArg(), (CARD8)r1.getArg(), (CARD8)r2.getArg(), (CARD8)r4.getArg());
					REPLACE_OPCODE_5(newOpcode)
					//logger.info("AAAA SLLLLLADDSL %3d %3d %3d %3d", r0.getArg(), r1.getArg(), r2.getArg(), r4.getArg());
					continue;
				} else {
					//logger.info("BBBB SLLLLLADDSL %3d %3d %3d %3d", r0.getArg(), r1.getArg(), r2.getArg(), r4.getArg());
				}
			}
			// LL m / DEC / DBL / RLDI m n / ACD / WDL p
			if (EQUALS_6(LL, DEC, DBL, RLDI, ACD, WDL)) {
				if (r0.getArg() < 16 && r3.getArg0() < 16 && r3.getArg1() < 16 && r5.getArg() < 16) {
					const CARD32 len = LENGTH_6;
					Run newOpcode(rLLDECDBLRLDIACDWDL, len, (CARD8)r0.getArg(), (CARD8)r3.getArg0(), (CARD8)r3.getArg1(), (CARD8)r5.getArg());
					REPLACE_OPCODE_6(newOpcode)
					//logger.info("AAAA LLDECRDBRLDIACDWDL %3d %3d %3d %3d", r0.getArg(), r1.getArg0(), r1.getArg1(), r5.getArg());
					continue;
				} else {
					//logger.info("BBBB LLDECRDBRLDIACDWDL %3d %3d %3d %3d", r0.getArg(), r1.getArg0(), r1.getArg1(), r5.getArg());
				}
			}
			// LL m / DEC / DBL / RLDI m n / ACD / RDL p
			if (EQUALS_6(LL, DEC, DBL, RLDI, ACD, RDL)) {
				if (r0.getArg() < 16 && r3.getArg0() < 16 && r3.getArg1() < 16 && r5.getArg() < 16) {
					const CARD32 len = LENGTH_6;
					Run newOpcode(rLLDECDBLRLDIACDRDL, len, (CARD8)r0.getArg(), (CARD8)r3.getArg0(), (CARD8)r3.getArg1(), (CARD8)r5.getArg());
					REPLACE_OPCODE_6(newOpcode)
					//logger.info("AAAA LLDECRDBRLDIACDRDL %3d %3d %3d %3d", r0.getArg(), r1.getArg0(), r1.getArg1(), r5.getArg());
					continue;
				} else {
					//logger.info("BBBB LLDECRDBRLDIACDRDL %3d %3d %3d %3d", r0.getArg(), r1.getArg0(), r1.getArg1(), r5.getArg());
				}
			}
			// LL m / DBL / RLDI n o / ACD
			if (EQUALS_4(LL, DBL, RLDI, ACD)) {
				if (r0.getArg() < 32 && r2.getArg0() < 32 && r2.getArg1() < 32) {
					const CARD32 len = LENGTH_4;
					Run newOpcode(rLLDBLRLDIACD, len, (CARD8)r0.getArg(), (CARD8)r2.getArg0(), (CARD8)r2.getArg1());
					REPLACE_OPCODE_4(newOpcode)
					//logger.info("AAAA LLDBLRLDIACD");
					continue;
				} else {
					//logger.info("BBBB LLDBLRLDIACD %3d %3d %3d", r0.getArg(), r3.getArg0(), r3.getArg1());
				}
			}
			// RL m / LLD n / ACD / SLD o
			if (EQUALS_4(RL, LLD, ACD, SLD)) {
				if (r0.getArg() < 256 && r1.getArg() < 16 && r3.getArg() < 16) {
					const CARD32 len = LENGTH_4;
					Run newOpcode;
					newOpcode.set3a(rRLLLDACDSLD, len, (CARD8)r0.getArg(), (CARD8)r1.getArg(), (CARD8)r3.getArg());
					REPLACE_OPCODE_4(newOpcode)
					//logger.info("AAAA RLLLDACDSLD %3d %3d %3d", r0.getArg(), r1.getArg(), r3.getArg());
					continue;
				} else {
					//logger.info("BBBB RLLLDACDSLD %3d %3d %3d", r0.getArg(), r1.getArg(), r3.getArg());
				}
			}
			// RL m / RLDI 0 n / ACD / SLD o
			if (EQUALS_4(RL, RLDI, ACD, SLD)) {
				if (r0.getArg() < 32 && r1.getArg0() == 0 && r1.getArg1() < 32 && r3.getArg() < 32) {
					const CARD32 len = LENGTH_4;
					Run newOpcode(rRLRLDIACDSLD, len, (CARD8)r0.getArg(), (CARD8)r1.getArg1(), (CARD8)r3.getArg());
					REPLACE_OPCODE_4(newOpcode)
					continue;
				} else {
					//logger.info("BBBB RLRLDIACDSLD %3d %3d %3d %3d", r0.getArg(), r1.getArg0(), r1.getArg1(), r3.getArg());
				}
			}
			// LL m / LLD n / ACD / RL o
			if (EQUALS_4(LL, LLD, ACD, RL)) {
				if (r0.getArg() < 256 && r1.getArg() < 16 && r3.getArg() < 16) {
					const CARD32 len = LENGTH_4;
					Run newOpcode;
					newOpcode.set3a(rLLLLDACDRL, len, (CARD8)r0.getArg(), (CARD8)r1.getArg(), (CARD8)r3.getArg());
					REPLACE_OPCODE_4(newOpcode)
					//logger.info("AAAA LLLLDACDRL %3d %3d %3d", r0.getArg(), r1.getArg(), r3.getArg());
					continue;
				} else {
					//logger.info("BBBB LLLLDACDRL %3d %3d %3d", r0.getArg(), r1.getArg(), r3.getArg());
				}
			}
			//  LL m / LL n / JLB o
			if (EQUALS_3(LL, LL, JLB)) {
				const CARD16 offset = r0.getLength() + r1.getLength() + r2.getArg();
				if ((r0.getArg() < 16 && r1.getArg() < 16 && offset == (CARD16)SignExtend((CARD8)offset))) {
					const CARD32 len    = LENGTH_3;
					Run newOpcode;
					newOpcode.set3j(rLLLLJLB, len, (CARD8)r0.getArg(), (CARD8)r1.getArg(), (CARD8)offset);
					REPLACE_OPCODE_3(newOpcode)
					//logger.info("AAAA LLLLJLB %3d %3d %3d", r0.getArg(), r1.getArg(), r2.getArg());
					continue;
				} else {
					//logger.info("BBBB LLLLJLB %3d %3d %3d", r0.getArg(), r1.getArg(), r2.getArg());
				}
			}
			// RLIL m n / SL o
			if (EQUALS_2(RLIL, SL)) {
				if (r1.getArg() < 256) {
					const CARD32 len = LENGTH_2;
					Run newOpcode;
					newOpcode.set3c(rRLILSL, len, (CARD8)r0.getArg0(), (CARD8)r0.getArg1(), (CARD8)r1.getArg());
					REPLACE_OPCODE_2(newOpcode)
					//logger.info("AAAA RLILSL %3d %3d %3d", r0.getArg0(), r0.getArg1(), r1.getArg());
					continue;
				} else {
					//logger.info("BBBB RLILSL %3d %3d %3d", r0.getArg0(), r0.getArg1(), r1.getArg());
				}
			}
			// PL m / LL n / JNEB o
			if (EQUALS_3(PL, LL, JNEB)) {
				const CARD16 offset = r0.getLength() + r1.getLength() + r2.getArg();
				if ((r0.getArg() < 16 && r1.getArg() < 16 && offset == (CARD16)SignExtend((CARD8)offset))) {
					const CARD32 len    = LENGTH_3;
					Run newOpcode;
					newOpcode.set3j(rPLLLJNEB, len, (CARD8)r0.getArg(), (CARD8)r1.getArg(), (CARD8)offset);
					REPLACE_OPCODE_3(newOpcode)
					//logger.info("AAAA PLLLJNEB %3d %3d %3d", r0.getArg(), r1.getArg(), SignExtend((CARD16)r2.getArg()));
					continue;
				} else {
					//logger.info("BBBB PLLLJNEB %3d %3d %3d", r0.getArg(), r1.getArg(), SignExtend((CARD16)r2.getArg()));
				}
			}
			// LL m / JNEB n
			if (EQUALS_2(LL, JNEB)) {
				const CARD16 offset = r0.getLength() + r1.getArg();
				if (r0.getArg() < 256 && offset == (CARD16)SignExtend((CARD8)offset)) {
					const CARD32 len    = LENGTH_2;
					Run newOpcode(rLLJNEB, len, (CARD8)r0.getArg(), (CARD8)offset);
					REPLACE_OPCODE_2(newOpcode)
					//logger.info("AAAA LLJNEB %3d %3d", r0.getArg(), SignExtend((CARD16)r1.getArg()));
					continue;
				} else {
					//logger.info("BBBB LLJNEB %3d %3d", r0.getArg(), SignExtend((CARD16)r1.getArg()));
				}
			}
			// LK m / SL n / SL o
			if (EQUALS_3(LK, SL, SL)) {
				if (CHECK_3(r0, r1, r2)) {
					const CARD32 len = LENGTH_3;
					Run newOpcode(rLKSLSL, len, (CARD8)r0.getArg(), (CARD8)r1.getArg(), (CARD8)r2.getArg());
					REPLACE_OPCODE_3(newOpcode)
					//logger.info("AAAA LKSLSL %3d %3d %3d", r0.getArg(), r1.getArg(), r2.getArg());
					continue;
				} else {
					//logger.info("BBBB LKSLSL %3d %3d %3d", r0.getArg(), r1.getArg(), r2.getArg());
				}
			}
			// LK m / SL n / PL o / DBL
			if (EQUALS_4(LK, SL, PL, DBL)) {
				if (CHECK_3(r0, r1, r2)) {
					const CARD32 len = LENGTH_4;
					Run newOpcode(rLKSLPLDBL, len, (CARD8)r0.getArg(), (CARD8)r1.getArg(), (CARD8)r2.getArg());
					REPLACE_OPCODE_4(newOpcode)
					//logger.info("AAAA LKSLPLDBL %3d %3d %3d", r0.getArg(), r1.getArg(), r2.getArg());
					continue;
				} else {
					//logger.info("BBBB LKSLPLDBL %3d %3d %3d", r0.getArg(), r1.getArg(), r2.getArg());
				}
			}
			// RLDI m n / ACD / RL o / SL p
			if (EQUALS_4(RLDI, ACD, RL, SL)) {
				if (r0.getArg0() < 16 && r0.getArg1() < 16 && r2.getArg() < 16 && r3.getArg() < 16) {
					const CARD32 len = LENGTH_4;
					Run newOpcode(rRLDIACDRLSL, len, (CARD8)r0.getArg0(), (CARD8)r0.getArg1(), (CARD8)r2.getArg(), (CARD8)r3.getArg());
					REPLACE_OPCODE_4(newOpcode)
					//logger.info("AAAA RLDIACDRLSL %3d %3d %3d %3d", r0.getArg0(), r0.getArg1(), r2.getArg(), r3.getArg());
					continue;
				} else {
					//logger.info("BBBB RLDIACDRLSL %3d %3d %3d %3d", r0.getArg0(), r0.getArg1(), r2.getArg(), r3.getArg());
				}
			}
			// RL m / SL n / RLDI o p
			if (EQUALS_3(RL, SL, RLDI)) {
				if (r0.getArg() < 16 && r1.getArg() && r2.getArg0() < 16 && r2.getArg1() < 16) {
					const CARD32 len = LENGTH_3;
					Run newOpcode(rRLSLRLDI, len, (CARD8)r0.getArg(), (CARD8)r1.getArg(), (CARD8)r2.getArg0(), (CARD8)r2.getArg1());
					REPLACE_OPCODE_3(newOpcode)
					//logger.info("AAAA RLSLRLDI %3d %3d %3d %3d", r0.getArg(), r1.getArg(), r2.getArg0(), r2.getArg1());
					continue;
				} else {
					//logger.info("BBBB RLSLRLDI %3d %3d %3d %3d", r0.getArg(), r1.getArg(), r2.getArg0(), r2.getArg1());
				}
			}
			// LL m / DEC / LL n / RD o
			if (EQUALS_4(LL, DEC, LL, RD)) {
				if (CHECK_3(r0, r2, r3)) {
					const CARD32 len = LENGTH_4;
					Run newOpcode(rLLDECLLRD, len, (CARD8)r0.getArg(), (CARD8)r1.getArg(), (CARD8)r3.getArg());
					REPLACE_OPCODE_4(newOpcode)
					//logger.info("AAAA LLDECLLRD %3d %3d %3d", r0.getArg(), r2.getArg(), r3.getArg());
					continue;
				} else {
					//logger.info("BBBB LLDECLLRD %3d %3d %3d", r0.getArg(), r2.getArg(), r3.getArg());
				}
			}
			// LA m / LL n / PSF 0 o
			if (EQUALS_3(LA, LL, PSF)) {
				if (r0.getArg() < 64 && r1.getArg() < 64 && r2.getArg0() == 0 && r2.getArg1() < 16) {
					const CARD32 len = LENGTH_3;
					Run newOpcode;
					newOpcode.set3d(rLALLPSF, len, (CARD8)r0.getArg(), (CARD8)r1.getArg(), (CARD8)r2.getArg1());
					REPLACE_OPCODE_3(newOpcode)
					//logger.info("AAAA LALLPSF %3d %3d %3d", r0.getArg(), r1.getArg(), r2.getArg1());
					continue;
				} else {
					//logger.info("BBBB LALLPSF %3d %3d %3d", r0.getArg(), r1.getArg(), r2.getArg1());
				}
			}
			//  LL m / DUP / DEC / LL n / RD o
			if (EQUALS_5(LL, DUP, DEC, LL, RD)) {
				if (CHECK_3(r0, r3, r4)) {
					const CARD32 len = LENGTH_5;
					Run newOpcode(rLLDUPDECLLRD, len, (CARD8)r0.getArg(), (CARD8)r3.getArg(), (CARD8)r4.getArg());
					REPLACE_OPCODE_5(newOpcode)
					//logger.info("AAAA LLDUPDECLLRD %3d %3d %3d", r0.getArg(), r3.getArg(), r4.getArg());
					continue;
				} else {
					//logger.info("BBBB LLDUPDECLLRD %3d %3d %3d", r0.getArg(), r3.getArg(), r4.getArg());
				}
			}
			// PL m / DBL / PL n / LL o
			if (EQUALS_4(PL, DBL, PL, LL)) {
				if (CHECK_3(r0, r2, r3)) {
					const CARD32 len = LENGTH_4;
					Run newOpcode(rPLDBLPLLL, len, (CARD8)r0.getArg(), (CARD8)r2.getArg(), (CARD8)r3.getArg());
					REPLACE_OPCODE_4(newOpcode)
					//logger.info("AAAA PLDBLPLLL %3d %3d %3d", r0.getArg(), r2.getArg(), r3.getArg());
					continue;
				} else {
					//logger.info("BBBB PLDBLPLLL %3d %3d %3d", r0.getArg(), r2.getArg(), r3.getArg());
				}
			}
			// LL m / JULB n
			if (EQUALS_2(LL, JULB)) {
				const CARD16 offset = r0.getLength() + r1.getArg();
				if (r0.getArg() < 256 && offset == (CARD16)SignExtend((CARD8)offset)) {
					const CARD32 len    = LENGTH_2;
					Run newOpcode(rLLJULB, len, (CARD8)r0.getArg(), (CARD8)offset);
					REPLACE_OPCODE_2(newOpcode)
					//logger.info("AAAA LLJULB %3d %3d", r0.getArg(), SignExtend((CARD16)r1.getArg()));
					continue;
				} else {
					//logger.info("BBBB LLJULB %3d %3d", r0.getArg(), SignExtend((CARD16)r1.getArg()));
				}
			}
			//  SLD m / RLDIL n o
			if (EQUALS_2(SLD, RLDIL)) {
				if (r0.getArg() < 32 && r1.getArg0() < 32 && r1.getArg1() < 32) {
					const CARD32 len = LENGTH_2;
					Run newOpcode(rSLDRLDIL, len, (CARD8)r0.getArg(), (CARD8)r1.getArg0(), (CARD8)r1.getArg1());
					REPLACE_OPCODE_2(newOpcode)
					//logger.info("AAAA SLDRLDIL %3d %3d %3d", r0.getArg(), r1.getArg0(), r1.getArg1());
					continue;
				} else {
					//logger.info("BBBB SLDRLDIL %3d %3d %3d", r0.getArg(), r1.getArg0(), r1.getArg1());
				}
			}
			// MUL / REC / LLD m / DADD
			if (EQUALS_4(MUL, REC, LLD, DADD)) {
				const CARD32 len = LENGTH_4;
				Run newOpcode(rMULRECLLDDADD, len, (CARD8)r2.getArg());
				REPLACE_OPCODE_4(newOpcode)
				continue;
			}
			// LL m / JUGB n
			if (EQUALS_2(LL, JUGB)) {
				const CARD16 offset = r0.getLength() + r1.getArg();
				if (r0.getArg() < 256 && offset == (CARD16)SignExtend((CARD8)offset)) {
					const CARD32 len    = LENGTH_2;
					Run newOpcode(rLLJUGB, len, (CARD8)r0.getArg(), (CARD8)offset);
					REPLACE_OPCODE_2(newOpcode)
					//logger.info("AAAA LLJUGB %3d %3d", r0.getArg(), SignExtend((CARD16)r1.getArg()));
					continue;
				} else {
					//logger.info("BBBB LLJUGB %3d %3d", r0.getArg(), SignExtend((CARD16)r1.getArg()));
				}
			}
			// IOR / JZ n
			if (EQUALS_2(IOR, JZ)) {
				const CARD16 offset = r0.getLength() + r1.getArg();
				const CARD32 len    = LENGTH_2;
				Run newOpcode(rIORJZ, len, offset);
				REPLACE_OPCODE_2(newOpcode)
				continue;
			}
			// LLD m / UDCMP
			if (EQUALS_2(LLD, UDCMP)) {
				const CARD32 len    = LENGTH_2;
				Run newOpcode(rLLDUDCMP, len, r0.getArg());
				REPLACE_OPCODE_2(newOpcode)
				continue;
			}
			// LL m / JGB n
			if (EQUALS_2(LL, JGB)) {
				const CARD16 offset = r0.getLength() + r1.getArg();
				if (r0.getArg() < 256 && offset == (CARD16)SignExtend((CARD8)offset)) {
					const CARD32 len    = LENGTH_2;
					Run newOpcode(rLLJGB, len, (CARD8)r0.getArg(), (CARD8)offset);
					REPLACE_OPCODE_2(newOpcode)
					//logger.info("AAAA LLJGB %3d %3d", r0.getArg(), SignExtend((CARD16)r1.getArg()));
					continue;
				} else {
					//logger.info("BBBB LLJGB %3d %3d", r0.getArg(), SignExtend((CARD16)r1.getArg()));
				}
			}
			// LL m / SHIFTI n
			if (EQUALS_2(LL, SHIFTI)) {
				if (r0.getArg() < 256 && r1.getArg() == (CARD16)SignExtend((CARD8)r1.getArg())) {
					const CARD32 len = LENGTH_2;
					Run newOpcode(rLLSHIFTI, len, (CARD8)r0.getArg(), (CARD8)r1.getArg());
					REPLACE_OPCODE_2(newOpcode)
					continue;
				} else {
					//logger.info("BBBB LLSHIFTI %3d %3d", r0.getArg(), r1.getArg());
				}
			}
			// LL m / JEB n
			if (EQUALS_2(LL, JEB)) {
				const CARD16 offset = r0.getLength() + r1.getArg();
				if (r0.getArg() < 256 && offset == (CARD16)SignExtend((CARD8)offset)) {
					const CARD32 len    = LENGTH_2;
					Run newOpcode(rLLJEB, len, (CARD8)r0.getArg(), (CARD8)offset);
					REPLACE_OPCODE_2(newOpcode)
					//logger.info("AAAA LLJEB %3d %3d", r0.getArg(), SignExtend((CARD16)r1.getArg()));
					continue;
				} else {
					//logger.info("BBBB LLJEB %3d %3d", r0.getArg(), SignExtend((CARD16)r1.getArg()));
				}
			}
			// IOR / JNZ n
			if (EQUALS_2(IOR, JNZ)) {
				const CARD16 offset = r0.getLength() + r1.getArg();
				const CARD32 len    = LENGTH_2;
				Run newOpcode(rIORJNZ, len, offset);
				REPLACE_OPCODE_2(newOpcode)
				continue;
			}
		}
		if (oneMore) continue;
		break;
	}
	// Pass 2
	for(;;) {
		int oneMore = 0;
		for(int i = 0; i < hotSpot->opcodes.size(); i++) {
			const Run& r0 = hotSpot->opcodes.value(i + 0);
			const Run& r1 = hotSpot->opcodes.value(i + 1);
			const Run& r2 = hotSpot->opcodes.value(i + 2);

			// SL m / LL n / LL o
			if (EQUALS_3(SL, LL, LL)) {
//				if (CHECK_3(r0, r1, r2)) {
//					const CARD32 len = LENGTH_3;
//					Run newOpcode(rSLLLLL, len, (CARD8)r0.getArg(), (CARD8)r1.getArg(), (CARD8)r2.getArg());
//					REPLACE_OPCODE_3(newOpcode)
//					continue;
//				} else {
//					//logger.info("BBBB SLLLLL %3d %3d %3d", r0.getArg(), r1.getArg(), r2.getArg());
//				}
			}
			// LL m / LL n
			if (EQUALS_2(LL, LL)) {
				if (CHECK_2(r0, r1)) {
					const CARD32 len = LENGTH_2;
					Run newOpcode(rLLLL, len, (CARD8)r0.getArg(), (CARD8)r1.getArg());
					REPLACE_OPCODE_2(newOpcode)
					continue;
				} else {
					//logger.info("BBBB LLLL %3d %3d", r0.getArg(), r1.getArg());
				}
			}
			// LK m / SL n
			if (EQUALS_2(LK, SL)) {
				if (CHECK_2(r0, r1)) {
					const CARD32 len = LENGTH_2;
					Run newOpcode(rLKSL, len, (CARD8)r0.getArg(), (CARD8)r1.getArg());
					REPLACE_OPCODE_2(newOpcode)
					continue;
				} else {
					//logger.info("BBBB LKSL %3d %3d", r0.getArg(), r1.getArg());
				}
			}
			// LL m / LI n
			if (EQUALS_2(LL, LI)) {
				if (CHECK_2(r0, r1)) {
					const CARD32 len = LENGTH_2;
					Run newOpcode(rLLLI, len, (CARD8)r0.getArg(), (CARD8)r1.getArg());
					REPLACE_OPCODE_2(newOpcode)
					//logger.info("AAAA LLLI %3d %3d", r0.getArg(), r1.getArg());
					continue;
				} else {
					//logger.info("BBBB LLLI %3d %3d", r0.getArg(), r1.getArg());
				}
			}
			// SLD m / SLD n / SLD o
			if (EQUALS_3(SLD, SLD, SLD)) {
				if (CHECK_3(r0, r1, r2)) {
					const CARD32 len = LENGTH_3;
					Run newOpcode(rSLDSLDSLD, len, (CARD8)r0.getArg(), (CARD8)r1.getArg(), (CARD8)r2.getArg());
					REPLACE_OPCODE_3(newOpcode)
					//logger.info("AAAA SLDSLDSLD %3d %3d %3d", r0.getArg(), r1.getArg(), r2.getArg());
					continue;
				} else {
					//logger.info("BBBB SLDSLDSLD %3d %3d %3d", r0.getArg(), r1.getArg(), r2.getArg());
				}
			}
			// SLD m / SLD n
			if (EQUALS_2(SLD, SLD)) {
				if (CHECK_2(r0, r1)) {
					const CARD32 len = LENGTH_2;
					Run newOpcode(rSLDSLD, len, (CARD8)r0.getArg(), (CARD8)r1.getArg());
					REPLACE_OPCODE_2(newOpcode)
					//logger.info("AAAA SLDSLD %3d %3d", r0.getArg(), r1.getArg());
					continue;
				} else {
					//logger.info("BBBB SLDSLD %3d %3d", r0.getArg(), r1.getArg());
				}
			}
			// LI m / J n
			if (EQUALS_2(LI, J)) {
				const CARD16 offset = r0.getLength() + r1.getArg();
				if (r0.getArg() < 256 && offset == (CARD16)SignExtend((CARD8)offset)) {
					const CARD32 len    = LENGTH_2;
					Run newOpcode(rLIJ, len, (CARD8)r0.getArg(), (CARD8)offset);
					REPLACE_OPCODE_2(newOpcode)
					//logger.info("AAAA LIJ %3d %3d", r0.getArg(), SignExtend((CARD8)r1.getArg()));
					continue;
				} else {
					//logger.info("BBBB LIJ %3d %3d", r0.getArg(), SignExtend((CARD8)r1.getArg()));
				}
			}
			// LL m / J n
			if (EQUALS_2(LL, J)) {
				const CARD16 offset = r0.getLength() + r1.getArg();
				if (r0.getArg() < 256 && offset == (CARD16)SignExtend((CARD8)offset)) {
					const CARD32 len    = LENGTH_2;
					Run newOpcode(rLLJ, len, (CARD8)r0.getArg(), (CARD8)offset);
					REPLACE_OPCODE_2(newOpcode)
					//logger.info("AAAA LLJ %3d %3d", r0.getArg(), SignExtend((CARD8)r1.getArg()));
					continue;
				} else {
					//logger.info("BBBB LLJ %3d %3d", r0.getArg(), SignExtend((CARD8)r1.getArg()));
				}
			}
			// LL m / INC
			if (EQUALS_2(LL, INC)) {
				const CARD32 len = LENGTH_2;
				Run newOpcode(rLLINC, len, r0.getArg());
				REPLACE_OPCODE_2(newOpcode)
				//logger.info("AAAA LLINC %3d %3d", r0.getArg());
				continue;
			}
			// LLD m / LLD n
			if (EQUALS_2(LLD, LLD)) {
				if (CHECK_2(r0, r1)) {
					const CARD32 len = LENGTH_2;
					Run newOpcode(rLLDLLD, len, (CARD8)r0.getArg(), (CARD8)r1.getArg());
					REPLACE_OPCODE_2(newOpcode)
					//logger.info("AAAA LLDLLD %3d %3d", r0.getArg(), r1.getArg());
					continue;
				} else {
					//logger.info("BBBB LLDLLD %3d %3d", r0.getArg(), r1.getArg());
				}
			}
			// REC / SL m
			if (EQUALS_2(REC, SL)) {
				const CARD32 len = LENGTH_2;
				Run newOpcode(rRECSL, len, r1.getArg());
				REPLACE_OPCODE_2(newOpcode)
				//logger.info("AAAA LLINC %3d %3d", r0.getArg());
				continue;
			}
			// LLD n / ACD
			if (EQUALS_2(LLD, ACD)) {
				const CARD32 len    = LENGTH_2;
				Run newOpcode(rLLDACD, len, r0.getArg());
				REPLACE_OPCODE_2(newOpcode)
				continue;
			}
			// LLD m / LL n
			if (EQUALS_2(LLD, LL)) {
				if (CHECK_2(r0, r1)) {
					const CARD32 len = LENGTH_2;
					Run newOpcode(rLLDLL, len, (CARD8)r0.getArg(), (CARD8)r1.getArg());
					REPLACE_OPCODE_2(newOpcode)
					//logger.info("AAAA LLDLL %3d %3d", r0.getArg(), r1.getArg());
					continue;
				} else {
					//logger.info("BBBB LLDLL %3d %3d", r0.getArg(), r1.getArg());
				}
			}
			// LLD m / LI n
			if (EQUALS_2(LLD, LI)) {
				if (r0.getArg() < 256 && r1.getArg() == (CARD16)SignExtend((CARD8)r1.getArg())) {
					const CARD32 len = LENGTH_2;
					Run newOpcode(rLLDLI, len, (CARD8)r0.getArg(), (CARD8)r1.getArg());
					REPLACE_OPCODE_2(newOpcode)
					//logger.info("AAAA LLDLI %3d %3d", r0.getArg(), r1.getArg());
					continue;
				} else {
					//logger.info("BBBB LLDLI %3d %3d", r0.getArg(), r1.getArg());
				}
			}
			// PL m / LI n
			if (EQUALS_2(PL, LI)) {
				if (r0.getArg() < 256 && r1.getArg() == (CARD16)SignExtend((CARD8)r1.getArg())) {
					const CARD32 len = LENGTH_2;
					Run newOpcode(rPLLI, len, (CARD8)r0.getArg(), (CARD8)r1.getArg());
					REPLACE_OPCODE_2(newOpcode)
					//logger.info("AAAA PLLI %3d %3d", r0.getArg(), r1.getArg());
					continue;
				} else {
					//logger.info("BBBB PLLI %3d %3d", r0.getArg(), r1.getArg());
				}
			}
			// LA n / LP
			if (EQUALS_2(LA, LP)) {
				const CARD32 len    = LENGTH_2;
				Run newOpcode(rLALP, len, r0.getArg());
				REPLACE_OPCODE_2(newOpcode)
				continue;
			}
			// LL m / WSF 0 n
			if (EQUALS_2(LL, WSF)) {
				if (r0.getArg() < 256 && r1.getArg0() == 0) {
					const CARD32 len = LENGTH_2;
					Run newOpcode(rLLWSF, len, (CARD8)r0.getArg(), (CARD8)r1.getArg1());
					REPLACE_OPCODE_2(newOpcode)
					//logger.info("AAAA LLWSF %3d %3d %3d", r0.getArg(), r1.getArg0(), r1.getArg1());
					continue;
				} else {
					//logger.info("BBBB LLWSF %3d %3d %3d", r0.getArg(), r1.getArg0(), r1.getArg1());
				}
			}
			// LI m / SL n
			if (EQUALS_2(LI, SL)) {
				if (r0.getArg() == (CARD16)SignExtend((CARD8)r0.getArg()) && r1.getArg() < 256) {
					const CARD32 len = LENGTH_2;
					Run newOpcode(rLISL, len, (CARD8)r0.getArg(), (CARD8)r1.getArg());
					REPLACE_OPCODE_2(newOpcode)
					//logger.info("AAAA LISL %3d %3d", r0.getArg(), r1.getArg());
					continue;
				} else {
					//logger.info("BBBB LISL %3d %3d", r0.getArg(), r1.getArg());
				}
			}
			// LA m / LI n
			if (EQUALS_2(LA, LI)) {
				if (r0.getArg() < 256 && r1.getArg() == (CARD16)SignExtend((CARD8)r1.getArg())) {
					const CARD32 len = LENGTH_2;
					Run newOpcode(rLALI, len, (CARD8)r0.getArg(), (CARD8)r1.getArg());
					REPLACE_OPCODE_2(newOpcode)
					//logger.info("AAAA LALI %3d %3d", r0.getArg(), r1.getArg());
					continue;
				} else {
					//logger.info("BBBB LALI %3d %3d", r0.getArg(), r1.getArg());
				}
			}
			// SLD m / LLD n
			if (EQUALS_2(SLD, LLD)) {
				if (CHECK_2(r0, r1)) {
					const CARD32 len = LENGTH_2;
					Run newOpcode(rSLDLLD, len, (CARD8)r0.getArg(), (CARD8)r1.getArg());
					REPLACE_OPCODE_2(newOpcode)
					//logger.info("AAAA SLDLLD %3d %3d", r0.getArg(), r1.getArg());
					continue;
				} else {
					//logger.info("BBBB SLDLLD %3d %3d", r0.getArg(), r1.getArg());
				}
			}
			// SL m / LL n
			if (EQUALS_2(SL, LL)) {
				if (CHECK_2(r0, r1)) {
					const CARD32 len = LENGTH_2;
					Run newOpcode(rSLLL, len, (CARD8)r0.getArg(), (CARD8)r1.getArg());
					REPLACE_OPCODE_2(newOpcode)
					//logger.info("AAAA SLLL %3d %3d", r0.getArg(), r1.getArg());
					continue;
				} else {
					//logger.info("BBBB SLLL %3d %3d", r0.getArg(), r1.getArg());
				}
			}
		}
		if (oneMore) continue;
		break;
	}
}
