package mh.majuro.mesa;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public final class Memory {
	protected static final Logger logger = LoggerFactory.getLogger(Memory.class);
	
	public static final int mPDA = 0x00010000; // 0200000
	public static final int mGFT = 0x00020000; // 0400000

	public static final short mAV  = 0x0100; //  0400
	public static final short mSD  = 0x0200; // 01000
	public static final short mETT = 0x0400; // 02000

	public static final int IO_REGION_PAGE = 0x80;
	
	public static final int PAGE_SIZE_BITS   = 8;
	public static final int PAGE_SIZE        = 1 << PAGE_SIZE_BITS;
	public static final int PAGE_OFFSET_MASK = PAGE_SIZE - 1;
	
	public static final int WORD_SIZE_BITS = 4;
	public static final int WORD_SIZE      = 1 << WORD_SIZE_BITS;

	public static final int MAX_VM_BITS = 30;
	public static final int MAX_RM_BITS = 24;
	public static final int MAX_RM_PAGE = 4086 * WORD_SIZE;
	
	// mpas store mapping from virtual memory page number to real page number
	public short maps[];
	// flags sore flag of virtual memory;
	public short flags[];
	
	// memory store all real memory
	public short memory[];
	
	private int vmBits = 0;
	private int rmBits = 0;
	private int vmPage = 0;
	private int rmPage = 0;

	private Memory(int vmBits_, int rmBits_) {
		vmBits = vmBits_;
		rmBits = rmBits_;
		
		if (MAX_VM_BITS < vmBits) throw new MesaAssertionError();
		if (MAX_RM_BITS < rmBits) throw new MesaAssertionError();
		
		int vmSize = 1 << vmBits;
		int rmSize = 1 << rmBits;
		
		vmPage = vmSize / PAGE_SIZE;
		rmPage = rmSize / PAGE_SIZE;
		if (MAX_RM_PAGE < rmPage) rmPage = MAX_RM_PAGE;
		
		maps   = new short[vmPage];
		flags  = new short[vmPage];
		memory = new short[rmPage * PAGE_SIZE];
		
		// initialize mapFlags
		// map all real page to virtual page
		short rp = 0;
		// map real page page [ioRegionPage..256)
		for (int i = IO_REGION_PAGE; i < 256; i++) {
			maps [i] = rp++;
			flags[i] = Type.MapFlags.CLEAR;
		}
		// then map real page page [0..ioRegionPage)
		for (int i = 0; i < IO_REGION_PAGE; i++) {
			maps [i] = rp++;
			flags[i] = Type.MapFlags.CLEAR;
		}
		// then map real page [256..rmPage)
		for (int i = 256; i < rmPage; i++) {
			maps [i] = rp++;
			flags[i] = Type.MapFlags.CLEAR;
		}
		// then set remaining virtual page with vacant [rmPage..vmPage)
		for (int i = rmSize; i < vmPage; i++) {
			maps [i] = 0;
			flags[i] = Type.MapFlags.VACANT;
		}
	}
	
	private int displayPage = 0;
	private void reserveDisplayPage(int displayPage_) {
		if (displayPage_ <= 0) throw new MesaAssertionError();
		if (displayPage != 0) throw new MesaAssertionError();
		
		displayPage = displayPage_;
		// unmap real page from last real page for display page
		for(int i = rmPage - displayPage; i < rmPage; i++) {
			maps [i] = 0;
			flags[i] = Type.MapFlags.VACANT;
		}
	}
	
	private int getPage(int longPointer) {
		return longPointer >>> PAGE_SIZE_BITS;
	}
	private int getMemoryOffset(int page, int longPointer) {
		return (page << PAGE_SIZE_BITS) | (longPointer & PAGE_OFFSET_MASK);
	}
	
	private int offsetRead(int longPointer) {
		if (longPointer < 0) throw new MesaAssertionError();
		int vp = getPage(longPointer);
		short flag = flags[vp];
		// If flag is vacant flag, call PageFautl
		if (Type.MapFlags.isVacant(flag)) Processor.PageFault(longPointer);
		// Do mapFlag maintenance
		if (!Type.MapFlags.isReferenced(flag)) {
			flags[vp] = (short)(flag | Type.MapFlags.REFERENCED);
		}
		int page = maps[vp] & 0xffff;
		// TODO Is there case that access to Page 0 memory?
		if (page == 0) throw new MesaAssertionError();
		return getMemoryOffset(page, longPointer);
	}
	private short readMemory(int offset) {
		return memory[offset];
	}
	private short read(int longPointer) {
		return readMemory(offsetRead(longPointer));
	}
	private int offsetWrite(int longPointer) {
		if (longPointer < 0) throw new MesaAssertionError();
		int vp = getPage(longPointer);
		short flag = flags[vp];
		// If flag is vacant flag, call PageFautl
		if (Type.MapFlags.isVacant(flag)) Processor.PageFault(longPointer);
		// If flag is protec flag, call WriteProtectFautl
		if (Type.MapFlags.isProtect(flag)) Processor.WriteProtectFault(longPointer);
		// Do mapFlag maintenance
		if (!Type.MapFlags.isReferencedAndDirty(flag)) {
			flags[vp] = (short)(flag | Type.MapFlags.REFERENCED_AND_DIRTY);
		}
		
		int page = maps[vp] & 0xffff;
		// TODO Is there case that access to Page 0 memory?
		if (page == 0) throw new MesaAssertionError();
		return getMemoryOffset(page, longPointer);
	}
	private void writeMemory(int offset, short newValue) {
		memory[offset] = newValue;
	}
	private void write(int longPointer, short newValue) {
		writeMemory(offsetWrite(longPointer), newValue);
	}
	
	private short readFlag(int vp) {
		return flags[vp];
	}
	private void writeFlag(int vp, short newValue) {
		// allow only right most 3 bits
		if ((newValue & ~7) != 0) throw new MesaAssertionError();
		flags[vp] = newValue;
	}
	
	private int readMap(int vp) {
		return maps[vp] & 0xffff;
	}
	private void writeMap(int vp, int newValue) {
		if (Util.HighHalfAsInt(newValue) != 0) throw new MesaAssertionError();
		maps[vp] = (short)newValue;
	}
	//
	// Public methods
	//
	
	public static Memory instance = null;
	public static void createInstance(int vmBits, int rmBits) {
		if (instance == null) {
			instance = new Memory(vmBits, rmBits);
		} else {
			throw new MesaAssertionError();
		}
	}
	public static void clearInstance() {
		instance = null;
	}
	public static int GetVMPage() {
		return instance.vmPage;
	}
	
	// LongPointer access
	public static short Read(int longPointer) {
		return instance.read(longPointer);
	}
	public static void Write(int longPointer, short newValue) {
		instance.write(longPointer, newValue);
	}
	public static int ReadDbl(int longPointer) {
		short lowHalf  = Read(longPointer + 0);
		short highHalf = Read(longPointer + 1);
		return Util.asInt(highHalf, lowHalf);
	}
	
	// Raw Access
	public static short ReadMemory(int offset) {
		return instance.readMemory(offset);
	}
	public static void WriteMemory(int offset, short newValue) {
		instance.writeMemory(offset, newValue);
	}
	public static int OffsetRead(int longPointer) {
		return instance.offsetRead(longPointer);
	}
	public static int OffsetWrite(int longPointer) {
		return instance.offsetWrite(longPointer);
	}

	// Code
	public static short ReadCode(short offset) {
		return Read(Processor.CB + Util.asInt(offset));
	}
	public static short ReadCode(int offset) {
		return Read(Processor.CB + offset);
	}
	
	// MDS access
	public static int LengthenPointer(short pointer) {
		return Processor.MDS + Util.asInt(pointer);
	}
	public static short ReadMDS(short pointer) {
		return Read(LengthenPointer(pointer));
	}
	public static short ReadMDS(short pointer, int offset) {
		return Read(LengthenPointer(pointer) + offset);
	}
	public static void WriteMDS(short pointer, short newValue) {
		Write(LengthenPointer(pointer), newValue);
	}
	public static void WriteMDS(short pointer, int offset, short newValue) {
		Write(LengthenPointer(pointer) + offset, newValue);
	}
	public static int ReadDblMDS(short pointer) {
		return ReadDbl(LengthenPointer(pointer));
	}
	public static int ReadDblMDS(short pointer, int offset) {
		return ReadDbl(LengthenPointer(pointer) + offset);
	}

	
	// GetCodeByte returns [0..256)
	public static int GetCodeByte() {
		boolean even = (Processor.PC & 1) == 0;
		short word = ReadCode((short)(Processor.PC / 2));
		Processor.PC++;

		return even ? Util.HighByte(word) : Util.LowByte(word);
	}
	
	// GetCodeWord returns short
	public static short GetCodeWord() {
		int left  = GetCodeByte();
		int right = GetCodeByte();

		return Util.asShort(left, right);
	}
	
	// String
	public static int FetchByte(int longPointer, short offset) {
		return FetchByte(longPointer, Util.asInt(offset));
	}
	public static int FetchByte(int longPointer, int offset) {
		longPointer += offset / 2;
		short word = Read(longPointer);
		return ((offset % 2) == 0) ? Util.HighByte(word) : Util.LowByte(word);
	}
	public static void StoreByte(int longPointer, short offset, byte data) {
		StoreByte(longPointer, Util.asInt(offset), data);
	}
	public static void StoreByte(int longPointer, int offset, byte data) {
		longPointer += offset / 2;
		short word = Read(longPointer);
		if ((offset % 2) == 0) {
			// replace left = high
			word = Util.asShort(data, word);
		} else {
			// replace right = low
			word = Util.asShort((word >>> 8), data);
		}
		Write(longPointer, word);
	}
	
	// Field
	private static short Field_MaskTable(int n) {
		return (short)((1 << (n + 1)) - 1);
	}
	public static short ReadField(short source, byte spec8) {
		int pos  = Util.LeftNibble(spec8);
		int size = Util.RightNibble(spec8);
		
		if (WORD_SIZE < (pos + size + 1)) throw new MesaAssertionError();
		int shift = WORD_SIZE - (pos + size + 1);
		return (short)((source >> shift) & Field_MaskTable(size));
	}
	public static short WriteField(short dest, byte spec8, short data) {
		int pos  = Util.LeftNibble(spec8);
		int size = Util.RightNibble(spec8);

		if (WORD_SIZE < (pos + size + 1)) throw new MesaAssertionError();
		int shift  = WORD_SIZE - (pos + size + 1);
		short mask = (short)(Field_MaskTable(size) << shift);
		return (short)((dest & ~mask) | ((data << shift) & mask));
	}
	
	// Misc access
	public static short ReadFlag(int vp) {
		return instance.readFlag(vp);
	}
	public static void WriteFlag(int vp, short newValue) {
		instance.writeFlag(vp, newValue);
	}
	
	// returns real page number
	public static int ReadMap(int vp) {
		return instance.readMap(vp);
	}
	// newValue is real page number for longPointer
	public static void WriteMap(int vp, int newValue) {
		instance.writeMap(vp, newValue);
	}

	public static void ReserveDisplayPage(int displayPage) {
		instance.reserveDisplayPage(displayPage);
	}
	
	
    // TODO Below this line, method has NO TEST CASE
	
	// FetchLink
	public static int FetchLink(int offset) {
		short word = (short)Type.GlobalOverhead.word.get(Type.GlobalBase(Processor.GF));
		int ptr = (Type.GlobalWord.codelinks.getData(word) != 0) ? Processor.CB : Type.GlobalBase(Processor.GF);	
		return Memory.ReadDbl(ptr - (offset + 1) * 2);
	}
	
	public static void dumpAV() {
		logger.debug("dumpAV START");

		for(int i = 0; i <= 31; i++) {
			short data = Memory.ReadMDS(mAV, i);
			int tag  = Type.AVItem.tag.getData(data);
			
			if (data == 0) continue;
			if (tag != Type.AVItem.frame) continue;
			
			logger.info(String.format("AV %3d %04X %d", i, data, tag));
		}
		logger.debug("dumpAV STOP");
	}
	public static void dumpSD() {
		logger.debug("dumpSD START");
		int ptr = Memory.LengthenPointer(mSD);
		
		for(int i = 0; i < 2; i++) {
			int data32 = Memory.ReadDbl(ptr);
			int taggedGFI = Type.NewProcDesc.taggedGFI.get(ptr);
			int pc        = Type.NewProcDesc.pc.get(ptr);
			
			logger.info(String.format("SD  %2d  taggedGFI %04X  pc %04X  data32 %08X", i, taggedGFI, pc, data32));
			ptr += Type.NewProcDesc.SIZE;
		}
		logger.debug("dumpSD STOP");
	}
}
