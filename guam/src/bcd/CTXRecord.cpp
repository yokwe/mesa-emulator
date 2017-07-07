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
// CTXRecord.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("ctxrecord");

#include "CTXRecord.h"

#include "Symbols.h"


QList<CTXIndex*> CTXIndex::all;

CTXIndex::CTXIndex(Symbols* symbols_, CARD16 index_) : symbols(symbols_), index(index_), value(0) {
	all.append(this);
}

QString CTXIndex::toString() {
	if (index == CTXIndex::CTX_NULL) return "#NULL#";
	if (value == 0) return QString("ctx-%1").arg(index);
	return QString("ctx-%1-%2").arg(index).arg(value ? value->toString() : "#EMPTY#");
}
void CTXIndex::resolve() {
	logger.info("all %d", all.size());
	for(CTXIndex *p: all) {
		const CARD16 index = p->index;

		if (p->value) continue;
		if (p->index == CTX_NULL) continue;

		if (p->symbols == 0) {
			logger.error("p->symbols == null  p = %s", p->toString().toLocal8Bit().constData());
			ERROR();
		}
		if (p->symbols->ctx.contains(index)) {
			logger.info("resolve ctx %4d", index);
			p->value = p->symbols->ctx[index];
		} else {
			logger.error("Unknown index %d  p = %s", index, p->toString().toLocal8Bit().constData());
			ERROR();
		}
	}
}

QString Symbols::toString(Closure value) {
	TO_STRING_PROLOGUE(Closure)

	MAP_ENTRY(NONE)
	MAP_ENTRY(UNIT)
	MAP_ENTRY(RC)
	MAP_ENTRY(FULL)

	TO_STRING_EPILOGUE
}

QString CTXRecord::toString(CtxType value) {
	TO_STRING_PROLOGUE(CtxType)

	MAP_ENTRY(SIMPLE)
	MAP_ENTRY(INCLUDED)
	MAP_ENTRY(IMPORTED)
	MAP_ENTRY(NIL)

	TO_STRING_EPILOGUE
}


//CTXRecord: TYPE = RECORD [
//  mark(0:0..0), varUpdated(0:1..1): BOOLEAN,
//  seList(0:2..15): ISEIndex,
//  level(1:0..2): ContextLevel,
//  extension(1:3..47): SELECT ctxType(1:3..4): * FROM
//    simple => [ctxNew(1:5..15): CTXIndex], -- for DeSoto
//    included => [
//      chain(1:5..15): IncludedCTXIndex,
//      copied(2:0..1): Closure � none,
//      module(2:2..15): MDIndex,
//      map(3:0..10): CTXIndex,
//      closed(3:11..11), complete(3:12..12), restricted(3:13..13): BOOLEAN,
//      reset(3:14..15): BOOLEAN],
//    imported => [includeLink(1:5..15): IncludedCTXIndex],
//    nil => []
//    ENDCASE];
CTXRecord::CTXRecord(Symbols* symbols, CARD16 index_) {
	index = index_;

	CARD16 u0 = symbols->file->getCARD16();
	mark       = bitField(u0, 0);
	varUpdated = bitField(u0, 1);
	seList     = bitField(u0, 2, 15);

	CARD16 u1 = symbols->file->getCARD16();
	level   = bitField(u1, 0, 2);
	ctxType = (CtxType)bitField(u1, 3, 4);

	switch(ctxType) {
	case CtxType::SIMPLE:
		simple.ctxNew = new CTXIndex(symbols, bitField(u1, 5, 15));
		break;
	case CtxType::INCLUDED:
		included.chain = new CTXIndex(symbols, bitField(u1, 5, 15));
		{
			CARD16 u2 = symbols->file->getCARD16();
			CARD16 u3 = symbols->file->getCARD16();
			included.copied     = (Closure)bitField(u2, 0, 1);
			included.module     = new MDIndex(symbols, bitField(u2, 2, 15));
			included.map        = new CTXIndex(symbols, bitField(u3, 0, 10));
			included.closed     = bitField(u3, 11);
			included.complete   = bitField(u3, 12);
			included.restricted = bitField(u3, 13);
			included.reset      = bitField(u3, 14, 15);
		}
		break;
	case CtxType::IMPORTED:
		imported.includeLink = new CTXIndex(symbols, bitField(u1, 5, 15));
		break;
	case CtxType::NIL:
		break;
	default:
		ERROR();
	}

}

QString CTXRecord::toString() {
	if (index == CTXIndex::CTX_NULL) return "#NULL#";

	QString header = QString("%1 %2 %3 %4").arg(index, 4).arg(seList, 4).arg(level).arg(toString(ctxType));

	switch(ctxType) {
	case CtxType::SIMPLE:
		return QString("%1 %2").arg(header).arg(simple.ctxNew->toString());
	case CtxType::INCLUDED:
		return QString("%1 %2 %3 %4 %5").arg(header).
				arg(included.chain->toString()).
				arg(::toString(included.copied)).
				arg(included.module->toString()).
				arg(included.map->toString());
	case CtxType::IMPORTED:
		return QString("%1 %2").arg(header).
				arg(imported.includeLink->toString());
	case CtxType::NIL:
		return header;
	default:
		ERROR();
	}
}
