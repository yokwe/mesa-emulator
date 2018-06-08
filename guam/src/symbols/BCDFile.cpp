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
// BCDFile.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("bcdfile");

#include "../mesa/Memory.h"

#include "BCDFile.h"

#include "BCD.h"
#include "Symbols.h"

BCDFile::~BCDFile() {}

CARD16 BCDFile::getCARD16() {
	CARD16 b0 = getCARD8();
	CARD16 b1 = getCARD8();
	return (b0 << 8) | b1;
}
CARD32 BCDFile::getCARD32() {
	CARD32 b0 = getCARD16();
	CARD32 b1 = getCARD16();
	return (b1 << 16) | b0;
}
void   BCDFile::get(int size, CARD8* data) {
	for(int i = 0; i < size; i++) {
		data[i] = getCARD8();
	}
}

bool BCDFile::isBCDFile() {
	int oldPosition = getPosition();

    position(0);
	CARD16 word0 = getCARD16();

	setPosition(oldPosition);
	return word0 == BCD::VersionID;
}
bool BCDFile::isSymbolsFile() {
	int oldPosition = getPosition();

	position(Environment::wordsPerPage);
    CARD16 word256 = getCARD16();

	setPosition(oldPosition);
    return word256 == Symbols::VersionID;
}


//
// BCDFileFile
//

class BCDFileFile : public BCDFile {
public:
	BCDFileFile(QString path) : file(path) {
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
	~BCDFileFile() {
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

BCDFile* BCDFile::getInstance(QString path) {
	return new BCDFileFile(path);
}


//
// BCDFileMesaMemory
//

//void PageFault(CARD32 ptr) {
//	logger.fatal("%s %X", __FUNCTION__, ptr);
//}
//void WriteProtectFault(CARD32 ptr) {
//	logger.fatal("%s %X", __FUNCTION__, ptr);
//}

class BCDFileMesaMemory : public BCDFile {
public:
	BCDFileMesaMemory(CARD32 ptr_) {
		ptr = ptr_;
		pos = 0;
		if (Memory::isVacant(ptr)) {
			logger.fatal("ptr is not mapped. ptr = %X", ptr);
			ERROR();
		}
	}
	~BCDFileMesaMemory() {
	}

	int getPosition() {
		return pos;
	}
	void setPosition(int newPosition) {
		if (newPosition < 0) ERROR();
		pos = (CARD32)newPosition;
	}
	CARD8 getCARD8() {
		CARD32 p = ptr + (pos / 2);
		if (Memory::isVacant(p)) {
//			logger.fatal("p is not mapped. p = %X", p);
//			logBackTrace();
			ERROR_Abort();
		}
		BytePair word = {*Fetch(p)};
		CARD8 data =  ((pos % 2) == 0) ? (BYTE)word.left : (BYTE)word.right;

		pos++;
		return data;
	}

private:
	CARD32 ptr;
	CARD32 pos;
};
BCDFile* BCDFile::getInstance(CARD32 ptr) {
	return new BCDFileMesaMemory(ptr);
}
