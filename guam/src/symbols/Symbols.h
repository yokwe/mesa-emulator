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
	struct Key {
		const Symbols* symbols;
		const CARD16   index;

		bool operator<(const Key &that) const {
				if (symbols != that.symbols) return symbols < that.symbols;
				return index < that.index;
		}

		Key(const Key& that) : symbols(that.symbols), index(that.index) {}
		Key(const Symbols* symbols_, const CARD16 index_) : symbols(symbols_), index(index_) {}
	};

	//ByteLength: CARDINAL = 8;
	static const CARD16 BYTE_LENGTH = 8;
	//WordLength: CARDINAL = 16;
	static const CARD16 WORD_LENGTH = 16;

	//Base: TYPE = Table.Base;
	//Limit: CARDINAL = Table.Limit;
	static const CARD16 LIMIT = 040000;

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
	enum class TypeClass {
        MODE, BASIC, ENUMERATED, RECORD, REF,
        ARRAY, ARRAYDESC, TRANSFER, DEFINITION, UNION,
        SEQUENCE, RELATIVE, SUBRANGE, LONG, REAL,
        OPAQUE, ZONE, ANY, NIL, BITS,
        FIXED_SEQUENCE
	};
	QString toString(TypeClass value);

	//TransferMode: TYPE = {proc, port, signal, error, process, program, none};
	enum class TransferMode {
		PROC, PORT, SIGNAL, ERROR, PROCESS, PROGRAM, NONE
	};
	QString toString(TransferMode value);

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
	enum class ExtensionType {VALUE, FORM, DEFAULT, NONE};
	QString toString(ExtensionType value);

	//Linkage: TYPE = {val, ref, type, manifest, none};	-- for import/export
	enum class Linkage {VAL, REF, TYPE, MANIFEST, NONE};
	QString toString(Linkage value);

	//
	//RefClass: TYPE = {none, simple, composite};
	enum class RefClass {NONE, SIMPLE, COMPOSITE};
	QString toString(RefClass value);


	//ContextLevel: TYPE = [0..7];
	//  lZ: ContextLevel = 0;	-- context level of non-frame records
	//  lG: ContextLevel = 1;	-- context level of global frame
	//  lL: ContextLevel = lG+1;	-- context level of outer procedures
	static const CARD16 LZ = 0;
	static const CARD16 LG = 1;
	static const CARD16 LL = LG + 1;
	//
	//Closure: TYPE = {none, unit, rc, full};  -- completeness of copied contexts
	enum class Closure {NONE, UNIT, RC, FULL};
	QString toString(Closure value);

	//LinkTag: TYPE = {variable, procedure, type};
	enum class LinkTag {VARIABLE, PROCEDURE, TYPE};
	QString toString(LinkTag value);

	//VarTag: TYPE = MACHINE DEPENDENT {var(0), proc0(1), type(2), proc1(3)};
	enum class VarTag {VAR, PROC0, TYPE, PROC1};
	QString toString(VarTag value);

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
	static constexpr const char* PREFIX = "ht";
	static const CARD16  HT_NULL = 0;

	Symbols*  symbols;
	CARD16    index;

public:
	static HTIndex* getNull();
	static HTIndex* getInstance(Symbols* symbols_, CARD16 index_);

	bool isNull() const {
		return index == HT_NULL;
	}
	QString toString() const;

	const HTRecord& getValue() const;
private:
	HTIndex(Symbols* symbols_, CARD16 index_) : symbols(symbols_), index(index_) {}
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
	static HTRecord* find(Symbols* symbols, CARD16 index);

	const bool    anyInternal;
	const bool    anyPublic;
	const CARD16  link;
	const CARD16  ssIndex;
	const QString value;

	QString toString() const;

private:
	typedef symbols::Key Key;
	static QMap<Key, HTRecord*> all;

	HTRecord(Symbols* symbols_, CARD16 index_, bool anyInternal_, bool anyPublic_, CARD16 link_, CARD16 ssIndex_, QString value_) :
		symbols(symbols_), index(index_), anyInternal(anyInternal_), anyPublic(anyPublic_), link(link_), ssIndex(ssIndex_), value(value_) {
		Key key(symbols, index);
		all[key] = this;
	}
};



//CTXIndex: TYPE = Base RELATIVE ORDERED POINTER [0..3777B] TO CTXRecord;
//CTXNull: CTXIndex = FIRST[CTXIndex];
class CTXIndex {
private:
	static constexpr const char* PREFIX = "ctx";
	static const CARD16  CTX_NULL = 0;

	Symbols*   symbols;
	CARD16     index;

public:
	static CTXIndex* getNull();
	static CTXIndex* getInstance(Symbols* symbols_, CARD16 index_);

	bool isNull() const {
		return index == CTX_NULL;
	}
	QString toString() const;
	const CTXRecord& getValue() const;

private:
	CTXIndex(Symbols* symbols_, CARD16 index_) : symbols(symbols_), index(index_) {}
};

class CTXRecord {
private:
	const Symbols*  symbols;
	const CARD16    index;

public:
	typedef symbols::Closure Closure;

	enum class Tag {SIMPLE, INCLUDED, IMPORTED, NIL};
	static QString toString(Tag value);

	class Simple {
	public:
		const CTXIndex* ctxNew;

		QString toString() const;
		Simple(CTXIndex* ctxNew_) : ctxNew(ctxNew_) {}
	};
	class Included {
	public:
		const CTXIndex* chain; // IncludedCTXIndex
		const Closure   copied;
		const MDIndex*  module;
		const CTXIndex* map;
		const bool      closed;
		const bool      complete;
		const bool      restricted;
		const bool      reset;

		QString toString() const;
		Included(CTXIndex* chain_, Closure copied_, MDIndex* module_, CTXIndex* map_, bool closed_, bool complete_, bool restricted_, bool reset_) :
			chain(chain_), copied(copied_), module(module_), map(map_), closed(closed_), complete(complete_), restricted(restricted_), reset(reset_) {}
	};
	class Imported {
	public:
		const CTXIndex* includeLink; // IncludedCTXIndex

		QString toString() const;
		Imported(CTXIndex* includeLink_) : includeLink(includeLink_) {}
	};

	static CTXRecord* getInstance(Symbols* symbols, CARD16 index);
	static CTXRecord* find(Symbols* symbols, CARD16 index);

//	bool   mark;
//	bool   varUpdated;
	const SEIndex* seList;
	const CARD16   level;
	const Tag      tag;
	const void*    tagValue;

	const Simple&   getSimple() const;
	const Included& getIncluded() const;
	const Imported& getImported() const;

	QString toString() const;

private:
	typedef symbols::Key Key;
	static QMap<Key, CTXRecord*> all;

	CTXRecord(Symbols* symbols_, CARD16 index_, SEIndex* seList_, CARD16 level_, Tag tag_, void* tagValue_) :
		symbols(symbols_), index(index_), seList(seList_), level(level_), tag(tag_), tagValue(tagValue_) {
		Key key(symbols, index);
		all[key] = this;
	}
};

//MDIndex: TYPE = Base RELATIVE ORDERED POINTER [0..Limit) TO MDRecord;
//MDNull: MDIndex = LAST[MDIndex];
//OwnMdi: MDIndex = FIRST[MDIndex];
class MDIndex {
private:
	static constexpr const char* PREFIX = "md";
	static const CARD16  MD_NULL = symbols::LIMIT - 1;
	static const CARD16  MD_OWN  = 0;

	Symbols*  symbols;
	CARD16    index;

public:
	static MDIndex* getNull();
	static MDIndex* getInstance(Symbols* symbols_, CARD16 index_);

	bool isNull() const {
		return index == MD_NULL;
	}
	QString toString() const;
	const MDRecord& getValue() const;

private:
	MDIndex(Symbols* symbols_, CARD16 index_) : symbols(symbols_), index(index_) {}
};


class MDRecord {
private:
	const Symbols*  symbols;
	const CARD16    index;

public:
	static MDRecord* getInstance(Symbols* symbols, CARD16 index);
	static MDRecord* find(Symbols* symbols, CARD16 index);

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
	typedef symbols::Key Key;
	static QMap<Key, MDRecord*> all;

	MDRecord(Symbols* symbols_, CARD16 index_, Stamp* stamp_, HTIndex* moduleId_, HTIndex* fileId_,
			bool shared_, bool exported_, CTXIndex* ctx_, CTXIndex* defaultImport_, CARD16 file_) :
		symbols(symbols_), index(index_), stamp(stamp_), moduleId(moduleId_), fileId(fileId_),
		shared(shared_), exported(exported_), ctx(ctx_), defaultImport(defaultImport_), file(file_) {
		Key key(symbols, index);
		all[key] = this;
	}
};


//SEIndex: TYPE = Base RELATIVE ORDERED POINTER [0..Limit) TO SERecord;
//MDNull: MDIndex = LAST[MDIndex];
//OwnMdi: MDIndex = FIRST[MDIndex];
class SEIndex {
private:
	static constexpr const char* PREFIX = "se";
	static const CARD16  SE_NULL = 0;

	Symbols*  symbols;
	CARD16    index;

public:
	static SEIndex* getNull();
	static SEIndex* getInstance(Symbols* symbols_, CARD16 index_);

	bool isNull() const {
		return index == SE_NULL;
	}
	QString toString() const;
//	const SERecord& getValue() const;

private:
	SEIndex(Symbols* symbols_, CARD16 index_) : symbols(symbols_), index(index_) {}
};


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
class BTIndex {
private:
	static constexpr const char* PREFIX = "bt";
	static const CARD16  BT_NULL = symbols::LIMIT - 1;
	static const CARD16  BT_ROOT = 0;

	Symbols*  symbols;
	CARD16    index;

public:
	static BTIndex* getNull();
	static BTIndex* getInstance(Symbols* symbols_, CARD16 index_);

	bool isNull() const {
		return index == BT_NULL;
	}
	QString toString() const;
//	const BTRecord& getValue() const;

private:
	BTIndex(Symbols* symbols_, CARD16 index_) : symbols(symbols_), index(index_) {}
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
