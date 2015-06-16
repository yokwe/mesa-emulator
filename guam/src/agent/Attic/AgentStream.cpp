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
#include "../util/ByteBuffer.h"

#include "../mesa/Pilot.h"
#include "../mesa/Memory.h"
#include "../mesa/MesaThread.h"

#include "Agent.h"
#include "AgentStream.h"

#include "StreamDefault.h"
#include "StreamTcpService.h"

#define DEBUG_SHOW_AGENT_STREAM 1


//
// AgentStream
//

QMap<CARD32, AgentStream::Handler*> AgentStream::handlerMap;
AgentStream::Handler*               AgentStream::defaultHandler = 0;
QMap<CARD16, AgentStream::Task*>    AgentStream::taskMap;
CARD16                              AgentStream::interruptSelector = 0;


void AgentStream::setDefaultHandler(AgentStream::Handler* handler) {
	if (defaultHandler != 0) ERROR();
	defaultHandler = handler;
}

void AgentStream::addHandler(AgentStream::Handler* handler) {
	const CARD32 serverID = handler->serverID;
	if (handlerMap.contains(serverID)) {
		//logger.fatal("serverID = %5d  name = %s", serverID, handler->name.toLocal8Bit().constData());
		for(AgentStream::Handler* myHandler: handlerMap) {
			logger.fatal("serverID = %5d  name = %s", myHandler->serverID, myHandler->name.toLocal8Bit().constData());
		}
		ERROR();
	}
	handlerMap[serverID] = handler;
}

AgentStream::Handler* AgentStream::getHandler(CARD32 serverID) {
	return handlerMap.contains(serverID) ? handlerMap[serverID] : defaultHandler;
}

void AgentStream::addTask(Task* task) {
	const CARD32 hTask = task->hTask;
	if (taskMap.contains(hTask)) {
		logger.fatal("hTask = %5d", hTask);
		ERROR();
	}
	taskMap[hTask] = task;
}
AgentStream::Task* AgentStream::getTask(CARD16 hTask) {
	if (taskMap.contains(hTask)) return taskMap[hTask];
	logger.fatal("hTask = %d", hTask);
	ERROR();
	return 0;
}

void AgentStream::notifyInterrupt() {
	InterruptThread::notifyInterrupt(interruptSelector);
}

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
	AgentStream::Handler::initialize();
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
			interruptSelector = fcb->interruptSelector;
		}
		fcb->agentStopped = 0;
	}

	if (fcb->iocbHead == 0) {
		if (DEBUG_SHOW_AGENT_STREAM) logger.debug("AGENT %s  fcb->iocbHead == 0", name);
		return; // Return if there is no IOCB
	}

	if (DEBUG_SHOW_AGENT_STREAM) {
		logger.debug("AGENT %s  head = %8X  command = %10s  result = %10s  interrupt = %04X", name, fcb->iocbHead, AgentStream::getCommandString(fcb->headCommand), AgentStream::getResultString(fcb->headResult), fcb->interruptSelector);
	}
	if (fcb->iocbHead) {
		CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb = (CoProcessorIOFaceGuam::CoProcessorIOCBType*)Store(fcb->iocbHead);
		AgentStream::Handler* handler = Handler::getHandler(iocb->serverID);

		if (DEBUG_SHOW_AGENT_STREAM) {
			const char* headCommandString = getCommandString(fcb->headCommand);
			Handler::debugDump(logger, headCommandString, iocb);
		}

		Handler::ResultType headResult;
		switch(fcb->headCommand) {
		case CoProcessorIOFaceGuam::C_idle: {
			headResult = handler->idle(iocb);
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
			headResult = handler->accept(iocb);
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
			Task* task = handler->createTask();
			iocb->mesaPut.hTask = iocb->mesaGet.hTask = task->hTask;
			iocb->pcConnectionState = CoProcessorIOFaceGuam::S_connected;

			if (DEBUG_SHOW_AGENT_STREAM) task->debugDump(logger);
			//
			headResult = handler->connect(iocb);
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
			headResult = handler->destroy(iocb);
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
			headResult = handler->read(iocb);
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
			headResult = handler->write(iocb);
		}
			break;
		default:
			logger.fatal("headCommand = %d", fcb->headCommand);
			ERROR();
		}

		switch(headResult) {
		case Handler::ResultType::completed:
		case Handler::ResultType::inProgress:
		case Handler::ResultType::error:
			fcb->headResult = static_cast<CARD16>(headResult);
			break;
		default: {
			logger.fatal("result = %d", static_cast<CARD16>(headResult));
			ERROR();
		}
		}

		if (DEBUG_SHOW_AGENT_STREAM) {
			const char* headResultString = getResultString(fcb->headResult);
			logger.debug("    result %s", headResultString);
		}
	}
}


//
// AgentStream::Task
//

CARD16 AgentStream::Task::hTaskNext = 0;

void AgentStream::Task::debugDump(log4cpp::Category& logger) {
	QString readMessage;
	QString writeMessage;

	for(int i = 0; i < readList.size(); i++) {
		const AgentStream::Block& block = readList.at(i);
		if (i) readMessage.append(" ");

		readMessage.append(block.toEscapedString());
	}
	for(int i = 0; i < writeList.size(); i++) {
		const AgentStream::Block& block = writeList.at(i);
		if (i) writeMessage.append(" ");

		writeMessage.append(block.toEscapedString());
	}

	logger.debug("        %04d  readList[%s]  writeList[%s]", hTask, readMessage.toLocal8Bit().constData(), writeMessage.toLocal8Bit().constData());
}


//
// AgentStream::Handler
//

AgentStream::Handler::~Handler() {
	//
}
void AgentStream::Handler::initialize() {
	setDefaultHandler(new StreamDefault());
	addHandler(new StreamTcpService());
}

AgentStream::Handler* AgentStream::Handler::getHandler(CARD32 serverID) {
	if (defaultHandler == 0) ERROR();
	return (handlerMap.contains(serverID)) ? handlerMap[serverID] : defaultHandler;
}

void AgentStream::Handler::debugDump(log4cpp::Category& logger, const char* name, CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) {
	logger.debug("%s", name);
	logger.debug("    serverID = %-11s  mesaIsServer = %d  mesaState = %-10s  pcState = %-10s",
		AgentStream::getServerIDString(iocb->serverID), iocb->mesaIsServer, AgentStream::getStateString(iocb->mesaConnectionState), AgentStream::getStateString(iocb->pcConnectionState));

	if (iocb->mesaPut.bytesWritten != iocb->mesaPut.bytesRead) {
		logger.debug("        put  sst = %3d  u2 = %02X  write = %4d  read = %4d  interrupt = %d  writeLocked = %d",
			iocb->mesaPut.subSequence, iocb->mesaPut.u2, iocb->mesaPut.bytesWritten, iocb->mesaPut.bytesRead, iocb->mesaPut.interruptMesa, iocb->mesaPut.writeLockedByMesa);
	}

	if (iocb->mesaGet.bytesWritten || iocb->mesaGet.bytesRead || iocb->mesaGet.interruptMesa) {
		logger.debug("        get  sst = %3d  u2 = %02X  write = %4d  read = %4d  interrupt = %d  writeLocked = %d",
			iocb->mesaGet.subSequence, iocb->mesaGet.u2, iocb->mesaGet.bytesWritten, iocb->mesaGet.bytesRead, iocb->mesaGet.interruptMesa, iocb->mesaGet.writeLockedByMesa);
	}

	if (iocb->mesaGet.hTask) {
		AgentStream::Task* task = AgentStream::getTask(iocb->mesaGet.hTask);
		task->debugDump(logger);
	}
}

//
// Block
//

AgentStream::Block::Block(const QByteArray& data) {
	this->data = data;
}

CARD32 AgentStream::Block::get32() const {
	// Sanity check
	if (data.size() != sizeof(CARD32)) {
		logger.fatal("data.size() = %d", data.size());
		ERROR();
	}
	quint8 buffer[sizeof(CARD32)];
	for(size_t i = 0; i < sizeof(buffer); i++) buffer[i] = data.at(i);
	BigEndianByteBuffer bb(buffer, sizeof(buffer));
	CARD32 ret = bb.get32();
	return ret;
}

void AgentStream::Block::put32(CARD32 value) {
	quint8 buffer[sizeof(CARD32)];
	BigEndianByteBuffer bb(buffer, sizeof(buffer));
	bb.put32(value);
	for(quint32 i = 0; i < sizeof(buffer); i++) data.append(buffer[i]);
}

// copy from TransferRec to AgentStream::Block
void AgentStream::Block::put(CoProcessorIOFaceGuam::TransferRec* mesaPut) {
	// Sanity check
	if (mesaPut->bytesWritten < mesaPut->bytesRead) {
		logger.fatal("bytesWritten = %d  bytesRead = %d", mesaPut->bytesWritten, mesaPut->bytesRead);
		ERROR();
	}

	// Copy data from mesaPut->buffer[mesaPut->bytesRead..mesaPut->bytesWritten) to data
	data.clear();
	char* buffer = (char*)Store(mesaPut->buffer);
	for(CARD32 i = mesaPut->bytesRead; i < mesaPut->bytesWritten; i++) {
		data.append(buffer[i]);
	}
	mesaPut->bytesRead = mesaPut->bytesWritten;
}

// copy from AgentStream::Block to TransferRec
void AgentStream::Block::get(CoProcessorIOFaceGuam::TransferRec* mesaGet) {
	// Sanity check
	const int dataSize       = (CARD32)data.size();
	const int bufferByteSize = mesaGet->bufferSize * Environment::bytesPerWord;
	const int bytesWritten   = mesaGet->bytesWritten;
	if (bufferByteSize < (bytesWritten + dataSize)) {
		logger.fatal("bufferByteSize = %d  bytesWritten = %d  dataSize = %d", bufferByteSize, bytesWritten, dataSize);
		ERROR();
	}

	// Copy data from data to mesaGet->bufer[mesaGet->bytesWritten..mesaGet->bytesWritten + data.size())
	char* buffer = (char*)Store(mesaGet->buffer);
	for(int i = 0; i < dataSize; i++) {
		buffer[mesaGet->bytesWritten + i] = data.at(i);
	}
	mesaGet->bytesWritten += dataSize;
}

char AgentStream::Block::at(int i) const {
	LittleEndianByteBuffer bb((CARD8*)data.data(), data.size());
	return bb.get8(i);
}

QString AgentStream::Block::toEscapedString() const {
	const int size = getSize();
	// Special for 4 byte data
	if (size == 4) {
		const CARD32 value = get32();
		if ((CARD8)value < 'A' || 'Z' < (CARD8)value) {
			return QString::number(get32());
		}
	}
	{
		QString ret;
		ret.append(QString("(%1)\"").arg(size));
		for(int j = 0; j < size; j++) {
			unsigned char c = at(j);
			if (c == 0x0a) {
				ret.append("\\n");
			} else if (c == 0x0d) {
				ret.append("\\r");
			} else if (c < 0x20 || 0x7e < c) {
				ret.append(QString("\\x%1").arg((int)c, 2, 16, QChar('0')));
			} else {
				ret.append(QChar(c));
			}
		}
		ret.append("\"");
		return ret;
	}
}
