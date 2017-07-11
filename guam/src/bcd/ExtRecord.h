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
// ExtRecord.h
//

#ifndef EXTRECORD_H__
#define EXTRECORD_H__

#include "../util/Util.h"
#include "../mesa/MesaBasic.h"

#include "Table.h"
#include "Symbols.h"

class ExtRecord;
class SEIndex;
class TreeLink;


//ExtIndex: TYPE = Base RELATIVE POINTER [0..Limit) TO ExtRecord;
//ExtNull: ExtIndex = LAST[ExtIndex];

class ExtIndex {
public:
	static const CARD16 Ext_NULL = Table::LIMIT - 1;

	static void resolve();

	Symbols*  symbols;
	CARD16    index;
	ExtRecord* value;

	ExtIndex(Symbols* symbols_, CARD16 index_);

	QString toString();

private:
	static QList<ExtIndex*> all;
};

//-- symbol table extensions
//
//ExtRecord: PUBLIC TYPE = RECORD [
//  type (0:0..1): Symbols.ExtensionType[value..default],
//  sei (0:2..15): Symbols.ISEIndex,
//  tree (1:0..15): Tree.Link]
class ExtRecord {
public:
	typedef Symbols::ExtensionType ExtensionType;
	CARD16        index;

	ExtensionType type;
	SEIndex*      sei;
	TreeLink*     tree;

	ExtRecord(Symbols* symbols, CARD16 index);

	QString toString();
};



#endif
