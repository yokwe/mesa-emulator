package mh.majuro.mesa;

import mh.majuro.UnexpectedException;
import mh.majuro.mesa.Type.CARD16;
import mh.majuro.mesa.Type.POINTER;

public final class LFCache {
	private static int     lf;
	private static int     offset;
	private static int     pageEnd;
	private static short[] page;
	
	public static void setLF(@POINTER int newValue) {
		lf      = newValue & 0xFFFF;
		offset  = lf % Memory.PAGE_SIZE;
		pageEnd = Memory.PAGE_SIZE - offset - 1;
		page    = Memory.storePage(Memory.lengthenPointer(lf));
	}
	public static @POINTER int getLF() {
		return lf;
	}
	public static @CARD16 int fetch(int index) {
		// NOTE index can be negative value to access LocalOverhead
		int pageIndex = offset + index;
		if (0 <= pageIndex) {
			if (pageIndex <= pageEnd) {
				if (Perf.ENABLE) Perf.lfCacheHit++;
				return page[pageIndex] & 0xFFFF;
			} else {
				if (Perf.ENABLE) Perf.lfCacheMiss++;
				return Memory.fetchMDS(lf + index);
			}
		} else {
			throw new UnexpectedException("");
		}
	}
	public static void store(@CARD16 int index, int newValue) {
		// NOTE index can be negative value to access LocalOverhead
		int pageIndex = offset + index;
		if (0 <= pageIndex) {
			if (pageIndex <= pageEnd) {
				if (Perf.ENABLE) Perf.lfCacheHit++;
				page[pageIndex] = (short)newValue;
			} else {
				if (Perf.ENABLE) Perf.lfCacheMiss++;
				Memory.storeMDS(lf + index, newValue);
			}
		} else {
			throw new UnexpectedException("");
		}
	}

}
