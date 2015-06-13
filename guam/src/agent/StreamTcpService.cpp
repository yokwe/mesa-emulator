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

const char* StreamTcpService::getMsgIDString(MsgID value) {
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


AgentStream::Task* StreamTcpService::createTask() {
	return (AgentStream::Task*)new TcpServiceTask();
}

AgentStream::Handler::ResultType StreamTcpService::idle(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/) {
	return ResultType::completed;
}

AgentStream::Handler::ResultType StreamTcpService::accept(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/) {
	ERROR();
	return ResultType::error;
}

AgentStream::Handler::ResultType StreamTcpService::connect(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/) {
	return ResultType::completed;
}

AgentStream::Handler::ResultType StreamTcpService::destroy(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/) {
	ERROR();
	return ResultType::error;
}

AgentStream::Handler::ResultType StreamTcpService::read(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) {
	TcpServiceTask* task = (TcpServiceTask*)AgentStream::getTask(iocb->mesaPut.hTask);
	{
		AgentStream::Block block(&iocb->mesaPut);
		task->readList.append(block);
		logger.debug("    block  mesaPut  %s", block.toHexString().toLocal8Bit().constData());
	}

	if (task->readList.size() == 4) {
		AgentStream::Block block0 = task->readList.at(0);
		AgentStream::Block block1 = task->readList.at(1);
		AgentStream::Block block2 = task->readList.at(2);
		AgentStream::Block block3 = task->readList.at(3);

		MsgID msgID = static_cast<MsgID>(block0.get32());
		switch(msgID) {
		case MsgID::connect:
			task->connect(iocb);
			break;
		case MsgID::listen:
			task->listen(iocb);
			break;
		case MsgID::put:
			task->put(iocb);
			break;
		case MsgID::get:
			task->get(iocb);
			break;
		case MsgID::close:
			task->close(iocb);
			break;
		case MsgID::setWaitTime:
			task->setWaitTime(iocb);
			break;
		case MsgID::endStream:
			task->endStream(iocb);
			break;
		case MsgID::shutDown:
			task->shutDown(iocb);
			break;
		case MsgID::reset:
			task->reset(iocb);
			break;
		default:
			logger.fatal("msgID = %d", block0.get32());
			ERROR();
		}
		task->readList.removeFirst(); // 0
		task->readList.removeFirst(); // 1
		task->readList.removeFirst(); // 2
		task->readList.removeFirst(); // 3
	}
	return ResultType::completed;
}

AgentStream::Handler::ResultType StreamTcpService::write(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) {
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

	TcpServiceTask* task = (TcpServiceTask*)AgentStream::getTask(iocb->mesaPut.hTask);
	ResultType ret;
	if (task->writeList.isEmpty()) {
		ret = ResultType::inProgress;
	} else {
		AgentStream::Block block = task->writeList.at(0);
		logger.debug("    block  %s", block.toHexString().toLocal8Bit().constData());

		block.get(&iocb->mesaGet);

		task->writeList.removeFirst();
		if (DEBUG_SHOW_STREAM_TCP_SERVICE) {
			task->debugDump(logger, __FUNCTION__);
			logger.debug("        get  sst = %3d  u2 = %02X  write = %4d  read = %4d  hTask = %d  interrupt = %d  writeLocked = %d",
				iocb->mesaGet.subSequence, iocb->mesaGet.u2, iocb->mesaGet.bytesWritten, iocb->mesaGet.bytesRead, iocb->mesaGet.hTask, iocb->mesaGet.interruptMesa, iocb->mesaGet.writeLockedByMesa);
		}
		ret =  ResultType::completed;
	}
	return ret;
}


CARD32                                      StreamTcpService::SocketInfo::socketIDNext = 1;
QMap<CARD32, StreamTcpService::SocketInfo*> StreamTcpService::socketMap;

void StreamTcpService::addSocket(SocketInfo* socketInfo) {
	const CARD32 socketID = socketInfo->socketID;
	if (socketMap.contains(socketID)) {
		logger.fatal("socketID = %d", socketID);
		ERROR();
	}
	socketMap[socketID] = socketInfo;

}
StreamTcpService::SocketInfo* StreamTcpService::getSocket(CARD32 socketID) {
	if (socketID == 0) return new SocketInfo();
	if (socketMap.contains(socketID)) return socketMap[socketID];
	logger.fatal("socketID_ = %d", socketID);
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
void StreamTcpService::TcpServiceTask::connect(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/) {
	const AgentStream::Block& block1 = readList.at(1);
	const AgentStream::Block& block2 = readList.at(2);
	const AgentStream::Block& block3 = readList.at(3);

	const QString remoteAddress = block1.getIPAddress();
	const CARD32  remotePort    = block2.get32();
	const CARD32  localPort     = block3.get32();

	logger.debug("%04d  Task::%-11s  remote %s:%d  localPort %5d", hTask, __FUNCTION__, remoteAddress.toLocal8Bit().constData(), remotePort, localPort);
	SocketInfo* socketInfo = new SocketInfo;
	{
		bool success = socketInfo->socket.bind(localPort);
		if (!success) {
			logger.debug("error = %s", socketInfo->socket.errorString().toLocal8Bit().constData());
			ERROR();
		}
	}

	socketInfo->socket.connectToHost(remoteAddress, remotePort, 0);
	{
		bool success = socketInfo->socket.waitForConnected();
		if (!success) {
			logger.debug("error = %s", socketInfo->socket.errorString().toLocal8Bit().constData());
			ERROR();
		}
	}

	// Output response
	{
		AgentStream::Block status(Status::success);
		AgentStream::Block socketID(socketInfo->socketID);

		writeList.append(status);
		writeList.append(socketID);
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
void StreamTcpService::TcpServiceTask::listen(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/) {
	const CARD32 socketID = readList.at(1).get32();
	const CARD32 timeout  = readList.at(2).get32();
	logger.debug("%04d  Task::%-11s  socketID %4d  timeout %4d", hTask, __FUNCTION__, socketID, timeout);
	/*SocketInfo* socket =*/ getSocket(socketID);
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
void StreamTcpService::TcpServiceTask::put(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/) {
	const CARD32 socketID = readList.at(1).get32();
	const CARD32 length   = readList.at(2).get32();
	logger.debug("%04d  Task::%-11s  socketID %4d  length %4d", hTask, __FUNCTION__, socketID, length);
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
void StreamTcpService::TcpServiceTask::get(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) {
	const CARD32 socketID = readList.at(1).get32();
	const CARD32 length   = readList.at(2).get32();
	logger.debug("%04d  Task::%-11s  socketID %4d  length %4d", hTask, __FUNCTION__, socketID, length);
	SocketInfo* socketInfo = getSocket(socketID);

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
	logger.debug("    socketID = %4d  mesaGet.bytesWritten = %4d", socketInfo->socketID, iocb->mesaGet.bytesWritten);
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
void StreamTcpService::TcpServiceTask::close(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/) {
	const CARD32 socketID = readList.at(1).get32();
	logger.debug("%04d  Task::%-11s  socketID %4d", hTask, __FUNCTION__, socketID);
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
void StreamTcpService::TcpServiceTask::setWaitTime(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/) {
	const CARD32 socketID = readList.at(1).get32();
	const CARD32 timeout  = readList.at(2).get32();
	logger.debug("%04d  Task::%-11s  socketID %4d  timeout %4d", hTask, __FUNCTION__, socketID, timeout);
	SocketInfo* socket = getSocket(socketID);
	socket->timeout = timeout;
	// TODO
	ERROR();
}

//    PutCommand[stream, endStream];
//    PutLongNumber[stream, LONG[0]];
//    PutLongNumber[stream, LONG[0]];
//    PutLongNumber[stream, LONG[0]];
void StreamTcpService::TcpServiceTask::endStream(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/) {
	logger.debug("%04d  Task::%-11s", hTask, __FUNCTION__);
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
void StreamTcpService::TcpServiceTask::shutDown(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/) {
	const CARD32 socketID = readList.at(1).get32();
	logger.debug("%04d  Task::%-11s  socketID %4d", hTask, __FUNCTION__, socketID);
	//SocketInfo* socket = SocketInfo::get(socketID);
	// TODO
	ERROR();
}


//    PutCommand[stream, reset];
//    PutLongNumber[stream, socketInfo.socketID];
//    PutLongNumber[stream, LONG[0]];
//    PutLongNumber[stream, LONG[0]];
void StreamTcpService::TcpServiceTask::reset(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/) {
	const CARD32 socketID = readList.at(1).get32();
	logger.debug("%04d  Task::%-11s  socketID %4d", hTask, __FUNCTION__, socketID);
	//SocketInfo* socket = SocketInfo::get(socketID);
	// TODO
	ERROR();
}
