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
// StreamTcpService.h
//

#include "AgentStream.h"

#include <QtCore>
#include <QTcpSocket>

class StreamTcpService : public AgentStream::Handler {
public:
	// MsgId: TYPE = MACHINE DEPENDENT {connect(0), listen(1), put(2), get(3), close(4), setWaitTime(5), endStream(6), shutDown(7), reset(8)};
	enum MsgID : CARD32 {
		connect = 0, listen = 1, put = 2, get = 3, close = 4,
		setWaitTime = 5, endStream = 6, shutDown = 7, reset = 8,
	};
	static const char* getMsgIdString(MsgID msgId);

	enum Status : CARD32 {
		success = 0, failure = 1,
	};
	// Status: TYPE = MACHINE DEPENDENT {success(0), failure(1)};
	const char* getStatusString(Status status);

	// State: TYPE = {closed, established, finWait, closing};
	enum State : CARD32 {
		closed = 0, established = 1, finWait = 2, closing = 3,
	};
	const char* getStateString(State state);

	StreamTcpService();
	void process(CoProcessorIOFaceGuam::CoProcessorFCBType* fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb);

protected:
	class Task {
	public:
		static const CARD32 MESSAGE_OFFSET = 1000;
		static const CARD32 MESSAGE_SIZE   = 4;

		static CARD16 hTaskNext;
		static QMap<CARD32, Task*> map;
		static Task* get(CARD16 hTask_);

		const CARD16 hTask;

		quint32 msgCount;
		MsgID   msg0;
		CARD32  msg1;
		CARD32  msg2;
		CARD32  msg3;

		QList<QByteArray> outputList;
		QList<QByteArray> inputList;

		Task() : hTask(++hTaskNext) {
			clear();
			map.insert(hTask, this);
		}
		void clear() {
			msgCount = 0;
			msg0 = MsgID::connect;
			msg1 = msg2 = msg3 = 0;
			//
			outputList.clear();
			inputList.clear();
		}

		void connect    (CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb);
		void listen     (CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb);
		void put        (CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb);
		void get        (CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb);
		void close      (CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb);
		void setWaitTime(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb);
		void endStream  (CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb);
		void shutDown   (CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb);
		void reset      (CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb);
	};

	class SocketInfo {
	public:
		static CARD32 socketIDNext;
		static QMap<CARD32, SocketInfo*> map;
		static SocketInfo* get(CARD32 socketID_);

		const CARD32 socketID;

		CARD32 state;
		CARD32 localAddress;
		CARD32 remoteAddress;
		CARD16 localPort;
		CARD16 remotePort;
		CARD32 timeout;

		QTcpSocket socket;

		SocketInfo() : socketID(++socketIDNext) {
			map.insert(socketID, this);
			clear();
		}

		void clear() {
			state = 0;
			localAddress = remoteAddress = localPort = remotePort = 0;
			timeout = 0;
		}
	};
};
