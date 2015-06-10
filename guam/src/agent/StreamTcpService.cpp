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
// StreamTcpService.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("tcpservice");


#include "../util/Debug.h"
#include "../util/ByteBuffer.h"

#include "../mesa/Pilot.h"
#include "../mesa/Memory.h"
#include "../mesa/MesaThread.h"

#include "StreamTcpService.h"

#define DEBUG_SHOW_STREAM_TCP_SERVICE 1

StreamTcpService::StreamTcpService() : AgentStream::Handler(CoProcessorServerIDs::tcpService, "TcpService") {}

const char* StreamTcpService::getMsgIdString(MsgID value) {
	static QMap<MsgID, const char*> map {
		{MsgID::connect,     "connect"},
		{MsgID::listen,      "listen"},
		{MsgID::put,         "put"},
		{MsgID::get,         "get"},
		{MsgID::close,       "close"},
		{MsgID::setWaitTime, "setWaitTime"},
		{MsgID::endStream,   "endStream"},
		{MsgID::shutDown,    "shutDown"},
		{MsgID::reset,       "reset"},
	};

	if (map.contains(value)) {
		return map[value];
	}
	logger.fatal("value = %d", value);
	ERROR();
	return "UNKNONW";
}

const char* StreamTcpService::getStatusString(Status value) {
	static QMap<Status, const char*> map {
		{Status::success, "success"},
		{Status::failure, "failure"},
	};

	if (map.contains(value)) {
		return map[value];
	}
	logger.fatal("value = %d", value);
	ERROR();
	return "UNKNONW";
}

// State: TYPE = {closed, established, finWait, closing};
const char* StreamTcpService::getStateString(State value) {
	static QMap<State, const char*> map {
		{State::closed,      "closed"},
		{State::established, "established"},
		{State::finWait,     "finWait"},
		{State::closing,     "closing"},
	};

	if (map.contains(value)) {
		return map[value];
	}
	logger.fatal("value = %d", value);
	ERROR();
	return "UNKNONW";
}

CARD16 StreamTcpService::idle(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) {
	if (DEBUG_SHOW_STREAM_TCP_SERVICE) debugDump(logger, __FUNCTION__, iocb);

	// Sanity check
	if (iocb->serverID != this->serverID) ERROR();
	if (iocb->mesaPut.hTask) {
		logger.fatal("mesaPut.hTask = %d", iocb->mesaPut.hTask);
		ERROR();
	}
	if (iocb->mesaGet.hTask) {
		logger.fatal("mesaGet.hTask = %d", iocb->mesaGet.hTask);
		ERROR();
	}

	Task* task = new Task();
	iocb->mesaPut.hTask  = iocb->mesaGet.hTask = task->hTask;
	iocb->pcConnectionState = CoProcessorIOFaceGuam::S_connected;
	return CoProcessorIOFaceGuam::R_completed;
}

CARD16 StreamTcpService::accept(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) {
	if (DEBUG_SHOW_STREAM_TCP_SERVICE) debugDump(logger, __FUNCTION__, iocb);

	// Sanity check
	if (iocb->serverID != this->serverID) ERROR();
	if (iocb->mesaPut.hTask == 0) {
		logger.fatal("mesaPut.hTask = %d", iocb->mesaPut.hTask);
		ERROR();
	}
	if (iocb->mesaGet.hTask == 0) {
		logger.fatal("mesaGet.hTask = %d", iocb->mesaGet.hTask);
		ERROR();
	}
	// TODO implements accept
	ERROR();
	return CoProcessorIOFaceGuam::R_error;
}

CARD16 StreamTcpService::connect(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) {
	if (DEBUG_SHOW_STREAM_TCP_SERVICE) debugDump(logger, __FUNCTION__, iocb);

	// Sanity check
	if (iocb->serverID != this->serverID) ERROR();
	if (iocb->mesaPut.hTask == 0) {
		logger.fatal("mesaPut.hTask = %d", iocb->mesaPut.hTask);
		ERROR();
	}
	if (iocb->mesaGet.hTask == 0) {
		logger.fatal("mesaGet.hTask = %d", iocb->mesaGet.hTask);
		ERROR();
	}
	// TODO implements accept
	ERROR();
	return CoProcessorIOFaceGuam::R_error;
}

CARD16 StreamTcpService::destroy(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) {
	if (DEBUG_SHOW_STREAM_TCP_SERVICE) debugDump(logger, __FUNCTION__, iocb);

	// Sanity check
	if (iocb->serverID != this->serverID) ERROR();
	if (iocb->mesaPut.hTask == 0) {
		logger.fatal("mesaPut.hTask = %d", iocb->mesaPut.hTask);
		ERROR();
	}
	if (iocb->mesaGet.hTask == 0) {
		logger.fatal("mesaGet.hTask = %d", iocb->mesaGet.hTask);
		ERROR();
	}
	// TODO implements accept
	ERROR();
	return CoProcessorIOFaceGuam::R_error;
}

CARD16 StreamTcpService::read(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) {
	if (DEBUG_SHOW_STREAM_TCP_SERVICE) debugDump(logger, __FUNCTION__, iocb);

	// Sanity check
	if (iocb->serverID != this->serverID) ERROR();
	if (iocb->mesaPut.hTask == 0) {
		logger.fatal("mesaPut.hTask = %d", iocb->mesaPut.hTask);
		ERROR();
	}
	if (iocb->mesaGet.hTask == 0) {
		logger.fatal("mesaGet.hTask = %d", iocb->mesaGet.hTask);
		ERROR();
	}
	// TODO implements accept
	ERROR();
	return CoProcessorIOFaceGuam::R_error;
}

CARD16 StreamTcpService::write(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) {
	if (DEBUG_SHOW_STREAM_TCP_SERVICE) debugDump(logger, __FUNCTION__, iocb);

	// Sanity check
	if (iocb->serverID != this->serverID) ERROR();
	if (iocb->mesaPut.hTask == 0) {
		logger.fatal("mesaPut.hTask = %d", iocb->mesaPut.hTask);
		ERROR();
	}
	if (iocb->mesaGet.hTask == 0) {
		logger.fatal("mesaGet.hTask = %d", iocb->mesaGet.hTask);
		ERROR();
	}
	// TODO implements accept
	ERROR();
	return CoProcessorIOFaceGuam::R_error;
}



CARD32                                      StreamTcpService::SocketInfo::socketIDNext = 1;
QMap<CARD32, StreamTcpService::SocketInfo*> StreamTcpService::SocketInfo::map;

CARD16                                StreamTcpService::Task::hTaskNext = 1;
QMap<CARD32, StreamTcpService::Task*> StreamTcpService::Task::map;

StreamTcpService::SocketInfo* StreamTcpService::SocketInfo::get(CARD32 socketID_) {
	if (socketID_ == 0) return new SocketInfo();
	if (map.contains(socketID_)) return map[socketID_];
	logger.fatal("socketID_ = %d", socketID_);
	ERROR();
	return 0;
}

StreamTcpService::Task* StreamTcpService::Task::get(CARD16 hTask_) {
	if (hTask_ == 0) return new Task();
	if (map.contains(hTask_)) return map[hTask_];
	logger.fatal("hTask_ = %d", hTask_);
	ERROR();
	return 0;
}



//    PutCommand[stream, connect];
//    PutLongNumber[stream, LOOPHOLE[remote]];
//    PutLongNumber[stream, LONG[rPort]];
//    PutLongNumber[stream, LOOPHOLE[LONG[lPort]]];
//    ----
//    status _ GetStatus[stream];
//    if (status == failure) {
//      reason _ GetReason[stream];
//    }
//    socketID _ GetLongNumber[stream];
void StreamTcpService::Task::connect(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) {
	const CARD32 remoteAddress = msg1;
	const CARD32 remotePort    = msg2;
	const CARD32 localPort     = msg3;
	const CARD8  a = (CARD8)(remoteAddress >>  8); // AA
	const CARD8  b = (CARD8)(remoteAddress >>  0); // BB
	const CARD8  c = (CARD8)(remoteAddress >> 24); // CC
	const CARD8  d = (CARD8)(remoteAddress >> 16); // DD

	QString address = QString("%1.%2.%3.%4").arg(a).arg(b).arg(c).arg(d);
	logger.debug("address = %s", address.toLocal8Bit().constData());


	logger.info("connect  %4d  remote %s:%d  localPort %5d", hTask, address.toLocal8Bit().constData(), remotePort, localPort);
	SocketInfo* socketInfo = new SocketInfo;
	{
		bool success = socketInfo->socket.bind(localPort);
		if (!success) {
			logger.debug("error = %s", socketInfo->socket.errorString().toLocal8Bit().constData());
			ERROR();
		}
	}

	socketInfo->socket.connectToHost(address, remotePort, 0);
	{
		bool success = socketInfo->socket.waitForConnected();
		if (!success) {
			logger.debug("error = %s", socketInfo->socket.errorString().toLocal8Bit().constData());
			ERROR();
		}
	}

	// Output response
	{
		BigEndianByteBuffer  bb((CARD8*)Store(iocb->mesaGet.buffer), iocb->mesaGet.bufferSize * sizeof(CARD16));
		// Status
		{
			bb.clear();
			bb.put32(Status::success);
			QByteArray qba((const char*)bb.getData(), (int)bb.getPos());
			outputList.append(qba);
		}
		// socketID
		{
			bb.clear();
			bb.put32(socketInfo->socketID);
			QByteArray qba((const char*)bb.getData(), (int)bb.getPos());
			outputList.append(qba);
		}
	}
}



//    PutCommand[stream, listen];
//    PutLongNumber[stream, LOOPHOLE[LONG[lPort]]];
//    PutLongNumber[stream, timeout];
//    PutLongNumber[stream, LONG[0]];
//    ----
//    status _ GetStatus[stream];
//    if (status == failure) {
//      reason _ GetReason[stream];
//    }
//    tmpSocketID _ GetLongNumber[stream];
//    status _ GetStatusForListen[stream, 2000, tmpSocketID ! TcpStream.Suspended => TcpStream.Failed[remoteReject]];
//    if (status == failure) {
//      reason _ GetReason[stream];
//    }
//    socketID _ GetLongNumber[stream];
void StreamTcpService::Task::listen(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/) {
	const CARD32 socketID = msg1;
	const CARD32 timeout  = msg2;
	logger.info("listen  %4d  %4d  %4d", hTask, socketID, timeout);
	/*SocketInfo* socket =*/ SocketInfo::get(socketID);
	// TODO
	ERROR();
}



//    DO
//      PutCommand[stream, put];
//      PutLongNumber[stream, socketInfo.socketID];
//      PutLongNumber[stream, length];
//      PutLongNumber[stream, LONG[0]];
//      Stream.PutBlock[stream, block, FALSE];
//      status _ GetStatusX[stream, 10000];
//      IF status = success THEN {
//	      putLen _ GetLongNumber[stream];
//  	    -- IF putLen = 0 THEN TcpStream.Suspended[noRouteToDestination];
//      } else {
//        reason _ GetReason[stream];
//        LOOP
//      }
//      ENDLOOP
void StreamTcpService::Task::put(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/) {
	const CARD32 socketID = msg1;;
	const CARD32 length   = msg2;
	logger.info("put  %4d  %4d  %4d", hTask, socketID, length);
	//SocketInfo* socket = SocketInfo::get(socketID);
	// TODO
	ERROR();
}


//    PutCommand[stream, get];
//    PutLongNumber[stream, socketInfo.socketID];
//    PutLongNumber[stream, length];
//    PutLongNumber[stream, LONG[0]];
//    ----
//    status _ GetStatusXX[stream, length, 10000];
//    IF status = success THEN {
//      dataLen: LONG CARDINAL _ GetLongNumber[stream];
//    } else {
//      reason _ GetReason[stream];
//    }
void StreamTcpService::Task::get(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) {
	const CARD32 socketID = msg1;
	const CARD32 length   = msg2;
	logger.info("get  %4d  %4d  %4d", hTask, socketID, length);
	SocketInfo* socketInfo = SocketInfo::get(socketID);

	quint32 bytesAvailable = (quint32)(socketInfo->socket.bytesAvailable());
	if (0 < bytesAvailable) {
		// Output response
		BigEndianByteBuffer  bb((CARD8*)Store(iocb->mesaGet.buffer), iocb->mesaGet.bufferSize * sizeof(CARD16));
		bb.put32(Status::success);
		bb.put32((quint32)bytesAvailable);
		//
		QByteArray data = socketInfo->socket.read(bb.remaining());
		quint32 dataSize = data.size();
		bb.putAll((const quint8*)data.constData(), dataSize);
		logger.debug("data = %s!", data.constData());
		iocb->mesaGet.bytesWritten = bb.getPos();
	} else {
		// Output response
		BigEndianByteBuffer  bb((CARD8*)Store(iocb->mesaGet.buffer), iocb->mesaGet.bufferSize * sizeof(CARD16));
		bb.put32(Status::success);
		bb.put32(0);
		iocb->mesaGet.bytesWritten = bb.getPos();
	}
	logger.info("    socketID = %4d  mesaGet.bytesWritten = %4d", socketInfo->socketID, iocb->mesaGet.bytesWritten);
}

//    PutCommand[stream, close];
//    PutLongNumber[stream, socketInfo.socketID];
//    PutLongNumber[stream, LONG[0]];
//    PutLongNumber[stream, LONG[0]];
//    ----
//    status _ GetStatus[stream];
//    IF status = failure THEN {
//      reason _ GetReason[stream];
//    }
void StreamTcpService::Task::close(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/) {
	const CARD32 socketID = msg1;
	logger.info("close  %4d  %4d", hTask, socketID);
	//SocketInfo* socket = SocketInfo::get(socketID);
	// TODO
	ERROR();
}


//    PutCommand[stream, setWaitTime];
//    PutLongNumber[stream, socketInfo.socketID];
//    PutLongNumber[stream, timeout];
//    PutLongNumber[stream, LONG[0]];
//    ----
//    status _ GetStatus[stream];
//    IF status = failure THEN {
//      reason _ GetReason[stream];
//    }
void StreamTcpService::Task::setWaitTime(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/) {
	const CARD32 socketID = msg1;
	const CARD32 timeout  = msg2;
	logger.info("setWaitTime  %4d  %4d  %4d", hTask, socketID, timeout);
	SocketInfo* socket = SocketInfo::get(socketID);
	socket->timeout = timeout;
	// TODO
	ERROR();
}

//    PutCommand[stream, endStream];
//    PutLongNumber[stream, LONG[0]];
//    PutLongNumber[stream, LONG[0]];
//    PutLongNumber[stream, LONG[0]];
void StreamTcpService::Task::endStream(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/) {
	logger.info("endStream  %4d", hTask);
	// TODO
	ERROR();
}

//    PutCommand[stream, shutDown];
//    PutLongNumber[stream, socketInfo.socketID];
//    PutLongNumber[stream, LONG[1]];
//    PutLongNumber[stream, LONG[0]];
//    ----
//    status _ GetStatus[stream];
//    IF status = failure THEN {
//      reason _ GetReason[stream];
//    }
void StreamTcpService::Task::shutDown(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/) {
	const CARD32 socketID = msg1;
	logger.info("shutDown  %4d  %4d", hTask, socketID);
	//SocketInfo* socket = SocketInfo::get(socketID);
	// TODO
	ERROR();
}


//    PutCommand[stream, reset];
//    PutLongNumber[stream, socketInfo.socketID];
//    PutLongNumber[stream, LONG[0]];
//    PutLongNumber[stream, LONG[0]];
void StreamTcpService::Task::reset(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/) {
	const CARD32 socketID = msg1;
	logger.info("reset  %4d  %4d", hTask, socketID);
	//SocketInfo* socket = SocketInfo::get(socketID);
	// TODO
	ERROR();
}

