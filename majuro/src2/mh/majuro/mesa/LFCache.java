package mh.majuro.mesa;

public class LFCache {
	private static short   lf;
	private static int     pageEnd;
	private static short[] page;
	public static void setLF(short newValue) {
		lf = newValue;
		pageEnd = Memory.PAGE_SIZE - (lf % Memory.PAGE_SIZE) - 1;
		page = Memory.storePage(Memory.lengthenPointer(lf));
		
	}
	public static short getLF() {
		return lf;
	}
	public static short fetchLF(short ptr) {
		int index = Short.toUnsignedInt(ptr);
		if (index <= pageEnd) {
			if (Perf.ENABLE) Perf.lfCacheHit++;
			return page[index];
		} else {
			if (Perf.ENABLE) Perf.lfCacheMiss++;
			return Memory.fetchMDS((short)(lf + ptr));
		}
	}

}
