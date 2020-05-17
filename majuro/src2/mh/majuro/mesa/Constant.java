package mh.majuro.mesa;

import mh.majuro.mesa.Type.CARD16;
import mh.majuro.mesa.Type.CARD8;
import mh.majuro.mesa.Type.LONG_POINTER;
import mh.majuro.mesa.Type.POINTER;

public final class Constant {
	public static final @CARD16 int cSS = 14;
	// cSV = SIZE[StateVector] + MAX[SIZE[ControlLink], SIZE[FSIndex], SIZE[LONG POINTER]]
	// SIZE[StateVector] = 16 and SIZE[LONG POINTER] = 2. So cSV should be 16 + 2 = 18
	public static final @CARD16 int cSV = 18;
	//public static final @CARD16 int cWM = 10; // Noone use this value - cWDC <= cWM
	public static final @CARD16 int cWDC = 7;
	//public static final @CARD16 int cTickMin = 15;
	//public static final @CARD16 int cTickMax = 60;
	// 1 tick = 40 milliseconds
	public static final @CARD16 int cTick = 40;

	public static final @LONG_POINTER int mPDA = 0x00010000; // 0200000
	public static final @LONG_POINTER int mGFT = 0x00020000; // 0400000

	public static final @POINTER int mAV  = 0x0100; //  0400
	public static final @POINTER int mSD  = 0x0200; // 01000
	public static final @POINTER int mETT = 0x0400; // 02000

	public static final @CARD16 int qFrameFault        = 0;
	public static final @CARD16 int qPageFault         = 1;
	public static final @CARD16 int qWriteProtectFault = 2;

	public static final @CARD16 int sBoot            = 001; //  1
	public static final @CARD16 int sBoundsTrap      = 016; // 14
	public static final @CARD16 int sBreakTrap       = 000; //  0
	public static final @CARD16 int sCodeTrap        = 007; //  7
	public static final @CARD16 int sControlTrap     = 006; //  6
	public static final @CARD16 int sDivCheckTrap    = 013; // 11
	public static final @CARD16 int sDivZeroTrap     = 012; // 10
	public static final @CARD16 int sInterruptError  = 014; // 12
	public static final @CARD16 int sOpcodeTrap      = 005; //  5
	public static final @CARD16 int sPointerTrap     = 017; // 15
	public static final @CARD16 int sProcessTrap     = 015; // 13
	public static final @CARD16 int sRescheduleError = 003; //  3
	public static final @CARD16 int sStackError      = 002; //  2
	public static final @CARD16 int sUnboundTrap     = 011; //  9
	public static final @CARD16 int sXferTrap        = 004; //  4
	public static final @CARD16 int sHardwareError   = 010; //  8


	// Instruction code
	public static final @CARD8 int zNOOP    = 00;
	public static final @CARD8 int zLL0     = 01;
	public static final @CARD8 int zLL1     = 02;
	public static final @CARD8 int zLL2     = 03;
	public static final @CARD8 int zLL3     = 04;
	public static final @CARD8 int zLL4     = 05;
	public static final @CARD8 int zLL5     = 06;
	public static final @CARD8 int zLL6     = 07;

	public static final @CARD8 int zLL7     = 010;
	public static final @CARD8 int zLL8     = 011;
	public static final @CARD8 int zLL9     = 012;
	public static final @CARD8 int zLL10    = 013;
	public static final @CARD8 int zLL11    = 014;
	public static final @CARD8 int zLLB     = 015;
	public static final @CARD8 int zLLD0    = 016;
	public static final @CARD8 int zLLD1    = 017;

	public static final @CARD8 int zLLD2    = 020;
	public static final @CARD8 int zLLD3    = 021;
	public static final @CARD8 int zLLD4    = 022;
	public static final @CARD8 int zLLD5    = 023;
	public static final @CARD8 int zLLD6    = 024;
	public static final @CARD8 int zLLD7    = 025;
	public static final @CARD8 int zLLD8    = 026;
	public static final @CARD8 int zLLD10   = 027;

	public static final @CARD8 int zLLDB    = 030;
	public static final @CARD8 int zSL0     = 031;
	public static final @CARD8 int zSL1     = 032;
	public static final @CARD8 int zSL2     = 033;
	public static final @CARD8 int zSL3     = 034;
	public static final @CARD8 int zSL4     = 035;
	public static final @CARD8 int zSL5     = 036;
	public static final @CARD8 int zSL6     = 037;

	public static final @CARD8 int zSL7     = 040;
	public static final @CARD8 int zSL8     = 041;
	public static final @CARD8 int zSL9     = 042;
	public static final @CARD8 int zSL10    = 043;
	public static final @CARD8 int zSLB     = 044;
	public static final @CARD8 int zSLD0    = 045;
	public static final @CARD8 int zSLD1    = 046;
	public static final @CARD8 int zSLD2    = 047;

	public static final @CARD8 int zSLD3    = 050;
	public static final @CARD8 int zSLD4    = 051;
	public static final @CARD8 int zSLD5    = 052;
	public static final @CARD8 int zSLD6    = 053;
	public static final @CARD8 int zSLD8    = 054;
	public static final @CARD8 int zPL0     = 055;
	public static final @CARD8 int zPL1     = 056;
	public static final @CARD8 int zPL2     = 057;

	public static final @CARD8 int zPL3     = 060;
	public static final @CARD8 int zPLB     = 061;
	public static final @CARD8 int zPLD0    = 062;
	public static final @CARD8 int zPLDB    = 063;
	public static final @CARD8 int zLG0     = 064;
	public static final @CARD8 int zLG1     = 065;
	public static final @CARD8 int zLG2     = 066;
	public static final @CARD8 int zLGB     = 067;

	public static final @CARD8 int zLGD0    = 070;
	public static final @CARD8 int zLGD2    = 071;
	public static final @CARD8 int zLGDB    = 072;
	public static final @CARD8 int zSGB     = 073;
	public static final @CARD8 int zBNDCK   = 074;
	public static final @CARD8 int zBRK     = 075;
	//
	public static final @CARD8 int zSTC     = 077;

	public static final @CARD8 int zR0      = 0100;
	public static final @CARD8 int zR1      = 0101;
	public static final @CARD8 int zRB      = 0102;
	public static final @CARD8 int zRL0     = 0103;
	public static final @CARD8 int zRLB     = 0104;
	public static final @CARD8 int zRD0     = 0105;
	public static final @CARD8 int zRDB     = 0106;
	public static final @CARD8 int zRDL0    = 0107;

	public static final @CARD8 int zRDLB    = 0110;
	public static final @CARD8 int zW0      = 0111;
	public static final @CARD8 int zWB      = 0112;
	public static final @CARD8 int zPSB     = 0113;
	public static final @CARD8 int zWLB     = 0114;
	public static final @CARD8 int zPSLB    = 0115;
	public static final @CARD8 int zWDB     = 0116;
	public static final @CARD8 int zPSD0    = 0117;

	public static final @CARD8 int zPSDB    = 0120;
	public static final @CARD8 int zWDLB    = 0121;
	public static final @CARD8 int zPSDLB   = 0122;
	public static final @CARD8 int zRLI00   = 0123;
	public static final @CARD8 int zRLI01   = 0124;
	public static final @CARD8 int zRLI02   = 0125;
	public static final @CARD8 int zRLI03   = 0126;
	public static final @CARD8 int zRLIP    = 0127;

	public static final @CARD8 int zRLILP   = 0130;
	public static final @CARD8 int zRLDI00  = 0131;
	public static final @CARD8 int zRLDIP   = 0132;
	public static final @CARD8 int zRLDILP  = 0133;
	public static final @CARD8 int zRGIP    = 0134;
	public static final @CARD8 int zRGILP   = 0135;
	public static final @CARD8 int zWLIP    = 0136;
	public static final @CARD8 int zWLILP   = 0137;

	public static final @CARD8 int zWLDILP  = 0140;
	public static final @CARD8 int zRS      = 0141;
	public static final @CARD8 int zRLS     = 0142;
	public static final @CARD8 int zWS      = 0143;
	public static final @CARD8 int zWLS     = 0144;
	public static final @CARD8 int zR0F     = 0145;
	public static final @CARD8 int zRF      = 0146;
	public static final @CARD8 int zRL0F    = 0147;

	public static final @CARD8 int zRLF     = 0150;
	public static final @CARD8 int zRLFS    = 0151;
	public static final @CARD8 int zRLIPF   = 0152;
	public static final @CARD8 int zRLILPF  = 0153;
	public static final @CARD8 int zW0F     = 0154;
	public static final @CARD8 int zWF      = 0155;
	public static final @CARD8 int zPSF     = 0156;
	public static final @CARD8 int zPS0F    = 0157;

	public static final @CARD8 int zWS0F    = 0160;
	public static final @CARD8 int zWL0F    = 0161;
	public static final @CARD8 int zWLF     = 0162;
	public static final @CARD8 int zPSLF    = 0163;
	public static final @CARD8 int zWLFS    = 0164;
	public static final @CARD8 int zSLDB    = 0165;
	public static final @CARD8 int zSGDB    = 0166;
	public static final @CARD8 int zLLKB    = 0167;

	public static final @CARD8 int zRKIB    = 0170;
	public static final @CARD8 int zRKDIB   = 0171;
	public static final @CARD8 int zLKB     = 0172;
	public static final @CARD8 int zSHIFT   = 0173;
	public static final @CARD8 int zSHIFTSB = 0174;
	public static final @CARD8 int zMBP     = 0175;
	public static final @CARD8 int zRBP     = 0176;
	public static final @CARD8 int zWBP     = 0177;


	public static final @CARD8 int zCATCH   = 0200;
	public static final @CARD8 int zJ2      = 0201;
	public static final @CARD8 int zJ3      = 0202;
	public static final @CARD8 int zJ4      = 0203;
	public static final @CARD8 int zJ5      = 0204;
	public static final @CARD8 int zJ6      = 0205;
	public static final @CARD8 int zJ7      = 0206;
	public static final @CARD8 int zJ8      = 0207;

	public static final @CARD8 int zJB      = 0210;
	public static final @CARD8 int zJW      = 0211;
	public static final @CARD8 int zJEP     = 0212;
	public static final @CARD8 int zJEB     = 0213;
	public static final @CARD8 int zJEBB    = 0214;
	public static final @CARD8 int zJNEP    = 0215;
	public static final @CARD8 int zJNEB    = 0216;
	public static final @CARD8 int zJNEBB   = 0217;

	public static final @CARD8 int zJLB     = 0220;
	public static final @CARD8 int zJGEB    = 0221;
	public static final @CARD8 int zJGB     = 0222;
	public static final @CARD8 int zJLEB    = 0223;
	public static final @CARD8 int zJULB    = 0224;
	public static final @CARD8 int zJUGEB   = 0225;
	public static final @CARD8 int zJUGB    = 0226;
	public static final @CARD8 int zJULEB   = 0227;

	public static final @CARD8 int zJZ3     = 0230;
	public static final @CARD8 int zJZ4     = 0231;
	public static final @CARD8 int zJZB     = 0232;
	public static final @CARD8 int zJNZ3    = 0233;
	public static final @CARD8 int zJNZ4    = 0234;
	public static final @CARD8 int zJNZB    = 0235;
	public static final @CARD8 int zJDEB    = 0236;
	public static final @CARD8 int zJDNEB   = 0237;

	public static final @CARD8 int zJIB     = 0240;
	public static final @CARD8 int zJIW     = 0241;
	public static final @CARD8 int zREC     = 0242;
	public static final @CARD8 int zREC2    = 0243;
	public static final @CARD8 int zDIS     = 0244;
	public static final @CARD8 int zDIS2    = 0245;
	public static final @CARD8 int zEXCH    = 0246;
	public static final @CARD8 int zDEXCH   = 0247;

	public static final @CARD8 int zDUP     = 0250;
	public static final @CARD8 int zDDUP    = 0251;
	public static final @CARD8 int zEXDIS   = 0252;
	public static final @CARD8 int zNEG     = 0253;
	public static final @CARD8 int zINC     = 0254;
	public static final @CARD8 int zDEC     = 0255;
	public static final @CARD8 int zDINC    = 0256;
	public static final @CARD8 int zDBL     = 0257;

	public static final @CARD8 int zDDBL    = 0260;
	public static final @CARD8 int zTRPL    = 0261;
	public static final @CARD8 int zAND     = 0262;
	public static final @CARD8 int zIOR     = 0263;
	public static final @CARD8 int zADDSB   = 0264;
	public static final @CARD8 int zADD     = 0265;
	public static final @CARD8 int zSUB     = 0266;
	public static final @CARD8 int zDADD    = 0267;

	public static final @CARD8 int zDSUB    = 0270;
	public static final @CARD8 int zADC     = 0271;
	public static final @CARD8 int zACD     = 0272;
	public static final @CARD8 int zAL0IB   = 0273;
	public static final @CARD8 int zMUL     = 0274;
	public static final @CARD8 int zDCMP    = 0275;
	public static final @CARD8 int zUDCMP   = 0276;
	public static final @CARD8 int zVMFIND  = 0277;

	public static final @CARD8 int zLI0     = 0300;
	public static final @CARD8 int zLI1     = 0301;
	public static final @CARD8 int zLI2     = 0302;
	public static final @CARD8 int zLI3     = 0303;
	public static final @CARD8 int zLI4     = 0304;
	public static final @CARD8 int zLI5     = 0305;
	public static final @CARD8 int zLI6     = 0306;
	public static final @CARD8 int zLI7     = 0307;

	public static final @CARD8 int zLI8     = 0310;
	public static final @CARD8 int zLI9     = 0311;
	public static final @CARD8 int zLI10    = 0312;
	public static final @CARD8 int zLIN1    = 0313;
	public static final @CARD8 int zLINI    = 0314;
	public static final @CARD8 int zLIB     = 0315;
	public static final @CARD8 int zLIW     = 0316;
	public static final @CARD8 int zLINB    = 0317;

	public static final @CARD8 int zLIHB    = 0320;
	public static final @CARD8 int zLID0    = 0321;
	public static final @CARD8 int zLA0     = 0322;
	public static final @CARD8 int zLA1     = 0323;
	public static final @CARD8 int zLA2     = 0324;
	public static final @CARD8 int zLA3     = 0325;
	public static final @CARD8 int zLA6     = 0326;
	public static final @CARD8 int zLA8     = 0327;

	public static final @CARD8 int zLAB     = 0330;
	public static final @CARD8 int zLAW     = 0331;
	public static final @CARD8 int zGA0     = 0332;
	public static final @CARD8 int zGA1     = 0333;
	public static final @CARD8 int zGAB     = 0334;
	public static final @CARD8 int zGAW     = 0335;
	public static final @CARD8 int zCAW     = 0336;
	public static final @CARD8 int zEFC0    = 0337;

	public static final @CARD8 int zEFC1    = 0340;
	public static final @CARD8 int zEFC2    = 0341;
	public static final @CARD8 int zEFC3    = 0342;
	public static final @CARD8 int zEFC4    = 0343;
	public static final @CARD8 int zEFC5    = 0344;
	public static final @CARD8 int zEFC6    = 0345;
	public static final @CARD8 int zEFC7    = 0346;
	public static final @CARD8 int zEFC8    = 0347;

	public static final @CARD8 int zEFC9    = 0350;
	public static final @CARD8 int zEFC10   = 0351;
	public static final @CARD8 int zEFC11   = 0352;
	public static final @CARD8 int zEFC12   = 0353;
	public static final @CARD8 int zEFCB    = 0354;
	public static final @CARD8 int zLFC     = 0355;
	public static final @CARD8 int zSFC     = 0356;
	public static final @CARD8 int zRET     = 0357;

	public static final @CARD8 int zKFCB    = 0360;
	public static final @CARD8 int zME      = 0361;
	public static final @CARD8 int zMX      = 0362;
	public static final @CARD8 int zBLT     = 0363;
	public static final @CARD8 int zBLTL    = 0364;
	public static final @CARD8 int zBLTC    = 0365;
	public static final @CARD8 int zBLTCL   = 0366;
	public static final @CARD8 int zLP      = 0367;

	public static final @CARD8 int zESC     = 0370;
	public static final @CARD8 int zESCL    = 0371;
	public static final @CARD8 int zLGA0    = 0372;
	public static final @CARD8 int zLGAB    = 0373;
	public static final @CARD8 int zLGAW    = 0374;
	public static final @CARD8 int zDESC    = 0375;

	public static final @CARD8 int zRESRVD  = 0377;

	//
	public static final @CARD8 int a00      = 00;
	public static final @CARD8 int a01      = 01;
	public static final @CARD8 int aMW      = 02;
	public static final @CARD8 int aMR      = 03;
	public static final @CARD8 int aNC      = 04;
	public static final @CARD8 int aBC      = 05;
	public static final @CARD8 int aREQ     = 06;
	public static final @CARD8 int aSM      = 07;

	public static final @CARD8 int aSMF     = 010;
	public static final @CARD8 int aGMF     = 011;
	public static final @CARD8 int aAF      = 012;
	public static final @CARD8 int aFF      = 013;
	public static final @CARD8 int aPI      = 014;
	public static final @CARD8 int aPO      = 015;
	public static final @CARD8 int aPOR     = 016;
	public static final @CARD8 int aSPP     = 017;

	public static final @CARD8 int aDI      = 020;
	public static final @CARD8 int aEI      = 021;
	public static final @CARD8 int aXOR     = 022;
	public static final @CARD8 int aDAND    = 023;
	public static final @CARD8 int aDIOR    = 024;
	public static final @CARD8 int aDXOR    = 025;
	public static final @CARD8 int aROTATE  = 026;
	public static final @CARD8 int aDSHIFT  = 027;
	public static final @CARD8 int aLINT    = 030;
	public static final @CARD8 int aJS      = 031;
	public static final @CARD8 int aRCFS    = 032;
	public static final @CARD8 int bRC      = 033;
	public static final @CARD8 int aUDIV    = 034;
	public static final @CARD8 int aLUDIV   = 035;
	public static final @CARD8 int bROB     = 036;
	public static final @CARD8 int bWOB     = 037;

	public static final @CARD8 int bDSK     = 040;
	public static final @CARD8 int bXE      = 041;
	public static final @CARD8 int bXF      = 042;
	public static final @CARD8 int bLSK     = 043;
	public static final @CARD8 int aBNDCKL  = 044;
	public static final @CARD8 int aNILCK   = 045;
	public static final @CARD8 int aNILCKL  = 046;
	public static final @CARD8 int aBLTLR   = 047;
	public static final @CARD8 int aBLEL    = 050;
	public static final @CARD8 int aBLECL   = 051;
	public static final @CARD8 int aCKSUM   = 052;
	public static final @CARD8 int aBITBLT  = 053;
	public static final @CARD8 int aTXTBLT  = 054;
	public static final @CARD8 int aBYTBLT  = 055;
	public static final @CARD8 int aBYTBLTR = 056;
	public static final @CARD8 int aVERSION = 057;

	public static final @CARD8 int aDMUL    = 060;
	public static final @CARD8 int aSDIV    = 061;
	public static final @CARD8 int aSDDIV   = 062;
	public static final @CARD8 int aUDDIV   = 063;
	public static final @CARD8 int a64      = 064;
	public static final @CARD8 int a65      = 065;
	public static final @CARD8 int a66      = 066;
	public static final @CARD8 int a67      = 067;
	public static final @CARD8 int a70      = 070;
	public static final @CARD8 int a71      = 071;
	public static final @CARD8 int a72      = 072;
	public static final @CARD8 int a73      = 073;
	public static final @CARD8 int a74      = 074;
	public static final @CARD8 int a75      = 075;
	public static final @CARD8 int a76      = 076;
	public static final @CARD8 int a77      = 077;

	// Floating Point (100B-137B are reserved)
	public static final @CARD8 int aFADD    = 0100;
	public static final @CARD8 int aFSUB    = 0101;
	public static final @CARD8 int aFMUL    = 0102;
	public static final @CARD8 int aFDIV    = 0103;
	public static final @CARD8 int aFCOMP   = 0104;
	public static final @CARD8 int aFIX     = 0105;
	public static final @CARD8 int aFLOAT   = 0106;
	public static final @CARD8 int aFIXI    = 0107;
	public static final @CARD8 int aFIXC    = 0110;
	public static final @CARD8 int aFSTICKY = 0111;
	public static final @CARD8 int aFREM    = 0112;
	public static final @CARD8 int aROUND   = 0113;
	public static final @CARD8 int aROUNDI  = 0114;
	public static final @CARD8 int aROUNDC  = 0115;
	public static final @CARD8 int aFSQRT   = 0116;
	public static final @CARD8 int aFSC     = 0117;

	// Cedar collector and allocator (140B-157B are reserved)
	//public static final @CARD8 int aRECLAIMREF            = 0140;
	//public static final @CARD8 int aALTERCOUNT            = 0141;
	//public static final @CARD8 int aRESETSTKBITS          = 0142;
	//public static final @CARD8 int aGCSETUP               = 0143;
	//public static final @CARD8 int a144                   = 0144;
	//public static final @CARD8 int aENUMERATERECLAIMABLE  = 0145;
	//public static final @CARD8 int a146                   = 0146;
	//public static final @CARD8 int aCREATEREF             = 0147;
	//public static final @CARD8 int a150                   = 0150;
	//public static final @CARD8 int aREFTYPE               = 0151;
	//public static final @CARD8 int aCANONICALREFTYPE      = 0152;
	//public static final @CARD8 int aALLOCQUANTIZED        = 0153;
	//public static final @CARD8 int aALLOCHEAP             = 0154;
	//public static final @CARD8 int aFREEOBJECT            = 0155;
	//public static final @CARD8 int aFREEQUANTIZED         = 0156;
	//public static final @CARD8 int aFREEPREFIXED          = 0157;

	//  Read / Write Registers
	public static final @CARD8 int aWRPSB   = 0160;
	public static final @CARD8 int aWRMDS   = 0161;
	public static final @CARD8 int aWRWP    = 0162;
	public static final @CARD8 int aWRWDC   = 0163;
	public static final @CARD8 int aWRPTC   = 0164;
	public static final @CARD8 int aWRIT    = 0165;
	public static final @CARD8 int aWRXTS   = 0166;
	public static final @CARD8 int aWRMP    = 0167;
	public static final @CARD8 int aRRPSB   = 0170;
	public static final @CARD8 int aRRMDS   = 0171;
	public static final @CARD8 int aRRWP    = 0172;
	public static final @CARD8 int aRRWDC   = 0173;
	public static final @CARD8 int aRRPTC   = 0174;
	public static final @CARD8 int aRRIT    = 0175;
	public static final @CARD8 int aRRXTS   = 0176;
	public static final @CARD8 int a177     = 0177;

	// Processor Dependent (200B-237B are reserved)
	//public static final @CARD8 int aINPUT   = 0200;
	//public static final @CARD8 int aOUTPUT  = 0201;
	//public static final @CARD8 int aLOADRAMJ= 0202;

	// Dandelion
	//public static final @CARD8 int aBANDBLT = 0203;

	// ESCAlphaExtra3.mesa
	//public static final @CARD8 int aLOCKQUEUE = 206B;

	// Dolphin
	//public static final @CARD8 int aRPRINTER  = 0203;
	//public static final @CARD8 int aWPRINTER  = 0204;
	//public static final @CARD8 int aREADRAM   = 0205;
	//public static final @CARD8 int aJRAM      = 0206;
	//public static final @CARD8 int aCCOPR     = 0207;
	//public static final @CARD8 int aFPOPR     = 0210;
	//public static final @CARD8 int aSTARTIO   = 0211;

	//Guam
	public static final @CARD8 int aCALLAGENT    = 0211;
	public static final @CARD8 int aMAPDISPLAY   = 0212;
	public static final @CARD8 int aSTOPEMULATOR = 0213;
	public static final @CARD8 int a214          = 0214;
	//
	public static final @CARD8 int aCOLORBLT     = 0300;
	public static final @CARD8 int a305          = 0305;
	public static final @CARD8 int a306          = 0306;
}
