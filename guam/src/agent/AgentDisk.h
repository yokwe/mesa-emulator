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
// AgentDisk.h
//

#ifndef AGENTDISK_H__
#define AGENTDISK_H__

#include "Agent.h"
#include "DiskFile.h"

#include <QtCore>

class AgentDisk : public Agent {
public:
	class IOThread: public QRunnable {
	public:
		// Wait interval in milliseconds for QWaitCondition::wait
		static const int WAIT_INTERVAL = 1000;
		static const QThread::Priority PRIORITY = QThread::HighPriority;

		static void stop() {
			stopThread = 1;
		}

		IOThread() {
			interruptSelector = 0;
		}

		void run();
		void reset();

		void setInterruptSelector(CARD16 interruptSelector);

		void enqueue(DiskIOFaceGuam::DiskIOCBType* iocb, DiskFile* diskFile);
		void process(DiskIOFaceGuam::DiskIOCBType* iocb, DiskFile* diskFile);

	private:
		class Item {
		public:
			DiskIOFaceGuam::DiskIOCBType* iocb;
			DiskFile*                     diskFile;

			Item(DiskIOFaceGuam::DiskIOCBType* iocb_, DiskFile* diskFile_) : iocb(iocb_), diskFile(diskFile_) {}
			Item(const Item& that) : iocb(that.iocb), diskFile(that.diskFile) {}
		};

		static int        stopThread;

		CARD16            interruptSelector;
		QMutex            ioMutex;
		QWaitCondition    ioCV;
		QLinkedList<Item> ioQueue;
	};

	IOThread ioThread;

	AgentDisk() : Agent(GuamInputOutput::disk, "Disk") {
		fcb = 0;
		dcb = 0;
	}

	CARD32 getFCBSize();

	void Initialize();
	void Call();

	void addDiskFile(DiskFile* diskFile);

private:
	DiskIOFaceGuam::DiskFCBType* fcb;
	DiskIOFaceGuam::DiskDCBType* dcb;
	QList<DiskFile*>             diskFileList;
};

#endif
