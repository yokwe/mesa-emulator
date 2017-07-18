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
// CTXIndex.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("ctx");

#include "CTXIndex.h"
#include "BCDFile.h"

#include "MDIndex.h"
#include "SEIndex.h"

//
// CTXIndex
//
QMap<CTXIndex::Key, CTXIndex*> CTXIndex::all;
void CTXIndex::checkAll() {
	for(CTXIndex* e: all.values()) {
		if (e->isNull()) continue;
		CTXRecord* value = CTXRecord::find(e->symbols, e->index);
		if (value == 0) {
			logger.warn("Bogus %s", e->toString().toLocal8Bit().constData());
		}
	}
}
CTXIndex* CTXIndex::getNull() {
	return getInstance(0, CTX_NULL);
}
CTXIndex* CTXIndex::getInstance(Symbols* symbols, CARD16 index) {
	Key key(symbols, index);
	if (all.contains(key)) return all[key];

	return new CTXIndex(symbols, index);
}
QString CTXIndex::toString() const {
	if (isNull()) return QString("%1-NULL").arg(PREFIX);
	return QString("%1-%2").arg(PREFIX).arg(index);
}
const CTXRecord& CTXIndex::getValue() const {
	CTXRecord* ret = CTXRecord::find(symbols, index);
	if (ret == 0) {
		logger.fatal("Cannot find  symbols = %p  index = %d", symbols, index);
		ERROR();
	}
	return *ret;
}

//FirstCtxSe: PROC [h: Handle, ctx: CTXIndex] RETURNS [ISEIndex] = {
//  RETURN [IF ctx = CTXNull THEN ISENull ELSE h.ctxb[ctx].seList]};
const SEIndex* CTXIndex::firstCtxSe() const {
    return isNull() ? SEIndex::getNull() : getValue().seList;
}



//
// CTXRecord
//

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

QMap<CTXRecord::Key, CTXRecord*> CTXRecord::all;
CTXRecord* CTXRecord::find(Symbols* symbols, CARD16 index) {
	Key key(symbols, index);
	return all.value(key, 0);
}

CTXRecord* CTXRecord::getInstance(Symbols* symbols, CARD16 index) {
 	CARD16 u0 = symbols->file->getCARD16();
    SEIndex* seList = SEIndex::getInstance(symbols, bitField(u0, 2, 15));

 	CARD16 u1 = symbols->file->getCARD16();
 	CARD16 level = bitField(u1, 0, 2);

 	Tag tag = (Tag)bitField(u1, 3, 4);
 	void* tagValue = 0;
 	switch(tag) {
 	case Tag::SIMPLE: {
 		CTXIndex* ctxNew = CTXIndex::getInstance(symbols, bitField(u1, 5, 15));

 		tagValue = new Simple(ctxNew);
 	}
 		break;
 	case Tag::INCLUDED: {
 		CTXIndex* chain = CTXIndex::getInstance(symbols, bitField(u1, 5, 15));

 		CARD16 u2 = symbols->file->getCARD16();
 		Closure copied = (Closure)bitField(u2, 0, 1);
 		MDIndex* module = MDIndex::getInstance(symbols, bitField(u2, 2, 15));

 		CARD16 u3 = symbols->file->getCARD16();
 		CTXIndex* map = CTXIndex::getInstance(symbols, bitField(u3, 0, 10));
 		bool closed = bitField(u3, 11);
 		bool complete = bitField(u3, 12);
 		bool restricted = bitField(u3, 13);
 		bool reset = bitField(u3, 14, 15);

 		tagValue = new Included(chain, copied, module, map, closed, complete, restricted, reset);
 	}
 		break;
 	case Tag::IMPORTED: {
 		CTXIndex* includeLink = CTXIndex::getInstance(symbols, bitField(u1, 5, 15));

 		tagValue = new Imported(includeLink);
 	}
 		break;
 	case Tag::NIL:
 		tagValue = 0;
 		break;
 	default:
 		ERROR();
 	}

    return new CTXRecord(symbols, index, seList, level, tag, tagValue);
}
const CTXRecord::Simple&   CTXRecord::getSimple() const {
	if (tag != Tag::SIMPLE) ERROR();
	if (tagValue == 0) ERROR();
	Simple *ret = (Simple*)tagValue;
	return *ret;
}
const CTXRecord::Included& CTXRecord::getIncluded() const {
	if (tag != Tag::INCLUDED) ERROR();
	if (tagValue == 0) ERROR();
	Included *ret = (Included*)tagValue;
	return *ret;
}
const CTXRecord::Imported& CTXRecord::getImported() const {
	if (tag != Tag::IMPORTED) ERROR();
	if (tagValue == 0) ERROR();
	Imported *ret = (Imported*)tagValue;
	return *ret;
}


QString CTXRecord::toString(Tag value) {
	TO_STRING_PROLOGUE(Tag)

	MAP_ENTRY(SIMPLE)
	MAP_ENTRY(INCLUDED)
	MAP_ENTRY(IMPORTED)
	MAP_ENTRY(NIL)

	TO_STRING_EPILOGUE
}

QString CTXRecord::Simple::toString() const {
	return QString("%1").arg(this->ctxNew->toString());
}
QString CTXRecord::Included::toString() const {
	return QString("%1 %2 %3").arg(Symbols::toString(copied)).arg(module->toString()).arg(map->toString());
}
QString CTXRecord::Imported::toString() const {
	return QString("%1").arg(this->includeLink->toString());
}
QString CTXRecord::toString() const {
	switch(tag) {
	case Tag::SIMPLE:
		return QString("%1 %2 %3 [%4]").arg(seList->toString()).arg(level).arg(toString(tag)).arg(getSimple().toString());
		break;
	case Tag::INCLUDED:
		return QString("%1 %2 %3 [%4]").arg(seList->toString()).arg(level).arg(toString(tag)).arg(getIncluded().toString());
		break;
	case Tag::IMPORTED:
		return QString("%1 %2 %3 [%4]").arg(seList->toString()).arg(level).arg(toString(tag)).arg(getImported().toString());
	case Tag::NIL:
		return QString("%1 %2 %3").arg(seList->toString()).arg(level).arg(toString(tag));
	default:
		ERROR();
		return "???";
	}
}
