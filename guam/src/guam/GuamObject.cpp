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
// GuamObject.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("guamobject");

#include "GuamObject.h"

#include "../mesa/Memory.h"
#include "../opcode/Interpreter.h"
#include "../opcode/HotSpot.h"

GuamObject::GuamObject() {
	preference = new Preference();
	section    = preference->getAsString("Main", "Section");
	logger.info("Section = %s", qPrintable(section));

	displayWidth  = preference->getAsUINT32(section, "DisplayWidth");
	displayHeight = preference->getAsUINT32(section, "DisplayHeight");
}

void GuamObject::init() {
	QString diskPath         = preference->getAsString(section, "DiskPath");
	QString germPath         = preference->getAsString(section, "GermPath");
	QString bootSwitch       = preference->getAsString(section, "BootSwitch");
	QString networkInterface = preference->getAsString(section, "NetworkInterface");

	quint32 vmBits           = preference->getAsUINT32(section, "VMBits");
	quint32 rmBits           = preference->getAsUINT32(section, "RMBits");

	mesaProcessor.setDiskPath(diskPath);
	mesaProcessor.setGermPath(germPath);
	mesaProcessor.setBootSwitch(bootSwitch);

	mesaProcessor.setMemorySize(vmBits, rmBits);
	mesaProcessor.setDisplaySize(displayWidth, displayHeight);
	mesaProcessor.setNetworkInterfaceName(networkInterface);

	//extern void initTraceCallRegist_Dawn();
	//initTraceCallRegist_Dawn();
	mesaProcessor.initialize();
}

QImage* GuamObject::getDisplayImage() {
	uchar* data = (uchar*)Memory::getDisplayPage()->word;
	CARD32 bytesPerLine = Memory::getDisplayBytesPerLine();

	QImage* image = new QImage(data, displayWidth, displayHeight, bytesPerLine, QImage::Format_Mono);
	// 0 => white  1 => black
	for(CARD32 i = 0; i < (displayHeight * bytesPerLine); i++) data[i] = 1;

	return image;
}

void GuamObject::startEmulator() {
	mesaProcessor.boot();
	mesaProcessor.wait();

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

	HotSpot::stats();
	//
	emulatorStopped();
}
