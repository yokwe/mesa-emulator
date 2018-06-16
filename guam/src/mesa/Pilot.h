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
// Pilot.h
//

#ifndef PILOT_H__
#define PILOT_H__ 1

#include "MesaBasic.h"

#include "Type.h"


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// APilot/15.3/Mesa/Public/Environment.mesa
//
class Environment {
public:
	//  bitsPerWord: CARDINAL = 16;
	static const int bitsPerWord = 16;

	//  logBitsPerWord: CARDINAL = 4; -- logarithm of bitsPerWord
	static const int logBitsPerWord = 4;

	//  bitsPerByte, bitsPerCharacter: CARDINAL = 8;
	static const int bitsPerByte = 8;
	static const int bitsPerCharacter = 8;

	//  logBitsPerByte, logBitsPerChar: CARDINAL = 3; -- logarithm of bitsPerByte
	static const int logBitsPerByte = 3;
	static const int logBitsPerChar = 3;

	//  bytesPerWord, charsPerWord: CARDINAL = bitsPerWord/bitsPerByte;
	static const int bytesPerWord = bitsPerWord / bitsPerByte;
	static const int charsPerWord = bitsPerWord / bitsPerCharacter;

	//  logBytesPerWord, logCharsPerWord: CARDINAL = logBitsPerWord - logBitsPerByte;  -- logarithm of bytesPerWord
	static const int logBytesPerWord = logBitsPerWord - logBitsPerByte;
	static const int logCharsPerWord = logBitsPerWord - logBitsPerChar;

	//  wordsPerPage: CARDINAL = 256;
	static const int wordsPerPage = 256;

	//  bytesPerPage, charsPerPage: CARDINAL = wordsPerPage*bytesPerWord;
	static const int bytesPerPage = wordsPerPage * bytesPerWord;
	static const int charsPerPage = wordsPerPage * charsPerWord;

	//  logWordsPerPage: CARDINAL = 8; -- logarithm of wordsPerPage
	static const int logWordsPerPage = 8;

	//  logBytesPerPage, logCharsPerPage: CARDINAL = logWordsPerPage + logBytesPerWord;  -- logarithm of bytesPerPage
	static const int logBytesPerPage = logWordsPerPage + logBytesPerWord;
	static const int logCharsPerPage = logWordsPerPage + logCharsPerWord;


	//PageNumber: TYPE = LONG CARDINAL --[0..2**24-2]--;
	//  -- Note that 2**24-2 is one less than the highest numbered VM page
	//  --  provided by the hardware; the highest numbered VM page, 2**24-1, is
	//  --  pre-empted for system purposes.

	//  firstPageNumber: PageNumber = 0;
	static const int firstPageNumber = 0;

	//  lastPageNumber: PageNumber = 16777214 --2**24-2--;
	static const int lastPageNumber = 16777214;


	//PageOffset: TYPE = PageNumber;
	//  firstPageOffset: PageOffset = firstPageNumber;
	static const int firstPageOffset = firstPageNumber;

	//  lastPageOffset: PageOffset = lastPageNumber;
	static const int lastPageOffset = lastPageNumber;

	//PageCount: TYPE = LONG CARDINAL --[0..2**24-1]--;
	//  firstPageCount: PageCount = 0;
	static const int firstPageCount = 0;

	//  lastPageCount: PageCount = lastPageNumber+1;
	static const int lastPageCount = lastPageNumber + 1;
};

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// APilot/15.3/Mesa/Public/ColorBlt.mesa
//
class ColorBlt {
public:

	//Address: TYPE = MACHINE DEPENDENT RECORD [
	//  word: LONG POINTER,
	//  pixel: CARDINAL];
	struct Address {
		LONG_POINTER word;
		CARDINAL     pixel;
	} __attribute__((packed));

	//PatternParm: TYPE = MACHINE DEPENDENT RECORD [
	//  -- used with Src to describe pattern brick
	//  reserved: [0..7] � 0,
	//  unpacked: BOOLEAN � FALSE, -- must be FALSE for bit source
	//  yOffset: [0..15],
	//  widthMinusOne: [0..15],
	//  -- restricted to 0 for bit source
	//  heightMinusOne: [0..15]];

	// NOTE Definition of PatternParm is wrong
	// Format of PatternParm is same as BitBlt.GrayParm
	// Below is modified PatternMap

	//PatternParm: TYPE = MACHINE DEPENDENT RECORD [
	//  unpacked(0:0..3): NIBBLE = 0,
	//  yOffset(0:4..7): NIBBLE,
	//  widthMinusOne(0:8..11): NIBBLE,
	//  heightMinusOne(0:12..15): NIBBLE];
	union PatternParm {
		CARD16 u;
		struct {
			CARD16 heightMinusOne : 4;
			CARD16 widthMinusOne  : 4;
			CARD16 yOffset        : 4;
			CARD16 unpacked       : 4;
		};
	};

	//Direction: TYPE = {forward, backward};
	static const CARD16 D_forward  = 0;
	static const CARD16 D_backword = 1;

	//PixelType: TYPE = {bit, display};
	static const CARD16 PT_bit     = 0;
	static const CARD16 PT_display = 1;

	//SrcFunc: TYPE = {null, complement};
	static const CARD16 SF_null       = 0;
	static const CARD16 SF_complement = 1;

	//DstFunc: TYPE = MACHINE DEPENDENT {src(0), srcIfDstLE1(1), srcIf0(2), srcIfDstNot0(3), srcIfNot0(4), srcIfDst0(5), pixelXor(6), srcXorDst(7)};
	//-- pixelXor: src if dst=0, else dst if src=0, else 0
	static const CARD16 DF_src          = 0;
	static const CARD16 DF_srcIfDstLE1  = 1;
	static const CARD16 DF_srcIf0       = 2;
	static const CARD16 DF_srcIfDstNot0 = 3;
	static const CARD16 DF_srcIfNot0    = 4;
	static const CARD16 DF_srcIfDst0    = 5;
	static const CARD16 DF_pixelXor     = 6;
	static const CARD16 DF_srcXorDst    = 7;

	//ColorBltFlags: TYPE = MACHINE DEPENDENT RECORD [
	//  -- determines the ColorBlt function
	//  direction: Direction � forward,
	//  srcType: PixelType � bit,
	//  dstType: PixelType � bit,
	//  pattern: BOOLEAN � FALSE,
	//  srcFunc: SrcFunc � null,
	//  dstFunc: DstFunc � src,
	//  reserved: [0..255] � 0];
	union ColorBltFlags {
		CARD16 u;
		struct {
			CARD16 reserved  : 8;
			CARD16 dstFunc   : 3;
			CARD16 srcFunc   : 1;
			CARD16 pattern   : 1;
			CARD16 dstType   : 1;
			CARD16 srcType   : 1;
			CARD16 direction : 1;
		};
	};

	//ColorMapping: TYPE = ARRAY [0..1] OF Color;
	struct ColorMapping {
		CARD16 color[2];
	};

	//Color: TYPE = NATURAL;

	//SrcDesc: TYPE = MACHINE DEPENDENT RECORD [
	//  SELECT OVERLAID * FROM
	//    pattern => [pattern: PatternParm],
	//    srcPpl => [srcPpl: INTEGER],
	//    ENDCASE];

	//CBTable, ColorBltTable: TYPE = MACHINE DEPENDENT RECORD [
	//  dst: Address,
	//  dstPpl: INTEGER,
	//  src: Address,
	//  srcDesc: SrcDesc,
	//  width: CARDINAL,
	//  height: CARDINAL,
	//  flags: ColorBltFlags,
	//  colorMapping: ColorMapping];
	struct ColorBltTable {
		Address         dst;
		INT16           dstPpl;
		Address         src;
		union {
			INT16       srcPpl;
			PatternParm pattern;
		};
		CARD16          width;
		CARD16          height;
		ColorBltFlags   flags;
		ColorMapping    colorMapping;
	};
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// APilot/15.3/Mesa/Public/LevelVKeys.mesa
//
class LevelVKeys {
public:
	  enum KeyName {
	    null             = 0,
	    Bullet           = 1,
	    SuperSub         = 2,
	    Case             = 3,
	    Strikeout        = 4,
	    KeypadTwo        = 5,
	    KeypadThree      = 6,
	    SingleQuote      = 7,
	    KeypadAdd        = 8,
	    KeypadSubtract   = 9,
	    //
	    KeypadMultiply   = 10,
	    KeypadDivide     = 11,
	    KeypadClear      = 12,
	    Point            = 13,
	    Adjust           = 14,
	    Menu             = 15,
	    Five             = 16,
	    Four             = 17,
	    Six              = 18,
	    E                = 19,
	    //
	    Seven            = 20,
	    D                = 21,
	    U                = 22,
	    V                = 23,
	    Zero             = 24,
	    K                = 25,
	    Dash             = 26,
	    P                = 27,
	    Slash            = 28,
	    Font             = 29,
	    //
	    Same             = 30,
	    BS               = 31,
	    Three            = 32,
	    Two              = 33,
	    W                = 34,
	    Q                = 35,
	    S                = 36,
	    A                = 37,
	    Nine             = 38,
	    I                = 39,
	    //
	    X                = 40,
	    O                = 41,
	    L                = 42,
	    Comma            = 43,
	    Quote            = 44,
	    RightBracket     = 45,
	    Open             = 46,
	    Special          = 47,
	    One              = 48,
	    Tab              = 49,
	    //
	    ParaTab          = 50,
	    F                = 51,
	    Props            = 52,
	    C                = 53,
	    J                = 54,
	    B                = 55,
	    Z                = 56,
	    LeftShift        = 57,
	    Period           = 58,
	    SemiColon        = 59,
	    //
	    NewPara          = 60,
	    OpenQuote        = 61,
	    Delete           = 62,
	    Next             = 63,
	    R                = 64,
	    T                = 65,
	    G                = 66,
	    Y                = 67,
	    H                = 68,
	    Eight            = 69,
	    //
	    N                = 70,
	    M                = 71,
	    Lock             = 72,
	    Space            = 73,
	    LeftBracket      = 74,
	    Equal            = 75,
	    RightShift       = 76,
	    Stop             = 77,
	    Move             = 78,
	    Undo             = 79,
	    //
	    Margins          = 80,
	    KeypadSeven      = 81,
	    KeypadEight      = 82,
	    KeypadNine       = 83,
	    KeypadFour       = 84,
	    KeypadFive       = 85,
	    English          = 86,
	    KeypadSix        = 87,
	    Katakana         = 88,
	    Copy             = 89,
	    //
	    Find             = 90,
	    Again            = 91,
	    Help             = 92,
	    Expand           = 93,
	    KeypadOne        = 94,
	    DiagnosticBitTwo = 95,
	    DiagnosticBitOne = 96,
	    Center           = 97,
	    KeypadZero       = 98,
	    Bold             = 99,
	    //
	    Italic           = 100,
	    Underline        = 101,
	    Superscript      = 102,
	    Subscript        = 103,
	    Smaller          = 104,
	    KeypadPeriod     = 105,
	    KeypadComma      = 106,
	    LeftShiftAlt     = 107,
	    DoubleQuote      = 108,
	    Defaults         = 109,
	    //
	    Hiragana         = 110,
	    RightShiftAlt    = 111
	  };
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// APilot/15.3/Faces/Friends/ProcessorFaceExtras.mesa
//
class ProcessorFaceExtras {
public:
	//MachineType: TYPE = MACHINE DEPENDENT {
	//  altoI (1), altoII (2), altoIIXM (3), dolphin (4), dorado (5), dandelion (6),
	//  dicentra (7), daybreak(8), daisy(9), kiku(10), daylight(11),
	//  tridlion(12), dahlia(13), (17B)};
	static const CARD16 MT_altoI     =  1;
	static const CARD16 MT_altoII    =  2;
	static const CARD16 MT_altoIIXM  =  3;
	static const CARD16 MT_dolphin   =  4;
	static const CARD16 MT_dorado    =  5;
	static const CARD16 MT_dandelion =  6;
	static const CARD16 MT_dicentra  =  7;
	static const CARD16 MT_daybreak  =  8;
	static const CARD16 MT_daisy     =  9;
	static const CARD16 MT_kiku      = 10;
	static const CARD16 MT_daylight  = 11;
	static const CARD16 MT_tridlion  = 12;
	static const CARD16 MT_dahlia    = 13;

	//VersionResult: TYPE = MACHINE DEPENDENT RECORD [
	//  machineType (0: 0..3): MachineType,
	//  majorVersion (0: 4..7): [0..17B],  -- incremented by incompatible changes
	//  unused (0: 8..13): [0..77B],
	//  floatingPoint (0: 14..14): BOOLEAN,
	//  cedar (0: 15..15): BOOLEAN,
	//  releaseDate (1): CARDINAL];  -- days since January 1, 1901
	struct VersionResult {
		union {
			struct {
				CARD16 cedar         : 1;
				CARD16 floatingPoint : 1;
				CARD16 unused        : 6;
				CARD16 majorVersion  : 4;
				CARD16 machineType   : 4;
			};
			CARD16 u0;
		};
		CARD16 releaseDate;
	};
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// APilot/15.0.1/Mesa/Public/Device.mesa
//
class Device {
public:
	//Type: TYPE = PRIVATE RECORD [CARDINAL];
	//nullType:  Type = [0];
	//Ethernet:  TYPE = CARDINAL [5..16);
	//Floppy: TYPE = CARDINAL [17..24);
	//PilotDisk: TYPE = CARDINAL [64..1024);

	static const CARD16 T_nullType     =  0;
	static const CARD16 T_ethernet     =  6;
	static const CARD16 T_anyFloppy    = 17;
	static const CARD16 T_anyPilotDisk = 64;

	// APilot/15.3/Faces/Private/FloppyHeadGuam.mesa
	//   microFloppy: Device.Type = [23]; -- 1.44MB, 3 1/2" disks
	//   microFloppyQuad: Device.Type = [22]; -- 2.88MB, 3 1/2" disks
	static const CARD16 T_microFloppy     = 23;
	static const CARD16 T_microFloppyQuad = 22;

	// APilot/15.3/Pilot/Public/DeviceTypesExtras11.mesa
    //   Stream: TYPE = CARDINAL [4000..4010);
    //   simpleDataStream: Device.Type = [FIRST[Stream]]; -- Guam Stream.
	static const CARD16 T_simpleDataStream = 4000;
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// APilot/15.0.1/Mesa/Public/PrincOps.mesa
//
class PrincOps {
public:
	//  FSIndex: TYPE = CARDINAL[0..256);
	//  AllocationVector: TYPE = ARRAY FSIndex OF AVItem;
	//  AVHandle: TYPE = POINTER TO AllocationVector;
	//  AV: AVHandle = LOOPHOLE[400B];
	//
	//  AVHeapSize: CARDINAL = 40B;
	//  AVHeap: TYPE = ARRAY [0..AVHeapSize) OF AVItem;

	// AVHeapSize is actual number of AVItem in AV
	static const CARD32 AVHeapSize = 32; // 040

	//AVHeap: TYPE = ARRAY [0..AVHeapSize) OF AVItem;
	//LastAVHeapSlot: CARDINAL = AVHeapSize - 2;
	static const CARD32 LastAVHeapSlot = AVHeapSize - 2;

	//FrameSizeIndex: TYPE = [0..LastAVHeapSlot];
	//FrameVec, frameSizeMap: ARRAY FrameSizeIndex OF [0..MaxFrameSize] = [
	// 8, 12, 16, 20, 24, 28, 32, 40, 48, 56, 68, 80, 96, 112, 128, 148, 168, 192,
	// 224, 252, 508, 764, 1020, 1276, 1532, 1788, 2044, 2556, 3068, 3580, 4092];
	static const int FrameSizeMap[LastAVHeapSlot + 1];
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// APilot/15.0.1/Mesa/Public/VM.mesa
//
class VM {
public:
	//Interval: TYPE = RECORD [page: Environment.PageNumber, count: PageCount];
	struct Interval {
		CARD32 page;
		CARD32 count;
	};

	//Swappability: TYPE = {resident, swappable};
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// APilot/15.0.1/Mesa/Public/Space.mesa
//

class Space {
public:
	//Usage: TYPE = [0..2048);

	//Class: TYPE = MACHINE DEPENDENT{
	//  unknown(0), code(1), globalFrame(2), localFrame(3), zone(4), file(5), data(6),
	//  spareA(7), spareB(8), pilotResident(31)};

	//SwapUnitType: TYPE = {unitary, uniform, irregular};

	//Access: TYPE = {readWrite, readOnly};

	//Life: TYPE = {alive, dead};

};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// APilot/15.3/Faces/Public/ColorDisplayFace.mesa
//
class ColorDisplayFace {
public:
	// white: Color = 0;
	// black: Color = 1;

	static const CARD16 white = 0;
	static const CARD16 black = 1;
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// APilot/15.0.1/Pilot/Friends/BackingStore.mesa
//
class BackingStore {
public:
	//Data: TYPE = RECORD [
	//  data: ARRAY [0..7) OF UNSPECIFIED];
	//-- Describes backing storage of a run of virtual memory pages.
	//-- (Multiple coexisting implementations of this interface preclude the use of
	//--  an opaque type.)
	struct Data {
		CARD16 data[7];
	} __attribute__((packed));
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.0.1/Pilot/Private/VMData.mesa
//
class VMData {
public:
	//State: TYPE = RECORD [
	//  access: Space.Access,
	//  life: Space.Life,
	//  swappability: VM.Swappability];

	// Space.Access    => 2 bit
	// Space.Life      => 1 bit
	// VM.Swappability => 1 bit
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.0.1/Pilot/Private/VMDataInternal.mesa
//

class VMDataInternal {
public:
	//SwapUnitAvailability: TYPE = {available, busy};

	//SwapUnitState: TYPE = RECORD [  -- 4 bits.
	//  availability: SwapUnitAvailability,
	//  st: VMData.State];

	// 14 bits
	//PUniformSwapUnitData: TYPE = SwapUnitBase RELATIVE ORDERED POINTER
	//  --PSwapUnitDomain-- [0..037777B] TO UniformSwapUnitData;

	// 14 bits
	//PIrregularSwapUnitData: TYPE = SwapUnitBase RELATIVE ORDERED POINTER
	//  --PSwapUnitDomain-- [0..037777B] TO IrregularSwapUnitData;

	// 8 bits
	//TransferProcID: TYPE = [0..256);

	//Run: TYPE = RECORD [
	//  interval: VM.Interval,
	//  -- Attributes of the map unit containing this Run:
	//  usage: Space.Usage,  -- 11 bits.
	//  class: Space.Class,  -- 5 bits.
	//  transferProcID: TransferProcID,
	//  -- Data for this Run:
	//  startOfMapUnit: BOOLEAN,
	//  maybePartlyIn: BOOLEAN,
	//  endOfMapUnit: BOOLEAN,
	//  backingData: BackingStore.Data,
	//  -- swap unit structure of this Run:
	//  swapUnitData: SELECT swapUnits: Space.SwapUnitType FROM
	//    unitary => [swapUnitState: SwapUnitState],  --data is stored in run entry
	//    uniform => [uniformSwapUnitData: PUniformSwapUnitData],
	//    irregular => [irregularSwapUnitData: PIrregularSwapUnitData],
	//    ENDCASE];
	struct Run {
		// 0..3
		VM::Interval interval;
		// 4
		union {
			struct {
				CARD16 spaceClass :  5;
				CARD16 spaceUsage : 11;
			};
			CARD16 u4;
		};
		// 5
		union {
			struct {
				CARD16 filler         : 5;
				CARD16 endOfMapUnit   : 1;
				CARD16 maybePartlyIn  : 1;
				CARD16 startOfMapUnit : 1;
				CARD16 transferProcID : 8;
			};
			CARD16 u5;
		};
		// 6..12
		BackingStore::Data backingData;
		// 13
		union {
			struct {
				CARD16 filler1               : 10;
				CARD16 swapUnitState         :  4; // SwapUnitState
				CARD16 swapUnits             :  2; // Space.SwapUnitType
			} swapUnitState;
			struct {
				CARD16 uniformSwapUnitData   : 14; // PUniformSwapUnitData
				CARD16 swapUnits             :  2; // Space.SwapUnitType
			} uniformSwapUnitData;
			struct {
				CARD16 irregularSwapUnitData : 14; // PIrregularSwapUnitData
				CARD16 swapUnits             :  2; // Space.SwapUnitType
			} irregularSwapUnitData;
		} swapUnitData;
	} __attribute__((packed));

	//countRunPad: CARDINAL = 1;
	static const CARD16 countRunPad = 1;
	//PRun: TYPE = RunBase RELATIVE ORDERED POINTER [0..LAST[CARDINAL]] TO Run;
	//pRunFirst: PRun = FIRST[PRun] + countRunPad*SIZE[Run];  -- first real entry of table
	static const CARD16 pRunFirst = 0 + countRunPad * SIZE(Run);
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// APilot/15.0.1/Mesa/Friends/TimeStamp.mesa
//
class TimeStamp {
public:
	//Stamp: TYPE = RECORD [net, host: [0..377B], time: LONG CARDINAL];
	//Null: Stamp = Stamp[net: 0, host: 0, time: 0];

	struct Stamp {
		union {
			struct {
				CARD16 host : 8;
				CARD16 net  : 8;
			} __attribute__((packed));
			CARD16    u0;
		};
		CARD32 time;
//		CARD16 timeLow;
//		CARD16 timeHigh;
	} __attribute__((packed));
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// APilot/15.0.1/Mesa/Friends/BcdDefs.mesa
//
class BcdDefs {
public:
	// From BcdDefsExtras.mesa
	//GFTVersionID: CARDINAL = 12206;
	//Spare4: TYPE = PACKED ARRAY [0..4) OF BOOLEAN;
	//NewMesaModule: Spare4 = [TRUE, FALSE, FALSE, FALSE];
	//OldMesaModule: Spare4 = ALL[FALSE];
	static const CARD16 GFTVersionID  = 12206;
	static const CARD16 NewMesaModule =  0x04;
	static const CARD16 OldMesaModule =  0x00;

	// VersionID: CARDINAL = 6103;
	static const CARD16 VersionID = 6103;

	// tLimit: CARDINAL = 177777B; -- was Table.Limit-1;
	static const CARD16 tLimit = 65535U;

	// define XXIndex
	typedef CARD16 ATIndex;
	typedef CARD16 CTIndex;
	typedef CARD16 ENIndex;
	typedef CARD16 EXPIndex;
	typedef CARD16 FPIndex;
	typedef CARD16 FTIndex;
	typedef CARD16 GFIndex;
	typedef CARD16 IMPIndex;
	typedef CARD16 LFIndex;
	typedef CARD16 MTIndex;
	typedef CARD16 NTIndex;
	typedef CARD16 SGIndex;
	typedef CARD16 SPIndex;
	typedef CARD16 TMIndex;
	typedef CARD16 TYPIndex;
	typedef CARD16 PSIndex;

	typedef CARD16 NameRecord;

	//Namee: TYPE = RECORD [
	//  SELECT type: * FROM
	//    config => [cti: CTIndex],
	//    module => [mti: MTIndex],
	//    import => [impi: IMPIndex],
	//    export => [expi: EXPIndex]
	//    ENDCASE];
	static const CARD16 N_config = 0;
	static const CARD16 N_module = 1;
	static const CARD16 N_import = 2;
	static const CARD16 N_export = 3;
	struct Namee {
		CARD16 type;
		union {
			CTIndex  cti;
			MTIndex  mti;
			IMPIndex impi;
			EXPIndex expi;
			CARD16   u1;
		};
	};

	//NameRecord: TYPE = RECORD [CARDINAL];
	//NullName: NameRecord = [1];
	static const CARD16 NullName = 1;

	//NTIndex: TYPE = Table.Base RELATIVE POINTER [0..tLimit] TO NTRecord;
	//NTNull: NTIndex = LAST[NTIndex];
	static const CARD16 NTNull = tLimit;


	//-- Configuration Table
	//
	//CTRecord: TYPE = --MACHINE DEPENDENT-- RECORD [
	//  name: NameRecord,
	//  file: FTIndex,
	//  config: CTIndex,
	//  namedInstance: BOOLEAN,
	//  nControls: NATURAL,
	//  controls: ARRAY [0..0) OF Namee];  -- only config or module are valid
	struct CTRecord {
		NameRecord name;
		FTIndex file;
		CTIndex config;
		union {
			struct {
				CARD16 nControls     : 15;
				CARD16 namedInstance :  1;
			};
			CARD16 u3;
		};
		Namee controls[0];
	};

	//CTIndex: TYPE = Table.Base RELATIVE POINTER [0..tLimit] TO CTRecord;
	//CTNull: CTIndex = LAST[CTIndex];
	static const CARD16 CTNull = tLimit;


	//FTRecord: TYPE = RECORD [name: NameRecord, version: VersionStamp];
	struct FTRecord {
		CARD16           name;
		TimeStamp::Stamp version;
	} __attribute__((packed));
	//FTIndex: TYPE = Table.Base RELATIVE POINTER [0..tLimit] TO FTRecord;
	//FTNull: FTIndex = LAST[FTIndex];
	//FTSelf: FTIndex = LAST[FTIndex] - 1;
	static const CARD16 FTNull = tLimit;
	static const CARD16 FTSelf = tLimit - 1;

	//SegClass: TYPE = {code, symbols, acMap, other};
	static const CARD16 SC_code    = 0;
	static const CARD16 SC_symbols = 1;
	static const CARD16 SC_acMap   = 2;
	static const CARD16 SC_other   = 3;

	//SGRecord: TYPE = RECORD [
	//  file: FTIndex, base: CARDINAL,
	//  pages: [0..256), extraPages: [0..64), class: SegClass];
	struct SGRecord {
		CARD16 file;
		CARD16 base;
		union {
			struct {
				CARD16 segClass   : 2;
				CARD16 extraPages : 6;
				CARD16 pages      : 8;
			};
			CARD16 u2;
		};
	};

	//Link: TYPE = MACHINE DEPENDENT RECORD [
	//  rep(0): SELECT tag(0:0..1): LinkTag FROM
	//    procedure => [gfi(0:2..15): GFIndex, ep(1): CARDINAL],
	//    signal => [gfi(0:2..15): GFIndex, index(1): CARDINAL],
	//    variable => [gfi(0:2..15): GFIndex, offset(1): CARDINAL],
	//    type => [fill(0:2..15): [0..37777B], typeID(1): TYPIndex],
	//    ENDCASE];
	//NullLink, nullLink: Link = [procedure[0, 0]];
	//UnboundLink, unboundLink: Link = [variable[0, 0]];
	static const CARD16 LT_prodecure = 0;
	static const CARD16 LT_signal    = 1;
	static const CARD16 LT_variable  = 2;
	static const CARD16 LT_type      = 3;
	struct Link {
		union {
			struct {
				CARD16 gfi : 14;
				CARD16 tag :  2;
			};
			CARD16 u0;
		};
		union {
			CARD16 ep;     // procedure
			CARD16 index;  // signal
			CARD16 offset; // variable
			CARD16 typeID; // type
			CARD16 u1;
		};
	};

	//LinkFrag: TYPE = RECORD [frag: SEQUENCE length: NAT OF Link];
	struct LinkFrag {
		CARD16 length;
		Link   frag[0];
	};

	//  LFIndex: TYPE = Table.Base RELATIVE POINTER [0..tLimit] TO LinkFrag;
	//  LFNull: LFIndex = LAST[LFIndex];
	static const CARD16 LFNull = tLimit;

	//-- Module Table
	//LinkLocation: TYPE = {frame, code, dontcare};
	static const CARD16 LL_frame    = 0;
	static const CARD16 LL_code     = 1;
	static const CARD16 LL_dontcare = 2;

	//CodeDesc: TYPE = RECORD [
	//  sgi: SGIndex, offset, length: CARDINAL];
	struct CodeDesc {
		SGIndex sgi;
		CARD16  offset;
		CARD16  length;
	} __attribute__((packed));

	//MTRecord: TYPE = --MACHINE DEPENDENT-- RECORD [
	//  name: NameRecord,
	//  file: FTIndex,
	//  config: CTIndex,
	//  code: CodeDesc,
	//  sseg: SGIndex,
	//  links: LFIndex,
	//  linkLoc: LinkLocation,
	//  namedInstance, initial: BOOLEAN,
	//  boundsChecks, nilChecks: BOOLEAN,
	//  tableCompiled, residentFrame, crossJumped, packageable: BOOLEAN,
	//  packed: BOOLEAN, linkspace: BOOLEAN,
	//  spare: PACKED ARRAY [0..4) OF BOOLEAN,
	//  framesize: [0..PrincOps.MaxFrameSize),
	//  entries: ENIndex,
	//  atoms: ATIndex];
	struct MTRecord {
		NameRecord name;
		FTIndex    file;
		CTIndex    config;
		CodeDesc   code;
		SGIndex    sseg;
		LFIndex    links;
		union {
			struct {
				CARD16 spare         : 4;
				CARD16 linkspace     : 1;
				CARD16 packed        : 1;
				CARD16 packageable   : 1;
				CARD16 crossJumped   : 1;
				CARD16 residentFrame : 1;
				CARD16 tableCompiled : 1;
				CARD16 nilChecks     : 1;
				CARD16 boundsChecks  : 1;
				CARD16 initial       : 1;
				CARD16 namedInstance : 1;
				CARD16 linkLoc       : 2;
			};
			CARD16 u6;
		} __attribute__((packed));
		CARD16  framesize;
		ENIndex entries;
		ATIndex atoms;
	} __attribute__((packed));

	//MTIndex: TYPE = Table.Base RELATIVE POINTER [0..tLimit] TO MTRecord;
	//MTNull: MTIndex = LAST[MTIndex];
	static const CARD16 MTNull = tLimit;


	//-- Entry Vector Table
	//ENRecord: TYPE = RECORD [
	//   nEntries: CARDINAL, initialPC: ARRAY [0..0) OF PrincOps.BytePC];
	struct ENRecord {
		CARD16 nEntries;
		CARD16 ininialPC[0];
	} __attribute__((packed));
	//ENIndex: TYPE = Table.Base RELATIVE POINTER [0..tLimit] TO ENRecord;
	//ENNull: ENIndex = LAST[ENIndex];
	static const CARD16 ENNull = tLimit;

	//-- Portable Type
	//Portable: TYPE = {module, interface};
	static const CARD16 P_module    = 0;
	static const CARD16 P_interface = 1;

	//IMPRecord: TYPE = RECORD [
	//  name: NameRecord,
	//  port: Portable,
	//  namedInstance: BOOLEAN,
	//  file: FTIndex,
	//  gfi: GFIndex];
	struct IMPRecord {
		CARD16 name;
		union {
			struct {
				CARD16 filler        : 14;
				CARD16 namedInstance :  1;
				CARD16 port          :  1;
			};
			CARD16 u1;
		};
		CARD16 file;
		CARD16 gfi;
	} __attribute__((packed));


	//EXPRecord: TYPE = --MACHINE DEPENDENT-- RECORD [
	//  name: NameRecord,
	//  size: [0..377b],
	//  port: Portable,
	//  namedInstance, typeExported: BOOLEAN,
	//  file: FTIndex,
	//  links: ARRAY [0..0) OF Link];
	struct EXPRecord {
		CARD16 name;
		union {
			struct {
				CARD16 filler        : 5;
				CARD16 typeExported  : 1;
				CARD16 namedInstance : 1;
				CARD16 port          : 1;
				CARD16 size          : 8;
			};
			CARD16 u1;
		};
		CARD16 file;
		Link links[0];
	} __attribute__((packed));


	//BCD: TYPE = RECORD [
	//  versionIdent: CARDINAL,
	//  version: VersionStamp,
	//  creator: VersionStamp,
	//  sourceFile: FTIndex,
	//  unpackagedFile: FTIndex,
	//  nConfigs, nModules: CARDINAL,
	//  nImports, nExports: CARDINAL,
	//  nPages: [0..256),
	//  definitions, repackaged, typeExported, tableCompiled: BOOLEAN,
	//  spare: PACKED ARRAY [0..4) OF BOOLEAN,
	//  firstdummy: GFIndex,
	//  nDummies: CARDINAL,
	//  ssOffset: CARDINAL, -- string table
	//  ssLimit: CARDINAL,
	//  ctOffset: CARDINAL, -- config table
	//  ctLimit: CTIndex,
	//  mtOffset: CARDINAL, -- module table
	//  mtLimit: MTIndex,
	//  impOffset: CARDINAL, -- import table
	//  impLimit: IMPIndex,
	//  expOffset: CARDINAL, -- export table
	//  expLimit: EXPIndex,
	//  enOffset: CARDINAL, -- entry table
	//  enLimit: ENIndex,
	//  sgOffset: CARDINAL, -- segment table
	//  sgLimit: SGIndex,
	//  ftOffset: CARDINAL, -- file table
	//  ftLimit: FTIndex,
	//  spOffset: CARDINAL, -- space table
	//  spLimit: SPIndex,
	//  ntOffset: CARDINAL, -- name table
	//  ntLimit: NTIndex,
	//  typOffset: CARDINAL, -- type table
	//  typLimit: TYPIndex,
	//  tmOffset: CARDINAL, -- type map table
	//  tmLimit: TMIndex,
	//  fpOffset: CARDINAL, -- frame pack table
	//  fpLimit: FPIndex,
	//  lfOffset: CARDINAL, -- link fragment table
	//  lfLimit: LFIndex,
	//  atOffset: CARDINAL, -- atom table
	//  atLimit: ATIndex,
	//  apOffset: CARDINAL, -- atom printname table
	//  apLimit: CARDINAL];
	struct BCD {
		CARD16 versionIdent;
		TimeStamp::Stamp version;
		TimeStamp::Stamp creator;
		CARD16 sourceFile;
		CARD16 unpackagedFile;
		CARD16 nConfigs;
		CARD16 nModules;
		CARD16 nImports;
		CARD16 nExports;
		union {
			struct {
				CARD16 spare : 4;
				CARD16 tableCompiled : 1;
				CARD16 typeExported  : 1;
				CARD16 repackaged    : 1;
				CARD16 definitions   : 1;
				CARD16 nPages        : 8;
			};
			CARD16 flags;
		};
		GFIndex  firstdummy;
		CARD16   nDummies;
		CARD16   ssOffset;
		CARD16   ssLimit;
		CARD16   ctOffset;
		CTIndex  ctLimit;
		CARD16   mtOffset;
		MTIndex  mtLimit;
		CARD16   impOffset;
		IMPIndex impLimit;
		CARD16   expOffset;
		EXPIndex expLimit;
		CARD16   enOffset;
		ENIndex  enLimit;
		CARD16   sgOffset;
		SGIndex  sgLimit;
		CARD16   ftOffset;
		FTIndex  ftLimit;
		CARD16   spOffset;
		SPIndex  spLimit;
		CARD16   ntOffset;
		NTIndex  ntLimit;
		CARD16   typOffset;
		TYPIndex typLimit;
		CARD16   tmOffset;
		TMIndex  tmLimit;
		CARD16   fpOffset;
		FPIndex  fpLimit;
		CARD16   lfOffset;
		LFIndex  lfLimit;
		CARD16   atOffset;
		ATIndex  atLimit;
		CARD16   apOffset;
		CARD16   apLimit;
	} __attribute__((packed));
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// APilot/15.0.1/Mesa/Friends/SDDefs.mesa
//
class SDDefs {
public:
	static const CARD16 sGermChecksum     = 0120;
	static const CARD16 sSystemDate       = 0121;
	static const CARD16 sFirstPilot       = 0260;
	static const CARD16 sFirstGermRequest = 0320;
	static const CARD16 sLastGermRequest  = 0377;
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// APilot/15.0.1/Pilot/Public/Pilot.bootmesa
//
class PilotBootmesa {
public:
	//-- Priority levels 3, 4, 6, and 7 are reserved for Pilot.  If you add a
	//-- faultable process at one of those levels, you must also add a state
	//-- vector for it.
	//STATEVECTORCOUNT: 0, 1;  -- Client Low.
	//STATEVECTORCOUNT: 1, 3;  -- Client Main.
	//STATEVECTORCOUNT: 2, 2;  -- Client High.
	//STATEVECTORCOUNT: 3, 2;  -- Pilot Realtime Swappable: NetworkStream processes.
	//STATEVECTORCOUNT: 4, 4;
	//  -- Prio 4 = PageFaultLow and PageFaultHigh combined:
	//  -- 1 for basic Swapper processes + 1 for OperationHandler + 1 for FloppySpace +
	//  -- 1 for process calling the debugger (typically faults when forked).
	//STATEVECTORCOUNT: 5, 1;  -- Faultable real time devices
	//STATEVECTORCOUNT: 6, 3;  -- Real Time Processes + 1 for SA800Requester.
	//STATEVECTORCOUNT: 7, 1;  -- FrameFaultProcess.
	static const int StateVectorCountMap[8];

	// From Pilot.bootmesa
	//FRAMEWEIGHT: 0, 20;
	//FRAMEWEIGHT: 1, 26;
	//FRAMEWEIGHT: 2, 15;
	//FRAMEWEIGHT: 3, 16;
	//FRAMEWEIGHT: 4, 16;
	//FRAMEWEIGHT: 5, 12;
	//FRAMEWEIGHT: 6, 8;
	//FRAMEWEIGHT: 7, 8;
	//FRAMEWEIGHT: 8, 5;
	//FRAMEWEIGHT: 9, 5;
	//FRAMEWEIGHT: 10, 7;
	//FRAMEWEIGHT: 11, 2;
	//FRAMEWEIGHT: 12, 2;
	//FRAMEWEIGHT: 13, 1;
	//FRAMEWEIGHT: 14, 1;
	//FRAMEWEIGHT: 15, 1;
	//FRAMEWEIGHT: 16, 1;
	//FRAMEWEIGHT: 17, 1;
	//FRAMEWEIGHT: 18, 1;
	//FRAMEWEIGHT: 19, 0;
	//FRAMEWEIGHT: 20, 0;
	//FRAMEWEIGHT: 21, 0;
	//FRAMEWEIGHT: 22, 0;
	//FRAMEWEIGHT: 23, 0;
	//FRAMEWEIGHT: 24, 0;
	//FRAMEWEIGHT: 25, 0;
	//FRAMEWEIGHT: 26, 0;
	//FRAMEWEIGHT: 27, 0;
	//FRAMEWEIGHT: 28, 0;
	//FRAMEWEIGHT: 29, 0;
	//FRAMEWEIGHT: 30, 0;  -- "30" = PrincOps.LastAVSlot.
	static const int FrameWeightMap[PrincOps::AVHeapSize - 1];
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.0.1/Pilot/Public/System.mesa
//
class System {
public:
	//Switches: TYPE = PACKED ARRAY CHARACTER OF UpDown;
	//UpDown: TYPE = MACHINE DEPENDENT {up (0), down (1)};
	//defaultSwitches: Switches = ALL[up];
	struct Switches {
		CARD16 word[16]; // Array of 256 bits
	};

	// UniversalID: TYPE [5];
	struct UniversalID {
		CARD16 a, b, c, d, e;
	};

	struct NetworkNumber {
		CARD16 word[2];
	} __attribute__((packed));
	struct HostNumber {
		CARD16 word[3];
	} __attribute__((packed));
	struct SocketNumber {
		CARD16 word[1];
	} __attribute__((packed));

	//NetworkAddress: TYPE = MACHINE DEPENDENT RECORD [
	//   net: NetworkNumber,
	//   host: HostNumber,
	//   socket: SocketNumber];
	struct NetworkAddress {
		NetworkNumber net;
		HostNumber    host;
		SocketNumber  socket;
	} __attribute__((packed));
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.3/Faces/Friends/PilotDiskFace.mesa
//
class PilotDiskFace {
public:
	//DiskAddress: TYPE = MACHINE DEPENDENT RECORD [
	//  cylinder(0): CARDINAL, head(1:0..7): [0..256), sector(1:8..15): [0..256)];
	struct DiskAddress {
		CARD16 cylinder;
		union {
			struct {
				CARD16 sector : 8;
				CARD16 head   : 8;
			};
			CARD16 u1;
		};
	};

	//Operation: TYPE = MACHINE DEPENDENT RECORD [
	//  clientHeader(0): DiskAddress,  -- address of first sector of request
	//  reserved1(2): LONG POINTER � NIL,
	//  -- reserved1 reserves space for backwards compatibility.  MUST BE NIL
	//  dataPtr(4): LONG POINTER,  -- first (page aligned) data address of operation
	//  incrementDataPtr(6:0..0): BOOLEAN,  -- if TRUE, add wordsPerPage to dataPtr
	//  -- after each successful data write
	//  enableTrackBuffer(6:1..1): BOOLEAN � FALSE,
	//  command(6:2..7): Command,  -- six bits for backward compatibility
	//  tries(6:8..15): Tries,
	//    -- Indication of how hard the Face should retry the operation in the presence
	//    -- of errors.  Increasing values of tries indicates that non-decreasing
	//    -- effort should be expended in retrying the failing operation.  Retries
	//    -- include device specific recovery (ECC, recalibrates, ...)
	//  pageCount(7): CARDINAL,  -- sectors remaining for this operation.
	//  deviceStatus(10B): RECORD [a, b: UNSPECIFIED] � NULL,
	//  diskHeader(12B): DiskAddress � NULL,  -- readHeader commands put header here.
	//  device(14B): DeviceHandle];
	struct Operation {
		DiskAddress clientHeader;     // address of first sector of request
		CARD32 reserved1;             // LONG POINER = NIL
		CARD32 dataPtr;               // first (page aligned) data address of operation
		union {
			struct {
				CARD16 tries   : 8;           // Tries
				CARD16 command : 6;           // Command
				CARD16 enableTrackBuffer : 1; // FALSE
				CARD16 incrementDataPtr  : 1; // if TRUE, add wordsPerPage to dataPtr after each successful data write
			};
			CARD16 u6;
		};
		CARD16 pageCount;             // sectors remaining for this operation
		CARD32 deviceStatus;          // RECORD[a, b: UNSPEC] = NULL
		DiskAddress diskHeader;       // NULL
		CARD16 device;                // DeviceHandle = TYPE[1]
	} __attribute__((packed));

	//Command: TYPE = MACHINE DEPENDENT {noOp(0), read(1), write(2), verify(3),
	//  format(4), readHeader(5), readHeaderAndData(6), makeBootable(7),
	//  makeUnbootable(8), getBootLocation(9), reserved10(10), reserved11(11),
	//  reserved12(12), reserved13(13), reserved14(14), reserved15(15),
	//  reserved16(16), reserved17(17), reserved18(18), reserved19(19)};
	//-- Use of reserved10 thru reserved19 are available for knowledgeable machine
	//-- dependent clients (eg. diagnostics) which use them with cooperating heads.
	enum Command {
		noOp = 0, read = 1, write = 2, verify = 3,
		format = 4, readHeader = 5, readHeaderAndData = 6, makeBootable = 7,
		makeUnbootable = 8, getBootLocation = 9, reserved10 = 10, reserved11 = 11,
		reserved12 = 12, reserved13 = 13, reserved14 = 14, reserved15 = 15,
		reserved16 = 16, reserved17 = 17, reserved18 = 18, reserved19 = 19
	};

	//Status: TYPE = MACHINE DEPENDENT {
	//  inProgress(0), goodCompletion(1), notReady(2), recalibrateError(3),
	//  seekTimeout(4), headerCRCError(5), reserved6(6), dataCRCError(7),
	//  headerNotFound(8), reserved9(9), dataVerifyError(10), overrunError(11),
	//  writeFault(12), memoryError(13), memoryFault(14), clientError(15),
	//  operationReset(16), otherError(17)};
	//-- reserved6 and reserved9 are provided for backwards compatibility.
	enum Status {
		inProgress = 0, goodCompletion = 1, notReady = 2, recalibrateError = 3,
		seekTimeout = 4, headerCRCError = 5, reserved6 = 6, dataCRCError = 7,
		headerNotFount = 8, reserved9 = 9, dataVerifyError = 10, overrunError = 11,
		writeFault = 12, memoryError = 13, memoryFault = 14, clientError = 15,
		operationReset = 16, otherError = 17
	};
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.0.1/Pilot/Friends/PilotDisk.mesa
//
class PilotDisk {
public:
	//FileID: TYPE = MACHINE DEPENDENT RECORD [
	//  name(0): SELECT OVERLAID * FROM
	//    volumeRelative => [
	//      fileID(0): File.ID, pad(2): ARRAY [0..3) OF WORD � ALL[0]],
	//    unique => [id(0): System.UniversalID]
	//    ENDCASE];
	union FileID {
		struct {
			CARD32 fileID;
			CARD16 pad[3];
		} __attribute__((packed));
		System::UniversalID id;
	};
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.0.1/Pilot/Friends/Boot.mesa
//
class Boot {
public:
	//mdsiGerm: BootFile.MDSIndex = 0;
	//-- the MDS of the Germ, as defned in the PrincOps.  Known to the initial microcode.
	static const int mdsiGerm = 0;

	//pageGerm: Environment.PageNumber = 1;
	//--  Page where Germ's image starts (within Germ's MDS).  Known to the initial microcode.
	static const int pageGerm = 1;

	//countGermVM: Environment.PageCount = 96 - pageGerm;
	//-- Amount of virtual memory reserved for Germ and its buffers.
	static const int countGermVM = 96 - pageGerm;

	//currentRequestBasicVersion: CARDINAL = 3456B;
	static const CARD16 currentRequestBasicVersion     = 03456;

	//currentRequestExtensionVersion: CARDINAL = 7654B;
	static const CARD16 currentRequestExtensionVersion = 07654;

	//Session: TYPE = {continuingAfterOutLoad, newSession};
	static const CARD16 S_continuingAfterOutLoad = 0;
	static const CARD16 S_newSession             = 1;

	//Action: TYPE = MACHINE DEPENDENT RECORD [act(0): CARDINAL];
	//inLoad: Action = [0];
	//-- restore volatile processor state from BootFile-format snapshot.
	//outLoad: Action = [1];
	//-- save volatile processor state in BootFile-format snapshot.
	//bootPhysicalVolume: Action = [2];
	//-- do inLoad using Location specified indirectly in pilot entry of PVBootFiles array of physical volume root page of disk specified by accompanying Location.  Value known by the initial microcode.
	//teledebug: Action = [3];
	//-- speak to Ethernet as Teledebug Server.  Return when commanded to.
	//noOp: Action = [4];  -- simply enter and exit the Germ.
	static const CARD16 A_inLoad             = 0;
	static const CARD16 A_outLoad            = 1;
	static const CARD16 A_bootPhysicalVolume = 2;
	static const CARD16 A_teledebug          = 3;
	static const CARD16 A_noOp               = 4;


	//DiskFileID: TYPE = MACHINE DEPENDENT RECORD [
	//  fID (0): PilotDisk.FileID,
	//  firstPage (5): PilotDisk.FilePageNumber,
	//  da (7): PilotDiskFace.DiskAddress];
#pragma pack(push, 1)
	struct DiskFileID {
		PilotDisk::FileID          fID;
		CARD32                     firstPage;
		PilotDiskFace::DiskAddress da;
	} __attribute__((packed));
#pragma pack(pop)

	//EthernetRequest: TYPE = MACHINE DEPENDENT RECORD [
	//  bfn(0): EthernetBootFileNumber, address(3): System.NetworkAddress];
	//EthernetBootFileNumber: TYPE = RECORD [HostNumbers.HostNumber];
	//-- Ethernet boot file numbers are allocated from the same name space as
	//-- HostNumbers. Of course, these numbers do not represent hosts.
	struct EthernetRequest {
		System::HostNumber     bfn;
		System::NetworkAddress address;
	} __attribute__((packed));


	//Location: TYPE = MACHINE DEPENDENT RECORD [  -- format known by the initial microcode.
	//  -- Description of boot file location:
	//  deviceType (0): Device.Type,  -- e.g. sa4000, ethernet
	//  deviceOrdinal (1): CARDINAL,  -- position of device within all those of same type
	//  vp (2): SELECT OVERLAID * FROM
	//    disk => [diskFileID (2): DiskFileID],
	//    ethernetOne => [bootFileNumber (2): CARDINAL, net (3), host (4): CARDINAL � 0],
	//    ethernet => [ethernetRequest (2): EthernetRequest],
	//    any => [a (2), b (3), c (4), d (5), e (6), f (7), g (10B), h (11B): UNSPECIFIED],
	//    ENDCASE];
	struct Location {
		CARD16 deviceType;    // Device.Type
		CARD16 deviceOrdinal;
		union {
			DiskFileID      diskFileID;
			EthernetRequest ethernetRequest;
			struct {
				CARD16 a, b, c, d, e, f, g, h;
			};
		};
	};


	//Request: TYPE = MACHINE DEPENDENT RECORD [
	//  -- Basic portion of Request: (format known by the initial microcode)
	//  -- IF YOU CHANGE THE FORMAT OF THIS PORTION, YOU MUST INCREMENT currentRequestBasicVersion AND GENERATE NEW INITIAL MICROCODE!
	//  requestBasicVersion (0B): CARDINAL � currentRequestBasicVersion,
	//  action (1B): Action,
	//  location (2B): Location,
	//  switches (16B): System.Switches � System.defaultSwitches,  -- When calling OutLoad, these are the default switches to be used for system when InLoaded later.  When calling InLoad, these are switches to be used for system being InLoaded; defaultSwitches means use the ones built into the boot file.  When returning from InLoad, these are the switches passed from caller of InLoad or, if he passed defaultSwitches, the switches built into the boot file.
	//
	//  -- Extension portion of Request: (not used by the initial microcode)
	//  requestExtensionVersion (15B): CARDINAL � currentRequestExtensionVersion,
	//
	//   -- Extensions for InLoad - "results":
	//  pStartListHeader  (36B): StartList.Base,  -- only valid after inload of virgin boot file.
	//
	//  -- Extensions for OutLoad - "arguments":
	//  inLoadMode (40B): BootFile.InLoadMode,  -- real page numbers significant?
	//  -- As a side-effect of the cross-mds call mechanism, the entry point of the system being outLoaded is stored in pInitialLink� in the caller's mds and the mds of the system being outLoaded is passed to the Germ.
	//
	//  -- Extensions for OutLoad - "results":
	//  session (41B): Session ];  -- just finished OutLoad, or InLoaded later?
	struct Request {
		CARD16           requestBasicVersion;
		CARD16           action;
		Location         location;
		CARD16           requestExtensionVersion;
		System::Switches switches;
		CARD32           pStartListHeader;
		CARD16           inLoadMode;
		CARD16           session;
	};


	//-- Types of boot files pointed to from root pages of physical and logical
	//-- volumes, and delivered by boot servers:
	//-- The following cannot be changed without invalidating all Pilot volumes.
	//-- Known to the initial microcode.
	//BootFileType: TYPE = MACHINE DEPENDENT{
	//  hardMicrocode (0), softMicrocode (1), germ (2), pilot (3), debugger (4),
	//  debuggee (5), initialMicrocode (6)};
	static const CARD16 BFT_hardMicrocode    = 0;
	static const CARD16 BFT_softMicrocode    = 1;
	static const CARD16 BFT_germ             = 2;
	static const CARD16 BFT_pilot            = 3;
	static const CARD16 BFT_debugger         = 4;
	static const CARD16 BFT_debuggee         = 5;
	static const CARD16 BFT_initialMicrocode = 6;

	//PVBootFiles: TYPE = ARRAY BootFileType [hardMicrocode..pilot] OF DiskFileID;
	struct PVBootFiles {
		DiskFileID bootFile[4];  // 0..3
	} __attribute__((packed));

	//LVBootFiles: TYPE = ARRAY BootFileType [hardMicrocode..debuggee] OF DiskFileID;
	struct LVBootFiles {
		DiskFileID bootFile[6];  // 0..5
	} __attribute__((packed));

};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// APilot/15.0.1/Pilot/Private/LogicalVolumeFormat.mesa
//
class LogicalVolumeFormat {
public:
	//maxLogicalVolumeLabelLength: CARDINAL = 40;  -- DO NOT EVER CHANGE THIS VALUE!
	static const CARD16 maxLogicalVolumeLabelLength = 40;

	//lvRootSeal: CARDINAL = CARDINAL[131313B];  -- DO NOT EVER CHANGE THIS VALUE!
	static const CARD16 lvRootSeal = 0131313;

	//currentVersion: CARDINAL = 9;
	static const CARD16 currentVersion = 9;

	//LockState: TYPE = MACHINE DEPENDENT {unlocked(0), write(1), read(2), spare(3)};
	static const CARD16 LC_unlocked = 0;
	static const CARD16 LC_write    = 1;
	static const CARD16 LC_read     = 2;
	static const CARD16 LC_spare    = 3;

	//VolumeLock: TYPE = MACHINE DEPENDENT RECORD [
	//  owner (0): Volume.ID � Volume.nullID,
	//  lock (5:0..15): SELECT stateTag (5:0..1): LockState FROM
	//    unlocked => [null (5:2..15): [0..16384) � 0],
	//    write => [null (5:2..15): [0..16384) � 0],
	//    read => [count (5:2..5): [0..16) � 0, null (5:6..15): [0..1024) � 0],
	//  ENDCASE];
	struct VolumeLock {
		System::UniversalID owner;
		union {
			CARD16 u5;
			struct {
				CARD16 null     : 10;
				CARD16 count    :  4;
				CARD16 stateTag :  2;
			};
		};
	} __attribute__((packed));


	//Descriptor: TYPE = MACHINE DEPENDENT RECORD [
	//  seal(0):          CARDINAL  -- absolutely must be 1st field
	//  version(1):       CARDINAL  -- absolutely must be 2nd field
	//  vID(2):           Volume.ID,
	//  labelLength(7):   CARDINAL [0..maxLogicalVolumeLabelLength] � 0,
	//  label(10B):       LabelArray � nullName,
	//  type(34B):        Volume.Type,
	//  volumeSize(35B):  Volume.PageCount,
	//  bootingInfo(37B): Boot.LVBootFiles � nullBoot,
	//  pad(125B):	      CARDINAL � 0,
	//  pad1(126B:0..14):  [0..77777B] � 0,
	//  changing(126B:15..15): BOOLEAN � TRUE,  -- see documentation for semantics.
	//  freePageCount(127B):       Volume.PageCount � 0,
	//  vamStart(131B):            Volume.PageNumber � 1,
	//  primaryETableStartPage(133B): Volume.PageNumber � 0,
	//  copyETableStartPage(135B): Volume.PageNumber � 0,
	//  lowerBound(137B):          Volume.PageNumber
	//  volumeRootDirectory(141B): File.ID � File.nullID,
	//  rootFileID(143B):          ARRAY PilotFileTypes.PilotRootFileType OF File.ID
	//  lastIDAllocated(165B):     LONG CARDINAL
	//  scavengerLogVolume(167B):  Volume.ID
	//  lastTimeOpendForWrite(174B):         System.GreenwichMeanTime �
	//  statusPrimaryETableFile(176B:0..7):  [0..400B) � 0,
	//  statusCopyETableFile(176B:8..15):    [0..400B) � 0,
	//  numberOfFile(177B):        LONG CARDINAL � 0,
	//  lock(201B): VolumeLock,
	//  numberOfBadPagesLastTime(207B): Volume.PageCount � 0,
	//  fill(211B):                ARRAY [0..377B - 211B) OF WORD � ALL[0],
	//  checksum(377B):            CARDINAL � 0];  -- MUST be the last field

#pragma pack(push, 1)
	struct Descriptor {
		CARD16              seal;
		CARD16              version;
		System::UniversalID vID;
		CARD16              labelLength;
		CARD8               label[maxLogicalVolumeLabelLength];
		CARD16              type;
		CARD32              volumeSize;
		Boot::LVBootFiles   bootingInfo;

		CARD16              pad;
		union {
			CARD16 u126;
			struct {
				CARD16      changing :  1;
				CARD16      pad1     : 15;
			};
		};
		CARD32              freePageCount;
		CARD32              vamStart;
		CARD32              primaryETableStartPage;
		CARD32              copyETableStartPage;
		CARD32              lowerBound;
		CARD32              volumeRootDirectory;
		CARD32              rootFileID[9]; // PilotRootFileType: TYPE = PilotFileType [6..14];
		CARD32              lastIDAllocated;
		System::UniversalID scavengerLogVolume;
		CARD32              lastTimeOpendForWrite;
		union {
			CARD16 u176;
			struct {
				CARD16      statusCopyETableFile    : 8;
				CARD16      statusPrimaryETableFile : 8;
			};
		};
		CARD32              numberOfFile;
		VolumeLock          lock;
		CARD32              numberOfBadPagesLastTime;
		CARD16              fill[118];
		CARD16              checksum;
	} __attribute__((packed));
#pragma pack(pop)
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// APilot/15.0.1/Pilot/Private/PhysicalVolumeFormat.mesa
//
class PhysicalVolumeFormat {
public:
	//  seal, Seal: CARDINAL = 121212B;
	static const CARD16 seal = 0121212;

	//  currentVersion: CARDINAL = 7;
	static const CARD16 currentVersion = 7;

	//  maxSubVols: CARDINAL [10..10] = 10;
	static const CARD16 maxSubVols = 10;

	//  -- SubVolumeDesc as stored on disk in physical volume root page and in
	//  -- marker pages.
	//  SubVolumeDesc: TYPE = MACHINE DEPENDENT RECORD [
	//    lvID(0): System.VolumeID,
	//    lvSize(5): PageCount,
	//    lvPage(7): PageNumber,
	//    pvPage(11B): PageNumber,
	//    nPages(13B): PageCount];
#pragma pack(push, 1)
	struct SubVolumeDesc {
		System::UniversalID lvID;
		CARD32 lvSize;
		CARD32 lvPage;
		CARD32 pvPage;
		CARD32 nPages;
	} __attribute__((packed));
#pragma pack(pop)

	//  physicalVolumeLabelLength: CARDINAL = 40;
	//
	//  LabelArray: TYPE = PACKED ARRAY [0..physicalVolumeLabelLength) OF CHARACTER;
	static const CARD16 physicalVolumeLabelLength = 40;

	//  Descriptor: TYPE = MACHINE DEPENDENT RECORD [
	//    -- the following information is relatively static
	//    seal(0): CARDINAL � Seal,  -- must be 1st field
	//    version(1): CARDINAL � currentVersion,  -- must be 2nd field
	//    labelLength(2): CARDINAL [0..physicalVolumeLabelLength] � 0,
	//    pvID(3): System.PhysicalVolumeID,
	//    bootingInfo(10B): Boot.PVBootFiles � nullPVBootFiles,
	//    -- must be at this offset as the microcode knows where to find it.
	//    label(54B): LabelArray � nullLabel | NULL,
	//    subVolumeCount(100B): CARDINAL [0..maxSubVols],
	//    subVolumeMarkerID(101B): MarkerID � [System.nullID],
	//    badPageCount(106B): PageCount � 0,
	//    maxBadPages(110B): PageCount � maxNumberBadPages,
	//    dataLostPageCount(112B): PageCount � 0,
	//    maxDataLostPages(114B): PageCount � maxNumberDataLostPages,
	//    onLineCount(116B): CARDINAL � 0,  -- TEMPORARILY unused.
	//    subVolumes(117B): ARRAY [0..maxSubVols) OF SubVolumeDesc,
	//    fill1(321B): ARRAY [0..374B - 321B) OF WORD � ALL[0],  -- fill to whole page
	//    localTimeParametersValid(374B:0..15): BOOLEAN � FALSE,
	//    localTimeParameters(375B): System.LocalTimeParameters � LOOPHOLE[LONG[0]],
	//    checksum(377B): CARDINAL � 0,  -- MUST be the last field of this page
	//
	//    -- followed, on immediately following pages, by a BadPageList with maxBadPages entries
	//
	//    badPageList(400B): BadPageListArray];
#pragma pack(push, 1)
	struct Descriptor {
		CARD16              seal;
		CARD16              version;
		CARD16              labelLength;
		System::UniversalID pvID;
		Boot::PVBootFiles   bootingInfo;
		CARD8               label[physicalVolumeLabelLength];
		CARD16              subVolumeCount;
		System::UniversalID subVolumeMarkerID;
		CARD32              badPageCount;
		CARD32              maxBadPages;
		CARD32              dataLostPageCount;
		CARD32              maxDataLostPages;
		CARD16              onLineCount;
		SubVolumeDesc       subVolumes[maxSubVols];
		CARD16              fill[43];
		CARD16              localTimeParametersValid;
		CARD32              localTimeParameters;
		CARD16              checksum;
	} __attribute__((packed));
#pragma pack(pop)
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.3/Pilot/Private/GermOpsImpl.mesa
//
class GermOpsImpl {
public:
	//pageGerm: PageNumber = Boot.pageGerm + Boot.mdsiGerm*Environment.maxPagesInMDS;
	//pageEndGermVM: PageNumber = pageGerm + Boot.countGermVM;
	//-- end VM page usable by Germ.
	static const CARD32 pageGerm = Boot::pageGerm + Boot::mdsiGerm * 1;
	static const CARD32 pageEndGermVM = pageGerm + Boot::countGermVM;
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.3/Faces/Private/GuamInputOutput.mesa
//
class GuamInputOutput {
public:
	// aCALLAGENT: ESCAlpha.alpha = 211B;
	// aMAPDISPLAY: ESCAlpha.alpha = 212B;
	// aSTOPEMULATOR: ESCAlpha.alpha = 213B;
	static const CARD8 aCALLAGENT    = 0211;
	static const CARD8 aMAPDISPLAY   = 0212;
	static const CARD8 aSTOPEMULATOR = 0213;

	// AgentDeviceIndex: TYPE = MACHINE DEPENDENT {
	//   null(0), disk(1), floppy(2), network(3), parallel(4), keyboard(5), beep(6),
	//   mouse(7), processor(8), stream(9), serial(10), tty(11), display(12),
	//   reserved3(13), reserved2(14), reserved1(15)};
	enum AgentDeviceIndex {
		null = 0, disk = 1, floppy = 2, network = 3, parallel = 4,
		keyboard = 5, beep = 6, mouse = 7, processor = 8, stream = 9,
		serial = 10, tty = 11, display = 12, reserved3 = 13, reserved2 = 14, reserved1 = 15
	};
	static const int AgentDeviceIndex_SIZE = reserved1 + 1;

	// IORegionType: PRIVATE TYPE = MACHINE DEPENDENT RECORD [
	//   fcbPtrs(0): ARRAY AgentDeviceIndex OF LONG POINTER];
	struct IORegionType {
		LONG_POINTER fcbptrs[reserved1 + 1];
	};
	// ioRegionPtr: READONLY LONG POINTER TO IORegionType;
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.3/Faces/Public/BiParallelPortFace.mesa
//
class BiParallelPortFace {
public:
	//Operation: TYPE = RECORD [
	//  dataBufferPtr: LONG POINTER,	-- ptr to buffer that contains data to be transferred out
	//  					--	or will contain data transferred in
	//  bufferByteLength: CARDINAL		-- buffer size in bytes
	//  ];
	struct Operation {
		CARD32 dataBufferPtr;
		CARD16 bufferByteLength;
	} __attribute__((packed));

	//TransferMode: TYPE = { put, get };
	static const CARD16 TM_put = 0;
	static const CARD16 TM_get = 1;

	//ControlOrData: TYPE = { control, data };
	static const CARD16 CD_control;
	static const CARD16 CD_data;
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.3/Faces/Friends/DisplayFace.mesa
//
class DisplayFace {
public:
	//Cursor: TYPE = ARRAY [0..16) OF WORD;
	struct Cursor {
		CARD16 word[16];
	};
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.3/Faces/Friends/FloppyDiskFace.mesa
//
class FloppyDiskFace {
public:
	//Density: TYPE = MACHINE DEPENDENT {single, double}; --Selection of either FM (single density) or MFM (double density) recording. This is an available selection when formatting a new diskette. When accessng a previously recorded diskette, the client must provide the correct setting. (Note that track00 on IBM format 8" diskettes and all tracks of Troy format diskettes will be single density.)

	//Function: TYPE = MACHINE DEPENDENT {
	//  nop,                 --Does not transfer any data but does create an asychronous operation and returns a valid endng status.
	//  readSector,          --Reads one or more sectors from the diskette starting at the specified disk address.
	//  writeSector,         --Writes one or more sectors with a data addres mark starting at the specified disk address.
	//  writeDeletedSector,  --Writes one or more sectors with a deleted data addres mark starting at the specified disk address.
	//  readID,              --Reads first encountered record ID from the specified disk address (the value of sector in the disk address is ignored).
	//  formatTrack};        --format one or more tracks starting at the specified disk cylinder & head
	static const CARD16 F_nop                = 0;
	static const CARD16 F_readSector         = 1;
	static const CARD16 F_writeSector        = 2;
	static const CARD16 F_writeDeletedSector = 3;
	static const CARD16 F_readID             = 4;
	static const CARD16 F_formatTrack        = 5;

	//DiskAddress: TYPE = MACHINE DEPENDENT RECORD [
	//  cylinder(0):       CARDINAL,           --must be IN [0..numberOfCylinders)
	//  head(1:0..7):      Environment.Byte,   --must be IN [0..numberOfHeads)
	//  sector(1:8..15):   Environment.Byte];  --must be IN [1..maxSectorsPerTrack]
	struct DiskAddress {
		CARD16 cylinder;
		union {
			struct {
				CARD16 sector : 8;
				CARD16 head   : 8;
			};
			CARD16 u1;
		};
	};

	//Operation: TYPE = MACHINE DEPENDENT RECORD [
	//  device(0):                    DeviceHandle,
	//  function(1):                  Function,
	//  address(2):                   DiskAddress,  -- ignored if nop.  Sector is ignored in formatTrack operations.
	//  dataPtr(4):                   LONG POINTER, -- ignored if nop
	//  incrementDataPointer(6:0..0): BOOLEAN,      -- ignored if nop, formatTrack
	//  tries(6:1..15):               Tries,        -- Indication of how hard the Face should retry the operation in the presence of errors.  Increasing values of tries indicates that non-decreasing effort should be expended in retrying the failing operation.  Retries include device specific recovery (recalibrates, ...)
	//  count(7):                     CARDINAL];    -- ignored if nop, readID.  Number of tracks to format if formatTrack (must not exceed MaxTracksPerFormatOperation); number of sectors to transfer otherwise.
	struct Operation {
		CARD16      device;
		CARD16      function;
		DiskAddress address;
		CARD32      dataPtr;
		union {
			struct {
				CARD16 tries                : 15;
				CARD16 incrementDataPointer :  1;
			};
			CARD16 u6;
		};
		CARD16      count;
	};

	//Status: TYPE = MACHINE DEPENDENT {
	//  inProgress, -- operation is not yet complete
	//  goodCompletion, -- operation has completed normally
	//  diskChange, -- drive has gone not ready since last successful operation (use DiskChangeClear to reset, then resubmit operation if desired)
	//  notReady, -- drive is not ready
	//  cylinderError, -- can't locate specified cylinder
	//  deletedData, -- The sector contained a deleted data address mark.
	//  recordNotFound, -- can't find record for specified disk address
	//  headerError, -- bad checksum in header
	//  dataError, -- bad checksum in data
	//  dataLost, -- sector contained more data than expected (from context)
	//  writeFault, -- disk is write-protected (hardware or from context)
	//  memoryError, -- dataPtr does not point to valid memory (not resident, too small, write-protected, etc.)
	//  invalidOperation, -- operation does not make sense
	//  aborted, -- Reset has been called
	//  otherError}; -- unexpected software or hardware problem
	static const CARD16 S_inProgress       =  0;
	static const CARD16 S_goodCompletion   =  1;
	static const CARD16 S_diskChange       =  2;
	static const CARD16 S_notReady         =  3;
	static const CARD16 S_cylinderError    =  4;
	static const CARD16 S_deletedData      =  5;
	static const CARD16 S_recordNotFound   =  6;
	static const CARD16 S_headerError      =  7;
	static const CARD16 S_dataError        =  8;
	static const CARD16 S_dataLost         =  9;
	static const CARD16 S_writeFault       = 10;
	static const CARD16 S_memoryError      = 11;
	static const CARD16 S_invalidOperation = 12;
	static const CARD16 S_aborted          = 13;
	static const CARD16 S_otherError       = 14;
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.3/RS232C/Public/RS232CEnvironment.mesa
//
class RS232CEnvironment {
public:
	//CharLength: TYPE = [5..8];

	//Correspondent: TYPE = RECORD [[0..255]];
	typedef CARD8 Correspondent;

	//FlowControl: TYPE = MACHINE DEPENDENT RECORD [
	//  type(0): {none, xOnXOff},
	//  xOn(1), xOff(2): UNSPECIFIED];
	struct FlowControl {
		CARD16 type;
		CARD16 xOn;
		CARD16 xOff;
	};
	static const CARD16 FCT_none    = 0;
	static const CARD16 FCT_xOnXOff = 1;

	//LineSpeed: TYPE = {
	//  bps50, bps75, bps110, bps134p5, bps150, bps300, bps600, bps1200, bps2400,
	//  bps3600, bps4800, bps7200, bps9600, bps19200, bps28800, bps38400, bps48000,
	//  bps56000, bps57600};
	static const CARD16 LS_bps50    =  0;
	static const CARD16 LS_bps75    =  1;
	static const CARD16 LS_bps110   =  2;
	static const CARD16 LS_bps134p5 =  3;
    static const CARD16 LS_bps150   =  4;
    static const CARD16 LS_bps300   =  5;
    static const CARD16 LS_bps600   =  6;
    static const CARD16 LS_bps1200  =  7;
    static const CARD16 LS_bps2400  =  8;
    static const CARD16 LS_bps3600  =  9;
    static const CARD16 LS_bps4800  = 10;
    static const CARD16 LS_bps7200  = 11;
    static const CARD16 LS_bps9600  = 12;
    static const CARD16 LS_bps19200 = 13;
    static const CARD16 LS_bps28800 = 14;
    static const CARD16 LS_bps38400 = 15;
    static const CARD16 LS_bps48000 = 16;
    static const CARD16 LS_bps56000 = 17;
    static const CARD16 LS_bps57600 = 18;

	//LineType: TYPE = {
	//  bitSynchronous, byteSynchronous, asynchronous, autoRecognition};
    static const CARD16 LT_bitSynchronous  = 0;
    static const CARD16 LT_byteSynchronous = 1;
    static const CARD16 LT_asynchronous    = 2;
    static const CARD16 LT_autoRecognition = 3;

	//Parity: TYPE = {none, odd, even, one, zero};
    static const CARD16 P_none = 0;
    static const CARD16 P_odd  = 1;
    static const CARD16 P_even = 2;
    static const CARD16 P_one  = 3;
    static const CARD16 P_zero = 4;

	//StopBits: TYPE = [1..2];

	//SyncChar: TYPE = Environment.Byte;

	//SyncCount
	//SyncCount: TYPE = [0..7];
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.3/Faces/Private/NewRS232CFace.mesa
//
class NewRS232CFace {
public:
	//ParameterRecord: TYPE = MACHINE DEPENDENT RECORD [
	//  charLength(0:8..10): RS232CEnvironment.CharLength,
	//  clientType(2:1..7): ClientType,
	//  correspondent(2:8..15): RS232CEnvironment.Correspondent,
	//  echo(2:0..0): BOOLEAN,
	//  flowControl(4:0..47): RS232CEnvironment.FlowControl,
	//  frameTimeout(3): CARDINAL,
	//  lineSpeed(0:3..7): RS232CEnvironment.LineSpeed,
	//  lineType(0:0..2): RS232CEnvironment.LineType,
	//  parity(0:11..13): RS232CEnvironment.Parity,
	//  stopBits(0:14..15): RS232CEnvironment.StopBits,
	//  syncChar(1:8..15): RS232CEnvironment.SyncChar,
	//  syncCount(1:0..7): RS232CEnvironment.SyncCount];

	//ParameterRecord: TYPE = MACHINE DEPENDENT RECORD [
	//  lineType(0:0..2): RS232CEnvironment.LineType,
	//  lineSpeed(0:3..7): RS232CEnvironment.LineSpeed,
	//  charLength(0:8..10): RS232CEnvironment.CharLength,
	//  parity(0:11..13): RS232CEnvironment.Parity,
	//  stopBits(0:14..15): RS232CEnvironment.StopBits,
	//  syncCount(1:0..7): RS232CEnvironment.SyncCount];
	//  syncChar(1:8..15): RS232CEnvironment.SyncChar,
	//  echo(2:0..0): BOOLEAN,
	//  clientType(2:1..7): ClientType,
	//  correspondent(2:8..15): RS232CEnvironment.Correspondent,
	//  frameTimeout(3): CARDINAL,
	//  flowControl(4:0..47): RS232CEnvironment.FlowControl,
	struct ParameterRecord {
		union {
			struct {
				CARD16 stopBits   : 2;
				CARD16 parity     : 3;
				CARD16 charLength : 3;
				CARD16 lineSpeed  : 5;
				CARD16 lineType   : 3;
			};
			CARD16 u0;
		};
		union {
			struct {
				CARD16 syncChar  : 8;
				CARD16 syncCount : 8;
			};
			CARD16 u1;
		};
		union {
			struct {
				CARD16 correspondent : 8;
				CARD16 clientType     : 7;
				CARD16 echo          : 1;
			};
			CARD16 u2;
		};
		CARD16                         frameTimeout;
		RS232CEnvironment::FlowControl flowControl;
	};

	//Operation: TYPE = MACHINE DEPENDENT RECORD [
	//  dataBuffer(0:0..31): LONG POINTER,
	//  bufferByteLength(2:0..15): CARDINAL];
	struct Operation {
		CARD32 dataBuffer;
		CARD16 bufferByteLength;
	} __attribute__((packed));

	//ClientType: TYPE = MACHINE DEPENDENT {ascii, ebcdic, unknown};
	static const CARD16 CT_ascii    = 0;
	static const CARD16 CT_ebicidic = 1;
	static const CARD16 CT_unknown  = 2;

	//ControlRecord: TYPE = MACHINE DEPENDENT RECORD [
	//  dataTerminalReady(0:0..0): BOOLEAN,
	//  requestToSend(0:1..1): BOOLEAN,
	//  unused(0:2..15): [0..37777B] _ 0];
	union ControlRecord {
		struct {
			CARD16 unused            : 14;
			CARD16 requestToSend     :  1;
			CARD16 dataTerminalReady :  1;
		};
		CARD16 u;
	};

	//DeviceStatus: TYPE = MACHINE DEPENDENT RECORD [
	//  -- latched status bits
	//  breakDetected(0:0..0): BOOLEAN,
	//  dataLost(0:1..1): BOOLEAN,
	//  ringHeard(0:2..2): BOOLEAN,
	//  -- unlatched status bits
	//  carrierDetect(0:3..3): BOOLEAN,
	//  clearToSend(0:4..4): BOOLEAN,
	//  dataSetReady(0:5..5): BOOLEAN,
	//  ringIndicator(0:6..6): BOOLEAN,
	//  unused(0:7..15): [0..777B] _ 0];
	union DeviceStatus {
		struct {
			CARD16 unused        : 9;
			CARD16 ringIndicator : 1;
			CARD16 dataSetReady  : 1;
			CARD16 clearToSend   : 1;
			CARD16 carrierDetect : 1;
			CARD16 ringHeard     : 1;
			CARD16 dataLost      : 1;
			CARD16 breakDetected : 1;
		};
		CARD16 u;
	};

	//ParameterStatus: TYPE = MACHINE DEPENDENT {inProgress, completed, unimplemented, rejected};
	static const CARD16 PS_inProgress   = 0;
	static const CARD16 PS_completed    = 1;
	static const CARD16 PS_unimplmented = 2;
	static const CARD16 PS_rejected     = 3;
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.3/Faces/Private/BeepIOFaceGuam.mesa
//
class BeepIOFaceGuam {
public:
	//  BeepFCBType: TYPE = MACHINE DEPENDENT RECORD [
	//    frequency(0): CARDINAL]; -- 0 to disable beep
	struct BeepFCBType {
		CARD16 frequency;
	};
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.3/Faces/Private/CoProcessorIOFaceGuam.mesa
//
class CoProcessorIOFaceGuam {
public:
	//CoProcessorFCBType: TYPE = MACHINE DEPENDENT RECORD [
	//  iocbHead(0): CoProcessorIOCBPtr,
	//  iocbNext(2): CoProcessorIOCBPtr,
	//  headCommand(4): CommandType,
	//  filler5(5): WORD,
	//  headResult(6): ResultType,
	//  filler7(7): WORD,
	//  interruptSelector(8): GuamInputOutput.InterruptSelectorType,
	//  stopAgent(9): BOOLEAN,
	//  agentStopped(10): BOOLEAN,
	//  streamWordSize(11): CARDINAL];
	struct CoProcessorFCBType {
		CARD32 iocbHead;
		CARD32 iocbNext;
		CARD16 headCommand;
		CARD16 filler5;
		CARD16 headResult;
		CARD16 filler7;
		CARD16 interruptSelector;
		CARD16 stopAgent;
		CARD16 agentStopped;
		CARD16 streamWordSize;
	};

	//TransferRec: TYPE = MACHINE DEPENDENT RECORD [
	//  subSequence(0): INTEGER � 0,
	//  filler1(1): WORD � 0,
	//  filler2(2:0..12): [0..8191] � 0,
	//  endStream(2:13..13): BOOLEAN � FALSE,
	//  endRecord(2:14..14): BOOLEAN � FALSE,
	//  endSST(2:15..15): BOOLEAN � FALSE,
	//  filler3(3): WORD � 0,
	//  bytesWritten(4): CARDINAL � 0,
	//  filler5(5): WORD � 0,
	//  bytesRead(6): CARDINAL � 0,
	//  filler7(7): WORD � 0,
	//  hTask(8): UNSPECIFIED � 0,
	//  interruptMesa(9): BOOLEAN � FALSE,
	//  buffer(10): LONG POINTER � NIL,
	//  bufferSize(12): CARDINAL � 0,
	//  filler13(13): WORD � 0,
	//  writeLockedByMesa(14): BOOLEAN � FALSE,
	//  filler15(15): WORD � 0];
	struct TransferRec {
		INT16 subSequence;
		CARD16 filler1;
		union {
			struct {
				CARD16 endSST    :  1;
				CARD16 endRecord :  1;
				CARD16 endStream :  1;
				CARD16 filler2   : 13;
			};
			CARD16 u2;
		};
		CARD16 filler3;
		CARD16 bytesWritten;
		CARD16 filler5;
		CARD16 bytesRead;
		CARD16 filler7;
		CARD16 hTask;
		CARD16 interruptMesa;
		CARD32 buffer;
		CARD16 bufferSize;
		CARD16 filler13;
		CARD16 writeLockedByMesa;
		CARD16 filler15;
	};

	//CoProcessorIOCBPtr: TYPE = LONG POINTER TO CoProcessorIOCBType;
	//CoProcessorIOCBType: TYPE = MACHINE DEPENDENT RECORD [
	//  serverID(0): LONG CARDINAL,
	//  mesaConnectionState(2): ConnectionStateType � idle,
	//  filler3(3): WORD � 0,
	//  pcConnectionState(4): ConnectionStateType � idle,
	//  filler5(5): WORD � 0,
	//  mesaPut(6): TransferRec � [],
	//  mesaGet(22): TransferRec � [],
	//  nextIOCB(38): CoProcessorIOCBPtr � NIL,
	//  mesaIsServer(40): BOOLEAN � FALSE,
	//  filler41(41): WORD � 0];
	struct CoProcessorIOCBType {
		CARD32      serverID;
		CARD16      mesaConnectionState;
		CARD16      filler3;
		CARD16      pcConnectionState;
		CARD16      filler5;
		TransferRec mesaPut;
		TransferRec mesaGet;
		CARD32      nextIOCB;
		CARD16      mesaIsServer;
		CARD16      filler41;
	};

	//CommandType: TYPE = MACHINE DEPENDENT
	//  {idle(0), accept(1), connect(2), delete(3), read(4), write(5)};
	static const CARD16 C_idle    = 0;
	static const CARD16 C_accept  = 1;
	static const CARD16 C_connect = 2;
	static const CARD16 C_delete  = 3;
	static const CARD16 C_read    = 4;
	static const CARD16 C_write   = 5;

	//ConnectionStateType: TYPE = MACHINE DEPENDENT
	//  {idle(0), accepting(1), connected(2), deleted(3)};
	static const CARD16 S_idle      = 0;
	static const CARD16 S_accepting = 1;
	static const CARD16 S_connected = 2;
	static const CARD16 S_deleted   = 3;

	//ResultType: TYPE = MACHINE DEPENDENT
	//  {completed(0), inProgress(1), error(2)};
	static const CARD16 R_completed  = 0;
	static const CARD16 R_inProgress = 1;
	static const CARD16 R_error      = 2;
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.3/CoProcessorStream/Public/CoProcessorServerIDs.mesa
//
class CoProcessorServerIDs {
public:
	// msdosFileService, fileAccess: ServerID = 1;  -- service on DOS end.
	static const CARD32 msdosFileService                       = 1;
	static const CARD32 fileAccess                             = 1;
	// unixFileService, windowToUNIXFileService: ServerID = 2;  -- service on UNIX end.
	// triggerApplicationService, triggerApp: ServerID = 5;  -- service on nonGV end?

	// TcpStreamGuamImpl.tcpService = CoProcessorServerIDsExtra.tcpService
	static const CARD32 tcpService                             = 21;

	// mswindowsCopyAndPasteService, copyPaste: ServerID = 101;  -- service on MS Windows end.
	static const CARD32 mswindowsCopyAndPasteService           = 101;
	static const CARD32 copyPaste                              = 101;
	// interpressToPostScriptPrintingService, psPrntSrv: ServerID = 102;  -- service on nonGV end.
	static const CARD32 interpressToPostScriptPrintingService  = 102;
	static const CARD32 psPrntSrv                              = 102;
	// documentConversionService, docCnvSrv: ServerID = 103;  -- service on nonGV end.
	static const CARD32 documentConversionService              = 103;
	static const CARD32 docCnvSrv                              = 103;
	// dragAndDropToGVService, dragAndDropBWSID: ServerID = 104;  -- service on GlobalView end.
	// helpService, help: ServerID = 105;  -- service on nonGV end.
	static const CARD32 helpService                            = 105;
	static const CARD32 help                                   = 105;
	// workspaceWindowControlGVService, wkspWinCtrlMesaServerID: ServerID = 106;  -- service on GlobalView end.
	static const CARD32 workspaceWindowControlGVService        = 106;
	static const CARD32 wkspWinCtrlMesaServerID                = 106;
	// dragAndDropToMSWindowsService, dragAndDropAgentID: ServerID = 107;  -- service on MSWindows end.
	static const CARD32 dragAndDropToMSWindowsService          = 107;
	static const CARD32 dragAndDropAgentID                     = 107;
	// workspaceWindowControlMSWindowsService, wkspWinCtrlPCServerID: ServerID = 108;  -- service on MSWindows end.
	static const CARD32 workspaceWindowControlMSWindowsService = 108;
	static const CARD32 wkspWinCtrlPCServerID                  = 108;
	// windowInstallerToTTYService, windowInstallerAgentID: ServerID = 109;  -- service on MSWindows end to communicate with Stream-To-TTY Interface of the NSInstaller.
	static const CARD32 windowInstallerToTTYService            = 109;
	static const CARD32 windowInstallerAgentID                 = 109;
	// bootStreamAgentService, bootAgentID:  ServerID = 110; --service on MSWindows end. Communication with bootloader.
	static const CARD32 bootStreamAgentService                 = 110;
	static const CARD32 bootAgentID                            = 110;
	// installerStreamAgentService, installerAgentID: ServerID = 111; --service on MSWindows end.
	static const CARD32 installerStreamAgentService            = 111;
	static const CARD32 installerAgentID                       = 111;
	// scanService: ServerID = 715;  -- service on nonGV end.
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.3/Faces/Private/DiskIOFaceGuam.mesa
//
class DiskIOFaceGuam {
public:
	//  DiskDCBType: TYPE = MACHINE DEPENDENT RECORD [
	//    deviceType(0): Device.Type,
	//    numberOfCylinders(1): CARDINAL,
	//    numberOfHeads(2): CARDINAL,
	//    sectorsPerTrack(3): CARDINAL,
	//    agentDeviceData(4): ARRAY [0..6) OF WORD];
	struct DiskDCBType {
		CARD16 deviceType; // Device.Type
		CARD16 numberOfCylinders;
		CARD16 numberOfHeads;
		CARD16 sectorsPerTrack;
		CARD16 agentDeviceData[6];
	};

	//  DiskFCBType: TYPE = MACHINE DEPENDENT RECORD [
	//    nextIOCB(0): DiskIOCBPtr,
	//    interruptSelector(2): GuamInputOutput.InterruptSelectorType,
	//    stopAgent(3): BOOLEAN,
	//    agentStopped(4): BOOLEAN,
	//    numberOfDCBs(5): CARDINAL,
	//    dcbs(6): SEQUENCE COMPUTED CARDINAL OF DiskDCBType];
	struct DiskFCBType {
		CARD32 nextIOCB;          // DiskIOCBPtr
		CARD16 interruptSelector; // GuamInputOutput.InterruptSelectorType = WORD
		CARD16 stopAgent;         // BOOLEAN
		CARD16 agentStopped;      // BOOLEAN
		CARD16 numberOfDCBs;      // number of dcbs
		DiskDCBType dcbs[0];      // SEQUENCE COMPUTED CARDINAL OF DiskDCBType
	};


	//  DiskIOCBPtr: TYPE = LONG POINTER TO DiskIOCBType;
	//  DiskIOCBType: TYPE = MACHINE DEPENDENT RECORD [
	//    operation(0): PilotDiskFace.Operation,
	//    deviceIndex(13): CARDINAL,
	//    diskAddress(14): PilotDiskFace.DiskAddress,
	//    dataPtr(16): LONG POINTER,
	//    incrementDataPtr(18): BOOLEAN,
	//    command(19): PilotDiskFace.Command,
	//    pageCount(20): CARDINAL,
	//    status(21): PilotDiskFace.Status � inProgress,
	//    nextIOCB(22): DiskIOCBPtr � NIL,
	//    agentOperationData(24): ARRAY [0..10) OF WORD � ALL[0] ];
	struct DiskIOCBType {
		PilotDiskFace::Operation   operation;              // PilotDiskFace.Operation
		CARD16                     deviceIndex;
		PilotDiskFace::DiskAddress diskAddress;            // PilotDiskFace.DiskAddress
		CARD32                     dataPtr;                // LONG POINTER
		CARD16                     incrementDataPtr;       // BOOLEAN
		CARD16                     command;                // PilotDiskFace.Command
		CARD16                     pageCount;
		CARD16                     status;                 // PilotDiskFace.Status = inProgress
		CARD32                     nextIOCB;               // DiskIOCBPtr = NIL
		CARD16                     agentOperationData[10]; // ALL[0]
	};
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.3/Faces/Private/DisplayIOFaceGuam.mesa
//
class DisplayIOFaceGuam {
public:
	//DisplayCommand: TYPE = MACHINE DEPENDENT {
	//  nop(0), setCLTEntry(1), getCLTEntry(2), setBackground(3), setCursorPattern(4),
	//  updateRectangle(5), copyRectangle(6), patternFillRectangle(7)};
	static const CARD16 C_nop                  = 0;
	static const CARD16 C_setCLTEntry          = 1;
	static const CARD16 C_getCLTEntry          = 2;
	static const CARD16 C_setBackground        = 3;
	static const CARD16 C_setCursorPattern     = 4;
	static const CARD16 C_updateRectangle      = 5;
	static const CARD16 C_copyRectangle        = 6;
	static const CARD16 C_patternFillRectangle = 7;

	//DisplayCoordinate: TYPE = MACHINE DEPENDENT RECORD [
	//  x(0): CARDINAL,
	//  y(1): CARDINAL];
	struct DisplayCoordinate {
		CARD16 x;
		CARD16 y;
	};

	//DisplayRectangle: TYPE = MACHINE DEPENDENT RECORD [
	//  origin(0): DisplayCoordinate,
	//  width(2): CARDINAL,
	//  height(3): CARDINAL];
	struct DisplayRectangle {
		DisplayCoordinate origin;
		CARD16            width;
		CARD16            height;
	};

	//DisplayStatus: TYPE = MACHINE DEPENDENT {
	//  success(0), generalFailure(1), invalidCLTIndex(2), readOnlyCLT(3),
	//  invalidDestRectangle(4), invalidSourceRectangle(5)};
	static const CARD16 S_success                = 0;
	static const CARD16 S_generalFailure         = 1;
	static const CARD16 S_invalidCLTIndex        = 2;
	static const CARD16 S_readOnlyCLT            = 3;
	static const CARD16 S_invalidDestRectangle   = 4;
	static const CARD16 S_invalidSourceRectangle = 5;

	//DisplayType: TYPE = MACHINE DEPENDENT {
	//  monochrome(0), fourBitPlaneColor(1), byteColor(2)};
	static const CARD16 T_monochrome        = 0;
	static const CARD16 T_fourBitPlaneColor = 1;
	static const CARD16 T_byteColor         = 2;

	//-- Fields in LookupTableEntry are arranged from high to low within a 32-bit word.
	//LookupTableEntry: TYPE = MACHINE DEPENDENT RECORD [
	//  reserved(1:0..7): Environment.Byte,
	//  blue(1:8..15): Environment.Byte,
	//  green(0:0..7): Environment.Byte,
	//  red(0:8..15): Environment.Byte];
	struct LookupTableEntry {
		union {
			struct {
				CARD16 red      : 8;
				CARD16 green    : 8;
			};
			CARD16 u0;
		};
		union {
			struct {
				CARD16 blue     : 8;
				CARD16 reserved : 8;
			};
			CARD16 u1;
		};
	};

	//PatternFillMode: TYPE = MACHINE DEPENDENT {
	//  copy(0), and(1), or(2), xor(3)};
	static const CARD16 PFM_copy = 0;
	static const CARD16 PFM_and  = 1;
	static const CARD16 PFM_or   = 2;
	static const CARD16 PFM_xor  = 3;

	//-- Fields in DisplayFCBType have been arranged by function, not by position.
	//DisplayFCBType: TYPE = MACHINE DEPENDENT RECORD [
	//  -- initialization values
	//  displayType(37): DisplayType,
	//  displayWidth(38): CARDINAL,
	//  displayHeight(39): CARDINAL,
	//  displayMemoryAddress(2): LONG UNSPECIFIED,
	//  -- general display command information
	//  command(0): DisplayCommand,
	//  status(1): DisplayStatus,
	//  -- display data change parameters
	//  destRectangle(8): DisplayRectangle,
	//  sourceOrigin(12): DisplayCoordinate,
	//  colorMapping(6): ARRAY [0..1] OF CARDINAL,
	//  pattern(30): ARRAY [0..4) OF WORD,
	//  patternFillMode(34): PatternFillMode,
	//  -- display appearance parameters
	//  cursorPattern(14): DisplayFace.Cursor,
	//  complemented(35): BOOLEAN,
	//  -- color lookup table information
	//  colorIndex(36): CARDINAL,
	//  color(4): LookupTableEntry];

	//DisplayFCBType: TYPE = MACHINE DEPENDENT RECORD [
	//  command(0): DisplayCommand,
	//  status(1): DisplayStatus,
	//  displayMemoryAddress(2): LONG UNSPECIFIED,
	//  color(4): LookupTableEntry];
	//  colorMapping(6): ARRAY [0..1] OF CARDINAL,
	//  destRectangle(8): DisplayRectangle,
	//  sourceOrigin(12): DisplayCoordinate,
	//  cursorPattern(14): DisplayFace.Cursor,
	//  pattern(30): ARRAY [0..4) OF WORD,
	//  patternFillMode(34): PatternFillMode,
	//  -- display appearance parameters
	//  complemented(35): BOOLEAN,
	//  -- color lookup table information
	//  colorIndex(36): CARDINAL,
	//  -- initialization values
	//  displayType(37): DisplayType,
	//  displayWidth(38): CARDINAL,
	//  displayHeight(39): CARDINAL,
	//  -- general display command information
	//  -- display data change parameters

	struct DisplayFCBType {
		CARD16              command;
		CARD16              status;
		CARD32              displayMemoryAddress; // real address (not virtual address)
		LookupTableEntry    color;
		CARD16              colorMapping[2];
		DisplayRectangle    destRectangle;
		DisplayCoordinate   sourceOrigin;
		DisplayFace::Cursor cursorPattern;
		CARD16              pattern[4];
		CARD16              patternFillMode;
		CARD16              complemented;
		CARD16              colorIndex;
		CARD16              displayType;
		CARD16              displayWidth;
		CARD16              displayHeight;
	};
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.3/Faces/Private/EthernetIOFaceGuam.mesa
//
class EthernetIOFaceGuam {
public:
	//EthernetFCBType: TYPE = MACHINE DEPENDENT RECORD [
	//  receiveIOCB(0): EthernetIOCBPtr,
	//  transmitIOCB(2): EthernetIOCBPtr,
	//  receiveInterruptSelector(4): GuamInputOutput.InterruptSelectorType,
	//  transmitInterruptSelector(5): GuamInputOutput.InterruptSelectorType,
	//  stopAgent(6): BOOLEAN,
	//  receiveStopped(7): BOOLEAN,
	//  transmitStopped(8): BOOLEAN,
	//  hearSelf(9): BOOLEAN,
	//  processorID(10): HostNumbers.ProcessorID, -- set by head
	//  packetsMissed(13): CARDINAL,
	//  agentBlockSize(14): CARDINAL]; -- additional words in IOCB for agent
	struct EthernetFCBType {
		CARD32 receiveIOCB;
		CARD32 transmitIOCB;
		CARD16 receiveInterruptSelector;
		CARD16 transmitInterruptSelector;
		CARD16 stopAgent;
		CARD16 receiveStopped;
		CARD16 transmitStopped;
		CARD16 hearSelf;
		CARD16 processorID[3];
		CARD16 packetsMissed;
		CARD16 agentBlockSize;
	} __attribute__((packed));

	//EthernetIOCBPtr: TYPE = LONG POINTER TO EthernetIOCBType;
	//EthernetIOCBType: TYPE = MACHINE DEPENDENT RECORD [
	//  bufferAddress(0): LONG POINTER,
	//  bufferLength(2): CARDINAL, -- in bytes
	//  actualLength(3): CARDINAL, -- of received packet, in bytes
	//  dequeued(4:0..0): BOOLEAN,
	//  packetType(4:1..7): {receive, transmit},
	//  status(4:8..15): EthernetIOCBStatus � inProgress,
	//  retries(5): CARDINAL � 0,
	//  nextIOCB(6): EthernetIOCBPtr � NIL];
	struct EthernetIOCBType {
		CARD32 bufferAddress;
		CARD16 bufferLength;
		CARD16 actualLength;
		union {
			struct {
				CARD16 status     : 8;
				CARD16 packetType : 7;
				CARD16 dequeued   : 1;
			};
			CARD16 u4;
		};
		CARD16 retries;
		CARD32 nextIOCB;
	};

	// packetType(4:1..7): {receive, transmit},
	static const CARD16 PT_receive  = 0;
	static const CARD16 PT_transmit = 1;

	//EthernetIOCBStatus: TYPE = MACHINE DEPENDENT {
	//  inProgress(1), completedOK(2), tooManyCollisions(4), badCRC(8), alignmentError(16),
	//  packetTooLong(32), badCRCAndAlignmentError(128)};
	static const CARD16 S_inProgress              =   1;
	static const CARD16 S_completedOK             =   2;
	static const CARD16 S_tooManyCollisions       =   4;
	static const CARD16 S_badCRC                  =   8;
	static const CARD16 S_alignmentError          =  16;
	static const CARD16 S_packetTooLong           =  32;
	static const CARD16 S_bacCRDAndAlignmentError = 128;
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.3/Faces/Private/FloppyIOFaceGuam.mesa
//
class FloppyIOFaceGuam {
public:
	//  FloppyDCBType: TYPE = MACHINE DEPENDENT RECORD [
	//    deviceType(0): Device.Type,
	//    numberOfCylinders(1): CARDINAL,
	//    numberOfHeads(2): CARDINAL,
	//    sectorsPerTrack(3): CARDINAL,
	//    ready(4): BOOLEAN,
	//    diskChanged(5): BOOLEAN,
	//    twoSided(6): BOOLEAN,
	//    suggestedTries(7): CARDINAL];
	struct FloppyDCBType {
		CARD16 deviceType;        // Device.Type
		CARD16 numberOfCylinders;
		CARD16 numberOfHeads;
		CARD16 sectorsPerTrack;
		CARD16 ready;             // BOOLEAN
		CARD16 diskChanged;       // BOOLEAN
		CARD16 twoSided;          // BOOLEAN
		CARD16 suggestedTries;    // BOOLEAN
	};

	//  FloppyIOCBPtr: TYPE = LONG POINTER TO FloppyIOCBType;
	//  FloppyIOCBType: TYPE = MACHINE DEPENDENT RECORD [
	//    operation(0): FloppyDiskFace.Operation,
	//    density(8): FloppyDiskFace.Density,
	//    sectorLength(9): CARDINAL, -- in words
	//    sectorsPerTrack(10): CARDINAL,
	//    status(11): FloppyDiskFace.Status � inProgress,
	//    nextIOCB(12): FloppyIOCBPtr � NIL,
	//    retries(14): CARDINAL � 0,
	//    logStatus(15): UNSPECIFIED � 0];
	struct FloppyIOCBType {
		FloppyDiskFace::Operation operation;
		CARD16 density;            // FloppyDiskFace.Density
		CARD16 sectorLength;
		CARD16 sectorsPerTrack;
		CARD16 status;             // FloppyDiskFace.Status
		CARD32 nextIOCB;
		CARD16 retries;
		CARD16 logStatus;
	};

	//  FloppyFCBType: TYPE = MACHINE DEPENDENT RECORD [
	//    nextIOCB(0): FloppyIOCBPtr,
	//    interruptSelector(2): GuamInputOutput.InterruptSelectorType,
	//    stopAgent(3): BOOLEAN,
	//    agentStopped(4): BOOLEAN,
	//    numberOfDCBs(5): CARDINAL,
	//    dcbs(6): SEQUENCE COMPUTED CARDINAL OF FloppyDCBType];
	struct FloppyFCBType {
		CARD32 nextIOCB;          // FloppyIOCBPtr
		CARD16 interruptSelector; // GuamInputOutput.InterruptSelectorType
		CARD16 stopAgent;         // BOOLEAN
		CARD16 agentStopped;      // BOOLEAN
		CARD16 numberOfDCBs;      // BOOLEAN
		FloppyDCBType dcbs[0];    // SEQUENCE COMPUTED CARDINAL OF FloppyDCBType
	};
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.3/Faces/Private/KeyboardIOFaceGuam.mesa
//
class KeyboardIOFaceGuam {
public:
	//KeyboardFCBType: TYPE = MACHINE DEPENDENT RECORD [
	//  keyStates: ARRAY [0..7) OF WORD];
	struct KeyboardFCBType {
		CARD16 keyStates[7];
	};
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.3/Faces/Private/MouseIOFaceGuam.mesa
//
class MouseIOFaceGuam {
public:
	//MousePositionType: TYPE = MACHINE DEPENDENT RECORD [
	//  mouseXCoord(0): INTEGER,
	//  mouseYCoord(1): INTEGER];
	struct MousePositionType {
		INT16 mouseXCoord;
		INT16 mouseYCoord;
	};

	//MouseFCBType: TYPE = MACHINE DEPENDENT RECORD [
	//  currentMousePosition(0): MousePositionType,
	//  cursorOffset(2): MousePositionType,
	//  newValue(4): MousePositionType,
	//  command(6): MouseCommandType];
	struct MouseFCBType {
		MousePositionType currentMousePosition;
		MousePositionType cursorOffset;
		MousePositionType newValue;
		CARD16            command;
	};

	//MouseCommandType: TYPE = MACHINE DEPENDENT {
	//  nop(0), setPosition(1), setCursorPosition(2)};
	static const CARD16 C_nop = 0;
	static const CARD16 C_setPosition = 1;
	static const CARD16 C_setCursorPosition = 2;
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.3/Faces/Private/ParallelIOFaceGuam.mesa
//
class ParallelIOFaceGuam {
public:
	//ParallelFCBType: TYPE = MACHINE DEPENDENT RECORD [
	//  nextIOCB(0): ParallelIOCBPtr,
	//  interruptSelector(2): GuamInputOutput.InterruptSelectorType,
	//  stopAgent(3): BOOLEAN,
	//  agentStopped(4): BOOLEAN,
	//  numberOfPorts(5): CARDINAL];
	struct ParallelFCBType {
		CARD32 nextIOCB;
		CARD16 interruptSelector;
		CARD16 stopAgent;
		CARD16 agentStopped;
		CARD16 numberOfPorts;
	};

	//ParallelIOCBPtr: TYPE = LONG POINTER TO ParallelIOCBType;
	//ParallelIOCBType: TYPE = MACHINE DEPENDENT RECORD [
	//  operation(0): BiParallelPortFace.Operation,
	//  port(3): CARDINAL,
	//  transferMode(4): BiParallelPortFace.TransferMode,
	//  controlOrData(5): BiParallelPortFace.ControlOrData,
	//  nextIOCB(6): ParallelIOCBPtr � NIL,
	//  status(8): ParallelIOCBStatus � inProgress,
	//  bytesTransferred(9): CARDINAL];
	struct ParallelIOCBType {
		BiParallelPortFace::Operation operation;
		CARD16 port;
		CARD16 transferMode;
		CARD16 controlOrData;
		CARD32 nextIOCB;
		CARD16 status;
		CARD16 bytesTransferred;
	} __attribute__((packed));

	//ParallelIOCBStatus: TYPE = MACHINE DEPENDENT {
	//  inProgress(0), completed(1), aborted(2)};
	static const CARD16 S_inProgress = 0;
	static const CARD16 S_completed  = 1;
	static const CARD16 S_aborted    = 2;
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.3/Faces/Private/ProcessorIOFaceGuam.mesa
//
class ProcessorIOFaceGuam {
public:
	//  ProcessorFCBType: TYPE = MACHINE DEPENDENT RECORD [
	//    processorID(0): ProcessorFace.ProcessorID,
	//    microsecondsPerHundredPulses(3): CARDINAL,
	//    millisecondsPerTick(4): CARDINAL,
	//    alignmentFiller(5): WORD,
	//    realMemoryPageCount(6): LONG CARDINAL,
	//    virtualMemoryPageCount(8): LONG CARDINAL,
	//    gmt(10): System.GreenwichMeanTime,
	//    command(12): ProcessorCommand,
	//    status(13): ProcessorStatus];
	struct ProcessorFCBType {
		CARD16 processorID[3];
		CARD16 microsecondsPerHundredPulses;
		CARD16 millisecondsPerTick;
		CARD16 alignmentFiller;
		CARD32 realMemoryPageCount;
		CARD32 virtualMemoryPageCount;
		CARD32 gmt;
		CARD16 command;
		CARD16 status;
	};

	//  ProcessorCommand: TYPE = MACHINE DEPENDENT {
	//    noop(0), readGMT(1), writeGMT(2)};
	static const CARD16 C_noop     = 0;
	static const CARD16 C_readGMT  = 1;
	static const CARD16 C_writeGMT = 2;

	//  ProcessorStatus: TYPE = MACHINE DEPENDENT {
	//    inProgress(0), success(1), failure(2)};
	static const CARD16 S_inProgress = 0;
	static const CARD16 S_success    = 1;
	static const CARD16 S_failure    = 2;
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.3/Faces/Private/SerialIOFaceGuam.mesa
//
class SerialIOFaceGuam {
public:
	//SerialFaceFCBType: TYPE = MACHINE DEPENDENT RECORD [
	//  txIOCB(0): SerialIOCBPtr,
	//  rxIOCB(2): SerialIOCBPtr,
	//  stopAgent(4): BOOLEAN,
	//  agentStopped(5): BOOLEAN,
	//  interruptSelector(6): GuamInputOutput.InterruptSelectorType,
	//  command(7): SerialCommand,
	//  parameterBlock(8): NewRS232CFace.ParameterRecord,
	//  controlFlags(15): NewRS232CFace.ControlRecord,
	//  deviceStatus(16): NewRS232CFace.DeviceStatus,
	//  commandStatus(17): NewRS232CFace.ParameterStatus,
	//  agentBlockSize(18): CARDINAL]; -- additional words in IOCB for agent
	struct SerialFaceFCBType {
		CARD32 txIOCB;
		CARD32 rxIOCB;
		CARD16 stopAgent;
		CARD16 agentStopped;
		CARD16 interruptSelector;
		CARD16 command;
		NewRS232CFace::ParameterRecord parameterBlock;
		NewRS232CFace::ControlRecord controlFlags;
		NewRS232CFace::DeviceStatus deviceStatus;
		CARD16 commandStatus;
		CARD16 agentBlockSize;
	} __attribute__((packed));

	//SerialCommand: TYPE = MACHINE DEPENDENT {
	//  noOp(0), abortReceive(1), abortTransmit(2), breakOff(3), breakOn(4), getDeviceStatus(5),
	//  off(6), on(7), receive(8), setControlBits(9), setParameters(10), transmit(11)};
	static const CARD16 C_noOp            =  0;
	static const CARD16 C_abortReceive    =  1;
	static const CARD16 C_abortTransmit   =  2;
	static const CARD16 C_breakOff        =  3;
	static const CARD16 C_breakOn         =  4;
	static const CARD16 C_getDeviceStatus =  5;
	static const CARD16 C_off             =  6;
	static const CARD16 C_on              =  7;
	static const CARD16 C_receive         =  8;
	static const CARD16 C_setControlBits  =  9;
	static const CARD16 C_setParameters   = 10;
	static const CARD16 C_transmit        = 11;

	//SerialIOCBPtr: TYPE = LONG POINTER TO SerialIOCBType;
	//SerialIOCBType: TYPE = MACHINE DEPENDENT RECORD [
	//  operation(0): NewRS232CFace.Operation,
	//  dequeued(3:0..0): BOOLEAN,
	//  iocbType(3:1..15): {receive, transmit},
	//  actualLength(4): CARDINAL � 0,
	//  status(5): NewRS232CFace.TransferStatus � inProgress,
	//  nextIOCB(6): SerialIOCBPtr � NIL];
	struct SerialIOCBType {
		NewRS232CFace::Operation operation;
		union {
			struct {
				CARD16 iocbType : 15;
				CARD16 dequeued :  1;
			};
			CARD16 u3;
		};
		CARD16 actualLength;
		CARD16 status;
		CARD32 nextIOCB;
	};
	static const CARD16 T_receive = 0;
	static const CARD16 T_ransmit = 1;
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.3/Faces/Private/TTYPortIOFaceGuam.mesa
//
class TTYPortIOFaceGuam {
public:
	//TTYFCBType: TYPE = MACHINE DEPENDENT RECORD [
	//  ttyInterruptSelector(0):	GuamInputOutput.InterruptSelectorType,
	//  command(1):			TTYCommandType,
	//  status(2):			TTYStatusType,
	//  data(3):			CHARACTER,
	//  -- parameters from Head to Agent
	//  lineSpeed(4):		LineSpeedType,
	//  charLength(5):		CharLengthType,
	//  parity(6):			ParityType,
	//  stopBits(7):		StopBitsType,
	//  CTS(8):			BOOLEAN,
	//  DSR(9):			BOOLEAN,
	//  -- device status from Agent to Head
	//  RTS(10):			BOOLEAN,
	//  DTR(11):			BOOLEAN,
	//  readyToReceive(12):		BOOLEAN,
	//  readyToTransmit(13):	BOOLEAN];
	struct TTYFCBType {
		CARD16 ttyInterruptSelector;
		CARD16 command;
		CARD16 status;
		CARD16 data;
		CARD16 lineSpeed;
		CARD16 charLength;
		CARD16 parity;
		CARD16 stopBits;
		CARD16 CTS;
		CARD16 DSR;
		CARD16 RTS;
		CARD16 DTR;
		CARD16 readyToReceive;
		CARD16 readyToTransmit;
	};

	//TTYCommandType: TYPE = MACHINE DEPENDENT {
	//  nop(0), transmit(1), receive(2), open(3), close(4), setParams(5), sendBreak(6)};
	static const CARD16 C_nop       = 0;
	static const CARD16 C_transmit  = 1;
	static const CARD16 C_receive   = 2;
	static const CARD16 C_open      = 3;
	static const CARD16 C_close     = 4;
	static const CARD16 C_setParam  = 5;
	static const CARD16 C_sendBreak = 6;

	//TTYStatusType: TYPE = MACHINE DEPENDENT {
	//  success(0), breakDetected(1), dataLost(2), parityError(3), framingError(4)};
	static const CARD16 S_success       = 0;
	static const CARD16 S_breakDetected = 1;
	static const CARD16 S_dataLost      = 2;
	static const CARD16 S_parityError   = 3;
	static const CARD16 S_framingError  = 4;

	//CharLengthType: TYPE = MACHINE DEPENDENT
	//  {lengthIs5bits, lengthIs6bits, lengthIs7bits, lengthIs8bits};
	static const CARD16 CL_lengthIs5bits = 0;
	static const CARD16 CL_lengthIs6bits = 1;
	static const CARD16 CL_lengthIs7bits = 2;
	static const CARD16 CL_lengthIs8bits = 3;

	//LineSpeedType: TYPE = MACHINE DEPENDENT
	//  {bps50, bps75, bps110, bps150, bps300, bps600, bps1200, bps1800, bps2000, bps2400, bps3600, bps4800, bps7200, bps9600, bps19200};
	static const CARD16 LS_bps50    =  0;
	static const CARD16 LS_bps75    =  1;
	static const CARD16 LS_bps110   =  2;
	static const CARD16 LS_bps150   =  3;
	static const CARD16 LS_bps300   =  4;
	static const CARD16 LS_bps600   =  5;
	static const CARD16 LS_bps1200  =  6;
	static const CARD16 LS_bps1800  =  7;
	static const CARD16 LS_bps2000  =  8;
	static const CARD16 LS_bps2400  =  9;
	static const CARD16 LS_bps3600  = 10;
	static const CARD16 LS_bps4800  = 11;
	static const CARD16 LS_bps7200  = 12;
	static const CARD16 LS_bps9600  = 13;
	static const CARD16 LS_bps19200 = 14;

	//ParityType: TYPE = MACHINE DEPENDENT
	//  {none, odd, even};
	static const CARD16 P_none = 0;
	static const CARD16 P_odd  = 1;
	static const CARD16 P_even = 2;

	//StopBitsType: TYPE = MACHINE DEPENDENT
	//  {one, oneAndHalf, two};
	static const CARD16 SB_one        = 0;
	static const CARD16 SB_oneAndHalf = 1;
	static const CARD16 SB_two        = 2;
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.3/Faces/Private/RealMemoryImplGuam.mesa
//
class RealMemoryImplGuam {
public:
	//largestArraySize: CARDINAL = 4086;	-- supports almost 32 MB of real memory
	//
	//--RealMemory.--
	//allocMap: PUBLIC DESCRIPTOR FOR ARRAY OF WORD � DESCRIPTOR[allocMapArray];
	//allocMapArray: ARRAY [0..largestArraySize) OF WORD;  -- (initial state not important)
	static const int largestArraySize = 4086;
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.0.1/Mesa/Friends/LoadStateFormat.mesa
//
class LoadStateFormat {
public:
	//VersionID: CARDINAL = 06113;
	static const CARD16 VersionID = 6113;

	//Index: TYPE = CARDINAL;
	//nullIndex: Index = LAST[Index];
	static const CARD16 nullIndex = (CARD16)~0U;

	//ModuleInfoIndex: TYPE = Handle RELATIVE POINTER TO ARRAY [0..0) OF ModuleInfo;
	//BcdInfoIndex:    TYPE = Handle RELATIVE POINTER TO ARRAY Index[0..0) OF BcdInfo;

	//ID: TYPE = CARDINAL;

	//From PrincOps
	//  GlobalFrameHandle: TYPE = POINTER TO GlobalVariables
	//From PrincOpsExtras2
	//  GFTHandle: TYPE = GlobalFrameTable RELATIVE ORDERED POINTER [0..LAST[CARDINAL]] TO GFTItem;

	//ModuleInfo: TYPE = MACHINE DEPENDENT RECORD [
	//  resolved(0:0..0): BOOLEAN,
	//  cgfi(0:1..15): NATURAL,
	//  index(1): Index,
	//  globalFrame(2): SELECT OVERLAID * FROM
	//    oldWorld => [gf(2): PrincOps.GlobalFrameHandle],
	//    newWorld => [gfi(2): PrincOpsExtras2.GFTHandle],
	//    ENDCASE];
	struct ModuleInfo {
		union {
			CARD16 u0;
			struct {
				CARD16 cgfi     : 15;
				CARD16 resolved :  1;
			};
		};
		CARD16 index;
		CARD16 globalFrame; // can be PrincOps.GlobalFrameHandle or PrincOpsExtras2.GFTHandle
	} __attribute__((packed));


	//BcdInfo: TYPE = MACHINE DEPENDENT RECORD [
	//  exports(0:0..0), typeExported(0:1..1): BOOLEAN,
	//  pages(0:2..15): [1..256],
	//  base(1): LONG BASE POINTER TO BcdDefs.BCD,
	//  id(3): ID];
	struct BcdInfo {
		union {
			CARD16 u0;
			struct {
				CARD16 pages        : 14;
				CARD16 typeExported :  1;
				CARD16 exports      :  1;
			};
		};
		CARD32 base;
		CARD16 id;
	} __attribute__((packed));


	//Object: TYPE = MACHINE DEPENDENT RECORD [
	//  versionident: CARDINAL,
	//  nModules: CARDINAL,
	//  maxModules: CARDINAL,
	//  nBcds: CARDINAL,
	//  maxBcds: CARDINAL,
	//  nextID: ID,
	//  moduleInfo: ModuleInfoIndex,
	//  bcdInfo: BcdInfoIndex];

	struct Object {
		CARD16 versionident;
		CARD16 nModules;
		CARD16 maxModules;
		CARD16 nBcds;
		CARD16 maxBcds;
		CARD16 nextID;
		CARD16 moduleInfo;
		CARD16 bcdInfo;
	} __attribute__((packed));
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// From APilot/15.0.1/Pilot/Friends/CPSwapDefs.mesa
//
class CPSwapDefs {
public:
	//oldVersion: CARDINAL = 01054;
	//currentVersion: CARDINAL = 01055;
	static const CARD16 currentVersion = 1055;

	//SwapInfo: TYPE = MACHINE DEPENDENT RECORD [
	//  -- This is the actual usage of PSB.PDA.available.
	//  externalStateVector(0): LONG POINTER TO ExternalStateVector,
	//  availableC(2), availableD(3), availableE(4): WORD];
	struct SwapInfo {
		CARD32 externalStateVector; // LONG POINTER TO ExternalStateVector
		CARD16 availableC;
		CARD16 availableD;
		CARD16 availableE;
	} __attribute__((packed));

	static const CARD32 SWAPINFO = PDA + OFFSET(ProcessDataArea, available);


	//PESV: PROCEDURE[] RETURNS [LONG POINTER TO ExternalStateVector] = INLINE {
	//  RETURN[LOOPHOLE[PSB.PDA.available, SwapInfo].externalStateVector]};
	static const CARD32 PSEV = SWAPINFO + OFFSET(SwapInfo, externalStateVector);

	//ExternalStateVector: TYPE = MACHINE DEPENDENT RECORD [
	//	-- Items describing the boot session in general:
	//	version(0): CARDINAL,  -- version of CPSwapDefs. MUST BE FIRST.
	//	bootSession(1): LONG CARDINAL,  -- random number identifying boot session.
	//	    loadState(3): LONG POINTER TO LoadStateFormat.Object,
	//	    mapLog(5): LONG POINTER TO VMMapLog.Descriptor,
	//	    patchTable(7): LONG POINTER TO PatchTable,
	//	    breakBlocks(9): LONG POINTER TO BBArray,
	//	    breakpointHandlers(11): LONG POINTER TO BreakpointHandlerItem,
	//	    mds(13): CARDINAL,  -- MDS of *all* processes.
	//	    faultsBeingProcessed(14): ARRAY FaultType OF FaultQueuePtr,
	//	    systemVolumeID(20): Volume.ID,  -- (nullID if none or not known yet)
	//	    virtualMemoryCount(25): Environment.PageCount,
	//	    patchTableInUse(27:0..12): BOOLEAN,  -- (in use by debuggee)
	//	    breakBlocksInUse(27:13..13): BOOLEAN,  -- (in use by debuggee)
	//	    loadStateChanging(27:14..14): BOOLEAN,
	//	    loadStateDirty(27:15..15): BOOLEAN,  -- set by debuggee, reset by debugger
	//	    -- Items describing the current trip to or from the debugger:
	//	    swapData(28): SwapData,
	//	    spareA(48): WORD,
	//	    spareB(49): WORD];

	struct ExternalStateVector {
		CARD16 version;                 // version of CPSwapDefs. It must be 01055
		CARD32 bootSession;             // random number identifying boot session
		CARD32 loadState;               // LONG POINTER TO LoadStateFormat.Object
		CARD32 mapLog;                  // LONG POINTER TO VMMapLog.Descriptor
		CARD32 patchTable;              // LONG POINTER TO PatchTable
		CARD32 breakBlocks;             // LONG POINTER TO BBArray
		CARD32 breakpointHandlers;      // LONG POINTER TO BreakpointHandlerItem
		CARD16 mds;                     // MDS of *all* processes
		CARD16 faultsBeingProcessed[6];
		CARD16 systemVolumeID[5];       // -- nullID if none or not known yet
		CARD32 virtualMemoryCount;
		union {
			CARD16 u27;
			struct {
				CARD16 loadStateDirty    :  1; // set by debugee, reset by debugger
				CARD16 loadStateChanging :  1;
				CARD16 breakBlocksInUse  :  1;
				CARD16 patchTableInUse   : 13;
			};
		};
		CARD16 swapData[20];
		CARD16 spareA;
		CARD16 spareB;
	} __attribute__((packed));
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// APilot/15.0.1/Mesa/Public/PrincOpsExtras2.mesa
//
class PrincOpsExtras2 {
public:
//  -- GFT
//
//  GFTIndex: TYPE = CARDINAL[0..16384);
	static const CARD16 GFTIndex_FIRST = 0;
	static const CARD16 GFTIndex_LAST  = 16383;
//
//  GlobalFrameTable: TYPE = LONG BASE POINTER TO ARRAY GFTIndex OF GFTItem;
//
//  GFT: GlobalFrameTable = LOOPHOLE[400000B];
//
//  GFTHandle: TYPE = GlobalFrameTable RELATIVE ORDERED POINTER [0..LAST[CARDINAL]] TO GFTItem;
//
//  EmptyGFTItem: GFTItem = [inuse[globalFrame: NIL, codebase: [code[NIL]]]];
//  nullGFI: GFTIndex = FIRST[GFTIndex];
	static const CARD16 nullGFI = 0;

//  nullGFH: GFTHandle = FIRST[GFTHandle];
//
//	-- First page reserved for Germ
//  GermUseOnly: TYPE = GFTIndex[0..64);  -- Environment.wordsPerPage / SIZE[GFTItem]
	static const CARD16 GermUseOnly_FIRST =  0;
	static const CARD16 GermUseOnly_LAST  = 63;

//	-- reserved for Pilot use
//  Reserved: TYPE = GFTIndex[LAST[GermUseOnly]+1..LAST[GermUseOnly]+6);
	static const CARD16 Reserved_FIRST = GermUseOnly_LAST + 1;
	static const CARD16 Reserved_LAST  = GermUseOnly_LAST + 6;

//  CrossMDSSlot: GFTIndex = FIRST[Reserved];  -- used for crossMDS linkage in Germ
	static const CARD16 CrossMDSSlot = Reserved_FIRST;
//
//  GFTItem: TYPE = MACHINE DEPENDENT RECORD [
//	 SELECT OVERLAID * FROM
//	   inuse => [
//		 globalFrame: LongGlobalFrameHandle,
//		 codebase: PrincOps.GlobalCodebase],
//	   free => [
//		 next: GFTHandle,
//	     fill0: CARDINAL � 0,
//	     nullCodebase: LONG CARDINAL � 0]
//	   ENDCASE];
	struct GFTItem {
		CARD32      globalFrame;
		CARD32      codebase;
	};

//
//  codebaseLowOffset: CARDINAL = 2;
//  codebaseHighOffset: CARDINAL = 3;
//
//  GFTIndexToHandle: PROCEDURE [gfi: GFTIndex] RETURNS [GFTHandle] = INLINE {
//	RETURN[ LOOPHOLE[gfi * SIZE[GFTItem]] ] };
//
//  GFTHandleToIndex: PROCEDURE [gfh: GFTHandle] RETURNS [GFTIndex] = INLINE {
//	RETURN[ LOOPHOLE[gfh] / SIZE[GFTItem] ] };
};

#endif
