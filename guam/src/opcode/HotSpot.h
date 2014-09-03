/*
Copyright (c) 2014, Yasuhiro Hasegawa
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
// HotSpot.h
//

#ifndef HOTSPOT_H_
#define HOTSPOT_H_

#include "../mesa/MesaBasic.h"
#include "../mesa/Variable.h"
#include "../mesa/Memory.h"

#include "Run.h"

#include <QHash>
#include <QVector>

class HotSpot {
public:
	class Address {
	public:
		const CARD32 cb;
		const CARD16 pc;
		const CARD32 hash;
		Address() : cb(CodeCache::CB()), pc(PC), hash(getHash()) {}
		Address(CARD32 cb_, CARD16 pc_) : cb(cb_), pc(pc_), hash(getHash()) {}
		CARD32 getHash() {
			return (cb << 1) + pc;
		}
	};

	static void update_XFER() {
		update(MAX_COMPILE_LEVEL_XFER, THREASHOLD_COUNT_XFER);
	}
	static void update_JUMP() {
		//update(MAX_COMPILE_LEVEL_JUMP, THREASHOLD_COUNT_JUMP);
	}
	static void update_PROC() {
		update(MAX_COMPILE_LEVEL_PROC, THREASHOLD_COUNT_PROC);
	}
	static void update_BREAK() {}

	static void stats();

	const Address address;
	const CARD16  gfi;
	HotSpot() : gfi(GFI), lastCodeIsXFER(0), compiled(0), countRun(0), countInvoke(0), countCacheHit(0), nextCache1(0), nextCache2(0) {}

protected:
	static const int MAX_COMPILE_LEVEL_XFER =  99;
	static const int THREASHOLD_COUNT_XFER  = 300;

	//static const int MAX_COMPILE_LEVEL_JUMP =  1;
	//static const int THREASHOLD_COUNT_JUMP  = 30;

	static const int MAX_COMPILE_LEVEL_PROC =  99;
	static const int THREASHOLD_COUNT_PROC  = 300;

	static QHash<Address, HotSpot*> hotSpots;
	static HotSpot* compile(CARD32 cb, CARD16 pc, const int level);
	static void run(HotSpot* hotSpot);
	static void update(const int compileLevel, const int threasholdCount);
	static void peephole(HotSpot* hotSpot);

	QVector <Run> opcodes;
	CARD32 lastCodeIsXFER;
	int compiled;
	int countRun;
	int countInvoke;
	int countCacheHit;
	HotSpot* nextCache1;
	HotSpot* nextCache2;
};

inline bool operator==(const HotSpot::Address &e1, const HotSpot::Address &e2) {
	return e1.hash == e2.hash && e1.pc == e2.pc && e1.cb == e2.cb;
}

// Intentionally use old format to keep compatible with qt4 and qt5
inline uint qHash(const HotSpot::Address &key) {
	return key.hash;
}
#endif
