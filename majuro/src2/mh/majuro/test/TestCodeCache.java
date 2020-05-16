package mh.majuro.test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotEquals;

import org.junit.Test;

import mh.majuro.mesa.CodeCache;
import mh.majuro.mesa.Memory;

public class TestCodeCache extends TestBase {
	@Test
	public void testSetGetCB() {
		int value = 0x123456;
		
		// Before
		assertNotEquals(value, CodeCache.getCB());
		
		CodeCache.setCB(value);
		
		// After
		assertEquals(value, CodeCache.getCB());
	}
	
	@Test
	public void testSetGetPC() {
		int value = 0x9876;
		
		// Before
		assertNotEquals(value, CodeCache.getPC());
		
		CodeCache.setPC(value);
		
		// After
		assertEquals(value, CodeCache.getPC());
	}
	
	@Test
	public void testGetCodeByte() {
		int cb = 0x400080;
		int pc = 0x20;
		
		int va = cb + (pc / 2);
		
		// Before
		CodeCache.setCB(cb);
		CodeCache.setPC(pc);
		
		fillPageZero(va + 0);
		fillPageZero(va + 1);
		Memory.rawWrite(va + 0, 0x1234);
		Memory.rawWrite(va + 1, 0x5678);
		
		// After
		assertEquals(0x12, CodeCache.getCodeByte());
		assertEquals(0x34, CodeCache.getCodeByte());
		assertEquals(0x56, CodeCache.getCodeByte());
		assertEquals(0x78, CodeCache.getCodeByte());
	}

	@Test
	public void testGetCodeByte2() {
		int cb = 0x400080;
		int pc = 0x9876;
		
		int va = cb + (pc / 2);
		
		// Before
		CodeCache.setCB(cb);
		CodeCache.setPC(pc);
		
		fillPageZero(va + 0);
		fillPageZero(va + 1);
		Memory.rawWrite(va + 0, 0x1234);
		Memory.rawWrite(va + 1, 0x5678);
		
		// After
		assertEquals(0x12, CodeCache.getCodeByte());
		assertEquals(0x34, CodeCache.getCodeByte());
		assertEquals(0x56, CodeCache.getCodeByte());
		assertEquals(0x78, CodeCache.getCodeByte());
	}

	@Test
	public void testGetCodeByte3() {
		int cb = 0x400080;
		int pc = 0x00FE;
		
		int va = cb + (pc / 2);
		
		// Before
		CodeCache.setCB(cb);
		CodeCache.setPC(pc);
		
//		logger.info("va {}", String.format("%X", va));
		fillPageZero(va + 0);
		fillPageZero(va + 1);
		Memory.rawWrite(va + 0, 0x1234);
		Memory.rawWrite(va + 1, 0x5678);
		
		// After
		assertEquals(0x12, CodeCache.getCodeByte());
		assertEquals(0x34, CodeCache.getCodeByte());
		assertEquals(0x56, CodeCache.getCodeByte());
		assertEquals(0x78, CodeCache.getCodeByte());
	}

	@Test
	public void testGetCodeWord() {
		int cb = 0x400080;
		int pc = 0x9876;
		
		int va = cb + (pc / 2);
		
		// Before
		CodeCache.setCB(cb);
		CodeCache.setPC(pc);
		
		fillPageZero(va + 0);
		fillPageZero(va + 1);
		Memory.rawWrite(va + 0, 0x1234);
		Memory.rawWrite(va + 1, 0x5678);
		
		// After
		assertEquals(0x1234, CodeCache.getCodeWord());
		assertEquals(0x5678, CodeCache.getCodeWord());
	}
	@Test
	public void testGetCodeWord2() {
		int cb = 0x400080;
		int pc = 0x9876;
		
		int va = cb + (pc / 2);
		
		// Before
		CodeCache.setCB(cb);
		CodeCache.setPC(pc);
		
		fillPageZero(va + 0);
		fillPageZero(va + 1);
		Memory.rawWrite(va + 0, 0x1234);
		Memory.rawWrite(va + 1, 0x5678);
		
		// After
		assertEquals(0x12,   CodeCache.getCodeByte());
		assertEquals(0x3456, CodeCache.getCodeWord());
		assertEquals(0x78,   CodeCache.getCodeByte());
	}
	@Test
	public void testGetCodeWord3() {
		int cb = 0x400080;
		int pc = 0x00FE;
		
		int va = cb + (pc / 2);
		
		// Before
		CodeCache.setCB(cb);
		CodeCache.setPC(pc);
		
		fillPageZero(va + 0);
		fillPageZero(va + 1);
		Memory.rawWrite(va + 0, 0x1234);
		Memory.rawWrite(va + 1, 0x5678);
		
		// After
		assertEquals(0x12,   CodeCache.getCodeByte());
		assertEquals(0x3456, CodeCache.getCodeWord());
		assertEquals(0x78,   CodeCache.getCodeByte());
	}


}
