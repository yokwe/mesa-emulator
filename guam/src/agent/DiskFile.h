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
	struct Page { CARD16 word[PageSize]; };

	DiskFile() {
		page = 0;
		size = 0;
	}

	void attach(const QString& path);

	void readPage(CARD32 block, CARD16 *buffer) {
		memcpy(buffer, page + block, sizeof(Page));
	}

	void writePage(CARD32 block, CARD16 *buffer) {
		memcpy(page + block, buffer, sizeof(Page));
	}

	int verifyPage(CARD32 block, CARD16 *buffer) {
		return memcmp(page + block, buffer, sizeof(Page));
	}

	void setDiskDCBType(DiskIOFaceGuam::DiskDCBType *dcb);
	void setFloppyDCBType(FloppyIOFaceGuam::FloppyDCBType *dcb);

	CARD32 getDiskSize() {
		return size / PageSize;
	}

	const QString& getPath() {
		return path;
	}

	CARD32 getBlock(DiskIOFaceGuam::DiskIOCBType* iocb) {
		const CARD32 C = iocb->diskAddress.cylinder;
		const CARD32 H = iocb->diskAddress.head;
		const CARD32 S = iocb->diskAddress.sector;
		CARD32 block = ((C * DISK_NUMBER_OF_HEADS + H) * DISK_SECTORS_PER_TRACK + S);

		return block;
	}

	CARD32 getBlock(FloppyIOFaceGuam::FloppyIOCBType* iocb) {
		const CARD32 C = iocb->operation.address.cylinder;
		const CARD32 H = iocb->operation.address.head;
		const CARD32 S = iocb->operation.address.sector;
		CARD32 block = ((C * FLOPPY_NUMBER_OF_HEADS + H) * FLOPPY_SECTORS_PER_TRACK + S);

		return block;
	}

private:
	static const CARD32 DISK_NUMBER_OF_HEADS       =  2;
	static const CARD32 DISK_SECTORS_PER_TRACK     = 16;

	static const CARD32 FLOPPY_NUMBER_OF_HEADS     =  2;
	static const CARD32 FLOPPY_SECTORS_PER_TRACK   = 18;
	static const CARD32 FLOPPY_NUMBER_OF_CYLINDERS = 80;

	QString path;
	Page  *page;
	CARD32 size;
};

#endif
