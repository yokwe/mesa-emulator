package mh.majuro.mesa;

public final class Type {
	// NibblePair: TYPE = MACHINE DEPENDENT RECORD[left (0:0..3) NIBBLE, right(0:4..7) NIBBLE];
	public static int nibblePairLeft(short value) {
		return (value >>> 4) & 0x0F;
	}
	public static int nibblePiarRight(short value) {
		return value & 0x0F;
	}
	
	// BytePair: TYPE =  MACHINE DEPENDENT RECORD [left (0: 0..7), right (0: 8..15): BYTE];
	public static int bytePairLeft(int value) {
		return (value >>> 8) & 0xFF;
	}
	public static int bytePiarRight(int value) {
		return value & 0xFF;
	}
}
