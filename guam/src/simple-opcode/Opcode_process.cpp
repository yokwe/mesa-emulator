/*
Copyright (c) 2014, 2017, Yasuhiro Hasegawa
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
// Opcode_process.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("process");

#include "../util/Debug.h"
#include "../util/Perf.h"

#include "../mesa/Constant.h"
#include "../mesa/Type.h"
#include "../mesa/Memory.h"
#include "../mesa/Function.h"
#include "../mesa/MesaThread.h"

#include "Opcode.h"

#define OFFSET_PDA(m)      OFFSET(ProcessDataArea, m)
#define OFFSET_PDA2(m,n)   OFFSET3(ProcessDataArea, m, n)
#define OFFSET_PDA3(m,n,o) OFFSET4(ProcessDataArea, m, n, o)

#define OFFSET_SV(m) OFFSET(StateVector, m)
#define MAX(a,b) (((a) < (b)) ? (b) : (a))

//#define TRACE_RESCHEDULE

// 10.3.1 Queuing Procedures

// Dequeue: PROC[src: LONG POINTER, ps: PsbIndex]
static void Dequeue(LONG_POINTER src, PsbIndex psb) {
	PsbIndex prev;

	QueueHandle que = src;
	Queue queue = {que ? *Fetch(que) : (CARD16)0};
	PsbLink link = {*FetchPda(OFFSET_PDA3(block, psb, link))};
	if (link.next == psb) {
		prev = PsbNull;
	} else {
		PsbLink temp;
		prev = (que == 0) ? psb : queue.tail;
		for(;;) {
			temp.u = *FetchPda(OFFSET_PDA3(block, prev, link));
			if (temp.next == psb) break;
			prev = temp.next;
		}
		temp.next = link.next;
		*StorePda(OFFSET_PDA3(block, prev, link)) = temp.u;
	}
	if (que == 0) {
		PsbFlags flags = {*FetchPda(OFFSET_PDA3(block, psb, flags))};
		flags.cleanup = link.next;
		*StorePda(OFFSET_PDA3(block, psb, flags)) = flags.u;
	} else if (queue.tail == psb) {
		queue.tail = prev;
		*Store(que) = queue.u;
	}
}

// Equeue: PROC[dst: LONG POINTER, psb: PsbIndex]
static void Enqueue(LONG_POINTER dst, PsbIndex psb) {
	QueueHandle que = dst;
	Queue queue = {*Fetch(que)};
	PsbLink link = {*FetchPda(OFFSET_PDA3(block, psb, link))};
	if (queue.tail == PsbNull) {
		link.next = psb;
		*StorePda(OFFSET_PDA3(block, psb, link)) = link.u;
		queue.tail = psb;
		*Store(que) = queue.u;
	} else {
		PsbIndex prev = queue.tail;
		PsbLink currentlink = {*FetchPda(OFFSET_PDA3(block, prev, link))};
		if (link.priority <= currentlink.priority) {
			queue.tail = psb;
			*Store(que) = queue.u;
		} else {
			for(;;) {
				PsbLink nextlink = {*FetchPda(OFFSET_PDA3(block, currentlink.next, link))};
				if (nextlink.priority < link.priority) break;
				prev = currentlink.next;
				currentlink = nextlink;
			}
		}
		link.next = currentlink.next;
		*StorePda(OFFSET_PDA3(block, psb, link)) = link.u;
		currentlink.next = psb;
		*StorePda(OFFSET_PDA3(block, prev, link)) = currentlink.u;
	}
}

// Requeue: PROC[src, dst: LONG POINTER, psb: PsbIndex]
static void Requeue(LONG_POINTER src, LONG_POINTER dst, PsbIndex psb) {
	if (psb == PsbNull) ERROR();
	Dequeue(src, psb);
	Enqueue(dst, psb);
}

// 10.4.2.2 State Vector Allocation
// EmptyState: PROC[pri: Priority] RETURNS[BOOLEAN]
int EmptyState(CARD16 pri) {
	PDA_POINTER state = *FetchPda(OFFSET_PDA2(state, pri));
	return state == 0;
}

// AllocState: PROC[pri: Priority] RETURNS[state: StateHandle]
static LONG_POINTER AllocState(CARD16 pri) {
	PDA_POINTER offset = *FetchPda(OFFSET_PDA2(state, pri));
	if (offset == 0) ERROR();
	StateHandle state = LengthenPdaPtr(offset);
	*StorePda(OFFSET_PDA2(state, pri)) = *Fetch(state);
	return state;
}

// FreeState: PROC[pri: Priority, state: StateHandle]
static void FreeState(CARD16 pri, StateHandle state) {
	*Store(state) = *FetchPda(OFFSET_PDA2(state, pri));
	*StorePda(OFFSET_PDA2(state, pri)) = OffsetPda(state);
}


// 10.4.2.1 Saving and Loading Process State

// SaveProcess: PROC[preemption BOOLEAN]
void SaveProcess(int preemption) {
	try {
		PsbLink link = {*FetchPda(OFFSET_PDA3(block, PSB, link))};
		if (ValidContext()) *StoreLF(LO_OFFSET(0, pc)) = PC;
		link.preempted = preemption;
		if (preemption) {
			StateHandle state;
			if (!link.permanent) state = AllocState(link.priority);
			else state = LengthenPdaPtr(*FetchPda(OFFSET_PDA3(block, PSB, context)));
			SaveStack(state);
//			*Store(state + OFFSET(StateVector, frame)) = LF;
			*Store(state + OFFSET_SV(frame)) = LFCache::LF();
			if (!link.permanent) *StorePda(OFFSET_PDA3(block, PSB, context)) = OffsetPda(state);
		} else {
			if (!link.permanent) *StorePda(OFFSET_PDA3(block, PSB, context)) = LFCache::LF();
			else {
				StateHandle state = LengthenPdaPtr(*FetchPda(OFFSET_PDA3(block, PSB, context)));
				*Store(state + OFFSET_SV(frame)) = LFCache::LF();
			}
		}
		*StorePda(OFFSET_PDA3(block, PSB, link)) = link.u;
	} catch (Abort &abort) {
		ERROR();
	}
}

// LoadProcess: PROC RETURNS[frame: LocalFrameHandle]
LocalFrameHandle LoadProcess() {
	try {
		PsbLink link = {*FetchPda(OFFSET_PDA3(block, PSB, link))};
		LocalFrameHandle frame = *FetchPda(OFFSET_PDA3(block, PSB, context));
		if (link.preempted) {
			StateHandle state = LengthenPdaPtr(frame);
			LoadStack(state);
			frame = *Fetch(state + OFFSET_SV(frame));
			if (!link.permanent) FreeState(link.priority, state);
		} else {
			if (link.failed) {
				Push(0);
				link.failed = 0;
				*StorePda(OFFSET_PDA3(block, PSB, link)) = link.u;
			}
			if (link.permanent) {
				StateHandle state = LengthenPdaPtr(frame);
				frame = *Fetch(state + OFFSET_SV(frame));
			}
		}
		CARD32 mds = *FetchPda(OFFSET_PDA3(block, PSB, mds)) << WordSize;
		Memory::setMDS(mds);
		return frame;
	} catch (Abort &abort) {
		ERROR();
		return 0;
	}
}

//// 10.4.1 Scheduler
//// Reschedule: PROC[preemption: BOOLEAN = FALSE]
void Reschedule(int preemption) {
	PsbIndex psb;
	PsbLink link;

#ifdef  TRACE_RESCHEDULE
	CARD16 oldPSB = PSB;
	CARD16 oldGFI = GFI;
	CARD16 oldPC  = savedPC;
	CARD16 oldLF  = LFCache::LF();
#endif

	if (ProcessorThread::getRunning()) SaveProcess(preemption);
	Queue queue = {*FetchPda(OFFSET_PDA(ready))};
	if (queue.tail == PsbNull) goto BusyWait;
	link.u = *FetchPda(OFFSET_PDA3(block, queue.tail, link));
	for(;;) {
		psb = link.next;
		link.u = *FetchPda(OFFSET_PDA3(block, psb, link));
		if (link.permanent || link.preempted || !EmptyState(link.priority)) break;
		if (psb == queue.tail) goto BusyWait;
	}
	PSB = psb;
	PC = savedPC = 0;
	LFCache::setLF(LoadProcess());
	if (!ProcessorThread::getRunning()) {
		if (DEBUG_SHOW_RUNNING) logger.debug("start running");
		ProcessorThread::startRunning();
	}

#ifdef  TRACE_RESCHEDULE
	logger.debug("RESCHED  %04X %04X  %04X+%04X-%04X  %04X+%04X-%04X", oldPSB, PSB, oldGFI, oldPC, oldLF, GFI, PC, LFCache::LF());
#endif

//	running = 1;
//	ProcessorThread::startRunning();
	//logger.debug("%s XFER PSB = %4d  GFI = %04X  CB = %08X  PC = %04X  LF = %04X", __FUNCTION__, PSB, GFI, CB, PC, LF);
	XFER((CARD32)LFCache::LF(), 0, XT_processSwitch, 0);
	return;
BusyWait:
	if (!InterruptThread::isEnabled()) RescheduleError();
	if (ProcessorThread::getRunning()) {
		if (DEBUG_SHOW_RUNNING) logger.debug("stop  running");
		ProcessorThread::stopRunning();
		ERROR_RequestReschedule();
	}
//	running = 0;
//	ProcessorThread::stopRunning();
}


// 10.2.1 Monitor Entry

// EnerFailed: PROC[m: LONG POINTER TO Monitor]
// EnterFailed calls Reschedule at it's end.
// Also EnterFailed is called at very end of opcode implementation of ME and MR
// So even if RequestReschedule is thrown within Reschedule, it is fine.
static void EnterFailed(LONG_POINTER m) {
	PsbLink link = {*FetchPda(OFFSET_PDA3(block, PSB, link))};
	link.failed = 1;
	*StorePda(OFFSET_PDA3(block, PSB, link)) = link.u;
	Requeue(PDA + OFFSET_PDA(ready), m, PSB);
	Reschedule();
}

// 10.2.2 Monitor Exit

// Exit: PROC[m: LONG POINTER TO Monitor] RETURNS[requeue: BOOLEAN]
static int Exit(LONG_POINTER m) {
	Monitor mon = {*Fetch(m)};
	if (!mon.locked) ERROR();
	mon.locked = 0;
	*Store(m) = mon.u;
	int requeue = (mon.tail != PsbNull);
	if (requeue) {
		PsbLink link = {*FetchPda(OFFSET_PDA3(block, mon.tail, link))};
		Requeue(m, PDA + OFFSET_PDA(ready), link.next);
	}
	return requeue;
}

// 10.2.5 Notify and Broadcast

// WakeHead: PROC[c: LONG POINTER TO Condition]
static void WakeHead(LONG_POINTER c) {
	Condition cond = {*Fetch(c)};
	PsbLink link = {*FetchPda(OFFSET_PDA3(block, cond.tail, link))};
	PsbFlags flags = {*FetchPda(OFFSET_PDA3(block, link.next, flags))};
	flags.waiting = 0;
	*StorePda(OFFSET_PDA3(block, link.next, flags)) = flags.u;
	*StorePda(OFFSET_PDA3(block, link.next, timeout)) = 0;
	Requeue(c, PDA + OFFSET_PDA(ready), link.next);
}

// 10.3.2 Cleanup Links

// CleanupCondition: PROC[c: LONG POINTER TO Condition]
static void CleanupCondition(LONG_POINTER c) {
	Condition cond = {*Fetch(c)};
	PsbIndex psb = cond.tail;

	if (psb != PsbNull) {
		PsbFlags flags = {*FetchPda(OFFSET_PDA3(block, psb, flags))};
		if (flags.cleanup != PsbNull) {
			for(;;) {
				if (flags.cleanup == psb) {
					cond.wakeup = 0;
					cond.tail = PsbNull;
					*Store(c) = cond.u;
					return;
				}
				psb = flags.cleanup;
				flags.u = *FetchPda(OFFSET_PDA3(block, psb, flags));
				if (flags.cleanup == PsbNull) break;
			}
			PsbIndex head = psb;
			for(;;) {
				PsbLink link = {*FetchPda(OFFSET_PDA3(block, psb, link))};
				if (link.next == head) break;
				psb = link.next;
			}
			cond.tail = psb;
			*Store(c) = cond.u;
		}
	}
}


// 10.4.4.2 Interrupt Processing

// NotifyWakeup: PROC[c: LONG POINTER TO Condition] RETURNS[BOOLEAN]
static int NotifyWakeup(LONG_POINTER c) {
	int requeue = 0;
	CleanupCondition(c);
	Condition cond = {*Fetch(c)};
	if (cond.tail == PsbNull) {
		cond.wakeup = 1;
		*Store(c) = cond.u;
	} else {
		WakeHead(c);
		requeue = 1;
	}
	return requeue;
}

// Interrupt: PROC RETURNS [BOOLEAN]
int ProcessInterrupt() {
	UNSPEC mask = 1;
	int requeue = 0;
	UNSPEC wakeups = InterruptThread::getWP();
	InterruptThread::setWP(0);
	for(int level = InterruptLevel_SIZE - 1; 0 <= level; level--) {
		if (wakeups & mask) requeue = NotifyWakeup(PDA + OFFSET_PDA3(interrupt, level, condition)) || requeue;
		mask = Shift(mask, 1);
	}
	return requeue;
}

// CheckForInterrupts: PROC RETURNS[BOOLEAN]
//int CheckForInterrupt() {
//	int ret = InterruptThread::isPending() ? ProcessInterrupt() : 0;
//	return ret;
//}

// 10.4.3 Faults
// Fault: PROC[fi: FaultIndex] RETURNS[PsbIndex]
static inline PsbIndex Fault(FaultIndex fi) {
	PsbIndex faulted = PSB;
	Requeue(PDA + OFFSET_PDA(ready), PDA + OFFSET_PDA3(fault, fi, queue), faulted);
	NotifyWakeup(PDA + OFFSET_PDA3(fault, fi, condition));
	PC = savedPC;
	SP = savedSP;
	try {
		Reschedule(1);
	} catch (RequestReschedule& e) {
		ERROR();
	}
	return faulted;
}
// FaultOne: PROC[fi: FaultIndex, parameter: UNSPEC]
static void FaultOne(FaultIndex fi, UNSPEC parameter) {
	PsbIndex psb = Fault(fi);
	POINTER state = *FetchPda(OFFSET_PDA3(block, psb, context));
	*StorePda(state + OFFSET_SV(data[0])) = parameter;
	ERROR_Abort();
}
// FaultTwo: PROC[fi: FaultIndex, parameter: LONG UNSPEC]
static void FaultTwo(FaultIndex fi, LONG_UNSPEC parameter) {
	PsbIndex psb = Fault(fi);
	POINTER state = *FetchPda(OFFSET_PDA3(block, psb, context));
	*StorePda(state + OFFSET_SV(data[0])) = LowHalf(parameter);
	*StorePda(state + OFFSET_SV(data[1])) = HighHalf(parameter);
	ERROR_Abort();
}

// FrameFault: PROC[fsi: FSIndex]
void FrameFault(FSIndex fsi) {
	if (PERF_ENABLE) perf_FrameFault++;
	if (DEBUG_SHOW_FRAME_FAULT) {
		if (Opcode::getLast()) {
			logger.debug("%-10s %8d  %-8s  %8X+%4X  %8X", __FUNCTION__, fsi, Opcode::getLast()->getName(), CodeCache::CB(), savedPC, (CodeCache::CB() + savedPC));
		} else {
			logger.debug("%-10s %8d", __FUNCTION__, fsi);
		}
	}
	FaultOne(qFrameFault, fsi);
}

// PageFault: PROC[ptr: LONG POINTER]
void PageFault(LONG_POINTER ptr) {
	if (PERF_ENABLE) perf_PageFault++;
	if (DEBUG_SHOW_PAGE_FAULT) {
		if (Opcode::getLast()) {
			logger.debug("%-10s %08X  %-8s  %8X+%4X  %8X", __FUNCTION__, ptr, Opcode::getLast()->getName(), CodeCache::CB(), savedPC, (CodeCache::CB() + savedPC));
		} else {
			logger.debug("%-10s %08X", __FUNCTION__, ptr);
		}
	}
	if (DEBUG_STOP_AT_PAGE_FAULT) ERROR();
	FaultTwo(qPageFault, ptr);
}

// WriteProtectFault: PROC[ptr: LONG POINTER]
void WriteProtectFault(LONG_POINTER ptr) {
	if (DEBUG_SHOW_WRITE_PROTECT_FAULT) logger.debug("%s %08X", __FUNCTION__, ptr);
	FaultTwo(qWriteProtectFault, ptr);
}

// 10.4.5 Timeouts

// TimeoutScan: PROC RETURNS [BOOLEAN]
int TimeoutScan() {
	int requeue = 0;
	CARDINAL count = *FetchPda(OFFSET_PDA(count));
	for(PsbIndex psb = StartPsb; psb < (StartPsb + count); psb++) {
		Ticks timeout = *FetchPda(OFFSET_PDA3(block, psb, timeout));
		if (timeout && timeout == TimerThread::getPTC()) {
			PsbFlags flags = {*FetchPda(OFFSET_PDA3(block, psb, flags))};
			flags.waiting = 0;
			*StorePda(OFFSET_PDA3(block, psb, flags)) = flags.u;
			*StorePda(OFFSET_PDA3(block, psb, timeout)) = 0;
			Requeue(0, PDA + OFFSET_PDA(ready), psb);
			requeue = 1;
		}
	}
	return requeue;
}
// CheckForTimeouts: PROC RETURNS [BOOLEAN]
//int CheckForTimeouts() {
//	int ret = 0;
//	CARD32 now = Util::getMicroTime();
//	if (InterruptThread::isEnabled() && (lastTimeoutTime + TimeOutInterval) <= now) {
//		lastTimeoutTime += TimeOutInterval;
//		if ((lastTimeoutTime + TimeOutInterval) <= now) {
//			logger.error("%s timeout lost  %u", __FUNCTION__, now - lastTimeoutTime);
//			lastTimeoutTime = now;
//		}
//		PTC = PTC + 1;
//		if (PTC == 0) PTC = PTC + 1;
//		ret = TimeoutScan();
//	}
//	return ret;
//}

///////////////////////////////////////////////////////////////////////

// zME - 0361
void E_ME() {
	LONG_POINTER m = PopLong();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  ME  %08X", savedPC, m);
	MINIMAL_STACK();
	Monitor mon = {*Fetch(m)};
	if (!mon.locked) {
		mon.locked = 1;
		*Store(m) = mon.u;
		Push(1);
	} else {
		EnterFailed(m);
	}
}


// zMX - 0362
void E_MX() {
	LONG_POINTER m = PopLong();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  MX  %08X", savedPC, m);
	MINIMAL_STACK();
	// Reschedule must be placed at very end of implementation of opcode.
	if (Exit(m)) Reschedule();
}


// aMW - 02
void E_MW() {
	Ticks t = Pop();
	LONG_POINTER c = PopLong();
	LONG_POINTER m = PopLong();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  MW  %08X  %08X  %04X", savedPC, m, c, t);
	MINIMAL_STACK();
	CleanupCondition(c);
	int requeue = Exit(m);
	PsbFlags flags = {*FetchPda(OFFSET_PDA3(block, PSB, flags))};
	Condition cond = {*Fetch(c)};
	if ((!flags.abort) || (!cond.abortable)) {
		if (cond.wakeup) {
			cond.wakeup = 0;
			*Store(c) = cond.u;
		} else {
//			*StorePda(OFFSET_PDA3(block, PSB, timeout)) = ((t == 0) ? 0 : MAX(1U, (CARD16)((CARD32)PTC + (CARD32)t)));
			*StorePda(OFFSET_PDA3(block, PSB, timeout)) = ((t == 0) ? 0 : MAX(1U, (CARD16)((CARD32)TimerThread::getPTC() + (CARD32)t)));
			flags.waiting = 1;
			*StorePda(OFFSET_PDA3(block, PSB, flags)) = flags.u;
			Requeue(PDA + OFFSET_PDA(ready), c, PSB);
			requeue = 1;
		}
	}
	// Reschedule must be placed at very end of implementation of opcode.
	if (requeue) Reschedule();
}

// aMR - 03
void E_MR() {
	LONG_POINTER c = PopLong();
	LONG_POINTER m = PopLong();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  MR  %08X  %08X", savedPC, m, c);
	MINIMAL_STACK();
	Monitor mon = {*Fetch(m)};
	if (!mon.locked) {
		CleanupCondition(c);
		PsbFlags flags = {*FetchPda(OFFSET_PDA3(block, PSB, flags))};
		flags.cleanup = PsbNull;
		*StorePda(OFFSET_PDA3(block, PSB, flags)) = flags.u;
		if (flags.abort) {
			Condition cond = {*Fetch(c)};
			if (cond.abortable) ProcessTrap();
		}
		mon.locked = 1;
		*Store(m) = mon.u;
		Push(1);
	} else {
		EnterFailed(m);
	}
}


// aNC - 04
void E_NC() {
	LONG_POINTER c = PopLong();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  NC  %08X", savedPC, c);
	MINIMAL_STACK();
	CleanupCondition(c);
	Condition cond = {*Fetch(c)};
	if (cond.tail != PsbNull) {
		WakeHead(c);
		// Reschedule must be placed at very end of implementation of opcode.
		Reschedule();
	}
}

// aBC - 05
void E_BC() {
	LONG_POINTER c = PopLong();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  BC  %08X", savedPC, c);
	MINIMAL_STACK();
	int requeue = 0;
	CleanupCondition(c);
	for(Condition cond = {*Fetch(c)}; cond.tail != PsbNull; cond.u = *Fetch(c)) {
		WakeHead(c);
		requeue = 1;
	}
	// Reschedule must be placed at very end of implementation of opcode.
	if (requeue) Reschedule();
}
// aREQ - 06
void E_REQ() {
	PsbHandle psb = Pop();
	QueueHandle dstque = PopLong();
	QueueHandle srcque = PopLong();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  REQ  %08X  %08X  %04X", savedPC, srcque, dstque, psb);
	MINIMAL_STACK();
	Requeue(srcque, dstque, Index(psb));
	// Reschedule must be placed at very end of implementation of opcode.
	Reschedule();
}
// aSPP - 017
void E_SPP() {
	CARD16 priority = Pop();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  SPP  %d", savedPC, priority);
	MINIMAL_STACK();
	PsbLink link = {*FetchPda(OFFSET_PDA3(block, PSB, link))};
	link.priority = priority;
	*StorePda(OFFSET_PDA3(block, PSB, link)) = link.u;
	Requeue(PDA + OFFSET_PDA(ready), PDA + OFFSET_PDA(ready), PSB);
	// Reschedule must be placed at very end of implementation of opcode.
	Reschedule();
}
