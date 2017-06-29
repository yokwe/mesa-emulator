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
// main.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("main");

#include "../mesa/Memory.h"

#include "BCDOps.h"

void FrameFault(FSIndex fsi) {
	logger.fatal("fsi %d", fsi);
	ERROR();
}
void PageFault(LONG_POINTER ptr) {
	logger.fatal("ptr = %8X", ptr);
	ERROR();
}
void WriteProtectFault(LONG_POINTER ptr) {
	logger.fatal("ptr = %8X", ptr);
	ERROR();
}

int main(int /*argc*/, char** /*argv*/) {
	logger.info("START");
//	QString path("tmp/GermOpsImpl.bcd");
	QString path("tmp/UnpackedPilotKernel.bcd");
	quint32 vmBits = 22;
	quint32 rmBits = 20;
	CARD32  ptr = 0x10000;

	logger.info("vmBits = %2d  rmBits = %2d", vmBits, rmBits);
//	Memory::initialize(vmBits, rmBits, Agent::ioRegionPage);
	Memory::initialize(vmBits, rmBits, 0x80);

	quint32 size;
	logger.info("path %s", path.toLocal8Bit().constData());
	CARD16* p = (CARD16*)Util::mapFile(path, size);
	logger.info("size %u", size);
	quint32 nPage = (size + 511) / 512;
	logger.info("nPage %u", nPage);
	for(quint32 i = 0; i < nPage; i++) {
		CARD32 offset = 256 * i;
		CARD16* q = Memory::getAddress(ptr + offset);
		Util::toBigEndian(p + offset, q, 256);
	}
	Util::unmapFile(p);

	BCDOps bcd(ptr);

	logger.info("STOP");
	return 0;
}
