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
// MDRecord.h
//

#ifndef MDRECORD_H__
#define MDRECORD_H__

#include "../util/Util.h"
#include "../mesa/MesaBasic.h"

#include "Table.h"
#include "HTRecord.h"

class Symbols;
class MDRecord;

//MDIndex: TYPE = Base RELATIVE ORDERED POINTER [0..Limit) TO MDRecord;
//MDNull: MDIndex = LAST[MDIndex];
//OwnMdi: MDIndex = FIRST[MDIndex];
class MDIndex {
public:
	static const CARD16 MD_NULL = Table::LIMIT - 1;
	static const CARD16 OWM_MDI = 0;

	static void resolve();

	Symbols*  symbols;
	CARD16    index;
	MDRecord* value;

	MDIndex() : symbols(0), index(MD_NULL), value(0) {}
	MDIndex(const MDIndex& that) : symbols(that.symbols), index(that.index), value(that.value) {}
	MDIndex(Symbols& symbols_, CARD16 index_);

	QString toString();

private:
	static QList<MDIndex*> all;
};

//MDRecord: TYPE = RECORD [
//  stamp: TimeStamp.Stamp,
//  moduleId: HTIndex,		-- hash entry for module name
//  fileId: HTIndex,		-- hash entry for file name
//  shared: BOOLEAN,		-- overrides PRIVATE, etc.
//  exported: BOOLEAN,
//  ctx: IncludedCTXIndex,	-- context of copied entries
//  defaultImport: CTXIndex,	-- unnamed imported instance
//  file: FileIndex];		-- associated file
class MDRecord {
public:
	CARD16       index;

	VersionStamp stamp;
	HTIndex      moduleId;
	HTIndex      fileId;
	bool         shared;
	bool         exported;
//	CTXIndex     ctx;
//	CTXIndex     defaultImport;
	CARD16       ctx;
	CARD16       defaultImport;
	CARD16       file;

	MDRecord() : index(MDIndex::MD_NULL), stamp(), moduleId(), fileId(), shared(false), exported(false), ctx(0), defaultImport(0), file(0) {}
	MDRecord(const MDRecord& that) :
		index(that.index), stamp(that.stamp), moduleId(that.moduleId), fileId(that.fileId), shared(that.shared), exported(that.exported),
		ctx(that.ctx), defaultImport(that.defaultImport), file(0) {}
	MDRecord(Symbols& symbols, CARD16 index);

	bool isNull() {
		return index == MDIndex::MD_NULL;
	}
	QString toString();
};



#endif
