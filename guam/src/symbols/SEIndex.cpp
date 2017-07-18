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
// SEIndex.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("se");

#include "SEIndex.h"
#include "BCDFile.h"

#include "CTXIndex.h"
#include "HTIndex.h"

//
// SEIndex
//
QMap<SEIndex::Key, SEIndex*> SEIndex::all;
SEIndex::SEIndex(Symbols* symbols_, CARD16 index_) : symbols(symbols_), index(index_) {
	Key key(symbols, index_);
	all[key] = this;
}
void SEIndex::checkAll() {
	for(SEIndex* e: all.values()) {
		if (e->isNull()) continue;
		SERecord* value = SERecord::find(e->symbols, e->index);
		if (value == 0) {
			logger.warn("Bogus %s", e->toString().toLocal8Bit().constData());
		}
	}
}
SEIndex* SEIndex::getNull() {
	return SEIndex::getInstance(0, SEIndex::SE_NULL);
}
SEIndex* SEIndex::getInstance(Symbols* symbols, CARD16 index) {
	Key key(symbols, index);
	if (all.contains(key)) return all[key];

	return new SEIndex(symbols, index);
}
QString SEIndex::toString() const {
	if (isNull()) return QString("%1-NULL").arg(PREFIX);
	return QString("%1-%2").arg(PREFIX).arg(index);
}
const SERecord& SEIndex::getValue() const {
	SERecord* ret = SERecord::find(symbols, index);
	if (ret == 0) {
		logger.fatal("Cannot find  symbols = %p  index = %d", symbols, index);
		ERROR();
	}
	return *ret;
}

SEIndex* SEIndex::find(CARD16 index_) const {
	Key key(symbols, index_);
	if (all.contains(key)) return all[key];

	logger.fatal("Cannot find  symbols = %p  index = %d", symbols, index_);
	ERROR();
}

//XferMode: PROC [h: Handle, type: SEIndex] RETURNS [TransferMode] = {
//  sei: CSEIndex = UnderType[h, type];
//  RETURN [WITH t: h.seb[sei] SELECT FROM transfer => t.mode, ENDCASE => none]};
SEIndex::TransferMode SEIndex::xferMode() const {
	const SEIndex* sei = underType();
	sei->getValue().getCons().tag;
	return sei->getValue().getCons().tag == SERecord::Cons::Tag::TRANSFER ? sei->getValue().getCons().getTransfer().mode : SEIndex::TransferMode::NONE;
}

//UnderType: PROC [h: Handle, type: SEIndex] RETURNS [CSEIndex] = {
//  sei: SEIndex � type;
//  WHILE sei # SENull DO
//    WITH se: h.seb[sei] SELECT FROM
//      id => {IF se.idType # typeTYPE THEN ERROR; sei � SymbolOps.ToSei[se.idInfo]};
//      ENDCASE => EXIT;
//    ENDLOOP;
//  RETURN [LOOPHOLE[sei, CSEIndex]]};
const SEIndex* SEIndex::underType() const {
	const SEIndex* sei = this;
	while(!sei->isNull()) {
		if (sei->getValue().tag == SERecord::Tag::ID) {
			const SERecord::Id& se(sei->getValue().getId());
			if (!se.idType->isTypeType()) ERROR();
			sei = sei->find(se.idInfo);
		} else {
			break;
		}
	}
	return sei;
}

//NextSe: PROC [h: Handle, sei: ISEIndex] RETURNS [ISEIndex] = {
//  RETURN [
//    IF sei = SENull
//	    THEN ISENull
//	    ELSE
//	      WITH id: h.seb[sei] SELECT FROM
//	        terminal => ISENull,
//	        sequential => sei + SIZE[sequential id SERecord],
//	       linked => id.link,
//	       ENDCASE => ISENull]};
const SEIndex* SEIndex::nextSe() const {
	if (isNull()) {
		return SEIndex::getNull();
	} else {
		const SERecord::Id& id(getValue().getId());
		switch(getValue().getId().tag) {
		case SERecord::Id::Tag::TERMINAL:
			return SEIndex::getNull();
		case SERecord::Id::Tag::SEQUENTIAL:
			return SEIndex::find(index + 5); // Is this correct?
		case SERecord::Id::Tag::LINKED:
			return id.getLinked().linked;
		default:
			return SEIndex::getNull();
		}
	}
}


//  TypeLink: PROC [h: Handle, type: SEIndex] RETURNS [SEIndex] = {
//	sei: CSEIndex = UnderType[h, type];
//	RETURN [WITH se: h.seb[sei] SELECT FROM
//	  record => WITH se SELECT FROM linked => linkType, ENDCASE => SENull,
//	  ENDCASE => SENull]};
const SEIndex* SEIndex::typeLink() const {
	const SEIndex* sei = underType();
	switch(sei->getValue().getCons().tag) {
	case SERecord::Cons::Tag::RECORD:
		switch (sei->getValue().getCons().getRecord().tag) {
		case SERecord::Cons::Record::Tag::LINKED:
			return sei->getValue().getCons().getRecord().getLinked().linkType;
		default:
			return SEIndex::getNull();
		}
		break;
	default:
		return SEIndex::getNull();
	}
}



//
// SERecord
//
QMap<SERecord::Key, SERecord*> SERecord::all;
SERecord::SERecord(Symbols* symbols_, CARD16 index_, Tag tag_, void* tagValue_) : symbols(symbols_), index(index_), tag(tag_), tagValue(tagValue_) {
	Key key(symbols_, index_);
	all[key] = this;
	// register SEIndex::all
	SEIndex::getInstance(symbols_, index_);
}
SERecord* SERecord::find(Symbols* symbols, CARD16 index) {
	Key key(symbols, index);
	return all.value(key, 0);
}


QString SERecord::toString() const {
	switch(tag) {
	case Tag::ID:
		return QString("%1 %2").arg(toString(tag)).arg(getId().toString());
	case Tag::CONS:
		return QString("%1 %2").arg(toString(tag)).arg(getCons().toString());
	default:
		ERROR();
		return "???";
	}
}

QString SERecord::toString(Tag value) {
	TO_STRING_PROLOGUE(Tag)

	MAP_ENTRY(ID)
	MAP_ENTRY(CONS)

	TO_STRING_EPILOGUE
}
QString SERecord::Id::toString(Tag value) {
	TO_STRING_PROLOGUE(Tag)

	MAP_ENTRY(TERMINAL)
	MAP_ENTRY(SEQUENTIAL)
	MAP_ENTRY(LINKED)

	TO_STRING_EPILOGUE
}

const SERecord::Id&   SERecord::getId() const {
	if (tag != Tag::ID) ERROR();
	if (tagValue == 0) ERROR();
	Id* ret = (Id*)tagValue;
	return *ret;
}
const SERecord::Cons&   SERecord::getCons() const {
	if (tag != Tag::CONS) ERROR();
	if (tagValue == 0) ERROR();
	Cons* ret = (Cons*)tagValue;
	return *ret;
}
const SERecord::Id::Linked&   SERecord::Id::getLinked() const {
	if (tag != Tag::LINKED) ERROR();
	if (tagValue == 0) ERROR();
	Linked* ret = (Linked*)tagValue;
	return *ret;
}
QString SERecord::Id::Linked::toString() const {
	return QString("%1").arg(this->linked->toString());
}

QString SERecord::Id::toString() const {
	switch(tag) {
	case Tag::TERMINAL:
	case Tag::SEQUENTIAL:
		return QString("%1 %2 %3 %4 %5 %6").arg(idCtx->toString()).arg(idType->toString()).
				arg(idInfo).arg(idValue).arg(hash->getValue().value).arg(toString(tag));
	case Tag::LINKED:
		return QString("%1 %2 %3 %4 %5 %6 %7").arg(idCtx->toString()).arg(idType->toString()).
				arg(idInfo).arg(idValue).arg(hash->getValue().value).arg(toString(tag)).arg(getLinked().toString());
		break;
	default:
		ERROR();
		return "???";
	}
}

const SERecord::Cons::Basic&      SERecord::Cons::getBasic()      const {
	if (tag != Tag::BASIC) ERROR();
	if (tagValue == 0) ERROR();
	Basic* ret = (Basic*)tagValue;
	return *ret;
}
const SERecord::Cons::Enumerated& SERecord::Cons::getEnumerated() const {
	if (tag != Tag::ENUMERATED) ERROR();
	if (tagValue == 0) ERROR();
	Enumerated* ret = (Enumerated*)tagValue;
	return *ret;
}
const SERecord::Cons::Record&     SERecord::Cons::getRecord()     const {
	if (tag != Tag::RECORD) ERROR();
	if (tagValue == 0) ERROR();
	Record* ret = (Record*)tagValue;
	return *ret;
}
const SERecord::Cons::Ref&        SERecord::Cons::getRef()        const {
	if (tag != Tag::REF) ERROR();
	if (tagValue == 0) ERROR();
	Ref* ret = (Ref*)tagValue;
	return *ret;
}
const SERecord::Cons::Array&      SERecord::Cons::getArray()      const {
	if (tag != Tag::ARRAY) ERROR();
	if (tagValue == 0) ERROR();
	Array* ret = (Array*)tagValue;
	return *ret;
}
const SERecord::Cons::ArrayDesc&  SERecord::Cons::getArrayDesc()  const {
	if (tag != Tag::ARRAYDESC) ERROR();
	if (tagValue == 0) ERROR();
	ArrayDesc* ret = (ArrayDesc*)tagValue;
	return *ret;
}
const SERecord::Cons::Transfer&   SERecord::Cons::getTransfer()   const {
	if (tag != Tag::TRANSFER) ERROR();
	if (tagValue == 0) ERROR();
	Transfer* ret = (Transfer*)tagValue;
	return *ret;
}
const SERecord::Cons::Definition& SERecord::Cons::getDefinition() const {
	if (tag != Tag::DEFINITION) ERROR();
	if (tagValue == 0) ERROR();
	Definition* ret = (Definition*)tagValue;
	return *ret;
}
const SERecord::Cons::Union&      SERecord::Cons::getUnion()      const {
	if (tag != Tag::UNION) ERROR();
	if (tagValue == 0) ERROR();
	Union* ret = (Union*)tagValue;
	return *ret;
}
const SERecord::Cons::Sequence&   SERecord::Cons::getSequence()   const {
	if (tag != Tag::SEQUENCE) ERROR();
	if (tagValue == 0) ERROR();
	Sequence* ret = (Sequence*)tagValue;
	return *ret;
}
const SERecord::Cons::Relative&   SERecord::Cons::getRelative()   const {
	if (tag != Tag::RELATIVE) ERROR();
	if (tagValue == 0) ERROR();
	Relative* ret = (Relative*)tagValue;
	return *ret;
}
const SERecord::Cons::Subrange&   SERecord::Cons::getSubrange()   const {
	if (tag != Tag::SUBRANGE) ERROR();
	if (tagValue == 0) ERROR();
	Subrange* ret = (Subrange*)tagValue;
	return *ret;
}
const SERecord::Cons::Long&       SERecord::Cons::getLong()       const {
	if (tag != Tag::LONG) ERROR();
	if (tagValue == 0) ERROR();
	Long* ret = (Long*)tagValue;
	return *ret;
}
const SERecord::Cons::Real&       SERecord::Cons::getReal()       const {
	if (tag != Tag::REAL) ERROR();
	if (tagValue == 0) ERROR();
	Real* ret = (Real*)tagValue;
	return *ret;
}
const SERecord::Cons::Opaque&     SERecord::Cons::getOpaque()     const {
	if (tag != Tag::OPAQUE) ERROR();
	if (tagValue == 0) ERROR();
	Opaque* ret = (Opaque*)tagValue;
	return *ret;
}
const SERecord::Cons::Zone&       SERecord::Cons::getZone()       const {
	if (tag != Tag::ZONE) ERROR();
	if (tagValue == 0) ERROR();
	Zone* ret = (Zone*)tagValue;
	return *ret;
}
const SERecord::Cons::Bits&       SERecord::Cons::getBits()       const {
	if (tag != Tag::BITS) ERROR();
	if (tagValue == 0) ERROR();
	Bits* ret = (Bits*)tagValue;
	return *ret;
}
const SERecord::Cons::Record::Linked&   SERecord::Cons::Record::getLinked() const {
	if (tag != Tag::LINKED) ERROR();
	if (tagValue == 0) ERROR();
	Linked* ret = (Linked*)tagValue;
	return *ret;
}


QString SERecord::Cons::toString() const {
	switch(tag) {
	case Tag::MODE:
		return QString("%1").arg(toString(tag));
	case Tag::BASIC:
		return QString("%1 %2").arg(toString(tag)).arg(getBasic().toString());
	case Tag::ENUMERATED:
		return QString("%1 %2").arg(toString(tag)).arg(getEnumerated().toString());
	case Tag::RECORD:
		return QString("%1 %2").arg(toString(tag)).arg(getRecord().toString());
	case Tag::REF:
		return QString("%1 %2").arg(toString(tag)).arg(getRef().toString());
	case Tag::ARRAY:
		return QString("%1 %2").arg(toString(tag)).arg(getArray().toString());
	case Tag::ARRAYDESC:
		return QString("%1 %2").arg(toString(tag)).arg(getArrayDesc().toString());
	case Tag::TRANSFER:
		return QString("%1 %2").arg(toString(tag)).arg(getTransfer().toString());
	case Tag::DEFINITION:
		return QString("%1 %2").arg(toString(tag)).arg(getDefinition().toString());
	case Tag::UNION:
		return QString("%1 %2").arg(toString(tag)).arg(getUnion().toString());
	case Tag::SEQUENCE:
		return QString("%1 %2").arg(toString(tag)).arg(getSequence().toString());
	case Tag::RELATIVE:
		return QString("%1 %2").arg(toString(tag)).arg(getRelative().toString());
	case Tag::SUBRANGE:
		return QString("%1 %2").arg(toString(tag)).arg(getSubrange().toString());
	case Tag::LONG:
		return QString("%1 %2").arg(toString(tag)).arg(getLong().toString());
	case Tag::REAL:
		return QString("%1 %2").arg(toString(tag)).arg(getReal().toString());
	case Tag::OPAQUE:
		return QString("%1 %2").arg(toString(tag)).arg(getOpaque().toString());
	case Tag::ZONE:
		return QString("%1 %2").arg(toString(tag)).arg(getZone().toString());
	case Tag::ANY:
		return QString("%1").arg(toString(tag));
	case Tag::NIL:
		return QString("%1").arg(toString(tag));
	case Tag::BITS:
		return QString("%1 %2").arg(toString(tag)).arg(getBits().toString());
	case Tag::FIXED_SEQUENCE:
		return QString("%1").arg(toString(tag));
	default:
		ERROR();
		return "???";
	}
}

QString SERecord::Cons::Basic::toString() const {
	//          ordered(0:8..8): BOOLEAN,
	//          code(0:9..15): [0..16),
	//          length(1:0..15): BitLength],
	return QString("%1 %2 %3").arg(ordered ? "ordered" : "").arg(code).arg(length);
}
QString SERecord::Cons::Enumerated::toString() const {
	//          ordered(0:8..8), machineDep(0:9..9): BOOLEAN,
	//          unpainted(0:10..10): BOOLEAN,     -- un- for backward compatiblity
	//          sparse(0:11..15): BOOLEAN,
	//          valueCtx(1:0..15): CTXIndex,
	//          nValues(2:0..15): CARDINAL],
	return QString("%1 %2 %3 %4 %5 %6").arg(ordered ? "ordered" : "").arg(machineDep ? "machineDep" : "").arg(unpainted ? "unpainted" : "").
		arg(sparse ? "sparse" : "").arg(valueCtx->toString()).arg(nValues);
}
QString SERecord::Cons::Record::toString() const {
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
	switch(tag) {
	case Tag::NOT_LINKED:
		return QString("%1 %2 %3 %4 %5 %6 %7").arg(length).arg(argument ? "argument" : "").arg(monitored ? "monitored" : "").arg(machineDep ? "machineDep" : "").
			arg(painted ? "painted" : "").arg(fieldCtx->toString()).arg(toString(tag));
	case Tag::LINKED:
		return QString("%1 %2 %3 %4 %5 %6 %7 %8").arg(length).arg(argument ? "argument" : "").arg(monitored ? "monitored" : "").arg(machineDep ? "machineDep" : "").
			arg(painted ? "painted" : "").arg(fieldCtx->toString()).arg(toString(tag)).arg(getLinked().toString());
	default:
		ERROR();
		return "???";
	}
}
QString SERecord::Cons::Ref::toString() const {
	//          counted(0:8..8), ordered(0:9..9), readOnly(0:10..10), list(0:11..11), var(0:12..12), basing(0:13..15): BOOLEAN,
	//          refType(1:0..15): SEIndex],
	return QString("%1 %2 %3 %4 %5 %6 %7").arg(counted ? "counted" : "").arg(ordered ? "ordered" : "").arg(readOnly ? "readOnly" : "").
		arg(list ? "list" : "").arg(var ? "var" : "").arg(basing ? "basing" : "").arg(refType->toString());
}
QString SERecord::Cons::Array::toString() const {
	//          packed(0:8..15): BOOLEAN,
	//          indexType(1:0..15): SEIndex,
	//          componentType(2:0..15): SEIndex],
	return QString("%1 %2 %3").arg(packed ? "packed" : "").arg(indexType->toString()).arg(componentType->toString());
}
QString SERecord::Cons::ArrayDesc::toString() const {
	//          var(0:8..8), readOnly(0::9..15): BOOLEAN,
	//          describedType(1:0..15): SEIndex],
	return QString("%1 %2 %3").arg(var ? "var" : "").arg(readOnly ? "readOnly" : "").arg(describedType->toString());
}
QString SERecord::Cons::Transfer::toString() const {
	//          safe(0:8..8): BOOLEAN,
	//          mode(0:9..15): TransferMode,
	//          typeIn(1:0..15), typeOut(2:0..15): CSEIndex],
	return QString("%1 %2 %3 %4").arg(safe ? "safe" : "").arg(toString(mode)).arg(typeIn->toString()).arg(typeOut->toString());
}
QString SERecord::Cons::Definition::toString() const {
	//          named(0:8..15): BOOLEAN,
	//          defCtx(1:0..15): CTXIndex],
	return QString("%1 %2").arg(named ? "named" : "").arg(defCtx->toString());
}
QString SERecord::Cons::Union::toString() const {
	//          overlaid(0:12..12), controlled(0:13..13), machineDep(0:14..15): BOOLEAN,
	//          caseCtx(1:0..15): CTXIndex,
	//          tagSei(2:0..15): ISEIndex],
	return QString("%1 %2 %3 %4 %5").arg(overlaid ? "overlaid" : ""). arg(controlled ? "controlled" : "").arg(machineDep ? "machineDep" : "").
			arg(caseCtx->toString()).arg(tagSei->toString());
}
QString SERecord::Cons::Sequence::toString() const {
	//          packed(0:8..8): BOOLEAN,
	//          controlled(0:9..9), machineDep(0:10..15): BOOLEAN,
	//          tagSei(1:0..15): ISEIndex,
	//          componentType(2:0..15): SEIndex],
	return QString("%1 %2 %3 %4 %5").arg(packed ? "packed" : "").arg(controlled ? "controlled" : "").arg(machineDep ? "machineDep" : "").
			arg(tagSei->toString()).arg(componentType->toString());
}
QString SERecord::Cons::Relative::toString() const {
	//          baseType(1:0..15): SEIndex,
	//          offsetType(2:0..15): SEIndex,
	//          resultType(3:0..15): SEIndex],
	return QString("%1 %2 %3").arg(baseType->toString()).arg(offsetType->toString()).arg(resultType->toString());
}
QString SERecord::Cons::Subrange::toString() const {
	//          filled(0:8..8), empty(0:9..15): BOOLEAN,
	//          rangeType(1:0..15): SEIndex,
	//          origin(2:0..15): INTEGER,
	//          range(3:0..15): CARDINAL],
	return QString("%1 %2 %3 %4 %5").arg(filled ? "filled" : "").arg(empty ? "empty" : "").arg(rangeType->toString()).arg(origin).arg(range);
}
QString SERecord::Cons::Long::toString() const {
	return QString("%1").arg(rangeType->toString());
}
QString SERecord::Cons::Real::toString() const {
	return QString("%1").arg(rangeType->toString());
}
QString SERecord::Cons::Opaque::toString() const {
	return QString("%1 %2 %3").arg(lengthKnown ? "known" : "").arg(length).arg(id->toString());
}
QString SERecord::Cons::Zone::toString() const {
	return QString("%1%2").arg(counted ? "counted" : " ").arg(mds ? "mds" : " ");
}
QString SERecord::Cons::Bits::toString() const {
	return QString("%1").arg(length);
}

QString SERecord::Cons::Record::Linked::toString() const {
	return QString("%1").arg(this->linkType->toString());
}

QString SERecord::Cons::Record::toString(Tag value) {
	TO_STRING_PROLOGUE(Tag)

	MAP_ENTRY(NOT_LINKED)
	MAP_ENTRY(LINKED)

	TO_STRING_EPILOGUE
}


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

SERecord* SERecord::getInstance(Symbols* symbols, CARD16 index) {
	//  body(0:2..95): SELECT seTag(0:2..2): * FROM
	CARD16 u0 = symbols->file->getCARD16();
	Tag   tag      = (Tag)bitField(u0, 2);
	void* tagValue = 0;
	switch(tag) {
	case Tag::ID:
		tagValue = Id::getInstance(symbols, u0);
		break;
	case Tag::CONS:
		tagValue = Cons::getInstance(symbols, u0);
		break;
	default:
		ERROR();
		tagValue = 0;
	}

	return new SERecord(symbols, index, tag, tagValue);
}

SERecord::Id* SERecord::Id::getInstance(Symbols* symbols, CARD16 u0) {
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
	bool      extended = bitField(u0, 3);
	bool      public_  = bitField(u0, 4);
	CTXIndex* idCtx    = CTXIndex::getInstance(symbols, bitField(u0, 5, 15));

	CARD16 u1 = symbols->file->getCARD16();
	bool     immutable = bitField(u1, 0);
	bool     constant  = bitField(u1, 1);
	SEIndex* idType    = SEIndex::getInstance(symbols, bitField(u1, 2, 15));

	CARD16 idInfo  = symbols->file->getCARD16();
	CARD16 idValue = symbols->file->getCARD16();

	CARD16 u4 = symbols->file->getCARD16();
	HTIndex* hash      = HTIndex::getInstance(symbols, bitField(u4, 0, 12));
	bool     linkSpace = bitField(u4, 13);
	Tag      tag       = (Tag)bitField(u4, 14, 15);
	void*    tagValue;
	switch(tag) {
	case Tag::TERMINAL:
	case Tag::SEQUENTIAL:
		tagValue = 0;
		break;
	case Tag::LINKED:
	{
		SEIndex* linked = SEIndex::getInstance(symbols, symbols->file->getCARD16());
		tagValue = new Linked(linked);
	}
		break;
	default:
		ERROR();
		tagValue = 0;
	}

	return new Id(extended, public_, idCtx, immutable, constant, idType, idInfo, idValue, hash, linkSpace, tag, tagValue);
}

SERecord::Cons* SERecord::Cons::getInstance(Symbols* symbols, CARD16 u0) {
	//      typeInfo(0:3..63): SELECT typeTag(0:3..7): TypeClass FROM
	Tag   tag = (Tag)bitField(u0, 3, 7);
	void *tagValue;

	switch(tag) {
	case Tag::MODE:
		tagValue = 0;
		break;
	case Tag::BASIC:
		tagValue = Basic::getInstance(symbols, u0);
		break;
	case Tag::ENUMERATED:
		tagValue = Enumerated::getInstance(symbols, u0);
		break;
	case Tag::RECORD:
		tagValue = Record::getInstance(symbols, u0);
		break;
	case Tag::REF:
		tagValue = Ref::getInstance(symbols, u0);
		break;
	case Tag::ARRAY:
		tagValue = Array::getInstance(symbols, u0);
		break;
	case Tag::ARRAYDESC:
		tagValue = ArrayDesc::getInstance(symbols, u0);
		break;
	case Tag::TRANSFER:
		tagValue = Transfer::getInstance(symbols, u0);
		break;
	case Tag::DEFINITION:
		tagValue = Definition::getInstance(symbols, u0);
		break;
	case Tag::UNION:
		tagValue = Union::getInstance(symbols, u0);
		break;
	case Tag::SEQUENCE:
		tagValue = Sequence::getInstance(symbols, u0);
		break;
	case Tag::RELATIVE:
		tagValue = Relative::getInstance(symbols, u0);
		break;
	case Tag::SUBRANGE:
		tagValue = Subrange::getInstance(symbols, u0);
		break;
	case Tag::LONG:
		tagValue = Long::getInstance(symbols, u0);
		break;
	case Tag::REAL:
		tagValue = Real::getInstance(symbols, u0);
		break;
	case Tag::OPAQUE:
		tagValue = Opaque::getInstance(symbols, u0);
		break;
	case Tag::ZONE:
		tagValue = Zone::getInstance(symbols, u0);
		break;
	case Tag::ANY:
		tagValue = 0;
		break;
	case Tag::NIL:
		tagValue = 0;
		break;
	case Tag::BITS:
		tagValue = Bits::getInstance(symbols, u0);
		break;
	case Tag::FIXED_SEQUENCE:
		ERROR();
		tagValue = 0;
		break;
	default:
		ERROR();
		tagValue = 0;
	}
	return new Cons(tag, tagValue);
}

SERecord::Cons::Basic* SERecord::Cons::Basic::getInstance(Symbols* symbols, CARD16 u0) {
	//          ordered(0:8..8): BOOLEAN,
	//          code(0:9..15): [0..16),
	//          length(1:0..15): BitLength],
	bool   ordered = bitField(u0, 8);
	CARD16 code    = bitField(u0, 9, 15);
	CARD16 length  = symbols->file->getCARD16();

	return new Basic(ordered, code, length);
}

SERecord::Cons::Enumerated* SERecord::Cons::Enumerated::getInstance(Symbols* symbols, CARD16 u0) {
	//          ordered(0:8..8), machineDep(0:9..9): BOOLEAN,
	//          unpainted(0:10..10): BOOLEAN,     -- un- for backward compatiblity
	//          sparse(0:11..15): BOOLEAN,
	//          valueCtx(1:0..15): CTXIndex,
	//          nValues(2:0..15): CARDINAL],
	bool      ordered    = bitField(u0, 8);
	bool      machineDep = bitField(u0, 9);
	bool      unpainted  = bitField(u0, 10);
	bool      sparse     = bitField(u0, 11, 15);
	CTXIndex* valueCtx   = CTXIndex::getInstance(symbols, symbols->file->getCARD16());
	CARD16    nValues    = symbols->file->getCARD16();

	return new Enumerated(ordered, machineDep, unpainted, sparse, valueCtx, nValues);
}

SERecord::Cons::Record* SERecord::Cons::Record::getInstance(Symbols* symbols, CARD16 u0) {
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
	bool   privateFields = bitField(u0, 4);

	CARD16 length        = symbols->file->getCARD16();

	CARD16 u2 = symbols->file->getCARD16();
	bool      argumented = bitField(u2, 0);
	bool      monitored  = bitField(u2, 1);
	bool      machineDep = bitField(u2, 2);
	bool      painted    = bitField(u2, 3);
	CTXIndex* fieldCtx   = CTXIndex::getInstance(symbols, bitField(u2, 4, 14));

	Tag tag = (Tag)bitField(u2, 15);
	void* tagValue;
	switch(tag) {
	case Tag::NOT_LINKED:
		tagValue = 0;
		break;
	case Tag::LINKED:
	{
		SEIndex* linkType = SEIndex::getInstance(symbols, symbols->file->getCARD16());
		tagValue = new Linked(linkType);
	}
		break;
	default:
		ERROR();
		tagValue = 0;
	}

	return new Record(privateFields, length, argumented, monitored, machineDep, painted, fieldCtx, tag, tagValue);
}

SERecord::Cons::Ref* SERecord::Cons::Ref::getInstance(Symbols* symbols, CARD16 u0) {
	//          counted(0:8..8), ordered(0:9..9), readOnly(0:10..10), list(0:11..11), var(0:12..12), basing(0:13..15): BOOLEAN,
	//          refType(1:0..15): SEIndex],
	bool     counted  = bitField(u0, 8);
	bool     ordered  = bitField(u0, 9);
	bool     readOnly = bitField(u0, 10);
	bool     list     = bitField(u0, 11);
	bool     var      = bitField(u0, 12);
	bool     basing   = bitField(u0, 13, 15);
	SEIndex* refType = SEIndex::getInstance(symbols, symbols->file->getCARD16());

	return new Ref(counted, ordered, readOnly, list, var, basing, refType);
}

SERecord::Cons::Array* SERecord::Cons::Array::getInstance(Symbols* symbols, CARD16 u0) {
	//          packed(0:8..15): BOOLEAN,
	//          indexType(1:0..15): SEIndex,
	//          componentType(2:0..15): SEIndex],
	bool packed            = bitField(u0, 8, 15);
	SEIndex* indexType     = SEIndex::getInstance(symbols, symbols->file->getCARD16());
	SEIndex* componentType = SEIndex::getInstance(symbols, symbols->file->getCARD16());

	return new Array(packed, indexType, componentType);
}

SERecord::Cons::ArrayDesc* SERecord::Cons::ArrayDesc::getInstance(Symbols* symbols, CARD16 u0) {
	//          var(0:8..8), readOnly(0::9..15): BOOLEAN,
	//          describedType(1:0..15): SEIndex],
	bool     var           = bitField(u0, 8);
	bool     readOnly      = bitField(u0, 9, 15);
	SEIndex* describedType = SEIndex::getInstance(symbols, symbols->file->getCARD16());

	return new ArrayDesc(var, readOnly, describedType);
}

SERecord::Cons::Transfer* SERecord::Cons::Transfer::getInstance(Symbols* symbols, CARD16 u0) {
	//          safe(0:8..8): BOOLEAN,
	//          mode(0:9..15): TransferMode,
	//          typeIn(1:0..15), typeOut(2:0..15): CSEIndex],
	bool         safe    = bitField(u0, 8);
	TransferMode mode    = (TransferMode)bitField(u0, 9, 15);
	SEIndex*     typeIn  = SEIndex::getInstance(symbols, symbols->file->getCARD16());
	SEIndex*     typeOut = SEIndex::getInstance(symbols, symbols->file->getCARD16());

	return new Transfer(safe, mode, typeIn, typeOut);
}

SERecord::Cons::Definition* SERecord::Cons::Definition::getInstance(Symbols* symbols, CARD16 u0) {
	//          named(0:8..15): BOOLEAN,
	//          defCtx(1:0..15): CTXIndex],
	bool      named  = bitField(u0, 8, 15);
	CTXIndex* defCtx = CTXIndex::getInstance(symbols, symbols->file->getCARD16());

	return new Definition(named, defCtx);
}

SERecord::Cons::Union* SERecord::Cons::Union::getInstance(Symbols* symbols, CARD16 u0) {
	//          hints(0:8..11): RECORD [
	//            equalLengths(0:0..0): BOOLEAN,
	//            refField(0:1..1), default(0:2..2), voidable(0:3..3): BOOLEAN],
	//          overlaid(0:12..12), controlled(0:13..13), machineDep(0:14..15): BOOLEAN,
	//          caseCtx(1:0..15): CTXIndex,
	//          tagSei(2:0..15): ISEIndex],
	bool      overlaid   = bitField(u0, 12);
	bool      controlled = bitField(u0, 13);
	bool      machineDep = bitField(u0, 14, 15);
	CTXIndex* caseCtx    = CTXIndex::getInstance(symbols, symbols->file->getCARD16());
	SEIndex*  tagSei     = SEIndex::getInstance(symbols, symbols->file->getCARD16());

	return new Union(overlaid, controlled, machineDep, caseCtx, tagSei);
}

SERecord::Cons::Sequence* SERecord::Cons::Sequence::getInstance(Symbols* symbols, CARD16 u0) {
	//          packed(0:8..8): BOOLEAN,
	//          controlled(0:9..9), machineDep(0:10..15): BOOLEAN,
	//          tagSei(1:0..15): ISEIndex,
	//          componentType(2:0..15): SEIndex],
	bool     packed        = bitField(u0, 8);
	bool     controlled    = bitField(u0, 9);
	bool     machineDep    = bitField(u0, 10, 15);
	SEIndex* tagSei        = SEIndex::getInstance(symbols, symbols->file->getCARD16());
	SEIndex* componentType = SEIndex::getInstance(symbols, symbols->file->getCARD16());

	return new Sequence(packed, controlled, machineDep, tagSei, componentType);
}

SERecord::Cons::Relative* SERecord::Cons::Relative::getInstance(Symbols* symbols, CARD16 /*u0*/) {
	//          baseType(1:0..15): SEIndex,
	//          offsetType(2:0..15): SEIndex,
	//          resultType(3:0..15): SEIndex],
	SEIndex* baseType   = SEIndex::getInstance(symbols, symbols->file->getCARD16());
	SEIndex* offsetType = SEIndex::getInstance(symbols, symbols->file->getCARD16());
	SEIndex* resultType = SEIndex::getInstance(symbols, symbols->file->getCARD16());

	return new Relative(baseType, offsetType, resultType);
}

SERecord::Cons::Subrange* SERecord::Cons::Subrange::getInstance(Symbols* symbols, CARD16 u0) {
	//          filled(0:8..8), empty(0:9..15): BOOLEAN,
	//          rangeType(1:0..15): SEIndex,
	//          origin(2:0..15): INTEGER,
	//          range(3:0..15): CARDINAL],
	bool     filled    = bitField(u0, 8);
	bool     empty     = bitField(u0, 9, 15);
	SEIndex* rangeType = SEIndex::getInstance(symbols, symbols->file->getCARD16());
	INT16    origin    = (INT16)symbols->file->getCARD16();
	CARD16   range    = symbols->file->getCARD16();

	return new Subrange(filled, empty, rangeType, origin, range);
}

SERecord::Cons::Long* SERecord::Cons::Long::getInstance(Symbols* symbols, CARD16 /*u0*/) {
	//        long, real => [rangeType(1:0..15): SEIndex],
	SEIndex* rangeType = SEIndex::getInstance(symbols, symbols->file->getCARD16());

	return new Long(rangeType);
}

SERecord::Cons::Real* SERecord::Cons::Real::getInstance(Symbols* symbols, CARD16 /*u0*/) {
	//        long, real => [rangeType(1:0..15): SEIndex],
	SEIndex* rangeType = SEIndex::getInstance(symbols, symbols->file->getCARD16());

	return new Real(rangeType);
}

SERecord::Cons::Opaque* SERecord::Cons::Opaque::getInstance(Symbols* symbols, CARD16 u0) {
	//          lengthKnown(0:8..15): BOOLEAN,
	//          length(1:0..15): BitLength,
	//          id(2:0..15): ISEIndex],
	bool     lengthKnown = bitField(u0, 8, 15);
	CARD16   length      = symbols->file->getCARD16();
	SEIndex* id          = SEIndex::getInstance(symbols, symbols->file->getCARD16());

	return new Opaque(lengthKnown, length, id);
}

SERecord::Cons::Zone* SERecord::Cons::Zone::getInstance(Symbols* /*symbols*/, CARD16 u0) {
	//        zone => [counted(0:8..8), mds(0:9..15): BOOLEAN],
	bool counted = bitField(u0, 8);
	bool mds     = bitField(u0, 9, 15);

	return new Zone(counted, mds);
}

SERecord::Cons::Bits* SERecord::Cons::Bits::getInstance(Symbols* symbols, CARD16 /*u0*/) {
	//        bits => [length(1:0..31): BitCount],   -- placed here to avoid
	CARD32 length = symbols->file->getCARD32();

	return new Bits(length);
}
