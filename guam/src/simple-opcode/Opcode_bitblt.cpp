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
// Opcode_bitblt.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("bitblt");

#include "../util/Debug.h"
#include "../util/GuiOp.h"
#include "../util/Perf.h"

#include "../mesa/MesaBasic.h"
#include "../mesa/Memory.h"

#include "Opcode.h"

#include <memory>

class MonoBlt {
public:
	static MonoBlt* getInstance(ColorBlt::ColorBltTable& arg);

	virtual void process() = 0;

	virtual ~MonoBlt() {}

	static inline void FetchColorBltTable(POINTER ptr, ColorBlt::ColorBltTable* arg) {
		if (ptr & 0x0f) ERROR(); // ptr must be 16 word aligned
		CARD16* p = (CARD16*)arg;
		for(CARD32 i = 0; i < SIZE(*arg); i++) {
			p[i] = *FetchMds(ptr + i);
		}
	}

	static inline void FetchBitBltTable(POINTER ptr, ColorBlt::ColorBltTable* arg) {
		if (ptr & 0x0f) ERROR(); // ptr must be 16 word aligned
		BitBltArg bltarg;
		CARD16* p = (CARD16*)&bltarg;
		for(CARD32 i = 0; i < SIZE(bltarg); i++) {
			p[i] = *FetchMds(ptr + i);
		}

		// convert from BitBltArg to ColorBltTable
		arg->dst.word  = bltarg.dst.word;
		arg->dst.pixel = bltarg.dst.bit;

		arg->dstPpl    = bltarg.dstBpl;

		arg->src.word  = bltarg.src.word;
		arg->src.pixel = bltarg.src.bit;

		if (bltarg.flags.gray) {
			GrayParm grayParm = {(CARD16)bltarg.srcBpl};
			arg->pattern.unpacked       = 1;
			arg->pattern.yOffset        = grayParm.yOffset;
			arg->pattern.widthMinusOne  = grayParm.widthMinusOne;
			arg->pattern.heightMinusOne = grayParm.heightMinusOne;
		} else {
			arg->srcPpl = bltarg.srcBpl;
		}

		arg->width  = bltarg.width;
		arg->height = bltarg.height;

		arg->flags.direction = bltarg.flags.direction;

		CARD32 base = Memory::getDisplayVirtualPage() * PageSize;
		CARD32 size = Memory::getDisplayPageSize()    * PageSize;
		arg->flags.srcType   = (base <= bltarg.src.word && bltarg.src.word < (base + size));
		arg->flags.dstType   = (base <= bltarg.dst.word && bltarg.dst.word < (base + size));

		arg->flags.pattern   = bltarg.flags.gray;
		arg->flags.srcFunc   = bltarg.flags.srcFunc;

		switch (bltarg.flags.dstFunc) {
		case DF_null:
			arg->flags.dstFunc   = ColorBlt::DF_src;
			break;
		case DF_and:
			arg->flags.dstFunc   = ColorBlt::DF_srcIfDstNot0;
			break;
		case DF_or:
			arg->flags.dstFunc   = ColorBlt::DF_srcIfDst0;
			break;
		case DF_xor:
			arg->flags.dstFunc   = ColorBlt::DF_srcXorDst;
			break;
		}

		arg->flags.reserved  = 0;

		arg->colorMapping.color[0] = 0;
		arg->colorMapping.color[1] = 0;
	}

	static inline void Bump(ColorBlt::Address& address, int offset) {
		offset += address.pixel;
		// Don't use like  offset / WordSize. Cannot get correct value if offset has minus number.
		address.word += LongArithShift(offset, -Environment::logBitsPerWord);
		// Don't use like  offset % WordSize. Cannot get correct value if offset has minus number.
		address.pixel  = offset & (Environment::bitsPerWord - 1);
	}

	class MemoryCache {
	public:
		MemoryCache() {
			readPtr  = 0;
			writePtr = 0;
			word     = 0;
			address  = 0;
		}
		inline CARD16 read(CARD32 newPtr) {
			total++;
			if (newPtr == readPtr) {
				hit++;
				return word;
			}
			//
			readPtr  = newPtr;
			writePtr = 0;
			address  = Fetch(newPtr);
			word     = *address;
			return word;
		}
		inline CARD16 write(CARD32 newPtr) {
			total++;
			if (newPtr == writePtr) {
				hit++;
				return word;
			}
			//
			readPtr = writePtr = newPtr;
			address = Store(newPtr);
			word    = *address;
			return word;
		}
		inline void write(CARD32 newPtr, CARD16 newWord) {
			total++;
			if (newPtr == writePtr) {
				hit++;
				*address = newWord;
				word = newWord;
				return;
			}
			//
			readPtr = writePtr = newPtr;
			address = Store(newPtr);
		   *address = word = newWord;
		}
		static void stats() {
			logger.debug("MemoryCache stats   hit ratio is %5.2f%%   hit = %d  total = %d", (hit * 1.0 / total) * 100.0, hit, total);
		}
	private:
		CARD32  readPtr;
		CARD32  writePtr;
		CARD16 *address;
		CARD16  word;

		static int hit;
		static int total;
	};


protected:
	int width;
	int height;
	ColorBlt::Address src;
	ColorBlt::Address dst;
	int srcType;
	int dstType;
	int srcFunc;
	int dstFunc;

	MemoryCache cacheSrc;
	MemoryCache cacheDst;


	MonoBlt(ColorBlt::ColorBltTable& arg) {
		width = arg.width;
		height = arg.height;
		src = arg.src;
		dst = arg.dst;
		srcType = arg.flags.srcType;
		dstType = arg.flags.dstType;
		srcFunc = arg.flags.srcFunc;
		dstFunc = arg.flags.dstFunc;
	}


	static inline CARD16 Function_mono(int srcFunc, int dstFunc, CARD16 s, CARD16 d) {
		if (srcFunc == ColorBlt::SF_complement) s = ~s;

		switch(dstFunc) {
		case ColorBlt::DF_src:
			return s;
		case ColorBlt::DF_srcIfDstLE1:
		case ColorBlt::DF_srcIf0:
		case ColorBlt::DF_srcIfDstNot0:
			return d & s;
		case ColorBlt::DF_srcIfNot0:
		case ColorBlt::DF_srcIfDst0:
			return d | s;
		case ColorBlt::DF_pixelXor:
		case ColorBlt::DF_srcXorDst:
			return d ^ s;
		}
		ERROR();
		return 0;
	}

	inline CARD16 ReadPixel_pat(MemoryCache& cache, ColorBlt::Address address, int offset, CARD16 type, CARD16 unpacked) {
		Bump(address, offset);
		int shift = WordSize - (address.pixel + 1);
		CARD16 word = cache.read(address.word);
		if (word && unpacked) word = 0xffff;
		if (type == ColorBlt::PT_display) word = qToBigEndian(word); // mesa is big endian
		return (word >> shift) & 1;
	}
	inline CARD16 ReadPixel_bit(MemoryCache& cache, ColorBlt::Address address, int offset, CARD16 type) {
		Bump(address, offset);
		int shift = WordSize - (address.pixel + 1);
		CARD16 word = cache.read(address.word);
		if (type == ColorBlt::PT_display) word = qToBigEndian(word); // mesa is big endian
		return (word >> shift) & 1;
	}
	inline void WritePixel(MemoryCache& cache, ColorBlt::Address address, int offset, CARD16 bit, CARD16 type) {
		Bump(address, offset);

		CARD16 word = cache.write(address.word);
		if (type == ColorBlt::PT_display) word = qToBigEndian(word); // mesa is big endian

		int shift = WordSize - (address.pixel + 1);
		CARD16 mask = 1 << shift;

		//  = WriteField(word, spec, bit
		CARD16 result = (word & ~mask) | ((bit << shift) & mask);

		//logger.debug("%s  data  %d  %2d   %04X  %04X", __FUNCTION__, bit, offset, word, result);
		if (type == ColorBlt::PT_display) result = qFromBigEndian(result); // i386 is little endian
		cache.write(address.word, result);
	}
	inline void WritePixel_zero(MemoryCache& cache, ColorBlt::Address address, int offset, CARD16 type) {
		Bump(address, offset);

		CARD16 word = cache.write(address.word);
		if (type == ColorBlt::PT_display) word = qToBigEndian(word); // mesa is big endian

		int shift = WordSize - (address.pixel + 1);
		CARD16 mask = ~(1 << shift);
		CARD16 result = word & mask;

		//logger.debug("%s  data  %d  %2d   %04X  %04X", __FUNCTION__, bit, offset, word, result);
		if (type == ColorBlt::PT_display) result = qFromBigEndian(result); // i386 is little endian
		cache.write(address.word, result);
	}
	inline void WritePixel_one(MemoryCache& cache, ColorBlt::Address address, int offset, CARD16 type) {
		Bump(address, offset);

		CARD16 word = cache.write(address.word);
		if (type == ColorBlt::PT_display) word = qToBigEndian(word); // mesa is big endian

		int shift = WordSize - (address.pixel + 1);
		CARD16 mask = 1 << shift;
		CARD16 result = word | mask;

		//logger.debug("%s  data  %d  %2d   %04X  %04X", __FUNCTION__, bit, offset, word, result);
		if (type == ColorBlt::PT_display) result = qFromBigEndian(result); // i386 is little endian
		cache.write(address.word, result);
	}
	inline void WritePixel_not(MemoryCache& cache, ColorBlt::Address address, int offset, CARD16 type) {
		Bump(address, offset);

		CARD16 word = cache.write(address.word);
		if (type == ColorBlt::PT_display) word = qToBigEndian(word); // mesa is big endian

		int shift = WordSize - (address.pixel + 1);
		CARD16 mask = 1 << shift;
		CARD16 result = word ^ mask;

		//logger.debug("%s  data  %d  %2d   %04X  %04X", __FUNCTION__, bit, offset, word, result);
		if (type == ColorBlt::PT_display) result = qFromBigEndian(result); // i386 is little endian
		cache.write(address.word, result);
	}
};

int MonoBlt::MemoryCache::hit = 0;
int MonoBlt::MemoryCache::total = 0;
void MonoBlt_MemoryCache_stats() {
	if (!PERF_ENABLE) return;
	MonoBlt::MemoryCache::stats();
}


static int count_MonoBlt_bit          = 0;
static int count_MonoBlt_pat          = 0;
static int count_MonoBlt_pat_word     = 0;
static int count_MonoBlt_pat_0000     = 0;
static int count_MonoBlt_pat_0000_src = 0;
static int count_MonoBlt_pat_ffff     = 0;
static int count_MonoBlt_pat_ffff_src = 0;
static int count_MonoBlt_pat_ffff_xor = 0;
void MonoBlt_stats() {
	if (!PERF_ENABLE) return;
	logger.debug("MonoBlt stats   bit          = %8d", count_MonoBlt_bit);
	logger.debug("MonoBlt stats   pat          = %8d", count_MonoBlt_pat);
	logger.debug("MonoBlt stats   pat_word     = %8d", count_MonoBlt_pat_word);
	logger.debug("MonoBlt stats   pat_0000     = %8d", count_MonoBlt_pat_0000);
	logger.debug("MonoBlt stats   pat_0000_src = %8d", count_MonoBlt_pat_0000_src);
	logger.debug("MonoBlt stats   pat_ffff     = %8d", count_MonoBlt_pat_ffff);
	logger.debug("MonoBlt stats   pat_ffff_src = %8d", count_MonoBlt_pat_ffff_src);
	logger.debug("MonoBlt stats   pat_ffff_xor = %8d", count_MonoBlt_pat_ffff_xor);
}


class MonoBlt_pat_0000_src : public MonoBlt {
public:
	static MonoBlt* getInstance(ColorBlt::ColorBltTable& arg) {
		return new MonoBlt_pat_0000_src(arg);
	}

	void process() {
		for(int line = 0; line < height; line++) {
			// BitBltItem
			for(int offset = 0; offset < width; offset++) {
				WritePixel_zero(cacheDst, dst, offset, dstType);
			}
			Bump(dst, dstPpl);
		}
	}

private:
	int dstPpl;

	MonoBlt_pat_0000_src(ColorBlt::ColorBltTable& arg) : MonoBlt(arg) {
		if (arg.dstPpl == 0 || 32767 < width || 32767 < height) ERROR();
		if (arg.pattern.widthMinusOne || arg.flags.direction != DI_forward || arg.dstPpl < 0) ERROR();
		//
		if (arg.pattern.yOffset) ERROR();
		// arg.src.pixel is not used in this method. so ignore this check
		//if (arg.src.pixel) ERROR();

		dstPpl = arg.dstPpl;
		if (PERF_ENABLE) count_MonoBlt_pat_0000_src++;
	}
};

class MonoBlt_pat_0000 : public MonoBlt {
public:
	static MonoBlt* getInstance(ColorBlt::ColorBltTable& arg) {
		if (arg.flags.srcFunc == ColorBlt::SF_null && arg.flags.dstFunc == ColorBlt::DF_src) return MonoBlt_pat_0000_src::getInstance(arg);
		return new MonoBlt_pat_0000(arg);
	}

	void process() {
		for(int line = 0; line < height; line++) {
			// BitBltItem
			for(int offset = 0; offset < width; offset++) {
				CARD16 d = ReadPixel_bit(cacheDst, dst, offset, dstType);

				WritePixel(cacheDst, dst, offset, Function_mono(srcFunc, dstFunc, 0, d), dstType);
			}
			Bump(dst, dstPpl);
		}
	}

private:
	int dstPpl;

	MonoBlt_pat_0000(ColorBlt::ColorBltTable& arg) : MonoBlt(arg) {
		if (arg.dstPpl == 0 || 32767 < width || 32767 < height) ERROR();
		if (arg.pattern.widthMinusOne || arg.flags.direction != DI_forward || arg.dstPpl < 0) ERROR();
		//
		if (arg.pattern.yOffset || arg.src.pixel) ERROR();

		dstPpl = arg.dstPpl;
		if (PERF_ENABLE) count_MonoBlt_pat_0000++;
	}
};

class MonoBlt_pat_ffff_src : public MonoBlt {
public:
	static MonoBlt* getInstance(ColorBlt::ColorBltTable& arg) {
		return new MonoBlt_pat_ffff_src(arg);
	}

	void process() {
		for(int line = 0; line < height; line++) {
			// BitBltItem
			for(int offset = 0; offset < width; offset++) {
				WritePixel_one(cacheDst, dst, offset, dstType);
			}
			Bump(dst, dstPpl);
		}
	}

private:
	int dstPpl;

	MonoBlt_pat_ffff_src(ColorBlt::ColorBltTable& arg) : MonoBlt(arg) {
		if (arg.dstPpl == 0 || 32767 < width || 32767 < height) ERROR();
		if (arg.pattern.widthMinusOne || arg.flags.direction != DI_forward || arg.dstPpl < 0) ERROR();
		//
		if (arg.pattern.yOffset) ERROR();
		// arg.src.pixel is not used in this method. so ignore this check
		//if (arg.src.pixel) ERROR();

		dstPpl = arg.dstPpl;
		if (PERF_ENABLE) count_MonoBlt_pat_ffff_src++;
	}
};

class MonoBlt_pat_ffff_xor : public MonoBlt {
public:
	static MonoBlt* getInstance(ColorBlt::ColorBltTable& arg) {
		return new MonoBlt_pat_ffff_xor(arg);
	}

	void process() {
		for(int line = 0; line < height; line++) {
			// BitBltItem
			for(int offset = 0; offset < width; offset++) {
				WritePixel_not(cacheDst, dst, offset, dstType);
			}
			Bump(dst, dstPpl);
		}
	}

private:
	int dstPpl;

	MonoBlt_pat_ffff_xor(ColorBlt::ColorBltTable& arg) : MonoBlt(arg) {
		if (arg.dstPpl == 0 || 32767 < width || 32767 < height) ERROR();
		if (arg.pattern.widthMinusOne || arg.flags.direction != DI_forward || arg.dstPpl < 0) ERROR();
		//
		if (arg.pattern.yOffset) ERROR();
		// arg.src.pixel is not used in this method. so ignore this check
		//if (arg.src.pixel) ERROR();

		dstPpl = arg.dstPpl;
		if (PERF_ENABLE) count_MonoBlt_pat_ffff_xor++;
	}
};

class MonoBlt_pat_ffff : public MonoBlt {
public:
	static MonoBlt* getInstance(ColorBlt::ColorBltTable& arg) {
		if (arg.flags.srcFunc == ColorBlt::SF_null && arg.flags.dstFunc == ColorBlt::DF_src) return MonoBlt_pat_ffff_src::getInstance(arg);
		if (arg.flags.srcFunc == ColorBlt::SF_null && arg.flags.dstFunc == ColorBlt::DF_srcXorDst) return MonoBlt_pat_ffff_xor::getInstance(arg);
		return new MonoBlt_pat_ffff(arg);
	}

	void process() {
		for(int line = 0; line < height; line++) {
			// BitBltItem
			for(int offset = 0; offset < width; offset++) {
				CARD16 d = ReadPixel_bit(cacheDst, dst, offset, dstType);

				WritePixel(cacheDst, dst, offset, Function_mono(srcFunc, dstFunc, 1, d), dstType);
			}
			Bump(dst, dstPpl);
		}
	}

private:
	int dstPpl;

	MonoBlt_pat_ffff(ColorBlt::ColorBltTable& arg) : MonoBlt(arg) {
		if (arg.dstPpl == 0 || 32767 < width || 32767 < height) ERROR();
		if (arg.pattern.widthMinusOne || arg.flags.direction != DI_forward || arg.dstPpl < 0) ERROR();
		//
		if (arg.pattern.yOffset || arg.src.pixel) ERROR();

		dstPpl = arg.dstPpl;
		if (PERF_ENABLE) count_MonoBlt_pat_ffff++;
		//logger.debug("%s  srcFunc = %d  dstFunc = %d", __FUNCTION__, arg.flags.srcFunc, arg.flags.dstFunc);
	}
};

class MonoBlt_pat_word : public MonoBlt {
public:
	static MonoBlt* getInstance(ColorBlt::ColorBltTable& arg) {
		CARD16 word = *Fetch(arg.src.word);
		if (arg.pattern.unpacked && word) word = (CARD16)0xffff;
		//
		if (word == 0x0000) return MonoBlt_pat_0000::getInstance(arg);
		if (word == 0xffff) return MonoBlt_pat_ffff::getInstance(arg);

		return new MonoBlt_pat_word(arg, word);
	}

	void process() {
		for(int line = 0; line < height; line++) {
			// BitBltItem
			int shift = WordSize - 1;
			for(int offset = 0; offset < width; offset++) {
				CARD16 s = (word >> shift) & 1;
				CARD16 d = ReadPixel_bit(cacheDst, dst, offset, dstType);

				WritePixel(cacheDst, dst, offset, Function_mono(srcFunc, dstFunc, s, d), dstType);
				shift = (shift == 0) ? WordSize - 1 : shift - 1;
			}
			Bump(dst, dstPpl);
		}
	}

private:
	int dstPpl;
	CARD16 word;

	MonoBlt_pat_word(ColorBlt::ColorBltTable& arg, CARD16 word_) : MonoBlt(arg) {
		if (arg.dstPpl == 0 || 32767 < width || 32767 < height) ERROR();
		if (arg.pattern.widthMinusOne || arg.flags.direction != DI_forward || arg.dstPpl < 0) ERROR();
		//
		if (arg.pattern.yOffset || arg.src.pixel) ERROR();

		dstPpl = arg.dstPpl;
		word = word_;
		//
		if (PERF_ENABLE) count_MonoBlt_pat_word++;
	}
};

class MonoBlt_pat : public MonoBlt {
public:
	static MonoBlt* getInstance(ColorBlt::ColorBltTable& arg) {
		if (arg.pattern.heightMinusOne == 0 && arg.pattern.widthMinusOne == 0) return MonoBlt_pat_word::getInstance(arg);
		return new MonoBlt_pat(arg);
	}

	void process() {
		for(int line = 0; line < height; line++) {
			{
				// BitBltItem
				for(int offset = 0; offset < width; offset++) {
					int pos = ((offset + src.pixel) % grayWidth) - src.pixel;
					CARD16 s = ReadPixel_pat(cacheSrc, src, pos,    srcType, unpacked);
					CARD16 d = ReadPixel_bit(cacheDst, dst, offset, dstType);

					WritePixel(cacheDst, dst, offset, Function_mono(srcFunc, dstFunc, s, d), dstType);
				}
			}

			int offset = ((line % (heightMinusOne + 1)) == lastGray) ? grayBump : grayWidth;
			Bump(src, offset);
			Bump(dst, dstPpl);
		}
	}

private:
	int grayWidth;
	int grayBump;
	int lastGray;
	int unpacked;
	int heightMinusOne;
	int dstPpl;

	MonoBlt_pat(ColorBlt::ColorBltTable& arg) : MonoBlt(arg) {
		if (arg.dstPpl == 0 || 32767 < width || 32767 < height) ERROR();
		if (arg.pattern.widthMinusOne || arg.flags.direction != DI_forward || arg.dstPpl < 0) ERROR();

		grayWidth = (INT16)((arg.pattern.widthMinusOne + 1) * WordSize);
		grayBump = -grayWidth * arg.pattern.heightMinusOne;

		// ComputeDirection
		lastGray = arg.pattern.heightMinusOne - arg.pattern.yOffset;

		unpacked = arg.pattern.unpacked;
		heightMinusOne = arg.pattern.heightMinusOne;
		dstPpl = arg.dstPpl;
		//
		if (PERF_ENABLE) count_MonoBlt_pat++;
	}
};



class MonoBlt_bit : public MonoBlt {
public:
	static MonoBlt* getInstance(ColorBlt::ColorBltTable& arg) {
		return new MonoBlt_bit(arg);
	}

	void process() {
		int line = line_ini;

		for(;;) {
			if (line < 0 || height <= line) break;
			{
				// BitBltItem
				int offset = offset_ini;
				for(int i = 0; i < width; i++) {
					int pos = offset;
					CARD16 s = ReadPixel_bit(cacheSrc, src, pos,    srcType);
					CARD16 d = ReadPixel_bit(cacheDst, dst, offset, dstType);

					WritePixel(cacheDst, dst, offset, Function_mono(srcFunc, dstFunc, s, d), dstType);
					offset += offset_inc;
				}
			}

			Bump(src, bumpSrc);
			Bump(dst, bumpDst);
			line += line_inc;
		}
	}

private:
	int line_ini;
	int line_inc;
	int offset_ini;
	int offset_inc;
	int bumpSrc;
	int bumpDst;

	MonoBlt_bit(ColorBlt::ColorBltTable& arg) : MonoBlt(arg) {
//		if (arg.srcPpl == 0 || arg.dstPpl == 0 || 32767 < arg.width || 32767 < arg.height) ERROR();
//		if (arg.srcPpl == 0) ERROR();
//		if (arg.dstPpl == 0) ERROR();
		if (32767 < arg.width) ERROR();
		if (32767 < arg.height) ERROR();

		if (arg.flags.direction == DI_forward  && (arg.srcPpl < 0 || arg.dstPpl < 0)) ERROR();
		if (arg.flags.direction == DI_backward && (arg.srcPpl < 0 || arg.dstPpl < 0)) ERROR();

		line_ini   = (arg.flags.direction == DI_forward) ? 0 : arg.height - 1;
		line_inc   = (arg.flags.direction == DI_forward) ? 1 : -1;
		offset_ini = (arg.flags.direction == DI_forward) ? 0 : arg.width - 1;
		offset_inc = (arg.flags.direction == DI_forward) ? 1 : -1;
		bumpSrc = (arg.flags.direction == DI_forward) ? arg.srcPpl : -arg.srcPpl;
		bumpDst = (arg.flags.direction == DI_forward) ? arg.dstPpl : -arg.dstPpl;
		//
		if (PERF_ENABLE) count_MonoBlt_bit++;
	}
};

MonoBlt* MonoBlt::getInstance(ColorBlt::ColorBltTable& arg) {
	if (arg.flags.pattern) return MonoBlt_pat::getInstance(arg);
	else return MonoBlt_bit::getInstance(arg);
}


void E_COLORBLT_(Opcode*) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  COLORBLT  %d", savedPC, SP);
	if (SP == 1) {
		ColorBlt::ColorBltTable arg;

		POINTER ptr = Pop();
		MonoBlt::FetchColorBltTable(ptr, &arg);

//		int updateRect = 0;
//		GuiOp::Rect rect;
//
//		if (arg.flags.dstType == ColorBlt::PT_display) {
//			rect.width  = arg.width;
//			rect.height = arg.height;
//
//			CARD32 base = Memory::getDisplayVirtualPage() * PageSize;
//			CARD32 size = Memory::getDisplayPageSize() * PageSize;
//
//			ColorBlt::Address address = arg.dst;
//			MonoBlt::Bump(address, 0);
//
//			if (base <= address.word && address.word < (base + size)) {
//				CARD32 displayWorsdPerLine = Memory::getDisplayBytesPerLine() / 2;
//
//				if (arg.dstPpl != (displayWorsdPerLine * WordSize)) {
//					logger.fatal("arg.dstPpl = %d  displayWordPerLine = %d", arg.dstPpl, displayWorsdPerLine);
//					ERROR();
//				}
//
//				CARD32 offset = address.word - base;
//				rect.y = offset / displayWorsdPerLine;
//				rect.x = (offset % displayWorsdPerLine) * WordSize + address.pixel;
//				updateRect = 1;
//			}
//		}

		std::unique_ptr<MonoBlt> blt(MonoBlt::getInstance(arg));
		blt->process();

//		if (updateRect) {
//			logger.debug("updateDisplay %4d %4d %4d %4d", rect.x, rect.y, rect.width, rect.height);
//			GuiOp::updateDisplay(&rect);
//		}

		GuiOp::Rect rect(0, 0, 0, 0);
		GuiOp::updateDisplay(&rect);
	} else {
		logger.fatal("SP = %d", SP);
		ERROR();
	}
}


void E_BITBLT_(Opcode*) {
	if (DEBUG_TRACE_RUN) logger.debug("TRACE %6o  BITBLT  %d", savedPC, SP);
	if (SP == 1) {
		ColorBlt::ColorBltTable arg;

		POINTER ptr = Pop();
		MonoBlt::FetchBitBltTable(ptr, &arg);

		std::unique_ptr<MonoBlt> blt(MonoBlt::getInstance(arg));
		blt->process();

		GuiOp::Rect rect(0, 0, 0, 0);
		GuiOp::updateDisplay(&rect);

	} else {
		logger.fatal("SP = %d", SP);
		ERROR();
	}
}
/*
void fillTest(int shift) {
	printf("shift = %2d\n", shift);

	int widthInWord = 4;
	CARD16 srcData[5] = {0xf123, 0x4567, 0x89ab, 0xcdef, 0xffff};
	printf("       %04X %04X %04X %04X %04X\n", srcData[0], srcData[1], srcData[2], srcData[3], srcData[4]);

	CARD16 lineBuffer[5] = {0, 0, 0, 0, 0};

	if (0 <= shift) {
		CARD16 mask = (0xFFFFU) << shift;
		CARD16 lastWord = 0;
		for(int i = widthInWord - 1; 0 <= i; i--) {
			CARD16 word = srcData[i];
			CARD32 longWord = word;
			longWord <<= shift;
			lineBuffer[i] = (LowHalf(longWord) & mask) | lastWord;
			lastWord = HighHalf(longWord) & ~mask;
		}
	} else {
		CARD16 mask = (0xFFFFU) >> -shift;
		CARD16 lastWord = 0;
		shift = -shift;
		for(int i = 0; i < widthInWord; i++) {
			CARD16 word = srcData[i];
			CARD32 longWord = word << WordSize;
			longWord >>= shift;
			lineBuffer[i] = lastWord | (HighHalf(longWord) & mask);
			lastWord = LowHalf(longWord) & ~mask;
		}
		lineBuffer[widthInWord] = lastWord;
	}
	printf("       %04X %04X %04X %04X %04X\n\n", lineBuffer[0], lineBuffer[1], lineBuffer[2], lineBuffer[3], lineBuffer[4]);
}
*/


/*

------------
  src               0  0  =>  0
  src               0  1  =>  0
  src               1  0  =>  1
  src               1  1  =>  1
------------
  srcIfDstLE1       0  0  =>  0
  srcIfDstLE1       0  1  =>  0
  srcIfDstLE1       1  0  =>  0
  srcIfDstLE1       1  1  =>  1
------------
  srcIf0            0  0  =>  0
  srcIf0            0  1  =>  0
  srcIf0            1  0  =>  0
  srcIf0            1  1  =>  1
------------
  srcIfDstNot0      0  0  =>  0
  srcIfDstNot0      0  1  =>  0
  srcIfDstNot0      1  0  =>  0
  srcIfDstNot0      1  1  =>  1
------------
  srcIfNot0         0  0  =>  0
  srcIfNot0         0  1  =>  1
  srcIfNot0         1  0  =>  1
  srcIfNot0         1  1  =>  1
------------
  srcIfDst0         0  0  =>  0
  srcIfDst0         0  1  =>  1
  srcIfDst0         1  0  =>  1
  srcIfDst0         1  1  =>  1
------------
  pixelXor          0  0  =>  0
  pixelXor          0  1  =>  1
  pixelXor          1  0  =>  1
  pixelXor          1  1  =>  0
------------
  srcXorDst         0  0  =>  0
  srcXorDst         0  1  =>  1
  srcXorDst         1  0  =>  1
  srcXorDst         1  1  =>  0
------------
~ src               0  0  =>  1
~ src               0  1  =>  1
~ src               1  0  =>  0
~ src               1  1  =>  0
------------
~ srcIfDstLE1       0  0  =>  0
~ srcIfDstLE1       0  1  =>  1
~ srcIfDstLE1       1  0  =>  0
~ srcIfDstLE1       1  1  =>  0
------------
~ srcIf0            0  0  =>  0
~ srcIf0            0  1  =>  1
~ srcIf0            1  0  =>  0
~ srcIf0            1  1  =>  0
------------
~ srcIfDstNot0      0  0  =>  0
~ srcIfDstNot0      0  1  =>  1
~ srcIfDstNot0      1  0  =>  0
~ srcIfDstNot0      1  1  =>  0
------------
~ srcIfNot0         0  0  =>  1
~ srcIfNot0         0  1  =>  1
~ srcIfNot0         1  0  =>  0
~ srcIfNot0         1  1  =>  1
------------
~ srcIfDst0         0  0  =>  1
~ srcIfDst0         0  1  =>  1
~ srcIfDst0         1  0  =>  0
~ srcIfDst0         1  1  =>  1
------------
~ pixelXor          0  0  =>  1
~ pixelXor          0  1  =>  0
~ pixelXor          1  0  =>  0
~ pixelXor          1  1  =>  1
------------
~ srcXorDst         0  0  =>  1
~ srcXorDst         0  1  =>  0
~ srcXorDst         1  0  =>  0
~ srcXorDst         1  1  =>  1


static inline CARD16 Function(ColorBlt::ColorBltFlags flags, CARD16 s, CARD16 d) {
	// Function
	if (flags.srcFunc == ColorBlt::SF_complement) s ^= 1;

	switch(flags.dstFunc) {
	case ColorBlt::DF_src:
		d = s;
		break;
	case ColorBlt::DF_srcIfDstLE1:
		d &= s;
		break;
	case ColorBlt::DF_srcIf0:
		if (s == 0) d = 0;
		break;
	case ColorBlt::DF_srcIfDstNot0:
		if (d) d = s;
		break;
	case ColorBlt::DF_srcIfNot0:
		if (s) d = 1;
		break;
	case ColorBlt::DF_srcIfDst0:
		if (d == 0) d = s;
		break;
	case ColorBlt::DF_pixelXor:
		if (d == 0) d = s;
		else if (s) d = 0;
		break;
	case ColorBlt::DF_srcXorDst:
		d ^= s;
		break;
	default:
		ERROR();
		break;
	}

	return d;
}

*/



/*
static inline CARD16 ReadPixel(ColorBlt::Address address, int offset, CARD16 type, CARD16 pattern = 0, CARD16 unpacked = 0) {
	Bump(address, offset);
	int shift = WordSize - (address.pixel + 1);
	CARD16 word = *Fetch(address.word);
	if (pattern && word && unpacked) word = 0xffff;
	if (type == ColorBlt::PT_display) word = ntohs(word);
	return (word >> shift) & 1;
}

void OP_COLORBLT() {
	if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  aCOLORBLT  %d", savedPC, SP);

	ColorBlt::ColorBltTable arg;
	int line;
	INT       lastGray;
	INT       grayWidth;
	INT32  grayBump;
	int       xDirection, yDirection;
	ColorBlt::PatternParm patternParm;


	if (SP == 1) {
		POINTER ptr = Pop();
		FetchColorBltTable(ptr, &arg);
		logger.debug("dim      %4d x %4d", arg.width, arg.height);
		logger.debug("flags    %c %c %c %c %d %d %d", arg.flags.direction ? 'B' : 'F', arg.flags.srcType ? 'D' : 'B', arg.flags.dstType ? 'D' : 'B', arg.flags.pattern ? 'P' : ' ', arg.flags.srcFunc, arg.flags.dstFunc, arg.flags.reserved);
		//logger.debug("color    %04X %04X", arg.colorMapping.color[0], arg.colorMapping.color[1]);
		logger.debug("dst  %8X %3d  %4d", arg.dst.word, arg.dst.pixel, arg.dstPpl);
		if (arg.flags.pattern) {
			CARD16 t = *Fetch(arg.src.word);
			logger.debug("src  %8X %3d  (%04X)", arg.src.word, arg.src.pixel, t);
			logger.debug("pat  unpacked  %d  yOffset %d  widthMinusOne %d  heightMinusOne %d", arg.pattern.unpacked, arg.pattern.yOffset, arg.pattern.widthMinusOne, arg.pattern.heightMinusOne);
		} else {
			logger.debug("src  %8X %3d  %4d", arg.src.word, arg.src.pixel, arg.srcPpl);
		}

		if (((!arg.flags.pattern) && arg.srcPpl == 0) || arg.dstPpl == 0 || 32767 < arg.width || 32767 < arg.height) ERROR();


		if (arg.flags.pattern) {
			patternParm.u = arg.srcPpl;
			if (patternParm.widthMinusOne || arg.flags.direction != DI_forward || arg.dstPpl < 0) ERROR();
			grayWidth = (INT)((patternParm.widthMinusOne + 1) * WordSize);
			grayBump = -grayWidth * patternParm.heightMinusOne;
		} else {
			if (arg.flags.direction == DI_forward  && (arg.srcPpl < 0 || arg.dstPpl < 0)) ERROR();
			if (arg.flags.direction == DI_backward && (0 < arg.srcPpl || 0 < arg.dstPpl)) ERROR();
		}

		// ComputeDirection
		{
			yDirection = xDirection = arg.flags.direction;
		}

		if (arg.flags.pattern) {
			lastGray = (yDirection == DI_forward) ? (patternParm.heightMinusOne - patternParm.yOffset) : patternParm.yOffset;
			//logger.debug("grayWidth = %d", grayWidth);
			//logger.debug("grayBump  = %d", grayBump);
			//logger.debug("lastGray  = %d", lastGray);
		}
		line = (yDirection == DI_forward) ? 0 : arg.height - 1;
	} else {
		logger.fatal("SP = %d", SP);
		ERROR();
	}

//	logger.debug("pattern   = %d", arg.flags.pattern);
//	logger.debug("grayWidth = %d", grayWidth);
//	logger.debug("grayBump  = %d", grayBump);
//	logger.debug("lastGray  = %d", lastGray);
//	logger.debug("line      = %d", line);

	MINIMAL_STACK();
	for(;;) {
		if (line < 0 || arg.height <= line) break;
		{
			// BitBltItem
			INT offset = (xDirection == DI_forward) ? 0 : arg.width - 1;
			for(int i = 0; i < arg.width; i++) {
				INT pos = (arg.flags.pattern) ? (((offset + arg.src.pixel) % ABS(grayWidth)) - arg.src.pixel) : offset;
				CARD16 s = ReadPixel(arg.src, pos,    arg.flags.srcType, arg.flags.pattern, arg.pattern.unpacked);
				CARD16 d = ReadPixel(arg.dst, offset, arg.flags.dstType);

				WritePixel(arg.dst, offset, Function(arg.flags, s, d), arg.flags.dstType);
				offset += (xDirection == DI_forward) ? 1 : -1;
			}
		}

		INT32 offset = arg.flags.pattern ? (((line % (patternParm.heightMinusOne + 1)) == lastGray) ? grayBump : grayWidth) : arg.srcPpl;
		Bump(arg.src, offset);
		Bump(arg.dst, arg.dstPpl);
		line += (yDirection == DI_forward) ? 1 : -1;
//		if (InterruptPending()) {
//			{
//				// PushState
//				// TODO implement PushState
//				ERROR();
//			}
//			PC = savedPC;
//		}
	}
}
*/
