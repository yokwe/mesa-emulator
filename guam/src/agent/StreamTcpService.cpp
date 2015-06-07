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

#include "StreamTcpService.h"

#define DEBUG_SHOW_STREAM_TCP_SERVICE 1

StreamTcpService::StreamTcpService() : AgentStream::Stream(CoProcessorServerIDs::tcpService, "TcpService") {}

const char* StreamTcpService::getMsgIdString(CARD32 msgId) {
	switch(msgId) {
	case M_connect:     return "connect";
	case M_listen:      return "listen";
	case M_put:         return "put";
	case M_get:         return "get";
	case M_close:       return "close";
	case M_setWaitTime: return "setWaitTime";
	case M_endStream:   return "endStream";
	case M_shutDown:    return "shutDown";
	case M_reset:       return "reset";
	default:
		logger.fatal("msgId = %d", msgId);
		ERROR();
	}
	return "UNKNOWN";
}

const char* StreamTcpService::getStatusString(CARD32 status) {
	switch(status) {
	case S_success: return "success";
	case S_failure: return "failure";
	default:
		logger.fatal("status = %d", status);
		ERROR();
	}
	return "UNKNOWN";
}

// State: TYPE = {closed, established, finWait, closing};
const char* StreamTcpService::getStateString(CARD32 state) {
	switch(state) {
	case S_closed:      return "closed";
	case S_established: return "established";
	case S_finWait:     return "finWait";
	case S_closing:     return "closing";
	default:
		logger.fatal("state = %d", state);
		ERROR();
	}
	return "UNKNOWN";
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
		iocb->pcConnectionState = CoProcessorIOFaceGuam::S_accepting;
		fcb->headResult         = CoProcessorIOFaceGuam::R_completed;
		break;
	case CoProcessorIOFaceGuam::C_accept:
		ERROR();
		break;
	case CoProcessorIOFaceGuam::C_connect:
	{
		Task* task = new Task();
		iocb->pcConnectionState = CoProcessorIOFaceGuam::S_connected;
		iocb->mesaPut.hTask = task->hTask; // Need to be set not zero to enable actual put operation
		iocb->mesaGet.hTask = task->hTask; // Need to be set not zero to enable actual get operation
		fcb->headResult = CoProcessorIOFaceGuam::R_completed;
	}
		break;
	case CoProcessorIOFaceGuam::C_delete:
		ERROR();
		break;
	case CoProcessorIOFaceGuam::C_read:
	{
		Task* task = Task::get(iocb->mesaPut.hTask);
		iocb->mesaPut.hTask = iocb->mesaGet.hTask = task->hTask;

		BigEndianByteBuffer  bb((CARD8*)Store(iocb->mesaPut.buffer), iocb->mesaPut.bytesWritten);
		CARD32 message = bb.get32();
		task->addMessage(message);
	}
		break;
	case CoProcessorIOFaceGuam::C_write:
		iocb->mesaPut.hTask = 1; // Need to be set not zero to enable actual put operation
		ERROR();
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

void StreamTcpService::Task::addMessage(CARD32 message_) {
	logger.debug("addMessage %d %8X", messageSize, message_);

	message[messageSize++] = message_;

	if (messageSize == MESSAGE_SIZE) {
		switch(message[0]) {
		case M_connect:
			connect();
			break;
		case M_listen:
			listen();
			break;
		case M_put:
			put();
			break;
		case M_get:
			get();
			break;
		case M_close:
			close();
			break;
		case M_setWaitTime:
			setWaitTime();
			break;
		case M_endStream:
			endStream();
			break;
		case M_shutDown:
			shutDown();
			break;
		case M_reset:
			reset();
			break;
		default:
			logger.fatal("message[0] = %d", message[0] - MESSAGE_OFFSET);
			ERROR();
		}
		reset();
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
void StreamTcpService::Task::connect() {
	CARD32 remoteAddress = message[1];
	CARD32 remotePort    = message[2];
	CARD32 localPort     = message[3];
	logger.info("connect  %4d  %08X  %5d  %5d", hTask, remoteAddress, remotePort, localPort);
	/*SocketInfo* socket = */ SocketInfo::get(0);
	// TODO
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
void StreamTcpService::Task::listen() {
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
void StreamTcpService::Task::put() {
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
void StreamTcpService::Task::get() {
	CARD32 socketID = message[1];
	CARD32 length   = message[2];
	logger.info("get  %4d  %4d  %4d", hTask, socketID, length);
	//SocketInfo* socket = SocketInfo::get(socketID);
	// TODO
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
void StreamTcpService::Task::close() {
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
void StreamTcpService::Task::setWaitTime() {
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
void StreamTcpService::Task::endStream() {
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
void StreamTcpService::Task::shutDown() {
	CARD32 socketID = message[1];
	logger.info("shutDown  %4d  %4d", hTask, socketID);
	//SocketInfo* socket = SocketInfo::get(socketID);
	// TODO
}


//    PutCommand[stream, reset];
//    PutLongNumber[stream, socketInfo.socketID];
//    PutLongNumber[stream, LONG[0]];
//    PutLongNumber[stream, LONG[0]];
void StreamTcpService::Task::reset() {
	CARD32 socketID = message[1];
	logger.info("reset  %4d  %4d", hTask, socketID);
	//SocketInfo* socket = SocketInfo::get(socketID);
	// TODO
}

