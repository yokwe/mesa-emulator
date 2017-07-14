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
// MDIndex.h
//

#ifndef MDINDEX_H__
#define MDINDEX_H__

#include "../util/Util.h"
#include "../mesa/MesaBasic.h"

#include "BCD.h"
#include "Symbols.h"


//MDIndex: TYPE = Base RELATIVE ORDERED POINTER [0..Limit) TO MDRecord;
//MDNull: MDIndex = LAST[MDIndex];
//OwnMdi: MDIndex = FIRST[MDIndex];
class MDIndex {
private:
	static constexpr const char* PREFIX = "md";
	static const CARD16  MD_NULL = Symbols::LIMIT - 1;
	static const CARD16  MD_OWN  = 0;

	Symbols*  symbols;
	CARD16    index;

public:
	static MDIndex* getNull();
	static MDIndex* getInstance(Symbols* symbols_, CARD16 index_);

	bool isNull() const {
		return index == MD_NULL;
	}
	QString toString() const;
	const MDRecord& getValue() const;

private:
	MDIndex(Symbols* symbols_, CARD16 index_) : symbols(symbols_), index(index_) {}
};


class MDRecord {
private:
	const Symbols*  symbols;
	const CARD16    index;

public:
	static MDRecord* getInstance(Symbols* symbols, CARD16 index);
	static MDRecord* find(Symbols* symbols, CARD16 index);

	const Stamp*     stamp;
	const HTIndex*   moduleId;
	const HTIndex*   fileId;
	const bool       shared;
	const bool       exported;
	const CTXIndex*  ctx;
	const CTXIndex*  defaultImport;
	const CARD16     file;

	QString toString() const;

private:
	typedef Symbols::Key Key;
	static QMap<Key, MDRecord*> all;

	MDRecord(Symbols* symbols_, CARD16 index_, Stamp* stamp_, HTIndex* moduleId_, HTIndex* fileId_,
			bool shared_, bool exported_, CTXIndex* ctx_, CTXIndex* defaultImport_, CARD16 file_) :
		symbols(symbols_), index(index_), stamp(stamp_), moduleId(moduleId_), fileId(fileId_),
		shared(shared_), exported(exported_), ctx(ctx_), defaultImport(defaultImport_), file(file_) {
		Key key(symbols, index);
		all[key] = this;
	}
};

#endif
