package mh.majuro.mesa.type;

import mh.majuro.mesa.Type.CARD16;

//From APilot/15.3/Mesa/Public/PrincOpsExtras2.mesa
//GlobalWord: TYPE = MACHINE DEPENDENT RECORD [
//  gfi (0:0..13): GFTIndex,
//  trapxfers (0:14..14),
//  codelinks (0:15..15): BOOLEAN];
public final class GlobalWord {
	public static final int gfi(@CARD16 int value) {
		return (value >>> 2) & 0x3FFF;
	}
	public static final boolean trapxfers(@CARD16 int value) {
		return (value & 0x0002) != 0;
	}
	public static final boolean codelinks(@CARD16 int value) {
		return (value & 0x0001) != 0;
	}
}
