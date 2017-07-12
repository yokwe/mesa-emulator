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
// CTXRecord.h
//

#ifndef CTXRECORD_H__
#define CTXRECORD_H__

#include "../util/Util.h"
#include "../mesa/MesaBasic.h"

#include "BCD.h"

class Symbols;
class CTXRecord;
class MDIndex;
class SEIndex;

//CTXIndex: TYPE = Base RELATIVE ORDERED POINTER [0..3777B] TO CTXRecord;
// IncludedCTXIndex: TYPE = Base RELATIVE ORDERED POINTER [0..3777B] TO included CTXRecord;
//
//CTXNull: CTXIndex = FIRST[CTXIndex];
//  IncludedCTXNull: IncludedCTXIndex = LOOPHOLE[CTXNull];
//
//StandardContext: TYPE = CTXIndex[CTXNull+SIZE[simple CTXRecord]..CTXNull+6*SIZE[simple CTXRecord]];
class CTXIndex {
public:
	static const CARD16 CTX_NULL = 0;
	static void resolve();

	Symbols*   symbols;
	CARD16     index;
	CTXRecord* value;

	CTXIndex(Symbols* symbols, CARD16 index);

	QString toString();

	bool isNull() {
		return index == CTX_NULL;
	}
	bool equals(CTXIndex* that) {
		return this->index == that->index;
	}


private:
	static QList<CTXIndex*> all;
};


//CTXRecord: TYPE = RECORD [
//  mark(0:0..0), varUpdated(0:1..1): BOOLEAN,
//  seList(0:2..15): ISEIndex,
//  level(1:0..2): ContextLevel,
//  extension(1:3..47): SELECT ctxType(1:3..4): * FROM
//    simple => [ctxNew(1:5..15): CTXIndex], -- for DeSoto
//    included => [
//      chain(1:5..15): IncludedCTXIndex,
//      copied(2:0..1): Closure � none,
//      module(2:2..15): MDIndex,
//      map(3:0..10): CTXIndex,
//      closed(3:11..11), complete(3:12..12), restricted(3:13..13): BOOLEAN,
//      reset(3:14..15): BOOLEAN],
//    imported => [includeLink(1:5..15): IncludedCTXIndex],
//    nil => []
//    ENDCASE];
class CTXRecord {
public:
	//ContextLevel: TYPE = [0..7];
	//  lZ: ContextLevel = 0;	-- context level of non-frame records
	//  lG: ContextLevel = 1;	-- context level of global frame
	//  lL: ContextLevel = lG+1;	-- context level of outer procedures
	static const CARD16 L_Z = 0;
	static const CARD16 L_G = 1;
	static const CARD16 L_L = L_G + 1;

	enum class CtxType {
		SIMPLE, INCLUDED, IMPORTED, NIL,
	};
	static QString toString(CtxType value);

	//Closure: TYPE = {none, unit, rc, full};  -- completeness of copied contexts
	enum class Closure {
		NONE, UNIT, RC, FULL,
	};
	static QString toString(Closure value);


	CARD16   index;
	bool     mark;
	bool     varUpdated;
	SEIndex* seList;
	CARD16   level;
	CtxType  ctxType;
	union {
		struct {
			CTXIndex* ctxNew;
		} simple;
		struct {
			CTXIndex* chain;
			Closure   copied;
			MDIndex*  module;
			CTXIndex* map;
			bool      closed;
			bool      complete;
			bool      restricted;
			bool      reset;
		} included;
		struct {
			CTXIndex* includeLink;
		} imported;
	};

	CTXRecord(Symbols* symbols, CARD16 index);

	QString toString();
};


#endif
