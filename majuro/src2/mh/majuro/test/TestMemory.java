package mh.majuro.test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotEquals;

import org.junit.Test;

import mh.majuro.mesa.CodeCache;
import mh.majuro.mesa.Memory;

public class TestMemory extends TestBase {
	//
	// LongPointer access
	//
	@Test
	public void testFetch() {
		int longPointer = 0x123456;
		int value       = 65432;
		
		short[] page       = Memory.rawPage(longPointer);
		
		fill(page, 0);
		
		// Before
		{
			int actual = Memory.fetch(longPointer);
			assertNotEquals(value, actual);
		}
		
		Memory.store(longPointer, value);
		
		// After
		{
			int pageOffset = longPointer & Memory.PAGE_OFFSET_MASK;
			int actual= page[pageOffset] & 0xFFFF;
			assertEquals(value, actual);
		}
		{
			int actual = Memory.fetch(longPointer);
			assertEquals(value, actual);
		}
	}
	
	@Test
	public void testStore() {
		int longPointer = 0x123456;
		int value       = 65432;
		
		short[] page       = Memory.rawPage(longPointer);
		
		fill(page, 0);
		
		// Before
		{
			int actual = Memory.fetch(longPointer);
			assertNotEquals(value, actual);
		}

		Memory.store(longPointer, value);
		
		// After
		{
			int pageOffset = longPointer & Memory.PAGE_OFFSET_MASK;
			int actual= page[pageOffset] & 0xFFFF;
			assertEquals(value, actual);
		}
		{
			int actual = Memory.fetch(longPointer);
			assertEquals(value, actual);
		}
	}
	
	@Test
	public void testReadDbl() {
		int longPointer = 0x5678;
		int value       = (int)9876543210L;
		
		short[] page = Memory.rawPage(longPointer);
		
		fill(page, 0);
		
		// Before
		{
			int actual = Memory.readDbl(longPointer);
			assertNotEquals(value, actual);
		}
		
		Memory.store(longPointer + 0, value);
		Memory.store(longPointer + 1, value >> 16);
		
		// After
		{
			int pageOffset = longPointer & Memory.PAGE_OFFSET_MASK;
			assertEquals(value          & 0xFFFF, page[pageOffset + 0] & 0xFFFF); // low
			assertEquals((value >>> 16) & 0xFFFF, page[pageOffset + 1] & 0xFFFF); // high
		}
		{
			int actual = Memory.readDbl(longPointer);
			assertEquals(value, actual);
		}
	}

	@Test
	public void testReadDbl2() {
		int longPointer = 0x11FF;
		int value       = (int)9876543210L;
		
		{
			short[] page = Memory.rawPage(longPointer);
			fill(page, 0);
		}
		{
			short[] page = Memory.rawPage(longPointer + 1);
			fill(page, 0);
		}
		
		// Before
		{
			int actual = Memory.readDbl(longPointer);
			assertNotEquals(value, actual);
		}
		
		Memory.store(longPointer + 0, value);
		Memory.store(longPointer + 1, value >> 16);
		
		// After
		{
			int     pageOffset = longPointer & Memory.PAGE_OFFSET_MASK;
			short[] page       = Memory.rawPage(longPointer);
			assertEquals(value          & 0xFFFF, page[pageOffset] & 0xFFFF); // low
		}
		{
			int     pageOffset = (longPointer + 1) & Memory.PAGE_OFFSET_MASK;
			short[] page       = Memory.rawPage(longPointer + 1);
			assertEquals((value >>> 16) & 0xFFFF, page[pageOffset] & 0xFFFF); // high
		}
		{
			int actual = Memory.readDbl(longPointer);
			assertEquals(value, actual);
		}
	}

	@Test
	public void testMDS() {
		int value       = 0x8_0000;

		// Before
		{
			int actual = Memory.getMDS();
			assertNotEquals(value, actual);
		}
		
		Memory.setMDS(value);
		
		// After
		{
			int actual = Memory.getMDS();
			assertEquals(value, actual);
		}
	}

	@Test
	public void testLengthenPointer() {
		int value       = 0x1234;

		{
			int actual = Memory.lengthenPointer(value);
			int expect = Memory.getMDS() + value;
			
			assertEquals(expect, actual);
		}
	}
	
	@Test
	public void testFetchMDS() {
		int pointer = 0x5678;
		int value   = 65432;
		
		int     longPointer = Memory.lengthenPointer(pointer);
		short[] page        = Memory.rawPage(longPointer);
		
		fill(page, 0);
		
		// Before
		{
			int actual = Memory.fetchMDS(pointer);
			assertNotEquals(value, actual);
		}
		
		Memory.storeMDS(pointer, value);
		
		// After
		{
			int pageOffset = longPointer & Memory.PAGE_OFFSET_MASK;
			int actual= page[pageOffset] & 0xFFFF;
			assertEquals(value, actual);
		}
		{
			int actual = Memory.fetchMDS(pointer);
			assertEquals(value, actual);
		}
	}
	
	@Test
	public void testStoreMDS() {
		int pointer = 0x5678;
		int value   = 65432;
		
		int     longPointer = Memory.lengthenPointer(pointer);
		short[] page        = Memory.rawPage(longPointer);
		
		fill(page, 0);
		
		// Before
		{
			int actual = Memory.fetchMDS(pointer);
			assertNotEquals(value, actual);
		}

		Memory.storeMDS(pointer, value);
		
		// After
		{
			int pageOffset = longPointer & Memory.PAGE_OFFSET_MASK;
			int actual= page[pageOffset] & 0xFFFF;
			assertEquals(value, actual);
		}
		{
			int actual = Memory.fetchMDS(pointer);
			assertEquals(value, actual);
		}
	}
	
	@Test
	public void testReadDblMDS() {
		int pointer = 0x5678;
		int value   = (int)9876543210L;
		
		int     longPointer = Memory.lengthenPointer(pointer);
		short[] page        = Memory.rawPage(longPointer);
		
		fill(page, 0);
		
		// Before
		{
			int actual = Memory.readDbl(longPointer);
			assertNotEquals(value, actual);
		}
		
		Memory.store(longPointer + 0, value);
		Memory.store(longPointer + 1, value >> 16);
		
		// After
		{
			int pageOffset = longPointer & Memory.PAGE_OFFSET_MASK;
			assertEquals(value          & 0xFFFF, page[pageOffset + 0] & 0xFFFF); // low
			assertEquals((value >>> 16) & 0xFFFF, page[pageOffset + 1] & 0xFFFF); // high
		}
		{
			int actual = Memory.readDblMDS(pointer);
			assertEquals(value, actual);
		}
	}
	@Test
	public void testReadDblMDS2() {
		int pointer = 0x11FF;
		int value   = (int)9876543210L;
		
		int     longPointer = Memory.lengthenPointer(pointer);
		{
			short[] page = Memory.rawPage(longPointer);
			fill(page, 0);
		}
		{
			short[] page = Memory.rawPage(longPointer + 1);
			fill(page, 0);
		}
		
		// Before
		{
			int actual = Memory.readDbl(longPointer);
			assertNotEquals(value, actual);
		}
		
		Memory.store(longPointer + 0, value);
		Memory.store(longPointer + 1, value >> 16);
		
		// After
		{
			int     pageOffset = longPointer & Memory.PAGE_OFFSET_MASK;
			short[] page       = Memory.rawPage(longPointer);
			assertEquals(value          & 0xFFFF, page[pageOffset] & 0xFFFF); // low
		}
		{
			int     pageOffset = (longPointer + 1) & Memory.PAGE_OFFSET_MASK;
			short[] page       = Memory.rawPage(longPointer + 1);
			assertEquals((value >>> 16) & 0xFFFF, page[pageOffset] & 0xFFFF); // high
		}
		{
			int actual = Memory.readDbl(longPointer);
			assertEquals(value, actual);
		}
	}

	@Test
	public void testReadCode() {
		int offset = 0x8765;
		int value  = 65432;
		
		int     longPointer = CodeCache.getCB() + offset;
		short[] page        = Memory.rawPage(longPointer);
		fill(page, 0);
		
		// Before
		{
			int actual = Memory.fetch(longPointer);
			assertNotEquals(value, actual);
		}
		
		Memory.store(longPointer, value);
		
		// After
		{
			int pageOffset = longPointer & Memory.PAGE_OFFSET_MASK;
			int actual= page[pageOffset] & 0xFFFF;
			assertEquals(value, actual);
		}
		{
			int actual = Memory.readCode(offset);
			assertEquals(value, actual);
		}
	}

	@Test
	public void testFetchByte() {
		int longPointer = 0x123456;
		int value       = 0xFF;
		int offset      = 10;
		
		short[] page = Memory.rawPage(longPointer + (offset >>> 1));
		fill(page, 0);
		
		// Before
		{
			int actual = Memory.fetchByte(longPointer, offset);
			assertNotEquals(value, actual);
		}
		
		Memory.store(longPointer + (offset >>> 1), value << 8);
		
		// After
		{
			int actual = Memory.fetchByte(longPointer, offset);
			assertEquals(value, actual);
		}
	}

	@Test
	public void testFetchByte2() {
		int longPointer = 0x123456;
		int value       = 0xFF;
		int offset      = 11;
		
		short[] page = Memory.rawPage(longPointer + (offset >>> 1));
		fill(page, 0);
		
		// Before
		{
			int actual = Memory.fetchByte(longPointer, offset);
			assertNotEquals(value, actual);
		}
		
		Memory.store(longPointer + (offset >>> 1), value);
		
		// After
		{
			int actual = Memory.fetchByte(longPointer, offset);
			assertEquals(value, actual);
		}
	}

	@Test
	public void testFetchWord() {
		int longPointer = 0x10000;
		int value       = 0xFEDC;
		int offset      = 8;
		
		short[] page       = Memory.rawPage(longPointer);
		fill(page, 0);
		
		// Before
		{
			int actual = Memory.fetchWord(longPointer, offset);
			assertNotEquals(value, actual);
		}
		
		int va = longPointer + (offset >>> 1);
		int vo = va & Memory.PAGE_OFFSET_MASK;
		page[vo + 0] = (short)(value);

		// After
		{
			int actual = Memory.fetchWord(longPointer, offset);
			assertEquals(value, actual);
		}
	}

	@Test
	public void testFetchWord2() {
		int longPointer = 0x10000;
		int value       = 0xFEDC;
		int offset      = 9;
		
		short[] page       = Memory.rawPage(longPointer);
		fill(page, 0);
		
		// Before
		{
			int actual = Memory.fetchWord(longPointer, offset);
			assertNotEquals(value, actual);
		}
		
		int va = longPointer + (offset >>> 1);
		int vo = va & Memory.PAGE_OFFSET_MASK;
		page[vo + 0] = (short)((value >> 8) & 0x00FF);
		page[vo + 1] = (short)((value << 8) & 0xFF00);
		
		// After
		{
			int actual = Memory.fetchWord(longPointer, offset);
			assertEquals(value, actual);
		}
	}

	@Test
	public void testFetchWord3() {
		int longPointer = 0x10FF;
		int value       = 0xFEDC;
		int offset      = 1;
		
		{
			short[] page       = Memory.rawPage(longPointer);
			fill(page, 0);
		}
		{
			short[] page       = Memory.rawPage(longPointer + 1);
			fill(page, 0);
		}
		
		// Before
		{
			int actual = Memory.fetchWord(longPointer, offset);
			assertNotEquals(value, actual);
		}
		
		{
			int va = longPointer + (offset >>> 1);
			int vo = va & Memory.PAGE_OFFSET_MASK;
			short[] page = Memory.rawPage(va);
			page[vo] = (short)((value >> 8) & 0x00FF);
		}
		{
			int va = longPointer + (offset >>> 1) + 1;
			int vo = va & Memory.PAGE_OFFSET_MASK;
			short[] page = Memory.rawPage(va);
			page[vo] = (short)((value << 8) & 0xFF00);	
		}
		
		// After
		{
			int actual = Memory.fetchWord(longPointer, offset);
			assertEquals(value, actual);
		}
	}

	@Test
	public void testStoreByte() {
		int longPointer = 0x123456;
		int value       = 0xFF;
		int offset      = 10;
		
		{
			short[] page = Memory.rawPage(longPointer + (offset >>> 1));
			fill(page, 0);
		}
		
		// Before
		{
			int actual = Memory.fetchByte(longPointer, offset);
			assertNotEquals(value, actual);
		}
		
		Memory.storeByte(longPointer, offset, value);
		
		// After
		{
			int actual = Memory.fetchByte(longPointer, offset);
			assertEquals(value, actual);
		}
		{
			int word = Memory.fetch(longPointer + (offset >>> 1));
			assertEquals(value, (word >> 8) & 0x00FF);
		}
	}

	@Test
	public void testStoreByte2() {
		int longPointer = 0x123456;
		int value       = 0xFF;
		int offset      = 11;
		
		{
			short[] page = Memory.rawPage(longPointer + (offset >>> 1));
			fill(page, 0);
		}
		
		// Before
		{
			int actual = Memory.fetchByte(longPointer, offset);
			assertNotEquals(value, actual);
		}
		
		Memory.storeByte(longPointer, offset, value);
		
		// After
		{
			int actual = Memory.fetchByte(longPointer, offset);
			assertEquals(value, actual);
		}
		{
			int word = Memory.fetch(longPointer + (offset >>> 1));
			assertEquals(value, word & 0x00FF);
		}
	}
	
	@Test
	public void testFieldMask() {
		assertEquals(0x0001, Memory.fieldMask(0));
		assertEquals(0x0003, Memory.fieldMask(1));
		assertEquals(0x0007, Memory.fieldMask(2));
		assertEquals(0x000F, Memory.fieldMask(3));
		assertEquals(0x001F, Memory.fieldMask(4));
		assertEquals(0x003F, Memory.fieldMask(5));
		assertEquals(0x007F, Memory.fieldMask(6));
		assertEquals(0x00FF, Memory.fieldMask(7));
		assertEquals(0x01FF, Memory.fieldMask(8));
		assertEquals(0x03FF, Memory.fieldMask(9));
		assertEquals(0x07FF, Memory.fieldMask(10));
		assertEquals(0x0FFF, Memory.fieldMask(11));
		assertEquals(0x1FFF, Memory.fieldMask(12));
		assertEquals(0x3FFF, Memory.fieldMask(13));
		assertEquals(0x7FFF, Memory.fieldMask(14));
		assertEquals(0xFFFF, Memory.fieldMask(15));
	}
	
	@Test
	public void testReadField() {
		int source = 0xcafe;
		// spec8 = pos (4bits) and size (4bits)
		
		int d1 = Memory.readField(source, 0x03);
		int d2 = Memory.readField(source, 0x43);
		int d3 = Memory.readField(source, 0x83);
		int d4 = Memory.readField(source, 0xc3);
		//
		int d23   = Memory.readField(source, 0x47);
		int d1234 = Memory.readField(source, 0x0f);
		
		assertEquals(0xc, d1);
		assertEquals(0xa, d2);
		assertEquals(0xf, d3);
		assertEquals(0xe, d4);
		//
		assertEquals(0xaf,     d23);
		assertEquals(source, d1234);
	}
	
	@Test
	public void testWriteField() {
		int dest = 0xcafe;
		
		int t1 = Memory.writeField(dest, 0x03, 1);
		int t2 = Memory.writeField(dest, 0x43, 2);
		int t3 = Memory.writeField(dest, 0x83, 3);
		int t4 = Memory.writeField(dest, 0xc3, 4);
		//
		int t23 = Memory.writeField(dest, 0x47, 0xac);
		int t1234 = Memory.writeField(dest, (byte)0x0f, 0xacde);
		
		assertEquals(0x1afe, t1);
		assertEquals(0xc2fe, t2);
		assertEquals(0xca3e, t3);
		assertEquals(0xcaf4, t4);
		
		assertEquals(0xcace, t23);
		assertEquals(0xacde, t1234);
	}

	@Test
	public void testLengthenPDA() {
		int value       = 0x1234;

		{
			int actual = Memory.lengthenPDA(value);
			int expect = Memory.PDA + value;
			
			assertEquals(expect, actual);
		}
	}

	@Test
	public void testOffsetPDA() {
		int value       = Memory.PDA + 0x1234;

		{
			int actual = Memory.offsetPDA(value);
			int expect = value - Memory.PDA;
			
			assertEquals(expect, actual);
		}
	}
	
	@Test
	public void testFetchPDA() {
		int pointer = 0x5678;
		int value   = 65432;
		
		int     longPointer = Memory.lengthenPDA(pointer);
		short[] page        = Memory.rawPage(longPointer);
		
		fill(page, 0);
		
		// Before
		{
			int actual = Memory.fetchPDA(pointer);
			assertNotEquals(value, actual);
		}
		
		Memory.storePDA(pointer, value);
		
		// After
		{
			int pageOffset = longPointer & Memory.PAGE_OFFSET_MASK;
			int actual= page[pageOffset] & 0xFFFF;
			assertEquals(value, actual);
		}
		{
			int actual = Memory.fetchPDA(pointer);
			assertEquals(value, actual);
		}
	}
	
	@Test
	public void testStorePDA() {
		int pointer = 0x5678;
		int value   = 65432;
		
		int     longPointer = Memory.lengthenPDA(pointer);
		short[] page        = Memory.rawPage(longPointer);
		
		fill(page, 0);
		
		// Before
		{
			int actual = Memory.fetchPDA(pointer);
			assertNotEquals(value, actual);
		}
		
		Memory.storePDA(pointer, value);
		
		// After
		{
			int pageOffset = longPointer & Memory.PAGE_OFFSET_MASK;
			int actual= page[pageOffset] & 0xFFFF;
			assertEquals(value, actual);
		}
		{
			int actual = Memory.fetchPDA(pointer);
			assertEquals(value, actual);
		}
	}

}
