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
// DiskFile.h
//

#ifndef DISKFILE_H__
#define DISKFILE_H__

#include "../mesa/Constant.h"

#include <QtCore>

class DiskFile {
public:
	static const CARD32 DISK_NUMBER_OF_HEADS       =  2;
	static const CARD32 DISK_SECTORS_PER_TRACK     = 16;

	static const CARD32 FLOPPY_NUMBER_OF_HEADS     =  2;
	static const CARD32 FLOPPY_SECTORS_PER_TRACK   = 18;

	struct Page { CARD16 word[PageSize]; };

	// default constructor
	DiskFile() {
		page              = 0;
		size              = 0;
		maxBlock          = 0;
		numberOfCylinders = 0;
		numberOfHeads     = 0;
		sectorsPerTrack   = 0;
	}

	void attach(const QString& path);
	void detach();

	void readPage(CARD32 block, CARD16 *buffer) {
		readPage(block, buffer, SIZE(Page));
	}
	void readPage(CARD32 block, CARD16 *buffer, CARD32 sizeInWord);

	void writePage(CARD32 block, CARD16 *buffer) {
		writePage(block, buffer, SIZE(Page));
	}
	void writePage(CARD32 block, CARD16 *buffer, CARD32 sizeInWord);

	void zeroPage(CARD32 block);

	int verifyPage(CARD32 block, CARD16 *buffer);

	void setDiskDCBType(DiskIOFaceGuam::DiskDCBType *dcb);
	void setFloppyDCBType(FloppyIOFaceGuam::FloppyDCBType *dcb);

	CARD32 getBlockSize() {
		return size / Environment::bytesPerPage;
	}

	const QString& getPath() {
		return path;
	}

	CARD32 getBlock(DiskIOFaceGuam::DiskIOCBType* iocb) {
		const CARD32 C = iocb->diskAddress.cylinder;
		const CARD32 H = iocb->diskAddress.head;
		const CARD32 S = iocb->diskAddress.sector;
		CARD32 block = ((C * numberOfHeads + H) * sectorsPerTrack + S);

		return block;
	}

	CARD32 getBlock(FloppyIOFaceGuam::FloppyIOCBType* iocb) {
		const CARD32 C = iocb->operation.address.cylinder;
		const CARD32 H = iocb->operation.address.head;
		// Sector in Floppy is [1..maxSectorsPerTrack)
		const CARD32 S = iocb->operation.address.sector - 1;
		CARD32 block = (C * numberOfHeads + H) * sectorsPerTrack + S;

		return block;
	}

private:
	QString path;
	Page  *page;
	CARD32 size;
	CARD32 maxBlock;
	//
	CARD32 numberOfCylinders;
	CARD32 numberOfHeads;
	CARD32 sectorsPerTrack;
};

#endif
