package mh.majuro.mesa.type;

import mh.majuro.mesa.Type.CARD16;
import mh.majuro.mesa.Type.CARD8;

// BytePair: TYPE =  MACHINE DEPENDENT RECORD [left (0: 0..7), right (0: 8..15): BYTE];
public final class BytePair {
	public static @CARD8 int left(@CARD16 int value) {
		return (value >>> 8) & 0xFF;
	}
	public static @CARD8 int right(@CARD16 int value) {
		return value & 0xFF;
	}
	public static @CARD16 int make(@CARD8 int left, @CARD8 int right) {
		return ((left << 8) & 0xFF00) | (right & 0x00FF);
	}
}