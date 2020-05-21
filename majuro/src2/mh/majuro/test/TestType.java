package mh.majuro.test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotEquals;

import org.junit.Test;

import mh.majuro.mesa.Memory;
import mh.majuro.mesa.type.GlobalWord;
import mh.majuro.mesa.type.LocalOverhead;
import mh.majuro.mesa.type.LocalWord;
import mh.majuro.mesa.type.TaggedControlLink;

public class TestType extends TestBase {
	@Test
	public void testLocalWord() {
		int va = 0x10000;
		
		//LocalWord: TYPE = MACHINE DEPENDENT RECORD[
		//  available(0:0..7): BYTE,
		//  fsi(0:8..15): FSIndex];

		assertEquals(1, LocalWord.SIZE);

		{
			fillPageZero(va);
			
			assertNotEquals(0xAB, LocalWord.available.get(va));
			assertNotEquals(0xCD, LocalWord.fsi.get(va));

			Memory.rawWrite(va, 0xABCD);
			
			assertEquals(0xAB, LocalWord.available.get(va));
			assertEquals(0xCD, LocalWord.fsi.get(va));
		}
		
		{
			fillPageZero(va);
			
			assertNotEquals(0xAB, LocalWord.available.get(va));
			assertNotEquals(0xCD, LocalWord.fsi.get(va));

			LocalWord.available.set(va, 0xAB);
			assertEquals(0xAB, LocalWord.available.get(va));
			assertNotEquals(0xCD, LocalWord.fsi.get(va));
			
			LocalWord.fsi.set(va, 0xCD);
			assertEquals(0xAB, LocalWord.available.get(va));
			assertEquals(0xCD, LocalWord.fsi.get(va));
		}
	}
	
	@Test
	public void testLocalOverhead() {
		int va = 0x10000;
		
		//LocalOverhead : TYPE = MACHINE DEPENDENT RECORD [
		//  word (0):       LocalWord.
		//  returnlink(1) : ShortControlLink,
		//  globallink(2) : GFTHandle,
		//  pc(3):          CARDINAL,
		//  local(4):       LocaiVariables];

		assertEquals(4, LocalOverhead.SIZE);

		{
			fillPageZero(va);
			
			assertNotEquals(0x1234, LocalOverhead.word.get(va));
			assertNotEquals(0x2345, LocalOverhead.returnlink.get(va));
			assertNotEquals(0x3456, LocalOverhead.globallink.get(va));
			assertNotEquals(0x4567, LocalOverhead.pc.get(va));

			Memory.rawWrite(va + 0, 0x1234);
			Memory.rawWrite(va + 1, 0x2345);
			Memory.rawWrite(va + 2, 0x3456);
			Memory.rawWrite(va + 3, 0x4567);
			
			assertEquals(0x1234, LocalOverhead.word.get(va));
			assertEquals(0x2345, LocalOverhead.returnlink.get(va));
			assertEquals(0x3456, LocalOverhead.globallink.get(va));
			assertEquals(0x4567, LocalOverhead.pc.get(va));
		}
	}
	
	@Test
	public void testTaggedControlLink() {
		int va = 0x10000;
		
		//TaggedControl link: TYPE =  MACHINE DEPENDENT RECORD [
		//  data (0:  0..13): [0 .. 377778];
		//  tag  (0: 14..15): [0 .. 3],
		//  fill (1) :  UNSPECIFIED);

		assertEquals(2, TaggedControlLink.SIZE);

		{
			fillPageZero(va);
			
			assertNotEquals(0x1234, TaggedControlLink.data.get(va));
			assertNotEquals(0x0001, TaggedControlLink.tag.get(va));
			assertNotEquals(0x3456, TaggedControlLink.fill.get(va));

			Memory.rawWriteDbl(va, 0x456789AB);
			
			assertEquals(0x4567 >> 2, TaggedControlLink.data.get(va));
			assertEquals(0x0003, TaggedControlLink.tag.get(va));
			assertEquals(0x89AB, TaggedControlLink.fill.get(va));
		}
	}

	@Test
	public void testGlobalWord() {
		int va = 0x10000;
		
		// GlobalWord: TYPE = MACHINE DEPENDENT RECORD [
		//   gfi (0:0..13): GFTIndex,
		//   trapxfers (0:14..14),
		//   codelinks (0:15..15): BOOLEAN];

		assertEquals(1, GlobalWord.SIZE);

		{
			fillPageZero(va);
			
			assertNotEquals(0xABCD >> 2, GlobalWord.gfi.get(va));
//			assertNotEquals(false, GlobalWord.trapxfers.get(va));
			assertNotEquals(true, GlobalWord.codelinks.get(va));

			Memory.rawWrite(va, 0xABCD);
			
			assertEquals(0xABCD >> 2, GlobalWord.gfi.get(va));
			assertEquals(false, GlobalWord.trapxfers.get(va));
			assertEquals(true, GlobalWord.codelinks.get(va));
		}
		
		{
			fillPageZero(va);

			GlobalWord.gfi.set(va, 0xFFFF);			
			
			assertEquals(0xFFFC, Memory.rawRead(va));
		}
		
		{
			fillPageZero(va);

			GlobalWord.trapxfers.set(va, true);			
			
			assertEquals(0x0002, Memory.rawRead(va));
		}
		
		{
			fillPageZero(va);

			GlobalWord.codelinks.set(va, true);			
			
			assertEquals(0x0001, Memory.rawRead(va));
		}
	}

}
