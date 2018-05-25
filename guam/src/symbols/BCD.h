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

#include <QMap>

// Forward declaration of classes
class BCD;
class BCDFile;

namespace bcd {
	static const CARD16 T_LIMIT = 0177777;
}


// Stamp: TYPE = RECORD [net, host: [0..377B], time: LONG CARDINAL];
// Null: Stamp = Stamp[net: 0, host: 0, time: 0];
class Stamp {
public:
	static Stamp* getInstance(BCD* bcd);
	static Stamp* getNull();

	const CARD16    net;
	const CARD16    host;
	const CARD32    time;
	const QDateTime dateTime;

	QString toString() const;
	bool isNull() const {
		return net == 0 && host == 0 && time == 0;
	}

private:
	Stamp(CARD16 net_, CARD16 host_, CARD32 time_, QDateTime dateTime_) :
		net(net_), host(host_), time(time_), dateTime(dateTime_) {}
};

// NameRecord: TYPE = RECORD [CARDINAL];
// NullName: NameRecord = [1];
class NameRecord {
public:
	static const CARD16 NullName = 1;

	const CARD16  index;
	const QString name;

	NameRecord(CARD16 index_, QString name_) : index(index_), name(name_) {}
	NameRecord() : index(NullName), name("#NULL#") {}

	QString toString();
};


// FTRecord: TYPE = RECORD [name: NameRecord, version: VersionStamp];
class FTRecord {
public:
	// FTIndex: TYPE = Table.Base RELATIVE POINTER [0..tLimit] TO FTRecord;
	// FTNull: FTIndex = LAST[FTIndex];
	// FTSelf: FTIndex = LAST[FTIndex] - 1;
	static const CARD16 FT_NULL = bcd::T_LIMIT;
	static const CARD16 FT_SELF = bcd::T_LIMIT - 1;

	static FTRecord* getInstance(BCD* bcd, CARD16 index);
	static FTRecord* getNull();

	const CARD16  index;
	const QString name;
	const Stamp*  version;

	FTRecord(CARD16 index_, QString name_, Stamp* version_) : index(index_), name(name_), version(version_) {}

	bool isNull() const {
		return index == FT_NULL;
	}
	bool isSelf() const {
		return index == FT_SELF;
	}
	QString toString() const;

private:
	FTRecord() : index(FT_NULL), name(), version(0) {}
};

//  -- Segment Table
//
// SGRecord: TYPE = RECORD [
//   file: FTIndex, base: CARDINAL,
//   pages: [0..256), extraPages: [0..64), class: SegClass];
class SGRecord {
public:
	// SGIndex: TYPE = Table.Base RELATIVE POINTER [0..tLimit] TO SGRecord;
	// SGNull: SGIndex = LAST[SGIndex];
	static const CARD16 SG_NULL = bcd::T_LIMIT;

	// SegClass: TYPE = {code, symbols, acMap, other};
	enum class SegClass {
		CODE, SYMBOLS, AC_MAP, OTHER,
	};
	static QString toString(SegClass value);

	static SGRecord* getInstance(BCD* bcd, CARD16 index);
	static SGRecord* getNull();

	const CARD16    index;
	const CARD16    fileIndex;
	const FTRecord* file;
	const CARD16    base;
	const CARD16    pages;
	const CARD16    extraPages;
	const SegClass  segClass;

	QString toString() const;

private:
	SGRecord(CARD16 index_, CARD16 fileIndex_, FTRecord* file_, CARD16 base_, CARD16 pages_, CARD16 extraPages_, SegClass segClass_) :
		index(index_), fileIndex(fileIndex_), file(file_), base(base_), pages(pages_), extraPages(extraPages_), segClass(segClass_) {}
};


//-- Entry Vector Table
//
//ENRecord: TYPE = RECORD [
//   nEntries: CARDINAL, initialPC: ARRAY [0..0) OF PrincOps.BytePC];
//
//ENIndex: TYPE = Table.Base RELATIVE POINTER [0..tLimit] TO ENRecord;
//ENNull: ENIndex = LAST[ENIndex];
class ENRecord {
public:
	static const CARD16 EN_NULL = bcd::T_LIMIT;

	static ENRecord* getInstance(BCD* bcd, CARD16 index);
	static ENRecord* getNull();

	const CARD16          index;
	const QVector<CARD16> initialPC;

	QString toString() const;

private:
	ENRecord(CARD16 index_, QVector<CARD16> initialPC_) : index(index_), initialPC(initialPC_) {}
};


//CodeDesc: TYPE = RECORD [
//  sgi: SGIndex, offset, length: CARDINAL];
class CodeDesc {
public:
	static CodeDesc* getInstance(BCD* bcd);

	const SGRecord* sgi;
	const CARD16    offset;
	const CARD16    length;

	QString toString() const;
private:
	CodeDesc(SGRecord* sgi_, CARD16 offset_, CARD16 length_) : sgi(sgi_), offset(offset_), length(length_) {}
};


// LinkLocation: TYPE = {frame, code, dontcare};
//enum class LinkLocation {
//	FRAME, CODE, DONTCARE,
//};
//QString toString(LinkLocation value);


//MTRecord: TYPE = --MACHINE DEPENDENT-- RECORD [
//  name: NameRecord,
//  file: FTIndex,
//  config: CTIndex,
//  code: CodeDesc,
//  sseg: SGIndex,
//  links: LFIndex,
//  linkLoc: LinkLocation,
//  namedInstance, initial: BOOLEAN,
//  boundsChecks, nilChecks: BOOLEAN,
//  tableCompiled, residentFrame, crossJumped, packageable: BOOLEAN,
//  packed: BOOLEAN, linkspace: BOOLEAN,
//  spare: PACKED ARRAY [0..4) OF BOOLEAN,
//  framesize: [0..PrincOps.MaxFrameSize),
//  entries: ENIndex,
//  atoms: ATIndex];
//
//MTIndex: TYPE = Table.Base RELATIVE POINTER [0..tLimit] TO MTRecord;
//MTNull: MTIndex = LAST[MTIndex];
class MTRecord {
public:
	static const CARD16 MT_NULL = bcd::T_LIMIT;

	static MTRecord* getInstance(BCD* bcd, CARD16 index);
	static MTRecord* getNull();

	const CARD16    index;
	const QString   name;
	const FTRecord* file;
	const CARD16    config;
	const CodeDesc* code;
	const SGRecord* sseg;
	const CARD16    links;
//	const CARD16    linkLoc;
//	const bool      namedInstance;
//	const bool      initial;
//	const bool      boundsChecks;
//	const bool      nilChecks;
//	const bool      tableCompiled;
//	const bool      residentFrame;
//	const bool      crossJumped;
//	const bool      packageable;
//	const bool      packed;
//	const bool      linkspace;
//	const bool      spare0;
//	const bool      spare1;
//	const bool      spare2;
//	const bool      spare3;
	const CARD16    framesize;
	const ENRecord* entries;
	const CARD16    atoms;

	QString toString() const;
private:
	MTRecord(
		CARD16 index_, QString name_, FTRecord* file_, CARD16 config_,
		CodeDesc* code_, SGRecord* sseg_, CARD16 links_,
//		CARD16 linkLoc_,
//		bool namedInstance_, bool initial_, bool boundsChecks_, bool nilChecks_,
//		bool tableCompiled_, bool residentFrame_, bool crossJumped_, bool packageable_, bool packed_, bool linkspace_,
//		bool spare0_, bool spare1_, bool spare2_, bool spare3_,
		CARD16 framesize_, ENRecord* entries_, CARD16 atoms_) :
			index(index_), name(name_), file(file_), config(config_),
			code(code_), sseg(sseg_), links(links_),
//			linkLoc(linkLoc_),
//			namedInstance(namedInstance_), initial(initial_), boundsChecks(boundsChecks_), nilChecks(nilChecks_), tableCompiled(tableCompiled_),
//			residentFrame(residentFrame_), crossJumped(crossJumped_), packageable(packageable_), packed(packed_), linkspace(linkspace_),
//			spare0(spare0_), spare1(spare1_), spare2(spare2_), spare3(spare3_),
			framesize(framesize_), entries(entries_), atoms(atoms_) {}
};


class BCD {
public:
	//VersionID: CARDINAL = 6103
	static const CARD16 VersionID = 6103;

	BCD(BCDFile* bcdFile);

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

	BCDFile*  file;

	CARD16    versionIdent;
	Stamp*    version;
	Stamp*    creator;
	FTRecord* sourceFile;
	FTRecord* unpackagedFile;
//	CARD16    sourceFile;
//	CARD16    unpackagedFile;
	CARD16    nConfigs;
	CARD16    nModules;
	CARD16    nImports;
	CARD16    nExports;
	CARD16    nPages;
	bool      definitions;
	bool      repackaged;
	bool      typeExported;
	bool      tableCompiled;
	CARD16    spare4;
	CARD16    firstDummy;
	CARD16    nDummies;
	CARD16    ssOffset; // string table
	CARD16    ssLimit;
	CARD16    ctOffset; // config table
	CARD16    ctLimit;
	CARD16    mtOffset; // module table
	CARD16    mtLimit;
	CARD16    impOffset; // import table
	CARD16    impLimit;
	CARD16    expOffset; // export table
	CARD16    expLimit;
	CARD16    enOffset; // entry table
	CARD16    enLimit;
	CARD16    sgOffset; // segment table
	CARD16    sgLimit;
	CARD16    ftOffset; // file table
	CARD16    ftLimit;
	CARD16    spOffset; // space table
	CARD16    spLimit;
	CARD16    ntOffset; // name table
	CARD16    ntLimit;
	CARD16    typOffset; // type table
	CARD16    typLimit;
	CARD16    tmOffset; // type map table
	CARD16    tmLimit;
	CARD16    fpOffset; // frame pack table
	CARD16    fpLimit;
	CARD16    lfOffset; // link fragment table
	CARD16    lfLimit;
	CARD16    atOffset; // atom table
	CARD16    atLimit;
	CARD16    apOffset; // atom printname table
	CARD16    apLimit;

	QMap<CARD16, NameRecord*> ss;
	QMap<CARD16, FTRecord*>   ft;
//	QMap<CARD16, CTRecord*>   ct;
	QMap<CARD16, SGRecord*>   sg;
//	QMap<CARD16, TYPRecord*>  typ;
//	QMap<CARD16, LinkFrag*>   lf;
	QMap<CARD16, ENRecord*>   en;
//	QMap<CARD16, ATRecord*>   at;
	QMap<CARD16, MTRecord*>   mt;

private:
	void initializeNameRecord();
	void initializeFTRecord();
	void initializeSGRecord();
	void initializeENRecord();
	void initializeMTRecord();
};

#endif
