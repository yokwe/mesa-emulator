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
// AgentStream.h
//

#ifndef AGENTSTREAM_H__
#define AGENTSTREAM_H__

#include "Agent.h"

#include <QtCore>

class AgentStream : public Agent {
public:
	class Task {
	private:
		static CARD16 hTaskNext;
	public:
		const CARD16 hTask; // Start from one

		Task() : hTask(hTaskNext++) {};
	};

	class Data {
	private:
		static const int WAIT_TIME_IN_MILLISECOND = 1000;

		static QByteArray readMesa(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb);
		static void writeMesa(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb, QByteArray data);

		static QByteArray toByteArray(CARD32 data);
		static CARD32 toCARD32(QByteArray data);

		QMutex             mutex;
		QWaitCondition     cv;
		QQueue<QByteArray> queue;

	public:
		bool waitData(unsigned long time = WAIT_TIME_IN_MILLISECOND) {
			QMutexLocker locker(&mutex);
			const int size = queue.size();
			if (size) return true;

			return cv.wait(&mutex, time);
		}
		void put(QByteArray data) {
			QMutexLocker locker(&mutex);
			queue.enqueue(data);

			cv.wakeAll();
		}
		QByteArray get() {
			QMutexLocker locker(&mutex);
			return queue.dequeue();
		}
		int size() {
			QMutexLocker locker(&mutex);
			return queue.size();
		}

		// Create QByteArra data from iocb->mesaPut and append to queue
		void put(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) {
			put(readMesa(iocb));
		}

		// Take one QByteArray data from queue and append to iocb->mesaGet
		void get(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) {
			writeMesa(iocb, get());
		}

		void put32(CARD32 data) {
			put(toByteArray(data));
		}
		CARD32 get32() {
			return toCARD32(get());
		}
	};

	class Handler : public QRunnable {
	public:
		const CARD32 serverID;
		const char*  name;

		Data dataRead;
		Data dataWrite;

		Handler(CARD32 serverID_, const char* name_) : serverID(serverID_), name(name_) {}
		virtual ~Handler() {}

		virtual Task* createTask() = 0;

		// Return value of methods below is CoProcessorIOFaceGuam::R_*
		//   R_completed, R_inProgress, R_error
		virtual CARD16 idle   (CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb, Task* task) = 0;
		virtual CARD16 accept (CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb, Task* task) = 0;
		virtual CARD16 connect(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb, Task* task) = 0;
		// To avoid conflict with keyword "delete", use "destroy" instead.
		virtual CARD16 destroy(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb, Task* task) = 0;

//		virtual CARD16 read   (CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb, Task* task) = 0;
//		virtual CARD16 write  (CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb, Task* task) = 0;

		virtual void run() = 0;
		static void stop() {
			stopThread = true;
		}
	protected:
		// Wait interval in milliseconds for QWaitCondition::wait
		static const int WAIT_INTERVAL = 1000;
		static const QThread::Priority PRIORITY = QThread::HighPriority;

		static bool stopThread;
	};

	AgentStream() : Agent(GuamInputOutput::stream, "Stream") {
		fcb = 0;
	}

	CARD32 getFCBSize() {
		return SIZE(*fcb);
	}

	void Initialize();
	void Call();

	// Start and stop threads for AgentStream
	static void startThread();
	static void stopThread();

private:
	static CoProcessorIOFaceGuam::CoProcessorFCBType* fcb;
	static Handler*                                   defaultHandler;
	static QMap<CARD32, Handler*>                     handlerMap;
	static QMap<CARD32, Task*>                        taskMap;

	static void     setDefaultHandler(Handler* handler);
	static void     addHandler       (Handler* handler);
	static Handler* getHandler       (CARD32 serverID);

	static void  addTask   (Task* task);
	static void  removeTask(Task* task);
	static Task* getTask   (CARD32 taskID);
};

#endif
