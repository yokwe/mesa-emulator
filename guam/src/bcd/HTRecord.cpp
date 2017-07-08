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
// HTRecord.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("htrecord");

#include "HTRecord.h"

#include "Symbols.h"

#include "CTXRecord.h"


QList<HTIndex*> HTIndex::all;

HTIndex::HTIndex(Symbols* symbols_, CARD16 index_) : symbols(symbols_), index(index_), value(0) {
	all.append(this);
}

QString HTIndex::toString() {
	if (value == 0) return QString("ht-%1").arg(index);
	return QString("ht-%1-[%2]").arg(index).arg(value ? value->toString().toLocal8Bit().constData() : "#EMPTY#");
}
QString HTIndex::getValue() {
	if (value == 0) return "#NULL#";
	return value->value;
}
void HTIndex::resolve() {
	for(HTIndex* p: all) {
		const CARD16 index = p->index;
		if (index == HT_NULL) continue;
		if (p->value) continue;

		if (p->symbols == 0) {
			logger.error("p->symbols == null  p = %s", p->toString().toLocal8Bit().constData());
			ERROR();
		}

		if (p->symbols->ht.contains(index)) {
			p->value = p->symbols->ht[index];
//			logger.info("resolve ht %4d", index);
		} else {
			logger.error("Unknown index %d  p = %s", index, p->toString().toLocal8Bit().constData());
			ERROR();
		}
	}
}

HTRecord::HTRecord(Symbols* symbols, CARD16 index_, CARD16 lastSSIndex) {
	index = index_;
    // 0
	CARD16 word = symbols->file->getCARD16();
    anyInternal = bitField(word, 0);
    anyPublic   = bitField(word, 1);
    link        = bitField(word, 2, 15);
    // 1
    ssIndex     = symbols->file->getCARD16();
    // ss.substring(lastSSIndex, data.ssIndex);
    value       = symbols->ss.mid(lastSSIndex, ssIndex - lastSSIndex);
}

QString HTRecord::toString() {
	return QString("%1 %2%3 %4").arg(index, 4).arg(anyInternal ? "I" : " ").arg(anyPublic ? "P" : " ").arg(value);
}

