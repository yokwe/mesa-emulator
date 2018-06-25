/*
Copyright (c) 2018, Yasuhiro Hasegawa
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
// ModuleInfo.h
//

#ifndef MODULEINFO_H__
#define MODULEINFO_H__

#include "../util/Util.h"
#include "../mesa/MesaBasic.h"

#include "../symbols/BCD.h"

class ModuleInfo {
public:
	const quint64 bcdVersion;
	const QString bcdPath;

	const QString name;

	const quint64 fileVersion;
	const QString fileName;

	const CARD16  codeOffset;
	const CARD16  codeLength;
	const CARD16  frameSize;

	const QVector<CARD16> entries; // key is entryIndex and value is initialPC point to fsi

	QString toString() const;

	static ModuleInfo* getInstance(BCD& bcd, MTRecord& mt);

	// read json file and return instance
	static ModuleInfo* readJson(QString& path);

	// write values in instance to file
	void writeJson(QString& path) const;

private:
	ModuleInfo(quint64 bcdVersion_, QString bcdPath_, QString name_, quint64 fileVersion_, QString fileName_, CARD16 codeOffset_, CARD16 codeLength_, CARD16 frameSize_, QVector<CARD16> entries_) :
		bcdVersion(bcdVersion_), bcdPath(bcdPath_), name(name_), fileVersion(fileVersion_), fileName(fileName_), codeOffset(codeOffset_), codeLength(codeLength_), frameSize(frameSize_), entries(entries_) {}
};

class BCDInfo {
public:
	const quint64 bcdVersion;
	const QString bcdPath;

	const bool    defnitions;

	const QVector<ModuleInfo*> mt;
};

#endif
