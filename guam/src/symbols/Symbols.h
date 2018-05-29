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
#define SYMBOLS_H__

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

class TreeLink;
class TreeNode;


//
// Symbols
//
class Symbols {
public:
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

	//  altoBias: CARDINAL = 1;  -- AMesa/14.0/Compiler/Friends/FilePack.mesa
	static const CARD16 ALTO_BIAS = 1;

	static const CARD16 WORDS_PER_PAGE = 256;

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
	static QString toString(TypeClass value);

	//TransferMode: TYPE = {proc, port, signal, error, process, program, none};
	enum class TransferMode {
		PROC, PORT, SIGNAL, ERROR, PROCESS, PROGRAM, NONE
	};
	static QString toString(TransferMode value);

	//-- the following two values are guaranteed by the compiler
	//typeTYPE: CSEIndex = FIRST[CSEIndex] + SIZE[nil cons SERecord];
	//typeANY: CSEIndex = typeTYPE + SIZE[mode cons SERecord];
	static const CARD16 TYPE_TYPE = 1;
	static const CARD16 TYPE_ANY  = 2;

	//-- codes identifying the basic types (extensible)
	//codeANY: CARDINAL = 0;
	//codeINT: CARDINAL = 1;
	//codeCHAR: CARDINAL = 2;
	static const CARD16 CODE_ANY  = 0;
	static const CARD16 CODE_INT  = 1;
	static const CARD16 CODE_CHAR = 2;

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
	static QString toString(ExtensionType value);

	//Linkage: TYPE = {val, ref, type, manifest, none};	-- for import/export
	enum class Linkage {VAL, REF, TYPE, MANIFEST, NONE};
	static QString toString(Linkage value);

	//
	//RefClass: TYPE = {none, simple, composite};
	enum class RefClass {NONE, SIMPLE, COMPOSITE};
	static QString toString(RefClass value);


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
	static QString toString(Closure value);

	//LinkTag: TYPE = {variable, procedure, type};
	enum class LinkTag {VARIABLE, PROCEDURE, TYPE};
	static QString toString(LinkTag value);

	//VarTag: TYPE = MACHINE DEPENDENT {var(0), proc0(1), type(2), proc1(3)};
	enum class VarTag {VAR, PROC0, TYPE, PROC1};
	static QString toString(VarTag value);




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
   static bool isSymbolsSegment(BCD* bcd, int symbolBase);

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
