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
// AgentStream.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("agentstream");


#include "../util/Debug.h"

#include "../mesa/Pilot.h"
#include "../mesa/Memory.h"

#include "Agent.h"
#include "AgentStream.h"

#include "StreamDefault.h"
#include "StreamTcpService.h"

#define DEBUG_SHOW_AGENT_STREAM 0

QMap<CARD32, AgentStream::Stream*>AgentStream::Stream::map;

static QMap<CARD16, const char*> commandStringMap {
	{CoProcessorIOFaceGuam::C_idle,    "idle"},
	{CoProcessorIOFaceGuam::C_accept,  "accept"},
	{CoProcessorIOFaceGuam::C_connect, "connect"},
	{CoProcessorIOFaceGuam::C_delete,  "delete"},
	{CoProcessorIOFaceGuam::C_read,    "read"},
	{CoProcessorIOFaceGuam::C_write,   "write"},
};
const char* AgentStream::Stream::getCommandString(const CARD16 command) {
	if (commandStringMap.contains(command)) {
		return commandStringMap[command];
	}
	logger.fatal("command = %d", command);
	ERROR();
	return "UNKNONW";
}

static QMap<CARD16, const char*> stateStringMap {
	{CoProcessorIOFaceGuam::S_idle,      "idle"},
	{CoProcessorIOFaceGuam::S_accepting, "accepting"},
	{CoProcessorIOFaceGuam::S_connected, "connected"},
	{CoProcessorIOFaceGuam::S_deleted,   "deleted"},
};
const char* AgentStream::Stream::getStateString(const CARD16 state) {
	if (stateStringMap.contains(state)) {
		return stateStringMap[state];
	}
	logger.fatal("state = %d", state);
	ERROR();
	return "UNKNONW";
}

static QMap<CARD16, const char*> resultStringMap {
	{CoProcessorIOFaceGuam::R_completed,  "completed"},
	{CoProcessorIOFaceGuam::R_inProgress, "inProgress"},
	{CoProcessorIOFaceGuam::R_error,      "error"},
};
const char* AgentStream::Stream::getResultString(const CARD16 result) {
	if (resultStringMap.contains(result)) {
		return resultStringMap[result];
	}
	logger.fatal("result = %d", result);
	ERROR();
	return "UNKNONW";
}


static QMap<CARD32, const char*> serverIDStringMap {
	{CoProcessorServerIDs::fileAccess,                             "fileAccess"},
	{CoProcessorServerIDs::dragAndDropToGVService,                 "dragAndDrop"},
	{CoProcessorServerIDs::workspaceWindowControlGVService,        "wwc-gv"},
	{CoProcessorServerIDs::workspaceWindowControlMSWindowsService, "wwc-pc"},
	{CoProcessorServerIDs::tcpService,                             "tcpService"},
};
const char* AgentStream::Stream::getServerIDString(const CARD32 serverID) {
	if (serverIDStringMap.contains(serverID)) {
		return serverIDStringMap[serverID];
	}
	static char buffer[20];
	sprintf(buffer, "ID-%d", serverID);
	return buffer;
}


void AgentStream::Stream::addHandler(AgentStream::Stream* handler) {
	const CARD32 serverID = handler->serverID;
	if (map.contains(serverID)) {
		logger.fatal("serverID = %5d  name = %s", serverID, handler->name.toLocal8Bit().constData());
		ERROR();
	}
	map[serverID] = handler;
}
CARD16 AgentStream::Stream::processRequest(CoProcessorIOFaceGuam::CoProcessorFCBType* fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) {
	const CARD32 serverID = iocb->serverID;
	AgentStream::Stream* handler = 0;
	if (map.contains(serverID)) {
		handler = map[serverID];
	} else {
		const CARD32 defaultServerID = DEFAULT_SERVER_ID;
		handler = map[defaultServerID];
	}
	return handler->process(fcb, iocb);
}


void AgentStream::Stream::initialize() {
	AgentStream::Stream::addHandler(new StreamDefault());
	AgentStream::Stream::addHandler(new StreamTcpService());
}

void AgentStream::Initialize() {
	if (fcbAddress == 0) ERROR();

	fcb = (CoProcessorIOFaceGuam::CoProcessorFCBType *)Store(fcbAddress);
	fcb->iocbHead          = 0;
	fcb->iocbNext          = 0;
	fcb->headCommand       = 0;
	fcb->filler5           = 0;
	fcb->headResult        = 0;
	fcb->filler7           = 0;
	fcb->interruptSelector = 0;
	fcb->stopAgent         = 0;
	fcb->agentStopped      = 1;
	fcb->streamWordSize    = 0;

	// Initialize AgentStream::Stream
	AgentStream::Stream::initialize();
}

void AgentStream::Call() {
	fcb->headResult = CoProcessorIOFaceGuam::R_completed;

	if (fcb->stopAgent) {
		if (!fcb->agentStopped) {
			logger.info("AGENT %s stop", name);
		}
		fcb->agentStopped = 1;
		// Do nothing when ask to stop
		return;
	} else {
		if (fcb->agentStopped) {
			logger.info("AGENT %s start  %04X", name, fcb->interruptSelector);
		}
		fcb->agentStopped = 0;
	}

	if (fcb->iocbHead == 0) {
		if (DEBUG_SHOW_AGENT_STREAM) logger.debug("AGENT %s  fcb->iocbHead == 0", name);
		return; // Return if there is no IOCB
	}

	if (DEBUG_SHOW_AGENT_STREAM) {
		logger.debug("AGENT %s  head = %8X  next = %8X  command = %10s  result = %10s  interruptSelector = %04X", name, fcb->iocbHead, fcb->iocbNext, AgentStream::Stream::getCommandString(fcb->headCommand), AgentStream::Stream::getResultString(fcb->headResult), fcb->interruptSelector);
	}
	if (fcb->iocbHead) {
		CoProcessorIOFaceGuam::CoProcessorIOCBType* iocbHead = (CoProcessorIOFaceGuam::CoProcessorIOCBType*)Store(fcb->iocbHead);
		// Process just iocbHead
		fcb->headResult = AgentStream::Stream::processRequest(fcb, iocbHead);

//		CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb = iocbHead;
//		for(;;) {
//			if (iocb->nextIOCB == 0) break;
//			iocb = (CoProcessorIOFaceGuam::CoProcessorIOCBType*)Store(iocb->nextIOCB);
//			AgentStream::Stream::processRequest(fcb, iocb);
//		}

	}
}
