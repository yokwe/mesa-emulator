package mh.majuro.mesa;

import mh.majuro.mesa.Type.CARD8;
import mh.majuro.mesa.Type.LONG_POINTER;

public final class Processes {
	// 10.4.3 Faults
	public static void frameFault(@CARD8 int fsi) {
		// FIXME
	}
	public static void pageFault(@LONG_POINTER int ptr) {
		// FIXME
	}
	public static void writeProtectFault(@LONG_POINTER int ptr) {
		// FIXME
	}
}
