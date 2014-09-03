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
// DiskFile.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("diskfile");

#include "DiskFile.h"

void DiskFile::attach(const QString& path_) {
	path = path_;
	logger.info("DiskFile::attach %s", path.toLatin1().constData());

	page = (Page*)Util::mapFile(path, size);
}

void DiskFile::setDiskDCBType(DiskIOFaceGuam::DiskDCBType *dcb) {
	dcb->deviceType         = Device::T_anyPilotDisk;
	dcb->numberOfHeads      = DISK_NUMBER_OF_HEADS;
	dcb->sectorsPerTrack    = DISK_SECTORS_PER_TRACK;
	dcb->numberOfCylinders  = size / (dcb->numberOfHeads * dcb->sectorsPerTrack * sizeof(Page));
	dcb->agentDeviceData[0] = 0;
	dcb->agentDeviceData[1] = 0;
	dcb->agentDeviceData[2] = 0;
	dcb->agentDeviceData[3] = 0;
	dcb->agentDeviceData[4] = 0;
	dcb->agentDeviceData[5] = 0;

	if (size != (CARD32)(dcb->numberOfHeads * dcb->sectorsPerTrack * dcb->numberOfCylinders * sizeof(Page))) ERROR();
}

void DiskFile::setFloppyDCBType(FloppyIOFaceGuam::FloppyDCBType *dcb) {
	dcb->deviceType         = Device::T_microFloppy;
	dcb->numberOfHeads      = FLOPPY_NUMBER_OF_HEADS;
	dcb->sectorsPerTrack    = FLOPPY_SECTORS_PER_TRACK;
	dcb->numberOfCylinders  = FLOPPY_NUMBER_OF_CYLINDERS;
	dcb->ready              = 1;
	dcb->diskChanged        = 1;
	dcb->twoSided           = 1;
	dcb->suggestedTries     = 1;

	if (size != (CARD32)(dcb->numberOfHeads * dcb->sectorsPerTrack * dcb->numberOfCylinders * sizeof(Page))) ERROR();
}
