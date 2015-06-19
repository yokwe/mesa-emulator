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

#include "../mesa/Memory.h"
#include "../mesa/MesaThread.h"

#include "../util/ByteBuffer.h"

#include "Agent.h"
#include "AgentStream.h"

#include "Stream.h"
#include "StreamTCP.h"


#define DEBUG_SHOW_AGENT_STREAM 1

CARD16 AgentStream::Task::hTaskNext = 1; // Start from one

CoProcessorIOFaceGuam::CoProcessorFCBType* AgentStream::fcb = 0;
AgentStream::Handler*                      AgentStream::defaultHandler = 0;
QMap<CARD32, AgentStream::Handler*>        AgentStream::handlerMap;
QMap<CARD32, AgentStream::Task*>           AgentStream::taskMap;
bool                                       AgentStream::Handler::stopThread = false;

class DefaultHandler : public AgentStream::Handler {
public:
	DefaultHandler() : AgentStream::Handler(0, "default") {
		setAutoDelete(false);
	}

	AgentStream::Task* createTask() {
		return new AgentStream::Task();
	}
	void idle   (CoProcessorIOFaceGuam::CoProcessorFCBType* /*fcb*/) {
	}
	CARD16 accept (CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/, AgentStream::Task* /*task*/) {
		return CoProcessorIOFaceGuam::R_error;
	}
	CARD16 connect(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/, AgentStream::Task* /*task*/) {
		return CoProcessorIOFaceGuam::R_error;
	}
	CARD16 destroy(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/, AgentStream::Task* /*task*/) {
		return CoProcessorIOFaceGuam::R_error;
	}

	void run() {
		logger.info("AgentDisk::DefaultHandler::run START");

		stopThread = 0;
		QThread::currentThread()->setPriority(PRIORITY);
		int processCount = 0;

		for(;;) {
			if (stopThread) break;

			bool hasData = dataRead.waitData(WAIT_INTERVAL);
			if (hasData) {
				processCount++;
				dataRead.get();
			}
		}

		logger.info("processCount           = %8u", processCount);
		logger.info("AgentDisk::DefaultHandler::run STOP");
	}
};


QByteArray AgentStream::Data::readMesa(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) {
	CARD8* buffer = (CARD8*)Store(iocb->mesaPut.buffer);
	const CARD32 bytesRead = iocb->mesaPut.bytesRead;
	const CARD32 bytesWritten = iocb->mesaPut.bytesWritten;
	LittleEndianByteBuffer bb(buffer, bytesWritten);
	bb.setPos(bytesRead);

	QByteArray ret;
	while(bb.remaining()) ret.append(bb.get8());
	iocb->mesaPut.bytesRead = iocb->mesaPut.bytesWritten;

	return ret;
}

void AgentStream::Data::writeMesa(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb, QByteArray data) {
	CARD8* buffer = (CARD8*)Store(iocb->mesaGet.buffer);
	const CARD32 bytesWritten = iocb->mesaGet.bytesWritten;
	const CARD32 bufferSize  = iocb->mesaGet.bufferSize * Environment::bitsPerWord;
	LittleEndianByteBuffer bb(buffer, bufferSize);
	bb.setPos(bytesWritten);

	for(int i = 0; i < data.size(); i++) bb.put8(data.at(i));
	iocb->mesaGet.bytesWritten += data.size();
}

QByteArray AgentStream::Data::toByteArray(CARD32 data) {
	char buffer[sizeof(CARD32)];
	LittleEndianByteBuffer bb((quint8*)buffer, sizeof(buffer));
	bb.put32(data);

	QByteArray ret;
	for(quint32 i = 0; i < sizeof(buffer); i++) ret.append(buffer[i]);
	return ret;
}
CARD32 AgentStream::Data::toCARD32(QByteArray data) {
	// Sanity check
	if (data.size() != sizeof(CARD32)) {
		logger.fatal("data.size = (%d)%s", data.size(), data.toHex().constData());
		ERROR();
	}
	LittleEndianByteBuffer bb((quint8*)data.data(), data.size());
	return bb.get32();
}

QString AgentStream::Data::toIPAddress(CARD32 data) {
	const CARD8  a = (CARD8)(data >>  8); // AA
	const CARD8  b = (CARD8)(data >>  0); // BB
	const CARD8  c = (CARD8)(data >> 24); // CC
	const CARD8  d = (CARD8)(data >> 16); // DD

	return QString("%1.%2.%3.%4").arg(a).arg(b).arg(c).arg(d);
}

QString AgentStream::Data::toEscapedString(QByteArray data) {
	const int size = data.size();
	// Special for 4 byte data
	if (size == 4) {
		const CARD32 value = toCARD32(data);
		if ((CARD8)value < 'A' || 'Z' < (CARD8)value) {
			return QString::number(value);
		}
	}
	{
		QString ret;
		ret.append(QString("(%1)\"").arg(size));
		for(int j = 0; j < size; j++) {
			unsigned char c = data.at(j);
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

QString AgentStream::Data::toString() {
	QMutexLocker locker(&mutex);

	QString ret;
	ret.append("[");
	for(int i = 0; i < queue.size(); i++) {
		if (i) ret.append(" ");
		ret.append(toEscapedString(queue.at(i)));
	}
	ret.append("]");
	return ret;
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

	setDefaultHandler(new DefaultHandler);
	addHandler(new StreamTCP);
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

	if (DEBUG_SHOW_AGENT_STREAM) dump(logger);

	if (fcb->iocbHead == 0) {
		defaultHandler->idle(fcb);
		for(Handler* handler: handlerMap.values()) handler->idle(fcb);
		return;
	}

	if (fcb->iocbHead) {
		CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb = (CoProcessorIOFaceGuam::CoProcessorIOCBType*)Store(fcb->iocbHead);
		AgentStream::Handler* handler = getHandler(iocb->serverID);

		switch(fcb->headCommand) {
		case CoProcessorIOFaceGuam::C_accept: {
			// Sanity check
			if (iocb->mesaPut.hTask == 0) {
				logger.fatal("mesaPut.hTask = %d", iocb->mesaPut.hTask);
				ERROR();
			}
			if (iocb->mesaGet.hTask == 0) {
				logger.fatal("mesaGet.hTask = %d", iocb->mesaGet.hTask);
				ERROR();
			}

			Task* task = getTask(iocb->mesaPut.hTask);
			fcb->headResult = handler->accept(iocb, task);
		}
			break;
		case CoProcessorIOFaceGuam::C_connect: {
			// Sanity check
			if (iocb->mesaPut.hTask) {
				logger.fatal("mesaPut.hTask = %d", iocb->mesaPut.hTask);
				ERROR();
			}
			if (iocb->mesaGet.hTask) {
				logger.fatal("mesaGet.hTask = %d", iocb->mesaGet.hTask);
				ERROR();
			}

			Task* task = handler->createTask();
			addTask(task);
			iocb->mesaPut.hTask = iocb->mesaGet.hTask = task->hTask;
			iocb->pcConnectionState = CoProcessorIOFaceGuam::S_connected;
			//
			fcb->headResult = handler->connect(iocb, task);
		}
			break;
		case CoProcessorIOFaceGuam::C_delete: {
			// Sanity check
			if (iocb->mesaPut.hTask == 0) {
				logger.fatal("mesaPut.hTask = %d", iocb->mesaPut.hTask);
				ERROR();
			}
			if (iocb->mesaGet.hTask == 0) {
				logger.fatal("mesaGet.hTask = %d", iocb->mesaGet.hTask);
				ERROR();
			}

			Task* task = getTask(iocb->mesaPut.hTask);
			fcb->headResult = handler->destroy(iocb, task);
			//
			iocb->pcConnectionState = CoProcessorIOFaceGuam::S_deleted;
			iocb->mesaPut.hTask = iocb->mesaGet.hTask = 0;
			removeTask(task);
		}
			break;
		case CoProcessorIOFaceGuam::C_read: {
			// Sanity check
			if (iocb->mesaPut.hTask == 0) {
				logger.fatal("mesaPut.hTask = %d", iocb->mesaPut.hTask);
				ERROR();
			}
			if (iocb->mesaGet.hTask == 0) {
				logger.fatal("mesaGet.hTask = %d", iocb->mesaGet.hTask);
				ERROR();
			}

			// add data in iocb->mesaPut to dataRead
			QByteArray data = Data::readMesa(iocb);
			if (DEBUG_SHOW_AGENT_STREAM) {
				logger.debug("    read   %s", Data::toEscapedString(data).toLocal8Bit().constData());
			}
			handler->dataRead.put(data);
			fcb->headResult = CoProcessorIOFaceGuam::R_completed;
		}
			break;
		case CoProcessorIOFaceGuam::C_write: {
			// Sanity check
			if (iocb->mesaPut.hTask == 0) {
				logger.fatal("mesaPut.hTask = %d", iocb->mesaPut.hTask);
				ERROR();
			}
			if (iocb->mesaGet.hTask == 0) {
				logger.fatal("mesaGet.hTask = %d", iocb->mesaGet.hTask);
				ERROR();
			}

			// Check dataWrite and return if not empty
			if (handler->dataWrite.size()) {
				// There is data to return.
				QByteArray data = handler->dataWrite.get();
				Data::writeMesa(iocb, data);
				if (iocb->mesaGet.interruptMesa) notifyInterrupt();
				fcb->headResult = CoProcessorIOFaceGuam::R_completed;

				if (DEBUG_SHOW_AGENT_STREAM) {
					logger.debug("    write  %s", Data::toEscapedString(data).toLocal8Bit().constData());
				}
			} else {
				// There is no data to return
				fcb->headResult = CoProcessorIOFaceGuam::R_inProgress;
			}
		}
			break;
		default:
		{
			logger.fatal("headCommand = %d", fcb->headCommand);
			ERROR();
		}
			break;
		}
	}

	if (DEBUG_SHOW_AGENT_STREAM) logger.debug("    result %s", Stream::getResultString(fcb->headResult));
}


void AgentStream::setDefaultHandler(AgentStream::Handler* handler) {
	if (defaultHandler == 0) {
		defaultHandler = handler;
	} else {
		logger.fatal("defaultHandler  serverID = %d  name = %s", defaultHandler->serverID, defaultHandler->name);
		ERROR();
	}
}

void AgentStream::addHandler(AgentStream::Handler* handler) {
	const CARD16 serverID = handler->serverID;
	if (!handlerMap.contains(serverID)) {
		handlerMap.insert(serverID, handler);
	} else {
		Handler* oldHandler = handlerMap[serverID];
		logger.fatal("defaultHandler  serverID = %d  name = %s", serverID, oldHandler->name);
		ERROR();
	}
}

AgentStream::Handler* AgentStream::getHandler(CARD32 serverID) {
	return handlerMap.contains(serverID) ? handlerMap[serverID] : defaultHandler;
}

void  AgentStream::addTask   (Task* task) {
	if (taskMap.contains(task->hTask)) {
		logger.fatal("hTask = %d", task->hTask);
		ERROR();
	} else {
		taskMap.insert(task->hTask, task);
	}
}
void  AgentStream::removeTask(Task* task) {
	if (taskMap.contains(task->hTask)) {
		taskMap.remove(task->hTask);
	} else {
		logger.fatal("hTask = %d", task->hTask);
		ERROR();
	}
}
AgentStream::Task* AgentStream::getTask   (CARD32 taskID) {
	if (taskMap.contains(taskID)) {
		return taskMap[taskID];
	} else {
		logger.fatal("taskID = %d", taskID);
		ERROR();
		return 0;
	}
}

void AgentStream::startThread() {
	QThreadPool::globalInstance()->start(defaultHandler);
	for(Handler* handler: handlerMap.values()) {
		QThreadPool::globalInstance()->start(handler);
	}
}

void AgentStream::stopThread() {
	defaultHandler->stop();
	for(Handler* handler: handlerMap.values()) {
		handler->stop();
	}
}

void AgentStream::dump(log4cpp::Category& logger) {
	logger.debug("AGENT %s  %08X  %s", name, fcb->iocbHead, Stream::getCommandString(fcb->headCommand));
	if (fcb->iocbHead) {
		CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb = (CoProcessorIOFaceGuam::CoProcessorIOCBType*)Store(fcb->iocbHead);
		logger.debug("   %c%-10s  %c-%c  mesaGet[%04d%c%c%02X %X %3d %3d]  mesaPut[%04d%c%c%02X %X %3d %3d]",
				(iocb->mesaIsServer ? 'S' : ' '), Stream::getServerString(iocb->serverID),
			Stream::getConnectionStateString(iocb->mesaConnectionState)[0],
			Stream::getConnectionStateString(iocb->pcConnectionState)[0],
			iocb->mesaGet.hTask, (iocb->mesaGet.interruptMesa ? 'I' : ' '), (iocb->mesaGet.writeLockedByMesa ? 'L' : ' '), (iocb->mesaGet.subSequence & 0xFF), (iocb->mesaGet.u2 & 0x0F), iocb->mesaGet.bytesRead, iocb->mesaGet.bytesWritten,
			iocb->mesaPut.hTask, (iocb->mesaPut.interruptMesa ? 'I' : ' '), (iocb->mesaPut.writeLockedByMesa ? 'L' : ' '), (iocb->mesaPut.subSequence & 0xFF), (iocb->mesaPut.u2 & 0x0F), iocb->mesaPut.bytesRead, iocb->mesaPut.bytesWritten
			);

		CARD32 serverID = iocb->serverID;
		if (handlerMap.contains(serverID)) {
			Handler* handler = handlerMap[serverID];
			logger.debug("    %-10s  read%s  write%s", handler->name, handler->dataRead.toString().toLocal8Bit().constData(), handler->dataWrite.toString().toLocal8Bit().constData());
		}
	}
}

void AgentStream::notifyInterrupt() {
	InterruptThread::notifyInterrupt(fcb->interruptSelector);
}


QByteArray AgentStream::Handler::getData() {
	for(;;) {
		if (stopThread) throw StopThreadException();

		if (0 < dataRead.size()) break;
		dataRead.waitData();
	}
	return dataRead.get();
}

void AgentStream::Handler::putData(QByteArray data) {
	if (stopThread) throw StopThreadException();
	dataWrite.put(data);
}
