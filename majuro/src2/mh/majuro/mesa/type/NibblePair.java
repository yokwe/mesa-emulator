package mh.majuro.mesa.type;

import mh.majuro.mesa.Type.CARD16;

// 2.1.2 Bit, Nibble, Byte
// NibblePair: TYPE = MACHINE DEPENDENT RECORD[left (0:0..3) NIBBLE, right(0:4..7) NIBBLE];
public final class NibblePair {
	public static @CARD16 int left(@CARD16 int value) {
		return (value >>> 4) & 0x0F;
	}
	public static @CARD16 int right(@CARD16 int value) {
		return value & 0x0F;
	}
	public static @CARD16 int make(@CARD16 int left, @CARD16 int right) {
		return ((left << 4) & 0xF0) | (right & 0x0f);
	}
}