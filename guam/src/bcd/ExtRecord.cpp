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
// ExtRecord.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("extrecord");

#include "ExtRecord.h"

#include "Symbols.h"
#include "SERecord.h"


QList<ExtIndex*> ExtIndex::all;

ExtIndex::ExtIndex(Symbols* symbols_, CARD16 index_) : symbols(symbols_), index(index_), value(0) {
	all.append(this);
}

QString ExtIndex::toString() {
	if (index == ExtIndex::Ext_NULL) return "#NULL#";
	if (value == 0) return QString("lt-%1").arg(index);

	return QString("lt-%1-%2").arg(index).arg(value ? value->toString() : "#EMPTY#");
}
void ExtIndex::resolve() {
	for(ExtIndex *p: all) {
		const CARD16 index = p->index;

		if (p->value) continue;
		if (p->index == Ext_NULL) continue;

		if (p->symbols == 0) {
			logger.error("p->symbols == null %d p = %s", index, p->toString().toLocal8Bit().constData());
			ERROR();
		}
		if (p->symbols->ext.contains(index)) {
//			logger.info("resolve ext %4d", index);
			p->value = p->symbols->ext[index];
		} else {
			logger.error("Unknown %d = %s", index, p->toString().toLocal8Bit().constData());
			ERROR();
		}
	}
}

//ExtRecord: PUBLIC TYPE = RECORD [
//  type (0:0..1): Symbols.ExtensionType[value..default],
//  sei (0:2..15): Symbols.ISEIndex,
//  tree (1:0..15): Tree.Link]
ExtRecord::ExtRecord(Symbols* symbols, CARD16 index_) {
	index = index_;

	CARD16 u0 = symbols->file->getCARD16();

	type = (ExtensionType)bitField(u0, 0, 1);
	sei = new SEIndex(symbols, bitField(2, 15));
	tree = symbols->file->getCARD16();

	// Sanity check
	switch(type) {
	case ExtensionType::VALUE:
	case ExtensionType::FORM:
	case ExtensionType::DEFAULT:
		break;
	case ExtensionType::NONE:
		ERROR();
		break;
	default:
		ERROR();
		break;
	}
}

QString ExtRecord::toString() {
	return QString("%1 %2 %3").arg(Symbols::toString(type)).arg(sei->toString()).arg(tree);
}
