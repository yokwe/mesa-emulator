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
// MesaProcessor.h
//

#ifndef MESAPROCESSOR_H__
#define MESAPROCESSOR_H__

#include <QtCore>

#include "../agent/Agent.h"
#include "../agent/AgentBeep.h"
#include "../agent/AgentDisk.h"
#include "../agent/AgentDisplay.h"
#include "../agent/AgentFloppy.h"
#include "../agent/AgentKeyboard.h"
#include "../agent/AgentMouse.h"
#include "../agent/AgentNetwork.h"
#include "../agent/AgentProcessor.h"
#include "../agent/AgentStream.h"

#include "MesaThread.h"

class MesaProcessor {
public:
	// Number of thread used in QThreadPool
	static const int MAX_THREAD = 20;

	void setDiskPath(const QString& diskPath_) {
		diskPath = diskPath_;
	}
	void setGermPath(const QString& germPath_) {
		germPath = germPath_;
	}
	void setFloppyPath(const QString& floppyPath_) {
		floppyPath = floppyPath_;
	}
	void setBootSwitch(const QString& bootSwitch_) {
		bootSwitch = bootSwitch_;
	}
	void setMemorySize(int vmBits_, int rmBits_) {
		vmBits = vmBits_;
		rmBits = rmBits_;
	}
	void setDisplaySize(CARD16 displayWidth_, CARD16 displayHeight_) {
		displayWidth  = displayWidth_;
		displayHeight = displayHeight_;
	}
	void setNetworkInterfaceName(const QString& networkInterfaceName_) {
		networkInterfaceName = networkInterfaceName_;
	}

	void setBootRequestPV(CARD16 deviceOrdinal = 0);
	void setBootRequestEther(CARD16 deviceOrdinal = 0);
	void setBootRequestStream();

	void initialize();
	void boot();
	void stop();
	void wait();
	int  isRunning() {
		return getRunning();
	}

private:
	QString        diskPath;
	QString        germPath;
	QString        floppyPath;
	QString        bootSwitch;
	int            vmBits;
	int            rmBits;
	CARD16         displayWidth;
	CARD16         displayHeight;
	QString        networkInterfaceName;

	//
	DiskFile       diskFile;
	DiskFile       floppyFile;
	NetworkPacket  networkPacket;

	// agent
	AgentDisk      disk;
	AgentFloppy    floppy;
	AgentNetwork   network;
	//AgentParallel  parallel;
	AgentKeyboard  keyboard;
	AgentBeep      beep;
	AgentMouse     mouse;
	AgentProcessor processor;
	AgentStream    stream;
	//AgentSerial    serial;
	//AgentTTY       tty;
	AgentDisplay   display;
	//AgentReserved3 reserved3;

	ProcessorThread processorThread;
	InterruptThread interruptThread;
	TimerThread     timerThread;

	QAtomicInt     running;

	void loadGerm(QString& path);

	void setRunning(int newValue);
	int  getRunning();
};
#endif
