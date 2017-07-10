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
// Tree.h
//

#ifndef TREE_H__
#define TREE_H__

#include "../util/Util.h"
#include "../mesa/MesaBasic.h"

#include "Table.h"

class Symbols;
class TreeNode;
class SEIndex;
class HTIndex;
class LitRecord;


//Index: TYPE = Base RELATIVE POINTER [0..Limit) TO Tree.Node;
//NullIndex: Tree.Index = FIRST[Tree.Index];
class TreeIndex {
public:
	static const CARD16 TREE_NULL = 0;

	static void resolve();

	Symbols*  symbols;
	CARD16    index;
	TreeNode* value;

	TreeIndex(Symbols* symbols_, CARD16 index_);

	QString toString();

private:
	static QList<TreeIndex*> all;
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
	enum class Tag {
		SUBTREE, HASH, SYMBOL, LITERAL
	};
	static QString toString(Tag value);

	Tag tag;
	union {
		struct {
			TreeIndex* index;
		} subtree;
		struct {
			HTIndex* index;
		} hash;
		struct {
			SEIndex* index;
		} symbol;
		struct {
			LitRecord* info;
		} literal;
	};

	TreeLink(Symbols* symbols);

	QString toString();
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
public:
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

		MERGECONS};
	static QString toString(NodeName value);

	CARD16     index;
	bool       free_;
	NodeName   name;
	bool       attr1;
	bool       attr2;
	bool       attr3;
	bool       shared;
	CARD16     nSons;
	CARD16     info;
	TreeLink** son; // index starts with 1. Treat info as first member of son?

	TreeNode(Symbols* symbols, CARD16 index);

	QString toString();
};

#endif
