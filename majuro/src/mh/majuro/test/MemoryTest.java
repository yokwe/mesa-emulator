package mh.majuro.test;

import static org.junit.Assert.assertEquals;
import mh.majuro.mesa.Memory;
import mh.majuro.mesa.Processor;
import mh.majuro.mesa.Type;
import mh.majuro.mesa.Util;

import org.junit.Test;

public class MemoryTest extends TestBase {
	//
	// LongPointer access
	//
	@Test
	public void testRead() {
		int   longPointer = 0x123456;
		short value       = 0x1234;
		
		Memory.instance.memory[longPointer] = value;
		short newValue = Memory.Read(longPointer);
		
		assertEquals(value, newValue);
	}

	@Test
	public void testWrite() {
		int   longPointer = 0x123456;
		short value       = 0x1234;
		
		Memory.Write(longPointer, value);
		short newValue = Memory.instance.memory[longPointer];
		
		assertEquals(value, newValue);
	}

	@Test
	public void testReadDbl() {
		int longPointer = 0x123456;
		int value       = 0xfedcba98;
		
		Memory.instance.memory[longPointer + 0] = (short)(value & 0xffff);
		Memory.instance.memory[longPointer + 1] = (short)((value >> 16) & 0xffff);
		int newValue = Memory.ReadDbl(longPointer);
		
		assertEquals(value, newValue);
	}

	
	//
	// Code
	//
	@Test
	public void testReadCode() {
		Processor.CB = 0x123456;
		short offset = (short)0xcafe;
		short value  = (short)0x89ab;

		int longPointer = Processor.CB + Util.asInt(offset);
		Memory.instance.memory[longPointer] = value;
		short newValue = Memory.ReadCode(offset);

		assertEquals(value, newValue);
	}

	
	//
	// MDS access
	//
	@Test
	public void testLengthenPointer() {
		Processor.MDS = 0x123456;
		short pointer = (short)0xcafe;
		
		int value    = Processor.MDS + (pointer & 0xffff);
		int newValue = Memory.LengthenPointer(pointer);
		
		assertEquals(value, newValue);
	}
	
	@Test
	public void testReadMDS() {
		Processor.MDS = 0x123456;
		short pointer = (short)0xcafe;
		short value = 0x1234;
		
		int longPointer = Processor.MDS + (pointer & 0xffff);
		Memory.instance.memory[longPointer] = value;
		short newValue = Memory.ReadMDS(pointer);
		
		assertEquals(value, newValue);
	}

	@Test
	public void testWriteMDS() {
		Processor.MDS = 0x123456;
		short pointer = (short)0xcafe;
		short value = 0x1234;
		
		Memory.WriteMDS(pointer, value);
		int longPointer = Processor.MDS + (pointer & 0xffff);
		short newValue = Memory.instance.memory[longPointer];
		
		assertEquals(value, newValue);
	}

	@Test
	public void testReadDblMDS() {
		Processor.MDS = 0x123456;
		short pointer = (short)0xcafe;
		int value = 0x1234;
		
		int longPointer = Processor.MDS + (pointer & 0xffff);
		Memory.instance.memory[longPointer + 0] = (short)(value & 0xffff);
		Memory.instance.memory[longPointer + 1] = (short)((value >> 16) & 0xffff);
		int newValue = Memory.ReadDblMDS(pointer);
		
		assertEquals(value, newValue);
	}

	
	//
	// GetCodeByte
	//
	@Test
	public void testGetCodeByte_even() {
		Processor.CB = 0x123456;
		Processor.PC = (short)0x0010;
		short value  = (short)0xabcd;

		int longPointer = Processor.CB + (Util.asInt(Processor.PC) / 2);
		Memory.instance.memory[longPointer] = value;
		byte newValue = (byte)Memory.GetCodeByte();

		assertEquals((byte)Util.HighByte(value), newValue);
	}

	@Test
	public void testGetCodeByte_odd() {
		Processor.CB = 0x123456;
		Processor.PC = (short)0x0011;
		short value  = (short)0xabcd;

		int longPointer = Processor.CB + (Util.asInt(Processor.PC) / 2);
		Memory.instance.memory[longPointer] = value;
		byte newValue = (byte)Memory.GetCodeByte();

		assertEquals((byte)Util.LowByte(value), newValue);
	}

	
	//
	// GetCodeWord
	//
	@Test
	public void testGetCodeWord_notCross() {
		Processor.CB = 0x123456;
		Processor.PC = (short)0x0010;
		short value  = (short)0xabcd;

		int longPointer = Processor.CB + (Util.asInt(Processor.PC) / 2);
		Memory.instance.memory[longPointer] = value;
		short newValue = Memory.GetCodeWord();

		assertEquals(value, newValue);
	}

	@Test
	public void testGetCodeWord_cross() {
		Processor.CB = 0x123456;
		Processor.PC = (short)0x0011;
		short value  = (short)0xabcd;

		int longPointer = Processor.CB + (Util.asInt(Processor.PC) / 2);
		Memory.instance.memory[longPointer + 0] = (short)(value >> 8);
		Memory.instance.memory[longPointer + 1] = (short)(value << 8);
		short newValue = Memory.GetCodeWord();

		assertEquals(value, newValue);
	}

	//
	// FetchByte
	//
	@Test
	public void testFetchByte_int_even() {
		int   longPointer = 0x123456;
		int   offset      = 0x0010;
		short value       = (short)0xabcd;

		int p = longPointer + (offset / 2);
		Memory.instance.memory[p] = value;
		int newValue = Memory.FetchByte(longPointer, offset);

		assertEquals((value >> 8) & 0xff, newValue);
	}

	@Test
	public void testFetchByte_int_odd() {
		int longPointer = 0x123456;
		int offset      = 0x0011;
		int value       = 0xabcd;

		int p = longPointer + (offset / 2);
		Memory.instance.memory[p] = (short)value;
		int newValue = Memory.FetchByte(longPointer, offset);

		assertEquals(Util.LowByte(value), newValue);
	}


	@Test
	public void testFetchByte_short_even() {
		int   longPointer = 0x123456;
		short offset      = (short)0x8000;
		int value         = 0xabcd;

		int p = longPointer + (Util.asInt(offset) / 2);
		Memory.instance.memory[p] = (short)value;
		int newValue = Memory.FetchByte(longPointer, offset);

		assertEquals(Util.HighByte(value), newValue);
	}

	@Test
	public void testFetchByte_short_odd() {
		int   longPointer = 0x123456;
		short offset      = (short)0x8001;
		int   value       = 0xabcd;

		int p = longPointer + (Util.asInt(offset) / 2);
		Memory.instance.memory[p] = (short)value;
		int newValue = Memory.FetchByte(longPointer, offset);

		assertEquals(Util.LowByte(value), newValue);
	}

	//
	// StoreByte
	//
	@Test
	public void testStoreByte_int_even() {
		int  longPointer = 0x123456;
		int  offset      = 0x0010;
		byte value       = (byte)0xab;

		Memory.StoreByte(longPointer, offset, value);
		int p = longPointer + (offset / 2);
		short newValue = Memory.instance.memory[p];

		assertEquals(value, (byte)Util.HighByte(newValue));
	}

	@Test
	public void testStoreByte_int_odd() {
		int  longPointer = 0x123456;
		int  offset      = 0x0011;
		byte value       = (byte)0xab;

		Memory.StoreByte(longPointer, offset, value);
		int p = longPointer + (offset / 2);
		short newValue = Memory.instance.memory[p];

		assertEquals(value, (byte)Util.LowByte(newValue));
	}

	@Test
	public void testStoreByte_short_even() {
		int   longPointer = 0x123456;
		short offset      = (short)0x8010;
		int   value       = 0xab;

		Memory.StoreByte(longPointer, offset, (byte)value);
		int p = longPointer + (Util.asInt(offset) / 2);
		short newValue = Memory.instance.memory[p];

		assertEquals(value, Util.HighByte(newValue));
	}

	@Test
	public void testStoreByte_short_odd() {
		int   longPointer = 0x123456;
		short offset      = (short)0x8011;
		int   value       = 0xab;

		Memory.StoreByte(longPointer, offset, (byte)value);
		int p = longPointer + (Util.asInt(offset) / 2);
		short newValue = Memory.instance.memory[p];

		assertEquals(value, Util.LowByte(newValue));
	}

	
	@Test
	public void testReadField() {
		short source = (short)0xcafe;
		// spec8 = pos (4bits) and size (4bits)
		
		short d1 = Memory.ReadField(source, (byte)0x03);
		short d2 = Memory.ReadField(source, (byte)0x43);
		short d3 = Memory.ReadField(source, (byte)0x83);
		short d4 = Memory.ReadField(source, (byte)0xc3);
		//
		short d23 = Memory.ReadField(source, (byte)0x47);
		short d1234 = Memory.ReadField(source, (byte)0x0f);
		
		assertEquals((short)0xc, d1);
		assertEquals((short)0xa, d2);
		assertEquals((short)0xf, d3);
		assertEquals((short)0xe, d4);
		//
		assertEquals((short)0xaf, d23);
		assertEquals(source, d1234);
	}

	@Test
	public void testWriteField() {
		short dest = (short)0xcafe;
		
		short t1 = Memory.WriteField(dest, (byte)0x03, (short)1);
		short t2 = Memory.WriteField(dest, (byte)0x43, (short)2);
		short t3 = Memory.WriteField(dest, (byte)0x83, (short)3);
		short t4 = Memory.WriteField(dest, (byte)0xc3, (short)4);
		//
		short t23 = Memory.WriteField(dest, (byte)0x47, (short)0xac);
		short t1234 = Memory.WriteField(dest, (byte)0x0f, (short)0xacde);
		
		assertEquals((short)0x1afe, t1);
		assertEquals((short)0xc2fe, t2);
		assertEquals((short)0xca3e, t3);
		assertEquals((short)0xcaf4, t4);
		
		assertEquals((short)0xcace, t23);
		assertEquals((short)0xacde, t1234);
	}

	@Test
	public void testReadFlag() {
		int vp = 0x1234;
		
		for(short value = 0; value < 8; value++) {
			Memory.instance.flags[vp] = value;
			short newValue = Memory.ReadFlag(vp);
			
			assertEquals(value, newValue);
		}		
	}

	@Test
	public void testWriteFlag() {
		int vp = 0x1234;
		
		for(short value = 0; value < 8; value++) {
			Memory.WriteFlag(vp, value);
			
			short newValue = Memory.instance.flags[vp];
			
			assertEquals(value, newValue);
		}
	}
	
	@Test
	public void testReserveDisplayPage() {
		int displayPage = 10;
		
		Memory.ReserveDisplayPage(displayPage);
		int rmPage = Memory.MAX_RM_PAGE;
		for(int i = rmPage - displayPage; i < rmPage; i++) {
			assertEquals(Memory.instance.flags[i], Type.MapFlags.VACANT);
			assertEquals(Memory.instance.maps[i], 0);
		}
	}
	
	@Test
	public void testReadMemory() {
		int longPointer = 0x123456;
		short value     = (short)0xabcd;

		Memory.instance.memory[longPointer] = value;
		short newValue = Memory.ReadMemory(longPointer);
		
		assertEquals(value, newValue);
	}
	
	@Test
	public void testWriteMemory() {
		int longPointer = 0x123456;
		short value     = (short)0xabcd;

		Memory.WriteMemory(longPointer, value);
		short newValue = Memory.instance.memory[longPointer];
		
		assertEquals(value, newValue);
	}
	
	@Test
	public void testOffsetRead() {
		int value = 0x123456;

		int newValue = Memory.OffsetRead(value);
		
		assertEquals(value, newValue);
	}
	
	@Test
	public void testOffsetWrite() {
		int value = 0x123456;

		int newValue = Memory.OffsetWrite(value);
		
		assertEquals(value, newValue);
	}
}
