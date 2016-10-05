package mh.majuro.test;

import static org.junit.Assert.assertEquals;
import mh.majuro.mesa.Memory;
import mh.majuro.mesa.MesaAbort;
import mh.majuro.mesa.Opcode;
import mh.majuro.mesa.OpcodeInfo;
import mh.majuro.mesa.Processor;

import org.junit.Test;

public class Opcode0xxTest extends TestBase {
    // 00 NOOP

    // 01 LL0
	@Test
	public void testLL0() {
		int n = 0;
		writeOpcode((byte)OpcodeInfo.LL0.code);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.ReadMDS(Processor.LF, n), Processor.stack[0]);
	}

    // 02 LL1
	@Test
	public void testLL1() {
		int n = 1;
		writeOpcode((byte)OpcodeInfo.LL1.code);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.ReadMDS(Processor.LF, n), Processor.stack[0]);
	}


    // 03 LL2
	@Test
	public void testLL2() {
		int n = 2;
		writeOpcode((byte)OpcodeInfo.LL2.code);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.ReadMDS(Processor.LF, n), Processor.stack[0]);
	}

    // 04 LL3
	@Test
	public void testLL3() {
		int n = 3;
		writeOpcode((byte)OpcodeInfo.LL3.code);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.ReadMDS(Processor.LF, n), Processor.stack[0]);
	}

    // 05 LL4
	@Test
	public void testLL4() {
		int n = 4;
		writeOpcode((byte)OpcodeInfo.LL4.code);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.ReadMDS(Processor.LF, n), Processor.stack[0]);
	}

    // 06 LL5
	@Test
	public void testLL5() {
		int n = 5;
		writeOpcode((byte)OpcodeInfo.LL5.code);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.ReadMDS(Processor.LF, n), Processor.stack[0]);
	}

	// 07 LL6
	@Test
	public void testLL6() {
		int n = 6;
		writeOpcode((byte)OpcodeInfo.LL6.code);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.ReadMDS(Processor.LF, n), Processor.stack[0]);
	}

	// 010 LL7
	@Test
	public void testLL7() {
		int n = 7;
		writeOpcode((byte)OpcodeInfo.LL7.code);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.ReadMDS(Processor.LF, n), Processor.stack[0]);
	}

	// 011 LL8
	@Test
	public void testLL8() {
		int n = 8;
		writeOpcode((byte)OpcodeInfo.LL8.code);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.ReadMDS(Processor.LF, n), Processor.stack[0]);
	}

    // 012 LL9
	@Test
	public void testLL9() {
		int n = 9;
		writeOpcode((byte)OpcodeInfo.LL9.code);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.ReadMDS(Processor.LF, n), Processor.stack[0]);
	}

	// 013 LL10
	@Test
	public void testLL10() {
		int n = 10;
		writeOpcode((byte)OpcodeInfo.LL10.code);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.ReadMDS(Processor.LF, n), Processor.stack[0]);
	}

    // 014 LL11
	@Test
	public void testLL11() {
		int n = 11;
		writeOpcode((byte)OpcodeInfo.LL11.code);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.ReadMDS(Processor.LF, n), Processor.stack[0]);
	}

    // 015 LLB
	@Test
	public void testLLB() {
		int n = 30;
		writeOpcode((byte)OpcodeInfo.LLB.code, (byte)n);
		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.ReadMDS(Processor.LF, n), Processor.stack[0]);
	}

	// 016 LLD0
	@Test
	public void testLLD0() {
		int n = 0;
		writeOpcode((byte)OpcodeInfo.LLD0.code);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(2, Processor.SP);
		assertEquals(Memory.ReadMDS(Processor.LF, n + 0), Processor.stack[0]);
		assertEquals(Memory.ReadMDS(Processor.LF, n + 1), Processor.stack[1]);
	}
	
    // 017 LLD1
	@Test
	public void testLLD1() {
		int n = 1;
		writeOpcode((byte)OpcodeInfo.LLD1.code);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(2, Processor.SP);
		assertEquals(Memory.ReadMDS(Processor.LF, n + 0), Processor.stack[0]);
		assertEquals(Memory.ReadMDS(Processor.LF, n + 1), Processor.stack[1]);
	}
	
	// 020 LLD2
	@Test
	public void testLLD2() {
		int n = 2;
		writeOpcode((byte)OpcodeInfo.LLD2.code);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(2, Processor.SP);
		assertEquals(Memory.ReadMDS(Processor.LF, n + 0), Processor.stack[0]);
		assertEquals(Memory.ReadMDS(Processor.LF, n + 1), Processor.stack[1]);
	}
	
    // 021 LLD3
	@Test
	public void testLLD3() {
		int n = 3;
		writeOpcode((byte)OpcodeInfo.LLD3.code);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(2, Processor.SP);
		assertEquals(Memory.ReadMDS(Processor.LF, n + 0), Processor.stack[0]);
		assertEquals(Memory.ReadMDS(Processor.LF, n + 1), Processor.stack[1]);
	}
	
    // 022 LLD4
	@Test
	public void testLLD4() {
		int n = 4;
		writeOpcode((byte)OpcodeInfo.LLD4.code);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(2, Processor.SP);
		assertEquals(Memory.ReadMDS(Processor.LF, n + 0), Processor.stack[0]);
		assertEquals(Memory.ReadMDS(Processor.LF, n + 1), Processor.stack[1]);
	}
	
    // 023 LLD5
	@Test
	public void testLLD5() {
		int n = 5;
		writeOpcode((byte)OpcodeInfo.LLD5.code);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(2, Processor.SP);
		assertEquals(Memory.ReadMDS(Processor.LF, n + 0), Processor.stack[0]);
		assertEquals(Memory.ReadMDS(Processor.LF, n + 1), Processor.stack[1]);
	}
	
    // 024 LLD6
	@Test
	public void testLLD6() {
		int n = 6;
		writeOpcode((byte)OpcodeInfo.LLD6.code);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(2, Processor.SP);
		assertEquals(Memory.ReadMDS(Processor.LF, n + 0), Processor.stack[0]);
		assertEquals(Memory.ReadMDS(Processor.LF, n + 1), Processor.stack[1]);
	}
	
    // 025 LLD7
	@Test
	public void testLLD7() {
		int n = 7;
		writeOpcode((byte)OpcodeInfo.LLD7.code);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(2, Processor.SP);
		assertEquals(Memory.ReadMDS(Processor.LF, n + 0), Processor.stack[0]);
		assertEquals(Memory.ReadMDS(Processor.LF, n + 1), Processor.stack[1]);
	}
	
    // 026 LLD8
	@Test
	public void testLLD8() {
		int n = 8;
		writeOpcode((byte)OpcodeInfo.LLD8.code);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(2, Processor.SP);
		assertEquals(Memory.ReadMDS(Processor.LF, n + 0), Processor.stack[0]);
		assertEquals(Memory.ReadMDS(Processor.LF, n + 1), Processor.stack[1]);
	}
	
    // 027 LLD10
	@Test
	public void testLLD10() {
		int n = 10;
		writeOpcode((byte)OpcodeInfo.LLD10.code);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(2, Processor.SP);
		assertEquals(Memory.ReadMDS(Processor.LF, n + 0), Processor.stack[0]);
		assertEquals(Memory.ReadMDS(Processor.LF, n + 1), Processor.stack[1]);
	}
	
    // 030 LLDB
	@Test
	public void testLLDB() {
		int n = 20;
		writeOpcode((byte)OpcodeInfo.LLDB.code, (byte)n);
		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(2, Processor.SP);
		assertEquals(Memory.ReadMDS(Processor.LF, n + 0), Processor.stack[0]);
		assertEquals(Memory.ReadMDS(Processor.LF, n + 1), Processor.stack[1]);
	}

    // 031 SL0
	@Test
	public void testSL0() {
		int n = 0;
		writeOpcode((byte)OpcodeInfo.SL0.code);
		Processor.SP = 1;
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(Processor.stack[0], Memory.ReadMDS(Processor.LF, n));
	}

	// 032 SL1
	@Test
	public void testSL1() {
		int n = 1;
		writeOpcode((byte)OpcodeInfo.SL1.code);
		Processor.SP = 1;
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(Processor.stack[0], Memory.ReadMDS(Processor.LF, n));
	}

	// 033 SL2
	@Test
	public void testSL2() {
		int n = 2;
		writeOpcode((byte)OpcodeInfo.SL2.code);
		Processor.SP = 1;
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(Processor.stack[0], Memory.ReadMDS(Processor.LF, n));
	}

	// 034 SL3
	@Test
	public void testSL3() {
		int n = 3;
		writeOpcode((byte)OpcodeInfo.SL3.code);
		Processor.SP = 1;
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(Processor.stack[0], Memory.ReadMDS(Processor.LF, n));
	}

	// 035 SL4
	@Test
	public void testSL4() {
		int n = 4;
		writeOpcode((byte)OpcodeInfo.SL4.code);
		Processor.SP = 1;
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(Processor.stack[0], Memory.ReadMDS(Processor.LF, n));
	}

	// 036 SL5
	@Test
	public void testSL5() {
		int n = 5;
		writeOpcode((byte)OpcodeInfo.SL5.code);
		Processor.SP = 1;
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(Processor.stack[0], Memory.ReadMDS(Processor.LF, n));
	}

	// 037 SL6
	@Test
	public void testSL6() {
		int n = 6;
		writeOpcode((byte)OpcodeInfo.SL6.code);
		Processor.SP = 1;
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(Processor.stack[0], Memory.ReadMDS(Processor.LF, n));
	}

	// 040 SL7
	@Test
	public void testSL7() {
		int n = 7;
		writeOpcode((byte)OpcodeInfo.SL7.code);
		Processor.SP = 1;
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(Processor.stack[0], Memory.ReadMDS(Processor.LF, n));
	}

	// 041 SL8
	@Test
	public void testSL8() {
		int n = 8;
		writeOpcode((byte)OpcodeInfo.SL8.code);
		Processor.SP = 1;
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(Processor.stack[0], Memory.ReadMDS(Processor.LF, n));
	}

	// 042 SL9
	@Test
	public void testSL9() {
		int n = 9;
		writeOpcode((byte)OpcodeInfo.SL9.code);
		Processor.SP = 1;
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(Processor.stack[0], Memory.ReadMDS(Processor.LF, n));
	}

	// 043 SL10
	@Test
	public void testSL10() {
		int n = 10;
		writeOpcode((byte)OpcodeInfo.SL10.code);
		Processor.SP = 1;
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(Processor.stack[0], Memory.ReadMDS(Processor.LF, n));
	}

	// 044 SLB
	@Test
	public void testSLB() {
		int n = 20;
		writeOpcode((byte)OpcodeInfo.SLB.code, (byte)n);
		Processor.SP = 1;
		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(Processor.stack[0], Memory.ReadMDS(Processor.LF, n));
	}

	// 045 SLD0
	@Test
	public void testSLD0() {
		int n = 0;
		writeOpcode((byte)OpcodeInfo.SLD0.code);
		Processor.SP = 2;
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(Processor.stack[0], Memory.ReadMDS(Processor.LF, n + 0));
		assertEquals(Processor.stack[1], Memory.ReadMDS(Processor.LF, n + 1));
	}

	// 046 SLD1
	@Test
	public void testSLD1() {
		int n = 1;
		writeOpcode((byte)OpcodeInfo.SLD1.code);
		Processor.SP = 2;
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(Processor.stack[0], Memory.ReadMDS(Processor.LF, n + 0));
		assertEquals(Processor.stack[1], Memory.ReadMDS(Processor.LF, n + 1));
	}

	// 047 SLD2
	@Test
	public void testSLD2() {
		int n = 2;
		writeOpcode((byte)OpcodeInfo.SLD2.code);
		Processor.SP = 2;
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(Processor.stack[0], Memory.ReadMDS(Processor.LF, n + 0));
		assertEquals(Processor.stack[1], Memory.ReadMDS(Processor.LF, n + 1));
	}

	// 050 SLD3
	@Test
	public void testSLD3() {
		int n = 3;
		writeOpcode((byte)OpcodeInfo.SLD3.code);
		Processor.SP = 2;
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(Processor.stack[0], Memory.ReadMDS(Processor.LF, n + 0));
		assertEquals(Processor.stack[1], Memory.ReadMDS(Processor.LF, n + 1));
	}

	// 051 SLD4
	@Test
	public void testSLD4() {
		int n = 4;
		writeOpcode((byte)OpcodeInfo.SLD4.code);
		Processor.SP = 2;
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(Processor.stack[0], Memory.ReadMDS(Processor.LF, n + 0));
		assertEquals(Processor.stack[1], Memory.ReadMDS(Processor.LF, n + 1));
	}

	// 052 SLD5
	@Test
	public void testSLD5() {
		int n = 5;
		writeOpcode((byte)OpcodeInfo.SLD5.code);
		Processor.SP = 2;
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(Processor.stack[0], Memory.ReadMDS(Processor.LF, n + 0));
		assertEquals(Processor.stack[1], Memory.ReadMDS(Processor.LF, n + 1));
	}

	// 053 SLD6
	@Test
	public void testSLD6() {
		int n = 6;
		writeOpcode((byte)OpcodeInfo.SLD6.code);
		Processor.SP = 2;
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(Processor.stack[0], Memory.ReadMDS(Processor.LF, n + 0));
		assertEquals(Processor.stack[1], Memory.ReadMDS(Processor.LF, n + 1));
	}

	// 054 SLD8
	@Test
	public void testSLD8() {
		int n = 8;
		writeOpcode((byte)OpcodeInfo.SLD8.code);
		Processor.SP = 2;
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(Processor.stack[0], Memory.ReadMDS(Processor.LF, n + 0));
		assertEquals(Processor.stack[1], Memory.ReadMDS(Processor.LF, n + 1));
	}

	// 055 PL0
	@Test
	public void testPL0() {
		int n = 0;
		writeOpcode((byte)OpcodeInfo.PL0.code);
		Processor.SP = 1;
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Processor.stack[0], Memory.ReadMDS(Processor.LF, n));
	}

	// 056 PL1
	@Test
	public void testPL1() {
		int n = 1;
		writeOpcode((byte)OpcodeInfo.PL1.code);
		Processor.SP = 1;
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Processor.stack[0], Memory.ReadMDS(Processor.LF, n));
	}

	// 057 PL2
	@Test
	public void testPL2() {
		int n = 2;
		writeOpcode((byte)OpcodeInfo.PL2.code);
		Processor.SP = 1;
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Processor.stack[0], Memory.ReadMDS(Processor.LF, n));
	}

	// 060 PL3
	@Test
	public void testPL3() {
		int n = 3;
		writeOpcode((byte)OpcodeInfo.PL3.code);
		Processor.SP = 1;
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Processor.stack[0], Memory.ReadMDS(Processor.LF, n));
	}

	// 061 PLB
	@Test
	public void testPLB() {
		int n = 20;
		writeOpcode((byte)OpcodeInfo.PLB.code, (byte)n);
		Processor.SP = 1;
		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Processor.stack[0], Memory.ReadMDS(Processor.LF, n));
	}

	// 062 PLD0
	@Test
	public void testPLD0() {
		int n = 0;
		writeOpcode((byte)OpcodeInfo.PLD0.code);
		Processor.SP = 2;
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(2, Processor.SP);
		assertEquals(Processor.stack[0], Memory.ReadMDS(Processor.LF, n + 0));
		assertEquals(Processor.stack[1], Memory.ReadMDS(Processor.LF, n + 1));
	}
	
	// 063 PLDB
	@Test
	public void testPLDB() {
		int n = 20;
		writeOpcode((byte)OpcodeInfo.PLDB.code, (byte)n);
		Processor.SP = 2;
		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(2, Processor.SP);
		assertEquals(Processor.stack[0], Memory.ReadMDS(Processor.LF, n + 0));
		assertEquals(Processor.stack[1], Memory.ReadMDS(Processor.LF, n + 1));
	}

	// 064 LG0
	@Test
	public void testLG0() {
		int n = 0;
		writeOpcode((byte)OpcodeInfo.LG0.code);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.Read(Processor.GF + n), Processor.stack[0]);
	}

	// 065 LG1
	@Test
	public void testLG1() {
		int n = 1;
		writeOpcode((byte)OpcodeInfo.LG1.code);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.Read(Processor.GF + n), Processor.stack[0]);
	}

	// 066 LG2
	@Test
	public void testLG2() {
		int n = 2;
		writeOpcode((byte)OpcodeInfo.LG2.code);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.Read(Processor.GF + n), Processor.stack[0]);
	}

	// 067 LGB
	@Test
	public void testLGB() {
		int n = 20;
		writeOpcode((byte)OpcodeInfo.LGB.code, (byte)n);
		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.Read(Processor.GF + n), Processor.stack[0]);
	}

	// 070 LGD0
	@Test
	public void testLGD0() {
		int n = 0;
		writeOpcode((byte)OpcodeInfo.LGD0.code);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(2, Processor.SP);
		assertEquals(Memory.Read(Processor.GF + n + 0), Processor.stack[0]);
		assertEquals(Memory.Read(Processor.GF + n + 1), Processor.stack[1]);
	}
	
	// 071 LGD2
	@Test
	public void testLGD2() {
		int n = 2;
		writeOpcode((byte)OpcodeInfo.LGD2.code);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(2, Processor.SP);
		assertEquals(Memory.Read(Processor.GF + n + 0), Processor.stack[0]);
		assertEquals(Memory.Read(Processor.GF + n + 1), Processor.stack[1]);
	}
	
	// 072 LGDB
	@Test
	public void testLGDB() {
		int n = 20;
		writeOpcode((byte)OpcodeInfo.LGDB.code, (byte)n);
		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(2, Processor.SP);
		assertEquals(Memory.Read(Processor.GF + n + 0), Processor.stack[0]);
		assertEquals(Memory.Read(Processor.GF + n + 1), Processor.stack[1]);
	}
	
	// 073 SGB
	@Test
	public void testSGB() {
		int n = 20;
		writeOpcode((byte)OpcodeInfo.SGB.code, (byte)n);
		Processor.SP = 1;
		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(Processor.stack[0], Memory.Read(Processor.GF + n));
	}

	// 074 BNDCK
	@Test
	public void testBNDCK_success() {
		short index = 10;
		short range = 20;
		writeOpcode((byte)OpcodeInfo.BNDCK.code);
		Processor.Push(index);
		Processor.Push(range);
		Opcode.execute();
		
		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Processor.stack[0], index);
	}
	@Test
	public void testBNDCK_fail() {
		short index = 20;
		short range = 10;
		writeOpcode((byte)OpcodeInfo.BNDCK.code);
		Processor.Push(index);
		Processor.Push(range);
		
		boolean aborted = false;
		try {
			Opcode.execute();
		} catch (MesaAbort e) {
			aborted = true;
		}
		
		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Processor.stack[0], index);
		assertEquals(true, aborted);
	}
	
	// 075 BRK
	@Test
	public void testBRK_0() {
		writeOpcode((byte)OpcodeInfo.BRK.code);
		Processor.breakByte = 0;
		
		boolean aborted = false;
		try {
			Opcode.execute();
		} catch (MesaAbort e) {
			aborted = true;
		}
		
		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(true, aborted);
		assertEquals(Processor.breakByte, 0);
	}
	@Test
	public void testBRK_LL0() {
		writeOpcode((byte)OpcodeInfo.BRK.code);
		Processor.breakByte = OpcodeInfo.LL0.code;
		
		Opcode.execute();
		
		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.ReadMDS(Processor.LF, 0), Processor.stack[0]);
		assertEquals(Processor.breakByte, 0);
	}
}
