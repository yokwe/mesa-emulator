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
// Symbols.h
//

#ifndef SYMBOLS_H__
#define SYMBOLSS_H__

#include "../util/Util.h"
#include "../mesa/MesaBasic.h"

#include "BCD.h"

// Forward declaration of classes
class Symbols;

class BTIndex;
class BTRecord;
class CTXIndex;
class CTXRecord;
class ExtIndex;
class ExtRecord;
class HTIndex;
class HTRecord;
class LTIndex;
class LTRecord;
class MDIndex;
class MDRecord;
class SEIndex;
class SERecord;
class TreeNode;

namespace symbols {
	//ByteLength: CARDINAL = 8;
	static const CARD16 BYTE_LENGTH = 8;
	//WordLength: CARDINAL = 16;
	static const CARD16 WORD_LENGTH = 16;

	//Base: TYPE = Table.Base;
	//Limit: CARDINAL = Table.Limit;
	static const CARD16 LIMIT = 040000;

	//
	//-- semantic entry table declarations
	//
	//TypeClass: TYPE = {
	//  mode,
	//  basic,
	//  enumerated,
	//  record,
	//  ref,
	//  array,
	//  arraydesc,
	//  transfer,
	//  definition,
	//  union,
	//  sequence,
	//  relative,
	//  subrange,
	//  long,
	//  real,
	//  opaque,
	//  zone,
	//  any,
	//  nil,
	//  bits,
	//  fixedSequence};
	//
	//TransferMode: TYPE = {proc, port, signal, error, process, program, none};
	//
	//Unspec: TYPE [SIZE[CARDINAL]];
	//
	//SERecord: TYPE = RECORD [
	//  mark3, mark4: BOOLEAN,
	//  body: SELECT seTag: * FROM
	//    id => [
	//	extended: BOOLEAN,
	//	public: BOOLEAN,
	//	idCtx: CTXIndex,
	//	immutable, constant: BOOLEAN,
	//	idType: SEIndex,
	//	idInfo: Unspec,
	//	idValue: Unspec,
	//	hash: HTIndex,
	//	linkSpace: BOOLEAN,
	//	ctxLink: SELECT linkTag: * FROM
	//	  terminal => [],
	//	  sequential => [],
	//	  linked => [link: ISEIndex]
	//	  ENDCASE],
	//    cons => [
	//	typeInfo: SELECT typeTag: TypeClass FROM
	//	  mode => [],
	//	  basic => [
	//	    ordered: BOOLEAN,
	//	    code: [0..16),
	//	    length: BitLength],
	//	  enumerated => [
	//	    ordered, machineDep: BOOLEAN,
	//	    unpainted: BOOLEAN,		-- un- for backward compatiblity
	//	    sparse: BOOLEAN,
	//	    valueCtx: CTXIndex,
	//	    nValues: CARDINAL],
	//	  record => [
	//	    hints: RECORD [
	//	      comparable, assignable: BOOLEAN,
	//	      unifield, variant, privateFields: BOOLEAN,
	//	      refField, default, voidable: BOOLEAN],
	//	    length: BitLength,
	//	    argument, monitored, machineDep: BOOLEAN,
	//	    painted: BOOLEAN,
	//	    fieldCtx: CTXIndex,
	//	    linkPart: SELECT linkTag: * FROM
	//	      notLinked => [],
	//	      linked => [linkType: SEIndex]
	//	      ENDCASE],
	//	  ref => [
	//	    counted, ordered, readOnly, list, var, basing: BOOLEAN,
	//	    refType: SEIndex],
	//	  array => [
	//	    packed: BOOLEAN,
	//	    indexType: SEIndex,
	//	    componentType: SEIndex],
	//	  arraydesc => [
	//	    var, readOnly: BOOLEAN,
	//	    describedType: SEIndex],
	//	  transfer => [
	//	    safe: BOOLEAN,
	//	    mode: TransferMode,
	//	    typeIn, typeOut: CSEIndex],
	//	  definition => [
	//	    named: BOOLEAN,
	//	    defCtx: CTXIndex],
	//	  union => [
	//	    hints: RECORD [
	//	      equalLengths: BOOLEAN,
	//	      refField, default, voidable: BOOLEAN],
	//	    overlaid, controlled, machineDep: BOOLEAN,
	//	    caseCtx: CTXIndex,
	//	    tagSei: ISEIndex],
	//	  sequence => [
	//	    packed: BOOLEAN,
	//	    controlled, machineDep: BOOLEAN,
	//	    tagSei: ISEIndex,
	//	    componentType: SEIndex],
	//	  relative => [
	//	    baseType: SEIndex,
	//	    offsetType: SEIndex,
	//	    resultType: SEIndex],
	//	  subrange => [
	//	    filled, empty: BOOLEAN,
	//	    rangeType: SEIndex,
	//	    origin: INTEGER,
	//	    range: CARDINAL],
	//	  long, real => [rangeType: SEIndex],
	//	  opaque => [
	//	    lengthKnown: BOOLEAN,
	//	    length: BitLength,
	//	    id: ISEIndex],
	//	  zone => [counted, mds: BOOLEAN],
	//	  any => [],
	//	  nil => [],
	//	  bits => [length: BitCount],	-- placed here to avoid
	//	  ENDCASE],			-- changing symbol version id's
	//    ENDCASE];
	//
	//SEIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO SERecord;
	//
	//ISEIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO id SERecord;
	//CSEIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO cons SERecord;
	//RecordSEIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO record cons SERecord;
	//RefSEIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO ref cons SERecord;
	//ArraySEIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO array cons SERecord;
	//
	//SENull: SEIndex = FIRST[SEIndex];
	//ISENull: ISEIndex = LOOPHOLE[SENull];
	//CSENull: CSEIndex = LOOPHOLE[SENull];
	//  RecordSENull: RecordSEIndex = LOOPHOLE[SENull];
	//  RefSENull: RefSEIndex = LOOPHOLE[SENull];
	//  ArraySENull: ArraySEIndex = LOOPHOLE[SENull];
	//
	//-- the following two values are guaranteed by the compiler
	//typeTYPE: CSEIndex = FIRST[CSEIndex] + SIZE[nil cons SERecord];
	//typeANY: CSEIndex = typeTYPE + SIZE[mode cons SERecord];
	//
	//-- codes identifying the basic types (extensible)
	//codeANY: CARDINAL = 0;
	//codeINT: CARDINAL = 1;
	//codeCHAR: CARDINAL = 2;
	//
	//WordCount: TYPE = LONG CARDINAL;	-- sizes in words
	//
	//BitLength: TYPE = CARDINAL;
	//BitCount: TYPE = LONG CARDINAL;		-- sizes in bits
	//FieldBitCount: TYPE = CARDINAL;
	//PackedBitCount: TYPE = [1..ByteLength];
	//
	//BitAddress: TYPE = RECORD[
	//  wd: [0..LAST[CARDINAL]/WordLength],	-- word displacement
	//  bd: [0..WordLength)];   			-- bit displacement
	//
	//ExtensionType: TYPE = {value, form, default, none};
	//
	//Linkage: TYPE = {val, ref, type, manifest, none};	-- for import/export
	//
	//RefClass: TYPE = {none, simple, composite};
	//
	//-- context table declarations
	//
	//ContextLevel: TYPE = [0..7];
	//  lZ: ContextLevel = 0;	-- context level of non-frame records
	//  lG: ContextLevel = 1;	-- context level of global frame
	//  lL: ContextLevel = lG+1;	-- context level of outer procedures
	//
	//Closure: TYPE = {none, unit, rc, full};  -- completeness of copied contexts
	//
	//CTXRecord: TYPE = RECORD [
	//  mark, varUpdated: BOOLEAN,
	//  seList: ISEIndex,
	//  level: ContextLevel,
	//  extension: SELECT ctxType: * FROM
	//    simple => [ctxNew: CTXIndex],	-- for DeSoto
	//    included => [
	//	chain: IncludedCTXIndex,
	//	copied: Closure � none,
	//	module: MDIndex,
	//	map: CTXIndex,
	//	closed, complete, restricted: BOOLEAN,
	//	reset: BOOLEAN],
	//    imported => [includeLink: IncludedCTXIndex],
	//    nil => []
	//    ENDCASE];
	//
	//CTXIndex: TYPE = Base RELATIVE ORDERED POINTER [0..3777B] TO CTXRecord;
	// IncludedCTXIndex: TYPE = Base RELATIVE ORDERED POINTER [0..3777B] TO included CTXRecord;
	//
	//CTXNull: CTXIndex = FIRST[CTXIndex];
	//  IncludedCTXNull: IncludedCTXIndex = LOOPHOLE[CTXNull];
	//
	//StandardContext: TYPE = CTXIndex[CTXNull+SIZE[simple CTXRecord]..CTXNull+6*SIZE[simple CTXRecord]];
	//
	//-- module table declarations
	//
	//FileIndex: TYPE = [0..77777B];	-- internal file handle
	//NullFileIndex: FileIndex = LAST[FileIndex];
	//
	//MDIndex: TYPE = Base RELATIVE ORDERED POINTER [0..Limit) TO MDRecord;
	//MDNull: MDIndex = LAST[MDIndex];
	//
	//OwnMdi: MDIndex = FIRST[MDIndex];
	//
	//-- body table declarations
	//
	//BodyLink: TYPE = RECORD [which: {sibling, parent}, index: BTIndex];
	//
	//CatchIndex: TYPE = NATURAL;
	//BodyRecord: TYPE = RECORD [
	//  link: BodyLink,
	//  firstSon: BTIndex,
	//  type: RecordSEIndex,
	//  localCtx: CTXIndex,
	//  level: ContextLevel,
	//  sourceIndex: CARDINAL,
	//  info: BodyInfo,
	//  extension: SELECT kind: * FROM
	//    Callable => [
	//	inline: BOOLEAN,
	//	id: ISEIndex,
	//	ioType: CSEIndex,
	//	monitored, noXfers, resident: BOOLEAN,
	//	entry, internal: BOOLEAN,
	//	entryIndex: [0..256),
	//	hints: RECORD [safe, argUpdated, nameSafe, needsFixup: BOOLEAN],
	//	closure: SELECT nesting: * FROM
	//	  Outer => [],
	//	  Inner => [frameOffset: [0..PrincOps.MaxFrameSize)],
	//	  Catch => [index: CatchIndex]
	//	  ENDCASE],
	//    Other => [relOffset: [0..LAST[CARDINAL]/2]]
	//    ENDCASE];
	//
	//  BodyInfo: TYPE = RECORD [
	//    SELECT mark: * FROM
	//	Internal => [
	//	  bodyTree: Base RELATIVE POINTER [0..Limit),
	//	    --Tree.Index--
	//	  thread: Base RELATIVE POINTER [0..Limit),
	//	    --Tree.Index / LitDefs.STIndex--
	//	  frameSize: [0..PrincOps.MaxFrameSize]],
	//	External => [
	//	  bytes: [0..LAST[CARDINAL]/2],
	//	  startIndex, indexLength: CARDINAL]
	//	ENDCASE];
	//
	//BTIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO BodyRecord;
	//  CBTIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO Callable BodyRecord;
	//    ICBTIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO Inner Callable BodyRecord;
	//    OCBTIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO Outer Callable BodyRecord;
	//    CCBTIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO Catch Callable BodyRecord;
	//BTNull: BTIndex = LAST[BTIndex];
	//  CBTNull: CBTIndex = LOOPHOLE[BTNull];
	//  CCBTNull: CCBTIndex = LOOPHOLE[BTNull];
	//
	//RootBti: CBTIndex = FIRST[CBTIndex];
	//
	//-- until idValue big enough to hold a BcdDefs.Link
	//
	//LinkTag: TYPE = {variable, procedure, type};
	//VarTag: TYPE = MACHINE DEPENDENT {var(0), proc0(1), type(2), proc1(3)};
	//
	//DummyLink: TYPE = RECORD [gfi, ep: [0..377B]];
	//
	//-- allocation codes for table components
	//
	//seType: Table.Selector = 1;
	//htType: Table.Selector = 2;
	//ssType: Table.Selector = 3;
	//ctxType: Table.Selector = 4;
	//mdType: Table.Selector = 5;
	//bodyType: Table.Selector = 6;
}


//HTIndex: TYPE = CARDINAL [0..Limit/2);
//HTNull: HTIndex = FIRST[HTIndex];
class HTIndex {
private:
	static const CARD16 HT_NULL = 0;

	Symbols*  symbols;
	CARD16    index;
	HTRecord* value;

public:
	static HTIndex* getNull();
	static HTIndex* getInstance(Symbols* symbols_, CARD16 index_);

	static void resolve();

	bool isNull() const {
		return index == HT_NULL;
	}
	QString toString() const;

	const HTRecord& getValue();
private:
	static QList<HTIndex*> all;

	HTIndex(Symbols* symbols_, CARD16 index_) : symbols(symbols_), index(index_), value(0) {
		all.append(this);
	}
};


//HTRecord: TYPE = RECORD [
//  anyInternal, anyPublic: BOOLEAN,
//  link: HTIndex,
//  ssIndex: CARDINAL];
class HTRecord {
private:
	const Symbols*  symbols;
	const CARD16    index;

public:
	static HTRecord* getInstance(Symbols* symbols, CARD16 index, CARD16 lastSSIndex);

	const bool    anyInternal;
	const bool    anyPublic;
	const CARD16  link;
	const CARD16  ssIndex;
	const QString value;

	QString toString() const;

private:
	HTRecord(Symbols* symbols_, CARD16 index_, bool anyInternal_, bool anyPublic_, CARD16 link_, CARD16 ssIndex_, QString value_) :
		symbols(symbols_), index(index_), anyInternal(anyInternal_), anyPublic(anyPublic_), link(link_), ssIndex(ssIndex_), value(value_) {}
};



//CTXIndex: TYPE = Base RELATIVE ORDERED POINTER [0..3777B] TO CTXRecord;
//CTXNull: CTXIndex = FIRST[CTXIndex];
class CTXIndex {
private:
	static const CARD16 CTX_NULL = 0;

	Symbols*   symbols;
	CARD16     index;
	CTXRecord* value;

public:
	static CTXIndex* getNull();
	static CTXIndex* getInstance(Symbols* symbols_, CARD16 index_);

	static void resolve();

	bool isNull() const {
		return index == CTX_NULL;
	}
	QString toString() const;

	const CTXRecord& getValue();
private:
	static QList<CTXIndex*> all;

	CTXIndex(Symbols* symbols_, CARD16 index_) : symbols(symbols_), index(index_), value(0) {
		all.append(this);
	}
};


//MDIndex: TYPE = Base RELATIVE ORDERED POINTER [0..Limit) TO MDRecord;
//MDNull: MDIndex = LAST[MDIndex];
//OwnMdi: MDIndex = FIRST[MDIndex];
class MDIndex {
private:
	static const CARD16 MD_NULL = symbols::LIMIT - 1;
	static const CARD16 MD_OWN  = 0;

	Symbols*  symbols;
	CARD16    index;
	MDRecord* value;

public:
	static MDIndex* getNull();
	static MDIndex* getInstance(Symbols* symbols_, CARD16 index_);

	static void resolve();

	bool isNull() const {
		return index == MD_NULL;
	}
	QString toString() const;

	const MDRecord& getValue();
private:
	static QList<MDIndex*> all;

	MDIndex(Symbols* symbols_, CARD16 index_) : symbols(symbols_), index(index_), value(0) {
		all.append(this);
	}
};


//MDRecord: TYPE = RECORD [
//  stamp: TimeStamp.Stamp,
//  moduleId: HTIndex,		-- hash entry for module name
//  fileId: HTIndex,		-- hash entry for file name
//  shared: BOOLEAN,		-- overrides PRIVATE, etc.
//  exported: BOOLEAN,
//  ctx: IncludedCTXIndex,	-- context of copied entries
//  defaultImport: CTXIndex,	-- unnamed imported instance
//  file: FileIndex];		-- associated file
//
class MDRecord {
private:
	const Symbols*  symbols;
	const CARD16    index;

public:
	static MDRecord* getInstance(Symbols* symbols, CARD16 index);

	const Stamp*     stamp;
	const HTIndex*   moduleId;
	const HTIndex*   fileId;
	const bool       shared;
	const bool       exported;
	const CTXIndex*  ctx;
	const CTXIndex*  defaultImport;
	const CARD16     file;

	QString toString() const;

private:
	MDRecord(Symbols* symbols_, CARD16 index_, Stamp* stamp_, HTIndex* moduleId_, HTIndex* fileId_, bool shared_, bool exported_, CTXIndex* ctx_, CTXIndex* defaultImport_, CARD16 file_) :
		symbols(symbols_), index(index_), stamp(stamp_), moduleId(moduleId_), fileId(fileId_), shared(shared_), exported(exported_), ctx(ctx_), defaultImport(defaultImport_), file(file_) {}
};


//
// Symbols
//
class Symbols {
public:
	// VersionID: CARDINAL = 08140; -- AMesa/14.0/Compiler/Friends/SymbolSegment.mesa
	static const CARD16 VersionID = 8140;

	//-- symbol segment headers
	//
	//WordOffset: TYPE = CARDINAL;
	//BlockDescriptor: TYPE = RECORD [offset: WordOffset, size: CARDINAL];
	class BlockDescriptor {
	public:
		static BlockDescriptor* getInstance(BCD* bcd);

	    const CARD16 offset;
	    const CARD16 size;

	    QString toString() const;
	private:
	    BlockDescriptor(CARD16 offset_, CARD16 size_) : offset(offset_), size(size_) {}
	};

   static bool isSymbolsFile(BCD* bcd);

	//STHeader: TYPE = RECORD [
	//  versionIdent: CARDINAL,
	//  version: TimeStamp.Stamp,
	//  creator: TimeStamp.Stamp,
	//  sourceVersion: TimeStamp.Stamp,
	//  definitionsFile: BOOLEAN,
	//  directoryCtx, importCtx, outerCtx: Symbols.CTXIndex,
	//  hvBlock: BlockDescriptor,
	//  htBlock: BlockDescriptor,
	//  ssBlock: BlockDescriptor,
	//  outerPackBlock: BlockDescriptor,
	//  innerPackBlock: BlockDescriptor,
	//  constBlock: BlockDescriptor,
	//  seBlock: BlockDescriptor,
	//  ctxBlock: BlockDescriptor,
	//  mdBlock: BlockDescriptor,
	//  bodyBlock: BlockDescriptor,
	//  extBlock: BlockDescriptor,
	//  treeBlock: BlockDescriptor,
	//  litBlock: BlockDescriptor,
	//  sLitBlock: BlockDescriptor,
	//  epMapBlock: BlockDescriptor,
	//  spareBlock: BlockDescriptor,
	//  fgRelPgBase: CARDINAL,
	//  fgPgCount: [0..256]];
	BCD*             bcd;
	BCDFile*         file;

	CARD32           symbolBase;
	CARD32           offsetBase;

	CARD16           versionIdent;
	Stamp*    version;
	Stamp*    creator;
	Stamp*    sourceVersion;
	bool             definitionsFile;
	CTXIndex*        directoryCtx;
	CTXIndex*        importCtx;
	CTXIndex*        outerCtx;
	BlockDescriptor* hvBlock;
	BlockDescriptor* htBlock;
	BlockDescriptor* ssBlock;
	BlockDescriptor* outerPackBlock;
	BlockDescriptor* innerPackBlock;
	BlockDescriptor* constBlock;
	BlockDescriptor* seBlock;
	BlockDescriptor* ctxBlock;
	BlockDescriptor* mdBlock;
	BlockDescriptor* bodyBlock;
	BlockDescriptor* extBlock;
	BlockDescriptor* treeBlock;
	BlockDescriptor* litBlock;
	BlockDescriptor* sLitBlock;
	BlockDescriptor* epMapBlock;
	BlockDescriptor* spareBlock;
	CARD16           fgRelPgBase;
	CARD16           fgPgCount;
	//
	QString                   ss;
	QMap<CARD16, CTXRecord*>  ctx;
	QMap<CARD16, HTRecord*>   ht;
	QMap<CARD16, MDRecord*>   md;
	QMap<CARD16, SERecord*>   se;
    QMap<CARD16, BTRecord*>   bt;
    QMap<CARD16, ExtRecord*>  ext;
    QMap<CARD16, LTRecord*>   lt;
    QMap<CARD16, TreeNode*>   tree;

    Symbols(BCD* bcd, int symbolBase);

private:
	void initializeSS(BlockDescriptor* block);
	void initializeHT(BlockDescriptor* block);
	void initializeMD(BlockDescriptor* block);
	void initializeCTX(BlockDescriptor* block);
	void initializeSE(BlockDescriptor* block);
	void initializeBT(BlockDescriptor* block);
	void initializeLT(BlockDescriptor* block);
	void initializeExt(BlockDescriptor* block);
	void initializeTree(BlockDescriptor* block);
};

#endif
