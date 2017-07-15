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
SEIndex* SEIndex::getNull() {
	static SEIndex ret(0, SEIndex::SE_NULL);
	return &ret;
}
SEIndex* SEIndex::getInstance(Symbols* symbols, CARD16 index) {
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


//
// SERecord
//

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

QMap<SERecord::Key, SERecord*> SERecord::all;
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
	if (tag != Tag::ID) ERROR();
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

