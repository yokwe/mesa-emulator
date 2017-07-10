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
// LTRecord.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("ltrecord");

#include "Literals.h"

#include "Symbols.h"


QList<LTIndex*> LTIndex::all;

LTIndex::LTIndex(Symbols* symbols_, CARD16 index_) : symbols(symbols_), index(index_), value(0) {
	all.append(this);
}

QString LTIndex::toString() {
	if (index == LTIndex::LT_NULL) return "#NULL#";
	if (value == 0) return QString("lt-%1").arg(index);

	return QString("lt-%1-%2").arg(index).arg(value ? value->toString() : "#EMPTY#");
}
void LTIndex::resolve() {
	for(LTIndex *p: all) {
		const CARD16 index = p->index;

		if (p->value) continue;
		if (p->index == LT_NULL) continue;

		if (p->symbols == 0) {
			logger.error("p->symbols == null %d p = %s", index, p->toString().toLocal8Bit().constData());
			ERROR();
		}
		if (p->symbols->lt.contains(index)) {
//			logger.info("resolve md %4d", index);
			p->value = p->symbols->lt[index];
		} else {
			logger.error("Unknown %d = %s", index, p->toString().toLocal8Bit().constData());
			ERROR();
		}
	}
}

QString LTRecord::toString(Kind value) {
	TO_STRING_PROLOGUE(Kind)

	MAP_ENTRY(SHORT)
	MAP_ENTRY(LONG)

	TO_STRING_EPILOGUE
}


//LTRecord: TYPE = RECORD [
//  link(0:0..12): LTIndex,
//  datum(0:13..47): SELECT kind(0:13..15): * FROM
//    short => [value(1:0..15): WORD],
//    long => [
//      codeIndex(1:0..15): [0..LAST[CARDINAL]/2],
//      length(2:0..15): CARDINAL,
//      value(3): WordSequence]
//    ENDCASE];
LTRecord::LTRecord(Symbols* symbols, CARD16 index_) {
	index = index_;

	CARD16 u0 = symbols->file->getCARD16();
	kind = (Kind)bitField(u0, 13, 15);
	switch(kind) {
	case Kind::SHORT:
		short_.value = symbols->file->getCARD16();
		break;
	case Kind::LONG:
		long_.codeIndex = symbols->file->getCARD16();
		long_.length = symbols->file->getCARD16();
		long_.value = new CARD16[long_.length];
		for(CARD16 i = 0; i < long_.length; i++) {
			long_.value[i] = symbols->file->getCARD16();
		}
		break;
	default:
		ERROR();
		break;
	}
}

QString LTRecord::toString() {
	switch(kind) {
	case Kind::SHORT:
		return QString("%1 %2 %3").arg(index).arg(toString(kind)).arg(short_.value);
	case Kind::LONG:
	{
		QString values;
		for(CARD16 i = 0; i < long_.length; i++) {
			values.append(QString((i == 0) ? "%1" : " %1").arg(long_.value[i]));
		}
		return QString("%1 %2 %3 (%4)[%5]").arg(index).arg(toString(kind)).arg(long_.codeIndex).arg(long_.length).arg(values);
	}
		break;
	default:
		ERROR();
		return "???";
	}
}
