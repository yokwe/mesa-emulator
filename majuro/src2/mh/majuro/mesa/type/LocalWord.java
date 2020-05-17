package mh.majuro.mesa.type;

import mh.majuro.mesa.Type.CARD16;
import mh.majuro.mesa.Type.CARD8;

//LocalWord: TYPE = MACHINE DEPENDENT RECORD[
//  available(0:0..7): BYTE,
//  fsi(0:8..15): FSIndex];
public final class LocalWord {
	public static @CARD8 int fsi(@CARD16 int value) {
		return value & 0x0F;
	}
}