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

void StreamTCP::removeSocket(SocketInfo* socketInfo) {
	const CARD32 socketID = socketInfo->socketID;
	if (socketMap.contains(socketID)) {
		socketMap.remove(socketID);
		delete socketInfo;
	} else {
		logger.fatal("socketID = %d", socketID);
		ERROR();
	}
}



CARD32                               StreamTCP::SocketInfo::socketIDNext = 0;
QMap<CARD32, StreamTCP::SocketInfo*> StreamTCP::socketMap;


StreamTCP::StreamTCP() : AgentStream::Handler(CoProcessorServerIDs::tcpService, "tcp") {}

void StreamTCP::idle   (AgentStream::FCB* /*fcb*/) {
}

AgentStream::Result StreamTCP::accept (AgentStream::IOCB* /*iocb*/, AgentStream::Task* /*task*/) {
	return AgentStream::Result::error;
}
AgentStream::Result StreamTCP::connect(AgentStream::IOCB* /*iocb*/, AgentStream::Task* /*task*/) {
	return AgentStream::Result::completed;
}
AgentStream::Result StreamTCP::destroy(AgentStream::IOCB* /*iocb*/, AgentStream::Task* /*task*/) {
	return AgentStream::Result::error;
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
			case MsgID::connect:
				connect(arg1, arg2, arg3);
				break;
			case MsgID::get:
				get(arg1, arg2, arg3);
				break;
			case MsgID::shutDown:
				shutDown(arg1, arg2, arg3);
				break;
			case MsgID::close:
				close(arg1, arg2, arg3);
				break;
			case MsgID::put:
				put(arg1, arg2, arg3);
				break;

			case MsgID::listen:
			case MsgID::setWaitTime:
			case MsgID::endStream:
			case MsgID::reset:
				ERROR();
				break;
			default: {
				logger.fatal("msg = %d", msg);
				ERROR();
			}
			}
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
void StreamTCP::connect(const CARD32 arg1, const CARD32 arg2, const CARD32 arg3) {
	const CARD32 remote = arg1;
	const CARD32 rPort  = arg2;
	const CARD32 lPort  = arg3;

	SocketInfo* socketInfo = new SocketInfo;
	// Swap high low word for mesa byte order for long value
	socketInfo->remoteAddress = ((remote >> 16) & 0x0000FFFF) | ((remote << 16) & 0xFFFF0000);
	socketInfo->remotePort    = rPort;
	socketInfo->localAddress  = 0;
	socketInfo->localPort     = lPort;
	socketInfo->timeout       = 0;

	logger.debug("    connect  %04d  %08X:%d  localPort %d",
			socketInfo->socketID, socketInfo->remoteAddress, socketInfo->remotePort, socketInfo->localPort);

	SocketStream::SockAddress localAddress(SocketStream::INADDR_ANY, socketInfo->localPort);
	SocketStream::SockAddress remoteAddress(socketInfo->remoteAddress, socketInfo->remotePort);
	socketInfo->socket.bind(localAddress);
	socketInfo->socket.connect(remoteAddress);

	// Output response
	putData32(Status::success);
	putData32(socketInfo->socketID);
}


//    PutCommand[stream, get];
//    PutLongNumber[stream, socketInfo.socketID];
//    PutLongNumber[stream, length];
//    PutLongNumber[stream, LONG[0]];
//    ----
//    status _ GetStatusXX[stream, length, 10000];'
//	  IF status = success THEN {
//  	  dataLen: LONG CARDINAL _ GetLongNumber[stream];
//
//  	  IF dataLen # 0 THEN {
//  		Stream.SetInputOptions[stream, [terminateOnEndRecord: TRUE]];
//  		-- block.stopIndexPlusOne _ Inline.LowHalf[dataLen];
//  		[byteCount, why, ] _ Stream.GetBlock[stream, block];
//  	  }
//  	  ELSE {
//  		RETURN[0, closing];
//  	  }
//  	}
//  	ELSE {
//  	  reason _ GetReason[stream];
//  	  wsErr _ LOOPHOLE[reason - 10000];
//  	  IF socketInfo.state = finWait THEN  -- for PC NFS
//  		RETURN[0, closing];
//  	  IF wsErr = WSAETIMEDOUT THEN
//  		completionCode _ timeout
//  	  ELSE {
//  		ProcessReason[reason ! TcpStream.Failed => CONTINUE];
//  	TcpStream.Suspended[reset];
//  	  }
//  	};
static const int WAIT_TIME_IN_MILLI = 100;
void StreamTCP::get(const CARD32 arg1, const CARD32 arg2, const CARD32 arg3) {
	// Sanity check
	if (arg3 != 0) ERROR();

	const CARD32 socketID = arg1;
	const CARD32 length   = arg2;

	SocketInfo* socketInfo = getSocket(socketID);
	if (socketInfo->socket.select(WAIT_TIME_IN_MILLI)) {
		char buffer[2000];
		if (sizeof(buffer) < length) ERROR();

		int size = socketInfo->socket.read(buffer, length);

		QByteArray data;
		for(int i = 0; i < size; i++) data.append(buffer[i]);
		const quint32 dataSize = (quint32)data.size();
		logger.debug("    get      %04d  %3d / %3d", socketInfo->socketID, dataSize, length);

		// endRecord == TRUE
		AgentStream::StreamData streamData(data, 0, false, true, false);

		// Output response
		putData32(Status::success);
		putData32((quint32)streamData.data.size());
		putData(streamData);
	} else {
		logger.debug("    get      %04d  %3d / %3d", socketInfo->socketID, 0, length);

		// To indicate end of stream, return Status::success with zero length

		// Output response
		putData32(Status::failure);
		putData32(WSAETIMEDOUT);
	}
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
void StreamTCP::shutDown(const CARD32 arg1, const CARD32 arg2, const CARD32 arg3) {
	// Sanity check
	if (arg2 != 1) ERROR();
	if (arg3 != 0) ERROR();

	const CARD32 serverID = arg1;
	const int    how      = (int)arg2;

	SocketInfo* socketInfo = getSocket(serverID);
	socketInfo->socket.shutdown(how);

	logger.debug("    shutDown %04d  %d", socketInfo->socketID, how);

	// Output response
	putData32(Status::success);
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
void StreamTCP::close(const CARD32 arg1, const CARD32 arg2, const CARD32 arg3) {
	// Sanity check
	if (arg2 != 0) ERROR();
	if (arg3 != 0) ERROR();

	const CARD16 socketID = arg1;

	SocketInfo* socketInfo = getSocket(socketID);
	socketInfo->socket.close();
	removeSocket(socketInfo);

	// Output response
	putData32(Status::success);
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
void StreamTCP::put(const CARD32 arg1, const CARD32 arg2, const CARD32 arg3) {
	// Sanity check
	if (arg3 != 0) ERROR();

	const CARD16 socketID = arg1;
	const CARD32 length   = arg2;

	AgentStream::StreamData streamData  = getData();
	const QByteArray&       data        = streamData.data;
	SocketInfo*             socketInfo  = getSocket(socketID);
	int                     actualWrite = socketInfo->socket.write(data.data(), data.size());
	logger.debug("    put      %04d  %3d / %3d / %3d", socketInfo->socketID, actualWrite, length, data.size());

	// Output response
	putData32(Status::success);
	putData32(actualWrite);
}
