package mh.majuro.test;

import static org.junit.Assert.*;

import org.junit.Test;

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
			int expect = DEFAULT_MDS + value;
			
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

}
