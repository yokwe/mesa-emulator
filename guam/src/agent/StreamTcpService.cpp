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

StreamTcpService::StreamTcpService() : AgentStream::Stream(CoProcessorServerIDs::tcpService, "TcpService") {}

const char* StreamTcpService::getMsgIdString(MsgID msgId) {
	static QMap<MsgID, const char*> msgIDStringMap {
		{connect,     "connect"},
		{listen,      "listen"},
		{put,         "put"},
		{get,         "get"},
		{close,       "close"},
		{setWaitTime, "setWaitTime"},
		{endStream,   "endStream"},
		{shutDown,    "shutDown"},
		{reset,       "reset"},
	};

	if (msgIDStringMap.contains(msgId)) {
		return msgIDStringMap[msgId];
	}
	logger.fatal("msgId = %d", msgId);
	ERROR();
	return "UNKNONW";
}

static QMap<CARD32, const char*> statusStringMap {
	{StreamTcpService::S_success, "success"},
	{StreamTcpService::S_failure, "failure"},
};
const char* StreamTcpService::getStatusString(CARD32 status) {
	if (statusStringMap.contains(status)) {
		return statusStringMap[status];
	}
	logger.fatal("status = %d", status);
	ERROR();
	return "UNKNONW";
}

// State: TYPE = {closed, established, finWait, closing};
static QMap<CARD32, const char*> stateStringMap {
	{StreamTcpService::S_closed,      "closed"},
	{StreamTcpService::S_established, "established"},
	{StreamTcpService::S_finWait,     "finWait"},
	{StreamTcpService::S_closing,     "closing"},
};
const char* StreamTcpService::getStateString(CARD32 state) {
	if (stateStringMap.contains(state)) {
		return stateStringMap[state];
	}
	logger.fatal("state = %d", state);
	ERROR();
	return "UNKNONW";
}

CARD16 StreamTcpService::process(CoProcessorIOFaceGuam::CoProcessorFCBType* fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) {
	if (DEBUG_SHOW_STREAM_TCP_SERVICE) {
		logger.debug("    serverID = %-11s  mesaIsServer = %d  mesaState = %10s  pcState = %10s  next = %8X",
				getServerIDString(iocb->serverID), iocb->mesaIsServer, getStateString(iocb->mesaConnectionState), getStateString(iocb->pcConnectionState), iocb->nextIOCB);
		logger.debug("    %-8s  %-8s  %6X %6X", getCommandString(fcb->headCommand), getResultString(fcb->headResult), fcb->iocbHead, fcb->iocbNext);

		logger.debug("        put  sst = %3d  u2 = %02X  %6X  size = %4d written = %4d  read = %4d  hTask = %d  interrupt = %d  writeLocked = %d",
				iocb->mesaPut.subSequence, iocb->mesaPut.u2, iocb->mesaPut.buffer, iocb->mesaPut.bufferSize, iocb->mesaPut.bytesWritten, iocb->mesaPut.bytesRead, iocb->mesaPut.hTask, iocb->mesaPut.interruptMesa, iocb->mesaPut.writeLockedByMesa);
		logger.debug("        get  sst = %3d  u2 = %02X  %6X  size = %4d written = %4d  read = %4d  hTask = %d  interrupt = %d  writeLocked = %d",
				iocb->mesaGet.subSequence, iocb->mesaGet.u2, iocb->mesaGet.buffer, iocb->mesaGet.bufferSize, iocb->mesaGet.bytesWritten, iocb->mesaGet.bytesRead, iocb->mesaGet.hTask, iocb->mesaGet.interruptMesa, iocb->mesaGet.writeLockedByMesa);
	}
	// Sanity check
	if (iocb->serverID != this->serverID) ERROR();

	fcb->headResult = CoProcessorIOFaceGuam::R_error;
	switch (fcb->headCommand) {
	case CoProcessorIOFaceGuam::C_idle:
		ERROR(); // TODO implements this
		break;
	case CoProcessorIOFaceGuam::C_accept:
		ERROR(); // TODO implements this
		break;
	case CoProcessorIOFaceGuam::C_connect:
	{
		Task* task = new Task();
		iocb->pcConnectionState = CoProcessorIOFaceGuam::S_connected;
		iocb->mesaPut.hTask  = iocb->mesaGet.hTask = task->hTask;
		fcb->headResult = CoProcessorIOFaceGuam::R_completed;
	}
		break;
	case CoProcessorIOFaceGuam::C_delete:
		ERROR(); // TODO implements this
		break;
	case CoProcessorIOFaceGuam::C_read:
	{
		Task* task = Task::get(iocb->mesaPut.hTask);
		BigEndianByteBuffer  bb((CARD8*)Store(iocb->mesaPut.buffer), iocb->mesaPut.bytesWritten);
		bb.setPos(iocb->mesaPut.bytesRead);
		if (bb.remaining() != sizeof(CARD32)) ERROR();
		CARD32 message = bb.get32();
		task->addMessage(iocb, message);
		iocb->mesaPut.bytesRead = iocb->mesaPut.bytesWritten;
		fcb->headResult = CoProcessorIOFaceGuam::R_completed;
	}
		break;
	case CoProcessorIOFaceGuam::C_write:
		// DO nothing now
		fcb->headResult = CoProcessorIOFaceGuam::R_completed;
		if (iocb->mesaGet.interruptMesa && iocb->mesaGet.bytesRead < iocb->mesaGet.bytesWritten) {
			InterruptThread::notifyInterrupt(fcb->interruptSelector);
		}
		break;
	default:
		logger.fatal("headCommand = %d", fcb->headCommand);
		ERROR();
	}
	return fcb->headResult;
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

void StreamTcpService::Task::addMessage(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb, CARD32 message_) {
	logger.debug("addMessage %d %8X", messageSize, message_);

	message[messageSize++] = message_;

	if (messageSize == MESSAGE_SIZE) {
		MsgID msgid = static_cast<MsgID>(message[0] - MESSAGE_OFFSET);
		switch(msgid) {
		case MsgID::connect:
			connect(iocb);
			break;
		case MsgID::listen:
			listen(iocb);
			break;
		case MsgID::put:
			put(iocb);
			break;
		case MsgID::get:
			get(iocb);
			break;
		case MsgID::close:
			close(iocb);
			break;
		case MsgID::setWaitTime:
			setWaitTime(iocb);
			break;
		case MsgID::endStream:
			endStream(iocb);
			break;
		case MsgID::shutDown:
			shutDown(iocb);
			break;
		case MsgID::reset:
			reset(iocb);
			break;
		default:
			logger.fatal("message[0] = %d", message[0] - MESSAGE_OFFSET);
			ERROR();
		}
		clear();
	}
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
	const CARD32 remoteAddress = message[1];
	const CARD32 remotePort    = message[2];
	const CARD32 localPort     = message[3];
	const CARD8  a = (CARD8)(remoteAddress >>  8); // AA
	const CARD8  b = (CARD8)(remoteAddress >>  0); // BB
	const CARD8  c = (CARD8)(remoteAddress >> 24); // CC
	const CARD8  d = (CARD8)(remoteAddress >> 16); // DD

	QString address = QString("%1.%2.%3.%4").arg(a).arg(b).arg(c).arg(d);
	logger.debug("address = %s", address.toLocal8Bit().constData());


	logger.info("connect  %4d  remote %s:%d  localPort %5d", hTask, address.toLocal8Bit().constData(), remotePort, localPort);
	SocketInfo* socketInfo = new SocketInfo;
	socketInfo->socket.bind(localPort);
	logger.debug("After bind     state = %d", socketInfo->socket.state());

	socketInfo->socket.connectToHost(address, remotePort, 0);
	logger.debug("After connect  state = %d", socketInfo->socket.state());

	// Output response
	BigEndianByteBuffer  bb((CARD8*)Store(iocb->mesaGet.buffer), iocb->mesaGet.bufferSize * sizeof(CARD16));
	bb.put32(S_success);
	bb.put32(socketInfo->socketID);
	iocb->mesaGet.bytesWritten = bb.getPos();
	logger.info("    socketID = %4d  mesaGet.bytesWritten = %4d", socketInfo->socketID, iocb->mesaGet.bytesWritten);
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
	CARD32 socketID = message[1];
	CARD32 timeout  = message[2];
	logger.info("listen  %4d  %4d  %4d", hTask, socketID, timeout);
	/*SocketInfo* socket =*/ SocketInfo::get(socketID);
	// TODO
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
	CARD32 socketID = message[1];
	CARD32 length   = message[2];
	logger.info("put  %4d  %4d  %4d", hTask, socketID, length);
	//SocketInfo* socket = SocketInfo::get(socketID);
	// TODO
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
	CARD32 socketID = message[1];
	CARD32 length   = message[2];
	logger.info("get  %4d  %4d  %4d", hTask, socketID, length);
	SocketInfo* socketInfo = SocketInfo::get(socketID);

	quint32 bytesAvailable = (quint32)(socketInfo->socket.bytesAvailable());
	if (0 < bytesAvailable) {
		// Output response
		BigEndianByteBuffer  bb((CARD8*)Store(iocb->mesaGet.buffer), iocb->mesaGet.bufferSize * sizeof(CARD16));
		bb.put32(S_success);
		bb.put32((quint32)bytesAvailable);
		QByteArray data = socketInfo->socket.read(bb.remaining());
		quint32 dataSize = data.size();
		bb.putAll((const quint8*)data.constData(), dataSize);
		logger.debug("data = %s!", data.constData());
		iocb->mesaGet.bytesWritten = bb.getPos();
	} else {
		// Output response
		BigEndianByteBuffer  bb((CARD8*)Store(iocb->mesaGet.buffer), iocb->mesaGet.bufferSize * sizeof(CARD16));
		bb.put32(S_success);
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
	CARD32 socketID = message[1];
	logger.info("close  %4d  %4d", hTask, socketID);
	//SocketInfo* socket = SocketInfo::get(socketID);
	// TODO
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
	CARD32 socketID = message[1];
	CARD32 timeout  = message[2];
	logger.info("setWaitTime  %4d  %4d  %4d", hTask, socketID, timeout);
	SocketInfo* socket = SocketInfo::get(socketID);
	socket->timeout = timeout;
	// TODO
}

//    PutCommand[stream, endStream];
//    PutLongNumber[stream, LONG[0]];
//    PutLongNumber[stream, LONG[0]];
//    PutLongNumber[stream, LONG[0]];
void StreamTcpService::Task::endStream(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/) {
	logger.info("endStream  %4d", hTask);
	// TODO
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
	CARD32 socketID = message[1];
	logger.info("shutDown  %4d  %4d", hTask, socketID);
	//SocketInfo* socket = SocketInfo::get(socketID);
	// TODO
}


//    PutCommand[stream, reset];
//    PutLongNumber[stream, socketInfo.socketID];
//    PutLongNumber[stream, LONG[0]];
//    PutLongNumber[stream, LONG[0]];
void StreamTcpService::Task::reset(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/) {
	CARD32 socketID = message[1];
	logger.info("reset  %4d  %4d", hTask, socketID);
	//SocketInfo* socket = SocketInfo::get(socketID);
	// TODO
}

