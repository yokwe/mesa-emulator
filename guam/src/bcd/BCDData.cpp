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
// BCDData.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("bcddata");

#include "../mesa/Memory.h"

#include "BCDData.h"

static const int MAX_BIT = 15;

CARD16 BCDData::asCARD16(CARD16 word, int startBit, int stopBit) {
	if (startBit < 0)        ERROR();
	if (stopBit  < 0)        ERROR();
	if (stopBit  < startBit) ERROR();
	if (MAX_BIT  < startBit) ERROR();
	if (MAX_BIT  < stopBit)  ERROR();

	int shift  = MAX_BIT - stopBit;
	int mask   = ((int)(1L << (stopBit - startBit + 1)) - 1) << shift;

	return (CARD16)((word & mask) >> shift);
}

BCDData::~BCDData() {}

CARD16 BCDData::getCARD16() {
	CARD16 b0 = getCARD8();
	CARD16 b1 = getCARD8();
	return (b0 << 8) | b1;
}
CARD32 BCDData::getCARD32() {
	CARD32 b0 = getCARD16();
	CARD32 b1 = getCARD16();
	return (b1 << 16) | b0;
}
void   BCDData::get(int size, CARD8* data) {
	for(int i = 0; i < size; i++) {
		data[i] = getCARD8();
	}
}

//
//
//

class FileBCDData : public BCDData {
public:
	FileBCDData(QString path) : file(path) {
		if (!file.exists()) {
			logger.fatal("File does not exist. path = %s", path.toLocal8Bit().constData());
			ERROR();
		}
		bool result = file.open(QIODevice::OpenModeFlag::ReadOnly);
		if (!result) {
			logger.fatal("File open error %s", file.errorString().toLocal8Bit().constData());
			ERROR();
		}
		capacity = file.size();
		logger.info("%s  %s  %d", __FUNCTION__, path.toLocal8Bit().constData(), capacity);
	}
	~FileBCDData() {
		if (file.isOpen()) file.close();
	}

	int getPosition() {
		return (int)file.pos();
	}
	void setPosition(int newPosition) {
		if (newPosition < 0) ERROR();
		if (capacity <= newPosition) ERROR();

		file.seek(newPosition);
	}
	CARD8 getCARD8() {
		char data;
		quint32 nRead = (quint32)file.read(&data, 1);
		if (nRead != 1) {
			logger.fatal("nRead %d", nRead);
			ERROR();
		}
		return (CARD8)data;
	}

private:
	QFile file;
	int   capacity;
};

BCDData* BCDData::getInstance(QString path) {
	return new FileBCDData(path);
}


//
// MesaMemoryBCDData
//

void PageFault(CARD32 ptr) {
	logger.fatal("%s %X", __FUNCTION__, ptr);
}
void WriteProtectFault(CARD32 ptr) {
	logger.fatal("%s %X", __FUNCTION__, ptr);
}

class MesaMemoryBCDData : public BCDData {
public:
	MesaMemoryBCDData(CARD32 ptr_) {
		ptr = ptr_;
		pos = 0;
		if (Memory::isVacant(ptr)) {
			logger.fatal("ptr is not mapped. ptr = %X", ptr);
			ERROR();
		}
	}
	~MesaMemoryBCDData() {
	}

	int getPosition() {
		return pos;
	}
	void setPosition(int newPosition) {
		if (newPosition < 0) ERROR();
		pos = (CARD32)newPosition;
	}
	CARD8 getCARD8() {
		CARD8 data = FetchByte(ptr, pos);
		pos++;
		return data;
	}

private:
	CARD32 ptr;
	CARD32 pos;
};
BCDData* BCDData::getInstance(CARD32 ptr) {
	return new MesaMemoryBCDData(ptr);
}
