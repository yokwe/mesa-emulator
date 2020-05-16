package mh.majuro.mesa.type;

import mh.majuro.mesa.Memory;
import mh.majuro.mesa.Type.CARD16;
import mh.majuro.mesa.Type.LONG_POINTER;

//In Changed Chapter
//GlobalOverhead : TYPE =  MACHINE DEPENDENT RECORD (
//  available (0): UNSPECIFIED,
//  word      (1): GlobalWord,
//  global    (2): GlobaiVariables];

//APilot/15.0.1/Mesa/Public/PrincOpsExtra2.mesa
//ExtraGlobalWord: TYPE = MACHINE DEPENDENT RECORD [
//  unused (0:0..10): CARDINAL[0..2048),
//  started (0:11..11),
//  copy (0:12..12),
//  copied (0:13..13),
//  alloced (0:14..14),
//  shared (0:15..15): BOOLEAN];

//GlobalWord: TYPE = MACHINE DEPENDENT RECORD [
//gfi (0:0..13): GFTIndex,
//trapxfers (0:14..14),
//codelinks (0:15..15): BOOLEAN];

//GlobalOverhead: TYPE = MACHINE DEPENDENT RECORD [
//  extra (0): ExtraGlobalWord,
//  word (1): GlobalWord,
//  global (2): PrincOps.GlobalVariables];

public final class GlobalOverhead {
	public static final int SIZE = 2;
	
	public static @CARD16 int word(@LONG_POINTER int va) {
		return Memory.fetch(va + 1);
	}
	public static boolean trapxfer(@LONG_POINTER int va) {
		return GlobalWord.trapxfers(word(va));
	}
	public static boolean codelinks(@LONG_POINTER int va) {
		return GlobalWord.codelinks(word(va));
	}
	
}
