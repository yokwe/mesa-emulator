package mh.majuro.test;

import static org.junit.Assert.assertEquals;
import mh.majuro.mesa.Util;

import org.junit.Test;


public class UtilTest extends TestBase {
	@Test
	public void testAsIntByte() {
		assertEquals((int)1, Util.asInt((byte)1));
		assertEquals((int)200, Util.asInt((byte)200));
	}

	
	@Test
	public void testAsIntShort() {
		assertEquals((int)1,     Util.asInt((short)1));
		assertEquals((int)40000, Util.asInt((short)40000));
	}
	
	@Test
	public void testAsLongInt() {
		assertEquals((long)1L,          Util.asLong((int)1));
		assertEquals((long)4000000000L, Util.asLong((int)4000000000L));
	}
	
	@Test
	public void testLowHalf() {
		assertEquals((short)0x5678, Util.LowHalf(0x12345678));
		assertEquals((short)0x8765, Util.LowHalf(0x43218765));
	}
	
	@Test
	public void testHighHalf() {
		assertEquals((short)0x5678, Util.HighHalf(0x56781234));
		assertEquals((short)0x8765, Util.HighHalf(0x87654321));
	}
	
	@Test
	public void testAsIntShortShrt() {
		assertEquals((int)0x12345678, Util.asInt((short)0x1234, (short)0x5678));
		assertEquals((int)0x9876fedc, Util.asInt((short)0x9876, (short)0xfedc));
	}
	
	@Test
	public void testHighByte() {
		assertEquals((byte)0x12, (byte)Util.HighByte((short)0x1234));
		assertEquals((byte)0x98, (byte)Util.HighByte((short)0x9876));
	}
	
	@Test
	public void testLowByte() {
		assertEquals((byte)0x34, (byte)Util.LowByte((short)0x1234));
		assertEquals((byte)0x89, (byte)Util.LowByte((short)0x6789));
	}
	
	@Test
	public void testAsShortIntInt() {
		assertEquals((short)0x1234, Util.asShort(0x12, 0x34));
		assertEquals((short)0x89ab, Util.asShort(0x89, 0xab));
	}
	
	@Test
	public void testLeftNibble() {
		assertEquals((byte)0x1, Util.LeftNibble((byte)0x12));
		assertEquals((byte)0x9, Util.LeftNibble((byte)0x9a));
	}
	
	@Test
	public void testRightNibble() {
		//assertEquals((byte)0x2, (byte)Util.RightNibble((byte)0x12));
		assertEquals(0xa, Util.RightNibble((byte)0x9a));
	}
	
	@Test
	public void testShift() {
		assertEquals((short)0x1234, Util.Shift((short)0x1234,  0));
		assertEquals((short)0x3400, Util.Shift((short)0x1234,  8));
		assertEquals((short)0x0000, Util.Shift((short)0x1234, 16));
		
		assertEquals((short)0x8765, Util.Shift((short)0x8765,  0));
		assertEquals((short)0x6500, Util.Shift((short)0x8765,  8));
		assertEquals((short)0x0000, Util.Shift((short)0x8765, 16));
		
		assertEquals((short)0x0012, Util.Shift((short)0x1234,  -8));
		assertEquals((short)0x0000, Util.Shift((short)0x1234, -16));
		
		assertEquals((short)0x0087, Util.Shift((short)0x8765,  -8));
		assertEquals((short)0x0000, Util.Shift((short)0x8765, -16));
	}
	
	@Test
	public void testLongShift() {
		assertEquals(0x12345678, Util.LongShift(0x12345678,  0));
		assertEquals(0x56780000, Util.LongShift(0x12345678, 16));
		assertEquals(0x00000000, Util.LongShift(0x12345678, 32));
		
		assertEquals(0x87654321, Util.LongShift(0x87654321,  0));
		assertEquals(0x43210000, Util.LongShift(0x87654321, 16));
		assertEquals(0x00000000, Util.LongShift(0x87654321, 32));
		
		assertEquals(0x00001234, Util.LongShift(0x12345678, -16));
		assertEquals(0x00000000, Util.LongShift(0x12345678, -32));
		
		assertEquals(0x00008765, Util.LongShift(0x87654321, -16));
		assertEquals(0x00000000, Util.LongShift(0x87654321, -32));
	}
	
	@Test
	public void testLongArithShift() {
		assertEquals(0x12345678, Util.LongArithShift(0x12345678,  0));
		assertEquals(0x56780000, Util.LongArithShift(0x12345678, 16));
		assertEquals(0x00000000, Util.LongArithShift(0x12345678, 32));
		
		assertEquals(0x87654321, Util.LongArithShift(0x87654321,  0));
		assertEquals(0xc3210000, Util.LongArithShift(0x87654321, 16));
		assertEquals(0x00000000, Util.LongArithShift(0x87654321, 32));
		
		assertEquals(0x00001234, Util.LongArithShift(0x12345678, -16));
		assertEquals(0x00000000, Util.LongArithShift(0x12345678, -32));
		
		assertEquals(0xffff8765, Util.LongArithShift(0x87654321, -16));
		assertEquals(0xffffffff, Util.LongArithShift(0x87654321, -32));
	}
	
	@Test
	public void testRotate() {
		assertEquals((short)0x1234, Util.Rotate((short)0x1234,  0));
		assertEquals((short)0x2341, Util.Rotate((short)0x1234,  4));
		assertEquals((short)0x1234, Util.Rotate((short)0x1234, 16));
		
		assertEquals((short)0x8765, Util.Rotate((short)0x8765,  0));
		assertEquals((short)0x7658, Util.Rotate((short)0x8765,  4));
		assertEquals((short)0x8765, Util.Rotate((short)0x8765, 16));
		
		assertEquals((short)0x4123, Util.Rotate((short)0x1234,  -4));
		assertEquals((short)0x1234, Util.Rotate((short)0x1234, -16));
		
		assertEquals((short)0x5876, Util.Rotate((short)0x8765,  -4));
		assertEquals((short)0x8765, Util.Rotate((short)0x8765, -16));
	}
}
