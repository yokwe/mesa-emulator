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
// TreeIndex.h
//

#ifndef TREEINDEX_H__
#define TREEINDEX_H__

#include "../util/Util.h"
#include "../mesa/MesaBasic.h"

#include "BCD.h"
#include "Symbols.h"


class TreeIndex;
class TreeNode;
class TreeLink;
class LitRecord;


//Index: TYPE = Base RELATIVE POINTER [0..Limit) TO Tree.Node;
//NullIndex: Tree.Index = FIRST[Tree.Index];
class TreeIndex {
private:
	static constexpr const char* PREFIX = "tree";
	static const CARD16  TREE_NULL = 0;

	Symbols*  symbols;
	CARD16    index;


public:
	static void checkAll();
	static TreeIndex* getNull();
	static TreeIndex* getInstance(Symbols* symbols, CARD16 index);

	bool isNull() const {
		return index == TREE_NULL;
	}
	QString toString() const;

	const TreeNode& getValue() const;
private:
	static QList<TreeIndex*> all;
	TreeIndex(Symbols* symbols_, CARD16 index_) : symbols(symbols_), index(index_) {
		all.append(this);
	}
};


//Info: TYPE [SIZE[CARDINAL]];
//
//Node: TYPE = MACHINE DEPENDENT RECORD [
//  free (0: 0..0): BOOLEAN,    -- reserved for allocator
//  name (0: 1..8): NodeName,
//  attr1 (0: 9..9), attr2 (0: 10..10), attr3 (0: 11..11): BOOLEAN,
//  shared (0: 12..12): BOOLEAN,
//  nSons (0: 13..15): [0..MaxNSons],
//  info (1): Info,
//  son (2): ARRAY [1..1) OF Link];
class TreeNode {
private:
	const Symbols*  symbols;
	const CARD16    index;

public:
	static TreeNode* getInstance(Symbols* symbols, CARD16 index);
	static TreeNode* find(Symbols* symbols, CARD16 index);

	enum class NodeName {
	// -- general tree constructors
		LIST, ITEM,
	// -- declarations
		DECL, TYPE_DECL,
		BASIC_TC, ENUMERATED_TC, RECORD_TC, MONITORED_TC, VARIANT_TC,
		REF_TC, POINTER_TC, LIST_TC, ARRAY_TC, ARRAYDESC_TC, SEQUENCE_TC,
		PROC_TC, PROCESS_TC, PORT_TC, SIGNAL_TC, ERROR_TC, PROGRAM_TC,
		ANY_TC, DEFINITION_TC, UNION_TC, RELATIVE_TC,
		SUBRANGE_TC, LONG_TC, OPAQUE_TC, ZONE_TC, LINK_TC, VAR_TC,
		IMPLICIT_TC, FRAME_TC, DISCRIM_TC,
		ENTRY, INTERNAL,
		UNIT, DIRITEM, MODULE, BODY, INLINE, LAMBDA, BLOCK,
	//-- statements
		ASSIGN, EXTRACT,
		IF,
		CASE, CASETEST, CASESWITCH,
		BIND,
		DO, FORSEQ, UPTHRU, DOWNTHRU,
		RETURN, RESULT,
		GOTO, EXIT, LOOP,
		FREE,
		RESUME, REJECT, CONTINUE, RETRY, CATCHMARK,
		RESTART, STOP,
		LOCK, WAIT, NOTIFY, BROADCAST, UNLOCK,
		NULL_,
		LABEL,
		OPEN,
		ENABLE, CATCH,
		DSK, LSK, XE, XF,
		SYSCALL, CHECKED, SPARES2, SPARES3,
		SUBST, CALL, PORTCALL, SIGNAL, ERROR, SYSERROR, XERROR,
		START, JOIN,
	// -- expressions
		APPLY,
		CALLX, PORTCALLX, SIGNALX, ERRORX, SYSERRORX, STARTX, FORK, JOINX,
		INDEX, DINDEX, SEQINDEX, RELOC,
		CONSTRUCT, UNION, ROWCONS, SEQUENCE, LISTCONS,
		SUBSTX,
		IFX, CASEX, BINDX,
		ASSIGNX, EXTRACTX,
		OR, AND,
		RELE, RELN, RELL, RELGE, RELG, RELLE, IN, NOTIN,
		PLUS, MINUS, TIMES, DIV, MOD,
		DOT, CDOT, DOLLAR,
		CREATE,
		NOT,
		UMINUS,
		ADDR,
		UPARROW,
		MIN, MAX, LENGTHEN, ABS, ALL,
		SIZE, FIRST, LAST, PRED, SUCC,
		ARRAYDESC, LENGTH, BASE,
		LOOPHOLE,
		NIL,
		NEW,
		VOID,
		CLIT, LLIT,
		CAST, CHECK, FLOAT, PAD, CHOP, SAFEN,
		SYSCALLX, NARROW, ISTYPE,
		OPENX,
		MWCONST, CONS,
		ATOM, TYPECODE,
		STRINGINIT, TEXTLIT, SIGNALINIT, PROCINIT,
		INTOO, INTOC, INTCO, INTCC,

		THREAD,
		NONE,

		EXLIST,
		INITLIST,
		DITEM,

		SHORTEN,
		SELF,
		GCRT,
		PROCCHECK,

		ORD,
		VAL,

		MERGECONS
	};

	static QString toString(NodeName value);

//	const bool       free;
	const NodeName   name;
//	const bool       attr1;
//	const bool       attr2;
//	const bool       attr3;
	const bool       shared;
	const CARD16     nSons;
	const CARD16     info;
	TreeLink** const son;

	QString toString() const;

private:
	typedef Symbols::Key Key;
	static QMap<Key, TreeNode*> all;

	TreeNode(Symbols* symbols_, CARD16 index_, NodeName name_, bool shared_, CARD16 nSons_, CARD16 info_, TreeLink* son_[]) :
		symbols(symbols_), index(index_), name(name_), shared(shared_), nSons(nSons_), info(info_), son(son_) {
		Key key(symbols, index);
		all[key] = this;
	}
};


//Link: TYPE = RECORD [
//  SELECT tag(0:0..1): * FROM
//    subtree => [index(0:2..15): Tree.Index],
//    hash => [index(0:2..15): Symbols.HTIndex],
//    symbol => [index(0:2..15): Symbols.ISEIndex],
//    literal => [info(0:2..15): Literals.LitRecord]
//    ENDCASE];
class TreeLink {
public:
	enum class Tag {SUBTREE, HASH, SYMBOL, LITERAL};
	static QString toString(Tag value);

	class Subtree {
	public:
		const TreeIndex* index;

		QString toString() const;

		Subtree(TreeIndex* index_) : index(index_) {}
	};

	class Hash {
	public:
		const HTIndex* index;

		QString toString() const;

		Hash(HTIndex* index_) : index(index_) {}
	};

	class Symbol {
	public:
		const SEIndex* index; // ISEIndex

		QString toString() const;

		Symbol(SEIndex* index_) : index(index_) {}
	};

	class Literal {
	public:
		const LitRecord* info;

		QString toString() const;

		Literal(LitRecord* info_) : info(info_) {}
	};

	static TreeLink* getInstance(Symbols* symbols);

	const Tag   tag;
	const void* tagValue;

	const Subtree& getSubtree() const;
	const Hash&    getHash()    const;
	const Symbol&  getSymbol()  const;
	const Literal& getLiteral() const;
	
	//Null: Tree.Link = [subtree[index: Tree.NullIndex]];
	bool isNull() const {
		return tag == Tag::SUBTREE && getSubtree().index->isNull();
	}

	QString toString() const;

	TreeLink(Tag tag_, void* tagValue_) : tag(tag_), tagValue(tagValue_) {}
};
#endif
