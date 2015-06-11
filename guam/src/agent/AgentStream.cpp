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

const char* AgentStream::getCommandString(const CARD16 value) {
	static QMap<CARD16, const char*> map {
		{CoProcessorIOFaceGuam::C_idle,    "idle"},
		{CoProcessorIOFaceGuam::C_accept,  "accept"},
		{CoProcessorIOFaceGuam::C_connect, "connect"},
		{CoProcessorIOFaceGuam::C_delete,  "delete"},
		{CoProcessorIOFaceGuam::C_read,    "read"},
		{CoProcessorIOFaceGuam::C_write,   "write"},
	};

	if (map.contains(value)) return map[value];
	logger.fatal("value = %d", value);
	ERROR();
	return "UNKNONW";
}

const char* AgentStream::getStateString(const CARD16 value) {
	static QMap<CARD16, const char*> map {
		{CoProcessorIOFaceGuam::S_idle,      "idle"},
		{CoProcessorIOFaceGuam::S_accepting, "accepting"},
		{CoProcessorIOFaceGuam::S_connected, "connected"},
		{CoProcessorIOFaceGuam::S_deleted,   "deleted"},
	};

	if (map.contains(value)) return map[value];
	logger.fatal("value = %d", value);
	ERROR();
	return "UNKNONW";
}

const char* AgentStream::getResultString(const CARD16 value) {
	static QMap<CARD16, const char*> map {
		{CoProcessorIOFaceGuam::R_completed,  "completed"},
		{CoProcessorIOFaceGuam::R_inProgress, "inProgress"},
		{CoProcessorIOFaceGuam::R_error,      "error"},
	};

	if (map.contains(value)) return map[value];
	logger.fatal("value = %d", value);
	ERROR();
	return "UNKNONW";
}


const char* AgentStream::getServerIDString(const CARD32 value) {
	static QMap<CARD32, const char*> map {
		{CoProcessorServerIDs::fileAccess,                             "fileAccess"},
		{CoProcessorServerIDs::dragAndDropToGVService,                 "dragAndDrop"},
		{CoProcessorServerIDs::workspaceWindowControlGVService,        "wwc-gv"},
		{CoProcessorServerIDs::workspaceWindowControlMSWindowsService, "wwc-pc"},
		{CoProcessorServerIDs::tcpService,                             "tcpService"},
	};

	if (map.contains(value)) return map[value];
	static char buffer[20];
	sprintf(buffer, "ID-%d", value);
	return buffer;
}


void AgentStream::addHandler(AgentStream::Handler* handler) {
	const CARD32 serverID = handler->serverID;
	if (handlerMap.contains(serverID)) {
		logger.fatal("serverID = %5d  name = %s", serverID, handler->name.toLocal8Bit().constData());
		ERROR();
	}
	handlerMap[serverID] = handler;
}


void AgentStream::initialize() {
	addHandler(new StreamDefault());
	addHandler(new StreamTcpService());
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

	// Initialize AgentStream::Handler
	AgentStream::initialize();
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
		logger.debug("AGENT %s  head = %8X  next = %8X  command = %10s  result = %10s  interruptSelector = %04X", name, fcb->iocbHead, fcb->iocbNext, AgentStream::getCommandString(fcb->headCommand), AgentStream::getResultString(fcb->headResult), fcb->interruptSelector);
	}
	if (fcb->iocbHead) {
		CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb = (CoProcessorIOFaceGuam::CoProcessorIOCBType*)Store(fcb->iocbHead);
		const CARD32 serverID = iocb->serverID;

		AgentStream::Handler* handler = (handlerMap.contains(serverID)) ? handlerMap[serverID] : handlerMap[Handler::DEFAULT_ID];

		switch(fcb->headCommand) {
		case CoProcessorIOFaceGuam::C_idle: {
			fcb->headResult = handler->idle(iocb);
		}
			break;
		case CoProcessorIOFaceGuam::C_accept: {
			if (iocb->mesaPut.hTask == 0) {
				logger.fatal("mesaPut.hTask = %d", iocb->mesaPut.hTask);
				ERROR();
			}
			if (iocb->mesaGet.hTask == 0) {
				logger.fatal("mesaGet.hTask = %d", iocb->mesaGet.hTask);
				ERROR();
			}
			fcb->headResult = handler->accept(iocb);
		}
			break;
		case CoProcessorIOFaceGuam::C_connect: {
			if (iocb->mesaPut.hTask) {
				logger.fatal("mesaPut.hTask = %d", iocb->mesaPut.hTask);
				ERROR();
			}
			if (iocb->mesaGet.hTask) {
				logger.fatal("mesaGet.hTask = %d", iocb->mesaGet.hTask);
				ERROR();
			}
			Task* task = Task::getInstance();
			iocb->mesaPut.hTask  = iocb->mesaGet.hTask = task->hTask;
			//
			fcb->headResult = handler->connect(iocb);
		}
			break;
		case CoProcessorIOFaceGuam::C_delete: {
			if (iocb->mesaPut.hTask == 0) {
				logger.fatal("mesaPut.hTask = %d", iocb->mesaPut.hTask);
				ERROR();
			}
			if (iocb->mesaGet.hTask == 0) {
				logger.fatal("mesaGet.hTask = %d", iocb->mesaGet.hTask);
				ERROR();
			}
			//
			fcb->headResult = handler->destroy(iocb);
		}
			break;
		case CoProcessorIOFaceGuam::C_read: {
			if (iocb->mesaPut.hTask == 0) {
				logger.fatal("mesaPut.hTask = %d", iocb->mesaPut.hTask);
				ERROR();
			}
			if (iocb->mesaGet.hTask == 0) {
				logger.fatal("mesaGet.hTask = %d", iocb->mesaGet.hTask);
				ERROR();
			}
			//
			fcb->headResult = handler->read(iocb);
		}
			break;
		case CoProcessorIOFaceGuam::C_write: {
			if (iocb->mesaPut.hTask == 0) {
				logger.fatal("mesaPut.hTask = %d", iocb->mesaPut.hTask);
				ERROR();
			}
			if (iocb->mesaGet.hTask == 0) {
				logger.fatal("mesaGet.hTask = %d", iocb->mesaGet.hTask);
				ERROR();
			}
			//
			fcb->headResult = handler->write(iocb);
		}
			break;
		default:
			logger.fatal("headCommand = %d", fcb->headCommand);
			ERROR();
		}

	}
}


void AgentStream::Handler::debugDump(log4cpp::Category& logger, const char* name, CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) {
	logger.debug("%s", name);
	logger.debug("    serverID = %-11s  mesaIsServer = %d  mesaState = %-10s  pcState = %-10s",
		AgentStream::getServerIDString(iocb->serverID), iocb->mesaIsServer, AgentStream::getStateString(iocb->mesaConnectionState), AgentStream::getStateString(iocb->pcConnectionState));
	logger.debug("        put  sst = %3d  u2 = %02X  write = %4d  read = %4d  hTask = %d  interrupt = %d  writeLocked = %d",
		iocb->mesaPut.subSequence, iocb->mesaPut.u2, iocb->mesaPut.bytesWritten, iocb->mesaPut.bytesRead, iocb->mesaPut.hTask, iocb->mesaPut.interruptMesa, iocb->mesaPut.writeLockedByMesa);
	logger.debug("        get  sst = %3d  u2 = %02X  write = %4d  read = %4d  hTask = %d  interrupt = %d  writeLocked = %d",
		iocb->mesaGet.subSequence, iocb->mesaGet.u2, iocb->mesaGet.bytesWritten, iocb->mesaGet.bytesRead, iocb->mesaGet.hTask, iocb->mesaGet.interruptMesa, iocb->mesaGet.writeLockedByMesa);
}
