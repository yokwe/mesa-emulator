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
// StreamTCP.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("streamtcp");

#include "StreamTCP.h"

const char* StreamTCP::getMsgIDString(MsgID value) {
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

const char* StreamTCP::getStatusString(Status value) {
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
const char* StreamTCP::getStateString(State value) {
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


StreamTCP::SocketInfo* StreamTCP::getSocket(CARD32 socketID) {
	if (socketMap.contains(socketID)) return socketMap[socketID];
	logger.fatal("socketID = %d", socketID);
	ERROR();
	return 0;
}


CARD32                               StreamTCP::SocketInfo::socketIDNext = 0;
QMap<CARD32, StreamTCP::SocketInfo*> StreamTCP::socketMap;


StreamTCP::StreamTCP() : AgentStream::Handler(CoProcessorServerIDs::tcpService, "tcp") {}

void StreamTCP::idle   (CoProcessorIOFaceGuam::CoProcessorFCBType* /*fcb*/) {
}

CARD16 StreamTCP::accept (CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/, AgentStream::Task* /*task*/) {
	return CoProcessorIOFaceGuam::R_error;
}
CARD16 StreamTCP::connect(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/, AgentStream::Task* /*task*/) {
	return CoProcessorIOFaceGuam::R_completed;
}
CARD16 StreamTCP::destroy(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/, AgentStream::Task* /*task*/) {
	return CoProcessorIOFaceGuam::R_error;
}

void StreamTCP::run() {
	logger.info("StreamTCP::run START");

	stopThread = 0;
	QThread::currentThread()->setPriority(PRIORITY);
	int processCount = 0;

	try {
		for(;;) {
			if (stopThread) break;

			CARD32 msg  = getData32();
			CARD32 arg1 = getData32();
			CARD32 arg2 = getData32();
			CARD32 arg3 = getData32();

			MsgID msgID = static_cast<MsgID>(msg);
			logger.debug("MsgID  %-8s  %5d  %5d  %5d", getMsgIDString(msgID), arg1, arg2, arg3);
			switch(msgID) {
			case MsgID::connect: {
				connect(arg1, arg2, arg3);
			}
				break;
			case MsgID::listen:
			case MsgID::put:
			case MsgID::get:
			case MsgID::close:
			case MsgID::setWaitTime:
			case MsgID::endStream:
			case MsgID::shutDown:
			case MsgID::reset:
				break;
			default: {
				logger.fatal("msg = %d", msg);
				ERROR();
			}
			}

			putData32(Status::success);
			putData32(WSEReason::WSAEACCES);

		}
	} catch(AgentStream::Handler::StopThreadException&) {
		//
	}

	logger.info("processCount           = %8u", processCount);
	logger.info("StreamTCP::run STOP");
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
void StreamTCP::connect(CARD32 remote, CARD32 remotePort, CARD32 localPort) {
	const QString remoteAddress = AgentStream::Data::toIPAddress(remote);

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
	putData32(Status::success);
	putData32(socketInfo->socketID);

	logger.debug("    connect  %4d", socketInfo->socketID);
}
