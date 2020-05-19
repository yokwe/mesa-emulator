package mh.majuro.mesa.type;

import mh.majuro.mesa.Type.CARD16;
import mh.majuro.mesa.Type.LONG_POINTER;

// FIXME generate this source code from mesa record data

//LocalOverhead : TYPE = MACHINE DEPENDENT RECORD [
//  word (0):       LocalWord.
//  returnlink(1) : ShortControlLink,
//  globallink(2) : GFTHandle,
//  pc(3):          CARDINAL,
//  local(4):       LocaiVariables];
public final class LocalOverhead extends RecordBase {
	public static int SIZE = 4;
	public static int OFFSET_WORD       = 0;
	public static int OFFSET_RETURNLINK = 1;
	public static int OFFSET_GLOBALLINK = 2;
	public static int OFFSET_PC         = 3;
	
	// field offset
	public static @LONG_POINTER int word(@LONG_POINTER int base) {
		return base + OFFSET_WORD;
	}
	public static @LONG_POINTER int returnlink(@LONG_POINTER int base) {
		return base + OFFSET_RETURNLINK;
	}
	public static @LONG_POINTER int globallink(@LONG_POINTER int base) {
		return base + OFFSET_GLOBALLINK;
	}
	public static @LONG_POINTER int pc(@LONG_POINTER int base) {
		return base + OFFSET_PC;
	}
	public static @LONG_POINTER int word() {
		return word(0);
	}
	public static @LONG_POINTER int returnlink() {
		return returnlink(0);
	}
	public static @LONG_POINTER int globallink() {
		return globallink(0);
	}
	public static @LONG_POINTER int pc() {
		return pc(0);
	}
	
	// field get
	public static int getWord(@LONG_POINTER int base) {
		return get(LocalOverhead::word, base);
	}
	public static int getReturnlink(@LONG_POINTER int base) {
		return get(LocalOverhead::returnlink, base);
	}
	public static int getGloballink(@LONG_POINTER int base) {
		return get(LocalOverhead::globallink, base);
	}
	public static int getPc(@LONG_POINTER int base) {
		return get(LocalOverhead::pc, base);
	}
	
	// set
	public static void setWord(@LONG_POINTER int base, @CARD16 int newValue) {
		set(LocalOverhead::word, base, newValue);
	}
	public static void setReturnlink(@LONG_POINTER int base, @CARD16 int newValue) {
		set(LocalOverhead::returnlink, base, newValue);
	}
	public static void setGloballink(@LONG_POINTER int base, @CARD16 int newValue) {
		set(LocalOverhead::globallink, base, newValue);
	}
	public static void setPc(@LONG_POINTER int base, @CARD16 int newValue) {
		set(LocalOverhead::pc, base, newValue);
	}
	
	// bit field - get
	public static int getFsi(@LONG_POINTER int base) {
		return getBitField(LocalOverhead::word, LocalWord::getFsi, base);
	}
	// bit field - set
	public static void setFsi(@LONG_POINTER int base, @CARD16 int newValue) {
		setBitField(LocalOverhead::word, LocalWord::setFsi, base, newValue);
	}
}