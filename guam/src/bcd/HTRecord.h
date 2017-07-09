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
// HTRecord.h
//

#ifndef HTRECORD_H__
#define HTRECORD_H__

#include "../util/Util.h"
#include "../mesa/MesaBasic.h"

#include "BCD.h"

class Symbols;
class HTRecord;

//HTIndex: TYPE = CARDINAL [0..Limit/2);
//HTNull: HTIndex = FIRST[HTIndex];
class HTIndex {
public:
	static const CARD16 HT_NULL = 0;
	static void resolve();

	Symbols*  symbols;
	CARD16    index;
	HTRecord* value;

	HTIndex(Symbols* symbols, CARD16 index);

	QString toString();
	QString getValue();

private:
	static QList<HTIndex*> all;
};

//HTRecord: TYPE = RECORD [
//  anyInternal, anyPublic: BOOLEAN,
//  link: HTIndex,
//  ssIndex: CARDINAL];
class HTRecord {
public:
	CARD16  index;
	bool    anyInternal;
	bool    anyPublic;
	CARD16  link;
	CARD16  ssIndex;
	QString value;

	HTRecord(CARD16 index_) : index(index_), anyInternal(false), anyPublic(false), link(0), ssIndex(0), value("#SPECIAL#") {}
	HTRecord(Symbols* symbols, CARD16 index, CARD16 lastSSIndex);

	QString toString();
};


#endif
