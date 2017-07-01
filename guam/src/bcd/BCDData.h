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
// BCDData.h
//

#ifndef BCDDATA_H__
#define BCDDATA_H__

#include "../util/Util.h"
#include "../mesa/MesaBasic.h"

class BCDData {
public:
	static const int BYTES_PER_WORD = 2;

	// File file
	static BCDData* getInstance(QString path);
	// From mesa memory
	static BCDData* getInstance(CARD32 ptr);


	// Mesa field extraction
	static CARD16 asCARD16(CARD16 word, int startBit, int stopBit);
    static bool asBool(int word, int startBit, int stopBit) {
        return asCARD16(word, startBit, stopBit) != 0;
    }
    static bool asBool(int word, int startBit) {
        return asCARD16(word, startBit, startBit);
    }

    virtual ~BCDData() = 0;

	int  bytePosition() {
		return getPosition();
	}
	int  position() {
		return getPosition() / BYTES_PER_WORD;
	}
	void position(int newPositon) {
		setPosition(newPositon * BYTES_PER_WORD);
	}
	void rewind() {
		setPosition(0);
	}

	virtual int    getPosition()                = 0;
	virtual void   setPosition(int newPosition) = 0;
	virtual CARD8  getCARD8() = 0;

	CARD16 getCARD16();
	CARD32 getCARD32();
	INT16  getINT16() {
		return (INT16)getCARD16();
	}
	INT32  getINT32() {
		return (INT32)getCARD32();
	}
	void   get(int size, CARD8* data);
};

#endif

