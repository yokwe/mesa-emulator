package mh.majuro.test;

public class OpcodeEsc123xxTest {
	// Floating Point (100B-137B are reserved)
	// 0100 FADD
	// 0101 FSUB
	// 0102 FMUL
	// 0103 FDIV
	// 0104 FCOMP
	// 0105 FIX
	// 0106 FLOAT
	// 0107 FIXI
	
	// 0110 FIXC
	// 0111 FSTICKY
	// 0112 FREM
	// 0113 ROUND
	// 0114 ROUNDI
	// 0115 ROUNDC
	// 0116 FSQRT

	//  Read / Write Registers
	// 0160 WRPSB
	// 0161 WRMDS
	// 0162 WRWP
	// 0163 WRWDC
	// 0164 WRPTC
	// 0165 WRIT
	// 0166 WRXTS
	// 0167 WRMP
	
	// 0170 RRPSB
	// 0171 RRMDS
	// 0172 RRWP
	// 0173 RRWDC
	// 0174 RRPTC
	// 0175 RRIT
	// 0176 RRXTS

	
	// Guam
	//   GuamInputOutput.mesa
	// 0211 CALLAGENT
	// 0212 MAPDISPLAY
	// 0213 STOPEMULATOR
	// 0214 A214


	// ColorBlt.mesa
	// 0300 COLORBLT
	// 0301 WRITEPIXEL
	// 0302 BITBLTX
	//
	//
	// 0305 A305
	// 0306 A306
}
