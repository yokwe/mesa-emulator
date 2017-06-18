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


//
// Opcode_block.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("block");

#include "../util/Debug.h"

#include "../mesa/MesaBasic.h"
#include "../mesa/Memory.h"
#include "../mesa/Function.h"

#include "Opcode.h"

#define USE_FAST_BLT

static const int    DEBUG_FORCE_ABORT = 0;

static const CARD32 MASK_OFFSET = PageSize - 1;

__attribute__((always_inline)) static inline void BLT(CARD32& s, CARD32& d, CARD32& c, CARD32& r) {
	CARD16 *sp = Fetch(s);
	CARD16 *dp = Store(d);
	// NO PAGE FAULT AFTER THIS

	CARD32 sr = PageSize - (s & MASK_OFFSET);
	CARD32 dr = PageSize - (d & MASK_OFFSET);
	CARD32 run = (sr < dr) ? sr : dr;
	if (c < run) run = c;

	s += run;
	d += run;
	c -= run;
	r =  run;

	while(run--) *dp++ = *sp++;
}

__attribute__((always_inline)) static inline void BLTR(CARD32& s, CARD32& d, CARD32& c, CARD32& r) {
	CARD16 *sp = Fetch(s);
	CARD16 *dp = Store(d);
	// NO PAGE FAULT AFTER THIS

	CARD32 sr = (s & MASK_OFFSET) + 1;
	CARD32 dr = (d & MASK_OFFSET) + 1;
	CARD32 run = (sr < dr) ? sr : dr;
	if (c < run) run = c;

	s -= run;
	d -= run;
	c -= run;
	r =  run;

	while(run--) *dp-- = *sp--;
}

__attribute__((always_inline)) static inline CARD32 BLE(CARD32& a, CARD32& b, CARD32& c, CARD32& r) {
	CARD16 *ap = Fetch(a);
	CARD16 *bp = Fetch(b);
	// NO PAGE FAULT AFTER THIS

	CARD32 ar = PageSize - (a & MASK_OFFSET);
	CARD32 br = PageSize - (b & MASK_OFFSET);
	CARD32 run = (ar < br) ? ar : br;
	if (c < run) run = c;

	a += run;
	b += run;
	c -= run;
	r =  run;

	while(run--) {
		if (*ap++ != *bp++) return 0; // FALSE - NOT EQUAL
	}
	return 1; // TRUE - EQUAL
}

// zBLT - 0363
#ifdef USE_FAST_BLT
void E_BLT() {
	POINTER  dest   = Pop();
	CARDINAL count  = Pop();
	POINTER  source = Pop();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  BLT       %8X %8X %5d", savedPC, source, dest, count);

	if (count == 0) return;

	CARD32 d = LengthenPointer(dest);
	CARD32 s = LengthenPointer(source);
	CARD32 c = count;
	CARD32 r;

	for(;;) {
		BLT(s, d, c, r);
		if (c == 0) break;

		// Update stack in case of PageFault
		source += r;
		dest   += r;
		count  -= r;

		Push(source);
		Push(count);
		Push(dest);
		Discard();
		Discard();
		Discard();

		if (DEBUG_FORCE_ABORT) {
			PC = savedPC;
			SP = savedSP;
			ERROR_Abort();
		}
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
	}
}
#endif


// zBLTL - 0364
#ifdef USE_FAST_BLT
void E_BLTL() {
	LONG_POINTER dest   = PopLong();
	CARDINAL     count  = Pop();
	LONG_POINTER source = PopLong();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  BLTL      %8X %8X %5d", savedPC, source, dest, count);

	if (count == 0) return;

	CARD32 d = dest;
	CARD32 s = source;
	CARD32 c = count;
	CARD32 r;

	for(;;) {
		BLT(s, d, c, r);
		if (c == 0) break;

		// Update stack in case of PageFault
		source += r;
		dest   += r;
		count  -= r;

		PushLong(source);
		Push(count);
		PushLong(dest);
		Discard(); Discard();
		Discard();
		Discard(); Discard();

		if (DEBUG_FORCE_ABORT) {
			PC = savedPC;
			SP = savedSP;
			ERROR_Abort();
		}
	}
}
#else
void E_BLTL() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  BLTL", savedPC);
	for(;;) {
		LONG_POINTER dest   = PopLong();
		CARDINAL     count  = Pop();
		LONG_POINTER source = PopLong();
		if (count == 0) break;

		*Store(dest) = *Fetch(source);
		PushLong(source + 1);
		Push(count - 1);
		PushLong(dest + 1);
	}
}
#endif


// zBLTC - 0365
#ifdef USE_FAST_BLT
void E_BLTC() {
	POINTER  dest   = Pop();
	CARDINAL count  = Pop();
	POINTER  source = Pop();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  BLTC      %8X %8X %5d", savedPC, source, dest, count);

	if (count == 0) return;

	CARD32 d = LengthenPointer(dest);
	CARD32 s = CodeCache::CB() + source;
	CARD32 c = count;
	CARD32 r;

	for(;;) {
		BLT(s, d, c, r);
		if (c == 0) break;

		// Update stack in case of PageFault
		source += r;
		dest   += r;
		count  -= r;

		Push(source);
		Push(count);
		Push(dest);
		Discard();
		Discard();
		Discard();

		if (DEBUG_FORCE_ABORT) {
			PC = savedPC;
			SP = savedSP;
			ERROR_Abort();
		}
	}
}
#else
void E_BLTC() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  BLTC", savedPC);
	for(;;) {
		POINTER  dest   = Pop();
		CARDINAL count  = Pop();
		POINTER  source = Pop();
		if (count == 0) break;
		*StoreMds(dest) = ReadCode(source);
		Push(source + 1);
		Push(count  - 1);
		Push(dest   + 1);
	}
}
#endif


// zBLTCL - 0366
#ifdef USE_FAST_BLT
void E_BLTCL() {
	LONG_POINTER  dest   = PopLong();
	CARDINAL      count  = Pop();
	POINTER       source = Pop();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  BLTCL     %4X %8X %5d", savedPC, source, dest, count);

	if (count == 0) return;

	CARD32 d = dest;
	CARD32 s = CodeCache::CB() + source;
	CARD32 c = count;
	CARD32 r;

	for(;;) {
		BLT(s, d, c, r);
		if (c == 0) break;

		// Update stack in case of PageFault
		source += r;
		dest   += r;
		count  -= r;

		Push(source);
		Push(count);
		PushLong(dest);
		Discard();
		Discard();
		Discard(); Discard();

		if (DEBUG_FORCE_ABORT) {
			PC = savedPC;
			SP = savedSP;
			ERROR_Abort();
		}
	}
}
#else
void E_BLTCL() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  BLTCL", savedPC);
	for(;;) {
		LONG_POINTER dest   = PopLong();
		CARDINAL     count  = Pop();
		POINTER      source = Pop();
		if (count == 0) break;
		*Store(dest) = ReadCode(source);
		Push(source + 1);
		Push(count - 1);
		PushLong(dest + 1);
	}
}
#endif


// aBLTLR - 047
#ifdef USE_FAST_BLT
void E_BLTLR() {
	LONG_POINTER dest   = PopLong();
	CARDINAL     count  = Pop();
	LONG_POINTER source = PopLong();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  BLTLR     %8X %8X %5d", savedPC, source, dest, count);

	if (count == 0) return;

	CARD32 d = dest   + count - 1;
	CARD32 s = source + count - 1;
	CARD32 c = count;
	CARD32 r;

	for(;;) {
		BLTR(s, d, c, r);
		if (c == 0) break;

		// Update stack in case of PageFault
		count  -= r;

		PushLong(source);
		Push(count);
		PushLong(dest);
		Discard(); Discard();
		Discard();
		Discard(); Discard();

		if (DEBUG_FORCE_ABORT) {
			PC = savedPC;
			SP = savedSP;
			ERROR_Abort();
		}
	}
}
#else
void E_BLTLR() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  BLTLR", savedPC);
	for(;;) {
		LONG_POINTER dest   = PopLong();
		CARDINAL     count  = Pop();
		LONG_POINTER source = PopLong();
		if (count == 0) break;
		count = count - 1;
		*Store(dest + count) = *Fetch(source + count);
		PushLong(source);
		Push(count);
		PushLong(dest);
	}
}
#endif


// aBLEL - 050
#ifdef USE_FAST_BLT
void E_BLEL() {
	LONG_POINTER ptr1  = PopLong();
	CARDINAL     count = Pop();
	LONG_POINTER ptr2  = PopLong();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  BLEL  %8X %8X %5d", savedPC, ptr1, ptr2, count);

	if (count == 0) {
		Push(1); // TRUE - EQUAL
		return;
	}

	CARD32 a = ptr1;
	CARD32 b = ptr2;
	CARD32 c = count;
	CARD32 r;
	CARD32 result;

	for(;;) {
		result = BLE(a, b, c, r);
		if (!result) break; // break if result is FALSE - NOT EQUAL
		if (c == 0)  break; // break reach to end. TRUE - EQUAL

		// Update stack in case of PageFault
		ptr1  += r;
		ptr2  += r;
		count -= r;

		PushLong(ptr2);
		Push(count);
		PushLong(ptr1);
		Discard(); Discard();
		Discard();
		Discard(); Discard();

		if (DEBUG_FORCE_ABORT) {
			PC = savedPC;
			SP = savedSP;
			ERROR_Abort();
		}
	}
	Push(result);
}
#else
void E_BLEL() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  BLEL", savedPC);
	for(;;) {
		LONG_POINTER ptr1  = PopLong();
		CARDINAL     count = Pop();
		LONG_POINTER ptr2  = PopLong();
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
	}
}
#endif


// aBLECL - 051
#ifdef USE_FAST_BLT
void E_BLECL() {
	LONG_POINTER ptr    = PopLong();
	CARDINAL     count  = Pop();
	CARDINAL     offset = Pop();
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  BLECL %8X     %4X %5d", savedPC, ptr, offset, count);

	if (count == 0) {
		Push(1);
		return;
	}

	CARD32 a = ptr;
	CARD32 b = CodeCache::CB() + offset;
	CARD32 c = count;
	CARD32 r;
	CARD32 result;

	for(;;) {
		result = BLE(a, b, c, r);
		if (!result) break; // break if result is FALSE - NOT EQUAL
		if (c == 0)  break; // break reach to end. TRUE - EQUAL

		// Update stack in case of PageFault
		ptr    += r;
		offset += r;
		count  -= r;

		Push(offset);
		Push(count);
		PushLong(ptr);
		Discard();
		Discard();
		Discard(); Discard();

		if (DEBUG_FORCE_ABORT) {
			PC = savedPC;
			SP = savedSP;
			ERROR_Abort();
		}
	}
	Push(result);
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
	}
}
#endif


// aCKSUM - 052
__attribute__((always_inline)) static inline CARDINAL Checksum(CARDINAL chksum, CARDINAL data) {
	CARDINAL temp = chksum + data;
	if (temp < chksum) temp = temp + 1;
	if ((CARD16)0x8000 <= temp) {
		temp = temp * 2 + 1;
	} else {
		temp = temp * 2;
	}
	return temp;
}
#ifdef USE_FAST_BLT
void E_CKSUM() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  CKSUM", savedPC);
	LONG_POINTER source = PopLong();
	CARDINAL     count  = Pop();
	CARDINAL     cksum  = Pop();

	// No need to maintain stack, because count is small.
	CARD16* p = Fetch(source);
	CARD32  c = count;
	for(;;) {
		if (c == 0) break;
		cksum = Checksum(cksum, *p++);
		c--;
	}
	if (cksum == (CARDINAL)0xffff) cksum = 0;
	Push(cksum);
}
#else
void E_CKSUM() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  CKSUM", savedPC);
	for(;;) {
		LONG_POINTER source = PopLong();
		CARDINAL     count = Pop();
		CARDINAL     cksum = Pop();
		if (count == 0) {
			if (cksum == (CARDINAL)0xffff) cksum = 0;
			Push(cksum);
			break;
		}
		cksum = Checksum(cksum, *Fetch(source));
		Push(cksum);
		Push(count - 1);
		PushLong(source + 1);
	}
}
#endif


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

		StoreByte(destBase, destOffset, FetchByte(sourceBase, sourceOffset));

		PushLong(destBase);
		Push(destOffset + 1);
		Push(count - 1);
		PushLong(sourceBase);
		Push(sourceOffset + 1);
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
	}
}

