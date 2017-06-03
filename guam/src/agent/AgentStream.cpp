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
#include "../mesa/MesaThread.h"

#include "Agent.h"
#include "AgentStream.h"

class StreamDummy : public AgentStream::Stream {
public:
	StreamDummy(CARD32 serverID);

	quint16 idle   (CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb);
	quint16 accept (CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb);
	quint16 connect(CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb);
	quint16 destroy(CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb);
	quint16 read   (CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb);
	quint16 write  (CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb);

private:
};

StreamDummy::StreamDummy(CARD32 serverID) : Stream(QString("DUMMY-%1").arg(serverID), serverID) {
	//
}

quint16 StreamDummy::idle   (CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) {
	logger.error("%-8s idle %d %d", name.toLatin1().constData(), fcb->headCommand, iocb->serverID);
//	ERROR();
	return CoProcessorIOFaceGuam::R_error;
}
quint16 StreamDummy::accept (CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) {
	logger.error("%-8s accept %d %d", name.toLatin1().constData(), fcb->headCommand, iocb->serverID);
//	ERROR();
	return CoProcessorIOFaceGuam::R_error;
}
quint16 StreamDummy::connect(CoProcessorIOFaceGuam::CoProcessorFCBType * /*fcb*/, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) {
	logger.info("%-8s connect  state mesa = %d  pc = %d", name.toLatin1().constData(), iocb->mesaConnectionState, iocb->pcConnectionState);
//	ERROR();
	return CoProcessorIOFaceGuam::R_error;
}
quint16 StreamDummy::destroy(CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) {
	logger.info("%-8s destroy %d %d", name.toLatin1().constData(), fcb->headCommand, iocb->serverID);
	//	ERROR();
	return CoProcessorIOFaceGuam::R_error;
}
quint16 StreamDummy::read   (CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) {
	logger.error("%-8s read %d %d", name.toLatin1().constData(), fcb->headCommand, iocb->serverID);
	CoProcessorIOFaceGuam::TransferRec& tr = iocb->mesaPut;
	logger.info("mesaPut  sst: %d  end [Stream: %d  Record: %d  SST: %d]  written: %3d  read: %3d  hTask: %d  int: %d  buffer: %4X  bufferSize: %3d  lock: %d",
		tr.subSequence, tr.endStream, tr.endRecord, tr.endSST,
		tr.bytesWritten, tr.bytesRead, tr.hTask, tr.interruptMesa, tr.buffer, tr.bufferSize, tr.writeLockedByMesa);
	//	ERROR();
	return CoProcessorIOFaceGuam::R_error;
}
quint16 StreamDummy::write  (CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) {
	logger.error("%-8s write %d %d", name.toLatin1().constData(), fcb->headCommand, iocb->serverID);
	CoProcessorIOFaceGuam::TransferRec& tr = iocb->mesaGet;
	logger.info("mesaGet  sst: %d  end [Stream: %d  Record: %d  SST: %d]  written: %3d  read: %3d  hTask: %d  int: %d  buffer: %4X  bufferSize: %3d  lock: %d",
		tr.subSequence, tr.endStream, tr.endRecord, tr.endSST,
		tr.bytesWritten, tr.bytesRead, tr.hTask, tr.interruptMesa, tr.buffer, tr.bufferSize, tr.writeLockedByMesa);
//	ERROR();
	return CoProcessorIOFaceGuam::R_error;
}

//		{
//			logger.info("mesaGet  sst: %d  end [Stream: %d  Record: %d  SST: %d]  written: %3d  read: %3d  hTask: %d  int: %d  buffer: %4X  bufferSize: %3d  lock: %d",
//				tr.subSequence, tr.endStream, tr.endRecord, tr.endSST,
//				tr.bytesWritten, tr.bytesRead, tr.hTask, tr.interruptMesa, tr.buffer, tr.bufferSize, tr.writeLockedByMesa);
//		}
//		{
//			CoProcessorIOFaceGuam::TransferRec& tr = iocb->mesaPut;
//			logger.info("mesaPut  sst: %d  end [Stream: %d  Record: %d  SST: %d]  written: %3d  read: %3d  hTask: %d  int: %d  buffer: %4X  bufferSize: %3d  lock: %d",
//				tr.subSequence, tr.endStream, tr.endRecord, tr.endSST,
//				tr.bytesWritten, tr.bytesRead, tr.hTask, tr.interruptMesa, tr.buffer, tr.bufferSize, tr.writeLockedByMesa);
//		}

void AgentStream::addStream(Stream* stream) {
	quint32 serverID = stream->serverID;
	if (map.contains(serverID)) {
		logger.error("Duplicate serverID = %d", stream->serverID);
		ERROR();
	}
	map[stream->serverID] = stream;
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
//
//	BootStream* bootStream = new BootStream;
//	addStream((Stream*)bootStream);
}

void AgentStream::Call() {
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

	if (fcb->iocbHead != fcb->iocbNext) {
		logger.error("fcb->iocbHead = %04X  fcb->iocbNext = %04X", fcb->iocbHead, fcb->iocbNext);
		ERROR();
	}

	CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb = (CoProcessorIOFaceGuam::CoProcessorIOCBType*)Store(fcb->iocbHead);

	if (DEBUG_SHOW_AGENT_STREAM) {
		logger.debug("AGENT %s  head = %08X  next = %08X  command = %2d  result = %2d  interruptSelector = %04X", name, fcb->iocbHead, fcb->iocbNext, fcb->headCommand, fcb->headResult, fcb->interruptSelector);
		logger.debug("    serverID = %3d  mesaIsServer = %d  mesaConnectionState = %d  pcConnectionState = %d  next = %8X",
			iocb->serverID, iocb->mesaIsServer, iocb->mesaConnectionState, iocb->pcConnectionState, iocb->nextIOCB);
	}

	quint32 serverID = iocb->serverID;
	quint16 command = fcb->headCommand;

	if (!map.contains(serverID)) {
		Stream* dummy = new StreamDummy(serverID);
		logger.warn("Add dummy %s", dummy->name.toLatin1().constData());
		map[serverID] = dummy;
	}
	Stream* stream = map[serverID];
	quint16 result = CoProcessorIOFaceGuam::R_error;

	switch(command) {
	case CoProcessorIOFaceGuam::C_idle:
		result = stream->idle(fcb, iocb);
		break;
	case CoProcessorIOFaceGuam::C_accept:
		result = stream->accept(fcb, iocb);
		break;
	case CoProcessorIOFaceGuam::C_connect:
		result = stream->connect(fcb, iocb);
		break;
	case CoProcessorIOFaceGuam::C_delete:
		result = stream->destroy(fcb, iocb);
		break;
	case CoProcessorIOFaceGuam::C_read:
		result = stream->read(fcb, iocb);
		break;
	case CoProcessorIOFaceGuam::C_write:
		result = stream->write(fcb, iocb);
		break;
	default:
		logger.error("Unknown command = %d", command);
		ERROR();
	}

	if (DEBUG_SHOW_AGENT_STREAM) logger.debug("    result = %d", result);
	fcb->headResult = result;
}
