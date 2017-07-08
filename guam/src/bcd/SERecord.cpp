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
// SERecord.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("serecord");

#include "SERecord.h"

#include "Symbols.h"
#include "CTXRecord.h"
#include "HTRecord.h"


QList<SEIndex*> SEIndex::all;

SEIndex::SEIndex(Symbols* symbols_, CARD16 index_) : symbols(symbols_), index(index_), value(0) {
	all.append(this);
}

QString SEIndex::toString() {
	if (index == SEIndex::SE_NULL) return "#NULL#";
	if (value == 0) return QString("se-%1").arg(index);

	return QString("se-%1").arg(index);
//	return QString("se-%1-%2").arg(index).arg(value->toString());
}
void SEIndex::resolve() {
	logger.info("all %d", all.size());
	for(SEIndex *p: all) {
		const CARD16 index = p->index;

		if (p->value) continue;
		if (p->index == SE_NULL) continue;

		if (p->symbols == 0) {
			logger.error("p->symbols == null %d p = %s", index, p->toString().toLocal8Bit().constData());
			ERROR();
		}
		if (p->symbols->se.contains(index)) {
			logger.info("resolve se %4d", index);
			p->value = p->symbols->se[index];
		} else {
			logger.error("Unknown %d = %s", index, p->toString().toLocal8Bit().constData());
			ERROR();
		}
	}
}


QString SERecord::toString(SeTag value) {
	TO_STRING_PROLOGUE(SeTag)

	MAP_ENTRY(ID)
	MAP_ENTRY(CONS)

	TO_STRING_EPILOGUE
}

QString SERecord::toString(LinkTag value) {
	TO_STRING_PROLOGUE(LinkTag)

	MAP_ENTRY(TERMINAL)
	MAP_ENTRY(SEQUENTIAL)
	MAP_ENTRY(LINKED)

	TO_STRING_EPILOGUE
}


QString SERecord::toString(LinkTag2 value) {
	TO_STRING_PROLOGUE(LinkTag2)

	MAP_ENTRY(NOT_LINKED)
	MAP_ENTRY(LINKED)

	TO_STRING_EPILOGUE
}


QString SERecord::toString(TypeClass value) {
	TO_STRING_PROLOGUE(TypeClass)

	MAP_ENTRY(MODE)
	MAP_ENTRY(BASIC)
	MAP_ENTRY(ENUMERATED)
	MAP_ENTRY(RECORD)
	MAP_ENTRY(REF)
	//
	MAP_ENTRY(ARRAY)
	MAP_ENTRY(ARRAY_DESC)
	MAP_ENTRY(TRANSFER)
	MAP_ENTRY(DEFINITION)
	MAP_ENTRY(UNION)
	//
	MAP_ENTRY(SEQUENCE)
	MAP_ENTRY(RELATIVE)
	MAP_ENTRY(SUB_RANGE)
	MAP_ENTRY(LONG)
	MAP_ENTRY(REAL)
	//
	MAP_ENTRY(OPAQUE)
	MAP_ENTRY(ZONE)
	MAP_ENTRY(ANY)
	MAP_ENTRY(NIL)
	MAP_ENTRY(BITS)
	//
	MAP_ENTRY(FIXED_SEQUENCE)

	TO_STRING_EPILOGUE
}

QString SERecord::toString(TransferMode value) {
	TO_STRING_PROLOGUE(TransferMode)

	MAP_ENTRY(PROC)
	MAP_ENTRY(PORT)
	MAP_ENTRY(SIGNAL)
	MAP_ENTRY(ERROR)
	MAP_ENTRY(PROCESS)
	MAP_ENTRY(PROGRAM)
	MAP_ENTRY(NONE)

	TO_STRING_EPILOGUE
}


QString SERecord::toString() {
	if (index == SEIndex::SE_NULL) return "#NULL#";
	switch(seTag) {
	case SeTag::ID:
		return QString("SE %1 ID   %2").arg(index, 4).arg(id.hash->getValue());
	case SeTag::CONS:
		return QString("SE %1 CONS %2").arg(index, 4).arg(toString(cons.typeTag));
	default:
		ERROR();
	}
}

SERecord::SERecord(Symbols* symbols, CARD16 index_) {
	index = index_;

	CARD16 u0 = symbols->file->getCARD16();

    //  body(0:2..95): SELECT seTag(0:2..2): * FROM
	seTag = (SeTag)bitField(u0, 2);
	switch(seTag) {
	case SeTag::ID:
		initID(symbols, u0);
		break;
	case SeTag::CONS:
		initCONS(symbols, u0);
		break;
	default:
		ERROR();
	}
}

void SERecord::initID(Symbols* symbols, CARD16 u0) {
	id.extended = bitField(u0, 3);
	id.public_  = bitField(u0, 4);
	id.idCtx    = new CTXIndex(symbols, bitField(u0, 5, 15));

	CARD16 u1 = symbols->file->getCARD16();
	id.immutable = bitField(u1, 0);
	id.constant  = bitField(u1, 1);
	id.idType    = new SEIndex(symbols, bitField(u1, 2, 15));

	id.idInfo    = symbols->file->getCARD16(); // u2
	id.idValue   = symbols->file->getCARD16(); // u3

	CARD16 u4    = symbols->file->getCARD16();
	id.hash      = new HTIndex(symbols, bitField(u4, 0, 12));
	id.linkSpace = bitField(u4, 13);
	id.linkTag   = (LinkTag)bitField(u4, 14, 15);

	switch(id.linkTag) {
	case LinkTag::LINKED:
		id.linked.link = new SEIndex(symbols, symbols->file->getCARD16());
		break;
	case LinkTag::SEQUENTIAL:
	case LinkTag::TERMINAL:
		break;
	default:
		ERROR();
	}
}

void SERecord::initCONS(Symbols* symbols, CARD16 u0) {
	//      typeInfo(0:3..63): SELECT typeTag(0:3..7): TypeClass FROM
	cons.typeTag = (TypeClass)bitField(u0, 3, 7);
	switch(cons.typeTag) {
	case TypeClass::MODE:
		break;
	case TypeClass::BASIC:
		initBASIC(symbols, u0);
		break;
	case TypeClass::ENUMERATED:
		initENUMERATED(symbols, u0);
		break;
	case TypeClass::RECORD:
		initRECORD(symbols, u0);
		break;
	case TypeClass::REF:
		initREF(symbols, u0);
		break;
	case TypeClass::ARRAY:
		initARRAY(symbols, u0);
		break;
	case TypeClass::ARRAY_DESC:
		initARRAY_DESC(symbols, u0);
		break;
	case TypeClass::TRANSFER:
		initTRANSFER(symbols, u0);
		break;
	case TypeClass::DEFINITION:
		initDEFINITION(symbols, u0);
		break;
	case TypeClass::UNION:
		initUNION(symbols, u0);
		break;
	case TypeClass::SEQUENCE:
		initSEQUENCE(symbols, u0);
		break;
	case TypeClass::RELATIVE:
		initRELATIVE(symbols, u0);
		break;
	case TypeClass::SUB_RANGE:
		initSUB_RANGE(symbols, u0);
		break;
	case TypeClass::LONG:
		initLONG(symbols, u0);
		break;
	case TypeClass::REAL:
		initREAL(symbols, u0);
		break;
	case TypeClass::OPAQUE:
		initOPAQUE(symbols, u0);
		break;
	case TypeClass::ZONE:
		initZONE(symbols, u0);
		break;
	case TypeClass::ANY:
		initANY(symbols, u0);
		break;
	case TypeClass::NIL:
		initNIL(symbols, u0);
		break;
	case TypeClass::BITS:
		initBITS(symbols, u0);
		break;
	case TypeClass::FIXED_SEQUENCE:
		initFIXED_SEQUENCE(symbols, u0);
		break;
	default:
		logger.fatal("typeTag %d", (int)cons.typeTag);
		logger.fatal("typeTag %s", toString(cons.typeTag).toLocal8Bit().constData());
		ERROR();
	}
}

void SERecord::initMODE(Symbols* /*symbols*/, CARD16 /*u0*/) {
	//        mode => [],
}

void SERecord::initBASIC(Symbols* symbols, CARD16 u0) {
	//        basic => [
	//          ordered(0:8..8): BOOLEAN,
	//          code(0:9..15): [0..16),
	//          length(1:0..15): BitLength],
	cons.basic.ordered = bitField(u0, 8);
	cons.basic.code    = bitField(u0, 9, 15);
	cons.basic.length  = symbols->file->getCARD16();
}

void SERecord::initENUMERATED(Symbols* symbols, CARD16 u0) {
	//        enumerated => [
	//          ordered(0:8..8), machineDep(0:9..9): BOOLEAN,
	//          unpainted(0:10..10): BOOLEAN,     -- un- for backward compatiblity
	//          sparse(0:11..15): BOOLEAN,
	//          valueCtx(1:0..15): CTXIndex,
	//          nValues(2:0..15): CARDINAL],
	cons.enumerated.ordred     = bitField(u0, 8);
	cons.enumerated.machineDep = bitField(u0, 9);
	cons.enumerated.unpainted  = bitField(u0, 10);
	cons.enumerated.sparse     = bitField(u0, 11, 15);
	cons.enumerated.valueCtx   = new CTXIndex(symbols, symbols->file->getCARD16());
	cons.enumerated.nValues    = symbols->file->getCARD16();
}

void SERecord::initRECORD(Symbols* symbols, CARD16 u0) {
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
	cons.record.comparable    = bitField(u0, 8);
	cons.record.assignable    = bitField(u0, 9);
	cons.record.unifield      = bitField(u0, 10);
	cons.record.variant       = bitField(u0, 11);
	cons.record.privateFields = bitField(u0, 12);
	cons.record.refField      = bitField(u0, 13);
	cons.record.default_      = bitField(u0, 14);
	cons.record.voidable      = bitField(u0, 15);

	cons.record.length = symbols->file->getCARD16();

	CARD16 u2 = symbols->file->getCARD16();
	cons.record.argument   = bitField(u2, 0);
	cons.record.monitored  = bitField(u2, 1);
	cons.record.machineDep = bitField(u2, 2);
	cons.record.painted    = bitField(u2, 3);
	cons.record.fieldCtx   = new CTXIndex(symbols, bitField(u2, 4, 14));
	cons.record.linkTag    = (LinkTag2)bitField(u2, 15);

	switch(cons.record.linkTag) {
	case LinkTag2::LINKED:
		cons.record.linked.linkType = new SEIndex(symbols, symbols->file->getCARD16());
	case LinkTag2::NOT_LINKED:
		break;
	default:
		ERROR();
	}
}

void SERecord::initREF(Symbols* symbols, CARD16 u0) {
	//        ref => [
	//          counted(0:8..8), ordered(0:9..9), readOnly(0:10..10), list(0:11..11), var(0:12..12), basing(0:13..15): BOOLEAN,
	//          refType(1:0..15): SEIndex],
	cons.ref.counted  = bitField(u0, 8);
	cons.ref.ordered  = bitField(u0, 9);
	cons.ref.readOnly = bitField(u0, 10);
	cons.ref.list     = bitField(u0, 11);
	cons.ref.var      = bitField(u0, 12);
	cons.ref.basing   = bitField(u0, 13, 15);

	cons.ref.refType  = new SEIndex(symbols, symbols->file->getCARD16());
}
void SERecord::initARRAY(Symbols* symbols, CARD16 u0) {
	//        array => [
	//          packed(0:8..15): BOOLEAN,
	//          indexType(1:0..15): SEIndex,
	//          componentType(2:0..15): SEIndex],
	cons.array.packed        = bitField(u0, 8, 15);

	cons.array.indexType     = new SEIndex(symbols, symbols->file->getCARD16());
	cons.array.componentType = new SEIndex(symbols, symbols->file->getCARD16());

}
void SERecord::initARRAY_DESC(Symbols* symbols, CARD16 u0) {
	//        arraydesc => [
	//          var(0:8..8), readOnly(0::9..15): BOOLEAN,
	//          describedType(1:0..15): SEIndex],
	cons.arraydesc.var           = bitField(u0, 8);
	cons.arraydesc.readOnly      = bitField(u0, 9, 15);

	cons.arraydesc.describedType = new SEIndex(symbols, symbols->file->getCARD16());
}
void SERecord::initTRANSFER(Symbols* symbols, CARD16 u0) {
	//        transfer => [
	//          safe(0:8..8): BOOLEAN,
	//          mode(0:9..15): TransferMode,
	//          typeIn(1:0..15), typeOut(2:0..15): CSEIndex],
	cons.transfer.safe    = bitField(u0, 8);
	cons.transfer.mode    = (TransferMode)bitField(u0, 9, 15);

	cons.transfer.typeIn  = new SEIndex(symbols, symbols->file->getCARD16());
	cons.transfer.typeOut = new SEIndex(symbols, symbols->file->getCARD16());

}
void SERecord::initDEFINITION(Symbols* symbols, CARD16 u0) {
	//        definition => [
	//          named(0:8..15): BOOLEAN,
	//          defCtx(1:0..15): CTXIndex],
	cons.definition.named  = bitField(u0, 8, 15);

	cons.definition.defCtx = new CTXIndex(symbols, symbols->file->getCARD16());
}
void SERecord::initUNION(Symbols* symbols, CARD16 u0) {
	//        union => [
	//          hints(0:8..11): RECORD [
	//            equalLengths(0:0..0): BOOLEAN,
	//            refField(0:1..1), default(0:2..2), voidable(0:3..3): BOOLEAN],
	//          overlaid(0:12..12), controlled(0:13..13), machineDep(0:14..15): BOOLEAN,
	//          caseCtx(1:0..15): CTXIndex,
	//          tagSei(2:0..15): ISEIndex],
	cons.union_.equalLengths = bitField(u0, 8);
	cons.union_.refField     = bitField(u0, 9);
	cons.union_.default_     = bitField(u0, 10);
	cons.union_.voidable     = bitField(u0, 11);
	cons.union_.overlaid     = bitField(u0, 12);
	cons.union_.controlled   = bitField(u0, 13);
	cons.union_.machineDep   = bitField(u0, 14, 15);

	cons.union_.caseCtx      = new CTXIndex(symbols, symbols->file->getCARD16());
	cons.union_.tagSei       = new SEIndex(symbols, symbols->file->getCARD16());
}
void SERecord::initSEQUENCE(Symbols* symbols, CARD16 u0) {
	//        sequence => [
	//          packed(0:8..8): BOOLEAN,
	//          controlled(0:9..9), machineDep(0:10..15): BOOLEAN,
	//          tagSei(1:0..15): ISEIndex,
	//          componentType(2:0..15): SEIndex],
	cons.sequence.packed        = bitField(u0, 8);
	cons.sequence.controlled    = bitField(u0, 9);
	cons.sequence.machineDep    = bitField(u0, 10, 15);

	cons.sequence.tagSei        = new SEIndex(symbols, symbols->file->getCARD16());
	cons.sequence.componentType = new SEIndex(symbols, symbols->file->getCARD16());
}
void SERecord::initRELATIVE(Symbols* symbols, CARD16 /*u0*/) {
	//        relative => [
	//          baseType(1:0..15): SEIndex,
	//          offsetType(2:0..15): SEIndex,
	//          resultType(3:0..15): SEIndex],
	cons.relative.baseType   = new SEIndex(symbols, symbols->file->getCARD16());
	cons.relative.offsetType = new SEIndex(symbols, symbols->file->getCARD16());
	cons.relative.resultType = new SEIndex(symbols, symbols->file->getCARD16());
}
void SERecord::initSUB_RANGE(Symbols* symbols, CARD16 u0) {
	//        subrange => [
	//          filled(0:8..8), empty(0:9..15): BOOLEAN,
	//          rangeType(1:0..15): SEIndex,
	//          origin(2:0..15): INTEGER,
	//          range(3:0..15): CARDINAL],
	cons.subrange.filled    = bitField(u0, 8);
	cons.subrange.empty     = bitField(u0, 9, 15);
	cons.subrange.rangeType = new SEIndex(symbols, symbols->file->getCARD16());
	cons.subrange.origin    = (INT16)symbols->file->getCARD16();
	cons.subrange.range     = symbols->file->getCARD16();
}
void SERecord::initLONG(Symbols* symbols, CARD16 /*u0*/) {
	//        long, real => [rangeType(1:0..15): SEIndex],
	cons.long_.rangeType = new SEIndex(symbols, symbols->file->getCARD16());
}
void SERecord::initREAL(Symbols* symbols, CARD16 /*u0*/) {
	//        long, real => [rangeType(1:0..15): SEIndex],
	cons.real_.rangeType = new SEIndex(symbols, symbols->file->getCARD16());
}
void SERecord::initOPAQUE(Symbols* symbols, CARD16 u0) {
	//        opaque => [
	//          lengthKnown(0:8..15): BOOLEAN,
	//          length(1:0..15): BitLength,
	//          id(2:0..15): ISEIndex],
	cons.opaque.lengthKnown = bitField(u0, 8, 15);

	cons.opaque.length      = symbols->file->getCARD16();
	cons.opaque.id          = new SEIndex(symbols, symbols->file->getCARD16());
}
void SERecord::initZONE(Symbols* /*symbols*/, CARD16 u0) {
	//        zone => [counted(0:8..8), mds(0:9..15): BOOLEAN],
	cons.zone.counted = bitField(u0, 8);
	cons.zone.mds     = bitField(u0, 9, 15);
}
void SERecord::initANY(Symbols* /*symbols*/, CARD16 /*u0*/) {
	//        any => [],
}
void SERecord::initNIL(Symbols* /*symbols*/, CARD16 /*u0*/) {
	//        nil => [],
}
void SERecord::initBITS(Symbols* symbols, CARD16 /*u0*/) {
	//        bits => [length(1:0..31): BitCount],   -- placed here to avoid
	cons.bits.length = symbols->file->getCARD32();
}
void SERecord::initFIXED_SEQUENCE(Symbols* /*symbols*/, CARD16 /*u0*/) {
	ERROR();
}
