package mh.majuro.mesa;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

public final class Type {
	@Retention(RetentionPolicy.SOURCE)
	@Target({ElementType.TYPE_USE})
	public static @interface CARD8 {
	}

	@Retention(RetentionPolicy.SOURCE)
	@Target({ElementType.TYPE_USE})
	public static @interface CARD16 {
	}

	@Retention(RetentionPolicy.SOURCE)
	@Target({ElementType.TYPE_USE})
	public static @interface CARD32 {
	}

	@Retention(RetentionPolicy.SOURCE)
	@Target({ElementType.TYPE_USE})
	public static @interface INT8 {
	}
	@Retention(RetentionPolicy.SOURCE)
	@Target({ElementType.TYPE_USE})
	public static @interface INT16 {
	}

	@Retention(RetentionPolicy.SOURCE)
	@Target({ElementType.TYPE_USE})
	public static @interface POINTER {
	}

	@Retention(RetentionPolicy.SOURCE)
	@Target({ElementType.TYPE_USE})
	public static @interface LONG_POINTER {
	}

	@Retention(RetentionPolicy.SOURCE)
	@Target({ElementType.TYPE_USE})
	public static @interface PAGE_NUMBER {
	}

	
	// NibblePair: TYPE = MACHINE DEPENDENT RECORD[left (0:0..3) NIBBLE, right(0:4..7) NIBBLE];
	public static int nibblePairLeft(@CARD16 int value) {
		return (value >>> 4) & 0x0F;
	}
	public static int nibblePiarRight(@CARD16 int value) {
		return value & 0x0F;
	}
	
	// BytePair: TYPE =  MACHINE DEPENDENT RECORD [left (0: 0..7), right (0: 8..15): BYTE];
	public static int bytePairLeft(@CARD32 int value) {
		return (value >>> 8) & 0xFF;
	}
	public static int bytePiarRight(@CARD32 int value) {
		return value & 0xFF;
	}
}
