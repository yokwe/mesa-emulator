package mh.majuro.mesa.type;

import mh.majuro.mesa.LFCache;
import mh.majuro.mesa.Type.CARD16;
import mh.majuro.mesa.Type.POINTER;

//LocalOverhead : TYPE = MACHINE DEPENDENT RECORD [
//  word (0):       LocalWord.
//  returnlink(1) : ShortControlLink,
//  globallink(2) : GFTHandle,
//  pc(3):          CARDINAL,
//  local(4):       LocaiVariables];
public final class LocalOverhead {
	public static int SIZE = 4;
	public static int OFFSET_WORD       = 0;
	public static int OFFSET_RETURNLINK = 1;
	public static int OFFSET_GLOBALLINK = 2;
	public static int OFFSET_PC         = 3;
	
	public static @CARD16 int pc(@POINTER int pointer) {
		return LFCache.fetch(pointer - SIZE + OFFSET_PC);
	}
	public static void pc(@POINTER int pointer, @CARD16 int newValue) {
		LFCache.store(pointer - SIZE + OFFSET_PC, newValue);
	}
}