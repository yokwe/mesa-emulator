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
// LTIndex.h
//

#ifndef LTINDEX_H__
#define LTINDEX_H__

#include "../util/Util.h"
#include "../mesa/MesaBasic.h"

#include "BCD.h"
#include "Symbols.h"


//LTIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO LTRecord;
//LTNull: LTIndex = LAST[LTIndex];
class LTIndex {
private:
	static constexpr const char* PREFIX = "lt";
	static const CARD16  LT_NULL = Symbols::LIMIT - 1;

	Symbols*   symbols;
	CARD16     index;

public:
	static void checkAll();
	static LTIndex* getNull();
	static LTIndex* getInstance(Symbols* symbols, CARD16 index);

	bool isNull() const {
		return index == LT_NULL;
	}
	QString toString() const;
	const LTRecord& getValue() const;

private:
	typedef Symbols::Key Key;
	static QMap<Key, LTIndex*> all;

	LTIndex(Symbols* symbols_, CARD16 index_) : symbols(symbols_), index(index_) {
		Key key(symbols_, index);
		all[key] = this;
	}
};


//WordSequence: TYPE = ARRAY [0..0) OF WORD;
//
//LTRecord: TYPE = RECORD [
//  link(0:0..12): LTIndex,
//  datum(0:13..47): SELECT kind(0:13..15): * FROM
//    short => [value(1:0..15): WORD],
//    long => [
//      codeIndex(1:0..15): [0..LAST[CARDINAL]/2],
//      length(2:0..15): CARDINAL,
//      value(3): WordSequence]
//    ENDCASE];
class LTRecord {
private:
	const Symbols*  symbols;
	const CARD16    index;

public:
	class Short {
	public:
		const CARD16 value;

		QString toString() const;

		Short(CARD16 value_) : value(value_) {}
	};

	class Long {
	public:
		const CARD16  codeIndex;
		const CARD16  length;
		const CARD16* value;

		QString toString() const;

		Long(CARD16 codeIndex_, CARD16 length_, CARD16* value_) : codeIndex(codeIndex_), length(length_), value(value_) {}
	};

	static LTRecord* getInstance(Symbols* symbols, CARD16 index);
	static LTRecord* find(Symbols* symbols, CARD16 index);

	enum class Tag {SHORT, LONG};
	static QString toString(Tag value);

	// const LTIndex* link
	//   NOTE
	//   According to document Chap. B.9.3 Literal Table, link field is not used after compilation.
	//   So remove declaration from LTRecord
	const Tag      tag;
	const void*    tagValue;

	const Short& getShort() const;
	const Long&  getLong() const;

	QString toString() const;

private:
	typedef Symbols::Key Key;
	static QMap<Key, LTRecord*> all;

	LTRecord(Symbols* symbols_, CARD16 index_, Tag tag_, void* tagValue_) :
		symbols(symbols_), index(index_), tag(tag_), tagValue(tagValue_) {
		Key key(symbols, index);
		all[key] = this;

		LTIndex::getInstance(symbols_, index_);
	}
};


//LitRecord: TYPE = RECORD [
//  SELECT litTag(0:0..0): * FROM
//    word => [index(0:1..13): LTIndex],
//    string => [index(0:1..13): STIndex]
//    ENDCASE];
class LitRecord {
public:
	enum class Tag {WORD, STRING};
	static QString toString(Tag value);

	class Word {
	public:
		const LTIndex* index;

		QString toString() const;

		Word(LTIndex* index_) : index(index_) {}
	};

	class String {
	public:
		const CARD16 index; // STIndex*

		QString toString() const;

		String(CARD16 index_) : index(index_) {}
	};

	static LitRecord* getInstance(Symbols* symbols, CARD16 u0);

	const Tag   tag;
	const void* tagValue;

	const Word&   getWord()   const;
	const String& getString() const;

	QString toString() const;

	LitRecord(Tag tag_, void* tagValue_) : tag(tag_), tagValue(tagValue_) {}
};

#endif
