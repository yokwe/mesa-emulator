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


/*
 * Memory.h
 *
 *  Created on: 2013/07/23
 *      Author: hasegawa
 */

#ifndef MEMORY_H_
#define MEMORY_H_ 1

#include "../util/Util.h"
#include "../util/Perf.h"

#include "Constant.h"
#include "Type.h"
#include "Pilot.h"

#include "Variable.h"
#include "Function.h"

#define STACK_ERROR() { \
	logger.fatal("STACK_ERROR  %s -- %5d %s", __FUNCTION__, __LINE__, __FILE__); \
	StackError(); \
}
// 3.3.2 Evaluation Stack
static inline CARD32 StackCount() {
	return SP;
}
__attribute__((always_inline)) static inline void Push(CARD16 data) {
	if (SP == StackDepth) STACK_ERROR();
	stack[SP++] = data;
}
__attribute__((always_inline)) static inline CARD16 Pop() {
	if (SP == 0) STACK_ERROR();
	return stack[--SP];
}
// Note that double-word quantities are placed on the stack so that
// the least significant word is at the lower-numbered stack index.
__attribute__((always_inline)) static inline void PushLong(CARD32 data) {
//	Long t = {data};
//	Push(t.low);
//	Push(t.high);
	Push((CARD16)data);
	Push((CARD16)(data >> WordSize));
}
__attribute__((always_inline)) static inline CARD32 PopLong() {
//	Long t;
//	t.high = Pop();
//	t.low = Pop();
//	return t.u;
	CARD32 ret = Pop() << WordSize;
	return ret | Pop();
}
//static inline void MinimalStack() {
//	if (SP != 0) STACK_ERROR();
//}
#define MINIMAL_STACK() { \
	if (SP != 0) { \
		logger.fatal("MINIMAL_STACK  %s -- %5d %s", __FUNCTION__, __LINE__, __FILE__); \
		STACK_ERROR(); \
	} \
}

__attribute__((always_inline)) static inline void Recover() {
	if (SP == StackDepth) STACK_ERROR();
	SP++;
}
__attribute__((always_inline)) static inline void Discard() {
	if (SP == 0) STACK_ERROR();
	SP--;
}


// 3.1 Virtual Memory
class Memory {
public:
	static const CARD32 MAX_REALMEMORY_PAGE_SIZE = RealMemoryImplGuam::largestArraySize * WordSize;

	struct Page { CARD16 word[PageSize]; };
	struct Map {
		MapFlags mf;
		CARD16 rp;
	} __attribute__((packed));

	static Map     ReadMap(CARD32 vp);
	static void    WriteMap(CARD32 vp, Map map);
	static CARD16* Fetch(CARD32 virtualAddress);
	static CARD16* Store(CARD32 virtualAddress);

//  From APilot/15.3/Pilot/Private/GermOpsImpl.mesa
//	The BOOTING ACTION defined by the Principles of Operation should include:
//	   1. Put real memory behind any special processor pages (I/O pages);
//        then put all remaining usable real memory behind other virtual memory pages beginning at virtual page 0,
//        and working upward sequentially (skipping any already-mapped special processor pages).
//	   2. Read consecutive pages of the Germ into virtual memory beginning at
//        page Boot.pageGerm + Boot.mdsiGerm*Environment.MaxPagesInMDS (i.e page 1).
//        At present, the way the initial microcode figures out the device and device address of the Germ,
//        and the number of pages which comprise the Germ, is processor-dependent.
//	   3. Set Boot.pRequest = to e.g. [Boot.currentRequestBasicVersion, Boot.bootPhysicalVolume, locationOfPhysicalVolume].
//	   4. Set WDC>0, NWW=0, MDS=Boot.mdsiGerm, STKP=0.
//	   5. Xfer[dest: Boot.pInitialLink].

	static void initialize(int vmBits, int rmBits, CARD16 ioRegionPage);

	static void finalize();

	static CARD16 *getAddress(CARD32 va);

	static inline CARD32 getVPSize() {
		return vpSize;
	}
	static inline CARD32 getRPSize() {
		return rpSize - displayPageSize;
	}

	static void reserveDisplayPage(CARD16 displayWidth_, CARD16 displayHeight_);
	static inline CARD32 getDisplayPageSize() {
		if (displayRealPage == 0) ERROR();
		return displayPageSize;
	}
	static inline CARD32 getDisplayBytesPerLine() {
		if (displayRealPage == 0) ERROR();
		return displayBytesPerLine;
	}

	static void mapDisplay(CARD32 vp, CARD32 rp, CARD32 pageCount);
	static inline CARD32 getDisplayRealPage() {
		if (displayRealPage == 0) ERROR();
		return displayRealPage;
	}
	static inline Page* getDisplayPage() {
		if (displayRealPage == 0) ERROR();
		return realPage[displayRealPage];
	}
	static inline CARD32 getDisplayVirtualPage() {
		if (displayVirtualPage == 0) ERROR();
		return displayVirtualPage;
	}
	static inline int isDisplayPage(CARD32 vp) {
		if (displayVirtualPage == 0) ERROR();
		return (displayVirtualPage <= vp && vp < (displayVirtualPage + displayPageSize));
	}

private:
	static CARD32  vpSize;
	static CARD32  rpSize;
	static Map    *maps;
	static CARD16 *pages;
	static Page  **realPage;
	static CARD32  displayPageSize;
	static CARD32  displayRealPage;
	static CARD32  displayVirtualPage;
	static CARD32  displayWidth;
	static CARD32  displayHeight;
	static CARD32  displayBytesPerLine;
};
class PageCache {
protected:
	static const CARD32 N_BIT = 14;
	static const CARD32 N_ENTRY = 1 << N_BIT;
	static const CARD32 MASK = (1 << N_BIT) - 1;
	//
	struct Entry {
		CARD32  vpno;
		CARD16* page;
	};
	long long   missConflict;
	long long   missEmpty;
	long long   hit;
	const char* name;
	Entry       entry[N_ENTRY];
	inline CARD32 hash(CARD32 vp_) {
		return ((vp_ >> N_BIT) ^ vp_) & MASK;
		// NOTE above expression calculate better hash value than "vp_ & MASK"
	}
	void clear() {
		for(CARD32 i = 0; i < N_ENTRY; i++) {
			entry[i].vpno = 0;
			entry[i].page = 0;
		}
		missEmpty = 0;
		missConflict = 0;
		hit  = 0;
	}
	inline void invalidate(CARD32 vp_) {
		const CARD32 index = hash(vp_);
		if (entry[index].vpno != vp_) return;
		// void entry of vp_
		entry[index].vpno = 0;
		entry[index].page = 0;
	}
	void stats() {
		int used = 0;
		for(CARD32 i = 0; i < N_ENTRY; i++) {
			if (entry[i].vpno) used++;
		}
		if (PERF_ENABLE) {
			long long total = (missEmpty + missConflict) + hit;
			logger.info("PageCache %12s  %5d / %5d  %10llu %6.2f%%   miss empty %10llu  conflict %10llu", name, used, N_ENTRY, total, ((double)hit / total) * 100.0, missEmpty, missConflict);
		} else {
			logger.info("PageCache %12s  %5d / %5d", name, used, N_ENTRY);
		}
	}
public:
	PageCache(const char* name_) {
		name = name_;
		clear();
		all.append(this);
	}
	static void invalidateAll(CARD32 vp_) {
		for(auto p: all) {
			p->invalidate(vp_);
		}
	}
	static void statsAll() {
		for(auto p: all) {
			p->stats();
		}
	}
private:
	static QVector<PageCache*> all;
};
class ReadCache : public PageCache {
public:
	ReadCache(const char* name) : PageCache(name) {}
	__attribute__((always_inline)) inline CARD16* fetch(CARD32 va) {
		const CARD32 vp = va / PageSize;
		const CARD32 of = va % PageSize;
		const CARD32 index = hash(vp);
		if (entry[index].vpno != vp) {
			if (PERF_ENABLE) {
				if (entry[index].vpno) missConflict++;
				else missEmpty++;
			}
			entry[index].vpno = vp;
			entry[index].page = Memory::Fetch(va) - of;
		} else {
			if (PERF_ENABLE) hit++;
		}
		return entry[index].page + of;
	}
};
class WriteCache : public PageCache {
public:
	WriteCache(const char* name) : PageCache(name) {}
	__attribute__((always_inline)) inline CARD16* store(CARD32 va) {
		const CARD32 vp = va / PageSize;
		const CARD32 of = va % PageSize;
		const CARD32 index = hash(vp);
		if (entry[index].vpno != vp) {
			if (PERF_ENABLE) {
				if (entry[index].vpno) missConflict++;
				else missEmpty++;
			}
			entry[index].vpno = vp;
			entry[index].page = Memory::Store(va) - of;
		} else {
			if (PERF_ENABLE) hit++;
		}
		return entry[index].page + of;
	}
};

// 3.1.3 Virtual Memory Access
extern ReadCache fetchCache;
static inline CARD16* Fetch(CARD32 virtualAddress) {
	if (PERF_ENABLE) perf_Fetch++;
	return fetchCache.fetch(virtualAddress);
}
extern WriteCache storeCache;
static inline CARD16* Store(CARD32 virtualAddress) {
	if (PERF_ENABLE) perf_Store++;
	return storeCache.store(virtualAddress);
}
static inline CARD32 ReadDbl(CARD32 virtualAddress) {
	if (PERF_ENABLE) perf_ReadDbl++;
	const CARD16* p0 = Fetch(virtualAddress);
	const CARD16* p1 = ((virtualAddress & (PageSize - 1)) == (PageSize - 1)) ? Fetch(virtualAddress + 1) : p0 + 1;
//	Long t;
//	t.low  = *p0;
//	t.high = *p1;
//	return t.u;
	return (*p1 << WordSize) | *p0;
}

// 3.2.1 Main Data Space Access
class MDSCache {
public:
	static CARD32 lengthenPointer(CARD16 pointer) {
		return mds + pointer;
	}
	static void invalidateAll() {
		for(CARD32 i = 0; i < N_ENTRY; i++) cacheMDS[i].invalidate();
	}
	static inline void invalidate(CARD32 vp) {
		// If vp is not in MDS region, just return
		if ((vp & ~MASK) != ((mds / PageSize) & ~MASK)) return;
		CARD32 index = vp & MASK;
		// invalidate entry of index
		cacheMDS[index].page = 0;
	}
	static void statsAll() {
		int used = 0;
		for(CARD32 i = 0; i < N_ENTRY; i++) {
			if (cacheMDS[i].page) used++;
		}
		logger.info("MDSCache  %3d / %3d", used, N_ENTRY);
	}
	//
	// MDS
	//
	static void setMDS(CARD32 newValue) {
		if (mds == newValue) return;
		mds = newValue;
		invalidateAll();
	}
	static CARD32 MDS() {
		return mds;
	}
	__attribute__((always_inline)) static inline CARD16* storeMDS(CARD16 ptr) {
		const CARD32 vp = ptr / PageSize;
		const CARD32 of = ptr % PageSize;
		{
			CARD16* ret = cacheMDS[vp].page;
			if (ret) return ret + of;
		}
		{
			CARD16* ret = Memory::Store(lengthenPointer(ptr));
			cacheMDS[vp].page = ret - of;
			return ret;
		}
	}
protected:
	static const CARD32 N_BIT = 8;
	static const CARD32 N_ENTRY = 1 << N_BIT;
	static const CARD32 MASK = (1 << N_BIT) - 1;
	struct Entry {
		CARD16* page;
		Entry() :page(0) {}
		void invalidate() {
			page = 0;
		}
	};

	static CARD32 mds;
	static Entry  cacheMDS[N_ENTRY];
};
__attribute__((always_inline)) static inline CARD32 LengthenPointer(CARD16 pointer) {
	return MDSCache::lengthenPointer(pointer);
}
__attribute__((always_inline)) static inline CARD16* FetchMds(CARD16 ptr) {
	if (PERF_ENABLE) perf_FetchMds++;
	return MDSCache::storeMDS(ptr);
}
__attribute__((always_inline)) static inline CARD16* StoreMds(CARD16 ptr) {
	if (PERF_ENABLE) perf_StoreMds++;
	return MDSCache::storeMDS(ptr);
}
__attribute__((always_inline)) static inline CARD32 ReadDblMds(CARD16 ptr) {
	if (PERF_ENABLE) perf_ReadDblMds++;
	const CARD16* p0 = MDSCache::storeMDS(ptr + 0);
	const CARD16* p1 = MDSCache::storeMDS(ptr + 1);
//	Long t;
//	t.low  = *p0;
//	t.high = *p1;
//	return t.u;
	return (*p1 << WordSize) | *p0;
}

class LFCache {
public:
	static void setLF(CARD16 newValue) {
		lf = newValue;
		endCacheLF = PageSize - (lf % PageSize) - 1;
		cacheLF = MDSCache::storeMDS(lf);
	}
	static CARD16 LF() {
		return lf;
	}
	__attribute__((always_inline)) static inline CARD16* storeLF(CARD16 ptr) {
		if (PERF_ENABLE) perf_storeLF++;
		if (ptr <= endCacheLF) {
			if (PERF_ENABLE) perf_storeLFHit++;
			return cacheLF + ptr;
		}
		return MDSCache::storeMDS(lf + ptr);
	}
protected:
	static CARD16  lf;
	static CARD16  endCacheLF;
	static CARD16* cacheLF;
};
__attribute__((always_inline)) static inline CARD16* FetchLF(CARD16 ptr) {
	if (PERF_ENABLE) perf_FetchLF++;
	return LFCache::storeLF(ptr);
}
__attribute__((always_inline)) static inline CARD16* StoreLF(CARD16 ptr) {
	if (PERF_ENABLE) perf_StoreLF++;
	return LFCache::storeLF(ptr);
}
__attribute__((always_inline)) static inline CARD32 ReadDblLF(CARD16 ptr) {
	if (PERF_ENABLE) perf_ReadDblLF++;
	const CARD16* p0 = LFCache::storeLF(ptr + 0);
	const CARD16* p1 = LFCache::storeLF(ptr + 1);
//	Long t;
//	t.low  = *p0;
//	t.high = *p1;
//	return t.u;
	return (*p1 << WordSize) | *p0;
}


// 3.1.4.3 Code Segments
class CodeCache {
public:
	__attribute__((always_inline)) static inline CARD8 getCodeByte() {
		if (PC < startPC || endPC < PC) {
			if (PERF_ENABLE) miss++;
			setup();
		} else {
			if (PERF_ENABLE) hit++;
		}
		return page[(PC++ + offset) ^1];
		// TODO Code above is for Little Endian
		// TODO For Big Endian code should be "return page[(PC++ + offset)];"
	}
	static inline CARD16 getCodeWord() {
		BytePair ret;
		ret.left  = getCodeByte();
		ret.right = getCodeByte();
		return ret.u;
	}
	static inline CARD32 CB() {
		return CB_;
	}
	static inline void setCB(CARD32 newValue) {
		CB_ = newValue;
		invalidate();
	}
	static void stats();
private:
	static const CARD32 PAGE_SIZE = PageSize * 2;
	static const CARD32 PAGE_MASK = PAGE_SIZE - 1;
	static CARD8* page;
	static INT32  offset;  // byte offset of PC to access data in page (can be negative)
	static CARD16 startPC; // valid PC range (startPC <= PC <= endPC)
	static CARD16 endPC;   // valid PC range (startPC <= PC <= endPC)
	static CARD32 CB_;
	static long long miss;
	static long long hit;
	//
	static void setup();
	static inline void invalidate() {
		startPC = 0xffff;
		endPC   = 0;
		page    = 0;
	}
};
static inline CARD16* FetchCode(CARD16 offset) {
	if (PERF_ENABLE) perf_FetchCode++;
	return fetchCache.fetch(CodeCache::CB() + offset);
}
static inline CARD16 ReadCode(CARD16 offset) {
	return *FetchCode(offset);
}

// 4.3 Instruction Fetch
__attribute__((always_inline)) static inline CARD8 GetCodeByte() {
	if (PERF_ENABLE) perf_GetCodeByte++;
	return CodeCache::getCodeByte();
}
__attribute__((always_inline)) static inline CARD16 GetCodeWord() {
	if (PERF_ENABLE) perf_GetCodeWord++;
	return CodeCache::getCodeWord();
}

// 7.4 String Instructions
static inline BYTE FetchByte(LONG_POINTER ptr, LONG_CARDINAL offset) {
	if (PERF_ENABLE) perf_FetchByte++;
	ptr += offset / 2;
	BytePair word = {*Fetch(ptr)};
	return ((offset % 2) == 0) ? (BYTE)word.left : (BYTE)word.right;
}
static inline CARD16 FetchWord(LONG_POINTER ptr, LONG_CARDINAL offset) {
	BytePair ret;
	ret.left  = FetchByte(ptr, offset);
	ret.right = FetchByte(ptr, offset + 1);
	return ret.u;
}
static inline void StoreByte(LONG_POINTER ptr, LONG_CARDINAL offset, BYTE data) {
	if (PERF_ENABLE) perf_StoreByte++;
	ptr += offset / 2;
	BytePair word = {*Fetch(ptr)};
	if ((offset % 2) == 0) {
		word.left = data;
	} else {
		word.right = data;
	}
	*Store(ptr) = word.u;
}

// 7.5 Field Instruction
//const UNSPEC Field_MaskTable[WordSize] = {
//		0x0001, 0x0003, 0x0007, 0x000f, 0x001f, 0x003f, 0x007f, 0x00ff,
//		0x01ff, 0x03ff, 0x07ff, 0x0fff, 0x1fff, 0x3fff, 0x7fff, 0xffff
//};
static inline CARD16 Field_MaskTable(CARD8 n) {
	return (CARD16)((1U << (n + 1)) - 1);
}

static inline UNSPEC ReadField(UNSPEC source, CARD8 spec8) {
	if (PERF_ENABLE) perf_ReadField++;
	FieldSpec spec = {spec8};

	if (WordSize < (spec.pos + spec.size + 1)) ERROR();
	int shift = WordSize - (spec.pos + spec.size + 1);
	// shift is always positive
	// return Shift(source, -shift) & MaskTable(spec.size);
	return (source >> shift) & Field_MaskTable(spec.size);
}
static inline UNSPEC WriteField(UNSPEC dest, CARD8 spec8, UNSPEC data) {
	if (PERF_ENABLE) perf_WriteField++;
	FieldSpec spec = {spec8};

	if (WordSize < (spec.pos + spec.size + 1)) ERROR();
	int shift = WordSize - (spec.pos + spec.size + 1);
	// shift is always positive
	//UNSPEC mask = Shift(MaskTable[spec.size], shift);
	UNSPEC mask = Field_MaskTable(spec.size) << shift;
	//return (dest & ~mask) | (Shift(data, shift) & mask);
	return (dest & ~mask) | ((data << shift) & mask);
}


// 9.4.2 External Function Calls
static inline CARD32 FetchLink(CARD32 offset) {
	GlobalWord word = {*Fetch(GO_OFFSET(GF, word))};
	//CARD32 pointer = word.codelinks ? (CodeCache::CB() - (CARD32)((offset + 1) * 2)) : (GlobalBase(GF) - (CARD32)((offset + 1) * 2));
	CARD32 pointer = (word.codelinks ? CodeCache::CB() : GlobalBase(GF)) - (CARD32)((offset + 1) * 2);
	return ReadDbl(pointer);
}

// 10.1.1 Process Data Area
static inline LONG_POINTER LengthenPdaPtr(POINTER ptr) {
	return PDA + ptr;
}
static inline POINTER OffsetPda(LONG_POINTER ptr) {
	if ((ptr & 0xffff0000) != (PDA & 0xffff0000)) ERROR();
	return (CARD16)(ptr - PDA);
}
static inline CARD16* FetchPda(POINTER ptr) {
	if (PERF_ENABLE) perf_FetchPda++;
	return fetchCache.fetch(LengthenPdaPtr(ptr));
}
static inline CARD16* StorePda(POINTER ptr) {
	if (PERF_ENABLE) perf_StorePda++;
	return storeCache.store(LengthenPdaPtr(ptr));
}

// 9.5.3 Trap Handlers
static inline void SaveStack(StateHandle state) {
	for(int sp = 0; sp < StackDepth; sp++) {
		*Store(state + OFFSET(StateVector, stack[sp])) = stack[sp];
	}
	StateWord word;
	word.breakByte = breakByte;
	word.stkptr = SP;
	*Store(state + OFFSET(StateVector, word)) = word.u;
	SP = savedSP = 0;
	breakByte = 0;
}
static inline void LoadStack(StateHandle state) {
	StateWord word;
	word.u = *Fetch(state + OFFSET(StateVector, word));
	for(int sp = 0; sp < StackDepth; sp++) {
		stack[sp] = *Fetch(state + OFFSET(StateVector, stack[sp]));
	}
	SP = savedSP = word.stkptr;
	breakByte = word.breakByte;
}

// 10.4.1 Scheduler
static inline int ValidContext() {
	return (SIZE(CodeSegment) * 2) <= PC;
}

#endif /* MEMORY_H_ */
