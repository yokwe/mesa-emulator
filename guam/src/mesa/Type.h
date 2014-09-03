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
// Types.h
//

#ifndef TYPE_H_
#define TYPE_H_ 1

#include "../util/Util.h"

#include "MesaBasic.h"
#include "Constant.h"

// 2.1 Basic Data Types
const int WordSize = Environment::bitsPerWord;

// 2.1.2 Bit, Nibble, Byte
// NibblePair: TYPE = MACHINE DEPENDENT RECORD[left (0:0..3) NIBBLE, right(0:4..7) NIBBLE];
union NibblePair {
	CARD8 u;
	struct {
		CARD8 right: 4;
		CARD8 left : 4;
	};
};
// BytePair: TYPE =  MACHINE DEPENDENT RECORD [left (0: 0..7), right (0: 8..15): BYTE];
union BytePair {
	CARD16 u;
	struct {
		CARD16 right : 8;
		CARD16 left  : 8;
	};
};

static inline BYTE HighByte(UNSPEC u) {
	BytePair t = {u};
	return t.left;
}
static inline BYTE LowByte(UNSPEC u) {
	BytePair t = {u};
	return t.right;
}

// 2.1.3.1 Basic Logical Operators
static inline UNSPEC Not(UNSPEC u) {
	return ~u;
}
static inline int Odd(UNSPEC u) {
	return u & 0x01;
}

static inline UNSPEC Shift(UNSPEC data, int count) {
	if (0 < count) {
		if (16 <= count) return 0;
		return (UNSPEC)(data << count);
	}
	if (count < 0) {
		if (count <= -16) return 0;
		return (UNSPEC)(data >> (-count));
	}
	return data;
}
static inline UNSPEC Rotate(UNSPEC data, int count) {
	if (0 < count) {
		if (16 <= count) count = count % 16;
		int t = data << count;
		return (UNSPEC)((t & (0xffff)) | ((t >> 16) & 0xffff));
	}
	if (count < 0) {
		if (count <= -16) count = -(-count % 16);
		int t = data << (16 + count);
		return (UNSPEC)((t & (0xffff)) | ((t >> 16) & 0xffff));
	}
	return data;
}

// 2.1.3.2 Basic Arithmetic Operator
static inline INT16 ArithShift(INT16 data, int count) {
	if (0 < count) {
		if (16 <= count) return 0;
		return (INT16)(((data << count) & 0x7fff) | (data & 0x8000));
	}
	if (count < 0) {
		if (count <= -16) return 0;
		return (INT16)(data >> (-count));
	}
	return data;
}

// 2.3.1 Long Types
// When these types are stored in memory, the low-order (least significant) sixteen bits
// occupy the first memory word (at the lower numbered address), and the high-order (most
// significant) sixteen bits occupy the second memory word(at the higher memory address).
//         |15    31|0   15|
// address  n        n+1    n+2

// Long: TYPE = MACHINE DEPENDENT RECORD[low(0), high(1): UNSPECIFIED];
union Long {
	CARD32 u;
	struct {
		CARD16 low;
		CARD16 high;
	};
};

static inline UNSPEC HighHalf(LONG_UNSPEC u) {
//	Long t = {u};
//	return t.high;
	return (UNSPEC)(u >> WordSize);
}
static inline UNSPEC LowHalf(LONG_UNSPEC u) {
//	Long t = {u};
//	return t.low;
	return (UNSPEC)u;
}

static inline LONG_UNSPEC LongShift(LONG_UNSPEC data, int count) {
	if (0 < count) {
		if (32 <= count) return 0;
		return (LONG_UNSPEC)(data << count);
	}
	if (count < 0) {
		if (count <= -32) return 0;
		return (LONG_UNSPEC)(data >> (-count));
	}
	return data;
}
static inline INT32 LongArithShift(INT32 data, int count) {
	if (0 < count) {
		if (32 <= count) return 0;
		return (INT32)(((data << count) & 0x7fffffff) | (data & 0x80000000));
	}
	if (count < 0) {
		if (count <= -32) count = -32;
		return (INT32)(data >> (-count));
	}
	return data;
}

// 2.4.1 Assignment
static inline INT16 SignExtend(BYTE z) {
	return (INT8)z;
}

// 3.1.1 Virtual Memory Mapping
//MapFlags: TYPE = MACHINE DEPENDENT RECORD (
//  reserved (0:0..12) : UNSPEClFIED[0..17777B],
//  protected (0:13..13) : BOOLEAN,
//  dirty (0:14..14): BOOLEAN,
//  referenced (0:15..15): BOOLEAN];
union MapFlags {
	CARD16 u;
	struct {
		CARD16 referenced :  1;
		CARD16 dirty      :  1;
		CARD16 protect    :  1;
		CARD16 reserved   : 13;
	};
};
static inline int Vacant(MapFlags mf) {
	return mf.protect && mf.dirty && !mf.referenced;
}
static inline int Protect(MapFlags mf) {
	return mf.protect;
}

// 3.1.4.3 Code Segments
struct CodeSegment {
	UNSPEC available[4];
	UNSPEC block[0];
};

// 3.2 Main Data Spaces
// MdsHandle: TYPE = LONG POINTER TO MainDataSpace;
typedef LONG_POINTER MdsHandle;
// MainDataSpace: TYPE = BLOCK

// 3.2.2.1 Reserved Locations
// AV: POINTER TO AllocationVector;
// SD: POINTER TO SystemData;
// ETT: POINTER TO EscTrapTable;

// 3.2.2.2 Local and Global Frames
// GlobalFrameHandle: TYPE = LONG POINTER TO GlobalVariables;
// GlobalVariables: TYPE = BLOCK;
typedef LONG_POINTER GlobalFrameHandle;

//GlobalFrameBase: TYPE = LONG POINER TO GlobalOverhead;
typedef LONG_POINTER GlobalFrameBase;


// From APilot/15.3/Mesa/Public/PrincOpsExtras2.mesa
// GlobalWord: TYPE = MACHINE DEPENDENT RECORD [
//   gfi (0:0..13): GFTIndex,
//   trapxfers (0:14..14),
//   codelinks (0:15..15): BOOLEAN];
union GlobalWord {
	CARD16 u;
	struct {
		CARD16 codelinks :  1;
		CARD16 trapxfers :  1;
		CARD16 gfi       : 14;
	};
};

//GlobalOverhead : TYPE =  MACHINE DEPENDENT RECORD (
//  available (0): UNSPECIFIED,
//  word      (1): GlobalWord,
//  global    (2): GlobaiVariables];
struct GlobalOverhead {
	UNSPEC       available;
	CARD16       word;
	UNSPEC       global[0];
};

// GlobalBase: PROC[frame: GlobalFrameHandle] RETURNS [GlobalFrameBase]
static inline GlobalFrameBase GlobalBase(GlobalFrameHandle frame) {
	return frame - SIZE(GlobalOverhead);
}

#define GO_OFFSET(p,m) (GlobalBase(p) + OFFSET(GlobalOverhead, m))


// 9.1.4.1 Global Frame Table
// GFT: LONG POINTER TO GlobalFrameTable = LOOPHOLE[mGFT];
const LONG_POINTER GFT = mGFT;

// GlobalFrameTable: TYPE = LONG BASE POINTER TO ARRAY GFTIndex OF GFTItem;
// GFTIndex: TYPE = [0..16384);
// GFTHandle: TYPE = GlobalFrameTable RELATIVE POINTER TO GFTItem;
// GFTItem: TYPE = MACHINE DEPENDENT RECORD[
//   globalFrame(0): GlobalFrameHandle,
//   codebase(2): LONG POINTER TO CodeSegment];
struct GFTItem {
	GlobalFrameHandle globalFrame;
	LONG_POINTER      codebase;
};
//typedef GFTItem GlobalFrameTable[16384];
const int GFTIndex_SIZE = 16384;

struct GlobalFrameTable {
	GFTItem item[GFTIndex_SIZE];
};
typedef POINTER GFTHandle;
typedef CARD16 GFTIndex;


//#define GFT_OFFSET(n,m) (GFT + OFFSET(GlobalFrameTable, item[n]) + OFFSET(GFTItem, m))
#define GFT_OFFSET(n,m) (GFT + OFFSET(GlobalFrameTable, item[(n / SIZE(GFTItem))].m))


// LocalFrameHandle: TYPE = POINTER TO LocalVaribles;
// LocalVariables: TYPE = BLOCK;
typedef POINTER LocalFrameHandle;

//LocalWord: TYPE = MACHINE DEPENDENT RECORD[
//  available(0:0..7): BYTE,
//  fsi(0:8..15): FSIndex];
union LocalWord {
	CARD16 u;
	struct {
		CARD16 fsi       : 8;
		CARD16 available : 8;
	};
};

//LocalFrameBase: TYPE = POINTER TO LocalOverhead;
typedef CARD16 LocalFrameBase;

//LocalOverhead : TYPE = MACHINE DEPENDENT RECORD [
//  word (0):       LocalWord.
//  returnlink(1) : ShortControlLink,
//  globallink(2) : GFTHandle,
//  pc(3):          CARDINAL,
//  local(4):       LocaiVariables];
struct LocalOverhead {
	CARD16    word;
	UNSPEC    returnlink;
	GFTHandle globallink;
	CARDINAL  pc;
	UNSPEC    local[0];
};

// LocalBase: PROC[frame: LocalFrameHandle] RETURNS[LocalFrameBase]
static inline LocalFrameBase LocalBase(LocalFrameHandle frame) {
	return (CARD16)(frame - (CARD16)SIZE(LocalOverhead));
}

#define LO_OFFSET(p,m) (CARD16)(LocalBase(p) + OFFSET(LocalOverhead, m))


// 3.3.2 Evaluation Stack
const int StackDepth = cSS;

//// 7.5 Field Instruction
//FieldSpec: TYPE = MACHINE DEPENDENT RECORD [
// pos (0:0..3):  NIBBLE,
// size (0:4..7): NIBBLE];
union FieldSpec {
	CARD8 u;
	struct {
		CARD8 size : 4;
		CARD8 pos  : 4;
	};
};

//FieldDesc: TYPE = MACHINE DEPENDENT RECORD [
//  offset(0:0..7) : BYTE,
//  field(0:8..15) : FieldSpec];
union FieldDesc {
	CARD16 u;
	struct {
		CARD16 field  : 8;
		CARD16 offset : 8;
	};
};


// 8.4.1 Bit Transfer Utilities
//BitAddress: TYPE = MACHINE DEPENDENT RECORD[
//  word(0): LONG POINTER,
//  reserved(2:0..11): [0..777B] = 0,
//  bit(2:12:15): [0..WordSize)];
struct BitAddress {
	LONG_POINTER word;
	union {
		struct {
			CARD16       bit      :  4;
			CARD16       reserved : 12;
		};
		CARD16 u;
	};
} __attribute__((packed));



// 8.4.2.1 BitBlt Arguments

//BitBltFlags: TYPE = MACHINE DEPENDENT RECORD[
//  direction(0:0..0): Direction,
//  disjoint(0:1..1): BOOLEAN,
//  disjointItems(0:2..2) BOOLEAN,
//  gray(0:3..3) BOOLEAN,
//  srcFunc(0:4..4) SrcFunc,
//  dstFunc(0:5..6) DstFunc,
//  reserved(0:7..15]: [0..777B] = 0];
union BitBltFlags {
	CARD16 u;
	struct {
		CARD16 reserved     : 9;
		CARD16 dstFunc      : 2;
		CARD16 srcFunc      : 1;
		CARD16 gray         : 1;
		CARD16 disjointItems: 1;
		CARD16 disjoint     : 1;
		CARD16 direction    : 1;
	};
};

// SrcFunc: TYPE = MACHINE DEPENDENT {null, complement};
enum SrcFunc { SF_null = 0, SF_complement = 1 };

// DstFunc: TYPE = MACHINE DEPENDENT {null, and, or, xor};
enum DstFunc { DF_null = 0, DF_and = 1, DF_or = 2, DF_xor = 3 };

// Direction: TYPE = MACHINE DEPENDENT {forward, backward};
enum Direction { DI_forward = 0, DI_backward = 1 };

//BitBltArg: TYPE = MACHINE DPENEDENT RECORD[
//  dst(0): BitAddress,
//  dstBpl(3): INTEGER,
//  src(4) BitAddress,
//  srcBpl(7): INTEGER,
//  width(8): CARDINAL,
//  height(9): CARDINAL,
//  flags(10): BitBltFlags,
//  reserved(11): UNSPEC = 0];
struct BitBltArg {
	BitAddress  dst;
	INT16       dstBpl;
	BitAddress  src;
	INT16       srcBpl;
	CARDINAL    width;
	CARDINAL    height;
	BitBltFlags flags;
	UNSPEC      reserved;
};

//GrayParm: TYPE = MACHINE DEPENDENT RECORD[
//  reserved(0:0..3): NIBBLE = 0,
//  yOffset(0:4..7): NIBBLE,
//  widthMinusOne(0:8..11): NIBBLE,
//  heightMinusOne(0:12..15): NIBBLE];
union GrayParm {
	CARD16 u;
	struct {
		CARD16 heightMinusOne : 4;
		CARD16 widthMinusOne  : 4;
		CARD16 yOffset        : 4;
		CARD16 reserved       : 4;
	};
};

// 8.4.3 Text Block Transfer
enum Function {F_display, F_format, F_resolve, F_unused};

// 8.4.3.1 Font Representation

// Font: TYPE;
// FontHandle: TYPE = LONG POINTER TO Font;

static const int fontRecordAlignment = 16;

// FontRecord: TYPE = MACHINE DEPENDENT RECORD [
//   rasters(0): FontRasters,
//   spacingWidths(2): SpacingWidths,
//   printerWidths(4): PrinterWidths,
//   flags(6): FlagsArray,
//   rasterInfos(8): RasterInfos,
//   height(10): CARDINAL];
struct FontRecord {
	LONG_POINTER rasters;
	LONG_POINTER spacingWidths;
	LONG_POINTER printerWidths;
	LONG_POINTER flags;
	LONG_POINTER rasterInfos;
	CARDINAL     height;
} __attribute__((packed));

// FontBitsPtr: TYPE = LONG BASE POINTER TO ARRAY[0..0) OF UNSPECIFIED;
// FontRasters: TYPE = LONG BASE POINTER TO <<tasters>> ARRAY [0..0) OF WORD;

// Byte: TYPE = [0..255)
// SpacingWidths: TYPE = LONG POINTER TO PACKED ARRAY Byte OF SpacingWidth;
// SpacingWidth: TYPE = Byte;
struct SpacingWidthsRecord {
	CARD8 width[256];
} __attribute__((packed));

// PrinterWidths: TYPE = LONG POINTER TO ARRAY Byte OF PrinterWidth;
// PrinterWidth: TYPE = CARDINAL;
struct PrinterWidthsRecord {
	CARD16 width[256];
} __attribute__((packed));

// FlagsArray: TYPE = LONG POINTER TO PACKED ARRAY Byte OF Flags;
// Flags: TYPE = MACHINE DEPENDENT RECORD [
//   pad(0:0..0): BOOLEAN,
//   stop(0:1..1): BOOLEAN];
struct FlagsArrayRecord {
	// 256 * 2 / 16 = 32
	CARD16 flags[32];
} __attribute__((packed));

// RasterInfos: TYPE = LONG POINTER TO ARRAY Byte OF RasterInfo;
// RasterInfo: TYPE = MACHINE DEPENDENT RECORD [
//   leftKern: BOOLEAN,
//   rightKern: BOOLEAN,
//   offset: RasterOffset];
struct RasterInfo {
	union {
		CARD16 u;
		struct {
			CARD16 offset    : 14;
			CARD16 rightKern :  1;
			CARD16 leftKern  :  1;
		};
	};
};

// RasterOffsetDomain: = CARDINAL[0..37777B];
// RasterOffset: TYPE = FontRasters RELATIVE POINTER [0..37777B] TO <<raster>> UNSPECIFIED;
// RasterOffsetFromDomain: PROC[domain: RasterOffsetDomain] RETURNS[RAsterOffset] = INLINE {RETURN[LOOPHOLE[domain]]};
// RasterDomainFromOffset: PROC{offset: RasterOffset] RETURNS[RasterOffsetDomain] = INLINE {RETURN[LOOPHOLE[offset]]};

static const CARD16 maxLeftKern  = 1;
static const CARD16 maxRightKern = 1;

// 8.4.3.2 TextBlt Arguments and Results

// TxtBltArg: TYPE = MACHINE DEPENDENT RECORD [
//   reserved(0:0..13): [0.37777B] = 0,
//   function(0:14..15): Function,
//   last(1): CARDINAL,
//   text(2): LONG POINTER TO ARRAY CARDIANL Of BytePair,
//   font(4): FontHandle,
//   dst(6): LONG POINTER,
//   dstBpl(8): CARDINAL,
//   margin(9): CARDINAL,
//   space(10): INTEGER,
//   coord(11): LONG POINTER TO ARRAY CARDINAL OF CARDINAL];
#pragma pack(push, 1)
struct TxtBltArg {
	union {
		CARD16 u0;
		struct {
			CARD16 function :  2;
			CARD16 reserved : 14;
		};
	};
	CARD16 last;
	LONG_POINTER text;
	LONG_POINTER font;
	LONG_POINTER dst;
	CARD16       dstBpl;
	CARD16       margin;
	INT16        space;
	LONG_POINTER coord;
} __attribute__((packed));
#pragma pack(pop)

// Result: TYPE = MACHINE DEPENDENT {normal, margin, stop, unused};
enum Result {R_normal, R_margin, R_stop, R_unused};


// 9.1 Control Links

// ControlLink: TYPE = LONG UNSPECIFIED;
typedef CARD32 ControlLink;

// ShortControlLink: TYPE = UNSPECIFIED;
typedef CARD16 ShortControlLink;

// LinkType: TYPE  =  {frame, procedure, indirect};
enum LinkType { LT_frame = 0, LT_oldProcedure = 1, LT_indirect = 2, LT_newProcedure = 3 };

//TaggedControl link: TYPE =  MACHINE DEPENDENT RECORD [
//  data (0:  0..13): [0 .. 377778];
//  tag  (0: 14..15): [0 .. 3],
//  fill (1) :  UNSPECIFIED);
union TaggedControlLink {
	CARD32 u;
	struct {
		CARD16 tag  :  2;
		CARD16 data : 14;
		CARD16 fill;
	};
};

// ControlLink Type : PROCEDURE [link: ControlLink]  RETURNS [LinkType]
static inline LinkType ControlLinkType(ControlLink link) {
	TaggedControlLink cl = {link};
	return (LinkType)cl.tag;
}

// 9.1.1 Frame Control Links

// FrameLink : TYPE = LocalFrameHandle;
typedef LocalFrameHandle FrameLink;

// MakeFramelink: PROCEDURE [link : ControlLink] RETURNS [FrameLink]
static inline FrameLink MakeFrameLink(ControlLink link) {
	if (ControlLinkType(link) != LT_frame) ERROR();
	return (CARD16)link;
}

// 9.1.2 Indirect Control Links

// IndirectLink: TYPE = POINTER TO ControlLink;
typedef CARD16 IndirectLink;

// MakeIndirectLink: PROC[link: ControlLink] RETURNS[Indirectlink]
static inline IndirectLink MakeIndirectLink(ControlLink link) {
	if (ControlLinkType(link) != LT_indirect) ERROR();
	return (CARD16)link;
}

// 9.1.3 Procedure Descriptor
//ProcDesc: TYPE =  MACHINE DEPENDENT RECORD [
//  taggedGF(0) :UNSPECIFIED,
//  pc(1}: CARDINAL];
union ProcDesc {
	CARD32 u;
	struct {
		CARD16 taggedGF;
		CARD16 pc;
	};
};

// MakeProcDesc: PROC[link: ControlLink] RETURNS[ProcDesc]
static inline CARD32 MakeProcDesc(ControlLink link) {
	if (ControlLinkType(link) != LT_oldProcedure) ERROR();
	return link;
}

// 9.1.4 new Procedure Descriptor
// NewProcDesc: TYPE = MACHINE DEPENDENT RECORD[
//   taggedGFI(0): UNSPEC,
//   pc(1): CARDINAL];
union NewProcDesc {
	CARD32 u;
	struct {
		CARD16 taggedGFI;
		CARD16 pc;
	};
};

// MakeNewProcDesc: PROC[link: ControlLink] RETURNS [NewProcDesc]
static inline CARD32 MakeNewProcDesc(ControlLink link) {
	if (ControlLinkType(link) != LT_newProcedure) ERROR();
	return link;
}


// 9.2.1 Frame Allocation Vector
//AV: POINTER TO AllocationVector =  LOOPHOLE(mAV];
const POINTER AV  = mAV;  // POINTER TO AllocationVector

//AVItem: TYPE = MACHINE DEPENDENT RECORD [
//  data (0: 0 .. 13): [0 .. 377778],
//  tag (0: 14 .. 15) :  MACHINE DEPENDENT {frame, empty, indirect, unused}];
union AVItem {
	CARD16 u;
	struct {
		CARD16 tag  :  2;
		CARD16 data : 14;
	};
};
enum AVItemTag { AT_frame = 0, AT_empty = 1, AT_indirect = 2, AT_unused = 3 };


#define AV_OFFSET(n) (AV + OFFSET(AllocationVector, item[n]))


//FSindex: TYPE = [0 .. 256);
typedef CARD8 FSIndex;
const int FSIndex_SIZE = 256;

//AllocationVector: TYPE =  ARRAY FSlndex OF AVItem;
//typedef CARD16 AllocationVector[256];
struct AllocationVector {
	AVItem item[FSIndex_SIZE];
};

// AVFrame : PROC[avi : AVItem] RETURNS [LocalFrameHandle]
static inline LocalFrameHandle AVFrame(CARD16 u) {
	AVItem avi = {u};
	if (avi.tag != AT_frame) ERROR();
	return u;
}

// AVLink : PROC[avi : AVItem] RETURNS [POINTER TO AVItem]
static inline POINTER AVLink(CARD16 u) {
	AVItem avi = {u};
	if (avi.tag != AT_frame) ERROR();
	return u;
}

// 9.3 Control Transfer primitives
enum XferType { XT_return = 0, XT_call = 1, XT_localCall = 2, XT_port = 3, XT_xfer = 4, XT_trap = 5, XT_processSwitch = 6, XT_unused = 7};


// 9.4.5 Coroutine Transfers

// PortLink: TYPE = POINTER TO Port;
typedef POINTER PortLink;

// Port: TYPE = MACHINE DEPENDENT RECORD[
//   inport(0):  FrameLink,
//   unused(1):  UNSPECIFIED,
//   outport(2): ControlLink];
struct Port {
	FrameLink   inport;
	UNSPEC      unused;
	ControlLink outport;
};

#define PORT_OFFSET(p,m) (p + OFFSET(Port, m))


// 9.5.1 Trap Routines

// SD: POINTER TO SystemData = LOOPHOLE[mSD];
const POINTER SD  = mSD;

// SystemData: TYPE = ARRAY SDIndex OF ControlLink;
// SDIndex: TYPE = [0..256);
typedef CARD8 SDIndex;
//typedef ControlLink SystemData[256];
struct SystemData {
	ControlLink item[256];
};

#define SD_OFFSET(n) (SD + OFFSET(SystemData, item[n]))


// ETT: POINTER TO EscTrapTable = LOOPHOLE[mETT]
const POINTER ETT = mETT;

// EscTrapTable: TYPE = ARRAY BYTE OF ControlLink;
//typedef ControlLink EscTrapTable[256];
struct EscTrapTable {
	ControlLink item[256];
};

#define ETT_OFFSET(n) (ETT + OFFSET(EscTrapTable, item[n]))


// 9.5.3 Trap Handler

// StateHandler: TYPE = LONG POINTER TO StateVector
typedef LONG_POINTER StateHandle;

//StateWord: TYPE = MACHINE DEPENDENT RECORD[
//  breakByte(0:0..7), stkptr(0:8..15): BYTE];
union StateWord {
	CARD16 u;
	struct {
		CARD16 stkptr   : 8;
		CARD16 breakByte: 8;
	};
};

// StateVector: TYPE = MACHINE DEPENDENT RECORD [
//   stack  (0) : ARRAY [0.. StackDepth) OF UNSPECIFIED,
//   word  (14) : StateWord,
//   frame (15) : LocalFrameHandle,
//   data  (16) : BLOCK];
struct StateVector {
	UNSPEC           stack[StackDepth];
	CARD16           word;
	LocalFrameHandle frame;
	CARD16           data[2]; // 2 words for LONG UNSEPC
};

// TransferDescriptor: TYPE = MACHINE DEPENDENT RECORD[
//   src(0): ShortControlLink,
//   reserved(1): UNSPECIFIED = 0,
//   CoontrolLink(2): dst];
struct TransferDescriptor {
	ShortControlLink src;
	UNSPEC           reserved;
	ControlLink      dst;
};

// 10.1.1 Process Data Area
// ProcessDataArea is move to end of file
const CARD16 PsbNull = 0;

// PsbIndex: TYPE = [0.1024);
typedef CARD16 PsbIndex;

// StartPsb is moved to end of file


// 10.1.2 Process State Blocks
// ProcessStateBlock is moved to end of file

// Priority: TYPE = [0..7];
const int Priority_SIZE = 8;

//PsbLink: TYPE = MACHINE DEPENDENT RECORD[
//  priority(0:0..2): Priority,
//  next(0:3..12): PsbIndex,
//  failed(0:13..13): BOOLEAN,
//  permanent(0:14..14): BOOLEAN,
//  preempted(0:15..15): BOOLEAN];
union PsbLink {
	CARD16 u;
	struct {
		CARD16 preempted :  1;
		CARD16 permanent :  1;
		CARD16 failed    :  1;
		CARD16 next      : 10;
		CARD16 priority  :  3;
	};
};


//PsbFlags: TYPE = MACHINE DEPENDENT RECORD[
//  available(0:0..2): [0..7],
//  cleanup(0:3..12): PsbIndex,
//  reserved(0:13..13): BIT = 0,
//  waiting(0:14..14): BOOLEAN,
//  abort(0:15..15): BOOLEAN];
union PsbFlags {
	CARD16 u;
	struct {
		CARD16 abort     :  1;
		CARD16 waiting   :  1;
		CARD16 reserved  :  1;
		CARD16 cleanup   : 10;
		CARD16 available :  3;
	};
};


//// 10.1.3 Monitor Locks
//Monitor: TYPE = MACHINE DEPENDENT RECORD[
//  reserved(0:0..2): [0..7] = 0,
//  tail(0:3..12): PsbIndex,
//  available(0:13..14): [0..3],
//  locked(0:15..15): BOOLEAN];
union Monitor {
	CARD16 u;
	struct {
		CARD16 locked    :  1;
		CARD16 available :  2;
		CARD16 tail      : 10;
		CARD16 reserved  :  3;
	};
};

//// 10.1.4 Condition Variables
//Condition: TYPE = MACHIND DEPENDENT RECORD[
//  reserved(0:0..2): [0..7] = 0,
//  tail(0:3..12): PsbIndex,
//  available(0:13..13): BIT,
//  abortable(0:14..14): BOOLEAN,
//  wakeup(0:15..15): BOOLEAN];
union Condition {
	CARD16 u;
	struct {
		CARD16 wakeup    :  1;
		CARD16 abortable :  1;
		CARD16 available :  1;
		CARD16 tail      : 10;
		CARD16 reserved  :  3;
	};
};

//// 10.1.5 Process Queue
//QueueHandle: TYPE = LONG POINTER TO Queue;
typedef LONG_POINTER QueueHandle;
//Queue: TYPE = MACHINE DEPENDENT RECORD[
//  reserved1(0:0..2): [0..7] = 0,
//  tail(0:3..12): PsbIndex,
//  reserved2(0:13..15): [0..7]];
union Queue {
	CARD16 u;
	struct {
		CARD16 reserved2 :  3;
		CARD16 tail      : 10;
		CARD16 reserved1 :  3;
	};
};

// 10.4.2.2. State Vector Allocation
// StateAllocationTable: TYPE = ARRAY Priority OF POINTER TO StateVector
typedef POINTER PDA_POINTER;
typedef PDA_POINTER StateAllocationTable[Priority_SIZE];
//struct StateAllocationTable {
//	StateVector item[Priority_SIZE];
//};

// 10.4.3 Faults
// FaultVector: TYPE = ARRAY FaultIndex OF FaultQueue;
// FaultIndex: TYPE = [0..8);
// FaultQueue: TYPE = MACHINE DEPENDENT RECORD[
//   queue(0): Queue,
//   condition(1): Condition];
struct FaultQueue {
	CARD16 queue;
	CARD16 condition;
};
// ** NOTE** FaultQueue is used to calculate offset with OFFSET macro.
//           Don't defined with bit filed for condition and available

typedef CARD16 FaultIndex;
const int FaultIndex_SIZE = 8;
typedef FaultQueue FaultVector[FaultIndex_SIZE];
//struct FaultVector {
//	FaultQueue item[FaultIndex_SIZE];
//};

// 10.4.4. Interrupts

// InterruptVector: TYPE = ARRAY InterruptLevel OF InterruptItem;
// InterruptLvel: TYPE = [0..WordSize);
// InterruptItem: TYPE = MACHINE DPENEDENT RECORD[
//   condition(0): Condition,
//   available(1): UNSPECIFIED];
const int InterruptLevel_SIZE = WordSize;
struct InterruptItem {
		CARD16 condition;
		CARD16 available;
};
// ** NOTE** InterruptItem is used to calculate offset with OFFSET macro.
//           Don't defined with bit filed for condition and available

// typedef InterruptItem InterruptVector[InterruptLevel_SIZE];
struct InterruptVector {
	InterruptItem item[InterruptLevel_SIZE];
};


// 10.4.5 Timeouts

// Ticks: TYPE = CARDINAL
typedef CARDINAL Ticks;


// 10.1.2 Process State Blocks

// ProcessStateBlock: TYPE = MACHINE DPENDENT RECORD[
//   link(0): PsbLink,
//   flags(1): PsbFlags,
//   context(2): POINTER,
//   timeout(3): Ticks,
//   mds(4): CARDINAL,
//   available(5): UNSPECIFIED,
//   stickty(6): LONG UNSPECIFIED];
struct ProcessStateBlock {
	CARD16      link;
	CARD16      flags;
	POINTER     context;
	Ticks       timeout;
	CARDINAL	mds;
	UNSPEC      available;
	LONG_UNSPEC sticky;
};

// PsbHandle: TYPE = POINTER TO ProcessStateBlock;
typedef POINTER PsbHandle;

static inline PsbHandle Handle(PsbIndex index) {
	return index * SIZE(ProcessStateBlock);
}
static inline PsbIndex Index(PsbHandle handle) {
	return handle / SIZE(ProcessStateBlock);
}


// 10.1.1 Process Data Area

// PDA: LONG POINTER TO ProcessDataArea = LOOPHOLE[mPDA];
const LONG_POINTER PDA = mPDA;

// ProcessDataArea: TYPE = MACHINE DEPENDENT RECORD[
//   SELECT OVERLAID * FROM
//     header => [
//       ready: Queue,
//       count: CARDINAL,
//       unused: UNSPECIFIED,
//       available: ARRAY [0..5) OF UNSPECIFIED,
//       state: StateAllocationTable,
//       interrupt: InterruptVector,
//       fault: FaultVector],
//     blocks => [
//       block: ARRAY [0..0) OF ProcessStateBlock],
//     ENDCASE];
// PsbIndex: TYPE = [0..1024);
const int PsbIndex_SIZE = 1024;
struct ProcessDataArea {
	union {
		struct {
			Queue ready;
			CARD16 count;
			UNSPEC unused;
			CARD16 available[5];
			// StateAliocationTable: TYPE = ARRAY Priority OF POINTER TO StateVector;
			PDA_POINTER state[Priority_SIZE];
			//InterruptVector interrupt;
			InterruptItem interrupt[InterruptLevel_SIZE];
			//FaultVector fault;
			FaultQueue fault[FaultIndex_SIZE];
		};
		ProcessStateBlock block[0];
	};
};

// StartPsb: PsbIndex = (SIZE[ProcessDataArea] + SIZE[ProcessStateBlock] - 1) / SIZE[ProcessStateBlock];
// StartPsb <= (64 + 7) / 8 = 8
const CARD32 StartPsb = (SIZE(ProcessDataArea) + SIZE(ProcessStateBlock) - 1) / SIZE(ProcessStateBlock);


#endif
