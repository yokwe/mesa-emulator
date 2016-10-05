package mh.majuro.test;

import static org.junit.Assert.assertEquals;
import mh.majuro.mesa.Type.Field16;
import mh.majuro.mesa.Type.Field32;
import mh.majuro.mesa.Type.Field8;

import org.junit.Test;

public class TypeTest extends TestBase {
	static final class Item8 {
		public static final Field8 data0 = Field8.getInstance(0);
		public static final Field8 data1 = Field8.getInstance(0,  0,  3);
		public static final Field8 data2 = Field8.getInstance(0,  4,  7);
	}
	
	static final class Item16 {
		public static final Field16 data0 = Field16.getInstance(0);
		public static final Field16 data1 = Field16.getInstance(0,  0,  7);
		public static final Field16 data2 = Field16.getInstance(0,  8, 15);
	}

	static final class Item32 {
		public static final Field32 data0 = Field32.getInstance(0);
		public static final Field32 data1 = Field32.getInstance(0,  0, 15);
		public static final Field32 data2 = Field32.getInstance(0, 16, 31);
	}

	@Test
	public void testItem8() {
		byte set0 = Item8.data0.setData((byte)0, ~0);
		byte set1 = Item8.data1.setData((byte)0, ~0);
		byte set2 = Item8.data2.setData((byte)0, ~0);
		
		int get0 = Item8.data0.getData((byte)~0);
		int get1 = Item8.data1.getData((byte)~0);
		int get2 = Item8.data2.getData((byte)~0);
		
		assertEquals((byte)0xff, set0);
		assertEquals((byte)0xf0, set1);
		assertEquals((byte)0x0f, set2);
		
		assertEquals(0xff, get0);
		assertEquals(0x0f, get1);
		assertEquals(0x0f, get2);
	}

	@Test
	public void testItem16() {
		short set0 = Item16.data0.setData((short)0, ~0);
		short set1 = Item16.data1.setData((short)0, ~0);
		short set2 = Item16.data2.setData((short)0, ~0);
		
		int get0 = Item16.data0.getData((short)~0);
		int get1 = Item16.data1.getData((short)~0);
		int get2 = Item16.data2.getData((short)~0);
		
		assertEquals((short)0xffff, set0);
		assertEquals((short)0xff00, set1);
		assertEquals((short)0x00ff, set2);
		
		assertEquals(0xffff, get0);
		assertEquals(0x00ff, get1);
		assertEquals(0x00ff, get2);
	}

	@Test
	public void testItem32() {
		int set0 = Item32.data0.setData(0, ~0);
		int set1 = Item32.data1.setData(0, ~0);
		int set2 = Item32.data2.setData(0, ~0);
		
		int get0 = Item32.data0.getData(~0);
		int get1 = Item32.data1.getData(~0);
		int get2 = Item32.data2.getData(~0);
		
		assertEquals(0xffffffff, set0);
		assertEquals(0xffff0000, set1);
		assertEquals(0x0000ffff, set2);
		
		assertEquals(0xffffffff, get0);
		assertEquals(0x0000ffff, get1);
		assertEquals(0x0000ffff, get2);
	}
}
