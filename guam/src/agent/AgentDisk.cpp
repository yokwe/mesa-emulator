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
// AgentDisk.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("agentdisk");


#include "../util/Debug.h"

#include "../mesa/Pilot.h"
#include "../mesa/Memory.h"
#include "../mesa/MesaThread.h"

#include "Agent.h"
#include "AgentDisk.h"
#include "DiskFile.h"

static const CARD32 DEBUG_DONT_USE_THREAD = 1;

int AgentDisk::IOThread::stopThread = 0;

static int readCount = 0;
static int writeCount = 0;
static int verifyCount = 0;

void AgentDisk::IOThread::run() {
	logger.info("AgentDisk::IOThread::run START");

	stopThread = 0;
	int processCount = 0;
	QThread::currentThread()->setPriority(PRIORITY);

	try {
		for(;;) {
			if (stopThread) break;

			DiskIOFaceGuam::DiskIOCBType* iocb     = 0;
			DiskFile*                     diskFile = 0;
			{
				QMutexLocker locker(&ioMutex);
				if (ioQueue.isEmpty()) {
					for(;;) {
						bool ret = ioCV.wait(&ioMutex, WAIT_INTERVAL);
						if (ret) break;
						if (stopThread) goto exitLoop;
					}
				}

				Item item = ioQueue.first();
				iocb      = item.iocb;
				diskFile  = item.diskFile;
				ioQueue.removeFirst();
			}

			process(iocb, diskFile);
			InterruptThread::notifyInterrupt(interruptSelector);
			processCount++;
		}
	} catch(Abort& e) {
		logger.fatal("Unexpected Abort %s %d %s", e.file, e.line, e.func);
		ProcessorThread::stop();
	}
exitLoop:
	logger.info("processCount           = %8u", processCount);
	logger.info("readCount              = %8u", readCount);
	logger.info("writeCount             = %8u", writeCount);
	logger.info("verifyCount            = %8u", verifyCount);
	logger.info("AgentDisk::IOThread::run STOP");
}
void AgentDisk::IOThread::reset() {
	QMutexLocker locker(&ioMutex);
	ioQueue.clear();
}

void AgentDisk::IOThread::setInterruptSelector(CARD16 interruptSelector) {
	this->interruptSelector = interruptSelector;
}

void AgentDisk::IOThread::enqueue(DiskIOFaceGuam::DiskIOCBType* iocb, DiskFile* diskFile) {
	QMutexLocker locker(&ioMutex);
	Item item(iocb, diskFile);

	ioQueue.append(item);
	if (ioQueue.size() == 1) ioCV.wakeOne();
}

void AgentDisk::IOThread::process(DiskIOFaceGuam::DiskIOCBType* iocb, DiskFile* diskFile) {
	CARD32 block = diskFile->getBlock(iocb);

	//"AGENT %s %d", name, fcb->command
	PilotDiskFace::Command command = (PilotDiskFace::Command)iocb->command;
	switch(command) {
	case PilotDiskFace::read: {
		if (DEBUG_SHOW_AGENT_DISK) logger.debug("IOThread::process %4d READ   %08X + %3d dataPtr = %08X  nextIOCB = %08X", iocb->deviceIndex, block, iocb->pageCount, iocb->dataPtr, iocb->nextIOCB);

		CARD32 dataPtr = iocb->dataPtr;
		for(int i = 0; i < iocb->pageCount; i++) {
			CARD16 *buffer = Memory::getAddress(dataPtr);
			diskFile->readPage(block++, buffer);
			dataPtr += PageSize;
		}
		//
		iocb->pageCount = 0;
		iocb->status = PilotDiskFace::goodCompletion;
		//
		readCount++;
	}
		break;
	case PilotDiskFace::write: {
		if (DEBUG_SHOW_AGENT_DISK) logger.debug("IOThread::process %4d WRITE  %08X + %3d dataPtr = %08X  nextIOCB = %08X", iocb->deviceIndex, block, iocb->pageCount, iocb->dataPtr, iocb->nextIOCB);

		CARD32 dataPtr = iocb->dataPtr;
		for(int i = 0; i < iocb->pageCount; i++) {
			CARD16 *buffer = Memory::getAddress(dataPtr);
			diskFile->writePage(block++, buffer);
			dataPtr += PageSize;
		}
		//
		iocb->pageCount = 0;
		iocb->status = PilotDiskFace::goodCompletion;
		//
		writeCount++;
	}
		break;
	case PilotDiskFace::verify: {
		if (DEBUG_SHOW_AGENT_DISK) logger.debug("IOThread::process %4d VERIFY %08X + %3d dataPtr = %08X  nextIOCB = %08X", iocb->deviceIndex, block, iocb->pageCount, iocb->dataPtr, iocb->nextIOCB);

		int ret = 0;
		CARD32 dataPtr = iocb->dataPtr;
		for(int i = 0; i < iocb->pageCount; i++) {
			CARD16 *buffer = Memory::getAddress(dataPtr);
			ret |= diskFile->verifyPage(block++, buffer);
			dataPtr += PageSize;
		}
		//
		iocb->pageCount = 0;
		iocb->status = ret ? PilotDiskFace::dataVerifyError : PilotDiskFace::goodCompletion;
		//
		verifyCount++;
	}
		break;
	default:
		logger.fatal("command = %d", command);
		ERROR();
		break;
	}
}



CARD32 AgentDisk::getFCBSize() {
	return SIZE(DiskIOFaceGuam::DiskFCBType) + SIZE(DiskIOFaceGuam::DiskDCBType) * diskFileList.size();
}

void AgentDisk::Initialize() {
	if (fcbAddress == 0) ERROR();

	fcb = (DiskIOFaceGuam::DiskFCBType*)Store(fcbAddress);
	fcb->nextIOCB = 0;
	fcb->interruptSelector = 0;
	fcb->stopAgent = 0;
	fcb->agentStopped = 1;
	fcb->numberOfDCBs = diskFileList.size();

	if (fcb->numberOfDCBs == 0) ERROR();

	dcb = fcb->dcbs;
	for(int i = 0; i < fcb->numberOfDCBs; i++) {
		DiskFile* diskFile = diskFileList[i];
		diskFile->setDiskDCBType(dcb + i);
		logger.info("AGENT %s  %i  CHS = %5d %2d %2d  %s", name, i, dcb[i].numberOfCylinders, dcb[i].numberOfHeads, dcb[i].sectorsPerTrack, diskFile->getPath().toLatin1().constData());
	}

	ioThread.setAutoDelete(false);
}

void AgentDisk::Call() {
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
			ioThread.reset();
			ioThread.setInterruptSelector(fcb->interruptSelector);
		}
		fcb->agentStopped = 0;
	}

	if (fcb->nextIOCB == 0) {
		if (DEBUG_SHOW_AGENT_DISK) logger.debug("AGENT %s fcb->nextIOCB == 0", name);
		return; // Return if there is no IOCB
	}
	CARD32 nextIOCB = fcb->nextIOCB;
	DiskIOFaceGuam::DiskIOCBType *iocb = (DiskIOFaceGuam::DiskIOCBType *)Store(nextIOCB);
	for(;;) {
		// sanity check
		CARD16 deviceIndex = iocb->deviceIndex;
		if (fcb->numberOfDCBs <= deviceIndex) {
			logger.fatal("AGENT %s deviceIndex = %d", name, deviceIndex);
			ERROR();
		}
		DiskFile* diskFile = diskFileList[deviceIndex];
		PilotDiskFace::Command command = (PilotDiskFace::Command)iocb->command;
		switch(command) {
		case PilotDiskFace::read:
		case PilotDiskFace::write:
		case PilotDiskFace::verify:
			break;
		default:
			logger.fatal("AGENT %s command = %d", name, command);
			ERROR();
		}

		if (DEBUG_DONT_USE_THREAD) {
			CARD32 block = diskFile->getBlock(iocb);

			switch(command) {
			case PilotDiskFace::read: {
				logger.debug("IOThread::process %4d READ   %08X + %3d dataPtr = %08X  nextIOCB = %08X", iocb->deviceIndex, block, iocb->pageCount, iocb->dataPtr, iocb->nextIOCB);

				CARD32 dataPtr = iocb->dataPtr;
				for(int i = 0; i < iocb->pageCount; i++) {
					CARD16 *buffer = Memory::getAddress(dataPtr);
					diskFile->readPage(block++, buffer);
					dataPtr += PageSize;
				}
				//
				iocb->pageCount = 0;
				iocb->status = PilotDiskFace::goodCompletion;
				//
				readCount++;
			}
				break;
			case PilotDiskFace::write: {
				logger.debug("IOThread::process %4d WRITE  %08X + %3d dataPtr = %08X  nextIOCB = %08X", iocb->deviceIndex, block, iocb->pageCount, iocb->dataPtr, iocb->nextIOCB);

				CARD32 dataPtr = iocb->dataPtr;
				for(int i = 0; i < iocb->pageCount; i++) {
					CARD16 *buffer = Memory::getAddress(dataPtr);
					diskFile->writePage(block++, buffer);
					dataPtr += PageSize;
				}
				//
				iocb->pageCount = 0;
				iocb->status = PilotDiskFace::goodCompletion;
				//
				writeCount++;
			}
				break;
			case PilotDiskFace::verify: {
				logger.debug("IOThread::process %4d VERIFY %08X + %3d dataPtr = %08X  nextIOCB = %08X", iocb->deviceIndex, block, iocb->pageCount, iocb->dataPtr, iocb->nextIOCB);

				int ret = 0;
				CARD32 dataPtr = iocb->dataPtr;
				for(int i = 0; i < iocb->pageCount; i++) {
					CARD16 *buffer = Memory::getAddress(dataPtr);
					ret |= diskFile->verifyPage(block++, buffer);
					dataPtr += PageSize;
				}
				//
				iocb->pageCount = 0;
				iocb->status = ret ? PilotDiskFace::dataVerifyError : PilotDiskFace::goodCompletion;
				//
				verifyCount++;
			}
				break;
			default:
				logger.fatal("AGENT %s command = %d", name, command);
				ERROR();
			}
		} else {
			ioThread.enqueue(iocb, diskFile);
		}

		if (iocb->nextIOCB == 0) break;
		// advance to next IOCB
		nextIOCB = iocb->nextIOCB;
		iocb = (DiskIOFaceGuam::DiskIOCBType *)Store(nextIOCB);
	}

	if (DEBUG_DONT_USE_THREAD) {
		InterruptThread::notifyInterrupt(fcb->interruptSelector);
	}

	// notify with interrupt
	//WP |= fcb->interruptSelector;
	//InterruptThread::notifyInterrupt(fcb->interruptSelector);
}

void AgentDisk::addDiskFile(DiskFile* diskFile) {
	diskFileList.append(diskFile);
}
