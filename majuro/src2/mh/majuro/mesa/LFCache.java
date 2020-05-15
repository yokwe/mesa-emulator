package mh.majuro.mesa;

import mh.majuro.mesa.Type.CARD16;
import mh.majuro.mesa.Type.POINTER;

public class LFCache {
	private static int   lf;
	private static int     pageEnd;
	private static short[] page;
	
	public static void setLF(@POINTER int newValue) {
		lf = newValue & 0xFFFF;
		pageEnd = Memory.PAGE_SIZE - (lf % Memory.PAGE_SIZE) - 1;
		page = Memory.storePage(Memory.lengthenPointer(lf));
		
	}
	public static @POINTER int getLF() {
		return lf;
	}
	public static @CARD16 int fetchLF(@POINTER int pointer) {
		int index = pointer & 0xFFFF;
		if (index <= pageEnd) {
			if (Perf.ENABLE) Perf.lfCacheHit++;
			return page[index] & 0xFFFF;
		} else {
			if (Perf.ENABLE) Perf.lfCacheMiss++;
			return Memory.fetchMDS(lf + pointer);
		}
	}

}
