/*
Copyright (c) 2014, 2017, Yasuhiro Hasegawa
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
// StreamBoot.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("boot");


#include "../util/Debug.h"

#include "../mesa/Pilot.h"
#include "../mesa/Memory.h"
#include "../mesa/MesaThread.h"

#include "Agent.h"
#include "AgentStream.h"
#include "StreamBoot.h"


StreamBoot::StreamBoot(QString path_) : Stream("BOOT", CoProcessorServerIDs::bootAgentID) {
	path = path_;
	map  = (quint16*)Util::mapFile(path, mapSize);
	mapSize /= Environment::bytesPerWord;
	pos = 0;
	logger.info("%3d %-8s %s", serverID, name.toLatin1().constData(), path.toLatin1().constData());
}

quint16 StreamBoot::idle   (CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) {
	logger.error("%-8s idle %d %d", name.toLatin1().constData(), fcb->headCommand, iocb->serverID);
	ERROR();
	return CoProcessorIOFaceGuam::R_error;
}
quint16 StreamBoot::accept (CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) {
	logger.error("%-8s accept %d %d", name.toLatin1().constData(), fcb->headCommand, iocb->serverID);
	ERROR();
	return CoProcessorIOFaceGuam::R_error;
}
quint16 StreamBoot::connect(CoProcessorIOFaceGuam::CoProcessorFCBType * /*fcb*/, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) {
	logger.info("%-8s connect  state mesa = %d  pc = %d", name.toLatin1().constData(), iocb->mesaConnectionState, iocb->pcConnectionState);
	iocb->pcConnectionState = CoProcessorIOFaceGuam::S_connected;
	// Need to assign non-zero to mesaGet.hTaskactually. See CoProcessorFace.Get
	iocb->mesaGet.hTask = 1;
	// rewind position
	pos = 0;
	return CoProcessorIOFaceGuam::R_completed;
}
quint16 StreamBoot::destroy(CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) {
	logger.info("%-8s destroy %d %d", name.toLatin1().constData(), fcb->headCommand, iocb->serverID);
	return CoProcessorIOFaceGuam::R_error;
}
quint16 StreamBoot::read   (CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) {
	logger.error("%-8s read %d %d", name.toLatin1().constData(), fcb->headCommand, iocb->serverID);
	ERROR();
	return CoProcessorIOFaceGuam::R_error;
}
quint16 StreamBoot::write  (CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) {
	CoProcessorIOFaceGuam::TransferRec& tr = iocb->mesaGet;
	if (DEBUG_SHOW_STREAM_BOOT) logger.info("%-8s write %8X+%d", name.toLatin1().constData(), pos, tr.bufferSize);
	if (tr.writeLockedByMesa) {
		logger.warn("writeLockedByMesa");
		return CoProcessorIOFaceGuam::R_inProgress;
	}
	if (tr.buffer == 0) {
		logger.fatal("tr.buffer = 0");
		ERROR();
	}
	if (tr.bytesRead != 0) {
		logger.fatal("tr.bytesRead = %d", tr.bytesRead);
		ERROR();
	}
	if (tr.bytesWritten != 0) {
		logger.fatal("tr.bytesWritten = %d", tr.bytesWritten);
		ERROR();
	}
	if (tr.bufferSize != Environment::bytesPerPage) {
		logger.fatal("tr.bufferSize = %d", tr.bufferSize);
		ERROR();
	}

	quint16* buffer = (quint16*)Store(tr.buffer);
	quint32  size   = tr.bufferSize / Environment::bytesPerWord;
	if (mapSize < (pos + size)) size = mapSize - pos;
	quint32  nextPos = pos + size;
	if (DEBUG_SHOW_STREAM_BOOT) logger.info("DATA %4X => %4X", pos * Environment::bytesPerWord, nextPos * Environment::bytesPerWord);

	Util::fromBigEndian(map + pos, buffer, size);
	pos = nextPos;
	tr.bytesWritten = size * Environment::bytesPerWord;

	if (pos == mapSize) {
		tr.endStream = 1;
	}

	// generate interrupt if needed
	if (tr.interruptMesa) {
		if (fcb->interruptSelector) {
			fcb->headResult = CoProcessorIOFaceGuam::R_completed;
			InterruptThread::notifyInterrupt(fcb->interruptSelector);
		}
	}

	return CoProcessorIOFaceGuam::R_completed;
}
