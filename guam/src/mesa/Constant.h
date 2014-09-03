/*
Copyright (c) 2014, Yasuhiro Hasegawa
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
  3. The name of the author may not be used to endorse or promote products derived
     from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/


//
// Constants.h
//

#ifndef CONSTANT_H_
#define CONSTANT_H_ 1

#include "MesaBasic.h"

#include "Pilot.h"

// for compatibility
const CARD16 PageSize = Environment::wordsPerPage;

const CARD16 cSS = 14;
// cSV = SIZE[StateVector] + MAX[SIZE[ControlLink], SIZE[FSIndex], SIZE[LONG POINTER]]
// SIZE[StateVector] = 16 and SIZE[LONG POINTER] = 2. So cSV should be 16 + 2 = 18
const CARD16 cSV = 18;
//const CARD16 cWM = 10; // Noone use this value - cWDC <= cWM
const CARD16 cWDC = 7;
//const CARD16 cTickMin = 15;
//const CARD16 cTickMax = 60;

// 1 tick = 40 milliseconds
const CARD16 cTick = 40;

const CARD32 mPDA = 0x00010000; // 0200000
const CARD32 mGFT = 0x00020000; // 0400000

const CARD16 mAV  = 0x0100; //  0400
const CARD16 mSD  = 0x0200; // 01000
const CARD16 mETT = 0x0400; // 02000

const CARD16 qFrameFault        = 0;
const CARD16 qPageFault         = 1;
const CARD16 qWriteProtectFault = 2;

const CARD16 sBoot            = 001; //  1
const CARD16 sBoundsTrap      = 016; // 14
const CARD16 sBreakTrap       = 000; //  0
const CARD16 sCodeTrap        = 007; //  7
const CARD16 sControlTrap     = 006; //  6
const CARD16 sDivCheckTrap    = 013; // 11
const CARD16 sDivZeroTrap     = 012; // 10
const CARD16 sInterruptError  = 014; // 12
const CARD16 sOpcodeTrap      = 005; //  5
const CARD16 sPointerTrap     = 017; // 15
const CARD16 sProcessTrap     = 015; // 13
const CARD16 sRescheduleError = 003; //  3
const CARD16 sStackError      = 002; //  2
const CARD16 sUnboundTrap     = 011; //  9
const CARD16 sXferTrap        = 004; //  4
const CARD16 sHardwareError   = 010; //  8


// Instruction code
const CARD8 zNOOP    = 00;
const CARD8 zLL0     = 01;
const CARD8 zLL1     = 02;
const CARD8 zLL2     = 03;
const CARD8 zLL3     = 04;
const CARD8 zLL4     = 05;
const CARD8 zLL5     = 06;
const CARD8 zLL6     = 07;

const CARD8 zLL7     = 010;
const CARD8 zLL8     = 011;
const CARD8 zLL9     = 012;
const CARD8 zLL10    = 013;
const CARD8 zLL11    = 014;
const CARD8 zLLB     = 015;
const CARD8 zLLD0    = 016;
const CARD8 zLLD1    = 017;

const CARD8 zLLD2    = 020;
const CARD8 zLLD3    = 021;
const CARD8 zLLD4    = 022;
const CARD8 zLLD5    = 023;
const CARD8 zLLD6    = 024;
const CARD8 zLLD7    = 025;
const CARD8 zLLD8    = 026;
const CARD8 zLLD10   = 027;

const CARD8 zLLDB    = 030;
const CARD8 zSL0     = 031;
const CARD8 zSL1     = 032;
const CARD8 zSL2     = 033;
const CARD8 zSL3     = 034;
const CARD8 zSL4     = 035;
const CARD8 zSL5     = 036;
const CARD8 zSL6     = 037;

const CARD8 zSL7     = 040;
const CARD8 zSL8     = 041;
const CARD8 zSL9     = 042;
const CARD8 zSL10    = 043;
const CARD8 zSLB     = 044;
const CARD8 zSLD0    = 045;
const CARD8 zSLD1    = 046;
const CARD8 zSLD2    = 047;

const CARD8 zSLD3    = 050;
const CARD8 zSLD4    = 051;
const CARD8 zSLD5    = 052;
const CARD8 zSLD6    = 053;
const CARD8 zSLD8    = 054;
const CARD8 zPL0     = 055;
const CARD8 zPL1     = 056;
const CARD8 zPL2     = 057;

const CARD8 zPL3     = 060;
const CARD8 zPLB     = 061;
const CARD8 zPLD0    = 062;
const CARD8 zPLDB    = 063;
const CARD8 zLG0     = 064;
const CARD8 zLG1     = 065;
const CARD8 zLG2     = 066;
const CARD8 zLGB     = 067;

const CARD8 zLGD0    = 070;
const CARD8 zLGD2    = 071;
const CARD8 zLGDB    = 072;
const CARD8 zSGB     = 073;
const CARD8 zBNDCK   = 074;
const CARD8 zBRK     = 075;
//
const CARD8 zSTC     = 077;

const CARD8 zR0      = 0100;
const CARD8 zR1      = 0101;
const CARD8 zRB      = 0102;
const CARD8 zRL0     = 0103;
const CARD8 zRLB     = 0104;
const CARD8 zRD0     = 0105;
const CARD8 zRDB     = 0106;
const CARD8 zRDL0    = 0107;

const CARD8 zRDLB    = 0110;
const CARD8 zW0      = 0111;
const CARD8 zWB      = 0112;
const CARD8 zPSB     = 0113;
const CARD8 zWLB     = 0114;
const CARD8 zPSLB    = 0115;
const CARD8 zWDB     = 0116;
const CARD8 zPSD0    = 0117;

const CARD8 zPSDB    = 0120;
const CARD8 zWDLB    = 0121;
const CARD8 zPSDLB   = 0122;
const CARD8 zRLI00   = 0123;
const CARD8 zRLI01   = 0124;
const CARD8 zRLI02   = 0125;
const CARD8 zRLI03   = 0126;
const CARD8 zRLIP    = 0127;

const CARD8 zRLILP   = 0130;
const CARD8 zRLDI00  = 0131;
const CARD8 zRLDIP   = 0132;
const CARD8 zRLDILP  = 0133;
const CARD8 zRGIP    = 0134;
const CARD8 zRGILP   = 0135;
const CARD8 zWLIP    = 0136;
const CARD8 zWLILP   = 0137;

const CARD8 zWLDILP  = 0140;
const CARD8 zRS      = 0141;
const CARD8 zRLS     = 0142;
const CARD8 zWS      = 0143;
const CARD8 zWLS     = 0144;
const CARD8 zR0F     = 0145;
const CARD8 zRF      = 0146;
const CARD8 zRL0F    = 0147;

const CARD8 zRLF     = 0150;
const CARD8 zRLFS    = 0151;
const CARD8 zRLIPF   = 0152;
const CARD8 zRLILPF  = 0153;
const CARD8 zW0F     = 0154;
const CARD8 zWF      = 0155;
const CARD8 zPSF     = 0156;
const CARD8 zPS0F    = 0157;

const CARD8 zWS0F    = 0160;
const CARD8 zWL0F    = 0161;
const CARD8 zWLF     = 0162;
const CARD8 zPSLF    = 0163;
const CARD8 zWLFS    = 0164;
const CARD8 zSLDB    = 0165;
const CARD8 zSGDB    = 0166;
const CARD8 zLLKB    = 0167;

const CARD8 zRKIB    = 0170;
const CARD8 zRKDIB   = 0171;
const CARD8 zLKB     = 0172;
const CARD8 zSHIFT   = 0173;
const CARD8 zSHIFTSB = 0174;
const CARD8 zMBP     = 0175;
const CARD8 zRBP     = 0176;
const CARD8 zWBP     = 0177;


const CARD8 zCATCH   = 0200;
const CARD8 zJ2      = 0201;
const CARD8 zJ3      = 0202;
const CARD8 zJ4      = 0203;
const CARD8 zJ5      = 0204;
const CARD8 zJ6      = 0205;
const CARD8 zJ7      = 0206;
const CARD8 zJ8      = 0207;

const CARD8 zJB      = 0210;
const CARD8 zJW      = 0211;
const CARD8 zJEP     = 0212;
const CARD8 zJEB     = 0213;
const CARD8 zJEBB    = 0214;
const CARD8 zJNEP    = 0215;
const CARD8 zJNEB    = 0216;
const CARD8 zJNEBB   = 0217;

const CARD8 zJLB     = 0220;
const CARD8 zJGEB    = 0221;
const CARD8 zJGB     = 0222;
const CARD8 zJLEB    = 0223;
const CARD8 zJULB    = 0224;
const CARD8 zJUGEB   = 0225;
const CARD8 zJUGB    = 0226;
const CARD8 zJULEB   = 0227;

const CARD8 zJZ3     = 0230;
const CARD8 zJZ4     = 0231;
const CARD8 zJZB     = 0232;
const CARD8 zJNZ3    = 0233;
const CARD8 zJNZ4    = 0234;
const CARD8 zJNZB    = 0235;
const CARD8 zJDEB    = 0236;
const CARD8 zJDNEB   = 0237;

const CARD8 zJIB     = 0240;
const CARD8 zJIW     = 0241;
const CARD8 zREC     = 0242;
const CARD8 zREC2    = 0243;
const CARD8 zDIS     = 0244;
const CARD8 zDIS2    = 0245;
const CARD8 zEXCH    = 0246;
const CARD8 zDEXCH   = 0247;

const CARD8 zDUP     = 0250;
const CARD8 zDDUP    = 0251;
const CARD8 zEXDIS   = 0252;
const CARD8 zNEG     = 0253;
const CARD8 zINC     = 0254;
const CARD8 zDEC     = 0255;
const CARD8 zDINC    = 0256;
const CARD8 zDBL     = 0257;

const CARD8 zDDBL    = 0260;
const CARD8 zTRPL    = 0261;
const CARD8 zAND     = 0262;
const CARD8 zIOR     = 0263;
const CARD8 zADDSB   = 0264;
const CARD8 zADD     = 0265;
const CARD8 zSUB     = 0266;
const CARD8 zDADD    = 0267;

const CARD8 zDSUB    = 0270;
const CARD8 zADC     = 0271;
const CARD8 zACD     = 0272;
const CARD8 zAL0IB   = 0273;
const CARD8 zMUL     = 0274;
const CARD8 zDCMP    = 0275;
const CARD8 zUDCMP   = 0276;
const CARD8 zVMFIND  = 0277;

const CARD8 zLI0     = 0300;
const CARD8 zLI1     = 0301;
const CARD8 zLI2     = 0302;
const CARD8 zLI3     = 0303;
const CARD8 zLI4     = 0304;
const CARD8 zLI5     = 0305;
const CARD8 zLI6     = 0306;
const CARD8 zLI7     = 0307;

const CARD8 zLI8     = 0310;
const CARD8 zLI9     = 0311;
const CARD8 zLI10    = 0312;
const CARD8 zLIN1    = 0313;
const CARD8 zLINI    = 0314;
const CARD8 zLIB     = 0315;
const CARD8 zLIW     = 0316;
const CARD8 zLINB    = 0317;

const CARD8 zLIHB    = 0320;
const CARD8 zLID0    = 0321;
const CARD8 zLA0     = 0322;
const CARD8 zLA1     = 0323;
const CARD8 zLA2     = 0324;
const CARD8 zLA3     = 0325;
const CARD8 zLA6     = 0326;
const CARD8 zLA8     = 0327;

const CARD8 zLAB     = 0330;
const CARD8 zLAW     = 0331;
const CARD8 zGA0     = 0332;
const CARD8 zGA1     = 0333;
const CARD8 zGAB     = 0334;
const CARD8 zGAW     = 0335;
const CARD8 zCAW     = 0336;
const CARD8 zEFC0    = 0337;

const CARD8 zEFC1    = 0340;
const CARD8 zEFC2    = 0341;
const CARD8 zEFC3    = 0342;
const CARD8 zEFC4    = 0343;
const CARD8 zEFC5    = 0344;
const CARD8 zEFC6    = 0345;
const CARD8 zEFC7    = 0346;
const CARD8 zEFC8    = 0347;

const CARD8 zEFC9    = 0350;
const CARD8 zEFC10   = 0351;
const CARD8 zEFC11   = 0352;
const CARD8 zEFC12   = 0353;
const CARD8 zEFCB    = 0354;
const CARD8 zLFC     = 0355;
const CARD8 zSFC     = 0356;
const CARD8 zRET     = 0357;

const CARD8 zKFCB    = 0360;
const CARD8 zME      = 0361;
const CARD8 zMX      = 0362;
const CARD8 zBLT     = 0363;
const CARD8 zBLTL    = 0364;
const CARD8 zBLTC    = 0365;
const CARD8 zBLTCL   = 0366;
const CARD8 zLP      = 0367;

const CARD8 zESC     = 0370;
const CARD8 zESCL    = 0371;
const CARD8 zLGA0    = 0372;
const CARD8 zLGAB    = 0373;
const CARD8 zLGAW    = 0374;
const CARD8 zDESC    = 0375;

const CARD8 zRESRVD  = 0377;

//
const CARD8 a00      = 00;
const CARD8 a01      = 01;
const CARD8 aMW      = 02;
const CARD8 aMR      = 03;
const CARD8 aNC      = 04;
const CARD8 aBC      = 05;
const CARD8 aREQ     = 06;
const CARD8 aSM      = 07;

const CARD8 aSMF     = 010;
const CARD8 aGMF     = 011;
const CARD8 aAF      = 012;
const CARD8 aFF      = 013;
const CARD8 aPI      = 014;
const CARD8 aPO      = 015;
const CARD8 aPOR     = 016;
const CARD8 aSPP     = 017;

const CARD8 aDI      = 020;
const CARD8 aEI      = 021;
const CARD8 aXOR     = 022;
const CARD8 aDAND    = 023;
const CARD8 aDIOR    = 024;
const CARD8 aDXOR    = 025;
const CARD8 aROTATE  = 026;
const CARD8 aDSHIFT  = 027;
const CARD8 aLINT    = 030;
const CARD8 aJS      = 031;
const CARD8 aRCFS    = 032;
const CARD8 bRC      = 033;
const CARD8 aUDIV    = 034;
const CARD8 aLUDIV   = 035;
const CARD8 bROB     = 036;
const CARD8 bWOB     = 037;

const CARD8 bDSK     = 040;
const CARD8 bXE      = 041;
const CARD8 bXF      = 042;
const CARD8 bLSK     = 043;
const CARD8 aBNDCKL  = 044;
const CARD8 aNILCK   = 045;
const CARD8 aNILCKL  = 046;
const CARD8 aBLTLR   = 047;
const CARD8 aBLEL    = 050;
const CARD8 aBLECL   = 051;
const CARD8 aCKSUM   = 052;
const CARD8 aBITBLT  = 053;
const CARD8 aTXTBLT  = 054;
const CARD8 aBYTBLT  = 055;
const CARD8 aBYTBLTR = 056;
const CARD8 aVERSION = 057;

const CARD8 aDMUL    = 060;
const CARD8 aSDIV    = 061;
const CARD8 aSDDIV   = 062;
const CARD8 aUDDIV   = 063;
const CARD8 a64      = 064;
const CARD8 a65      = 065;
const CARD8 a66      = 066;
const CARD8 a67      = 067;
const CARD8 a70      = 070;
const CARD8 a71      = 071;
const CARD8 a72      = 072;
const CARD8 a73      = 073;
const CARD8 a74      = 074;
const CARD8 a75      = 075;
const CARD8 a76      = 076;
const CARD8 a77      = 077;

// Floating Point (100B-137B are reserved)
const CARD8 aFADD    = 0100;
const CARD8 aFSUB    = 0101;
const CARD8 aFMUL    = 0102;
const CARD8 aFDIV    = 0103;
const CARD8 aFCOMP   = 0104;
const CARD8 aFIX     = 0105;
const CARD8 aFLOAT   = 0106;
const CARD8 aFIXI    = 0107;
const CARD8 aFIXC    = 0110;
const CARD8 aFSTICKY = 0111;
const CARD8 aFREM    = 0112;
const CARD8 aROUND   = 0113;
const CARD8 aROUNDI  = 0114;
const CARD8 aROUNDC  = 0115;
const CARD8 aFSQRT   = 0116;
const CARD8 aFSC     = 0117;

// Cedar collector and allocator (140B-157B are reserved)
//const CARD8 aRECLAIMREF            = 0140;
//const CARD8 aALTERCOUNT            = 0141;
//const CARD8 aRESETSTKBITS          = 0142;
//const CARD8 aGCSETUP               = 0143;
//const CARD8 a144                   = 0144;
//const CARD8 aENUMERATERECLAIMABLE  = 0145;
//const CARD8 a146                   = 0146;
//const CARD8 aCREATEREF             = 0147;
//const CARD8 a150                   = 0150;
//const CARD8 aREFTYPE               = 0151;
//const CARD8 aCANONICALREFTYPE      = 0152;
//const CARD8 aALLOCQUANTIZED        = 0153;
//const CARD8 aALLOCHEAP             = 0154;
//const CARD8 aFREEOBJECT            = 0155;
//const CARD8 aFREEQUANTIZED         = 0156;
//const CARD8 aFREEPREFIXED          = 0157;

//  Read / Write Registers
const CARD8 aWRPSB   = 0160;
const CARD8 aWRMDS   = 0161;
const CARD8 aWRWP    = 0162;
const CARD8 aWRWDC   = 0163;
const CARD8 aWRPTC   = 0164;
const CARD8 aWRIT    = 0165;
const CARD8 aWRXTS   = 0166;
const CARD8 aWRMP    = 0167;
const CARD8 aRRPSB   = 0170;
const CARD8 aRRMDS   = 0171;
const CARD8 aRRWP    = 0172;
const CARD8 aRRWDC   = 0173;
const CARD8 aRRPTC   = 0174;
const CARD8 aRRIT    = 0175;
const CARD8 aRRXTS   = 0176;
const CARD8 a177     = 0177;

// Processor Dependent (200B-237B are reserved)
//const CARD8 aINPUT   = 0200;
//const CARD8 aOUTPUT  = 0201;
//const CARD8 aLOADRAMJ= 0202;

// Dandelion
//const CARD8 aBANDBLT = 0203;

// ESCAlphaExtra3.mesa
//const CARD8 aLOCKQUEUE = 206B;

// Dolphin
//const CARD8 aRPRINTER  = 0203;
//const CARD8 aWPRINTER  = 0204;
//const CARD8 aREADRAM   = 0205;
//const CARD8 aJRAM      = 0206;
//const CARD8 aCCOPR     = 0207;
//const CARD8 aFPOPR     = 0210;
//const CARD8 aSTARTIO   = 0211;

//Guam
const CARD8 aCALLAGENT    = 0211;
const CARD8 aMAPDISPLAY   = 0212;
const CARD8 aSTOPEMULATOR = 0213;
const CARD8 a214          = 0214;
//
const CARD8 aCOLORBLT     = 0300;
const CARD8 a305          = 0305;
const CARD8 a306          = 0306;

#endif
