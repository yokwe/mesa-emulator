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
	logger.debug("connect  hTask = %04d", task->hTask);
	return CoProcessorIOFaceGuam::R_completed;
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
	// TODO implements destroy
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

	Task* task = Task::getInstance(iocb->mesaPut.hTask);
	{
		Block block(&iocb->mesaPut);
		task->inputList.append(block);
		logger.debug("    block  mesaPut  %s", block.toHexString().toLocal8Bit().constData());
	}

	if (DEBUG_SHOW_STREAM_TCP_SERVICE) task->debugDump(logger, __FUNCTION__);

	if (task->inputList.size() == 4) {
		Block block0 = task->inputList.at(0);
		Block block1 = task->inputList.at(1);
		Block block2 = task->inputList.at(2);
		Block block3 = task->inputList.at(3);

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
		task->inputList.removeFirst(); // 0
		task->inputList.removeFirst(); // 1
		task->inputList.removeFirst(); // 2
		task->inputList.removeFirst(); // 3
	}
	if (DEBUG_SHOW_STREAM_TCP_SERVICE) task->debugDump(logger, __FUNCTION__);

	return CoProcessorIOFaceGuam::R_completed;
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

	Task* task = Task::getInstance(iocb->mesaPut.hTask);
	if (DEBUG_SHOW_STREAM_TCP_SERVICE) task->debugDump(logger, __FUNCTION__);

	if (task->outputList.isEmpty()) {
		return CoProcessorIOFaceGuam::R_inProgress;
	} else {
		Block block = task->outputList.at(0);
		logger.debug("    block %d %s", task->inputList.size(), block.toHexString().toLocal8Bit().constData());

		block.get(&iocb->mesaGet);

		task->outputList.removeFirst();
		if (DEBUG_SHOW_STREAM_TCP_SERVICE) task->debugDump(logger, __FUNCTION__);
		return CoProcessorIOFaceGuam::R_completed;
	}
}



CARD32                                      StreamTcpService::SocketInfo::socketIDNext = 1;
QMap<CARD32, StreamTcpService::SocketInfo*> StreamTcpService::SocketInfo::map;

CARD16                                StreamTcpService::Task::hTaskNext = 1;
QMap<CARD32, StreamTcpService::Task*> StreamTcpService::Task::map;

StreamTcpService::SocketInfo* StreamTcpService::SocketInfo::getInstance(CARD32 socketID_) {
	if (socketID_ == 0) return new SocketInfo();
	if (map.contains(socketID_)) return map[socketID_];
	logger.fatal("socketID_ = %d", socketID_);
	ERROR();
	return 0;
}

StreamTcpService::Task* StreamTcpService::Task::getInstance(CARD16 hTask_) {
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
void StreamTcpService::Task::connect(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/) {
	Block block1 = inputList.at(1);
	Block block2 = inputList.at(2);
	Block block3 = inputList.at(3);

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
		Block status(Status::success);
		Block socketID(socketInfo->socketID);

		outputList.append(status);
		outputList.append(socketID);
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
	const CARD32 socketID = inputList.at(1).get32();
	const CARD32 timeout  = inputList.at(2).get32();
	logger.debug("%04d  Task::%-11s  socketID %4d  timeout %4d", hTask, __FUNCTION__, socketID, timeout);
	/*SocketInfo* socket =*/ SocketInfo::getInstance(socketID);
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
	const CARD32 socketID = inputList.at(1).get32();
	const CARD32 length   = inputList.at(2).get32();
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
void StreamTcpService::Task::get(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) {
	const CARD32 socketID = inputList.at(1).get32();
	const CARD32 length   = inputList.at(2).get32();
	logger.debug("%04d  Task::%-11s  socketID %4d  length %4d", hTask, __FUNCTION__, socketID, length);
	SocketInfo* socketInfo = SocketInfo::getInstance(socketID);

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
void StreamTcpService::Task::close(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/) {
	const CARD32 socketID = inputList.at(1).get32();
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
void StreamTcpService::Task::setWaitTime(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/) {
	const CARD32 socketID = inputList.at(1).get32();
	const CARD32 timeout  = inputList.at(2).get32();
	logger.debug("%04d  Task::%-11s  socketID %4d  timeout %4d", hTask, __FUNCTION__, socketID, timeout);
	SocketInfo* socket = SocketInfo::getInstance(socketID);
	socket->timeout = timeout;
	// TODO
	ERROR();
}

//    PutCommand[stream, endStream];
//    PutLongNumber[stream, LONG[0]];
//    PutLongNumber[stream, LONG[0]];
//    PutLongNumber[stream, LONG[0]];
void StreamTcpService::Task::endStream(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/) {
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
void StreamTcpService::Task::shutDown(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/) {
	const CARD32 socketID = inputList.at(1).get32();
	logger.debug("%04d  Task::%-11s  socketID %4d", hTask, __FUNCTION__, socketID);
	//SocketInfo* socket = SocketInfo::get(socketID);
	// TODO
	ERROR();
}


//    PutCommand[stream, reset];
//    PutLongNumber[stream, socketInfo.socketID];
//    PutLongNumber[stream, LONG[0]];
//    PutLongNumber[stream, LONG[0]];
void StreamTcpService::Task::reset(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/) {
	const CARD32 socketID = inputList.at(1).get32();
	logger.debug("%04d  Task::%-11s  socketID %4d", hTask, __FUNCTION__, socketID);
	//SocketInfo* socket = SocketInfo::get(socketID);
	// TODO
	ERROR();
}

void StreamTcpService::Task::debugDump(log4cpp::Category& logger, const char* name) {
	QString outList;
	QString inList;

	for(int i = 0; i < outputList.size(); i++) {
		const Block& block = outputList.at(i);
		if (i) outList.append(" ");
		outList.append(QString::number(block.getSize()));
	}
	for(int i = 0; i < inputList.size(); i++) {
		const Block& block = inputList.at(i);
		if (i) inList.append(" ");
		inList.append(QString::number(block.getSize()));
	}

	logger.debug("%04d  %-11s  inputList [%s]  outputList[%s]", hTask, name, inList.toLocal8Bit().constData(), outList.toLocal8Bit().constData());
}

//
// StreamTcpService::Block
//
CARD32 StreamTcpService::Block::get32() const {
	// Sanity check
	if (data.size() != sizeof(CARD32)) {
		logger.fatal("data.size() = %d", data.size());
		ERROR();
	}
	quint8 buffer[sizeof(CARD32)];
	for(size_t i = 0; i < sizeof(buffer); i++) buffer[i] = data.at(i);
	BigEndianByteBuffer bb(buffer, sizeof(buffer));
	return bb.get32(0);
}

void StreamTcpService::Block::put32(CARD32 value) {
	quint8 buffer[8];
	BigEndianByteBuffer bb(buffer, sizeof(buffer));
	bb.put32(value);
	for(quint32 i = 0; i < bb.getPos(); i++) data.append(buffer[i]);
}

// copy from TransferRec to Block
void StreamTcpService::Block::put(CoProcessorIOFaceGuam::TransferRec* mesaPut) {
	// Sanity check
	if (mesaPut->bytesWritten < mesaPut->bytesRead) {
		logger.fatal("bytesWritten = %d  bytesRead = %d", mesaPut->bytesWritten, mesaPut->bytesRead);
		ERROR();
	}

	BigEndianByteBuffer bb((CARD8*)Store(mesaPut->buffer), mesaPut->bytesWritten);
	// copy from bytesRead to bytesWritten
	bb.setPos(mesaPut->bytesRead);
	while(0 < bb.remaining()) data.append(bb.get8());
	// update bytesRead
	mesaPut->bytesRead = bb.getPos();
}

// copy from Block to TransferRec
void StreamTcpService::Block::get(CoProcessorIOFaceGuam::TransferRec* mesaGet) {
	// Sanity check
	const CARD32 dataSize       = (CARD32)data.size();
	const CARD32 bufferByteSize = mesaGet->bufferSize * Environment::bytesPerWord;
	const CARD32 bytesWritten   = mesaGet->bytesWritten;
	if (bufferByteSize < (bytesWritten + dataSize)) {
		logger.fatal("bufferByteSize = %d  bytesWritten = %d  dataSize = %d", bufferByteSize, bytesWritten, dataSize);
		ERROR();
	}

	BigEndianByteBuffer bb((CARD8*)Store(mesaGet->buffer), bufferByteSize);
	bb.setPos(mesaGet->bytesWritten); // Move to end
	for(int i = 0; i < data.size(); i++) bb.put8(data.at(i));
	mesaGet->bytesWritten = bb.getPos();
}
