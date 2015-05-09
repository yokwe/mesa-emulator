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
// AgentFloppy.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("agentflop");


#include "../util/Debug.h"

#include "../mesa/Pilot.h"
#include "../mesa/Memory.h"
#include "../mesa/MesaThread.h"

#include "Agent.h"
#include "AgentFloppy.h"

CARD32 AgentFloppy::getFCBSize() {
	if (diskFileList.size() == 0) return 0;
	return SIZE(FloppyIOFaceGuam::FloppyFCBType) + SIZE(FloppyIOFaceGuam::FloppyDCBType) * diskFileList.size();
}

void AgentFloppy::Initialize() {
	if (fcbAddress == 0) ERROR();

	fcb = (FloppyIOFaceGuam::FloppyFCBType *)Store(fcbAddress);
	fcb->nextIOCB = 0;
	fcb->interruptSelector = 0;
	fcb->stopAgent = 0;
	fcb->agentStopped = 1;
	fcb->numberOfDCBs = diskFileList.size();

	dcb = fcb->dcbs;
	for(int i = 0; i < fcb->numberOfDCBs; i++) {
		DiskFile* diskFile = diskFileList[i];
		diskFile->setFloppyDCBType(fcb->dcbs + i);
		logger.debug("AGENT %s  %i  CHS = %5d %2d %2d  %s", name, i, dcb[i].numberOfCylinders, dcb[i].numberOfHeads, dcb[i].sectorsPerTrack, diskFile->getPath().toLatin1().constData());
	}
}

void AgentFloppy::Call() {
	if (fcb->stopAgent) {
		if (!fcb->agentStopped) {
			logger.info("AGENT %s stop", name);
		}
		fcb->agentStopped = 1;
		// Do nothing when ask to stop
		return;
	} else {
		if (fcb->agentStopped) {
			logger.info("AGENT %s start", name);
		}
		fcb->agentStopped = 0;
	}

	if (fcb->nextIOCB == 0) {
		if (DEBUG_SHOW_AGENT_FLOPPY) logger.debug("AGENT %s fcb->nextIOCB == 0", name);
		return; // Return if there is no IOCB
	}
	FloppyIOFaceGuam::FloppyIOCBType *iocb = (FloppyIOFaceGuam::FloppyIOCBType *)Store(fcb->nextIOCB);
	for(;;) {
		CARD16 deviceIndex = iocb->operation.device; // TODO Is this correct?
		if (fcb->numberOfDCBs <= deviceIndex) ERROR();
		DiskFile* diskFile = diskFileList[deviceIndex];
		CARD32 block = diskFile->getBlock(iocb);

		//"AGENT %s %d", name, fcb->command
		CARD16 command = iocb->operation.function;
		switch(command) {
		case FloppyDiskFace::F_nop:
			iocb->status = FloppyDiskFace::S_notReady;
			break;
		case FloppyDiskFace::F_readSector: {
			if (DEBUG_SHOW_AGENT_FLOPPY) logger.debug("AGENT %s %4d READ   %08X + %3d dataPtr = %08X  nextIOCB = %08X", name, deviceIndex, block, iocb->operation.count, iocb->operation.dataPtr, iocb->nextIOCB);

			CARD32 dataPtr = iocb->operation.dataPtr;
			for(int i = 0; i < iocb->operation.count; i++) {
				CARD16 *buffer = Store(dataPtr);
				diskFile->readPage(block++, buffer);
				dataPtr += PageSize;
			}
			iocb->operation.count = 0;
			iocb->status = FloppyDiskFace::S_goodCompletion;
			ERROR();
		}
			break;
		case FloppyDiskFace::F_writeSector: {
			if (DEBUG_SHOW_AGENT_FLOPPY) logger.debug("AGENT %s %4d WRITE  %08X + %3d dataPtr = %08X  nextIOCB = %08X", name, deviceIndex, block, iocb->operation.count, iocb->operation.dataPtr, iocb->nextIOCB);

			CARD32 dataPtr = iocb->operation.dataPtr;
			for(int i = 0; i < iocb->operation.count; i++) {
				CARD16 *buffer = Fetch(dataPtr);
				diskFile->writePage(block++, buffer);
				dataPtr += PageSize;
			}
			iocb->operation.count = 0;
			iocb->status = FloppyDiskFace::S_goodCompletion;
			ERROR();
		}
			break;
		default:
			logger.fatal("AGENT %s %5d  %08X + %3d dataPtr = %08X  nextIOCB = %08X", name, command, block, iocb->operation.count, iocb->operation.dataPtr, iocb->nextIOCB);
			ERROR();
			break;
		}

		if (iocb->nextIOCB == 0) break;
		// advance to next IOCB
		iocb = (FloppyIOFaceGuam::FloppyIOCBType *)Store(iocb->nextIOCB);
	}

	// notify with interrupt
	//WP |= fcb->interruptSelector;
	InterruptThread::notifyInterrupt(fcb->interruptSelector);
}

void AgentFloppy::addDiskFile(DiskFile *diskFile) {
	this->diskFileList.append(diskFile);
}
