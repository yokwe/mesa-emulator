package mh.majuro.mesa;

public final class Type {
	public static abstract class Field {
		public final int offset;
		public final int shift;
		public final int mask;
		
		protected Field(int offset, int startBit, int stopBit, int maxBit) {
			if (offset   < 0)        throw new MesaAssertionError();
			if (startBit < 0)        throw new MesaAssertionError();
			if (stopBit  < 0)        throw new MesaAssertionError();
			if (stopBit  < startBit) throw new MesaAssertionError();
			if (maxBit   < startBit) throw new MesaAssertionError();
			if (maxBit   < stopBit)  throw new MesaAssertionError();
			
			this.offset = offset;
			this.shift  = maxBit - stopBit;
			this.mask   = ((int)(1L << (stopBit - startBit + 1)) - 1) << shift;
		}
		public String toString() {
			return String.format("offset = %2d  shift = %2d  mask = %04X", offset, shift, mask);
		}
	}
	
	public static abstract class Field8 extends Field {
		protected static final int MAX_BIT = 7;
		
		public static Field8 getInstance(int offset, int startBit, int stopBit) {
			if (startBit == 0 && stopBit == MAX_BIT) return new Field8Whole(offset);
			return new Field8Generic(offset, startBit, stopBit);
		}
		public static Field8 getInstance(int offset) {
			return new Field8Whole(offset);
		}
		protected Field8(int offset, int startBit, int stopBit) {
			super(offset, startBit, stopBit, MAX_BIT);
		}
		
		public abstract int  getData(byte data);
		public abstract byte setData(byte data, int newValue);

	}
	public static final class Field8Generic extends Field8 {	
		private Field8Generic(int offset, int startBit, int stopBit) {
			super(offset, startBit, stopBit);
		}
		@Override
		public int getData(byte data) {
			return (data & mask) >>> shift;
		}
		@Override
		public byte setData(byte data, int newValue) {
			return (byte)((data & ~mask) | ((newValue << shift) & mask));
		}
	}
	public static final class Field8Whole extends Field8 {
		private Field8Whole(int offset) {
			super(offset, 0, MAX_BIT);
		}
		@Override
		public int getData(byte data) {
			return data & mask;
		}
		@Override
		public byte setData(byte data, int newValue) {
			return (byte)newValue;
		}
	}

	public static abstract class Field16 extends Field {
		protected static final int MAX_BIT = 15;
		
		public static Field16 getInstance(int offset, int startBit, int stopBit) {
			if (startBit == 0 && stopBit == MAX_BIT) return new Field16Whole(offset);
			return new Field16Generic(offset, startBit, stopBit);
		}
		public static Field16 getInstance(int offset) {
			return new Field16Whole(offset);
		}
		
		protected Field16(int offset, int startBit, int stopBit) {
			super(offset, startBit, stopBit, MAX_BIT);
		}
		
		public abstract int   getData(short data);
		public abstract short setData(short data, int newValue);
		public abstract int   get(int ptr);
		public abstract void  set(int ptr, int newValue);
	}
	public static final class Field16Generic extends Field16 {
		private Field16Generic(int offset, int startBit, int stopBit) {
			super(offset, startBit, stopBit);
		}
		
		@Override
		public int getData(short data) {
			return (data & mask) >>> shift;
		}
		@Override
		public short setData(short data, int newValue) {
			return (short)((data & ~mask) | ((newValue << shift) & mask));
		}
		@Override
		public int get(int ptr) {
			return getData(Memory.Read(ptr + offset));
		}
		@Override
		public void set(int ptr, int newValue) {
			int   memoryffset = Memory.OffsetWrite(ptr + offset);
			Memory.WriteMemory(memoryffset, setData(Memory.ReadMemory(memoryffset), newValue));
		}
	}
	public static final class Field16Whole extends Field16 {
		private Field16Whole(int offset) {
			super(offset, 0, MAX_BIT);
		}
		
		@Override
		public int getData(short data) {
			return data & mask;
		}
		@Override
		public short setData(short data, int newValue) {
			return (short)((data & ~mask) | ((newValue << shift) & mask));
		}
		@Override
		public int get(int ptr) {
			return getData(Memory.Read(ptr + offset));
		}
		@Override
		public void set(int ptr, int newValue) {
			int   memoryffset = Memory.OffsetWrite(ptr + offset);
			Memory.WriteMemory(memoryffset, setData(Memory.ReadMemory(memoryffset), newValue));
		}
	}

	public static abstract class Field32 extends Field {
		protected static final int MAX_BIT = 31;
		
		public static Field32 getInstance(int offset, int startBit, int stopBit) {
			if (startBit == 0 && stopBit == MAX_BIT) return new Field32Whole(offset);
			return new Field32Generic(offset, startBit, stopBit);
		}
		public static Field32 getInstance(int offset) {
			return new Field32Whole(offset);
		}
		
		protected Field32(int offset, int startBit, int stopBit) {
			super(offset, startBit, stopBit, MAX_BIT);
		}
		
		public abstract int  getData(int data);
		public abstract int  setData(int data, int newValue);
		public abstract int  get(int ptr);
		public abstract void set(int ptr, int newValue);
	}
	public static final class Field32Generic extends Field32 {
		private Field32Generic(int offset, int startBit, int stopBit) {
			super(offset, startBit, stopBit);
		}
		@Override
		public int getData(int data) {
			return (data & mask) >>> shift;
		}
		@Override
		public int get(int ptr) {
			return getData(Memory.ReadDbl(ptr + offset));
		}
		@Override
		public int  setData(int data, int newValue) {
			return (data & ~mask) | ((newValue << shift) & mask);
		}
		@Override
		public void set(int ptr, int newValue) {
			int   lowOffset  = Memory.OffsetWrite(ptr + 0);
			int   highOffset = Memory.OffsetWrite(ptr + 1);
			
			short lowHalf  = Memory.ReadMemory(lowOffset);
			short highHalf = Memory.ReadMemory(highOffset);
			int   data     = setData(Util.asInt(highHalf, lowHalf), newValue);
			
			Memory.WriteMemory(lowOffset,  Util.LowHalf(data));
			Memory.WriteMemory(highOffset, Util.HighHalf(data));
		}
	}
	public static final class Field32Whole extends Field32 {
		private Field32Whole(int offset) {
			super(offset, 0, MAX_BIT);
		}
		
		@Override
		public int getData(int data) {
			return data;
		}
		@Override
		public int setData(int data, int newValue) {
			return newValue;
		}
		public int get(int ptr) {
			return getData(Memory.ReadDbl(ptr + offset));
		}
		@Override
		public void set(int ptr, int newValue) {
			int   lowOffset  = Memory.OffsetWrite(ptr + 0);
			int   highOffset = Memory.OffsetWrite(ptr + 1);
			
			Memory.WriteMemory(lowOffset,  Util.LowHalf(newValue));
			Memory.WriteMemory(highOffset, Util.HighHalf(newValue));
		}
	}


	// 3.1.1 Virtual Memory Mapping
	//MapFlags: TYPE = MACHINE DEPENDENT RECORD (
	//  reserved (0:0..12) : UNSPEClFIED[0..17777B],
	//  protected (0:13..13) : BOOLEAN,
	//  dirty (0:14..14): BOOLEAN,
	//  referenced (0:15..15): BOOLEAN];
	public static final class MapFlags {
		// size
		public static final int SIZE = 1;
		// field
		public static final Field16 reserved   = Field16.getInstance(0,  0, 12);
		public static final Field16 protect    = Field16.getInstance(0, 13, 13);
		public static final Field16 dirty      = Field16.getInstance(0, 14, 14);
		public static final Field16 referenced = Field16.getInstance(0, 15, 15);
		//
		public static final short CLEAR      = 0;
		public static final short REFERENCED = 0x0001;
		public static final short DIRYT      = 0x0002;
		public static final short PROTECT    = 0x0004;
		public static final short VACANT     = DIRYT | PROTECT;
		public static final short REFERENCED_AND_DIRTY = REFERENCED | DIRYT;
		public static final boolean isReferenced(short value) {
			return (value & REFERENCED) == REFERENCED;
		}
		public static final boolean isReferencedAndDirty(short value) {
			return (value & REFERENCED_AND_DIRTY) == REFERENCED_AND_DIRTY;
		}
		public static final boolean isProtect(short value) {
			return (value & PROTECT) == PROTECT;
		}
		public static final boolean isVacant(short value) {
			return value == VACANT;
		}
	}
	
	// 3.1.4.3 Code Segments
	//CodeSegment: TYPE = MACHINE DEPENDENT RECORD [
	//  available (0) : ARRAY [0..4) OF UNSPECIFIED,
	//  code (4): BLOCK];
	public static final class CodeSegment {
		public static final int SIZE = 4;
	}

	// 3.2.2.2 Local and Global Frames
	//GlobalWord: TYPE = MACHINE DEPENDENT RECORD [
	//  gfi       (0:0..13): GFTIndex,
	//  trapxfers (0:14..14),
	//  codelinks (0:15..15): BOOLEAN];
	public static final class GlobalWord {
		// size
		public static final int SIZE = 1;
		// field
		public static final Field16 gfi       = Field16.getInstance(0,  0, 13);
		public static final Field16 trapxfers = Field16.getInstance(0, 14, 14);
		public static final Field16 codelinks = Field16.getInstance(0, 15, 15);
	}
	
	//GlobalFrameBase: TYPE = LONG POINTER TO GloalOverhead
	//GlobalOverhead : TYPE = MACHINE DEPENDENT RECORD (
	//  available (0): UNSPECIFIED,
	//  word      (1): GlobalWord,
	//  global    (2): GlobaiVariables];
	public static final class GlobalOverhead {
		// size
		public static final int SIZE = 2;
		// field
		public static final Field16 available = Field16.getInstance(0);
		public static final Field16 word      = Field16.getInstance(1);
		public static final Field16 global    = Field16.getInstance(2);
	}		
	
	// GlobalBase: PROC[frame GlobalFramehandle] RETURNS [GlobalFrameBase]
	public static int GlobalBase(int frame) {
		return frame - GlobalOverhead.SIZE;
	}

	//GFTIndex: TYPE = [0..16384);
	public static final int GFTIndex_SIZE = 16384;
	
	//GFTItem: TYPE = MACHINE DEPENDENT RECORD[
	//  globalFrame(0): GlobalFrameHandle,
	//  codebase(2): LONG POINTER TO CodeSegment];
	public static final class GFTItem {
		// size
		public static final int SIZE = 4;
		// field
		public static final Field32 globalFrame = Field32.getInstance(0);
		public static final Field32 codebase    = Field32.getInstance(2);
	}
	
	//LocalWord: TYPE = MACHINE DEPENDENT RECORD[
	//  available(0:0..7): BYTE,
	//  fsi(0:8..15): FSIndex];
	public static final class LocalWord {
		// size
		public static final int SIZE = 1;
		// field
		public static final Field16 available = Field16.getInstance(0,  0,  7);
		public static final Field16 fsi       = Field16.getInstance(0,  8, 15);
	}
	
	public static final class FSIndex {
		public static final int SIZE = 256;
	}

	//LocalOverhead : TYPE = MACHINE DEPENDENT RECORD [
	//  word (0):       LocalWord.
	//  returnlink(1) : ShortControlLink,
	//  globallink(2) : GFTHandle,
	//  pc(3):          CARDINAL,
	//  local(4):       LocaiVariables];
	public static final class LocalOverhead {
		// size
		public static final int SIZE = 4;
		// field
		public static final Field16 word       = Field16.getInstance(0);
		public static final Field16 returnlink = Field16.getInstance(1);
		public static final Field16 globallink = Field16.getInstance(2);
		public static final Field16 pc         = Field16.getInstance(3);
		public static final Field16 local      = Field16.getInstance(4);
	}		
	public static short LocalBase(short frame) {
		return (short)(frame - LocalOverhead.SIZE);
	}

	//FieldSpec: TYPE = MACHINE DEPENDENT RECORD [
	// pos  (0:0..3): NIBBLE,
	// size (0:4..7): NIBBLE];
	public static final class FieldSpec {
		// field
		public static final Field8 pos  = Field8.getInstance(0, 0, 3);
		public static final Field8 size = Field8.getInstance(0, 4, 7);
	}
	

	//FieldDesc: TYPE = MACHINE DEPENDENT RECORD [
	//  offset(0:0..7) : BYTE,
	//  field(0:8..15) : FieldSpec];
	public static final class FieldDesc {
		// size
		public static final int SIZE = 1;
		// field
		public static final Field16 offset = Field16.getInstance(0,  0,  7);
		public static final Field16 field  = Field16.getInstance(0,  8, 15);
	}
	
	//BitAddress: TYPE = MACHINE DEPENDENT RECORD[
	//  word(0): LONG POINTER,
	//  reserved(2:0..11): [0..777B] = 0,
	//  bit(2:12:15): [0..WordSize)];

	//BitBltFlags: TYPE = MACHINE DEPENDENT RECORD[
	//  direction(0:0..0): Direction,
	//  disjoint(0:1..1): BOOLEAN,
	//  disjointItems(0:2..2) BOOLEAN,
	//  gray(0:3..3) BOOLEAN,
	//  srcFunc(0:4..4) SrcFunc,
	//  dstFunc(0:5..6) DstFunc,
	//  reserved(0:7..15]: [0..777B] = 0];
	
	//BitBltArg: TYPE = MACHINE DPENEDENT RECORD[
	//  dst(0): BitAddress,
	//  dstBpl(3): INTEGER,
	//  src(4) BitAddress,
	//  srcBpl(7): INTEGER,
	//  width(8): CARDINAL,
	//  height(9): CARDINAL,
	//  flags(10): BitBltFlags,
	//  reserved(11): UNSPEC = 0];

	
	//LinkType: TYPE  =  {frame, procedure, indirect};
	public static final class LinkType {
		public static final int frame        = 0;
		public static final int oldProcedure = 1;
		public static final int indirect     = 2;
		public static final int newProcedure = 3;
	}
	
	//TaggedControlLink: TYPE =  MACHINE DEPENDENT RECORD [
	//  data (0:  0..13): [0 .. 377778];
	//  tag  (0: 14..15): [0 .. 3],
	//  fill (1) :  UNSPECIFIED);
	public static final class TaggedControlLink {
		// size
		public static final int SIZE = 2;
		
		// field
		public static final Field16 data = Field16.getInstance(0,  0, 13);
		public static final Field16 tag  = Field16.getInstance(0, 14, 15);
		public static final Field16 fill = Field16.getInstance(1);
	}


	//NewProcDesc: TYPE = MACHINE DEPENDENT RECORD[
	//  taggedGFI(0): UNSPEC,
	//  pc       (1): CARDINAL];
	public static final class NewProcDesc {
		// size
		public static final int SIZE = 2;
		// field
		public static final Field16 taggedGFI = Field16.getInstance(0);
		public static final Field16 pc        = Field16.getInstance(1);
	}

	//AVItem: TYPE = MACHINE DEPENDENT RECORD [
	//  data (0: 0 .. 13): [0 .. 377778],
	//  tag (0: 14 .. 15):  MACHINE DEPENDENT {frame, empty, indirect, unused}];
	public static final class AVItem {
		// size
		public static final int SIZE = 1;
		// enum
		public static final int frame    = 0;
		public static final int empty    = 1;
		public static final int indirect = 2;
		public static final int unused   = 3;
		// field
		public static final Field16 data = Field16.getInstance(0,  0, 13);
		public static final Field16 tag  = Field16.getInstance(0, 14, 15);
	}
	
	
	//From APilot/15.0.1/Pilot/Private/VMDataInternal.mesa
	public static final class VMDataInternal {
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
		public static final class Run {
			public static final int SIZE = 14;
			
			// field
			// only first 2 fields are used in VMFIND
			public static final Field32 intervalPage  = Field32.getInstance(0);
			public static final Field32 intervalCount = Field32.getInstance(2);
		}

		//countRunPad: CARDINAL = 1;
		public static final int countRunPad = 1;
		//PRun: TYPE = RunBase RELATIVE ORDERED POINTER [0..LAST[CARDINAL]] TO Run;
		//pRunFirst: PRun = FIRST[PRun] + countRunPad*SIZE[Run];  -- first real entry of table
		public static final int pRunFirst = 0 + countRunPad * Run.SIZE;;
	}

	//From APilot/15.3/Faces/Friends/ProcessorFaceExtras.mesa
	public static final class ProcessorFaceExtras {
		//MachineType: TYPE = MACHINE DEPENDENT {
		//  altoI (1), altoII (2), altoIIXM (3), dolphin (4), dorado (5), dandelion (6),
		//  dicentra (7), daybreak(8), daisy(9), kiku(10), daylight(11),
		//  tridlion(12), dahlia(13), (17B)};
		public static final int altoI     = 1;
		public static final int altoII    = 2;
		public static final int altoIIXM  = 3;
		public static final int dolphin   = 4;
		public static final int dorado    = 5;
		public static final int dandelion = 6;
		public static final int dicentra  = 7;
		public static final int daybreak  = 8;
		public static final int daisy     = 9;
		public static final int kiku      = 10;
		public static final int daylight  = 11;
		public static final int tridlion  = 12;
		public static final int dahlia    = 13;
		
		//VersionResult: TYPE = MACHINE DEPENDENT RECORD [
		//  machineType (0: 0..3): MachineType,
		//  majorVersion (0: 4..7): [0..17B],  -- incremented by incompatible changes
		//  unused (0: 8..13): [0..77B],
		//  floatingPoint (0: 14..14): BOOLEAN,
		//  cedar (0: 15..15): BOOLEAN,
		//  releaseDate (1): CARDINAL];  -- days since January 1, 1901
		public static final class VersionResult {
			public static final int SIZE = 2;
			
			public static final Field16 machineType   = Field16.getInstance(0,  0,  3);
			public static final Field16 majorVersion  = Field16.getInstance(0,  4,  7);
			public static final Field16 unused        = Field16.getInstance(0,  8, 13);
			public static final Field16 floatingPoint = Field16.getInstance(0, 14, 14);
			public static final Field16 cedar         = Field16.getInstance(0, 15, 15);
			public static final Field16 releaseDate   = Field16.getInstance(1);
		}
	}
}
