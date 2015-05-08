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
// LoadGerm.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("LoadGerm");

#include "../util/Perf.h"

#include "../util/Preference.h"

#include "../util/GuiOp.h"

#include "../mesa/MesaProcessor.h"
#include "../mesa/Memory.h"

#include "../opcode/Interpreter.h"

#include <QtCore>

int main() {
	logger.info("START");

	GuiOp::setContext(new NullGuiOp);

	Preference preference;

	QString group            = preference.getAsString("Main", "Section");
	logger.info("Section = %s", qPrintable(group));

	CARD32  displayWidth     = preference.getAsUINT32(group, "DisplayWidth");
	CARD32  displayHeight    = preference.getAsUINT32(group, "DisplayHeight");

	QString diskPath         = preference.getAsString(group, "DiskPath");
	QString germPath         = preference.getAsString(group, "GermPath");
	QString bootSwitch       = preference.getAsString(group, "BootSwitch");
	QString networkInterface = preference.getAsString(group, "NetworkInterface");

	quint32 vmBits           = preference.getAsUINT32(group, "VMBits");
	quint32 rmBits           = preference.getAsUINT32(group, "RMBits");

	// stop at MP 8000
	perf_stop_at_mp_8000 = 1;

	MesaProcessor mesaProcessor;

	mesaProcessor.setDiskPath(diskPath);
	mesaProcessor.setGermPath(germPath);
	mesaProcessor.setBootSwitch(bootSwitch);

	mesaProcessor.setMemorySize(vmBits, rmBits);
	mesaProcessor.setDisplaySize(displayWidth, displayHeight);
	mesaProcessor.setNetworkInterfaceName(networkInterface);

	mesaProcessor.initialize();

	// measure elapsed time between boot and MP8000
	QElapsedTimer elapsedTimer;
	elapsedTimer.start();
	mesaProcessor.boot();
	mesaProcessor.wait();
	quint64 elapsedTime = elapsedTimer.nsecsElapsed();

	Interpreter::stats();
	Run::stats();
	Perf_log();
	PageCache::statsAll();
	MDSCache::statsAll();
	CodeCache::stats();

	//extern void MonoBlt_MemoryCache_stats();
	//MonoBlt_MemoryCache_stats();

	//extern void MonoBlt_stats();
	//MonoBlt_stats();

	logger.info("elapsedTime = %lld msec", elapsedTime / (1000 * 1000)); // display as milliseconds

	logger.info("STOP");
	return 0;
}
