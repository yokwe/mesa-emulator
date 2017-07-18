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
// MDIndex.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("md");

#include "MDIndex.h"
#include "BCDFile.h"

#include "CTXIndex.h"
#include "HTIndex.h"

//
// MDIndex
//
QMap<MDIndex::Key, MDIndex*> MDIndex::all;
void MDIndex::checkAll() {
	for(MDIndex* e: all.values()) {
		if (e->isNull()) continue;
		MDRecord* value = MDRecord::find(e->symbols, e->index);
		if (value == 0) {
			logger.warn("Bogus %s", e->toString().toLocal8Bit().constData());
		}
	}
}
MDIndex* MDIndex::getNull() {
	return getInstance(0, MD_NULL);
}
MDIndex* MDIndex::getInstance(Symbols* symbols_, CARD16 index_) {
	Key key(symbols_, index_);
	if (all.contains(key)) return all[key];

	return new MDIndex(symbols_, index_);
}
QString MDIndex::toString() const {
	if (isNull()) return QString("%1-NULL").arg(PREFIX);
	return QString("%1-%2").arg(PREFIX).arg(index);
}
const MDRecord& MDIndex::getValue() const {
	MDRecord* ret = MDRecord::find(symbols, index);
	if (ret == 0) {
		logger.fatal("Cannot find  symbols = %p  index = %d", symbols, index);
		ERROR();
	}
	return *ret;
}


//
// MDRecord
//
QMap<MDRecord::Key, MDRecord*> MDRecord::all;
MDRecord* MDRecord::find(Symbols* symbols, CARD16 index) {
	Key key(symbols, index);
	return all.value(key, 0);
}
MDRecord* MDRecord::getInstance(Symbols* symbols, CARD16 index) {
	Stamp*   stamp    = Stamp::getInstance(symbols->bcd);
	HTIndex* moduleId = HTIndex::getInstance(symbols, symbols->file->getCARD16());

	CARD16   word     = symbols->file->getCARD16();
	HTIndex* fileId   = HTIndex::getInstance(symbols, bitField(word, 0, 12));
    bool     shared   = bitField(word, 13);
    bool     exported = bitField(word, 14, 15);

    CTXIndex* ctx           = CTXIndex::getInstance(symbols, symbols->file->getCARD16());
    CTXIndex* defaultImport = CTXIndex::getInstance(symbols, symbols->file->getCARD16());

    CARD16 file = symbols->file->getCARD16();

    return new MDRecord(symbols, index, stamp, moduleId, fileId, shared, exported, ctx, defaultImport, file);
}
QString MDRecord::toString() const {
//	return QString("%1 %2 %3 %4 %5 %6 %7 %8").
//			arg(index, 4).arg(stamp->toString()).arg(moduleId->getValue()).arg(fileId->getValue()).
//			arg(shared ? "S" : " ").arg(exported ? "E" : " ").arg(ctx->toString()).arg(defaultImport->toString());
	return QString("%1 %2 %3 %4 %5 %6 %7 %8").
			arg(index, 4).arg(stamp->toString()).arg(moduleId->getValue().value).arg(fileId->getValue().value).
			arg(shared ? "S" : " ").arg(exported ? "E" : " ").arg(ctx->toString()).arg(defaultImport->toString());
}

