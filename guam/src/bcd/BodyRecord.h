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
// BodyRecord.h
//

#ifndef BODYRECORD_H__
#define BODYRECORD_H__

#include "../util/Util.h"
#include "../mesa/MesaBasic.h"

#include "BCD.h"
#include "Table.h"

class Symbols;
class BodyRecord;
class CTXIndex;
class SEIndex;

//BTIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO BodyRecord;
//  CBTIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO Callable BodyRecord;
//    ICBTIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO Inner Callable BodyRecord;
//    OCBTIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO Outer Callable BodyRecord;
//    CCBTIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO Catch Callable BodyRecord;
//BTNull: BTIndex = LAST[BTIndex];
//  CBTNull: CBTIndex = LOOPHOLE[BTNull];
//  CCBTNull: CCBTIndex = LOOPHOLE[BTNull];
//
//RootBti: CBTIndex = FIRST[CBTIndex];
class BTIndex {
public:
	static const CARD16 BT_ROOT = 0;
	static const CARD16 BT_NULL = Table::LIMIT - 1;

	static void resolve();

	Symbols*    symbols;
	CARD16      index;
	BodyRecord* value;

	BTIndex(Symbols* symbols, CARD16 index);

	QString toString();

private:
	static QList<BTIndex*> all;
};


//-- body table declarations
//
//BodyLink: TYPE = RECORD [which(0:0..0): {sibling(0), parent(1)}, index(0:1..14): BTIndex];
//
//BodyInfo: TYPE = RECORD [
//  SELECT mark(0:0..0): * FROM
//    Internal => [
//      bodyTree(0:1..15): Base RELATIVE POINTER [0..Limit),
//        --Tree.Index--
//      thread(1:0..15): Base RELATIVE POINTER [0..Limit),
//        --Tree.Index / LitDefs.STIndex--
//      frameSize(2:0..15): [0..PrincOps.MaxFrameSize]],
//    External => [
//      bytes(0:1..15): [0..LAST[CARDINAL]/2],
//      startIndex(1:0..15), indexLength(2:0..15): CARDINAL]
//    ENDCASE];
//
//CatchIndex: TYPE = NATURAL;
//BodyRecord: TYPE = RECORD [
//  link(0:0..15): BodyLink,
//  firstSon(1:0..15): BTIndex,
//  type(2:0..15): RecordSEIndex,
//  localCtx(3:0..10): CTXIndex,
//  level(3:11..15): ContextLevel,
//  sourceIndex(4:0..15): CARDINAL,
//  info(5:0..47): BodyInfo,
//  extension(8:0..79): SELECT kind: * FROM
//    Callable => [
//      inline(8:1..1): BOOLEAN,
//      id(8:2..15): ISEIndex,
//      ioType(9:0..13): CSEIndex,
//      monitored(9:14..14), noXfers(9:15..15), resident(10:0..0): BOOLEAN,
//      entry(10:1..1), internal(10:2..2): BOOLEAN,
//      entryIndex(10:3..10): [0..256),
//      hints(10:11..15): RECORD [safe(0:0..0), argUpdated(0:1..1), nameSafe(0:2..2), needsFixup(0:3..3): BOOLEAN],
//      closure(11:0..31): SELECT nesting(11:0..1): * FROM
//        Outer => [],
//        Inner => [frameOffset(11:2..15): [0..PrincOps.MaxFrameSize)],
//        Catch => [index(12:0..15): CatchIndex]
//        ENDCASE],
//    Other => [relOffset(8:1..15): [0..LAST[CARDINAL]/2]]
//    ENDCASE];
class BodyRecord {
public:
	enum class Which {SIGLING, PARENT};
	static QString toString(Which value);

	enum class Mark {INTERNAL, EXTERNAL};
	static QString toString(Mark value);

	enum class Kind {CALLABLE, OTHER};
	static QString toString(Kind value);

	enum class Nesting {OUTER, INNER, CATCH};
	static QString toString(Nesting value);

	CARD16 index;
	struct {
		Which    which;
		BTIndex* index;
	} link;
	BTIndex*  firstSon;
	SEIndex*  type;
	CTXIndex* localCtx;
	CARD16    level;
	CARD16    sourceIndex;
	struct {
		Mark mark;
		union {
			struct {
				CARD16 bodyTree; // Tree.Index
				CARD16 thread;   // Tree.Index / LitDefs.STIndex
				CARD16 frameSize;
			} internal;
			struct {
				CARD16 bytes;
				CARD16 startIndex;
				CARD16 indexLength;
			} external;
		};
	} info;
	Kind kind;
	union {
		struct {
			bool     inLine;
			SEIndex* id;
			SEIndex* ioType;
			bool     monitored;
			bool     noXfers;
			bool     resident;
			bool     entry;
			bool     internal;
			CARD16   entryIndex;
			struct {
				bool safe;
				bool argUpdated;
				bool nameSafe;
				bool needsFixup;
			} hints;
			Nesting nesting;
			union {
				struct {
					CARD16 frameOffset;
				} inner;
				struct {
					CARD16 index;
				} catch_;
			};
		} callable;
		struct {
			CARD16 relOffset;
		} other;
	};
	BodyRecord(Symbols* symbols, CARD16 index);

	QString toString();
};


#endif
