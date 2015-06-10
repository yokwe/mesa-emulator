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
	enum class MsgID : CARD32 {
		connect = 0, listen = 1, put = 2, get = 3, close = 4,
		setWaitTime = 5, endStream = 6, shutDown = 7, reset = 8,
	};
	static const char* getMsgIDString(MsgID msgId);
	static const CARD32 MSG_ID_OFFSET = 1000;

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

	CARD16 idle   (CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb);
	CARD16 accept (CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb);
	CARD16 connect(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb);
	CARD16 destroy(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb);
	CARD16 read   (CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb);
	CARD16 write  (CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb);

protected:
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
		int getSize() {
			return data.size();
		}
		CARD32 get32() const;
		QString getIPAddress() {
			const CARD32 ipAddress = get32();
			const CARD8  a = (CARD8)(ipAddress >>  8); // AA
			const CARD8  b = (CARD8)(ipAddress >>  0); // BB
			const CARD8  c = (CARD8)(ipAddress >> 24); // CC
			const CARD8  d = (CARD8)(ipAddress >> 16); // DD

			return QString("%1.%2.%3.%4").arg(a).arg(b).arg(c).arg(d);
		}
		void put32(CARD32 value);

		void get(CoProcessorIOFaceGuam::TransferRec* mesaPut);
		void put(CoProcessorIOFaceGuam::TransferRec* mesaGet);

		QString toHexString() {
			QString ret(data.toHex().constData());
			return ret;
		}
	private:
		QByteArray data;
	};

	class Task {
	public:

		static CARD16 hTaskNext;
		static QMap<CARD32, Task*> map;
		static Task* get(CARD16 hTask_);

		const CARD16 hTask;

		QList<Block> outputList;
		QList<Block> inputList;

		Task() : hTask(++hTaskNext) {
			map.insert(hTask, this);
			this->clear();
		}

		void clear() {
			outputList.clear();
			inputList.clear();
		}
		void add(CoProcessorIOFaceGuam::TransferRec* mesaPut);

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
