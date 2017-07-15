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
// BTIndex.h
//

#ifndef BTINDEX_H__
#define BTINDEX_H__

#include "../util/Util.h"
#include "../mesa/MesaBasic.h"

#include "BCD.h"
#include "Symbols.h"


//BTIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO BodyRecord;
//BTNull: BTIndex = LAST[BTIndex];
//RootBti: CBTIndex = FIRST[CBTIndex];
class BTIndex {
private:
	static constexpr const char* PREFIX = "bt";
	static const CARD16  BT_NULL = Symbols::LIMIT - 1;
	static const CARD16  BT_ROOT = 0;

	Symbols*   symbols;
	CARD16     index;

public:
	static void checkAll();
	static BTIndex* getNull();
	static BTIndex* getInstance(Symbols* symbols_, CARD16 index_);

	bool isNull() const {
		return index == BT_NULL;
	}
	QString toString() const;
	const BTRecord& getValue() const;

private:
	static QList<BTIndex*> all;
	BTIndex(Symbols* symbols_, CARD16 index_) : symbols(symbols_), index(index_) {
		all.append(this);
	}
};


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
class BTRecord {
private:
	const Symbols*  symbols;
	const CARD16    index;

public:
	class Callable {
	public:
		class Inner {
		public:
			const CARD16 frameOffset;

			QString toString() const;
			Inner(CARD16 frameOffset_) : frameOffset(frameOffset_) {}
		};

		class Catch {
		public:
			const CARD16 index;

			QString toString() const;
			Catch(CARD16 index_) : index(index_) {}
		};

		static Callable* getInstance(Symbols* symbols, CARD16 u0);

		enum class Tag {OUTER, INNER, CATCH};
		static QString toString(Tag value);

		const bool     inline_;
		const SEIndex* id;
		const SEIndex* ioType;
		const bool     monitored;
		const bool     noXfers;
		const bool     resident;
		const bool     entry;
		const bool     internal;
		const CARD16   entryIndex;
//		struct {
//			bool safe;
//			bool argUpdated;
//			bool nameSafe;
//			bool needsFixup;
//		} hints;

		const Tag   tag;
		const void* tagValue;

		const Inner&   getInner() const;
		const Catch&   getCatch() const;

		QString toString() const;
		Callable(bool inline__, SEIndex* id_, SEIndex* ioType_, bool monitored_, bool noXfers_,
				bool resident_, bool entry_, bool internal_, CARD16 entryIndex_, Tag tag_, void* tagValue_) :
			inline_(inline__), id(id_), ioType(ioType_), monitored(monitored_), noXfers(noXfers_),
			resident(resident_), entry(entry_), internal(internal_), entryIndex(entryIndex_), tag(tag_), tagValue(tagValue_) {}
	};
	class Other {
	public:
		static Other* getInstance(Symbols* symbols, CARD16 u0);

		const CARD16 realOffset;

		QString toString() const;
		Other(CARD16 realOffset_) : realOffset(realOffset_) {}
	};

	class BodyLink {
	public:
		enum Which {SIBLING, PARENT};
		static QString toString(Which);

		static BodyLink* getInstance(Symbols* symbols);

		const Which    which;
		const BTIndex* index;

		QString toString() const;
		BodyLink(Which which_, BTIndex* index_) : which(which_), index(index_) {}
	};

	class BodyInfo {
	public:
		class Internal {
		public:
			const CARD16 bodyTree; // Tree.Index
			const CARD16 thread;   // Tree.Index
			const CARD16 frameSize;

			QString toString() const;
			Internal(CARD16 bodyTree_, CARD16 thread_, CARD16 frameSize_) : bodyTree(bodyTree_), thread(thread_), frameSize(frameSize_) {}
		};

		class External {
		public:
			const CARD16 bytes;
			const CARD16 startIndex;
			const CARD16 indexLength;

			QString toString() const;
			External(CARD16 bytes_, CARD16 startIndex_, CARD16 indexLength_) : bytes(bytes_), startIndex(startIndex_), indexLength(indexLength_) {}
		};

		static BodyInfo* getInstance(Symbols* symbols);

		enum Tag {INTERNAL, EXTERNAL};
		static QString toString(Tag);

		const Tag   tag;
		const void* tagValue;

		const Internal&   getInternal() const;
		const External&   getExternal() const;

		QString toString() const;
		BodyInfo(Tag tag_, void* tagValue_) : tag(tag_), tagValue(tagValue_) {}
	};

	static BTRecord* getInstance(Symbols* symbols, CARD16 index);
	static BTRecord* find(Symbols* symbols, CARD16 index);

	enum class Tag {CALLABLE, OTHER};
	static QString toString(Tag value);

	const BodyLink* link;
	const BTIndex*  firstSon;
	const SEIndex*  type; // RecordSEIndex
	const CTXIndex* localCtx;
	const CARD16    level;
	const CARD16    sourceIndex;
	const BodyInfo* info;
	const Tag       tag;
	const void*     tagValue;

	const Callable& getCallable() const;
	const Other   & getOther() const;

	QString toString() const;

private:
	typedef Symbols::Key Key;
	static QMap<Key, BTRecord*> all;

	BTRecord(Symbols* symbols_, CARD16 index_, BodyLink* link_, BTIndex* firstSon_,SEIndex* type_,
			CTXIndex* localCtx_, CARD16 level_, CARD16 sourceIndex_, BodyInfo* info_, Tag tag_, void* tagValue_) :
		symbols(symbols_), index(index_), link(link_), firstSon(firstSon_), type(type_),
		localCtx(localCtx_), level(level_), sourceIndex(sourceIndex_), info(info_), tag(tag_), tagValue(tagValue_) {
		Key key(symbols, index);
		all[key] = this;
	}
};


#endif
