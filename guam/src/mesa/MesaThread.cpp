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
// MesaThread.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("mesathread");

#include "../util/Debug.h"

#include "../agent/AgentNetwork.h"
#include "../agent/AgentDisk.h"

#include "../simple-opcode/Interpreter.h"

#include "MesaThread.h"
#include "Type.h"
#include "Variable.h"
#include "Function.h"
#include "Memory.h"

#include "LoadState.h"


//
// ProcessorThread
//
QWaitCondition ProcessorThread::cvRunning;
QAtomicInt     ProcessorThread::running;
int            ProcessorThread::stopThread;
int            ProcessorThread::rescheduleRequestCount;
QAtomicInt     ProcessorThread::requestReschedule;
QMutex         ProcessorThread::mutexRequestReschedule;

static int startRunningCount = 0;
static int stopRunningCount = 0;
static int timerCount = 0;
static int interruptCount = 0;
static int notifyCount = 0;
static int notifyWakeupCount = 0;

void ProcessorThread::startRunning() {
	if (running.testAndSetOrdered(0, 1)) {
		startRunningCount++;
		return;
	}
	logger.error("startRunning testAndSetOrdered returns false  running = %d", getRunning());
}
void ProcessorThread::stopRunning() {
	if (running.testAndSetOrdered(1, 0)) {
		stopRunningCount++;
		return;
	}
	logger.error("stopRunning testAndSetOrdered returns false  running = %d", getRunning());
}
void ProcessorThread::stop() {
	logger.info("ProcessorThread::stop");
	stopThread = 1;
}
void ProcessorThread::run() {
	logger.info("ProcessorThread::run START");
	QThread::currentThread()->setPriority(PRIORITY);
	TaggedControlLink bootLink = {SD_OFFSET(sBoot)};

	logger.info("bootLink  %04X %d %04X  %08X", bootLink.data, bootLink.tag, bootLink.fill, bootLink.u);
	if (DEBUG_STOP_MESSAGE_UNTIL_MP) Logger::pushPriority(QtFatalMsg);

	XFER(bootLink.u, 0, XT_call, 0);
	logger.info("GFI = %04X  CB  = %08X  GF = %08X", GFI, CodeCache::CB(), GF);
	logger.info("PC  = %04X  MDS = %08X  LF = %04X", PC, Memory::MDS(), LFCache::LF());

	int abortCount = 0;
	int rescheduleCount = 0;
	stopThread = 0;
	try {
		for(;;) {
			try {
				if (DEBUG_STOP_AT_NOT_RUNNING) {
					if (!getRunning()) ERROR();
				}
				Interpreter::execute();
			} catch(RequestReschedule& e) {
				rescheduleCount++;
				//logger.debug("Reschedule %-20s  %8d", e.func, rescheduleCount);
				QMutexLocker locker(&mutexRequestReschedule);
				for(;;) {
					// break if OP_STOPEMULATOR is called
					if (stopThread) goto exitLoop;

					scanLoadState();

					// If not running, wait someone wake me up.
					if (!getRunning()) {
						//logger.debug("waitRunning START");
						for(;;) {
							bool ret = cvRunning.wait(&mutexRequestReschedule, WAIT_INTERVAL);
							if (ret) break;
							if (stopThread) goto exitLoop;
							//logger.debug("waitRunning WAITING");
						}
						//logger.debug("waitRunning FINISH");
					}
					// Do reschedule.
					{
						//logger.debug("reschedule START");
						// to avoid race condition of update of rescheduleFlag, guard with mutexReschedule
						int needReschedule = 0;
						const int request = getRequestReschedule();
						if (request & REQUSEST_RESCHEDULE_INTERRUPT) {
							//logger.debug("reschedule INTERRUPT");
							// process interrupt
							if (ProcessInterrupt()) needReschedule = 1;
						}
						if (request & REQUESET_RESCHEDULE_TIMER) {
							//logger.debug("reschedule TIMER");
							// process timeout
							if (TimerThread::processTimeout()) needReschedule = 1;
						}
						if (needReschedule) Reschedule(1);
						setRequestReschedule(0);
						//logger.debug("reschedule FINISH");
					}
					// It still not running, continue loop again
					if (!getRunning()) continue;
					break;
				}
			} catch(Abort& e) {
				abortCount++;
				//logger.debug("Abort %-20s  %8d", e.func, abortCount);
			}
		}
	} catch (Error& e) {
		logger.fatal("Error %-20s %4d %s", e.func, e.line, e.file);
		// Output for postmortem  examination
		logger.fatal("GFI %4X  CB %8X  PC %4X", GFI, CodeCache::CB(), PC);
	}

exitLoop:
	// stop relevant thread
	AgentNetwork::ReceiveThread::stop();
	AgentNetwork::TransmitThread::stop();
	AgentDisk::IOThread::stop();
	TimerThread::stop();
	InterruptThread::stop();

	logger.info("abortCount             = %8u", abortCount);
	logger.info("rescheduleCount        = %8u", rescheduleCount);
	logger.info("rescheduleRequestCount = %8u", rescheduleRequestCount);
	logger.info("timerCount             = %8u", timerCount);
	logger.info("interruptCount         = %8u", interruptCount);
	logger.info("notifyCount            = %8u", notifyCount);
	logger.info("notifyWakeupCount      = %8u", notifyWakeupCount);
	logger.info("startRunningCount      = %8u", startRunningCount);
	logger.info("stopRunningCount       = %8u", stopRunningCount);
	logger.info("ProcessorThread::run STOP");
}
void ProcessorThread::requestRescheduleTimer() {
	QMutexLocker locker(&mutexRequestReschedule);
	setRequestReschedule(getRequestReschedule() | REQUESET_RESCHEDULE_TIMER);
	if (!getRunning()) cvRunning.wakeOne();
}
void ProcessorThread::requestRescheduleInterrupt() {
	QMutexLocker locker(&mutexRequestReschedule);
	setRequestReschedule(getRequestReschedule() | REQUSEST_RESCHEDULE_INTERRUPT);
	if (!getRunning()) cvRunning.wakeOne();
}


//
// TimerThread
//
CARD16         TimerThread::PTC;
qint64         TimerThread::lastTimeoutTime;
int            TimerThread::stopThread;
QMutex         TimerThread::mutexTimer;
QWaitCondition TimerThread::cvTimer;

void TimerThread::stop() {
	logger.info("TimerThread::stop");
	stopThread = 1;
}
void TimerThread::setPTC(CARD16 newValue) {
	PTC = newValue;
	lastTimeoutTime = QDateTime::currentMSecsSinceEpoch();
}
void TimerThread::run() {
	logger.info("TimerThread::run START");
	QThread::currentThread()->setPriority(PRIORITY);

	lastTimeoutTime = QDateTime::currentMSecsSinceEpoch();
	stopThread = 0;
	QMutexLocker locker(&mutexTimer);
	for (;;) {
		if (stopThread) break;
		timerCount++;
		qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
		// I will wait until TIMER_INTERVAL is elapsed since preveiousTime.
		qint64 waitTime = lastTimeoutTime + TIMER_INTERVAL - currentTime;
		if (waitTime < 0) {
			// already elapsed
			if (-waitTime < TIMER_INTERVAL) {
				// Time difference is not significant. Hope it will recover.
			} else {
				// Time difference is significant. Assign value to lastTimeoutTime.
				lastTimeoutTime = currentTime;
				logger.warn("Timer lost.  lost = %d ms",(quint32) (-waitTime));
			}
		} else {
			// need to wait until TIMER_INTERVAL is elapsed since preveiousTime.
			Util::msleep((quint32) waitTime);
		}

		{
			ProcessorThread::requestRescheduleTimer();
			// wait procesTimeout is invoked
			for(;;) {
				bool ret = cvTimer.wait(&mutexTimer, WAIT_INTERVAL);
				if (ret) break;
				if (stopThread) goto exitLoop;
			}
		}
	}
exitLoop:
	logger.info("TimerThread::run STOP");
}

int TimerThread::processTimeout() {
	QMutexLocker locker(&mutexTimer);
	//logger.debug("processTimeout START");
	// Don't update lastTimeoutTimer, until actual process is performed
	lastTimeoutTime += TIMER_INTERVAL;

	PTC = PTC + 1;
	if (PTC == 0) PTC = PTC + 1;

	int ret = TimeoutScan();
	cvTimer.wakeOne();
	//logger.debug("processTimeout FINISH");
	return ret;
}


//
// InterruptThread
//
CARD16         InterruptThread::WP;
CARD16         InterruptThread::WDC;
QMutex         InterruptThread::mutexWP;
QWaitCondition InterruptThread::cvWP;
int            InterruptThread::stopThread;

void InterruptThread::stop() {
	logger.info("InterruptThread::stop");
	stopThread = 1;
}
CARD16 InterruptThread::getWP() {
	return WP;
}
void InterruptThread::setWP(CARD16 newValue) {
	QMutexLocker locker(&mutexWP);
	CARD16 oldValue = WP;
	WP = newValue;
	if (oldValue && !newValue) {
		// become no interrupt
	} else if (!oldValue && newValue) {
		// start interrupt, wake waiting thread
		cvWP.wakeOne();
	}
}
void InterruptThread::notifyInterrupt(CARD16 interruptSelector) {
	QMutexLocker locker(&mutexWP);
	notifyCount++;
	CARD16 newValue = (WP | interruptSelector);
	//
	CARD16 oldValue = WP;
	WP = newValue;
	if (oldValue && !newValue) {
		// become no interrupt
	} else if (!oldValue && newValue) {
		// start interrupt, wake waiting thread
		cvWP.wakeOne();
		notifyWakeupCount++;
	}
}
//int InterruptThread::isPending() {
//	return WP && isEnabled();
//}

void InterruptThread::run() {
	logger.info("InterruptThread::run START");
	QThread::currentThread()->setPriority(PRIORITY);

	stopThread = 0;
	QMutexLocker locker(&mutexWP);
	for (;;) {
		if (stopThread) break;
		interruptCount++;

		// wait until interrupt is arrived
		for(;;) {
			bool ret = cvWP.wait(&mutexWP, WAIT_INTERVAL);
			if (ret) break;
			if (stopThread) goto exitLoop;
		}

		ProcessorThread::requestRescheduleInterrupt();
	}
exitLoop:
	logger.info("InterruptThread::run STOP");
}
