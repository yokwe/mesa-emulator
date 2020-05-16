package mh.majuro.mesa.type;

//3.1.4.3 Code Segments
//CodeSegment: TYPE = MACHINE DEPENDENT RECORD [ 
//	available(0): ARRAY [0..4) OF UNSPECIFIED,
//	code(4): BLOCK];

//APilot/15.3/Mesa/Public/PrincOps.mesa
//CodeSegment: TYPE = MACHINE DEPENDENT RECORD [
//  header: PrefixHeader];
//
//PrefixHeader: TYPE = MACHINE DEPENDENT RECORD [
//  globalFsi (0:0..7): BYTE,
//  nlinks (0:8..15): [0..maxNLinks],
//  stops (1:0..0): BOOLEAN,
//  available (1:1..15): NAT,
//  mainBodyPC(2): BytePC,
//  catchCode(3): BytePC];

public final class CodeSegment {
	public static final int SIZE = 4; // unit is word
}
