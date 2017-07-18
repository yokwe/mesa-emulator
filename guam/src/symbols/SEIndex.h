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
// SEIndex.h
//

#ifndef SEINDEX_H__
#define SEINDEX_H__

#include "../util/Util.h"
#include "../mesa/MesaBasic.h"

#include "BCD.h"
#include "Symbols.h"

//SEIndex: TYPE = Base RELATIVE ORDERED POINTER [0..3777B] TO SERecord;
//SENull: SEIndex = FIRST[SEIndex];
class SEIndex {
private:
	static constexpr const char* PREFIX = "se";
	static const CARD16  SE_NULL = 0;
	//-- the following two values are guaranteed by the compiler
	//typeTYPE: CSEIndex = FIRST[CSEIndex] + SIZE[nil cons SERecord];
	//typeANY: CSEIndex = typeTYPE + SIZE[mode cons SERecord];
	static const CARD16 TYPE_TYPE = 1;
	static const CARD16 TYPE_ANY  = 2;

	Symbols*  symbols;
	CARD16    index;

public:
	typedef Symbols::TransferMode TransferMode;
	static void checkAll();

	static SEIndex* getNull();
	static SEIndex* getInstance(Symbols* symbols, CARD16 index);

	bool isNull() const {
		return index == SE_NULL;
	}
	bool isTypeType() const {
		return index == TYPE_TYPE;
	}
	bool isTypeAny() const {
		return index == TYPE_ANY;
	}
	bool equals(const SEIndex* that) const {
		return this->index == that->index;
	}
	bool equals(const CARD16 that) const {
		return this->index == that;
	}
	Symbols* getSymbols() const {
		return symbols;
	}
	QString toString() const;
	const SERecord& getValue() const;

	SEIndex* find(CARD16 index) const;

	TransferMode xferMode() const;
	const SEIndex* underType() const;
	const SEIndex* nextSe() const;
	const SEIndex* typeLink() const;

	QList<BTRecord*> getBTRecordList() const {
		return symbols->bt.values();
	}

private:
	typedef Symbols::Key Key;
	static QMap<Key, SEIndex*> all;

	SEIndex(Symbols* symbols_, CARD16 index_) : symbols(symbols_), index(index_) {
		Key key(symbols, index);
		all[key] = this;
	}
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
private:
	const Symbols*  symbols;
	const CARD16    index;

public:

	class Id {
	public:
		class Linked {
		public:
			const SEIndex* linked;

			QString toString() const;

			Linked(SEIndex* linked_) : linked(linked_) {}
		};

		enum class Tag {TERMINAL, SEQUENTIAL, LINKED};
		static QString toString(Tag value);

		static Id* getInstance(Symbols* symbols, CARD16 u0);

		const bool      extended;
		const bool      public_;
		const CTXIndex* idCtx;
		const bool      immutable;
		const bool      constant;
		const SEIndex*  idType;
		const CARD16    idInfo;
		const CARD16    idValue;
		const HTIndex*  hash;
		const bool      linkSpace;

		const Tag   tag;
		const void* tagValue;

		const Linked& getLinked() const;

		QString toString() const;

		Id(bool extended_, bool public__, CTXIndex* idCtx_, bool immutable_, bool constant_,
			SEIndex* idType_, CARD16 idInfo_, CARD16 idValue_, HTIndex* hash_, bool linkSpace_, Tag tag_, void* tagValue_) :
			extended(extended_), public_(public__), idCtx(idCtx_), immutable(immutable_), constant(constant_),
			idType(idType_), idInfo(idInfo_), idValue(idValue_), hash(hash_), linkSpace(linkSpace_), tag(tag_), tagValue(tagValue_) {}
	};

	class Cons {
	public:
		class Basic {
		public:
			static Basic* getInstance(Symbols* symbols, CARD16 u0);

			const bool   ordered;
			const CARD16 code;
			const CARD16 length; // BitLength

			QString toString() const;

			Basic(bool ordered_, CARD16 code_, CARD16 length_) : ordered(ordered_), code(code_), length(length_) {}
		};

		class Enumerated {
		public:
			static Enumerated* getInstance(Symbols* symbols, CARD16 u0);

			const bool      ordered;
			const bool      machineDep;
			const bool      unpainted;
			const bool      sparse;
			const CTXIndex* valueCtx;
			const CARD16    nValues;

			QString toString() const;
			Enumerated(bool ordered_, bool machineDep_, bool unpainted_, bool sparse_, CTXIndex* valueCtx_, CARD16 nValues_) :
				ordered(ordered_), machineDep(machineDep_), unpainted(unpainted_), sparse(sparse_), valueCtx(valueCtx_), nValues(nValues_) {}
		};

		class Record {
		public:
			class Linked {
			public:
				const SEIndex* linkType;

				QString toString() const;

				Linked(SEIndex* linkType_) : linkType(linkType_) {}
			};
			//          hints(0:8..15): RECORD [
			//            comparable(0:0..0), assignable(0:1..1): BOOLEAN,
			//            unifield(0:2..2), variant(0:3..3), privateFields(0:4..4): BOOLEAN,
			//            refField(0:5..5), default(0:6..6), voidable(0:7..7): BOOLEAN],
			enum class Tag {NOT_LINKED, LINKED};
			static QString toString(Tag value);

			static Record* getInstance(Symbols* symbols, CARD16 u0);

			const bool      privateFields;
			const CARD16    length;
			const bool      argument;
			const bool      monitored;
			const bool      machineDep;
			const bool      painted;
			const CTXIndex* fieldCtx;

			const Tag       tag;
			const void*     tagValue;

			const Linked& getLinked() const;

			QString toString() const;

			Record(bool privateFields_, CARD16 length_, bool argument_, bool monitored_, bool machineDep_,
				bool painted_, CTXIndex* fieldCtx_, Tag tag_, void* tagValue_) :
				privateFields(privateFields_), length(length_), argument(argument_), monitored(monitored_), machineDep(machineDep_),
				painted(painted_), fieldCtx(fieldCtx_), tag(tag_), tagValue(tagValue_) {}
		};

		class Ref {
		public:
			static Ref* getInstance(Symbols* symbols, CARD16 u0);

			const bool     counted;
			const bool     ordered;
			const bool     readOnly;
			const bool     list;
			const bool     var;
			const bool     basing;
			const SEIndex* refType;

			QString toString() const;

			Ref(bool counted_, bool ordered_, bool readOnly_, bool list_, bool var_, bool basing, SEIndex* refType_) :
				counted(counted_), ordered(ordered_), readOnly(readOnly_), list(list_), var(var_), basing(basing), refType(refType_) {}
		};

		class Array {
		public:
			static Array* getInstance(Symbols* symbols, CARD16 u0);

			const bool     packed;
			const SEIndex* indexType;
			const SEIndex* componentType;

			QString toString() const;

			Array(bool packed_, SEIndex* indexType_, SEIndex* componentType_) : packed(packed_), indexType(indexType_), componentType(componentType_) {}
		};

		class ArrayDesc {
		public:
			static ArrayDesc* getInstance(Symbols* symbols, CARD16 u0);

			const bool     var;
			const bool     readOnly;
			const SEIndex* describedType;

			QString toString() const;

			ArrayDesc(bool var_, bool readOnly_, SEIndex* describedType_) : var(var_), readOnly(readOnly_), describedType(describedType_) {}
		};

		class Transfer {
		public:
			typedef Symbols::TransferMode TransferMode;
			static QString toString(TransferMode value) {
				return Symbols::toString(value);
			}
			static Transfer* getInstance(Symbols* symbols, CARD16 u0);

			const bool         safe;
			const TransferMode mode;
			const SEIndex*     typeIn;  // CSEIndex
			const SEIndex*     typeOut; // CSEIndex

			QString toString() const;

			Transfer(bool safe_, TransferMode mode_, SEIndex* typeIn_, SEIndex* typeOut_) : safe(safe_), mode(mode_), typeIn(typeIn_), typeOut(typeOut_) {}
		};

		class Definition {
		public:
			static Definition* getInstance(Symbols* symbols, CARD16 u0);

			const bool      named;
			const CTXIndex* defCtx;

			QString toString() const;

			Definition(bool named_, CTXIndex* defCtx_) : named(named_), defCtx(defCtx_) {}
		};

		class Union {
		public:
			static Union* getInstance(Symbols* symbols, CARD16 u0);

			//hints(0:8..11): RECORD [
			//  equalLengths(0:0..0): BOOLEAN,
			//  refField(0:1..1), default(0:2..2), voidable(0:3..3): BOOLEAN],
			const bool      overlaid;
			const bool      controlled;
			const bool      machineDep;
			const CTXIndex* caseCtx;
			const SEIndex*  tagSei; // ISEIndex

			QString toString() const;

			Union(bool overlaid_, bool controlled_, bool machineDep_, CTXIndex* caseCtx_, SEIndex* tagSei_) :
				overlaid(overlaid_), controlled(controlled_), machineDep(machineDep_), caseCtx(caseCtx_), tagSei(tagSei_) {}
		};

		class Sequence {
		public:
			static Sequence* getInstance(Symbols* symbols, CARD16 u0);

			const bool     packed;
			const bool     controlled;
			const bool     machineDep;
			const SEIndex* tagSei; // ISEIndex
			const SEIndex* componentType;

			QString toString() const;

			Sequence(bool packed_, bool controlled_, bool machineDep_, SEIndex* tagSei_, SEIndex* componentType_) :
				packed(packed_), controlled(controlled_), machineDep(machineDep_), tagSei(tagSei_), componentType(componentType_) {}
		};

		class Relative {
		public:
			static Relative* getInstance(Symbols* symbols, CARD16 u0);

			const SEIndex* baseType;
			const SEIndex* offsetType;
			const SEIndex* resultType;

			QString toString() const;

			Relative(SEIndex* baseType_, SEIndex* offsetType_, SEIndex* resultType_) :
				baseType(baseType_), offsetType(offsetType_), resultType(resultType_) {}
		};

		class Subrange {
		public:
			static Subrange* getInstance(Symbols* symbols, CARD16 u0);

			const bool     filled;
			const bool     empty;
			const SEIndex* rangeType;
			const INT16    origin;
			const CARD16   range;

			QString toString() const;

			Subrange(bool filled_, bool empty_, SEIndex* rangeType_, INT16 origin_, CARD16 range_) :
				filled(filled_), empty(empty_), rangeType(rangeType_), origin(origin_), range(range_) {}
		};

		class Long {
		public:
			static Long* getInstance(Symbols* symbols, CARD16 u0);

			const SEIndex* rangeType;

			QString toString() const;

			Long(SEIndex* rangeType_) : rangeType(rangeType_) {}
		};

		class Real {
		public:
			static Real* getInstance(Symbols* symbols, CARD16 u0);

			const SEIndex* rangeType;

			QString toString() const;

			Real(SEIndex* rangeType_) : rangeType(rangeType_) {}
		};

		class Opaque {
		public:
			static Opaque* getInstance(Symbols* symbols, CARD16 u0);

			const bool     lengthKnown;
			const CARD16   length; // BitLength
			const SEIndex* id;     // ISEIndex

			QString toString() const;

			Opaque(bool lengthKnown_, CARD16 length_, SEIndex* id_) : lengthKnown(lengthKnown_), length(length_), id(id_) {}
		};

		class Zone {
		public:
			static Zone* getInstance(Symbols* symbols, CARD16 u0);

			const bool counted;
			const bool mds;

			QString toString() const;

			Zone(bool counted_, bool mds_) : counted(counted_), mds(mds_) {}
		};

		class Bits {
		public:
			static Bits* getInstance(Symbols* symbols, CARD16 u0);

			const CARD32 length; // BitCount

			QString toString() const;

			Bits(CARD32 length_) : length(length_) {}
		};

		typedef Symbols::TypeClass Tag;
		static QString toString(Tag value) {
			return Symbols::toString(value);
		}

		static Cons* getInstance(Symbols* symbols, CARD16 u0);

		const Tag   tag;
		const void* tagValue;

		const Basic&      getBasic()      const;
		const Enumerated& getEnumerated() const;
		const Record&     getRecord()     const;
		const Ref&        getRef()        const;
		const Array&      getArray()      const;
		const ArrayDesc&  getArrayDesc()  const;
		const Transfer&   getTransfer()   const;
		const Definition& getDefinition() const;
		const Union&      getUnion()      const;
		const Sequence&   getSequence()   const;
		const Relative&   getRelative()   const;
		const Subrange&   getSubrange()   const;
		const Long&       getLong()       const;
		const Real&       getReal()       const;
		const Opaque&     getOpaque()     const;
		const Zone&       getZone()       const;
		const Bits&       getBits()       const;

		QString toString() const;

		Cons(Tag tag_, void* tagValue_) : tag(tag_), tagValue(tagValue_) {}
	};



	static SERecord* getInstance(Symbols* symbols, CARD16 index);
	static SERecord* find(Symbols* symbols, CARD16 index);

	enum class Tag {ID, CONS};
	static QString toString(Tag value);

	const Tag   tag;
	const void* tagValue;

	const Id&   getId() const;
	const Cons& getCons() const;

	QString toString() const;

private:
	typedef Symbols::Key Key;
	static QMap<Key, SERecord*> all;

	SERecord(Symbols* symbols_, CARD16 index_, Tag tag_, void* tagValue_) : symbols(symbols_), index(index_), tag(tag_), tagValue(tagValue_) {
		Key key(symbols, index);
		all[key] = this;
		// register SEIndex::all
		SEIndex::getInstance(symbols_, index_);
	}
};

#endif
