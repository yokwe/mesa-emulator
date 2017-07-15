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
// HTIndex.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("ht");

#include "HTIndex.h"
#include "BCDFile.h"


//
// HTIndex
//
QList<HTIndex*> HTIndex::all;
void HTIndex::checkAll() {
	for(HTIndex* e: all) {
		if (e->isNull()) continue;
		HTRecord* value = HTRecord::find(e->symbols, e->index);
		if (value == 0) {
			logger.warn("Bogus %s", e->toString().toLocal8Bit().constData());
		}
	}
}

HTIndex* HTIndex::getNull() {
	static HTIndex ret(0, HTIndex::HT_NULL);
	return &ret;
}
HTIndex* HTIndex::getInstance(Symbols* symbols, CARD16 index) {
	return new HTIndex(symbols, index);
}
QString HTIndex::toString() const {
	if (isNull()) return QString("%1-NULL").arg(PREFIX);
	return QString("%1-%2").arg(PREFIX).arg(index);
}
const HTRecord& HTIndex::getValue() const {
	HTRecord* ret = HTRecord::find(symbols, index);
	if (ret == 0) {
		logger.fatal("Cannot find  symbols = %p  index = %d", symbols, index);
		ERROR();
	}
	return *ret;
}


//
// HTRecord
//
QMap<HTRecord::Key, HTRecord*> HTRecord::all;
HTRecord* HTRecord::find(Symbols* symbols, CARD16 index) {
	Key key(symbols, index);
	return all.value(key, 0);
}

HTRecord* HTRecord::getInstance(Symbols* symbols, CARD16 index, CARD16 lastSSIndex) {
    // 0
	CARD16 word = symbols->file->getCARD16();
    bool   anyInternal = bitField(word, 0);
    bool   anyPublic   = bitField(word, 1);
    CARD16 link        = bitField(word, 2, 15);
    // 1
    CARD16 ssIndex     = symbols->file->getCARD16();
    // ss.substring(lastSSIndex, data.ssIndex);
    QString value       = symbols->ss.mid(lastSSIndex, ssIndex - lastSSIndex);

    return new HTRecord(symbols, index, anyInternal, anyPublic, link, ssIndex, value);
}
QString HTRecord::toString() const {
	return QString("%1 %2%3 %4").arg(index, 4).arg(anyInternal ? "I" : " ").arg(anyPublic ? "P" : " ").arg(value);
}
