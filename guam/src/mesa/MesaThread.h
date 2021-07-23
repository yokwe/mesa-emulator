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
// MesaThread.h
//

#ifndef MESA_THREAD_H__
#define MESA_THREAD_H__

#include "MesaBasic.h"
#include "Constant.h"

#include "../util/Util.h"

#include <QtCore>

class TimerThread : public QRunnable {
public:
	static const QThread::Priority PRIORITY = QThread::NormalPriority;

	// Wait interval in milliseconds for QWaitCondition::wait
	static const int WAIT_INTERVAL = 1000;

	// Timer interval in milliseconds
	static const int TIMER_INTERVAL = cTick;

	static CARD16 getPTC() {
		return PTC;
	}
	static void   setPTC(CARD16 newValue);
	static void   stop();

	// To process timeout in other thread, create processTimeout
	static int    processTimeout();

	void run();
private:
	static CARD16         PTC;
	static qint64         lastTimeoutTime;
	static int            stopThread;
	static QMutex         mutexTimer;
	static QWaitCondition cvTimer;
};


class InterruptThread : public QRunnable {
public:
	static const QThread::Priority PRIORITY = QThread::NormalPriority;

	// Wait interval in milliseconds for QWaitCondition::wait
	static const int WAIT_INTERVAL = 1000;

	static CARD16 getWP();
	static void   setWP(CARD16 newValue);

	// WDC is changed from processor thread only, so there is no race condition.
	static inline CARD16 getWDC() {
		return WDC;
	}
	static inline void   setWDC(CARD16 newValue) {
		WDC = newValue;
	}
	static inline void enable() {
		setWDC(WDC - 1);
	}
	static inline void disable() {
		setWDC(WDC + 1);
	}
	static inline int isEnabled() {
		return WDC == 0;
	}
//	static int  isPending();
	static void notifyInterrupt(CARD16 interruptSelector);
	static void stop();

	void run();
private:
	static CARD16         WP;
	static CARD16         WDC;
	static QMutex         mutexWP;
	static QWaitCondition cvWP;
	static int            stopThread;
};


class ProcessorThread : public QRunnable {
public:
	static const QThread::Priority PRIORITY = QThread::LowPriority;

	// Wait interval in milliseconds for QWaitCondition::wait
	static const int WAIT_INTERVAL = 1000;

	// Use this condition variable to wake and wait processor
	static QWaitCondition cvRunning;

	static int getRunning() {
#if (QT_VERSION_CHECK(5, 0, 0) <= QT_VERSION)
		return running.loadAcquire();
#else
		return (int)running;
#endif
	}
	static void startRunning(); // running = 1
	static void stopRunning();  // running = 0

	static void stop();

	static void requestRescheduleTimer();
	static void requestRescheduleInterrupt();

	static int getRequestReschedule() {
#if (QT_VERSION_CHECK(5, 0, 0) <= QT_VERSION)
		return requestReschedule.loadAcquire();
#else
		return (int)requestReschedule;
#endif
	}

	static void checkRequestReschedule() {
		if (InterruptThread::isEnabled() && getRequestReschedule()) {
			rescheduleRequestCount++;
			ERROR_RequestReschedule();
		}
		// If stopThread is true, throw RequestReschedule
		if (stopThread) {
			ERROR_RequestReschedule();
		}
	}

	void run();

private:
	static QAtomicInt running;
	static int        stopThread;
	static int        rescheduleRequestCount;
	//
	static const int  REQUESET_RESCHEDULE_TIMER     = 0x01;
	static const int  REQUSEST_RESCHEDULE_INTERRUPT = 0x02;
	static QAtomicInt requestReschedule;
	static QMutex     mutexRequestReschedule;

	static void setRequestReschedule(int newValue) {
#if (QT_VERSION_CHECK(5, 0, 0) <= QT_VERSION)
		requestReschedule.storeRelease(newValue);
#else
		requestReschedule = newValue;
#endif
	}
};

#endif
