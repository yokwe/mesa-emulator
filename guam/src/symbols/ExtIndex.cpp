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
// ExtIndex.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("etx");

#include "ExtIndex.h"
#include "BCDFile.h"

#include "SEIndex.h"

//
// ExtIndex
//

ExtIndex* ExtIndex::getNull() {
	static ExtIndex ret(0, ExtIndex::EXT_NULL);
	return &ret;
}
ExtIndex* ExtIndex::getInstance(Symbols* symbols, CARD16 index) {
	return new ExtIndex(symbols, index);
}
QString ExtIndex::toString() const {
	if (isNull()) return QString("%1-NULL").arg(PREFIX);
	return QString("%1-%2").arg(PREFIX).arg(index);
}
const ExtRecord& ExtIndex::getValue() const {
	ExtRecord* ret = ExtRecord::find(symbols, index);
	if (ret == 0) {
		logger.fatal("Cannot find  symbols = %p  index = %d", symbols, index);
		ERROR();
	}
	return *ret;
}


//
// ExtRecord
//

//ExtRecord: PUBLIC TYPE = RECORD [
//  type (0:0..1): Symbols.ExtensionType[value..default],
//  sei (0:2..15): Symbols.ISEIndex,
//  tree (1:0..15): Tree.Link]

QMap<ExtRecord::Key, ExtRecord*> ExtRecord::all;
ExtRecord* ExtRecord::find(Symbols* symbols, CARD16 index) {
	Key key(symbols, index);
	return all.value(key, 0);
}

ExtRecord* ExtRecord::getInstance(Symbols* symbols, CARD16 index) {
 	CARD16 u0 = symbols->file->getCARD16();
 	ExtensionType type = (ExtensionType)bitField(u0, 0, 1);
    SEIndex*      sei  = SEIndex::getInstance(symbols, bitField(u0, 2, 15));
    CARD16        tree = symbols->file->getCARD16();

    return new ExtRecord(symbols, index, type, sei, tree);
}

QString ExtRecord::toString() const {
	return QString("%1 %2 %3").arg(toString(type)).arg(sei->toString()).arg(tree);
}

