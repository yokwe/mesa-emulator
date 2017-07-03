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
// BCD.h
//

#ifndef BCD_H__
#define BCD_H__

#include "../util/Util.h"
#include "../mesa/MesaBasic.h"

#include "BCDFile.h"

#include <QMap>

class BCD;
static const CARD16 T_LIMIT = 0177777;

CARD16 bitField(CARD16 word, int startBit, int stopBit);
CARD16 bitField(CARD16 word, int startBit);


// Stamp: TYPE = RECORD [net, host: [0..377B], time: LONG CARDINAL];
// Null: Stamp = Stamp[net: 0, host: 0, time: 0];
class VersionStamp {
public:
	VersionStamp() : net(0), host(0), time(0), dateTime() {}
	VersionStamp(const VersionStamp& that) : net(that.net), host(that.host), time(that.time), dateTime(that.dateTime) {}
	VersionStamp(BCD& bcd);

	QString toString();

private:
	CARD16    net;
	CARD16    host;
	CARD32    time;
	QDateTime dateTime;
};

// NameRecord: TYPE = RECORD [CARDINAL];
// NullName: NameRecord = [1];
class NameRecord {
public:
	static const CARD16 NullName = 1;

	CARD16  index;
	QString name;

	NameRecord(CARD16 index_, QString name_) : index(index_), name(name_) {}
	NameRecord() : index(NullName), name("#NULL#") {}

	QString toString();
};


// FTRecord: TYPE = RECORD [name: NameRecord, version: VersionStamp];
class FTRecord {
public:
	// FTNull: FTIndex = LAST[FTIndex];
	// FTSelf: FTIndex = LAST[FTIndex] - 1;
	static const CARD16 FTNull = T_LIMIT;
	static const CARD16 FTSelf = T_LIMIT - 1;

	CARD16       index;
	NameRecord   name;
	VersionStamp version;

	FTRecord() : index(FTNull), name(), version() {}
	FTRecord(const FTRecord& that) : index(that.index), name(that.name), version(that.version) {}
	FTRecord(BCD& bcd, CARD16 index);

	QString toString();
};

class BCD {
public:
	//VersionID: CARDINAL = 6103
	static const CARD16 VersionID = 6103;

	BCD(BCDFile& bcdFile);

	//BCD: TYPE = RECORD [
	//  versionIdent: CARDINAL,
	//  version: VersionStamp,
	//  creator: VersionStamp,
	//  sourceFile: FTIndex,
	//  unpackagedFile: FTIndex,
	//  nConfigs, nModules: CARDINAL,
	//  nImports, nExports: CARDINAL,
	//  nPages: [0..256),
	//  definitions, repackaged, typeExported, tableCompiled: BOOLEAN,
	//  spare: PACKED ARRAY [0..4) OF BOOLEAN,
	//  firstdummy: GFIndex,
	//  nDummies: CARDINAL,
	//  ssOffset; // string table
	//  ssLimit: CARDINAL,
	//  ctOffset; // config table
	//  ctLimit: CTIndex,
	//  mtOffset; // module table
	//  mtLimit: MTIndex,
	//  impOffset; // import table
	//  impLimit: IMPIndex,
	//  expOffset; // export table
	//  expLimit: EXPIndex,
	//  enOffset; // entry table
	//  enLimit: ENIndex,
	//  sgOffset; // segment table
	//  sgLimit: SGIndex,
	//  ftOffset; // file table
	//  ftLimit: FTIndex,
	//  spOffset; // space table
	//  spLimit: SPIndex,
	//  ntOffset; // name table
	//  ntLimit: NTIndex,
	//  typOffset; // type table
	//  typLimit: TYPIndex,
	//  tmOffset; // type map table
	//  tmLimit: TMIndex,
	//  fpOffset; // frame pack table
	//  fpLimit: FPIndex,
	//  lfOffset; // link fragment table
	//  lfLimit: LFIndex,
	//  atOffset; // atom table
	//  atLimit: ATIndex,
	//  apOffset; // atom printname table
	//  apLimit: CARDINAL];

	BCDFile&     file;

	CARD16       versionIdent;
	VersionStamp version;
	VersionStamp creator;
	FTRecord     sourceFile;
	FTRecord     unpackagedFile;
//	CARD16       sourceFile;
//	CARD16       unpackagedFile;
	CARD16       nConfigs;
	CARD16       nModules;
	CARD16       nImports;
	CARD16       nExports;
	CARD16       nPages;
	bool         definitions;
	bool         repackaged;
	bool         typeExported;
	bool         tableCompiled;
	CARD16       spare4;
	CARD16       firstDummy;
	CARD16       nDummies;
	CARD16       ssOffset; // string table
	CARD16       ssLimit;
	CARD16       ctOffset; // config table
	CARD16       ctLimit;
	CARD16       mtOffset; // module table
	CARD16       mtLimit;
	CARD16       impOffset; // import table
	CARD16       impLimit;
	CARD16       expOffset; // export table
	CARD16       expLimit;
	CARD16       enOffset; // entry table
	CARD16       enLimit;
	CARD16       sgOffset; // segment table
	CARD16       sgLimit;
	CARD16       ftOffset; // file table
	CARD16       ftLimit;
	CARD16       spOffset; // space table
	CARD16       spLimit;
	CARD16       ntOffset; // name table
	CARD16       ntLimit;
	CARD16       typOffset; // type table
	CARD16       typLimit;
	CARD16       tmOffset; // type map table
	CARD16       tmLimit;
	CARD16       fpOffset; // frame pack table
	CARD16       fpLimit;
	CARD16       lfOffset; // link fragment table
	CARD16       lfLimit;
	CARD16       atOffset; // atom table
	CARD16       atLimit;
	CARD16       apOffset; // atom printname table
	CARD16       apLimit;

	QMap<CARD16, NameRecord> ss;
	QMap<CARD16, FTRecord>   ft;
//	QMap<CARD16, CTRecord>   ct;
//	QMap<CARD16, SGRecord>   sg;
//	QMap<CARD16, TYPRecord>  typ;
//	QMap<CARD16, LinkFrag>   lf;
//	QMap<CARD16, ENRecord>   en;
//	QMap<CARD16, ATRecord>   at;
//	QMap<CARD16, MTRecord>   mt;

private:
	void initializeNameRecord();
	void initializeFTRecord();
};

#endif
