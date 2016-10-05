package mh.majuro.test;

import static org.junit.Assert.assertEquals;
import mh.majuro.mesa.Memory;
import mh.majuro.mesa.Opcode;
import mh.majuro.mesa.OpcodeInfo;
import mh.majuro.mesa.Processor;
import mh.majuro.mesa.Type;
import mh.majuro.mesa.Util;

import org.junit.Test;

public class Opcode1xxTest extends TestBase {
    // 0100 R0
	@Test
	public void testR0() {
		int n = 0;
		writeOpcode((byte)OpcodeInfo.R0.code);
		short ptr = 100;
		Processor.Push(ptr);

		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.ReadMDS(ptr, n), Processor.stack[0]);
	}
	
    // 0101 R1
	@Test
	public void testR1() {
		int n = 1;
		writeOpcode((byte)OpcodeInfo.R1.code);
		short ptr = 100;
		Processor.Push(ptr);

		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.ReadMDS(ptr, n), Processor.stack[0]);
	}
	
    // 0102 RB
	@Test
	public void testRB() {
		int n = 20;
		writeOpcode((byte)OpcodeInfo.RB.code, (byte)n);
		short ptr = 100;
		Processor.Push(ptr);

		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.ReadMDS(ptr, n), Processor.stack[0]);
	}

    // 0103 RL0
	@Test
	public void testRL0() {
		int n = 0;
		writeOpcode((byte)OpcodeInfo.RL0.code);
		int ptr = Processor.CB;
		Processor.PushLong(ptr);

		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.Read(ptr + n), Processor.stack[0]);
	}

    // 0104 RLB
	@Test
	public void testRLB() {
		int n = 20;
		writeOpcode((byte)OpcodeInfo.RLB.code, (byte)n);
		int ptr = Processor.CB;
		Processor.PushLong(ptr);

		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.Read(ptr + n), Processor.stack[0]);
	}

    // 0105 RD0
	@Test
	public void testRD0() {
		int n = 0;
		writeOpcode((byte)OpcodeInfo.RD0.code);
		short ptr = 100;
		Processor.Push(ptr);

		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(2, Processor.SP);
		assertEquals(Memory.ReadMDS(ptr, n + 0), Processor.stack[0]);
		assertEquals(Memory.ReadMDS(ptr, n + 1), Processor.stack[1]);
	}

	// 0106 RDB
	@Test
	public void testRDB() {
		int n = 20;
		writeOpcode((byte)OpcodeInfo.RDB.code, (byte)n);
		short ptr = 100;
		Processor.Push(ptr);

		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(2, Processor.SP);
		assertEquals(Memory.ReadMDS(ptr, n + 0), Processor.stack[0]);
		assertEquals(Memory.ReadMDS(ptr, n + 1), Processor.stack[1]);
	}

    // 0107 RDL0
	@Test
	public void testRDL0() {
		int n = 0;
		writeOpcode((byte)OpcodeInfo.RDL0.code);
		int ptr = Processor.CB;
		Processor.PushLong(ptr);

		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(2, Processor.SP);
		assertEquals(Memory.Read(ptr + n + 0), Processor.stack[0]);
		assertEquals(Memory.Read(ptr + n + 1), Processor.stack[1]);
	}

	// 0110 RDLB
	@Test
	public void testRDLB() {
		int n = 20;
		writeOpcode((byte)OpcodeInfo.RDLB.code, (byte)n);
		int ptr = Processor.CB;
		Processor.PushLong(ptr);

		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(2, Processor.SP);
		assertEquals(Memory.Read(ptr + n + 0), Processor.stack[0]);
		assertEquals(Memory.Read(ptr + n + 1), Processor.stack[1]);
	}

	// 0111 W0
	@Test
	public void testW0() {
		int n = 0;
		writeOpcode((byte)OpcodeInfo.W0.code);
		short ptr = 100;
		short value = (short)0xcafe;
		Processor.Push(value);
		Processor.Push(ptr);

		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(Memory.ReadMDS(ptr, n), value);
	}
	
    // 0112 WB
	@Test
	public void testWB() {
		int n = 20;
		writeOpcode((byte)OpcodeInfo.WB.code, (byte)n);
		short ptr = 100;
		short value = (short)0xcafe;
		Processor.Push(value);
		Processor.Push(ptr);

		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(Memory.ReadMDS(ptr, n), value);
	}
	
    // 0113 PSB
	@Test
	public void testPSB() {
		int n = 20;
		writeOpcode((byte)OpcodeInfo.PSB.code, (byte)n);
		short ptr = 100;
		short value = (short)0xcafe;
		Processor.Push(ptr);
		Processor.Push(value);

		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(ptr, Processor.stack[0]);
		assertEquals(Memory.ReadMDS(ptr, n), value);
	}
	
    // 0114 WLB
	@Test
	public void testWLB() {
		int n = 20;
		writeOpcode((byte)OpcodeInfo.WLB.code, (byte)n);
		int ptr = Processor.CB;
		short value = (short)0xcafe;
		Processor.Push(value);
		Processor.PushLong(ptr);

		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(Memory.Read(ptr + n), value);
	}
	
    // 0115 PSLB
	@Test
	public void testPSLB() {
		int n = 20;
		writeOpcode((byte)OpcodeInfo.PSLB.code, (byte)n);
		int ptr = Processor.CB;
		short value = (short)0xcafe;
		Processor.PushLong(ptr);
		Processor.Push(value);

		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(2, Processor.SP);
		assertEquals(Util.LowHalf(ptr),  Processor.stack[0]);
		assertEquals(Util.HighHalf(ptr), Processor.stack[1]);
		assertEquals(Memory.Read(ptr + n), value);
	}
	
    // 0116 WDB
	@Test
	public void testWDB() {
		int n = 20;
		writeOpcode((byte)OpcodeInfo.WDB.code, (byte)n);
		short ptr = 20;
		short valueu = (short)9001;
		short valuev = (short)9002;
		Processor.Push(valueu);
		Processor.Push(valuev);
		Processor.Push(ptr);

		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(valuev, Memory.ReadMDS(ptr, n + 1));
		assertEquals(valueu, Memory.ReadMDS(ptr, n + 0));
	}
	
    // 0117 PSD0
	@Test
	public void testPSD0() {
		int n = 0;
		writeOpcode((byte)OpcodeInfo.PSD0.code);
		short ptr = 20;
		short valueu = (short)9001;
		short valuev = (short)9002;
		Processor.Push(ptr);
		Processor.Push(valueu);
		Processor.Push(valuev);

		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(ptr,    Processor.stack[0]);
		assertEquals(valuev, Memory.ReadMDS(ptr, n + 1));
		assertEquals(valueu, Memory.ReadMDS(ptr, n + 0));
	}
	
    // 0120 PSDB
	@Test
	public void testPSDB() {
		int n = 30;
		writeOpcode((byte)OpcodeInfo.PSDB.code, (byte)n);
		short ptr = 20;
		short valueu = (short)9001;
		short valuev = (short)9002;
		Processor.Push(ptr);
		Processor.Push(valueu);
		Processor.Push(valuev);

		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(ptr,    Processor.stack[0]);
		assertEquals(valuev, Memory.ReadMDS(ptr, n + 1));
		assertEquals(valueu, Memory.ReadMDS(ptr, n + 0));
	}
	
    // 0121 WDLB
	@Test
	public void testWDLB() {
		int n = 20;
		writeOpcode((byte)OpcodeInfo.WDLB.code, (byte)n);
		int ptr = Processor.CB;
		short valueu = (short)9001;
		short valuev = (short)9002;
		Processor.Push(valueu);
		Processor.Push(valuev);
		Processor.PushLong(ptr);

		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(valuev, Memory.Read(ptr + n + 1));
		assertEquals(valueu, Memory.Read(ptr + n + 0));
	}
	
    // 0122 PSDLB
	@Test
	public void testPSDLB() {
		int n = 20;
		writeOpcode((byte)OpcodeInfo.PSDLB.code, (byte)n);
		int ptr = Processor.CB;
		short valueu = (short)9001;
		short valuev = (short)9002;
		Processor.PushLong(ptr);
		Processor.Push(valueu);
		Processor.Push(valuev);

		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(2, Processor.SP);
		assertEquals(Util.LowHalf(ptr),    Processor.stack[0]);
		assertEquals(Util.HighHalf(ptr),   Processor.stack[1]);
		assertEquals(valuev, Memory.Read(ptr + n + 1));
		assertEquals(valueu, Memory.Read(ptr + n + 0));
	}
	
    // 0123 RLI00
	@Test
	public void testRLI00() {
		int n = 0;
		writeOpcode((byte)OpcodeInfo.RLI00.code);
		short ptr = 20;
		Memory.WriteMDS(Processor.LF, ptr);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.ReadMDS(ptr, n), Processor.stack[0]);
	}

    // 0124 RLI01
	@Test
	public void testRLI01() {
		int n = 1;
		writeOpcode((byte)OpcodeInfo.RLI01.code);
		short ptr = 20;
		Memory.WriteMDS(Processor.LF, ptr);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.ReadMDS(ptr, n), Processor.stack[0]);
	}

    // 0125 RLI02
	@Test
	public void testRLI02() {
		int n = 2;
		writeOpcode((byte)OpcodeInfo.RLI02.code);
		short ptr = 20;
		Memory.WriteMDS(Processor.LF, ptr);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.ReadMDS(ptr, n), Processor.stack[0]);
	}

    // 0126 RLI03
	@Test
	public void testRLI03() {
		int n = 3;
		writeOpcode((byte)OpcodeInfo.RLI03.code);
		short ptr = 20;
		Memory.WriteMDS(Processor.LF, ptr);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.ReadMDS(ptr, n), Processor.stack[0]);
	}

    // 0127 RLIP
	@Test
	public void testRLIP() {
		int left  = 3;
		int right = 4;
		writeOpcode((byte)OpcodeInfo.RLIP.code, Util.asByte(left, right));
		short ptr = 20;
		Memory.WriteMDS(Processor.LF, left, ptr);
		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.ReadMDS(ptr, right), Processor.stack[0]);
	}

    // 0130 RLILP
	@Test
	public void testRLILP() {
		int left  = 3;
		int right = 4;
		writeOpcode((byte)OpcodeInfo.RLILP.code, Util.asByte(left, right));
		int ptr = Processor.CB;
		Memory.WriteMDS(Processor.LF, left + 0, Util.LowHalf(ptr));
		Memory.WriteMDS(Processor.LF, left + 1, Util.HighHalf(ptr));
		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.Read(ptr + right), Processor.stack[0]);
	}

	// 0131 RLDI00
	@Test
	public void testRLDI00() {
		writeOpcode((byte)OpcodeInfo.RLDI00.code);
		short ptr = 20;
		short valueu = (short)9001;
		short valuev = (short)9002;
		Memory.WriteMDS(Processor.LF, 0, ptr);
		Memory.WriteMDS(ptr, 0, valueu);
		Memory.WriteMDS(ptr, 1, valuev);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(2, Processor.SP);
		assertEquals(valueu, Processor.stack[0]);
		assertEquals(valuev, Processor.stack[1]);
	}

    // 0132 RLDIP
	@Test
	public void testRLDIP() {
		int left  = 3;
		int right = 4;
		writeOpcode((byte)OpcodeInfo.RLDIP.code, Util.asByte(left, right));
		short ptr = 20;
		short valueu = (short)9001;
		short valuev = (short)9002;
		Memory.WriteMDS(Processor.LF, left, ptr);
		Memory.WriteMDS(ptr, right + 0, valueu);
		Memory.WriteMDS(ptr, right + 1, valuev);
		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(2, Processor.SP);
		assertEquals(valueu, Processor.stack[0]);
		assertEquals(valuev, Processor.stack[1]);
	}

	// 0133 RLDILP
	@Test
	public void testRLDILP() {
		int left  = 3;
		int right = 4;
		writeOpcode((byte)OpcodeInfo.RLDILP.code, Util.asByte(left, right));
		int   ptr = Processor.CB;
		short valueu = (short)9001;
		short valuev = (short)9002;
		Memory.WriteMDS(Processor.LF, left + 0, Util.LowHalf(ptr));
		Memory.WriteMDS(Processor.LF, left + 1, Util.HighHalf(ptr));
		Memory.Write(ptr + right + 0, valueu);
		Memory.Write(ptr + right + 1, valuev);
		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(2, Processor.SP);
		assertEquals(valueu, Processor.stack[0]);
		assertEquals(valuev, Processor.stack[1]);
	}

    // 0134 RGIP
	@Test
	public void testRGIP() {
		int left  = 3;
		int right = 4;
		writeOpcode((byte)OpcodeInfo.RGIP.code, Util.asByte(left, right));
		short ptr = 20;
		Memory.Write(Processor.GF + left, ptr);
		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.ReadMDS(ptr, right), Processor.stack[0]);
	}

    // 0135 RGILP
	@Test
	public void testRGILP() {
		int left  = 3;
		int right = 4;
		writeOpcode((byte)OpcodeInfo.RGILP.code, Util.asByte(left, right));
		int ptr = Processor.CB;
		Memory.Write(Processor.GF + left + 0, Util.LowHalf(ptr));
		Memory.Write(Processor.GF + left + 1, Util.HighHalf(ptr));
		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.Read(ptr + right), Processor.stack[0]);
	}

    // 0136 WLIP
	@Test
	public void testWLIP() {
		int left  = 3;
		int right = 4;
		writeOpcode((byte)OpcodeInfo.WLIP.code, Util.asByte(left, right));
		short ptr = 20;
		Memory.WriteMDS(Processor.LF, left, ptr);
		short value = (short)0xcafe;
		Processor.Push(value);

		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(Memory.ReadMDS(ptr, right), value);
	}
	
    // 0137 WLILP
	@Test
	public void testWLILP() {
		int left  = 3;
		int right = 4;
		writeOpcode((byte)OpcodeInfo.WLILP.code, Util.asByte(left, right));
		int ptr = Processor.CB;
		Memory.WriteMDS(Processor.LF, left + 0, Util.LowHalf(ptr));
		Memory.WriteMDS(Processor.LF, left + 1, Util.HighHalf(ptr));
		short value = (short)0xcafe;
		Processor.Push(value);

		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(value, Memory.Read(ptr + right));
	}
	
    // 0140 WLDILP
	@Test
	public void testWLDILP() {
		int left  = 3;
		int right = 4;
		writeOpcode((byte)OpcodeInfo.WLDILP.code, Util.asByte(left, right));
		int ptr = Processor.CB;
		Memory.WriteMDS(Processor.LF, left + 0, Util.LowHalf(ptr));
		Memory.WriteMDS(Processor.LF, left + 1, Util.HighHalf(ptr));
		short valueu = (short)9001;
		short valuev = (short)9002;
		Processor.Push(valueu);
		Processor.Push(valuev);

		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(valuev, Memory.Read(ptr + right + 1));
		assertEquals(valueu, Memory.Read(ptr + right + 0));
	}

	// 0141 RS
	@Test
	public void testRS_even() {
		int n = 0;
		writeOpcode((byte)OpcodeInfo.RS.code, (byte)n);
		short value = (short)0xcafe;
		short index = 10;
		short ptr   = 20;
		Memory.WriteMDS(ptr, (n + index) / 2, value);
		Processor.Push(ptr);
		Processor.Push(index);

		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(0xca, Processor.stack[0]);
	}
	@Test
	public void testRS_odd() {
		int n = 1;
		writeOpcode((byte)OpcodeInfo.RS.code, (byte)n);
		short value = (short)0xcafe;
		short index = 10;
		short ptr   = 20;
		Memory.WriteMDS(ptr, (n + index) / 2, value);
		Processor.Push(ptr);
		Processor.Push(index);

		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(0xfe, Processor.stack[0]);
	}

    // 0142 RLS
	@Test
	public void testRLS_even() {
		int n = 0;
		writeOpcode((byte)OpcodeInfo.RLS.code, (byte)n);
		short value = (short)0xcafe;
		short index = 10;
		int   ptr   = Processor.CB;
		Memory.Write(ptr + (n + index) / 2, value);
		Processor.PushLong(ptr);
		Processor.Push(index);

		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(0xca, Processor.stack[0]);
	}
	@Test
	public void testRLS_odd() {
		int n = 1;
		writeOpcode((byte)OpcodeInfo.RLS.code, (byte)n);
		short value = (short)0xcafe;
		short index = 10;
		int   ptr   = Processor.CB;
		Memory.Write(ptr + (n + index) / 2, value);
		Processor.PushLong(ptr);
		Processor.Push(index);

		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(0xfe, Processor.stack[0]);
	}

	// 0143 WS
	@Test
	public void testWS_even() {
		int n = 0;
		writeOpcode((byte)OpcodeInfo.WS.code, (byte)n);
		short value = (short)0x34;
		short index = 10;
		short ptr   = 20;
		Processor.Push(value);
		Processor.Push(ptr);
		Processor.Push(index);

		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(value, Util.HighByte(Memory.ReadMDS(ptr, (index + n) / 2)));
	}
	@Test
	public void testWS_odd() {
		int n = 1;
		writeOpcode((byte)OpcodeInfo.WS.code, (byte)n);
		short value = (short)0x34;
		short index = 10;
		short ptr   = 20;
		Processor.Push(value);
		Processor.Push(ptr);
		Processor.Push(index);

		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(value, Util.LowByte(Memory.ReadMDS(ptr, (index + n) / 2)));
	}

    // 0144 WLS
	@Test
	public void testWLS_even() {
		int n = 0;
		writeOpcode((byte)OpcodeInfo.WLS.code, (byte)n);
		short value = (short)0x34;
		short index = 10;
		int   ptr   = Processor.CB;
		Processor.Push(value);
		Processor.PushLong(ptr);
		Processor.Push(index);

		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(value, Util.HighByte(Memory.Read(ptr + (index + n) / 2)));
	}
	@Test
	public void testWLS_odd() {
		int n = 1;
		writeOpcode((byte)OpcodeInfo.WLS.code, (byte)n);
		short value = (short)0x34;
		short index = 10;
		int   ptr   = Processor.CB;
		Processor.Push(value);
		Processor.PushLong(ptr);
		Processor.Push(index);
		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(value, Util.LowByte(Memory.Read(ptr + (index + n) / 2)));
	}

	// 0145 R0F
	@Test
	public void testR0F() {
		byte alpha = 0;
		alpha = Type.FieldSpec.pos.setData(alpha, 4);
		alpha = Type.FieldSpec.size.setData(alpha, 7); // 7 means 8 bits

		writeOpcode((byte)OpcodeInfo.R0F.code, alpha);
		short ptr = 20;
		Memory.WriteMDS(ptr, (short)0x1234);
		Processor.Push(ptr);

		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals((short)0x23, Processor.stack[0]);
	}
	
    // 0146 RF
	@Test
	public void testRF() {
		int offset = 20;
		byte field = 0;
		field = Type.FieldSpec.pos.setData(field, 4);
		field = Type.FieldSpec.size.setData(field, 7); // 7 means 8 bits
		short desc = 0;
		desc = Type.FieldDesc.field.setData(desc, field);
		desc = Type.FieldDesc.offset.setData(desc, offset);

		writeOpcode((byte)OpcodeInfo.RF.code, (byte)Util.HighByte(desc), (byte)Util.LowByte(desc));
		short ptr = 20;
		Memory.WriteMDS(ptr, offset, (short)0x1234);
		Processor.Push(ptr);

		Opcode.execute();

		assertEquals(Processor.savedPC + 3, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals((short)0x23, Processor.stack[0]);
	}
	
    // 0147 RL0F
	@Test
	public void testRL0F() {
		byte alpha = 0;
		alpha = Type.FieldSpec.pos.setData(alpha, 4);
		alpha = Type.FieldSpec.size.setData(alpha, 7); // 7 means 8 bits

		writeOpcode((byte)OpcodeInfo.RL0F.code, alpha);
		int ptr = Processor.CB;
		Memory.Write(ptr, (short)0x1234);
		Processor.PushLong(ptr);

		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals((short)0x23, Processor.stack[0]);
	}

	// 0150 RLF
	@Test
	public void testRLF() {
		int offset = 20;
		byte field = 0;
		field = Type.FieldSpec.pos.setData(field, 4);
		field = Type.FieldSpec.size.setData(field, 7); // 7 means 8 bits
		short desc = 0;
		desc = Type.FieldDesc.field.setData(desc, field);
		desc = Type.FieldDesc.offset.setData(desc, offset);

		writeOpcode((byte)OpcodeInfo.RLF.code, (byte)Util.HighByte(desc), (byte)Util.LowByte(desc));
		int ptr = Processor.CB;
		Memory.Write(ptr + offset, (short)0x1234);
		Processor.PushLong(ptr);

		Opcode.execute();

		assertEquals(Processor.savedPC + 3, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals((short)0x23, Processor.stack[0]);
	}
	
    // 0151 RLFS
	@Test
	public void testRLFS() {
		int offset = 20;
		byte field = 0;
		field = Type.FieldSpec.pos.setData(field, 4);
		field = Type.FieldSpec.size.setData(field, 7); // 7 means 8 bits
		short desc = 0;
		desc = Type.FieldDesc.field.setData(desc, field);
		desc = Type.FieldDesc.offset.setData(desc, offset);

		writeOpcode((byte)OpcodeInfo.RLFS.code);
		int ptr = Processor.CB;
		Memory.Write(ptr + offset, (short)0x1234);
		Processor.PushLong(ptr);
		Processor.Push(desc);

		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals((short)0x23, Processor.stack[0]);
	}

	// 0152 RLIPF
	@Test
	public void testRLIPF() {
		int left   = 3;
		int right  = 4;
		byte field = 0;
		field = Type.FieldSpec.pos.setData(field, 4);
		field = Type.FieldSpec.size.setData(field, 7); // 7 means 8 bits

		writeOpcode((byte)OpcodeInfo.RLIPF.code, Util.asByte(left, right), field);
		short ptr = 20;
		Memory.WriteMDS(Processor.LF, left, ptr);
		Memory.WriteMDS(ptr, right, (short)0x1234);

		Opcode.execute();

		assertEquals(Processor.savedPC + 3, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals((short)0x23, Processor.stack[0]);
	}

	// 0153 RLILPF
	@Test
	public void testRLILPF() {
		int left   = 3;
		int right  = 4;
		byte field = 0;
		field = Type.FieldSpec.pos.setData(field, 4);
		field = Type.FieldSpec.size.setData(field, 7); // 7 means 8 bits

		writeOpcode((byte)OpcodeInfo.RLILPF.code, Util.asByte(left, right), field);
		int ptr = Processor.CB;
		Memory.WriteMDS(Processor.LF, left + 0, Util.LowHalf(ptr));
		Memory.WriteMDS(Processor.LF, left + 1, Util.HighHalf(ptr));
		Memory.Write(ptr + right, (short)0x1234);

		Opcode.execute();

		assertEquals(Processor.savedPC + 3, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals((short)0x23, Processor.stack[0]);
	}

	// 0154 W0F
	@Test
	public void testW0F() {
		byte field = 0;
		field = Type.FieldSpec.pos.setData(field, 4);
		field = Type.FieldSpec.size.setData(field, 7); // 7 means 8 bits
		writeOpcode((byte)OpcodeInfo.W0F.code, field);
		short value = (short)0x34;
		short ptr   = 20;
		Processor.Push(value);
		Processor.Push(ptr);

		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(0, Processor.SP);
		short newValue = Memory.ReadMDS(ptr);
		assertEquals(0x0340, newValue & 0x0ff0);
	}

    // 0155 WF
	@Test
	public void testWF() {
		int offset = 20;
		byte field = 0;
		field = Type.FieldSpec.pos.setData(field, 4);
		field = Type.FieldSpec.size.setData(field, 7); // 7 means 8 bits
		short desc = 0;
		desc = Type.FieldDesc.field.setData(desc, field);
		desc = Type.FieldDesc.offset.setData(desc, offset);

		writeOpcode((byte)OpcodeInfo.WF.code, (byte)Util.HighByte(desc), (byte)Util.LowByte(desc));
		short value = (short)0x34;
		short ptr   = 20;
		Processor.Push(value);
		Processor.Push(ptr);

		Opcode.execute();

		assertEquals(Processor.savedPC + 3, Processor.PC);
		assertEquals(0, Processor.SP);
		short newValue = Memory.ReadMDS(ptr, offset);
		assertEquals(0x0340, newValue & 0x0ff0);
	}

    // 0156 PSF
	@Test
	public void testPSF() {
		int offset = 20;
		byte field = 0;
		field = Type.FieldSpec.pos.setData(field, 4);
		field = Type.FieldSpec.size.setData(field, 7); // 7 means 8 bits
		short desc = 0;
		desc = Type.FieldDesc.field.setData(desc, field);
		desc = Type.FieldDesc.offset.setData(desc, offset);

		writeOpcode((byte)OpcodeInfo.PSF.code, (byte)Util.HighByte(desc), (byte)Util.LowByte(desc));
		short value = (short)0x34;
		short ptr   = 20;
		Processor.Push(ptr);
		Processor.Push(value);

		Opcode.execute();

		assertEquals(Processor.savedPC + 3, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(ptr, Processor.stack[0]);
		short newValue = Memory.ReadMDS(ptr, offset);
		assertEquals(0x0340, newValue & 0x0ff0);
	}

    // 0157 PS0F
	@Test
	public void testPS0F() {
		byte field = 0;
		field = Type.FieldSpec.pos.setData(field, 4);
		field = Type.FieldSpec.size.setData(field, 7); // 7 means 8 bits

		writeOpcode((byte)OpcodeInfo.PS0F.code, field);
		short value = (short)0x34;
		short ptr   = 20;
		Processor.Push(ptr);
		Processor.Push(value);

		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(ptr, Processor.stack[0]);
		short newValue = Memory.ReadMDS(ptr);
		assertEquals(0x0340, newValue & 0x0ff0);
	}

    // 0160 WS0F
	@Test
	public void testWS0F() {
		byte field = 0;
		field = Type.FieldSpec.pos.setData(field, 4);
		field = Type.FieldSpec.size.setData(field, 7); // 7 means 8 bits

		writeOpcode((byte)OpcodeInfo.WS0F.code, field);
		short value = (short)0x34;
		short ptr   = 20;
		Processor.Push(ptr);
		Processor.Push(value);

		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(0, Processor.SP);
		short newValue = Memory.ReadMDS(ptr);
		assertEquals(0x0340, newValue & 0x0ff0);
	}

    // 0161 WL0F
	@Test
	public void testWL0F() {
		byte field = 0;
		field = Type.FieldSpec.pos.setData(field, 4);
		field = Type.FieldSpec.size.setData(field, 7); // 7 means 8 bits

		writeOpcode((byte)OpcodeInfo.WL0F.code, field);
		short value = (short)0x34;
		int   ptr   = Processor.CB;
		Processor.Push(value);
		Processor.PushLong(ptr);

		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(0, Processor.SP);
		short newValue = Memory.Read(ptr);
		assertEquals(0x0340, newValue & 0x0ff0);
	}

    // 0162 WLF
	@Test
	public void testWLF() {
		int offset = 20;
		byte field = 0;
		field = Type.FieldSpec.pos.setData(field, 4);
		field = Type.FieldSpec.size.setData(field, 7); // 7 means 8 bits
		short desc = 0;
		desc = Type.FieldDesc.field.setData(desc, field);
		desc = Type.FieldDesc.offset.setData(desc, offset);

		writeOpcode((byte)OpcodeInfo.WLF.code, (byte)Util.HighByte(desc), (byte)Util.LowByte(desc));
		short value = (short)0x34;
		int   ptr   = Processor.CB;
		Processor.Push(value);
		Processor.PushLong(ptr);

		Opcode.execute();

		assertEquals(Processor.savedPC + 3, Processor.PC);
		assertEquals(0, Processor.SP);
		short newValue = Memory.Read(ptr + offset);
		assertEquals(0x0340, newValue & 0x0ff0);
	}
	
    // 0163 PSLF
	@Test
	public void testPSLF() {
		int offset = 20;
		byte field = 0;
		field = Type.FieldSpec.pos.setData(field, 4);
		field = Type.FieldSpec.size.setData(field, 7); // 7 means 8 bits
		short desc = 0;
		desc = Type.FieldDesc.field.setData(desc, field);
		desc = Type.FieldDesc.offset.setData(desc, offset);

		writeOpcode((byte)OpcodeInfo.PSLF.code, (byte)Util.HighByte(desc), (byte)Util.LowByte(desc));
		short value = (short)0x34;
		int   ptr   = Processor.CB;
		Processor.PushLong(ptr);
		Processor.Push(value);

		Opcode.execute();

		assertEquals(Processor.savedPC + 3, Processor.PC);
		assertEquals(2, Processor.SP);
		assertEquals(Util.LowHalf(ptr),  Processor.stack[0]);
		assertEquals(Util.HighHalf(ptr), Processor.stack[1]);
		short newValue = Memory.Read(ptr + offset);
		assertEquals(0x0340, newValue & 0x0ff0);
	}
	
    // 0164 WLFS
	@Test
	public void testWLFS() {
		int offset = 20;
		byte field = 0;
		field = Type.FieldSpec.pos.setData(field, 4);
		field = Type.FieldSpec.size.setData(field, 7); // 7 means 8 bits
		short desc = 0;
		desc = Type.FieldDesc.field.setData(desc, field);
		desc = Type.FieldDesc.offset.setData(desc, offset);

		writeOpcode((byte)OpcodeInfo.WLFS.code);
		short value = (short)0x34;
		int   ptr   = Processor.CB;
		Processor.Push(value);
		Processor.PushLong(ptr);
		Processor.Push(desc);

		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(0, Processor.SP);
		short newValue = Memory.Read(ptr + offset);
		assertEquals(0x0340, newValue & 0x0ff0);
	}

	// 0165 SLDB
	@Test
	public void testSLDB() {
		int n = 0;
		writeOpcode((byte)OpcodeInfo.SLDB.code, (byte)n);
		Processor.SP = 2;
		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(Processor.stack[0], Memory.ReadMDS(Processor.LF, n + 0));
		assertEquals(Processor.stack[1], Memory.ReadMDS(Processor.LF, n + 1));
	}

	// 0166 SGDB
	@Test
	public void testSGDB() {
		int n = 0;
		writeOpcode((byte)OpcodeInfo.SGDB.code, (byte)n);
		Processor.SP = 2;
		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals(Processor.stack[0], Memory.Read(Processor.GF + n + 0));
		assertEquals(Processor.stack[1], Memory.Read(Processor.GF + n + 1));
	}
	
    // 0167 LLKB
	@Test
	public void testLLKB_code() {
		int alpha = 10;
		writeOpcode((byte)OpcodeInfo.LLKB.code, (byte)alpha);
		
		int link = 0xcafebabe;
		Memory.Write(Processor.GF - 1, (short)0x0001); // codelink = 1
		Memory.Write(Processor.CB - (alpha + 1) * 2 + 0, Util.LowHalf(link));
		Memory.Write(Processor.CB - (alpha + 1) * 2 + 1, Util.HighHalf(link));
		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(2, Processor.SP);
		assertEquals(Util.LowHalf(link),  Processor.stack[0]);
		assertEquals(Util.HighHalf(link), Processor.stack[1]);
	}
	@Test
	public void testLLKB_frame() {
		int alpha = 10;
		writeOpcode((byte)OpcodeInfo.LLKB.code, (byte)alpha);
		
		int link = 0xcafebabe;
		Memory.Write(Processor.GF - 1, (short)0x0000); // codelink = 0
		Memory.Write(Processor.GF -2 -(alpha + 1) * 2 + 0, Util.LowHalf(link));
		Memory.Write(Processor.GF -2 -(alpha + 1) * 2 + 1, Util.HighHalf(link));
		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(2, Processor.SP);
		assertEquals(Util.LowHalf(link),  Processor.stack[0]);
		assertEquals(Util.HighHalf(link), Processor.stack[1]);
	}

    // 0170 RKIB
	@Test
	public void testRKIB_code() {
		int alpha = 10;
		writeOpcode((byte)OpcodeInfo.RKIB.code, (byte)alpha);
		
		int link = Processor.MDS + 10;
		Memory.Write(Processor.GF - 1, (short)0x0001); // codelink = 1
		Memory.Write(Processor.CB - (alpha + 1) * 2 + 0, Util.LowHalf(link));
		Memory.Write(Processor.CB - (alpha + 1) * 2 + 1, Util.HighHalf(link));
		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.Read(link),  Processor.stack[0]);
	}
	@Test
	public void testRKIB_frame() {
		int alpha = 10;
		writeOpcode((byte)OpcodeInfo.RKIB.code, (byte)alpha);
		
		int link = Processor.MDS + 10;
		Memory.Write(Processor.GF - 1, (short)0x0000); // codelink = 0
		Memory.Write(Processor.GF -2 -(alpha + 1) * 2 + 0, Util.LowHalf(link));
		Memory.Write(Processor.GF -2 -(alpha + 1) * 2 + 1, Util.HighHalf(link));
		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals(Memory.Read(link),  Processor.stack[0]);
	}

    // 0171 RKDIB
	@Test
	public void testRKDIB_code() {
		int alpha = 10;
		writeOpcode((byte)OpcodeInfo.RKDIB.code, (byte)alpha);
		
		int link = Processor.MDS + 10;
		Memory.Write(Processor.GF - 1, (short)0x0001); // codelink = 1
		Memory.Write(Processor.CB - (alpha + 1) * 2 + 0, Util.LowHalf(link));
		Memory.Write(Processor.CB - (alpha + 1) * 2 + 1, Util.HighHalf(link));
		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(2, Processor.SP);
		assertEquals(Memory.Read(link + 0),  Processor.stack[0]);
		assertEquals(Memory.Read(link + 1),  Processor.stack[1]);
	}
	@Test
	public void testRKDIB_frame() {
		int alpha = 10;
		writeOpcode((byte)OpcodeInfo.RKDIB.code, (byte)alpha);
		
		int link = Processor.MDS + 10;
		Memory.Write(Processor.GF - 1, (short)0x0000); // codelink = 0
		Memory.Write(Processor.GF -2 -(alpha + 1) * 2 + 0, Util.LowHalf(link));
		Memory.Write(Processor.GF -2 -(alpha + 1) * 2 + 1, Util.HighHalf(link));
		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(2, Processor.SP);
		assertEquals(Memory.Read(link + 0),  Processor.stack[0]);
		assertEquals(Memory.Read(link + 1),  Processor.stack[1]);
	}

    // 0172 LKB
	@Test
	public void testLKB() {
		int alpha = 10;
		writeOpcode((byte)OpcodeInfo.LKB.code, (byte)alpha);
		
		short link = 104;
		Processor.stack[0] = link;
		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(0, Processor.SP);
		assertEquals((short)(link - alpha), Memory.ReadMDS(Processor.LF));
	}
	
    // 0173 SHIFT
	@Test
	public void testSHIFT_p() {
		writeOpcode((byte)OpcodeInfo.SHIFT.code);
		
		short shift = 10;
		short u     = 0x1234;

		Processor.Push((short)u);
		Processor.Push(shift);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals((short)(u << shift), Processor.stack[0]);
	}
	@Test
	public void testSHIFT_m() {
		writeOpcode((byte)OpcodeInfo.SHIFT.code);
		
		short shift = -10;
		short u     = 0x1234;

		Processor.Push((short)u);
		Processor.Push(shift);
		Opcode.execute();

		assertEquals(Processor.savedPC + 1, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals((short)((u & 0xffff) >>> -shift), Processor.stack[0]);
	}

	// 0174 SHIFTSB
	@Test
	public void testSHIFTSB_p() {
		int shift = 10;
		writeOpcode((byte)OpcodeInfo.SHIFTSB.code, (byte)shift);
		
		short u = 0x1234;

		Processor.Push((short)u);
		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals((short)(u << shift), Processor.stack[0]);
	}
	@Test
	public void testSHIFTSB_m() {
		int shift = -10;
		writeOpcode((byte)OpcodeInfo.SHIFTSB.code, (byte)shift);
		
		short u = 0x1234;

		Processor.Push((short)u);
		Opcode.execute();

		assertEquals(Processor.savedPC + 2, Processor.PC);
		assertEquals(1, Processor.SP);
		assertEquals((short)((u & 0xffff) >>> -shift), Processor.stack[0]);
	}
}
