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
// BTXIndex.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("bt");

#include "BTIndex.h"
#include "BCDFile.h"

#include "CTXIndex.h"
#include "SEIndex.h"


//
// BTIndex
//
QMap<BTIndex::Key, BTIndex*> BTIndex::all;
BTIndex::BTIndex(Symbols* symbols_, CARD16 index_) : symbols(symbols_), index(index_) {
	Key key(symbols_, index_);
	all[key] = this;
}
void BTIndex::checkAll() {
	for(BTIndex* e: all.values()) {
		if (e->isNull()) continue;
		BTRecord* value = BTRecord::find(e->symbols, e->index);
		if (value == 0) {
			logger.warn("Bogus %s", e->toString().toLocal8Bit().constData());
		}
	}
}
BTIndex* BTIndex::getNull() {
	return getInstance(0, BT_NULL);
}
BTIndex* BTIndex::getInstance(Symbols* symbols_, CARD16 index_) {
	Key key(symbols_, index_);
	if (all.contains(key)) return all[key];

	return new BTIndex(symbols_, index_);
}
QString BTIndex::toString() const {
	if (isNull()) return QString("%1-NULL").arg(PREFIX);
	return QString("%1-%2").arg(PREFIX).arg(index);
}
const BTRecord& BTIndex::getValue() const {
	BTRecord* ret = BTRecord::find(symbols, index);
	if (ret == 0) {
		logger.fatal("Cannot find  symbols = %p  index = %d", symbols, index);
		ERROR();
	}
	return *ret;
}


//
// BTRecord
//
QMap<BTRecord::Key, BTRecord*> BTRecord::all;
BTRecord::BTRecord(Symbols* symbols_, CARD16 index_, BodyLink* link_, BTIndex* firstSon_,SEIndex* type_,
		CTXIndex* localCtx_, CARD16 level_, CARD16 sourceIndex_, BodyInfo* info_, Tag tag_, void* tagValue_) :
	symbols(symbols_), index(index_), link(link_), firstSon(firstSon_), type(type_),
	localCtx(localCtx_), level(level_), sourceIndex(sourceIndex_), info(info_), tag(tag_), tagValue(tagValue_) {
	Key key(symbols, index);
	all[key] = this;

	BTIndex::getInstance(symbols_, index);
}
BTRecord* BTRecord::find(Symbols* symbols, CARD16 index) {
	Key key(symbols, index);
	return all.value(key, 0);
}

BTRecord* BTRecord::getInstance(Symbols* symbols, CARD16 index) {
	BodyLink* link = BodyLink::getInstance(symbols);
	BTIndex*  firstSon = BTIndex::getInstance(symbols, symbols->file->getCARD16());
	SEIndex*  type     = SEIndex::getInstance(symbols, symbols->file->getCARD16());

	CARD16 u3 = symbols->file->getCARD16();
	CTXIndex* localCtx = CTXIndex::getInstance(symbols, bitField(u3, 0, 10));
	CARD16    level    = bitField(u3, 11, 15);

	CARD16    sourceIndex = symbols->file->getCARD16();
	BodyInfo* info        = BodyInfo::getInstance(symbols);

	CARD16 u8 = symbols->file->getCARD16();
	Tag   tag = (Tag)bitField(u8, 0);
	void* tagValue = 0;
	switch(tag) {
	case Tag::CALLABLE:
		tagValue = Callable::getInstance(symbols, u8);
		break;
	case Tag::OTHER:
		tagValue = Other::getInstance(symbols, u8);
		break;
	default:
		ERROR();
	}

	return new BTRecord(symbols, index, link, firstSon, type, localCtx, level, sourceIndex, info, tag, tagValue);
}
//      inline(8:1..1): BOOLEAN,
//      id(8:2..15): ISEIndex,
//      ioType(9:0..13): CSEIndex,
//      monitored(9:14..14), noXfers(9:15..15), resident(10:0..0): BOOLEAN,
//      entry(10:1..1), internal(10:2..2): BOOLEAN,
//      entryIndex(10:3..10): [0..256),
//      hints(10:11..15): RECORD [safe(0:0..0), argUpdated(0:1..1), nameSafe(0:2..2), needsFixup(0:3..3): BOOLEAN],
//      closure(11:0..31): SELECT nesting(11:0..1): * FROM
//        Outer => [],
//        Inner => [frameOffset(11:2..15): [0..PrincOps.MaxFrameSize)],
//        Catch => [index(12:0..15): CatchIndex]
//        ENDCASE],
BTRecord::Callable* BTRecord::Callable::getInstance(Symbols* symbols, CARD16 u8) {
	bool inline_ = bitField(u8, 1);
	SEIndex* id = SEIndex::getInstance(symbols, bitField(u8, 2, 15));

	CARD16 u9 = symbols->file->getCARD16();
	SEIndex* ioType    = SEIndex::getInstance(symbols, bitField(u9, 0, 13));
	bool     monitored = bitField(u9, 14);
	bool     noXfers   = bitField(u9, 15);

	CARD16 u10 = symbols->file->getCARD16();
	bool resident = bitField(u10, 0);
	bool entry = bitField(u10, 1);
	bool internal = bitField(u10, 2);
	CARD16 entryIndex = bitField(u10, 3, 10);

	CARD16 u11 = symbols->file->getCARD16();
	Tag tag = (Tag)bitField(u11, 0, 1);
	void* tagValue = 0;
	switch(tag) {
	case Tag::OUTER:
		tagValue = 0;
		break;
	case Tag::INNER:
		tagValue = new Inner(bitField(u11, 2, 15));
		break;
	case Tag::CATCH:
		tagValue = new Catch(symbols->file->getCARD16());
		break;
	default:
		ERROR();
		tagValue = 0;
		break;
	}

	return new Callable(inline_, id, ioType, monitored, noXfers, resident, entry, internal, entryIndex, tag, tagValue);
}
const BTRecord::Callable::Inner& BTRecord::Callable::getInner() const {
	if (tag != Tag::INNER) ERROR();
	if (tagValue == 0) ERROR();
	Inner* ret = (Inner*)tagValue;
	return *ret;
}
const BTRecord::Callable::Catch& BTRecord::Callable::getCatch() const {
	if (tag != Tag::INNER) ERROR();
	if (tagValue == 0) ERROR();
	Catch* ret = (Catch*)tagValue;
	return *ret;
}
QString BTRecord::Callable::toString(Tag value) {
	TO_STRING_PROLOGUE(Tag)

	MAP_ENTRY(OUTER)
	MAP_ENTRY(INNER)
	MAP_ENTRY(CATCH)

	TO_STRING_EPILOGUE
}
QString BTRecord::Callable::toString() const {
	switch(tag) {
	case Tag::OUTER:
		return QString("%1 %2 %3 %4").arg(id->toString()).arg(ioType->toString()).arg(entryIndex).arg(toString(tag));
	case Tag::INNER:
		return QString("%1 %2 %3 %4 %5").arg(id->toString()).arg(ioType->toString()).arg(entryIndex).arg(toString(tag)).arg(getInner().toString());
	case Tag::CATCH:
		return QString("%1 %2 %3 %4 %5").arg(id->toString()).arg(ioType->toString()).arg(entryIndex).arg(toString(tag)).arg(getCatch().toString());
	default:
		ERROR();
		return "???";
	}
}
QString BTRecord::Callable::Inner::toString() const {
	return QString("%1").arg(frameOffset);
}
QString BTRecord::Callable::Catch::toString() const {
	return QString("%1").arg(index);
}

//    Other => [relOffset(8:1..15): [0..LAST[CARDINAL]/2]]
BTRecord::Other* BTRecord::Other::getInstance(Symbols* /*symbols*/, CARD16 u8) {
	CARD16 realOffset = bitField(u8, 1, 15);

	return new Other(realOffset);
}
QString BTRecord::Other::toString() const {
	return QString("%1").arg(realOffset);
}
QString BTRecord::toString(Tag value) {
	TO_STRING_PROLOGUE(Tag)

	MAP_ENTRY(CALLABLE)
	MAP_ENTRY(OTHER)

	TO_STRING_EPILOGUE
}
const BTRecord::Callable& BTRecord::getCallable() const {
	if (tag != Tag::CALLABLE) ERROR();
	if (tagValue == 0) ERROR();
	Callable* ret = (Callable*)tagValue;
	return *ret;
}
const BTRecord::Other& BTRecord::getOther() const {
	if (tag != Tag::OTHER) ERROR();
	if (tagValue == 0) ERROR();
	Other* ret = (Other*)tagValue;
	return *ret;
}
QString BTRecord::toString() const {
	switch(tag) {
	case Tag::CALLABLE:
		return QString("%1 %2 %3 %4 %5 %6").arg(link->toString()).arg(type->toString()).arg(info->toString()).arg(level).arg(toString(tag)).arg(getCallable().toString());
	case Tag::OTHER:
		return QString("%1 %2 %3 %4 %5 %6").arg(link->toString()).arg(type->toString()).arg(info->toString()).arg(level).arg(toString(tag)).arg(getOther().toString());
	default:
		ERROR();
		return "???";
	}
}



//
//BodyLink: TYPE = RECORD [which(0:0..0): {sibling(0), parent(1)}, index(0:1..14): BTIndex];
//
BTRecord::BodyLink* BTRecord::BodyLink::getInstance(Symbols* symbols) {
	// IMPORTANT
	//   BodyLink: TYPE = RECORD [which(0:0..0): {sibling(0), parent(1)}, index(0:1..14): BTIndex];
	//   Size of BodyLink is 15 bits.
	//   So need to shift 1 bit before process u0.
	CARD16 u0 = symbols->file->getCARD16();
	u0 <<= 1;

	Which    which = (Which)bitField(u0, 0, 0);
	BTIndex* index = BTIndex::getInstance(symbols, bitField(u0, 1, 14));

	return new BodyLink(which, index);
}
QString BTRecord::BodyLink::toString() const {
	return QString("%1 %2").arg(toString(which)).arg(index->toString());
}
QString BTRecord::BodyLink::toString(Which value) {
	TO_STRING_PROLOGUE(Which)

	MAP_ENTRY(SIBLING)
	MAP_ENTRY(PARENT)

	TO_STRING_EPILOGUE
}


//BodyInfo: TYPE = RECORD [
//  SELECT mark(0:0..0): * FROM
//    Internal => [
//      bodyTree(0:1..15): Base RELATIVE POINTER [0..Limit),
//        --Tree.Index--
//      thread(1:0..15): Base RELATIVE POINTER [0..Limit),
//        --Tree.Index / LitDefs.STIndex--
//      frameSize(2:0..15): [0..PrincOps.MaxFrameSize]],
//    External => [
//      bytes(0:1..15): [0..LAST[CARDINAL]/2],
//      startIndex(1:0..15), indexLength(2:0..15): CARDINAL]
//    ENDCASE];
BTRecord::BodyInfo* BTRecord::BodyInfo::getInstance(Symbols* symbols) {
	CARD16 u0 = symbols->file->getCARD16();
	Tag tag = (Tag)bitField(u0, 0);
	void* tagValue;
	switch(tag) {
	case Tag::INTERNAL: {
		CARD16 bodyTree  = bitField(u0, 1, 15);
		CARD16 thread    = symbols->file->getCARD16();
		CARD16 frameSize = symbols->file->getCARD16();

		tagValue = new Internal(bodyTree, thread, frameSize);
	}
		break;
	case Tag::EXTERNAL: {
		CARD16 bytes       = bitField(u0, 1, 15);
		CARD16 startIndex  = symbols->file->getCARD16();
		CARD16 indexLength = symbols->file->getCARD16();

		tagValue = new External(bytes, startIndex, indexLength);
	}
		break;
	default:
		ERROR();
	}

	return new BodyInfo(tag, tagValue);
}
QString BTRecord::BodyInfo::toString(Tag value) {
	TO_STRING_PROLOGUE(Tag)

	MAP_ENTRY(INTERNAL)
	MAP_ENTRY(EXTERNAL)

	TO_STRING_EPILOGUE
}
QString BTRecord::BodyInfo::Internal::toString() const {
	return QString("%1 %2 %3").arg(bodyTree).arg(thread).arg(frameSize);
}
QString BTRecord::BodyInfo::External::toString() const {
	return QString("%1 %2 %3").arg(bytes).arg(startIndex).arg(indexLength);
}
const BTRecord::BodyInfo::Internal&   BTRecord::BodyInfo::getInternal() const {
	if (tag != Tag::INTERNAL) ERROR();
	if (tagValue == 0) ERROR();
	Internal* ret = (Internal*)tagValue;
	return *ret;
}
const BTRecord::BodyInfo::External&   BTRecord::BodyInfo::getExternal() const {
	if (tag != Tag::EXTERNAL) ERROR();
	if (tagValue == 0) ERROR();
	External* ret = (External*)tagValue;
	return *ret;
}
QString BTRecord::BodyInfo::toString() const {
	switch(tag) {
	case Tag::INTERNAL:
		return QString("[%1 %2]").arg(toString(tag)).arg(getInternal().toString());
	case Tag::EXTERNAL:
		return QString("[%1 %2]").arg(toString(tag)).arg(getExternal().toString());
	default:
		ERROR();
		return "???";
	}
}
