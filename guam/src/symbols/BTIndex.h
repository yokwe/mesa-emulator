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
	static BTIndex* getNull();
	static BTIndex* getInstance(Symbols* symbols_, CARD16 index_);

	bool isNull() const {
		return index == BT_NULL;
	}
	QString toString() const;
	const BTRecord& getValue() const;

private:
	BTIndex(Symbols* symbols_, CARD16 index_) : symbols(symbols_), index(index_) {}
};




#endif
