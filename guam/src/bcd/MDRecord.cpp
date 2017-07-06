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
// MDRecord.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("mdrecord");

#include "MDRecord.h"

#include "Symbols.h"


QList<MDIndex*> MDIndex::all;

MDIndex::MDIndex(Symbols& symbols_, CARD16 index_) : symbols(&symbols_), index(index_), value(0) {
	all.append(this);
}


QString MDIndex::toString() {
	if (index == MDIndex::MD_NULL) return "#NULL#";
	if (index == MDIndex::OWM_MDI) return "#OWN#";
	return QString("md-%1-%2").arg(index).arg(value ? value->toString() : "#EMPTY#");
}
void MDIndex::resolve() {
	logger.info("all %d", all.size());
	for(MDIndex *p: all) {
		const CARD16 index = p->index;

		if (p->value) continue;
		if (p->index == MD_NULL) continue;

		if (p->symbols == 0) {
			logger.error("p->symbols == null  p = %s", p->toString());
			ERROR();
		}
		if (p->symbols->ht.contains(index)) {
			MDRecord& record = p->symbols->md[index];
			p->value = &record;
		} else {
			logger.error("Unknown index  p = %s", p->toString());
			ERROR();
		}
	}
}

MDRecord::MDRecord(Symbols& symbols, CARD16 index_) {
	index = index_;

	stamp    = VersionStamp(symbols.bcd);

	{
		CARD16 index = symbols.bcd.file.getCARD16();
		logger.info("moduleId %4d", index);
		moduleId = HTIndex(symbols, index);
	}

	CARD16 word = symbols.bcd.file.getCARD16();
	{
		CARD16 index = bitField(word, 0, 12);
		logger.info("fileId   %4d", index);
		fileId   = HTIndex(symbols, index);
	}
    shared   = bitField(word, 13);
    exported = bitField(word, 14, 15);

    ctx           = symbols.bcd.file.getCARD16();
    defaultImport = symbols.bcd.file.getCARD16();
//    ctx           = CTIndex(symbols, symbols.bcd.file.getCARD16());
//    defaultImport = CTIndex(symbols, symbols.bcd.file.getCARD16());

    file = symbols.bcd.file.getCARD16();;
}

QString MDRecord::toString() {
	return QString("%1 %2 %3 %4 %5 %6 %7 %8 %9").
			arg(index, 4).arg(stamp.toString()).arg(moduleId.toString()).arg(fileId.toString()).
			arg(shared ? "S" : " ").arg(exported ? "E" : " ").arg(ctx).arg(defaultImport).arg(file);
}
