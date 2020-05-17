package mh.majuro.test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotEquals;

import org.junit.Test;

import mh.majuro.mesa.LFCache;
import mh.majuro.mesa.Memory;

public class TestLFCache extends TestBase {
	@Test
	public void testSetGetLF() {
		int value = 0x9876;
		
		// Before
		assertNotEquals(value, LFCache.getLF());
		
		LFCache.setLF(value);
		
		// After
		assertEquals(value, LFCache.getLF());
	}

	@Test
	public void testFetchLF() {
		int mds   = 0x40000;
		int lf    = 0x3456;
		int index = 4;
		
		Memory.setMDS(mds);
		LFCache.setLF(lf);
		int va = Memory.lengthenPointer(lf + index);
		
		// Before
		assertNotEquals(0x1234, LFCache.fetch(index + 0));
		assertNotEquals(0x5678, LFCache.fetch(index + 1));
		
		Memory.rawWrite(va + 0, 0x1234);
		Memory.rawWrite(va + 1, 0x5678);

		// After
		assertEquals(0x1234, LFCache.fetch(index + 0));
		assertEquals(0x5678, LFCache.fetch(index + 1));
	}

	@Test
	public void testFetchLF2() {
		int mds   = 0x40000;
		int lf    = 0x34F0;
		int index = 15;
		
		Memory.setMDS(mds);
		LFCache.setLF(lf);
		int va = Memory.lengthenPointer(lf + index);
		
//		logger.info("va {}", String.format("%X", va));
		fillPageZero(va + 0);
		fillPageZero(va + 1);

		// Before
		assertNotEquals(0x1234, LFCache.fetch(index + 0));
		assertNotEquals(0x5678, LFCache.fetch(index + 1));
		
		Memory.rawWrite(va + 0, 0x1234);
		Memory.rawWrite(va + 1, 0x5678);

		// After
		assertEquals(0x1234, LFCache.fetch(index + 0));
		assertEquals(0x5678, LFCache.fetch(index + 1));
	}

	@Test
	public void testStoreLF() {
		int mds   = 0x40000;
		int lf    = 0x3456;
		int index = 4;
		
		Memory.setMDS(mds);
		LFCache.setLF(lf);
		int va = Memory.lengthenPointer(lf + index);
		
		// Before
		assertNotEquals(0x1234, LFCache.fetch(index + 0));
		assertNotEquals(0x5678, LFCache.fetch(index + 1));
		
		LFCache.store(index + 0, 0x1234);
		LFCache.store(index + 1, 0x5678);

		// After
		assertEquals(0x1234, Memory.rawRead(va + 0));
		assertEquals(0x5678, Memory.rawRead(va + 1));
	}

	@Test
	public void testStoreLF2() {
		int mds   = 0x40000;
		int lf    = 0x34F0;
		int index = 15;
		
		Memory.setMDS(mds);
		LFCache.setLF(lf);
		int va = Memory.lengthenPointer(lf + index);
		
		// Before
		assertNotEquals(0x1234, LFCache.fetch(index + 0));
		assertNotEquals(0x5678, LFCache.fetch(index + 1));
		
		LFCache.store(index + 0, 0x1234);
		LFCache.store(index + 1, 0x5678);

		// After
		assertEquals(0x1234, Memory.rawRead(va + 0));
		assertEquals(0x5678, Memory.rawRead(va + 1));
	}

}
