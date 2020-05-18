package mh.majuro.mesa.type;

import mh.majuro.mesa.Type.CARD16;

//FIXME generate this source code from mesa record data

//LocalWord: TYPE = MACHINE DEPENDENT RECORD[
//  available(0:0..7): BYTE,
//  fsi(0:8..15): FSIndex];
public final class LocalWord extends RecordBase {
	// get
	public static int getAvailable(@CARD16 int value) {
		return (value >> 8) & 0x00FF;
	}
	public static int getFsi(@CARD16 int value) {
		return (value >> 0) & 0x00FF;
	}

	// set
	public static @CARD16 int setAvailable(@CARD16 int value, @CARD16 int newValue) {
		return ((newValue << 8) & 0xFF00) | (value & 0x00FF);
	}
	public static @CARD16 int setFsi(@CARD16 int value, @CARD16 int newValue) {
		return (value & 0xFF00) | ((newValue << 0) & 0x00FF);
	}
}