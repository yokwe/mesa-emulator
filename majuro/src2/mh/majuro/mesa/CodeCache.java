package mh.majuro.mesa;

import mh.majuro.mesa.Type.CARD16;
import mh.majuro.mesa.Type.CARD8;
import mh.majuro.mesa.Type.LONG_POINTER;
import mh.majuro.mesa.type.BytePair;

public final class CodeCache {
	private static final int PAGE_SIZE_IN_BYTE = Memory.PAGE_SIZE * 2;
	private static final int PAGE_MASK_IN_BYTE = PAGE_SIZE_IN_BYTE - 1;
	
	private static int     cb;
	private static int     pc;
	private static int     wordPC;
	private static boolean even;
	
	private static int lastWordPC;
	private static int lastWord;

	private static short[] page;
	private static int     wordOffset;
	private static int     startPC;
	private static int     endPC;

	public static void setCB(@LONG_POINTER int newValue) {
		cb = newValue;
		invalidate();
	}
	public static @LONG_POINTER int getCB() {
		return cb;
	}
	
	public static void setPC(@CARD16 int newValue) {
		// size of pc is 16 bit
		pc     = newValue & 0xFFFF;
		wordPC = pc / 2;
		even   = (pc & 1) == 0;
		
		lastWordPC = -1;
	}
	public static @CARD16 int getPC() {
		return pc;
	}
	
	public static @CARD8 int getCodeByte() {		
		if (Perf.ENABLE) Perf.codeCacheCodeByte++;
		if (lastWordPC != wordPC) {
			// unit of pc is byte
			if (startPC <= pc && pc <= endPC) {
				if (Perf.ENABLE) Perf.codeCacheHit++;
			} else {
				if (Perf.ENABLE) Perf.codeCacheMiss++;
				setup();
			}
			lastWord = page[wordOffset + wordPC];
		}
		
		// RETURN[IF even THEN word.left ELSE word.right];
		int ret = even ? BytePair.left(lastWord) : BytePair.right(lastWord);
		
		// increment pc
		// size of pc is 16 bit
		pc     = (pc + 1) & 0xFFFF;
		wordPC = pc / 2;
		even   = !even;
		
		return ret;
	}
	public static @CARD16 int getCodeWord() {
		int left  = getCodeByte();
		int right = getCodeByte();
		
		return (left << 8) | right;
	}
	
	private static void invalidate() {
		startPC    = 0x10000;
		endPC      = 0;
		lastWordPC = -1;
	}
	
	private static void setup() {
		// To prevent bogus PageFault, PC need to have real value
		int va = cb + wordPC;
		page = Memory.fetchPage(va);
		// unit of offsetCB is byte
		int offsetCB = (cb * 2) & PAGE_MASK_IN_BYTE;
		
		if ((pc + offsetCB) < PAGE_SIZE_IN_BYTE) {
			// Valid PC range is [startPC..endPC]
			startPC    = 0;
			endPC      = PAGE_SIZE_IN_BYTE - offsetCB - 1;
			wordOffset = offsetCB / 2;
		} else {
			// Valid PC range is [startPC..endPC]
			startPC    = ((pc + offsetCB) & ~PAGE_MASK_IN_BYTE) - offsetCB;
			endPC      = startPC + PAGE_SIZE_IN_BYTE - 1; // to end of page
			wordOffset = (-startPC) / 2;
		}
	}

}
