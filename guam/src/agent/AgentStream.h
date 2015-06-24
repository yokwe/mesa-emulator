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

#include <log4cpp/Category.hh>

#include "Agent.h"

#include <QtCore>

class AgentStream : public Agent {
public:
	//CommandType: TYPE = MACHINE DEPENDENT
	//  {idle(0), accept(1), connect(2), delete(3), read(4), write(5)};
	enum class Command : CARD16 {
		idle    = CoProcessorIOFaceGuam::C_idle,
		accept  = CoProcessorIOFaceGuam::C_accept,
		connect = CoProcessorIOFaceGuam::C_connect,
		destroy = CoProcessorIOFaceGuam::C_delete,
		read    = CoProcessorIOFaceGuam::C_read,
		write   = CoProcessorIOFaceGuam::C_write,
	};
	const char* toString(Command value);

	//ConnectionStateType: TYPE = MACHINE DEPENDENT
	//  {idle(0), accepting(1), connected(2), deleted(3)};
	enum class State : CARD16 {
		idle      = CoProcessorIOFaceGuam::S_idle,
		accepting = CoProcessorIOFaceGuam::S_accepting,
		connected = CoProcessorIOFaceGuam::S_connected,
		deleted   = CoProcessorIOFaceGuam::S_deleted,
	};
	const char* toString(State value);

	//ResultType: TYPE = MACHINE DEPENDENT
	//  {completed(0), inProgress(1), error(2)};
	enum class Result : CARD16 {
		completed  = CoProcessorIOFaceGuam::R_completed,
		inProgress = CoProcessorIOFaceGuam::R_inProgress,
		error      = CoProcessorIOFaceGuam::R_error,
	};
	const char* toString(Result value);

	class Task {
	private:
		static CARD16 hTaskNext;
	public:
		const CARD16 hTask; // Start from one

		Task() : hTask(hTaskNext++) {};
	};

	class StreamData {
	public:
		const QByteArray data;
		const CARD8      sst;
		const bool       endSST;
		const bool       endRecord;
		const bool       endStream;

		StreamData(QByteArray data_, CARD8 sst_, bool endSST_, bool endRecord_, bool endStream_) :
			data(data_), sst(sst_), endSST(endSST_), endRecord(endRecord_), endStream(endStream_) {}
		StreamData(QByteArray data_) : data(data_), sst(0), endSST(false), endRecord(false), endStream(false) {}
		StreamData(const StreamData& that) : data(that.data), sst(that.sst), endSST(that.endSST), endRecord(that.endRecord), endStream(that.endStream) {}
	};

	class Data {
	private:
		static const int WAIT_TIME_IN_MILLISECOND = 1000;

		QMutex             mutex;
		QWaitCondition     cv;
		QQueue<StreamData> queue;

	public:
		static QByteArray readMesa (CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb);
		static void       writeMesa(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb, QByteArray data);

		static QByteArray toByteArray(CARD32 data);
		static CARD32     toCARD32(QByteArray data);
		static QString    toIPAddress(CARD32 data);

		bool waitData(unsigned long time = WAIT_TIME_IN_MILLISECOND) {
			QMutexLocker locker(&mutex);
			const int size = queue.size();
			if (size) return true;

			return cv.wait(&mutex, time);
		}
		void put(StreamData data) {
			QMutexLocker locker(&mutex);
			queue.enqueue(data);

			cv.wakeAll();
		}
		StreamData get() {
			QMutexLocker locker(&mutex);
			return queue.dequeue();
		}
		int size() {
			QMutexLocker locker(&mutex);
			return queue.size();
		}

		void put32(CARD32 data_) {
			StreamData data(toByteArray(data_));
			put(data);
		}
		CARD32 get32() {
			return toCARD32(get().data);
		}
		QString toString();
	};

	class Handler : public QRunnable {
	public:
		class StopThreadException {};

		const CARD32 serverID;
		const char*  name;

		Data dataRead;
		Data dataWrite;

		Handler(CARD32 serverID_, const char* name_) : serverID(serverID_), name(name_) {}
		virtual ~Handler() {}

		virtual Task* createTask() = 0;

		virtual void idle   (CoProcessorIOFaceGuam::CoProcessorFCBType* fcb) = 0;

		virtual Result accept (CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb, Task* task) = 0;
		virtual Result connect(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb, Task* task) = 0;
		// To avoid conflict with keyword "delete", use "destroy" instead.
		virtual Result destroy(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb, Task* task) = 0;

		virtual void run() = 0;
		static void stop() {
			stopThread = true;
		}

		CARD32 getData32();
		void   putData32(CARD32 value) {
			putData(AgentStream::Data::toByteArray(value));
		}

		// get data from dataRead, wait until data is arrived. If stopThread is true, generate StotThreadException
		StreamData getData();
		// put data to writeData
		void       putData(StreamData data_);
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

	void dump(log4cpp::Category& logger);

	// Start and stop threads for AgentStream
	static void startThread();
	static void stopThread();

	static void notifyInterrupt();
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
