package mh.majuro.mesa;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import mh.majuro.UnexpectedException;
import mh.majuro.mesa.Type.CARD16;
import mh.majuro.mesa.Type.CARD32;
import mh.majuro.mesa.Type.LONG_POINTER;
import mh.majuro.mesa.Type.PAGE_NUMBER;
import mh.majuro.mesa.Type.POINTER;

public final class Memory {
	private static final Logger logger = LoggerFactory.getLogger(Memory.class);
	
	private static final int HASH_BIT = 14;
	private static final int HASH_SIZE = 1 << HASH_BIT;
	private static final int HASH_MASK = (1 << HASH_BIT) - 1;
	private static int hash(@PAGE_NUMBER int vp_) {
		return ((vp_ >> HASH_BIT) ^ vp_) & HASH_MASK;
		// NOTE above expression calculate better hash value than "vp_ & MASK"
	}

	private static class Cache {
		@PAGE_NUMBER
		int      vp;
		MapFlags mf;
		short[]  page;
		
		Cache() {
			init();
		}
		void init() {
			vp        = 0;
			mf        = null;
			page      = null;
		}
	};


	// WORD_SIZE is number of bits in one WORD
	public static final int WORD_SIZE_BITS = 4;
	public static final int WORD_SIZE      = 1 << WORD_SIZE_BITS;

	// PAGE_SIZE is number of WORD in one PAGE
	public static final int PAGE_SIZE_BITS   = 8;
	public static final int PAGE_SIZE        = 1 << PAGE_SIZE_BITS;
	public static final int PAGE_OFFSET_MASK = PAGE_SIZE - 1;
	public static final int PAGE_END         = PAGE_SIZE - 1;
	
	// MAX_RM_PAGE is maximum number of PAGE for real memory
	public static final int MAX_VM_BITS = 30;
	public static final int MAX_RM_BITS = 24;
	public static final int MAX_RM_PAGE = 4086 * WORD_SIZE;
	
	// special memory location
	public static final @PAGE_NUMBER int IO_REGION_PAGE = 0x80;

	
	private static int vmBits = 0;
	private static int rmBits = 0;
	private static int vmPage = 0;
	private static int rmPage = 0;
	
	private static short[][]  rmPages;
	private static Cache[]    cache;
	private static MapFlags[] mapFlags;
	
	public static void stats() {
		int used = 0;
		for(int i = 0; i < cache.length; i++) {
			if (cache[i].vp != 0) used++;
		}
		
		logger.info(String.format("Cache %5d / %5d", used, cache.length));
	}

	
	public static void initialize(int vmBits_, int rmBits_) {
		vmBits = vmBits_;
		rmBits = rmBits_;
		
		if (MAX_VM_BITS < vmBits) throw new UnexpectedException();
		if (MAX_RM_BITS < rmBits) throw new UnexpectedException();
		
		int vmSize = 1 << vmBits;
		int rmSize = 1 << rmBits;
		
		vmPage = vmSize / PAGE_SIZE;
		rmPage = rmSize / PAGE_SIZE;
		if (MAX_RM_PAGE < rmPage) rmPage = MAX_RM_PAGE;

		rmPages  = new short[rmPage][PAGE_SIZE];
		cache    = new Cache[HASH_SIZE];
		mapFlags = new MapFlags[vmPage];

		// initialize element of cache
		for(int i = 0; i < cache.length; i++) {
			cache[i] = new Cache();
		}

		// initialize element of mapFlags
		for(int i = 0; i < mapFlags.length; i++) {
			mapFlags[i] = new MapFlags();
		}
		
		// See APilot/15.3/Pilot/Private/GermOpsImpl.mesa
		// The BOOTING ACTION defined by the Principles of Operation should include:
		// 	   1. Put real memory behind any special processor pages (I/O pages); then put all remaining usable real memory behind other virtual memory pages beginning at virtual page 0, and working upward sequentially (skipping any already-mapped special processor pages).
		// 	   2. Read consecutive pages of the Germ into virtual memory beginning at page Boot.pageGerm + Boot.mdsiGerm*Environment.MaxPagesInMDS (i.e page 1).  At present, the way the initial microcode figures out the device and device address of the Germ, and the number of pages which comprise the Germ, is processor-dependent.
		// 	   3. Set Boot.pRequest� to e.g. [Boot.currentRequestBasicVersion, Boot.bootPhysicalVolume, locationOfPhysicalVolume].
		// 	   4. Set WDC>0, NWW=0, MDS=Boot.mdsiGerm, STKP=0.
		// 	   5. Xfer[dest: Boot.pInitialLink].

		// set default value for mapFlags
		// map all real page to virtual page
		int rp = 0;
		// map real page page [ioRegionPage..256)
		for (int i = IO_REGION_PAGE; i < 256; i++) {
			mapFlags[i].set(rp++, MapFlags.CLEAR);
		}
		// then map real page [0..ioRegionPage)
		for (int i = 0; i < IO_REGION_PAGE; i++) {
			mapFlags[i].set(rp++, MapFlags.CLEAR);
		}
		// then map real page [256..rmPage)
		for (int i = 256; i < rmPage; i++) {
			mapFlags[i].set(rp++, MapFlags.CLEAR);
		}
		// then set remaining virtual page with vacant [rmPage..vmPage)
		for (int i = rmSize; i < vmPage; i++) {
			mapFlags[i].set(0, MapFlags.VACANT);
		}
	}
	
	public static void invalidate(@PAGE_NUMBER int vp) {
		Cache p = cache[hash(vp)];
		// if cache entry contains vp data, initialize cache entry.
		if (p.vp == vp) p.init();
	}
	
	//
	// raw level access to memory for unit test
	//
	public static short[] rawPage(@LONG_POINTER int va) {
		return rmPages[mapFlags[va >>> PAGE_SIZE_BITS].rmPage];
	}
	public static @CARD16 short rawRead(@LONG_POINTER int va) {
		return rawPage(va)[va & PAGE_OFFSET_MASK];
	}
	public static void rawWrite(@LONG_POINTER int va, @CARD16 int newValue) {
		rawPage(va)[va & PAGE_OFFSET_MASK] = (short)newValue;
	}
	
	//
	// map operation
	//
	MapFlags readMap(@PAGE_NUMBER int vp) {
		MapFlags mf = mapFlags[vp];
		if (mf.isVacant()) mf.rmPage = 0;
		return mf;
	}
	void writeMap(@PAGE_NUMBER int vp, MapFlags mf) {
		if (Perf.ENABLE) Perf.writeMap++;
		if (mf.isVacant()) mf.rmPage = 0;
		mapFlags[vp] = mf;
		
		invalidate(vp);
	}
	

	// page level memory access
	public static short[] fetchPage(@LONG_POINTER int va) {
		if (Perf.ENABLE) Perf.fetchPage++;
		
		int vp = va >>> PAGE_SIZE_BITS;

		Cache p = cache[hash(vp)];
		if (p.vp != vp) {
			if (Perf.ENABLE) {
				if (p.vp != 0) Perf.cacheMissConflict++;
				else Perf.cacheMissEmpty++;
			}
			// Fill cache entry
			MapFlags mf = mapFlags[vp];
			mf.checkFetch(va);
			mf.updateFlagFetch();
			
			p.vp   = vp;
			p.mf   = mf;
			p.page = rmPages[mf.rmPage];
		} else {
			if (Perf.ENABLE) Perf.cacheHit++;
		}
		return p.page;
	}
	public static short[] storePage(@LONG_POINTER int va) {
		if (Perf.ENABLE) Perf.storePage++;

		int vp = va >>> PAGE_SIZE_BITS;

		Cache p = cache[hash(vp)];
		if (p.vp != vp) {
			if (Perf.ENABLE) {
				if (p.vp != 0) Perf.cacheMissConflict++;
				else Perf.cacheMissEmpty++;
			}
			// Fill cache entry
			MapFlags mf = mapFlags[vp];
			mf.checkStore(va);
			mf.updateFlagStore();

			p.vp   = vp;
			p.mf   = mf;
			p.page = rmPages[mf.rmPage];
		} else {
			if (Perf.ENABLE) Perf.cacheHit++;
			if (!p.mf.isReferencedDirty()) p.mf.updateFlagStore();
		}
		return p.page;
	}

	// low level memory access
	public static @CARD16 int fetch(@LONG_POINTER int va) {
		if (Perf.ENABLE) Perf.fetch++;
		return fetchPage(va)[va & PAGE_OFFSET_MASK] & 0xFFFF;
	}
	public static void store(@LONG_POINTER int va, @CARD16 int newValue) {
		if (Perf.ENABLE) Perf.store++;
		fetchPage(va)[va & PAGE_OFFSET_MASK] = (short)newValue;
	}
	public static @CARD32 int readDbl(@LONG_POINTER int va) {
		if (Perf.ENABLE) Perf.readDbl++;
		
		short[] page = fetchPage(va);
		int     vo   = va & PAGE_OFFSET_MASK;
		if (vo != PAGE_END) {
			return (page[vo + 1] << WORD_SIZE) | (page[vo] & 0xFFFF);
		} else {
			return (fetchPage(va + 1)[0] << WORD_SIZE) | (page[vo] & 0xFFFF);
		}
	}

	
	// displayPage
	private static int displayPage = 0;
	public static void reserveDisplayPage(int displayPage_) {
		if (displayPage_ <= 0) throw new UnexpectedException();
		if (displayPage != 0) throw new UnexpectedException();
		
		displayPage = displayPage_;
		// unmap real page from last real page for display page
		for(int i = rmPage - displayPage; i < rmPage; i++) {
			mapFlags[i].set(0, MapFlags.VACANT);
		}
	}
	
	// mds
	private static @LONG_POINTER int mds;
	public static @LONG_POINTER int getMDS() {
		return mds;
	}
	public static void setMDS(@LONG_POINTER int newValue) {
		mds = newValue;
	}

	public static @LONG_POINTER int lengthenPointer(@POINTER int pointer) {
		return mds | (pointer & 0xFFFF);
	}
	public static @CARD16 int fetchMDS(@POINTER int pointer) {
		if (Perf.ENABLE) Perf.fetchMDS++;
		int va = lengthenPointer(pointer);
		return fetchPage(va)[va & PAGE_OFFSET_MASK] & 0xFFFF;
	}
	public static void storeMDS(@POINTER int pointer, @CARD16 int newValue) {
		if (Perf.ENABLE) Perf.storeMDS++;
		int va = lengthenPointer(pointer);
		fetchPage(va)[va & PAGE_OFFSET_MASK] = (short)newValue;
	}
	public static @CARD32 int readDblMDS(@POINTER int pointer) {
		if (Perf.ENABLE) Perf.readDblMDS++;
		int va = lengthenPointer(pointer);

		short[] page = fetchPage(va);
		int     vo   = va & PAGE_OFFSET_MASK;
		if (vo != PAGE_END) {
			return (page[vo + 1] << WORD_SIZE) | (page[vo] & 0xFFFF);
		} else {
			return (fetchPage(va + 1)[0] << WORD_SIZE) | (page[vo] & 0xFFFF);
		}		
	}

	// code
	public static @CARD16 int readCode(@CARD16 int offset) {
		if (Perf.ENABLE) Perf.readCode++;
		int va = CodeCache.getCB() + (offset & 0xFFFF);
		return fetchPage(va)[va & PAGE_OFFSET_MASK] & 0xFFFF;
	}
}
