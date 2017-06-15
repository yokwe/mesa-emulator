/*
Copyright (c) 2014, 2017, Yasuhiro Hasegawa
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


// TODO Don't call InterruptThread::isPending(). Like we did in Opcode_bitblt.cpp
//      PC = savedPC and return doesn't make sense.
// TODO Check correctness of each implementation. Especially *R() methods.
//      *R() need copy data backward in [0..count).

//
// Opcode_block.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("block");

#include "../util/Debug.h"

#include "../mesa/MesaBasic.h"
#include "../mesa/Memory.h"
#include "../mesa/Function.h"
#include "../mesa/MesaThread.h"

#include "Opcode.h"

#define USE_FAST_BLT

// zBLT - 0363
#ifdef USE_FAST_BLT
static inline void OP_BLT_QUICK(POINTER source, POINTER dest, CARDINAL count) {
	// Assume source region stay within one page
	// Assume dest region stay within one page
	CARD16 *s = FetchMds(source);
	CARD16 *d = StoreMds(dest);
	while(count--) *d++ = *s++;
}
static inline void OP_BLT_FAST(POINTER source, POINTER dest, CARDINAL count) {
	for(;;) {
		CARD16 sRun = PageSize - (source % PageSize);
		CARD16 dRun = PageSize - (dest   % PageSize);
		CARD16 run = (sRun < dRun) ? sRun : dRun;
		if (count < run) run = count;

		OP_BLT_QUICK(source, dest, run);

		source += run;
		count  -= run;
		dest   += run;
		if (count == 0) break;

		if (InterruptThread::isPending()) {
			PC = savedPC;
			Push(source);
			Push(count);
			Push(dest);
			break;
		}
	}
}
void E_BLT() {
	POINTER  dest   = Pop();
	CARDINAL count  = Pop();
	POINTER  source = Pop();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  BLT       %4X     %4X %5d", savedPC, source, dest, count);

	if (count == 0) return;

	CARDINAL dStartPage = dest                 / PageSize;
	CARDINAL sStartPage = source               / PageSize;
	CARDINAL dEndPage   = (dest + count - 1)   / PageSize;
	CARDINAL sEndPage   = (source + count - 1) / PageSize;
	if (dStartPage == dEndPage && sStartPage == sEndPage) {
		// Most of BLT is this case.
		OP_BLT_QUICK(source, dest, count);
	} else {
		OP_BLT_FAST(source, dest, count);
	}
}
#else
void E_BLT() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  BLT", savedPC);
	for(;;) {
		POINTER dest = Pop();
		CARDINAL count = Pop();
		POINTER source = Pop();
		if (count == 0) break;
		*StoreMds(dest) = *FetchMds(source);
		Push(source + 1);
		Push(count - 1);
		Push(dest + 1);
		if (InterruptThread::isPending()) {
			PC = savedPC;
			break;
		}
	}
}
#endif


// zBLTL - 0364
#ifdef USE_FAST_BLT
static inline void OP_BLTL_QUICK(LONG_POINTER source, LONG_POINTER dest, CARDINAL count) {
	// Assume source region stay within one page
	// Assume dest region stay within one page
	CARD16 *s = Fetch(source);
	CARD16 *d = Store(dest);
	while(count--) *d++ = *s++;
}
static void OP_BLTL_FAST(LONG_POINTER source, LONG_POINTER dest, CARDINAL count) {
	for(;;) {
		CARD16 sRun = PageSize - (source % PageSize);
		CARD16 dRun = PageSize - (dest   % PageSize);
		CARD16 run = (sRun < dRun) ? sRun : dRun;
		if (count < run) run = count;

		OP_BLTL_QUICK(source, dest, run);

		source += run;
		count  -= run;
		dest   += run;
		if (count == 0) break;

		if (InterruptThread::isPending()) {
			PC = savedPC;
			PushLong(source);
			Push(count);
			PushLong(dest);
			break;
		}
	}
}
void E_BLTL() {
	LONG_POINTER dest = PopLong();
	CARDINAL count  = Pop();
	LONG_POINTER source = PopLong();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  BLTL  %8X %8X %5d", savedPC, source, dest, count);

	if (count == 0) return;

	CARD32 dStartPage = dest                 / PageSize;
	CARD32 sStartPage = source               / PageSize;
	CARD32 dEndPage   = (dest + count - 1)   / PageSize;
	CARD32 sEndPage   = (source + count - 1) / PageSize;
	if (dStartPage == dEndPage && sStartPage == sEndPage) {
		// Most of BLTL is this case.
		OP_BLTL_QUICK(source, dest, count);
	} else {
		OP_BLTL_FAST(source, dest, count);
	}
}
#else
void E_BLTL() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  BLTL", savedPC);
	for(;;) {
		LONG_POINTER dest = PopLong();
		CARDINAL count = Pop();
		LONG_POINTER source = PopLong();
		if (count == 0) break;
		*Store(dest) = *Fetch(source);
		PushLong(source + 1);
		Push(count - 1);
		PushLong(dest + 1);
		if (InterruptThread::isPending()) {
			PC = savedPC;
			break;
		}
	}
}
#endif


// zBLTC - 0365
#ifdef USE_FAST_BLT
static inline void OP_BLTC_QUICK(POINTER source, POINTER dest, CARDINAL count) {
	// Assume source region stay within one page
	// Assume dest region stay within one page
	CARD16 *s = FetchCode(source); // ReadCode(offset) == *Fech(CB + offset)
	CARD16 *d = StoreMds(dest);
	while(count--) *d++ = *s++;
}
static inline void OP_BLTC_FAST(POINTER source, POINTER dest, CARDINAL count) {
	for(;;) {
		CARD16 sRun = PageSize - ((CodeCache::CB() + source) % PageSize);
		CARD16 dRun = PageSize - (dest          % PageSize);
		CARD16 run = (sRun < dRun) ? sRun : dRun;
		if (count < run) run = count;

		OP_BLTC_QUICK(source, dest, run);

		source += run;
		count  -= run;
		dest   += run;
		if (count == 0) break;

		if (InterruptThread::isPending()) {
			PC = savedPC;
			Push(source);
			Push(count);
			Push(dest);
			break;
		}
	}
}
void E_BLTC() {
	POINTER  dest   = Pop();
	CARDINAL count  = Pop();
	POINTER  source = Pop();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  BLTC      %4X     %4X %5d", savedPC, source, dest, count);

	if (count == 0) return;

	CARD32 dStartPage = dest                                   / PageSize;
	CARD32 sStartPage = (CodeCache::CB() + source)             / PageSize;
	CARD32 dEndPage   = (dest + count - 1)                     / PageSize;
	CARD32 sEndPage   = (CodeCache::CB() + source + count - 1) / PageSize;
	if (dStartPage == dEndPage && sStartPage == sEndPage) {
		// Most of BLTC is this case.
		OP_BLTC_QUICK(source, dest, count);
	} else {
		OP_BLTC_FAST(source, dest, count);
	}
}
#else
void E_BLTC() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  BLTC", savedPC);
	for(;;) {
		POINTER dest = Pop();
		CARDINAL count = Pop();
		POINTER source = Pop();
		if (count == 0) break;
		*StoreMds(dest) = ReadCode(source);
		Push(source + 1);
		Push(count - 1);
		Push(dest + 1);
		if (InterruptThread::isPending()) {
			PC = savedPC;
			break;
		}
	}
}
#endif


// zBLTCL - 0366
#ifdef USE_FAST_BLT
static inline void OP_BLTCL_QUICK(POINTER source, LONG_POINTER dest, CARDINAL count) {
	// Assume source region stay within one page
	// Assume dest region stay within one page
	CARD16 *s = FetchCode(source); // ReadCode(offset) == *Fech(CB + offset)
	CARD16 *d = Store(dest);
	while(count--) *d++ = *s++;
}
static inline void OP_BLTCL_FAST(POINTER source, LONG_POINTER dest, CARDINAL count) {
	for(;;) {
		CARD16 sRun = PageSize - ((CodeCache::CB() + source) % PageSize);
		CARD16 dRun = PageSize - (dest          % PageSize);
		CARD16 run = (sRun < dRun) ? sRun : dRun;
		if (count < run) run = count;

		OP_BLTCL_QUICK(source, dest, run);

		source += run;
		count  -= run;
		dest   += run;
		if (count == 0) break;

		if (InterruptThread::isPending()) {
			PC = savedPC;
			Push(source);
			Push(count);
			PushLong(dest);
			break;
		}
	}
}
void E_BLTCL() {
	LONG_POINTER  dest   = PopLong();
	CARDINAL      count  = Pop();
	POINTER       source = Pop();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  BLTCL     %4X %8X %5d", savedPC, source, dest, count);

	if (count == 0) return;

	CARD32 dStartPage = dest                                   / PageSize;
	CARD32 sStartPage = (CodeCache::CB() + source)             / PageSize;
	CARD32 dEndPage   = (dest + count - 1)                     / PageSize;
	CARD32 sEndPage   = (CodeCache::CB() + source + count - 1) / PageSize;
	if (dStartPage == dEndPage && sStartPage == sEndPage) {
		// Most of BLTCL is this case.
		OP_BLTCL_QUICK(source, dest, count);
	} else {
		OP_BLTCL_FAST(source, dest, count);
	}
}
#else
void E_BLTCL() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  BLTCL", savedPC);
	for(;;) {
		LONG_POINTER dest = PopLong();
		CARDINAL count = Pop();
		POINTER source = Pop();
		if (count == 0) break;
		*Store(dest) = ReadCode(source);
		Push(source + 1);
		Push(count - 1);
		PushLong(dest + 1);
		if (InterruptThread::isPending()) {
			PC = savedPC;
			break;
		}
	}
}
#endif


// aBLTLR - 047
#ifdef USE_FAST_BLT
static inline void OP_BLTLR_QUICK(LONG_POINTER source, LONG_POINTER dest, CARDINAL count) {
	CARD16 *s = Fetch(source + count - 1);
	CARD16 *d = Store(dest   + count - 1);
	while(count--) *d-- = *s--;
}
static inline void OP_BLTLR_FAST(LONG_POINTER source, LONG_POINTER dest, CARDINAL count) {
	for(;;) {
		CARD16 sRun = (source + count - 1) % PageSize;
		CARD16 dRun = (dest   + count - 1) % PageSize;
		CARD16 run = (sRun < dRun) ? sRun : dRun;
		run++;
		if (count < run) run = count;

		// The block below cannot replace with OP_BLTLR_QUICK(source, dest, run);
		// Because value of run and count can be different.
		{
			CARD16 *s = Fetch(source + count - 1);
			CARD16 *d = Store(dest   + count - 1);
			int r = run;
			while(r--) *d-- = *s--;
		}

		count  -= run;
		if (count == 0) break;

		if (InterruptThread::isPending()) {
			PC = savedPC;
			PushLong(source);
			Push(count);
			PushLong(dest);
			break;
		}
	}
}
void E_BLTLR() {
	LONG_POINTER dest = PopLong();
	CARDINAL count  = Pop();
	LONG_POINTER source = PopLong();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  BLTLR %8X %8X %5d", savedPC, source, dest, count);

	if (count == 0) return;

	CARD32 dStartPage = dest                 / PageSize;
	CARD32 sStartPage = source               / PageSize;
	CARD32 dEndPage   = (dest + count - 1)   / PageSize;
	CARD32 sEndPage   = (source + count - 1) / PageSize;
	if (dStartPage == dEndPage && sStartPage == sEndPage) {
		// Most of BLTLR is this case.
		OP_BLTLR_QUICK(source, dest, count);
	} else {
		OP_BLTLR_FAST(source, dest, count);
	}
}
#else
void E_BLTLR() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  BLTLR", savedPC);
	for(;;) {
		LONG_POINTER dest = PopLong();
		CARDINAL count = Pop();
		LONG_POINTER source = PopLong();
		if (count == 0) break;
		count = count - 1;
		*Store(dest + count) = *Fetch(source + count);
		PushLong(source);
		Push(count);
		PushLong(dest);
		if (InterruptThread::isPending()) {
			PC = savedPC;
			break;
		}
	}
}
#endif


// aBLEL - 050
#ifdef USE_FAST_BLT
static inline void OP_BLEL_QUICK(LONG_POINTER ptr1, LONG_POINTER ptr2, CARDINAL count) {
	CARD16 *p1 = Fetch(ptr1);
	CARD16 *p2 = Fetch(ptr2);

	while(count--) {
		if (*p1++ != *p2++) {
			Push(0);
			return;
		}
	}
	Push(1);
}
static inline void OP_BLEL_FAST(LONG_POINTER ptr1, LONG_POINTER ptr2, CARDINAL count) {
	for(;;) {
		CARD16 p1Run = PageSize - (ptr1 % PageSize);
		CARD16 p2Run = PageSize - (ptr2 % PageSize);
		CARD16 run = (p1Run < p2Run) ? p1Run : p2Run;
		if (count < run) run = count;

		CARD32 r = run;
		CARD16 *p1 = Fetch(ptr1);
		CARD16 *p2 = Fetch(ptr2);

		while(r--) {
			if (*p1++ != *p2++) {
				Push(0);
				return;
			}
		}

		ptr1  += run;
		count -= run;
		ptr2  += run;
		if (count == 0) {
			Push(1);
			break;
		}

		if (InterruptThread::isPending()) {
			PC = savedPC;
			PushLong(ptr2);
			Push(count);
			PushLong(ptr1);
			break;
		}
	}
}
void E_BLEL() {
	LONG_POINTER ptr1 = PopLong();
	CARDINAL count = Pop();
	LONG_POINTER ptr2 = PopLong();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  BLEL  %8X %8X %5d", savedPC, ptr1, ptr2, count);

	if (count == 0) {
		Push(1);
		return;
	}

	CARDINAL p1StartPage = ptr1               / PageSize;
	CARDINAL p2StartPage = ptr2               / PageSize;
	CARDINAL p1EndPage   = (ptr1 + count - 1) / PageSize;
	CARDINAL p2EndPage   = (ptr2 + count - 1) / PageSize;

	if (p1StartPage == p1EndPage && p2StartPage == p2EndPage) {
		// Most of BLEL is this case.
		OP_BLEL_QUICK(ptr1, ptr2, count);
	} else {
		OP_BLEL_FAST(ptr1, ptr2, count);
	}
}
#else
void E_BLEL() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  BLEL", savedPC);
	for(;;) {
		LONG_POINTER ptr1 = PopLong();
		CARDINAL count = Pop();
		LONG_POINTER ptr2 = PopLong();
		if (count == 0) {
			Push(1);
			break;
		}
		if (*Fetch(ptr1) != *Fetch(ptr2)) {
			Push(0);
			break;
		}
		PushLong(ptr2 + 1);
		Push(count - 1);
		PushLong(ptr1 + 1);
		if (InterruptThread::isPending()) {
			PC = savedPC;
			break;
		}
	}
}
#endif


// aBLECL - 051
#ifdef USE_FAST_BLT
static inline void OP_BLECL_QUICK(LONG_POINTER ptr, CARDINAL offset, CARDINAL count) {
	CARD16 *o = FetchCode(offset);
	CARD16 *p = Fetch(ptr);

	while(count--) {
		if (*o++ != *p++) {
			Push(0);
			return;
		}
	}
	Push(1);
}
static inline void OP_BLECL_FAST(LONG_POINTER ptr, CARDINAL offset, CARDINAL count) {
	for(;;) {
		CARD16 oRun = PageSize - ((CodeCache::CB() + offset) % PageSize);
		CARD16 pRun = PageSize - (ptr % PageSize);
		CARD16 run = (oRun < pRun) ? oRun : pRun;
		if (count < run) run = count;

		CARD32 r = run;
		CARD16 *o = FetchCode(offset);
		CARD16 *p = Fetch(ptr);

		while(r--) {
			if (*o++ != *p++) {
				Push(0);
				return;
			}
		}

		ptr    += run;
		count  -= run;
		offset += run;
		if (count == 0) {
			Push(1);
			return;
		}

		if (InterruptThread::isPending()) {
			PC = savedPC;
			Push(offset);
			Push(count);
			PushLong(ptr);
			break;
		}
	}
}
void E_BLECL() {
	LONG_POINTER ptr    = PopLong();
	CARDINAL     count  = Pop();
	CARDINAL     offset = Pop();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  BLECL %8X     %4X %5d", savedPC, ptr, offset, count);

	if (count == 0) {
		Push(1);
		return;
	}

	CARDINAL pStartPage = ptr                                    / PageSize;
	CARDINAL cStartPage = (CodeCache::CB() + offset)             / PageSize;
	CARDINAL pEndPage   = (ptr + count - 1)                      / PageSize;
	CARDINAL cEndPage   = (CodeCache::CB() + offset + count - 1) / PageSize;

	if (pStartPage == pEndPage && cStartPage == cEndPage) {
		// Most of BLECL is this case.
		OP_BLECL_QUICK(ptr, offset, count);
	} else {
		OP_BLECL_FAST(ptr, offset, count);
	}
}
#else
void E_BLECL() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  BLECL", savedPC);
	for(;;) {
		LONG_POINTER ptr = PopLong();
		CARDINAL count = Pop();
		CARDINAL offset = Pop();
		if (count == 0) {
			Push(1);
			break;
		}
		if (*Fetch(ptr) != ReadCode(offset)) {
			Push(0);
			break;
		}
		Push(offset + 1);
		Push(count - 1);
		PushLong(ptr + 1);
		if (InterruptThread::isPending()) {
			PC = savedPC;
			break;
		}
	}
}
#endif


// aCKSUM - 052
static CARDINAL Checksum(CARDINAL chksum, CARDINAL data) {
	CARDINAL temp = chksum + data;
	if (temp < chksum) temp = temp + 1;
	if ((CARD16)0x8000 <= temp) {
		temp = temp * 2 + 1;
	} else {
		temp = temp * 2;
	}
	return temp;
}
void E_CKSUM() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  CKSUM", savedPC);
	CARDINAL cksum;
	for(;;) {
		LONG_POINTER source = PopLong();
		CARDINAL count = Pop();
		cksum = Pop();
		if (count == 0) break;
		Push(Checksum(cksum, *Fetch(source)));
		Push(count - 1);
		PushLong(source + 1);
		if (InterruptThread::isPending()) {
			PC = savedPC;
			return;
		}
	}
	if (cksum == (CARDINAL)0xffff) cksum = 0;
	Push(cksum);
}


// aBYTBLT - 055
void E_BYTBLT() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  BYTBLT", savedPC);
	for(;;) {
		CARDINAL     sourceOffset = Pop();
		LONG_POINTER sourceBase   = PopLong();
		CARDINAL     count        = Pop();
		CARDINAL     destOffset   = Pop();
		LONG_POINTER destBase     = PopLong();
		if (count == 0) break;

		sourceBase   = sourceBase + sourceOffset / 2;
		sourceOffset = sourceOffset % 2;
		destBase     = destBase + destOffset / 2;
		destOffset   = destOffset % 2;
		StoreByte(destBase, destOffset, FetchByte(sourceBase, sourceOffset));
		if (sourceOffset == 1) {
			sourceBase = sourceBase + 1;
			sourceOffset = 0;
		} else {
			sourceOffset = 1;
		}
		if (destOffset == 1) {
			destBase = destBase + 1;
			destOffset = 0;
		} else {
			destOffset = 1;
		}

		PushLong(destBase);
		Push(destOffset);
		Push(count - 1);
		PushLong(sourceBase);
		Push(sourceOffset);
		if (InterruptThread::isPending()) {
			PC = savedPC;
			break;
		}
	}
}


// aBYTBLTR - 056
void E_BYTBLTR() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  BYTBLTR", savedPC);
	for(;;) {
		CARDINAL     sourceOffset = Pop();
		LONG_POINTER sourceBase   = PopLong();
		CARDINAL     count        = Pop();
		CARDINAL     destOffset   = Pop();
		LONG_POINTER destBase     = PopLong();
		if (count == 0) break;

		CARD32 d = (CARD32)destOffset   + (CARD32)count - 1;
		CARD32 s = (CARD32)sourceOffset + (CARD32)count - 1;
		StoreByte(destBase, d, FetchByte(sourceBase, s));

		PushLong(destBase);
		Push(destOffset);
		Push(count - 1);
		PushLong(sourceBase);
		Push(sourceOffset);
		if (InterruptThread::isPending()) {
			PC = savedPC;
			break;
		}
	}
}

