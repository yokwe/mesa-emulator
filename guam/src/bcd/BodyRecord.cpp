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
// BodyRecord.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("bodyrecord");

#include "BodyRecord.h"

#include "Symbols.h"

#include "CTXRecord.h"
#include "SERecord.h"


QList<BTIndex*> BTIndex::all;

BTIndex::BTIndex(Symbols* symbols_, CARD16 index_) : symbols(symbols_), index(index_), value(0) {
	all.append(this);
}

QString BTIndex::toString() {
	if (value == 0) return QString("bt-%1").arg(index);
	return QString("bt-%1-%2").arg(index).arg(value ? value->toString() : "#EMPTY#");
}
void BTIndex::resolve() {
//	logger.info("all %d", all.size());
	for(BTIndex *p: all) {
		const CARD16 index = p->index;

		if (p->value) continue;
		if (p->index == BT_NULL) continue;

		if (p->symbols == 0) {
			logger.error("p->symbols == null  p = %s", p->toString().toLocal8Bit().constData());
			ERROR();
		}
		if (p->symbols->body.contains(index)) {
//			logger.info("resolve body %4d", index);
			p->value = p->symbols->body[index];
		} else {
			logger.warn("Bogus index %d  p = %s", index, p->toString().toLocal8Bit().constData());
//			ERROR();
		}
	}
}

QString BodyRecord::toString(Which value) {
	TO_STRING_PROLOGUE(Which)

	MAP_ENTRY(SIGLING)
	MAP_ENTRY(PARENT)

	TO_STRING_EPILOGUE
}

QString BodyRecord::toString(Mark value) {
	TO_STRING_PROLOGUE(Mark)

	MAP_ENTRY(INTERNAL)
	MAP_ENTRY(EXTERNAL)

	TO_STRING_EPILOGUE
}

QString BodyRecord::toString(Kind value) {
	TO_STRING_PROLOGUE(Kind)

	MAP_ENTRY(CALLABLE)
	MAP_ENTRY(OTHER)

	TO_STRING_EPILOGUE
}

QString BodyRecord::toString(Nesting value) {
	TO_STRING_PROLOGUE(Nesting)

	MAP_ENTRY(OUTER)
	MAP_ENTRY(INNER)
	MAP_ENTRY(CATCH)

	TO_STRING_EPILOGUE
}


BodyRecord::BodyRecord(Symbols* symbols, CARD16 index_) {
	index = index_;

    // IMPORTANT
    // Length of BodyLink is 14 bit and right aligned.
    // So bit 1 is used for "which" and bit 2-15 is used for index
	CARD16 u0 = symbols->file->getCARD16();
	link.which = (Which)bitField(u0, 1);
	link.index = new BTIndex(symbols, bitField(u0, 2, 15));

	firstSon    = new BTIndex(symbols, symbols->file->getCARD16());
	type        = new SEIndex(symbols, symbols->file->getCARD16());

	CARD16 u3 = symbols->file->getCARD16();
	localCtx    = new CTXIndex(symbols, bitField(u3, 0, 10));
	level       = bitField(u3, 11, 15);

	sourceIndex = symbols->file->getCARD16();

	CARD16 u5 = symbols->file->getCARD16();
	info.mark = (Mark)bitField(u5, 0);
	switch(info.mark) {
	case Mark::INTERNAL:
		info.internal.bodyTree  = bitField(u5, 1, 15);
		info.internal.thread    = symbols->file->getCARD16();
		info.internal.frameSize = symbols->file->getCARD16();
		break;
	case Mark::EXTERNAL:
		info.external.bytes       = bitField(u5, 1, 15);
		info.external.startIndex  = symbols->file->getCARD16();
		info.external.indexLength = symbols->file->getCARD16();
		break;
	default:
		ERROR();
		break;
	}

	CARD16 u8 = symbols->file->getCARD16();
	kind = (Kind)bitField(u8, 0);
	switch(kind) {
	case Kind::CALLABLE:
	{
		//      inline(8:1..1): BOOLEAN,
		//      id(8:2..15): ISEIndex,
		//      ioType(9:0..13): CSEIndex,
		//      monitored(9:14..14), noXfers(9:15..15), resident(10:0..0): BOOLEAN,
		//      entry(10:1..1), internal(10:2..2): BOOLEAN,
		//      entryIndex(10:3..10): [0..256),
		//      hints(10:11..15): RECORD [safe(0:0..0), argUpdated(0:1..1), nameSafe(0:2..2), needsFixup(0:3..3): BOOLEAN],
		callable.inLine = bitField(u8, 1, 1);
		callable.id = new SEIndex(symbols, bitField(u8, 2, 15));

		CARD16 u9 = symbols->file->getCARD16();
		callable.ioType    = new SEIndex(symbols, bitField(u9, 0, 13));
		callable.monitored = bitField(u9, 14);
		callable.noXfers   = bitField(u9, 15);

		CARD16 u10 = symbols->file->getCARD16();
		callable.resident         = bitField(u10, 0);
		callable.entry            = bitField(u10, 1);
		callable.internal         = bitField(u10, 2);
		callable.entryIndex       = bitField(u10, 3, 10);
		callable.hints.safe       = bitField(u10, 11);
		callable.hints.argUpdated = bitField(u10, 12);
		callable.hints.nameSafe   = bitField(u10, 13);
		callable.hints.needsFixup = bitField(u10, 14);

		CARD16 u11 = symbols->file->getCARD16();
		callable.nesting = (Nesting)bitField(u11, 0, 1);
		switch(callable.nesting) {
		case Nesting::OUTER:
			break;
		case Nesting::INNER:
			callable.inner.frameOffset = bitField(u11, 2, 15);
			break;
		case Nesting::CATCH:
			callable.catch_.index = bitField(u11, 2, 15);
			break;
		default:
			ERROR();
			break;
		}
	}
		break;
	case Kind::OTHER:
		other.relOffset = bitField(u8, 1, 15);
		break;
	default:
		ERROR();
		break;
	}
}

QString BodyRecord::toString() {
	return QString("body-%1").arg(index);
}
