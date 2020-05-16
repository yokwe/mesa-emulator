package mh.majuro.mesa.type;

import mh.majuro.mesa.Type.CARD16;
import mh.majuro.mesa.Type.CARD32;

// Long: TYPE = MACHINE DEPENDENT RECORD[low(0), high(1): UNSPECIFIED];
// Above record is illustrate memory layout of CARD32
public final class Long {
	public static @CARD16 int low(@CARD32 int value) {
		return value & 0x0000FFFF;
	}
	public static @CARD16 int high(@CARD32 int value) {
		return (value >>> 16) & 0x0000FFFF;
	}
	public static @CARD32 int make(@CARD16 int high, @CARD16 int low) {
		return ((high << 16) & 0xFFFF0000) | (low & 0x0000FFFF);
	}
}