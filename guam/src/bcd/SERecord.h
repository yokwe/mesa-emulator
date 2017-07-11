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
// SERecord.h
//

#ifndef SERECORD_H__
#define SERECORD_H__

#include "../util/Util.h"
#include "../mesa/MesaBasic.h"

#include "Symbols.h"

class Symbols;

class CTXIndex;
class HTIndex;
class MDIndex;
class SERecord;

//SEIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO SERecord;
//SENull: SEIndex = FIRST[SEIndex];

class SEIndex {
public:
    //SENull: SEIndex = FIRST[SEIndex];
    static const CARD16 SE_NULL = 0;
    //-- the following two values are guaranteed by the compiler
    //typeTYPE: CSEIndex = FIRST[CSEIndex] + SIZE[nil cons SERecord];
    static const CARD16 TYPE_TYPE = 1;
    //typeANY: CSEIndex = typeTYPE + SIZE[mode cons SERecord];
    static const CARD16 TYPE_ANY = 2;


	static void resolve();

	Symbols*   symbols;
	CARD16     index;
	SERecord* value;

	SEIndex(Symbols* symbols, CARD16 index);

	QString toString();

	bool isNull() {
		return index == SE_NULL;
	}
	bool isTypeType() {
		return index == TYPE_TYPE;
	}
	bool isTypeAny() {
		return index == TYPE_ANY;
	}
	bool equals(SEIndex* that) {
		return this->index == that->index;
	}

	SEIndex* underType();

	static SEIndex* find(Symbols* symbols, CARD16 index);

private:
	static QList<SEIndex*> all;
};


//SERecord: TYPE = RECORD [
//  mark3(0:0..0), mark4(0:1..1): BOOLEAN,
//  body(0:2..95): SELECT seTag(0:2..2): * FROM
//    id => [
//      extended(0:3..3): BOOLEAN,
//      public(0:4..4): BOOLEAN,
//      idCtx(0:5..15): CTXIndex,
//      immutable(1:0..0), constant(1:1..1): BOOLEAN,
//      idType(1:2..15): SEIndex,
//      idInfo(2:0..15): Unspec,
//      idValue(3:0..15): Unspec,
//      hash(4:0..12): HTIndex,
//      linkSpace(4:13..13): BOOLEAN,
//      ctxLink(4:14..31): SELECT linkTag(4:14..15): * FROM
//        terminal => [],
//        sequential => [],
//        linked => [link(5:0..15): ISEIndex]
//        ENDCASE],
//    cons => [
//      typeInfo(0:3..63): SELECT typeTag(0:3..7): TypeClass FROM
//        mode => [],
//        basic => [
//          ordered(0:8..8): BOOLEAN,
//          code(0:9..15): [0..16),
//          length(1:0..15): BitLength],
//        enumerated => [
//          ordered(0:8..8), machineDep(0:9..9): BOOLEAN,
//          unpainted(0:10..10): BOOLEAN,     -- un- for backward compatiblity
//          sparse(0:11..15): BOOLEAN,
//          valueCtx(1:0..15): CTXIndex,
//          nValues(2:0..15): CARDINAL],
//        record => [
//          hints(0:8..15): RECORD [
//            comparable(0:0..0), assignable(0:1..1): BOOLEAN,
//            unifield(0:2..2), variant(0:3..3), privateFields(0:4..4): BOOLEAN,
//            refField(0:5..5), default(0:6..6), voidable(0:7..7): BOOLEAN],
//          length(1:0..15): BitLength,
//          argument(2:0..0), monitored(2:1..1), machineDep(2:2..2): BOOLEAN,
//          painted(2:3..3): BOOLEAN,
//          fieldCtx(2:4..14): CTXIndex,
//          linkPart(2:15..31): SELECT linkTag(2:15..15): * FROM
//            notLinked => [],
//            linked => [linkType(3:0..15): SEIndex]
//            ENDCASE],
//        ref => [
//          counted(0:8..8), ordered(0:9..9), readOnly(0:10..10), list(0:11..11), var(0:12..12), basing(0:13..15): BOOLEAN,
//          refType(1:0..15): SEIndex],
//        array => [
//          packed(0:8..15): BOOLEAN,
//          indexType(1:0..15): SEIndex,
//          componentType(2:0..15): SEIndex],
//        arraydesc => [
//          var(0:8..8), readOnly(0::9..15): BOOLEAN,
//          describedType(1:0..15): SEIndex],
//        transfer => [
//          safe(0:8..8): BOOLEAN,
//          mode(0:9..15): TransferMode,
//          typeIn(1:0..15), typeOut(2:0..15): CSEIndex],
//        definition => [
//          named(0:8..15): BOOLEAN,
//          defCtx(1:0..15): CTXIndex],
//        union => [
//          hints(0:8..11): RECORD [
//            equalLengths(0:0..0): BOOLEAN,
//            refField(0:1..1), default(0:2..2), voidable(0:3..3): BOOLEAN],
//          overlaid(0:12..12), controlled(0:13..13), machineDep(0:14..15): BOOLEAN,
//          caseCtx(1:0..15): CTXIndex,
//          tagSei(2:0..15): ISEIndex],
//        sequence => [
//          packed(0:8..8): BOOLEAN,
//          controlled(0:9..9), machineDep(0:10..15): BOOLEAN,
//          tagSei(1:0..15): ISEIndex,
//          componentType(2:0..15): SEIndex],
//        relative => [
//          baseType(1:0..15): SEIndex,
//          offsetType(2:0..15): SEIndex,
//          resultType(3:0..15): SEIndex],
//        subrange => [
//          filled(0:8..8), empty(0:9..15): BOOLEAN,
//          rangeType(1:0..15): SEIndex,
//          origin(2:0..15): INTEGER,
//          range(3:0..15): CARDINAL],
//        long, real => [rangeType(1:0..15): SEIndex],
//        opaque => [
//          lengthKnown(0:8..15): BOOLEAN,
//          length(1:0..15): BitLength,
//          id(2:0..15): ISEIndex],
//        zone => [counted(0:8..8), mds(0:9..15): BOOLEAN],
//        any => [],
//        nil => [],
//        bits => [length(1:0..31): BitCount],   -- placed here to avoid
//        ENDCASE],         -- changing symbol version id's
//    ENDCASE];
class SERecord {
public:
	enum class SeTag {
		ID, CONS,
	};
	static QString toString(SeTag value);

	enum class LinkTag {
		TERMINAL, SEQUENTIAL, LINKED,
	};
	static QString toString(LinkTag value);

	enum class LinkTag2 {
		NOT_LINKED, LINKED,
	};
	static QString toString(LinkTag2 value);

    //TransferMode: TYPE = {proc, port, signal, error, process, program, none};
    enum class TransferMode {
        PROC, PORT, SIGNAL, ERROR, PROCESS, PROGRAM, NONE,
    };
    static QString toString(TransferMode value);


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
        SEQUENCE, RELATIVE, SUBRANGE, LONG, REAL,
        OPAQUE, ZONE, ANY, NIL, BITS,
        FIXED_SEQUENCE,
    };
    static QString toString(TypeClass value);

	CARD16 index;
	SeTag  seTag;
	union {
		struct {
			bool      extended;
			bool      public_;
			CTXIndex* idCtx;
			bool      immutable;
			bool      constant;
			SEIndex*  idType;
			CARD16    idInfo;
			CARD16    idValue;
			HTIndex*  hash;
			bool      linkSpace;
			LinkTag   linkTag;
			union {
				struct {
					SEIndex* link;
				} linked;
			};
		} id;
		struct {
			TypeClass typeTag;
			union {
				struct {
					bool   ordered;
					CARD16 code;
					CARD16 length;
				} basic;
				struct {
					bool      ordred;
					bool      machineDep;
					bool      unpainted;
					bool      sparse;
					CTXIndex* valueCtx;
					CARD16    nValues;
				} enumerated;
				struct {
					bool      comparable;
					bool      assignable;
					bool      unifield;
					bool      variant;
					bool      privateFields;
					bool      refField;
					bool      default_;
					bool      voidable;
					CARD16    length;
					bool      argument;
					bool      monitored;
					bool      machineDep;
					bool      painted;
					CTXIndex* fieldCtx;
					LinkTag2  linkTag;
					union {
						struct {
							SEIndex* linkType;
						} linked;
					};
				} record;
				struct {
					bool     counted;
					bool     ordered;
					bool     readOnly;
					bool     list;
					bool     var;
					bool     basing;
					SEIndex* refType;
				} ref;
				struct {
					bool     packed;
					SEIndex* indexType;
					SEIndex* componentType;
				} array;
				struct {
					bool     var;
					bool     readOnly;
					SEIndex* describedType;
				} arraydesc;
				struct {
					bool         safe;
					TransferMode mode;
					SEIndex*     typeIn;
					SEIndex*     typeOut;
				} transfer;
				struct {
					bool      named;
					CTXIndex* defCtx;
				} definition;
				struct {
					bool      equalLengths;
					bool      refField;
					bool      default_;
					bool      voidable;
					bool      overlaid;
					bool      controlled;
					bool      machineDep;
					CTXIndex* caseCtx;
					SEIndex*  tagSei;
				} union_;
				struct {
					bool     packed;
					bool     controlled;
					bool     machineDep;
					SEIndex* tagSei;
					SEIndex* componentType;
				} sequence;
				struct {
					SEIndex* baseType;
					SEIndex* offsetType;
					SEIndex* resultType;
				} relative;
				struct {
					bool     filled;
					bool     empty;
					SEIndex* rangeType;
					INT16    origin;
					CARD16   range;
				} subrange;
				struct {
					SEIndex* rangeType;
				} long_;
				struct {
					SEIndex* rangeType;
				} real_;
				struct {
					bool lengthKnown;
					CARD16 length;
					SEIndex* id;
				} opaque;
				struct {
					bool counted;
					bool mds;
				} zone;
				struct {
					CARD32 length;
				} bits;
			};
		} cons;
	};

	SERecord(Symbols* symbols, CARD16 index);

	QString toString();

private:
	void initID(Symbols* symbols, CARD16 u0);
	void initCONS(Symbols* symbols, CARD16 u0);

    void initMODE(Symbols* symbols, CARD16 u0);
	void initBASIC(Symbols* symbols, CARD16 u0);
	void initENUMERATED(Symbols* symbols, CARD16 u0);
	void initRECORD(Symbols* symbols, CARD16 u0);
	void initREF(Symbols* symbols, CARD16 u0);
    void initARRAY(Symbols* symbols, CARD16 u0);
	void initARRAY_DESC(Symbols* symbols, CARD16 u0);
	void initTRANSFER(Symbols* symbols, CARD16 u0);
	void initDEFINITION(Symbols* symbols, CARD16 u0);
	void initUNION(Symbols* symbols, CARD16 u0);
    void initSEQUENCE(Symbols* symbols, CARD16 u0);
	void initRELATIVE(Symbols* symbols, CARD16 u0);
	void initSUB_RANGE(Symbols* symbols, CARD16 u0);
	void initLONG(Symbols* symbols, CARD16 u0);
	void initREAL(Symbols* symbols, CARD16 u0);
    void initOPAQUE(Symbols* symbols, CARD16 u0);
	void initZONE(Symbols* symbols, CARD16 u0);
	void initANY(Symbols* symbols, CARD16 u0);
	void initNIL(Symbols* symbols, CARD16 u0);
	void initBITS(Symbols* symbols, CARD16 u0);
    void initFIXED_SEQUENCE(Symbols* symbols, CARD16 u0);
};


#endif
