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

#include "HTRecord.h"
#include "MDRecord.h"

//-- symbol segment headers
//
//WordOffset: TYPE = CARDINAL;
//BlockDescriptor: TYPE = RECORD [offset: WordOffset, size: CARDINAL];
class BlockDescriptor {
public:
    CARD16 offset;
    CARD16 size;

    BlockDescriptor() : offset(0), size(0) {}
    BlockDescriptor(const BlockDescriptor& that) : offset(that.offset), size(that.size) {}
    BlockDescriptor(BCD& bcd);

    QString toString();
};


class Symbols {
public:
    static const CARD16 WORDS_PER_PAGE = 256;

    //tLimit: CARDINAL = 177777B;
    static const CARD16 T_LIMIT = 0177777;

    //VersionID: CARDINAL = 08140;
    static const CARD16 VersionID = 8140;

    //  altoBias: CARDINAL = 1;  -- AMesa/14.0/Compiler/Friends/FilePack.mesa
    static const CARD16 ALTO_BIAS = 1;

    // ByteLength: CARDINAL = 8;
    static const CARD16 BYTE_LENGTH = 8;

    // WordLength: CARDINAL = 16;
    static const CARD16 WORD_LENGTH = 16;

    //Base: TYPE = Table.Base;
    //Limit: CARDINAL = Table.Limit;
    static const CARD16 LIMIT = 0x4000;

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
    enum class TypeClass {
        MODE, BASIC, ENUMERATED, RECORD, REF,
        ARRAY, ARRAY_DESC, TRANSFER, DEFINITION, UNION,
        SEQUENCE, RELATIVE, SUB_RANGE, LONG, REAL,
        OPAQUE, ZONE, ANY, NIL, BITS,
        FIXED_SEQUENCE,
    };
    static QString toString(TypeClass value);

    //TransferMode: TYPE = {proc, port, signal, error, process, program, none};
    enum class TransferMode {
        PROC, PORT, SIGNAL, ERROR, PROCESS, PROGRAM, NONE,
    };
    static QString toString(TransferMode value);

    //  Unspec: TYPE [SIZE[CARDINAL]];

    //  SEIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO SERecord;
    //  ISEIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO id SERecord;
    //  CSEIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO cons SERecord;
    //  RecordSEIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO record cons SERecord;
    //  RefSEIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO ref cons SERecord;
    //  ArraySEIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO array cons SERecord;

    //  SENull: SEIndex = FIRST[SEIndex];
    //  ISENull: ISEIndex = LOOPHOLE[SENull];
    //  CSENull: CSEIndex = LOOPHOLE[SENull];
    //  RecordSENull: RecordSEIndex = LOOPHOLE[SENull];
    //  RefSENull: RefSEIndex = LOOPHOLE[SENull];
    //  ArraySENull: ArraySEIndex = LOOPHOLE[SENull];

    //-- codes identifying the basic types (extensible)
    //codeANY: CARDINAL = 0;
    //codeINT: CARDINAL = 1;
    //codeCHAR: CARDINAL = 2;

    //WordCount: TYPE = LONG CARDINAL;  -- sizes in words

    //BitLength: TYPE = CARDINAL;
    //BitCount: TYPE = LONG CARDINAL;       -- sizes in bits
    //FieldBitCount: TYPE = CARDINAL;
    //PackedBitCount: TYPE = [1..ByteLength];

    //BitAddress: TYPE = RECORD[
    //  wd(0:0..11): [0..LAST[CARDINAL]/WordLength], -- word displacement
    //  bd(0:12..15): [0..WordLength)];               -- bit displacement

    //ExtensionType: TYPE = {value, form, default, none};
    enum class ExtensionType {
        VALUE, FORM, DEFAULT, NONE,
    };
    static QString toString(ExtensionType value);

    //Linkage: TYPE = {val, ref, type, manifest, none};   -- for import/export
    enum class Linkage {
        VAL, REF, TYPE, MANIFEST, NONE,
    };
    static QString toString(Linkage value);

    //RefClass: TYPE = {none, simple, composite};
    enum class RefClass {
        NONE, SIMPLE, COMPOSITE,
    };
    static QString toString(RefClass value);

    //Closure: TYPE = {none, unit, rc, full};  -- completeness of copied contexts
    enum class Closure {
        NONE, UNIT, RC, FULL,
    };
    static QString toString(Closure value);

    //-- module table declarations
    //FileIndex: TYPE = [0..77777B];    -- internal file handle
    //NullFileIndex: FileIndex = LAST[FileIndex];
    static const CARD16 NULL_FILE_INDEX = 0x7fff;

    //BTIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO BodyRecord;
    //CBTIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO Callable BodyRecord;
    //ICBTIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO Inner Callable BodyRecord;
    //OCBTIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO Outer Callable BodyRecord;
    //CCBTIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO Catch Callable BodyRecord;
    //BTNull: BTIndex = LAST[BTIndex];
    static const CARD16 BT_NULL = LIMIT - 1;
    //CBTNull: CBTIndex = LOOPHOLE[BTNull];
    static const CARD16 CBT_NULL = BT_NULL;
    //CCBTNull: CCBTIndex = LOOPHOLE[BTNull];
    static const CARD16 CCBT_NULL = BT_NULL;
    //RootBti: CBTIndex = FIRST[CBTIndex];
    static const CARD16 ROOT_BIT = 0;

    //-- until idValue big enough to hold a BcdDefs.Link
    //LinkTag: TYPE = {variable, procedure, type};
    enum class LinkTag {
        VARIABLE, PROCEDURE, TYPE,
    };
    QString toString(LinkTag value);

    //VarTag: TYPE = MACHINE DEPENDENT {var(0), proc0(1), type(2), proc1(3)};
    enum class VarTag {
        VAR, PROC0, TYPE, PROC1,
    };
    QString toString(VarTag value);

    //DummyLink: TYPE = RECORD [gfi, ep: [0..377B]];
    //-- allocation codes for table components
    //seType: Table.Selector = 1;
    //htType: Table.Selector = 2;
    //ssType: Table.Selector = 3;
    //ctxType: Table.Selector = 4;
    //mdType: Table.Selector = 5;
    //bodyType: Table.Selector = 6;

    static bool isSymbolsFile(const BCD& bcd);


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
     BCD&            bcd;
     CARD32          symbolBase;
     CARD32          offsetBase;

     CARD16          versionIdent;
     VersionStamp    version;
     VersionStamp    creator;
     VersionStamp    sourceVersion;
     bool            definitionsFile;
     CARD16          directoryCtx; // CTXIndex
     CARD16          importCtx;    // CTXIndex
     CARD16          outerCtx;     // CTXIndex
     BlockDescriptor hvBlock;
     BlockDescriptor htBlock;
     BlockDescriptor ssBlock;
     BlockDescriptor outerPackBlock;
     BlockDescriptor innerPackBlock;
     BlockDescriptor constBlock;
     BlockDescriptor seBlock;
     BlockDescriptor ctxBlock;
     BlockDescriptor mdBlock;
     BlockDescriptor bodyBlock;
     BlockDescriptor extBlock;
     BlockDescriptor treeBlock;
     BlockDescriptor litBlock;
     BlockDescriptor sLitBlock;
     BlockDescriptor epMapBlock;
     BlockDescriptor spareBlock;
     CARD16          fgRelPgBase;
     CARD16          fgPgCount;
     //
     QString ss;
//     QMap<CARD16, CTXRecord>    ctx;
     QMap<CARD16, HTRecord>     ht;
     QMap<CARD16, MDRecord>     md;
//     QMap<CARD16, SERecord>     se;
//     QMap<CARD16, BodyRecord>   body;
//     QMap<CARD16, ExtRecord>    ext;
//     QMap<CARD16, LTRecord>     lt;
//     QMap<CARD16, Tree.Node>    tree;

     Symbols(BCD& bcd, int symbolBase);

private:
     void initializeSS();
     void initializeHT();
     void initializeMD();
};



#endif
