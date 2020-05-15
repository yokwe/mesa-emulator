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
	public static @interface PDA_POINTER {
	}
	
	@Retention(RetentionPolicy.SOURCE)
	@Target({ElementType.TYPE_USE})
	public static @interface PAGE_NUMBER {
	}

	//FieldSpec: TYPE = MACHINE DEPENDENT RECORD [
	// pos (0:0..3):  NIBBLE,
	// size (0:4..7): NIBBLE];
	@Retention(RetentionPolicy.SOURCE)
	@Target({ElementType.TYPE_USE})
	public static @interface FIELD_SPEC {
	}

	//FieldDesc: TYPE = MACHINE DEPENDENT RECORD [
    //  offset(0:0..7) : BYTE,
    //  field(0:8..15) : FieldSpec];
	@Retention(RetentionPolicy.SOURCE)
	@Target({ElementType.TYPE_USE})
	public static @interface FIELD_DESC {
	}

	
//	// NibblePair: TYPE = MACHINE DEPENDENT RECORD[left (0:0..3) NIBBLE, right(0:4..7) NIBBLE];
//	public static @CARD16 int nibblePairLeft(@CARD16 int value) {
//		return (value >>> 4) & 0x0F;
//	}
//	public static @CARD16 int nibblePiarRight(@CARD16 int value) {
//		return value & 0x0F;
//	}
	
	// BytePair: TYPE =  MACHINE DEPENDENT RECORD [left (0: 0..7), right (0: 8..15): BYTE];
	public static @CARD8 int bytePairLeft(@CARD16 int value) {
		return (value >>> 8) & 0xFF;
	}
	public static @CARD8 int bytePairRight(@CARD16 int value) {
		return value & 0xFF;
	}
}
