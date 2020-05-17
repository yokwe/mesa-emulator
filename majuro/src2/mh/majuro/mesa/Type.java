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

	
	// 2.1.3.1 Basic Logical Operators
	public static @CARD16 int shift(@CARD16 int data, int count) {
		if (0 < count) {
			return (data << count) & 0xFFFF;
		} else if (count < 0) {
			return (data >>> (-count)) & 0xFFFF;
		} else {
			return data;
		}
	}
	public static @CARD16 int Rotate(@CARD16 int data, int count) {
		if (0 < count) {
			if (16 <= count) count = count % 16;
			int t = (data & 0xFFFF) << count;
			return (t & (0xFFFF)) | ((t >> 16) & 0xFFFF);
		} else if (count < 0) {
			if (count <= -16) count = -(-count % 16);
			int t = (data & 0xFFFF) << (16 + count);
			return (t & (0xFFFF)) | ((t >> 16) & 0xFFFF);
		} else {
			return data;
		}
	}

	// 2.1.3.2 Basic Arithmetic Operator
	public static @INT16 int ArithShift(@INT16 int data, int count) {
		// FIXME is this correct?
		if (0 < count) {
			return data << count;
		} else if (count < 0) {
			return data >> (-count);
		} else {
			return data;
		}
	}

	// 2.3.1 Long Types
	public static @CARD16 int lowHalf(@CARD32 int value) {
		return value & 0xFFFF;
	}
	public static @CARD16 int highHalf(@CARD32 int value) {
		return (value >>> 16) & 0xFFFF;
	}
	public static @CARD32 int makeLong(@CARD16 int high, @CARD16 int low) {
		return ((high << 16) & 0xFFFF0000) | (low & 0x0000FFFF);
	}
}
