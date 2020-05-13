package mh.majuro.test;

import static org.junit.Assert.assertEquals;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import mh.majuro.mesa.CodeCache;
import mh.majuro.mesa.Memory;
import mh.majuro.mesa.Perf;

public class TestBase {
	private static final Logger logger = LoggerFactory.getLogger(TestBase.class);
	
	@Before
	public void setUp() throws Exception {
		logger.info("setUp");
		int vmBIts = 23;
		int rmBits = 20;
		
		int mds = 0x00030080;
		int cb  = 0x00030080;
		int pc  = 0x20;
		
		Memory.initialize(vmBIts, rmBits);
		
		Memory.setMDS(mds);

		CodeCache.setCB(cb);
		CodeCache.setPC(pc);
		
		Perf.initialize();
	}
	
	@After
	public void tearDown() throws Exception {
		logger.info("tearDown");
	}
	
	@Test
	public void myTest1() {
		logger.info("myTest1");
		int cb  = 0x00030000;
		int pc  = 0x20;
		
		CodeCache.setCB(cb);
		CodeCache.setPC(pc);
		
		{
			int p = cb;
			for(int i = 0; i < Memory.PAGE_SIZE; i++) {
				Memory.sepcialWrite(p++, (short)(0x1100 + i));
			}
			for(int i = 0; i < Memory.PAGE_SIZE; i++) {
				Memory.sepcialWrite(p++, (short)(0x2200 + i));
			}
		}

		int t0 = CodeCache.getCodeByte();
		int t1 = CodeCache.getCodeByte();
		int t2 = CodeCache.getCodeByte();
		int t3 = CodeCache.getCodeByte();
		
		int t = (cb + pc) & 0xFF;
		
		assertEquals(0x11, t0);
		assertEquals(t + 0, t1);
		assertEquals(0x11, t2);
		assertEquals(t + 1, t3);
	}
	@Test
	public void myTest2() {
		logger.info("myTest2");
		int cb  = 0x000300E0;
		int pc  = 0x3E;
		
		CodeCache.setCB(cb);
		CodeCache.setPC(pc);
		
		{
			int p = cb;
			for(int i = 0; i < Memory.PAGE_SIZE; i++) {
				Memory.sepcialWrite(p++, (short)(0x1100 + i));
			}
			for(int i = 0; i < Memory.PAGE_SIZE; i++) {
				Memory.sepcialWrite(p++, (short)(0x2200 + i));
			}
		}

		{
			int p = CodeCache.getPC();
			int b = CodeCache.getCodeByte();
			logger.info("{}", String.format("%04X  %04X", p, b));
		}
		{
			int p = CodeCache.getPC();
			int b = CodeCache.getCodeByte();
			logger.info("{}", String.format("%04X  %04X", p, b));
		}
		{
			int p = CodeCache.getPC();
			int b = CodeCache.getCodeByte();
			logger.info("{}", String.format("%04X  %04X", p, b));
		}
		{
			int p = CodeCache.getPC();
			int b = CodeCache.getCodeByte();
			logger.info("{}", String.format("%04X  %04X", p, b));
		}
		
		logger.info("AAA");
		Perf.stats();
		logger.info("BBB");
	}

}
