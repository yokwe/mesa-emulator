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
// Tree.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("tree");

#include "Tree.h"

#include "Symbols.h"

#include "SERecord.h"
#include "HTRecord.h"
#include "Literals.h"


QList<TreeIndex*> TreeIndex::all;

TreeIndex::TreeIndex(Symbols* symbols_, CARD16 index_) : symbols(symbols_), index(index_), value(0) {
	all.append(this);
}

QString TreeIndex::toString() {
	if (index == TREE_NULL) return "#NULL#";
	if (value == 0) return QString("tree-%1").arg(index);

	return QString("tree-%1-%2").arg(index).arg(value ? value->toString() : "#EMPTY#");
}
void TreeIndex::resolve() {
	for(TreeIndex *p: all) {
		const CARD16 index = p->index;

		if (p->value) continue;
		if (p->index == TREE_NULL) continue;

		if (p->symbols == 0) {
			logger.error("p->symbols == null %d p = %s", index, p->toString().toLocal8Bit().constData());
			ERROR();
		}
		if (p->symbols->tree.contains(index)) {
//			logger.info("resolve tree %4d", index);
			p->value = p->symbols->tree[index];
		} else {
			logger.error("Unknown %d = %s", index, p->toString().toLocal8Bit().constData());
			ERROR();
		}
	}
}

QString TreeNode::toString(NodeName value) {
	TO_STRING_PROLOGUE(NodeName)

	// -- general tree constructors
	MAP_ENTRY(LIST)
	MAP_ENTRY(ITEM)

	// -- declarations
	MAP_ENTRY(DECL)
	MAP_ENTRY(TYPE_DECL)
	MAP_ENTRY(BASIC_TC)
	MAP_ENTRY(ENUMERATED_TC)
	MAP_ENTRY(RECORD_TC)
	MAP_ENTRY(MONITORED_TC)
	MAP_ENTRY(VARIANT_TC)
	MAP_ENTRY(REF_TC)
	MAP_ENTRY(POINTER_TC)
	MAP_ENTRY(LIST_TC)
	MAP_ENTRY(ARRAY_TC)
	MAP_ENTRY(ARRAYDESC_TC)
	MAP_ENTRY(SEQUENCE_TC)
	MAP_ENTRY(PROC_TC)
	MAP_ENTRY(PROCESS_TC)
	MAP_ENTRY(PORT_TC)
	MAP_ENTRY(SIGNAL_TC)
	MAP_ENTRY(ERROR_TC)
	MAP_ENTRY(PROGRAM_TC)
	MAP_ENTRY(ANY_TC)
	MAP_ENTRY(DEFINITION_TC)
	MAP_ENTRY(UNION_TC)
	MAP_ENTRY(RELATIVE_TC)
	MAP_ENTRY(SUBRANGE_TC)
	MAP_ENTRY(LONG_TC)
	MAP_ENTRY(OPAQUE_TC)
	MAP_ENTRY(ZONE_TC)
	MAP_ENTRY(LINK_TC)
	MAP_ENTRY(VAR_TC)
	MAP_ENTRY(IMPLICIT_TC)
	MAP_ENTRY(FRAME_TC)
	MAP_ENTRY(DISCRIM_TC)
	MAP_ENTRY(ENTRY)
	MAP_ENTRY(INTERNAL)
	MAP_ENTRY(UNIT)
	MAP_ENTRY(DIRITEM)
	MAP_ENTRY(MODULE)
	MAP_ENTRY(BODY)
	MAP_ENTRY(INLINE)
	MAP_ENTRY(LAMBDA)
	MAP_ENTRY(BLOCK)

	//-- statements
	MAP_ENTRY(ASSIGN)
	MAP_ENTRY(EXTRACT)
	MAP_ENTRY(IF)
	MAP_ENTRY(CASE)
	MAP_ENTRY(CASETEST)
	MAP_ENTRY(CASESWITCH)
	MAP_ENTRY(BIND)
	MAP_ENTRY(DO)
	MAP_ENTRY(FORSEQ)
	MAP_ENTRY(UPTHRU)
	MAP_ENTRY(DOWNTHRU)
	MAP_ENTRY(RETURN)
	MAP_ENTRY(RESULT)
	MAP_ENTRY(GOTO)
	MAP_ENTRY(EXIT)
	MAP_ENTRY(LOOP)
	MAP_ENTRY(FREE)
	MAP_ENTRY(RESUME)
	MAP_ENTRY(REJECT)
	MAP_ENTRY(CONTINUE)
	MAP_ENTRY(RETRY)
	MAP_ENTRY(CATCHMARK)
	MAP_ENTRY(RESTART)
	MAP_ENTRY(STOP)
	MAP_ENTRY(LOCK)
	MAP_ENTRY(WAIT)
	MAP_ENTRY(NOTIFY)
	MAP_ENTRY(BROADCAST)
	MAP_ENTRY(UNLOCK)
	MAP_ENTRY(NULL_)
	MAP_ENTRY(LABEL)
	MAP_ENTRY(OPEN)
	MAP_ENTRY(ENABLE)
	MAP_ENTRY(CATCH)
	MAP_ENTRY(DSK)
	MAP_ENTRY(LSK)
	MAP_ENTRY(XE)
	MAP_ENTRY(XF)
	MAP_ENTRY(SYSCALL)
	MAP_ENTRY(CHECKED)
	MAP_ENTRY(SPARES2)
	MAP_ENTRY(SPARES3)
	MAP_ENTRY(SUBST)
	MAP_ENTRY(CALL)
	MAP_ENTRY(PORTCALL)
	MAP_ENTRY(SIGNAL)
	MAP_ENTRY(ERROR)
	MAP_ENTRY(SYSERROR)
	MAP_ENTRY(XERROR)
	MAP_ENTRY(START)
	MAP_ENTRY(JOIN)

	// -- expressions
	MAP_ENTRY(APPLY)
	MAP_ENTRY(CALLX)
	MAP_ENTRY(PORTCALLX)
	MAP_ENTRY(SIGNALX)
	MAP_ENTRY(ERRORX)
	MAP_ENTRY(SYSERRORX)
	MAP_ENTRY(STARTX)
	MAP_ENTRY(FORK)
	MAP_ENTRY(JOINX)
	MAP_ENTRY(INDEX)
	MAP_ENTRY(DINDEX)
	MAP_ENTRY(SEQINDEX)
	MAP_ENTRY(RELOC)
	MAP_ENTRY(CONSTRUCT)
	MAP_ENTRY(UNION)
	MAP_ENTRY(ROWCONS)
	MAP_ENTRY(SEQUENCE)
	MAP_ENTRY(LISTCONS)
	MAP_ENTRY(SUBSTX)
	MAP_ENTRY(IFX)
	MAP_ENTRY(CASEX)
	MAP_ENTRY(BINDX)
	MAP_ENTRY(ASSIGNX)
	MAP_ENTRY(EXTRACTX)
	MAP_ENTRY(OR)
	MAP_ENTRY(AND)
	MAP_ENTRY(RELE)
	MAP_ENTRY(RELN)
	MAP_ENTRY(RELL)
	MAP_ENTRY(RELGE)
	MAP_ENTRY(RELG)
	MAP_ENTRY(RELLE)
	MAP_ENTRY(IN)
	MAP_ENTRY(NOTIN)
	MAP_ENTRY(PLUS)
	MAP_ENTRY(MINUS)
	MAP_ENTRY(TIMES)
	MAP_ENTRY(DIV)
	MAP_ENTRY(MOD)
	MAP_ENTRY(DOT)
	MAP_ENTRY(CDOT)
	MAP_ENTRY(DOLLAR)
	MAP_ENTRY(CREATE)
	MAP_ENTRY(NOT)
	MAP_ENTRY(UMINUS)
	MAP_ENTRY(ADDR)
	MAP_ENTRY(UPARROW)
	MAP_ENTRY(MIN)
	MAP_ENTRY(MAX)
	MAP_ENTRY(LENGTHEN)
	MAP_ENTRY(ABS)
	MAP_ENTRY(ALL)
	MAP_ENTRY(SIZE)
	MAP_ENTRY(FIRST)
	MAP_ENTRY(LAST)
	MAP_ENTRY(PRED)
	MAP_ENTRY(SUCC)
	MAP_ENTRY(ARRAYDESC)
	MAP_ENTRY(LENGTH)
	MAP_ENTRY(BASE)
	MAP_ENTRY(LOOPHOLE)
	MAP_ENTRY(NIL)
	MAP_ENTRY(NEW)
	MAP_ENTRY(VOID)
	MAP_ENTRY(CLIT)
	MAP_ENTRY(LLIT)
	MAP_ENTRY(CAST)
	MAP_ENTRY(CHECK)
	MAP_ENTRY(FLOAT)
	MAP_ENTRY(PAD)
	MAP_ENTRY(CHOP)
	MAP_ENTRY(SAFEN)
	MAP_ENTRY(SYSCALLX)
	MAP_ENTRY(NARROW)
	MAP_ENTRY(ISTYPE)
	MAP_ENTRY(OPENX)
	MAP_ENTRY(MWCONST)
	MAP_ENTRY(CONS)
	MAP_ENTRY(ATOM)
	MAP_ENTRY(TYPECODE)
	MAP_ENTRY(STRINGINIT)
	MAP_ENTRY(TEXTLIT)
	MAP_ENTRY(SIGNALINIT)
	MAP_ENTRY(PROCINIT)
	MAP_ENTRY(INTOO)
	MAP_ENTRY(INTOC)
	MAP_ENTRY(INTCO)
	MAP_ENTRY(INTCC)
	MAP_ENTRY(THREAD)
	MAP_ENTRY(NONE)
	MAP_ENTRY(EXLIST)
	MAP_ENTRY(INITLIST)
	MAP_ENTRY(DITEM)
	MAP_ENTRY(SHORTEN)
	MAP_ENTRY(SELF)
	MAP_ENTRY(GCRT)
	MAP_ENTRY(PROCCHECK)
	MAP_ENTRY(ORD)
	MAP_ENTRY(VAL)
	MAP_ENTRY(MERGECONS)

	TO_STRING_EPILOGUE
}


QString TreeLink::toString(Tag value) {
	TO_STRING_PROLOGUE(Tag)

	MAP_ENTRY(SUBTREE)
	MAP_ENTRY(HASH)
	MAP_ENTRY(SYMBOL)
	MAP_ENTRY(LITERAL)

	TO_STRING_EPILOGUE
}

//Link: TYPE = RECORD [
//  SELECT tag(0:0..1): * FROM
//    subtree => [index(0:2..15): Tree.Index],
//    hash => [index(0:2..15): Symbols.HTIndex],
//    symbol => [index(0:2..15): Symbols.ISEIndex],
//    literal => [info(0:2..15): Literals.LitRecord]
//    ENDCASE];
TreeLink::TreeLink(Symbols* symbols) {
	CARD16 u0 = symbols->file->getCARD16();

	CARD16 u0a = bitField(u0, 0, 1);
	CARD16 u0b = bitField(u0, 2, 15);

	tag = (Tag)u0a;
	switch(tag) {
	case Tag::SUBTREE:
		subtree.index = new TreeIndex(symbols, u0b);
		break;
	case Tag::HASH:
		hash.index    = new HTIndex(symbols, u0b);
		break;
	case Tag::SYMBOL:
		symbol.index  = new SEIndex(symbols, u0b);
		break;
	case Tag::LITERAL:
		literal.info  = new LitRecord(symbols, u0b);
		break;
	default:
		ERROR();
		break;
	}
}
QString TreeLink::toString() {
	switch(tag) {
	case Tag::SUBTREE:
		return QString("%1 %2").arg(toString(tag)).arg(subtree.index->toString());
	case Tag::HASH:
		return QString("%1 %2").arg(toString(tag)).arg(hash.index->toString());
	case Tag::SYMBOL:
		return QString("%1 %2").arg(toString(tag)).arg(symbol.index->toString());
	case Tag::LITERAL:
		return QString("%1 %2").arg(toString(tag)).arg(literal.info->toString());
	default:
		ERROR();
		return "???";
	}
}

//Node: TYPE = MACHINE DEPENDENT RECORD [
//  free (0: 0..0): BOOLEAN,    -- reserved for allocator
//  name (0: 1..8): NodeName,
//  attr1 (0: 9..9), attr2 (0: 10..10), attr3 (0: 11..11): BOOLEAN,
//  shared (0: 12..12): BOOLEAN,
//  nSons (0: 13..15): [0..MaxNSons],
//  info (1): Info,
//  son (2): ARRAY [1..1) OF Link];
TreeNode::TreeNode(Symbols* symbols, CARD16 index_) {
	index = index_;

	CARD16 u0 = symbols->file->getCARD16();
	free_     = bitField(u0, 0);
	name      = (NodeName)bitField(u0, 1, 8);
	attr1     = bitField(u0, 9);
	attr2     = bitField(u0, 10);
	attr3     = bitField(u0, 11);
	shared    = bitField(u0, 12);
	nSons     = bitField(u0, 13, 15);
	info      = symbols->file->getCARD16();

	son = new TreeLink*[nSons];
	for(CARD16 i = 0; i < nSons; i++) {
        son[i] = new TreeLink(symbols);
	}
}

QString TreeNode::toString() {
	QString retSon;
	for(CARD16 i = 0; i < nSons; i++) {
		retSon.append(QString(" %1").arg(son[i]->toString()));
	}
	return QString("tree-%1 %2 %3 (%4)%5").arg(index).arg(toString(name)).arg(info).arg(nSons).arg(retSon);
}

