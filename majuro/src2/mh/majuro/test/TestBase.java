package mh.majuro.test;

import java.util.Random;

import org.junit.After;
import org.junit.Before;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import mh.majuro.mesa.CodeCache;
import mh.majuro.mesa.Memory;
import mh.majuro.mesa.Perf;

public class TestBase {
	protected static final Logger logger = LoggerFactory.getLogger(TestBase.class);
	
	protected static final Random random = new Random(System.currentTimeMillis());
	
	protected static void fill(short[] page) {
		for(int i = 0; i < page.length; i++) page[i] = (short)random.nextInt();
	}
	protected static void fill(short[] page, int value) {
		for(int i = 0; i < page.length; i++) page[i] = (short)value;
	}
	protected static void fill(short[] page, int start, int increment) {
		int value = start;
		for(int i = 0; i < page.length; i++) {
			page[i] = (short)value;
			value += increment;
		}
	}
	
	protected static final int DEFAULT_MDS = 0x0004_0000;
	protected static final int DEFAULT_CB  = 0x0003_0080;
	protected static final int DEFAULT_PC  = 0x0020;
	
	@Before
	public void setUp() throws Exception {
		int vmBIts = 23;
		int rmBits = 20;
		
		Perf.initialize();

		Memory.initialize(vmBIts, rmBits);
		
		Memory.setMDS(DEFAULT_MDS);

		CodeCache.setCB(DEFAULT_CB);
		CodeCache.setPC(DEFAULT_PC);
		
		fill(Memory.rawPage(0x0003_0000), 0x3000, 1);
		fill(Memory.rawPage(0x0003_0000), 0x3000, 1);
		fill(Memory.rawPage(0x0003_0100), 0x3100, 1);
		fill(Memory.rawPage(0x0004_0000), 0x4000, 1);
		fill(Memory.rawPage(0x0004_0100), 0x4100, 1);
		fill(Memory.rawPage(0x0005_0000), 0x5000, 1);
	}
	
	@After
	public void tearDown() throws Exception {
	}

}
