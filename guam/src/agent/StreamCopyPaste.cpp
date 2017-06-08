/*
Copyright (c) 2017, Yasuhiro Hasegawa
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
// StreamCopyPaste.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("copyPaste");


#include "../util/Debug.h"

#include "../mesa/Pilot.h"
#include "../mesa/Memory.h"
#include "../mesa/MesaThread.h"

#include "Agent.h"
#include "AgentStream.h"
#include "StreamCopyPaste.h"


StreamCopyPaste::StreamCopyPaste() : Stream("CopyPaste", CoProcessorServerIDs::copyPaste) {
	logger.info("%3d %-8s", serverID, name.toLatin1().constData());
}

quint16 StreamCopyPaste::idle   (CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) {
	logger.error("%-8s idle %d %d", name.toLatin1().constData(), fcb->headCommand, iocb->serverID);
	return CoProcessorIOFaceGuam::R_completed;
}
quint16 StreamCopyPaste::accept (CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) {
	logger.error("%-8s accept %d %d", name.toLatin1().constData(), fcb->headCommand, iocb->serverID);
	ERROR();
	return CoProcessorIOFaceGuam::R_error;
}
quint16 StreamCopyPaste::connect(CoProcessorIOFaceGuam::CoProcessorFCBType * /*fcb*/, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) {
	logger.info("%-8s connect  mesaIsServer = %d  state mesa = %d  pc = %d", name.toLatin1().constData(), iocb->mesaIsServer, iocb->mesaConnectionState, iocb->pcConnectionState);
	return CoProcessorIOFaceGuam::R_error;

//	iocb->pcConnectionState = CoProcessorIOFaceGuam::S_connected;
//	// Need to assign non-zero to mesaGet.hTaskactually. See CoProcessorFace.Get
//	iocb->mesaGet.hTask = 1;
//
//	return CoProcessorIOFaceGuam::R_completed;
}
quint16 StreamCopyPaste::destroy(CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) {
	logger.info("%-8s destroy %d %d", name.toLatin1().constData(), fcb->headCommand, iocb->serverID);
	return CoProcessorIOFaceGuam::R_error;
}
quint16 StreamCopyPaste::read   (CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) {
	logger.error("%-8s write %d %d", name.toLatin1().constData(), fcb->headCommand, iocb->serverID);
	return CoProcessorIOFaceGuam::R_error;
}
quint16 StreamCopyPaste::write  (CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) {
	logger.error("%-8s write %d %d", name.toLatin1().constData(), fcb->headCommand, iocb->serverID);
	CoProcessorIOFaceGuam::TransferRec& tr = iocb->mesaGet;
	logger.info("mesaGet  sst: %d  end [Stream: %d  Record: %d  SST: %d]  written: %3d  read: %3d  hTask: %d  int: %d  buffer: %4X  bufferSize: %3d  lock: %d",
		tr.subSequence, tr.endStream, tr.endRecord, tr.endSST,
		tr.bytesWritten, tr.bytesRead, tr.hTask, tr.interruptMesa, tr.buffer, tr.bufferSize, tr.writeLockedByMesa);

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

	return CoProcessorIOFaceGuam::R_error;
}
