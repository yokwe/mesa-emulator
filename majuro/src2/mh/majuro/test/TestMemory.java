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

}
