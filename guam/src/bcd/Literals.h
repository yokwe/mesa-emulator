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
// Literals.h
//

#ifndef LITERALS_H__
#define LITERALS_H__

#include "../util/Util.h"
#include "../mesa/MesaBasic.h"

#include "Table.h"
#include "HTRecord.h"

class Symbols;
class LTRecord;
class LTIndex;

//LitRecord: TYPE = RECORD [
//  SELECT litTag(0:0..0): * FROM
//    word => [index(0:1..13): LTIndex],
//    string => [index(0:1..13): STIndex]
//    ENDCASE];
class LitRecord {
public:
	enum class LitTag {WORD, STRING};
	static QString toString(LitTag value);

	LitTag litTag;
	union {
		struct {
			LTIndex* index;
		} word;
		struct {
//			STIndex* index;
			CARD16 index;
		} string;
	};

	LitRecord(Symbols* symbols, CARD16 u0);

	QString toString();
};

//LTIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO LTRecord;
//LTNull: LTIndex = LAST[LTIndex];
class LTIndex {
public:
	static const CARD16 LT_NULL = Table::LIMIT - 1;

	static void resolve();

	Symbols*  symbols;
	CARD16    index;
	LTRecord* value;

	LTIndex(Symbols* symbols_, CARD16 index_);

	QString toString();

private:
	static QList<LTIndex*> all;
};

//
//-- literal constants
//
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
//

class LTRecord {
public:
	enum class Kind {
		SHORT, LONG
	};
	CARD16       index;
	static QString toString(Kind value);

	// LTIndex* link
	//   NOTE
	//   According to document Chap. B.9.3 Literal Table, link field is not used after compilation.
	//   So remove declaration from LTRecord

	Kind kind;
	union {
		struct {
			CARD16 value;
		} short_;
		struct {
			CARD16 codeIndex;
			CARD16 length;
			CARD16* value;
		} long_;
	};

	LTRecord(Symbols* symbols, CARD16 index);

	bool isNull() {
		return index == LTIndex::LT_NULL;
	}
	QString toString();
};

#endif
