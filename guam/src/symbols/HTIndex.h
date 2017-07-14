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
// HTIndex.h
//

#ifndef HTINDEX_H__
#define HTINDEX_H__

#include "../util/Util.h"
#include "../mesa/MesaBasic.h"

#include "BCD.h"
#include "Symbols.h"


//HTIndex: TYPE = CARDINAL [0..Limit/2);
//HTNull: HTIndex = FIRST[HTIndex];
class HTIndex {
private:
	static constexpr const char* PREFIX = "ht";
	static const CARD16  HT_NULL = 0;

	Symbols*  symbols;
	CARD16    index;

public:
	static HTIndex* getNull();
	static HTIndex* getInstance(Symbols* symbols_, CARD16 index_);

	bool isNull() const {
		return index == HT_NULL;
	}
	QString toString() const;

	const HTRecord& getValue() const;
private:
	HTIndex(Symbols* symbols_, CARD16 index_) : symbols(symbols_), index(index_) {}
};


//HTRecord: TYPE = RECORD [
//  anyInternal, anyPublic: BOOLEAN,
//  link: HTIndex,
//  ssIndex: CARDINAL];
class HTRecord {
private:
	const Symbols*  symbols;
	const CARD16    index;

public:
	static HTRecord* getInstance(Symbols* symbols, CARD16 index, CARD16 lastSSIndex);
	static HTRecord* find(Symbols* symbols, CARD16 index);

	const bool    anyInternal;
	const bool    anyPublic;
	const CARD16  link;
	const CARD16  ssIndex;
	const QString value;

	QString toString() const;

private:
	typedef Symbols::Key Key;
	static QMap<Key, HTRecord*> all;

	HTRecord(Symbols* symbols_, CARD16 index_, bool anyInternal_, bool anyPublic_, CARD16 link_, CARD16 ssIndex_, QString value_) :
		symbols(symbols_), index(index_), anyInternal(anyInternal_), anyPublic(anyPublic_), link(link_), ssIndex(ssIndex_), value(value_) {
		Key key(symbols, index);
		all[key] = this;
	}
};

#endif
