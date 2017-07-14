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
// CTXIndex.h
//

#ifndef CTXINDEX_H__
#define CTXINDEX_H__

#include "../util/Util.h"
#include "../mesa/MesaBasic.h"

#include "BCD.h"
#include "Symbols.h"


//CTXIndex: TYPE = Base RELATIVE ORDERED POINTER [0..3777B] TO CTXRecord;
//CTXNull: CTXIndex = FIRST[CTXIndex];
class CTXIndex {
private:
	static constexpr const char* PREFIX = "ctx";
	static const CARD16  CTX_NULL = 0;

	Symbols*   symbols;
	CARD16     index;

public:
	static CTXIndex* getNull();
	static CTXIndex* getInstance(Symbols* symbols_, CARD16 index_);

	bool isNull() const {
		return index == CTX_NULL;
	}
	QString toString() const;
	const CTXRecord& getValue() const;

private:
	CTXIndex(Symbols* symbols_, CARD16 index_) : symbols(symbols_), index(index_) {}
};

class CTXRecord {
private:
	const Symbols*  symbols;
	const CARD16    index;

public:
	typedef Symbols::Closure Closure;

	enum class Tag {SIMPLE, INCLUDED, IMPORTED, NIL};
	static QString toString(Tag value);

	class Simple {
	public:
		const CTXIndex* ctxNew;

		QString toString() const;
		Simple(CTXIndex* ctxNew_) : ctxNew(ctxNew_) {}
	};
	class Included {
	public:
		const CTXIndex* chain; // IncludedCTXIndex
		const Closure   copied;
		const MDIndex*  module;
		const CTXIndex* map;
		const bool      closed;
		const bool      complete;
		const bool      restricted;
		const bool      reset;

		QString toString() const;
		Included(CTXIndex* chain_, Closure copied_, MDIndex* module_, CTXIndex* map_, bool closed_, bool complete_, bool restricted_, bool reset_) :
			chain(chain_), copied(copied_), module(module_), map(map_), closed(closed_), complete(complete_), restricted(restricted_), reset(reset_) {}
	};
	class Imported {
	public:
		const CTXIndex* includeLink; // IncludedCTXIndex

		QString toString() const;
		Imported(CTXIndex* includeLink_) : includeLink(includeLink_) {}
	};

	static CTXRecord* getInstance(Symbols* symbols, CARD16 index);
	static CTXRecord* find(Symbols* symbols, CARD16 index);

//	bool   mark;
//	bool   varUpdated;
	const SEIndex* seList;
	const CARD16   level;
	const Tag      tag;
	const void*    tagValue;

	const Simple&   getSimple() const;
	const Included& getIncluded() const;
	const Imported& getImported() const;

	QString toString() const;

private:
	typedef Symbols::Key Key;
	static QMap<Key, CTXRecord*> all;

	CTXRecord(Symbols* symbols_, CARD16 index_, SEIndex* seList_, CARD16 level_, Tag tag_, void* tagValue_) :
		symbols(symbols_), index(index_), seList(seList_), level(level_), tag(tag_), tagValue(tagValue_) {
		Key key(symbols, index);
		all[key] = this;
	}
};

#endif