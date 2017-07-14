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
BTIndex* BTIndex::getNull() {
	static BTIndex ret(0, BTIndex::BT_NULL);
	return &ret;
}
BTIndex* BTIndex::getInstance(Symbols* symbols_, CARD16 index_) {
	return new BTIndex(symbols_, index_);
}
QString BTIndex::toString() const {
	if (isNull()) return QString("%1-NULL").arg(PREFIX);
	return QString("%1-%2").arg(PREFIX).arg(index);
}
//const BTRecord& BTIndex::getValue() const {
//	BTRecord* ret = BTRecord::find(symbols, index);
//	return *ret;
//}

BTRecord* BTRecord::find(Symbols* symbols, CARD16 index) {
	Key key(symbols, index);
	BTRecord* ret = all.value(key, 0);
	if (ret == 0) {
		logger.fatal("Cannot find  symbols = %p  index = %d", symbols, index);
		ERROR();
	}
	return ret;
}


BTRecord* BTRecord::getInstance(Symbols* symbols, CARD16 index) {
	BodyLink* bodyLink = BodyLink::getInstance(symbols);
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

	return new BTRecord(symbols, index, bodyLink, firstSon, type, localCtx, level, sourceIndex, info, tag, tagValue);
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
QString BTRecord::BodyInfo::toString() {
	switch(tag) {
	case Tag::INTERNAL:
		return QString("%1 %2").arg(toString(tag)).arg(getInternal().toString());
	case Tag::EXTERNAL:
		return QString("%1 %2").arg(toString(tag)).arg(getExternal().toString());
	default:
		ERROR();
		return "???";
	}
}
