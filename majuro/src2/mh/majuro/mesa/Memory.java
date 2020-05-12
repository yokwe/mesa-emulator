package mh.majuro.mesa;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import mh.majuro.UnexpectedException;

public final class Memory {
	private static final Logger logger = LoggerFactory.getLogger(Memory.class);
	
	private static final int HASH_BIT = 14;
	private static final int HASH_SIZE = 1 << HASH_BIT;
	private static final int HASH_MASK = (1 << HASH_BIT) - 1;
	private static int hash(int vp_) {
		return ((vp_ >> HASH_BIT) ^ vp_) & HASH_MASK;
		// NOTE above expression calculate better hash value than "vp_ & MASK"
	}

	private static class Cache {
		int     vp;
		boolean flagStore;
		short[] page;
		
		Cache() {
			init();
		}
		void init() {
			vp        = 0;
			flagStore = false;
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
	
	// MAX_RM_PAGE is maximum number of PAGE for real memory
	public static final int MAX_VM_BITS = 30;
	public static final int MAX_RM_BITS = 24;
	public static final int MAX_RM_PAGE = 4086 * WORD_SIZE;
	
	// special memory location
	public static final int IO_REGION_PAGE = 0x80;

	
	private int vmBits = 0;
	private int rmBits = 0;
	private int vmPage = 0;
	private int rmPage = 0;
	
	private MapFlags[] mapFlags;
	private short[][]  rmPages;
	private Cache[]    cache;
	
	public void stats() {
		int used = 0;
		for(int i = 0; i < cache.length; i++) {
			if (cache[i].vp != 0) used++;
		}
		
		logger.info(String.format("Cache %5d / %5d", used, cache.length));
	}

	
	public Memory(int vmBits_, int rmBits_) {
		vmBits = vmBits_;
		rmBits = rmBits_;
		
		if (MAX_VM_BITS < vmBits) throw new UnexpectedException();
		if (MAX_RM_BITS < rmBits) throw new UnexpectedException();
		
		int vmSize = 1 << vmBits;
		int rmSize = 1 << rmBits;
		
		vmPage = vmSize / PAGE_SIZE;
		rmPage = rmSize / PAGE_SIZE;
		if (MAX_RM_PAGE < rmPage) rmPage = MAX_RM_PAGE;

		mapFlags = new MapFlags[vmPage];
		for(int i = 0; i < mapFlags.length; i++) {
			mapFlags[i] = new MapFlags();
		}
		
		rmPages  = new short[rmPage][PAGE_SIZE];
		
		// initialize mapFlags
		// map all real page to virtual page
		int rp = 0;
		// map real page page [ioRegionPage..256)
		for (int i = IO_REGION_PAGE; i < 256; i++) {
			mapFlags[i].set(rp++, MapFlags.CLEAR);
		}
		// then map real page page [0..ioRegionPage)
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
		
		// init cache related fields
		cache             = new Cache[HASH_SIZE];
		
		for(int i = 0; i < cache.length; i++) {
			cache[i] = new Cache();
		}
	}
	
	public void invalidate(int vp) {
		Cache p = cache[hash(vp)];
		if (p.vp == vp) {
			// if cache entry contains vp data, initialize cache entry.
			p.init();
		}
	}

	// page level memory access
	public short[] fetchPage(int va) {
		int vp = va >>> PAGE_SIZE_BITS;

		Cache p = cache[hash(vp)];
		if (p.vp != vp) {
			if (Perf.PERF_ENABLE) {
				if (p.vp != 0) Perf.cacheMissConflict++;
				else Perf.cacheMissEmpty++;
			}
			// Fill cache entry
			MapFlags mf = mapFlags[vp];
			mf.checkFetch(va);
			mf.updateFlagFetch();
			
			p.vp        = vp;
			p.flagStore = mf.isReferencedDirty();
			p.page      = rmPages[mf.rmPage];
		} else {
			if (Perf.PERF_ENABLE) Perf.cacheHit++;
		}
		return p.page;
	}
	public short[] storePage(int va) {
		if (Perf.PERF_ENABLE) Perf.cacheHit++;

		int vp = va >>> PAGE_SIZE_BITS;

		Cache p = cache[hash(vp)];
		if (p.vp != vp) {
			if (Perf.PERF_ENABLE) {
				if (p.vp != 0) Perf.cacheMissConflict++;
				else Perf.cacheMissEmpty++;
			}
			// Fill cache entry
			MapFlags mf = mapFlags[vp];
			mf.checkStore(va);
			mf.updateFlagStore();

			p.vp        = vp;
			p.flagStore = true;
			p.page      = rmPages[mf.rmPage];
		} else {
			if (Perf.PERF_ENABLE) Perf.cacheHit++;
			if (!p.flagStore) {
				p.flagStore = true;
				mapFlags[vp].updateFlagStore();
			}
		}
		return p.page;
	}

	// low level memory access
	public short fetch(int va) {
		return fetchPage(va)[va & PAGE_OFFSET_MASK];
	}
	public void store(int va, short newValue) {
		fetchPage(va)[va & PAGE_OFFSET_MASK] = newValue;
	}

	
	// displayPage
	private int displayPage = 0;
	public void reserveDisplayPage(int displayPage_) {
		if (displayPage_ <= 0) throw new UnexpectedException();
		if (displayPage != 0) throw new UnexpectedException();
		
		displayPage = displayPage_;
		// unmap real page from last real page for display page
		for(int i = rmPage - displayPage; i < rmPage; i++) {
			mapFlags[i].set(0, MapFlags.VACANT);
		}
	}
	
	// mds
	private int mds;
	public int getMDS() {
		return mds;
	}
	public void setMDS(int newValue) {
		mds = newValue;
	}

	public int lengthenPointer(short pointer) {
		return mds | (pointer & 0xFFFF);
	}
}
