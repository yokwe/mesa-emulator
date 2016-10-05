package mh.majuro.test;

import mh.majuro.mesa.Memory;
import mh.majuro.mesa.MesaAssertionError;
import mh.majuro.mesa.Opcode;
import mh.majuro.mesa.Processor;
import mh.majuro.mesa.Type;
import mh.majuro.mesa.Util;

import org.junit.After;
import org.junit.Before;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class TestBase {
	protected static final Logger logger = LoggerFactory.getLogger(TestBase.class);
	
	protected static final int VM_BITS = 25;
	protected static final int RM_BITS = 24;

	private static final int FrameSizeMap[] = {
			   8,   12,   16,   20,   24,
			  28,   32,   40,   48,   56,
			  68,   80,   96,  112,  128,
			 148,  168,  192,  224,  252,
			 508,  764, 1020, 1276, 1532,
			1788, 2044, 2556, 3068, 3580, 4092
		};

	private static final int FrameWeightMap[] = {
			20, 26, 15, 16, 16,
			12,  8,  8,  5,  5,
			 7,  2,  2,  1,  1,
			 1,  1,  1,  1,  0,
			 0,  0,  0,  0,  0,
			 0,  0,  0,  0,  0, 0
		};

	private void initAV(int origin, int limit) {
		// fill with empty
		for(int i = 0; i < Type.FSIndex.SIZE; i++) {
			Memory.WriteMDS(Memory.mAV, i, (short)Type.AVItem.empty);
		}

		// build AllocationVector
		int p = origin;
		for(int fsi = 0; fsi < FrameSizeMap.length; fsi++) {
			int size   = FrameSizeMap[fsi];
			int weight = FrameWeightMap[fsi];
			if (weight == 0) continue;

			for(int j = 0; j < weight; j++) {
				// align (p mod 4) == 0
				p = ((p + 3) & ~0x03);

				// round up to next page boundary
				if (((p + 8) % Memory.PAGE_SIZE) < (p % Memory.PAGE_SIZE)) {
					p = ((p + (Memory.PAGE_SIZE - 1)) & ~0x00ff);
				}

				Memory.Write(Processor.MDS + p + 0, (short)fsi);
				Memory.Write(Processor.MDS + p + 1, (short)0);
				Memory.Write(Processor.MDS + p + 2, (short)0);
				Memory.Write(Processor.MDS + p + 3, (short)0);
				Memory.Write(Processor.MDS + p + 4, Memory.ReadMDS(Memory.mAV, fsi));
				Memory.WriteMDS(Memory.mAV, fsi, (short)(p + Type.LocalOverhead.SIZE));
				
				p = p + size;
			}
		}
		if (limit <= p) {
			logger.debug("origin = %04X\n", origin);
			logger.debug("limit  = %04X\n", limit);
			logger.debug("p      = %04X\n", p);
			throw new MesaAssertionError();
		}
	}
	private void initSD() {
		// fill with empty
		for(int i = 0; i < 256; i++) {
			int ptr = Memory.LengthenPointer(Memory.mSD) + Type.NewProcDesc.SIZE * i;
			Type.NewProcDesc.taggedGFI.set(ptr, Type.LinkType.newProcedure);
			Type.NewProcDesc.pc.set(ptr, 0);
		}

		for(int i = 0; i < 256; i++) {
			int ptr = Memory.LengthenPointer(Memory.mSD) + Type.NewProcDesc.SIZE * i;
			Type.NewProcDesc.taggedGFI.set(ptr, (GFI_SD | Type.LinkType.newProcedure));
			Type.NewProcDesc.pc.set(ptr, (pc_SD | i));
		}
	}
	private void initETT() {
		// fill with empty
		for(int i = 0; i < 256; i++) {
			int ptr = Memory.LengthenPointer(Memory.mETT) + Type.NewProcDesc.SIZE * i;
			Type.NewProcDesc.taggedGFI.set(ptr, Type.LinkType.newProcedure);
			Type.NewProcDesc.pc.set(ptr, 0);
			
			ptr += Type.NewProcDesc.SIZE;
		}
		for(int i = 0; i < 256; i++) {
			int ptr = Memory.LengthenPointer(Memory.mETT) + Type.NewProcDesc.SIZE * i;
			short pc = (short)(pc_ETT | i);

			Type.NewProcDesc.taggedGFI.set(ptr, (GFI_ETT | Type.LinkType.newProcedure));
			Type.NewProcDesc.pc.set(ptr, pc);
			
			Memory.Write(Processor.CB + (pc / 2), (short)0);
		}
	}
	
	protected static final int GFI = 1;

	protected static final int GFI_GF  =  4; // 1
	protected static final int GFI_SD  =  8; // 2
	protected static final int GFI_ETT = 12; // 3
	protected static final int GFI_EFC = 16; // 4
	
	protected static final short pc_SD  = 0x1000;
	protected static final short pc_ETT = 0x2000;

	private void initGFT() {
		// fill with empty
		for(int i = 0; i < Type.GFTIndex_SIZE; i++) {
			int ptr = Memory.mGFT + Type.GFTItem.SIZE * i;
			Type.GFTItem.globalFrame.set(ptr, 0);
			Type.GFTItem.codebase.set(ptr, 0);
		}

		{
			int ptr = Memory.mGFT + Type.GFTItem.SIZE * GFI_GF;
			Type.GFTItem.globalFrame.set(ptr, Processor.GF);
			Type.GFTItem.codebase.set(ptr, Processor.CB);

		}
		{
			int ptr = Memory.mGFT + Type.GFTItem.SIZE * GFI_SD;
			Type.GFTItem.globalFrame.set(ptr, Processor.GF);
			Type.GFTItem.codebase.set(ptr, Processor.CB);

		}
		{
			int ptr = Memory.mGFT + Type.GFTItem.SIZE * GFI_SD;
			Type.GFTItem.globalFrame.set(ptr, Processor.GF);
			Type.GFTItem.codebase.set(ptr, Processor.CB);

		}
		{
			int ptr = Memory.mGFT + Type.GFTItem.SIZE * GFI_ETT;
			Type.GFTItem.globalFrame.set(ptr, Processor.GF);
			Type.GFTItem.codebase.set(ptr, Processor.CB);

		}
		{
			int ptr = Memory.mGFT + Type.GFTItem.SIZE * GFI_EFC;
			Type.GFTItem.globalFrame.set(ptr, Processor.GF);
			Type.GFTItem.codebase.set(ptr, Processor.CB);

		}
	}

	@Before
	public void setUp() throws Exception {
		Memory.createInstance(VM_BITS, RM_BITS);
		Processor.initialize();
		Opcode.initialize();
		
		for(int i = 0; i < Processor.StackDepth; i++) {
			Processor.stack[i] = (short)(2000 + i);
		}
		
		Processor.CB  = 0x00030080;
		Processor.MDS = 0x00040000;
		Processor.GF  = 0x00050080 + Type.GlobalOverhead.SIZE;
		Processor.PC  = 0x20;
		Processor.GFI = GFI;
		
		for(int i = 0; i < Memory.PAGE_SIZE; i++) {
			Memory.Write(Processor.CB  + i, (short)(1000 | i));
			Memory.Write(Processor.GF  + i, (short)(2000 | i));
			Memory.Write(Processor.MDS + i, (short)(3000 | i));
			Memory.Write(Processor.MDS + Memory.PAGE_SIZE + i, (short)(4000 | i));
			// for LF = 0x12D4
			Memory.Write(Processor.MDS + 0x1200 + i, (short)(5000 | i));
		}
		
		initAV(0x0600, 0x1aff);
		initSD();
		initETT();
		initGFT();
		
		int fsi = 10;
		Processor.LF = Memory.ReadMDS(Memory.mAV, fsi);
		Memory.WriteMDS(Memory.mAV, fsi, Memory.ReadMDS(Processor.LF));
		//logger.debug(String.format("LF = %04X", Processor.LF));

		Processor.PSB = 1;
		
		//Memory.dumpAV();
		//Memory.dumpSD();
	}
	
	@After
	public void tearDown() throws Exception {
		Memory.clearInstance();
	}
	
	protected void writeOpcode(byte data1, byte data2, byte data3, byte data4) {
		Memory.Write(Processor.CB + (Processor.PC / 2) + 0, Util.asShort(data1, data2));
		Memory.Write(Processor.CB + (Processor.PC / 2) + 1, Util.asShort(data3, data4));
	}
	protected  void writeOpcode(byte data1) {
		writeOpcode(data1, (byte)0, (byte)0, (byte)0);
	}
	protected  void writeOpcode(byte data1, byte data2) {
		writeOpcode(data1, data2, (byte)0, (byte)0);
	}
	protected  void writeOpcode(byte data1, byte data2, byte data3) {
		writeOpcode(data1, data2, data3, (byte)0);
	}
}
