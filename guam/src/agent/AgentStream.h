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
	//
	// Block
	//
	class Block {
	public:
		Block(CoProcessorIOFaceGuam::TransferRec* mesaPut) {
			put(mesaPut);
		}
		Block(const Block& that) {
			this->data = that.data;
		}
		Block(const CARD32 value) {
			put32(value);
		}
		int getSize() const {
			return data.size();
		}
		CARD32 get32() const;
		void   put32(CARD32 value);

		QString getIPAddress() const {
			const CARD32 ipAddress = get32();
			const CARD8  a = (CARD8)(ipAddress >>  8); // AA
			const CARD8  b = (CARD8)(ipAddress >>  0); // BB
			const CARD8  c = (CARD8)(ipAddress >> 24); // CC
			const CARD8  d = (CARD8)(ipAddress >> 16); // DD

			return QString("%1.%2.%3.%4").arg(a).arg(b).arg(c).arg(d);
		}

		void get(CoProcessorIOFaceGuam::TransferRec* mesaPut);
		void put(CoProcessorIOFaceGuam::TransferRec* mesaGet);

		QString toHexString() {
			QString ret(data.toHex().constData());
			return ret;
		}
	private:
		QByteArray data;
	};


	//
	// Task
	//
	class Task {
	protected:
		static       CARD16              hTaskNext;

		Task() : hTask(++hTaskNext) {
			addTask(this);
		}

	public:
		const CARD16 hTask;

		QList<Block> readList;
		QList<Block> writeList;

		void debugDump(log4cpp::Category& logger);
	};


	//
	// Handler
	//
	class Handler {
	public:
		//ResultType: TYPE = MACHINE DEPENDENT
		//  {completed(0), inProgress(1), error(2)};
		enum class ResultType : CARD16 {
			completed  = CoProcessorIOFaceGuam::R_completed,
			inProgress = CoProcessorIOFaceGuam::R_inProgress,
			error      = CoProcessorIOFaceGuam::R_error,
		};

		static void     initialize();
		static void     debugDump(log4cpp::Category& logger, const char* name, CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb);
		static Handler* getHandler(CARD32 serverID);

		const CARD32  serverID;
		const QString name;

		Handler(CARD32 serverID_, QString name_) : serverID(serverID_), name(name_) {}
		virtual ~Handler();

		// Return Handler specific Task
		virtual Task*      createTask() = 0;

		virtual ResultType idle   (CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) = 0;
		virtual ResultType accept (CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) = 0;
		virtual ResultType connect(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) = 0;
		virtual ResultType destroy(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) = 0;
		virtual ResultType read   (CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) = 0;
		virtual ResultType write  (CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) = 0;
	};


	//
	// AgentStream
	//
	static const char* getServerIDString(CARD32 serverID);
	static const char* getCommandString (CARD16 command);
	static const char* getStateString   (CARD16 state);
	static const char* getResultString  (CARD16 result);

	static void notifyInterrupt();

	static void                  setDefaultHandler(Handler* handler);
	static void                  addHandler(AgentStream::Handler* handler);
	static AgentStream::Handler* getHandler(CARD32 serverID_);

	static void                  addTask(Task* task_);
	static AgentStream::Task*    getTask(CARD16 hTask);

	AgentStream() : Agent(GuamInputOutput::stream, "Stream") {
		fcb = 0;
	}

	CARD32 getFCBSize() {
		return SIZE(*fcb);
	}

	void Initialize();
	void Call();

private:
	static QMap<CARD32, Handler*> handlerMap;
	static Handler*               defaultHandler;
	static QMap<CARD16, Task*>    taskMap;
	static CARD16                 interruptSelector;

	CoProcessorIOFaceGuam::CoProcessorFCBType* fcb;

	void initialize();
};

#endif
