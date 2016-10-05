package mh.majuro.mesa.opcode;

import static mh.majuro.mesa.Debug.TRACE_OPCODE;
import static mh.majuro.mesa.OpcodeInfo.*;
import mh.majuro.mesa.Memory;
import mh.majuro.mesa.MesaAssertionError;
import mh.majuro.mesa.Opcode.RegisterOpcode;
import mh.majuro.mesa.Processor;
import mh.majuro.mesa.Util;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public final class Opcode1xx {
	protected static final Logger logger = LoggerFactory.getLogger(Opcode1xx.class);

	static void Rn(int n) {
		short ptr = Processor.Pop();
		Processor.Push(Memory.ReadMDS(ptr, n));
	}

	// 0100 R0
	@RegisterOpcode(R0)
	public static final class OP_R0 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  R0", Processor.savedPC));
			Rn(0);
		}
	}
	
	// 0101 R1
	@RegisterOpcode(R1)
	public static final class OP_R1 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  R1", Processor.savedPC));
			Rn(1);
		}
	}

	// 0102 RB
	@RegisterOpcode(RB)
	public static final class OP_RB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  RB %02X", Processor.savedPC, alpha));
			Rn(alpha);
		}
	}

	static void RLn(int n) {
		int ptr = Processor.PopLong();
		Processor.Push(Memory.Read(ptr + n));
	}
	
	// 0103 RL0
	@RegisterOpcode(RL0)
	public static final class OP_RL0 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  RL0", Processor.savedPC));
			RLn(0);
		}
	}

	// 0104 RLB
	@RegisterOpcode(RLB)
	public static final class OP_RLB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  RLB %02X", Processor.savedPC, alpha));
			RLn(alpha);
		}
	}

	static void RDn(int n) {
		short ptr = Processor.Pop();
		short u = Memory.ReadMDS(ptr, n + 0);
		short v = Memory.ReadMDS(ptr, n + 1);
		
		Processor.Push(u);
		Processor.Push(v);
	}
	
	// 0105 RD0
	@RegisterOpcode(RD0)
	public static final class OP_RD0 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  RD0", Processor.savedPC));
			RDn(0);
		}
	}

	// 0106 RDB
	@RegisterOpcode(RDB)
	public static final class OP_RDB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  RDB %02X", Processor.savedPC, alpha));
			RDn(alpha);
		}
	}

	static void RDLn(int n) {
		int ptr = Processor.PopLong();
		short u = Memory.Read(ptr + n + 0);
		short v = Memory.Read(ptr + n + 1);
		
		Processor.Push(u);
		Processor.Push(v);
	}

	// 0107 RDL0
	@RegisterOpcode(RDL0)
	public static final class OP_RDL0 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  RDL0", Processor.savedPC));
			RDLn(0);
		}
	}

	// 0110 RDLB
	@RegisterOpcode(RDLB)
	public static final class OP_RDLB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  RDLB %02X", Processor.savedPC, alpha));
			RDLn(alpha);
		}
	}
	
	static void Wn(int n) {
		short ptr = Processor.Pop();
		Memory.WriteMDS(ptr, n, Processor.Pop());
	}

	// 0111 W0
	@RegisterOpcode(W0)
	public static final class OP_W0 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  W0", Processor.savedPC));
			Wn(0);
		}
	}

	// 0112 WB
	@RegisterOpcode(WB)
	public static final class OP_WB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  WB %02X", Processor.savedPC, alpha));
			Wn(alpha);
		}
	}

	// 0113 PSB
	@RegisterOpcode(PSB)
	public static final class OP_PSB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  PSB %02X", Processor.savedPC, alpha));
			
			short u   = Processor.Pop();
			short ptr = Processor.Pop();
			Memory.WriteMDS(ptr, alpha, u);
			//Processor.Recover();
			Processor.SP++;
		}
	}

	// 0114 WLB
	@RegisterOpcode(WLB)
	public static final class OP_WLB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  WLB %02X", Processor.savedPC, alpha));
			
			int ptr = Processor.PopLong();
			Memory.Write(ptr + alpha, Processor.Pop());
		}
	}

	// 0115 PSLB
	@RegisterOpcode(PSLB)
	public static final class OP_PSLB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  PSLB %02X", Processor.savedPC, alpha));
			
			short u = Processor.Pop();
			int ptr = Processor.PopLong();
			Memory.Write(ptr + alpha, u);
			//Processor.Recover();
			//Processor.Recover();
			Processor.SP += 2;
		}
	}

	// 0116 WDB
	@RegisterOpcode(WDB)
	public static final class OP_WDB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  WDB %02X", Processor.savedPC, alpha));

			short ptr = Processor.Pop();
			Memory.WriteMDS(ptr, alpha + 1, Processor.Pop());
			Memory.WriteMDS(ptr, alpha + 0, Processor.Pop());
		}
	}

	static void PSDn(int n) {
		short v = Processor.Pop();
		short u = Processor.Pop();
		short ptr = Processor.Pop();
		
		Memory.WriteMDS(ptr, n + 1, v);
		Memory.WriteMDS(ptr, n + 0, u);
		
		// Processor.Recover();
		Processor.SP++;
	}
	
	// 0117 PSD0
	@RegisterOpcode(PSD0)
	public static final class OP_PSD0 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  PSD0", Processor.savedPC));
			PSDn(0);
		}
	}

	// 0120 PSDB
	@RegisterOpcode(PSDB)
	public static final class OP_PSDB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  PSDB %02X", Processor.savedPC, alpha));
			PSDn(alpha);
		}
	}

	// 0121 WDLB
	@RegisterOpcode(WDLB)
	public static final class OP_WDLB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  WDLB %02X", Processor.savedPC, alpha));
			
			int ptr = Processor.PopLong();
			Memory.Write(ptr + alpha + 1, Processor.Pop());
			Memory.Write(ptr + alpha + 0, Processor.Pop());
		}
	}

	// 0122 PSDLB
	@RegisterOpcode(PSDLB)
	public static final class OP_PSDLB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  PSDLB %02X", Processor.savedPC, alpha));
			
			short v = Processor.Pop();
			short u = Processor.Pop();
			int ptr = Processor.PopLong();
			Memory.Write(ptr + alpha + 1, v);
			Memory.Write(ptr + alpha + 0, u);
			// Processor.Recover();
			// Processor.Recover();
			Processor.SP += 2;
		}
	}

	static void RLI0n(int n) {
		short ptr = Memory.ReadMDS(Processor.LF);
		Processor.Push(Memory.ReadMDS(ptr, n));
	}
	
	// 0123 RLI00
	@RegisterOpcode(RLI00)
	public static final class OP_RLI00 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  RLI00", Processor.savedPC));
			RLI0n(0);
		}
	}

	// 0124 RLI01
	@RegisterOpcode(RLI01)
	public static final class OP_RLI01 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  RLI01", Processor.savedPC));
			RLI0n(1);
		}
	}

	// 0125 RLI02
	@RegisterOpcode(RLI02)
	public static final class OP_RLI02 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  RLI02", Processor.savedPC));
			RLI0n(2);
		}
	}

	// 0126 RLI03
	@RegisterOpcode(RLI03)
	public static final class OP_RLI03 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  RLI03", Processor.savedPC));
			RLI0n(3);
		}
	}

	// 0127 RLIP
	@RegisterOpcode(RLIP)
	public static final class OP_RLIP implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			int left  = Util.LeftNibble(alpha);
			int right = Util.RightNibble(alpha);

			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  RLIP %02X", Processor.savedPC, alpha));
			
			short ptr = Memory.ReadMDS(Processor.LF, left);
			Processor.Push(Memory.ReadMDS(ptr, right));
		}
	}

	// 0130 RLILP
	@RegisterOpcode(RLILP)
	public static final class OP_RLILP implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			int left  = Util.LeftNibble(alpha);
			int right = Util.RightNibble(alpha);

			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  RLIP %02X", Processor.savedPC, alpha));
			
			int ptr = Memory.ReadDblMDS(Processor.LF, left);
			Processor.Push(Memory.Read(ptr + right));
		}
	}
	
	static void RLDInn(int left, int right) {
		short ptr = Memory.ReadMDS(Processor.LF, left);
		short u   = Memory.ReadMDS(ptr, right + 0);
		short v   = Memory.ReadMDS(ptr, right + 1);
		Processor.Push(u);
		Processor.Push(v);
	}

	// 0131 RLDI00
	@RegisterOpcode(RLDI00)
	public static final class OP_RLDI00 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  RLDI00", Processor.savedPC));
			
			RLDInn(0, 0);
		}
	}

	// 0132 RLDIP
	@RegisterOpcode(RLDIP)
	public static final class OP_RLDIP implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			int left  = Util.LeftNibble(alpha);
			int right = Util.RightNibble(alpha);

			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  RLDIP %02X", Processor.savedPC, alpha));
			
			RLDInn(left, right);
		}
	}

	// 0133 RLDILP
	@RegisterOpcode(RLDILP)
	public static final class OP_RLDILP implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			int left  = Util.LeftNibble(alpha);
			int right = Util.RightNibble(alpha);

			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  RLDILP %02X", Processor.savedPC, alpha));
			
			int ptr = Memory.ReadDblMDS(Processor.LF, left);
			short u = Memory.Read(ptr + right + 0);
			short v = Memory.Read(ptr + right + 1);
			Processor.Push(u);
			Processor.Push(v);
		}
	}

	// 0134 RGIP
	@RegisterOpcode(RGIP)
	public static final class OP_RGIP implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			int left  = Util.LeftNibble(alpha);
			int right = Util.RightNibble(alpha);

			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  RGIP %02X", Processor.savedPC, alpha));
			
			short ptr = Memory.Read(Processor.GF + left);
			Processor.Push(Memory.ReadMDS(ptr, right));
		}
	}

	// 0135 RGILP
	@RegisterOpcode(RGILP)
	public static final class OP_RGILP implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			int left  = Util.LeftNibble(alpha);
			int right = Util.RightNibble(alpha);

			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  RGILP %02X", Processor.savedPC, alpha));
			
			int ptr = Memory.ReadDbl(Processor.GF + left);
			Processor.Push(Memory.Read(ptr + right));
		}
	}

	// 0136 WLIP
	@RegisterOpcode(WLIP)
	public static final class OP_WLIP implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			int left  = Util.LeftNibble(alpha);
			int right = Util.RightNibble(alpha);

			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  WLIP %02X", Processor.savedPC, alpha));
			
			short ptr = Memory.ReadMDS(Processor.LF, left);
			Memory.WriteMDS(ptr, right, Processor.Pop());
		}
	}

	// 0137 WLILP
	@RegisterOpcode(WLILP)
	public static final class OP_WLILP implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			int left  = Util.LeftNibble(alpha);
			int right = Util.RightNibble(alpha);

			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  WLILP %02X", Processor.savedPC, alpha));
			
			int ptr = Memory.ReadDblMDS(Processor.LF, left);
			Memory.Write(ptr + right, Processor.Pop());
		}
	}

	// 0140 WLDILP
	@RegisterOpcode(WLDILP)
	public static final class OP_WLDILP implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			int left  = Util.LeftNibble(alpha);
			int right = Util.RightNibble(alpha);

			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  WLDILP %02X", Processor.savedPC, alpha));
			
			int ptr = Memory.ReadDblMDS(Processor.LF, left);
			Memory.Write(ptr + right + 1, Processor.Pop());
			Memory.Write(ptr + right + 0, Processor.Pop());
		}
	}

	// 0141 RS
	@RegisterOpcode(RS)
	public static final class OP_RS implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  RS %02X", Processor.savedPC, alpha));
			
			int index = Util.asInt(Processor.Pop());
			int ptr = Memory.LengthenPointer(Processor.Pop());
			Processor.Push((short)Memory.FetchByte(ptr, alpha + index));
		}
	}

	// 0142 RLS
	@RegisterOpcode(RLS)
	public static final class OP_RLS implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  RLS %02X", Processor.savedPC, alpha));
			
			int index = Util.asInt(Processor.Pop());
			int ptr = Processor.PopLong();
			Processor.Push((short)Memory.FetchByte(ptr, alpha + index));
		}
	}

	// 0143 WS
	@RegisterOpcode(WS)
	public static final class OP_WS implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  WS %02X", Processor.savedPC, alpha));
			
			int index = Util.asInt(Processor.Pop());
			int ptr = Memory.LengthenPointer(Processor.Pop());
			byte data = (byte)(Processor.Pop());
			Memory.StoreByte(ptr, alpha + index, data);
		}
	}

	// 0144 WLS
	@RegisterOpcode(WLS)
	public static final class OP_WLS implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  WLS %02X", Processor.savedPC, alpha));
			
			int index = Util.asInt(Processor.Pop());
			int ptr = Processor.PopLong();
			byte data = (byte)(Processor.Pop());
			Memory.StoreByte(ptr, alpha + index, data);
		}
	}

	// 0145 R0F
	@RegisterOpcode(R0F)
	public static final class OP_R0F implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  R0F %02X", Processor.savedPC, alpha));
			
			int ptr = Memory.LengthenPointer(Processor.Pop());
			Processor.Push(Memory.ReadField(Memory.Read(ptr), (byte)alpha));
		}
	}
	
	// 0146 RF
	@RegisterOpcode(RF)
	public static final class OP_RF implements Runnable {
		@Override
		public void run() {
			int word = Memory.GetCodeWord();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  RF %04X", Processor.savedPC, word));
			
			int  offset = Util.HighByte(word);
			byte spec   = (byte)Util.LowByte(word);
			
			int ptr = Memory.LengthenPointer(Processor.Pop());
			Processor.Push(Memory.ReadField(Memory.Read(ptr + offset), spec));
		}
	}

	// 0147 RL0F
	@RegisterOpcode(RL0F)
	public static final class OP_RL0F implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  RL0F %02X", Processor.savedPC, alpha));
			
			int ptr = Processor.PopLong();
			Processor.Push(Memory.ReadField(Memory.Read(ptr), (byte)alpha));
		}
	}

	// 0150 RLF
	@RegisterOpcode(RLF)
	public static final class OP_RLF implements Runnable {
		@Override
		public void run() {
			int word = Memory.GetCodeWord();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  RLF %04X", Processor.savedPC, word));
			
			int  offset = Util.HighByte(word);
			byte spec   = (byte)Util.LowByte(word);

			int ptr = Processor.PopLong();
			Processor.Push(Memory.ReadField(Memory.Read(ptr + offset), spec));
		}
	}

	// 0151 RLFS
	@RegisterOpcode(RLFS)
	public static final class OP_RLFS implements Runnable {
		@Override
		public void run() {
			short word = Processor.Pop();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  RLFS %04X", Processor.savedPC, word));
			
			int  offset = Util.HighByte(word);
			byte spec   = (byte)Util.LowByte(word);

			int ptr = Processor.PopLong();
			Processor.Push(Memory.ReadField(Memory.Read(ptr + offset), spec));
		}
	}

	// 0152 RLIPF
	@RegisterOpcode(RLIPF)
	public static final class OP_RLIPF implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  RLIPF %02X", Processor.savedPC, alpha));
			int spec = Memory.GetCodeByte();

			int left  = Util.LeftNibble(alpha);
			int right = Util.RightNibble(alpha);
			
			short ptr = Memory.ReadMDS(Processor.LF, left);
			Processor.Push(Memory.ReadField(Memory.ReadMDS(ptr, right), (byte)spec));
		}
	}

	// 0153 RLILPF
	@RegisterOpcode(RLILPF)
	public static final class OP_RLILPF implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  RLILPF %02X", Processor.savedPC, alpha));
			int spec = Memory.GetCodeByte();

			int left  = Util.LeftNibble(alpha);
			int right = Util.RightNibble(alpha);
			
			int ptr = Memory.ReadDblMDS(Processor.LF, left);
			Processor.Push(Memory.ReadField(Memory.Read(ptr + right), (byte)spec));
		}
	}

	// 0154 W0F
	@RegisterOpcode(W0F)
	public static final class OP_W0F implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  W0F %02X", Processor.savedPC, alpha));
			
			short ptr  = Processor.Pop();
			short data = Processor.Pop();
			
			int memoryOffset = Memory.OffsetWrite(Memory.LengthenPointer(ptr));
			Memory.WriteMemory(memoryOffset, Memory.WriteField(Memory.ReadMemory(memoryOffset), (byte)alpha, data));
		}
	}

	// 0155 WF
	@RegisterOpcode(WF)
	public static final class OP_WF implements Runnable {
		@Override
		public void run() {
			short word = Memory.GetCodeWord();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  WF %04X", Processor.savedPC, word));
			
			int  offset = Util.HighByte(word);
			byte field  = (byte)Util.LowByte(word);

			short ptr  = Processor.Pop();
			short data = Processor.Pop();
			
			int memoryOffset = Memory.OffsetWrite(Memory.LengthenPointer(ptr) + offset);
			Memory.WriteMemory(memoryOffset, Memory.WriteField(Memory.ReadMemory(memoryOffset), field, data));
		}
	}

	// 0156 PSF
	@RegisterOpcode(PSF)
	public static final class OP_PSF implements Runnable {
		@Override
		public void run() {
			short word = Memory.GetCodeWord();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  PSF %04X", Processor.savedPC, word));
			
			int  offset = Util.HighByte(word);
			byte field  = (byte)Util.LowByte(word);

			short data = Processor.Pop();
			short ptr  = Processor.Pop();
			
			int memoryOffset = Memory.OffsetWrite(Memory.LengthenPointer(ptr) + offset);
			Memory.WriteMemory(memoryOffset, Memory.WriteField(Memory.ReadMemory(memoryOffset), field, data));
			// Processor.Recover();
			Processor.SP++;
		}
	}

	// 0157 PS0F
	@RegisterOpcode(PS0F)
	public static final class OP_PS0F implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  PS0F %04X", Processor.savedPC, alpha));
			
			short data = Processor.Pop();
			short ptr  = Processor.Pop();
			
			int memoryOffset = Memory.OffsetWrite(Memory.LengthenPointer(ptr));
			Memory.WriteMemory(memoryOffset, Memory.WriteField(Memory.ReadMemory(memoryOffset), (byte)alpha, data));
			// Processor.Recover();
			Processor.SP++;
		}
	}

	// 0160 WS0F
	@RegisterOpcode(WS0F)
	public static final class OP_v implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  WS0F %02X", Processor.savedPC, alpha));
			
			short data = Processor.Pop();
			short ptr  = Processor.Pop();
			
			int memoryOffset = Memory.OffsetWrite(Memory.LengthenPointer(ptr));
			Memory.WriteMemory(memoryOffset, Memory.WriteField(Memory.ReadMemory(memoryOffset), (byte)alpha, data));
		}
	}

	// 0161 WL0F
	@RegisterOpcode(WL0F)
	public static final class OP_WL0F implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  WL0F %02X", Processor.savedPC, alpha));
			
			int   ptr  = Processor.PopLong();
			short data = Processor.Pop();
			
			int memoryOffset = Memory.OffsetWrite(ptr);
			Memory.WriteMemory(memoryOffset, Memory.WriteField(Memory.ReadMemory(memoryOffset), (byte)alpha, data));
		}
	}

	// 0162 WLF
	@RegisterOpcode(WLF)
	public static final class OP_WLF implements Runnable {
		@Override
		public void run() {
			short word = Memory.GetCodeWord();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  WLF %04X", Processor.savedPC, word));
			
			int  offset = Util.HighByte(word);
			byte field  = (byte)Util.LowByte(word);

			int   ptr  = Processor.PopLong();
			short data = Processor.Pop();
			
			int memoryOffset = Memory.OffsetWrite(ptr + offset);
			Memory.WriteMemory(memoryOffset, Memory.WriteField(Memory.ReadMemory(memoryOffset), field, data));
		}
	}

	// 0163 PSLF
	@RegisterOpcode(PSLF)
	public static final class OP_PSLF implements Runnable {
		@Override
		public void run() {
			short word = Memory.GetCodeWord();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  PSLF %04X", Processor.savedPC, word));
			
			int  offset = Util.HighByte(word);
			byte field  = (byte)Util.LowByte(word);

			short data = Processor.Pop();
			int   ptr  = Processor.PopLong();
			
			int memoryOffset = Memory.OffsetWrite(ptr + offset);
			Memory.WriteMemory(memoryOffset, Memory.WriteField(Memory.ReadMemory(memoryOffset), field, data));
			
			// Processor.Recover();
			// Processor.Recover();
			Processor.SP += 2;
		}
	}

	// 0164 WLFS
	@RegisterOpcode(WLFS)
	public static final class OP_WLFS implements Runnable {
		@Override
		public void run() {
			short word = Processor.Pop();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  WLFS %04X", Processor.savedPC, word));
			
			int  offset = Util.HighByte(word);
			byte field  = (byte)Util.LowByte(word);

			int   ptr  = Processor.PopLong();
			short data = Processor.Pop();
			
			int memoryOffset = Memory.OffsetWrite(ptr + offset);
			Memory.WriteMemory(memoryOffset, Memory.WriteField(Memory.ReadMemory(memoryOffset), field, data));
		}
	}
	
	// 0165 SLDB
	@RegisterOpcode(SLDB)
	public static final class OP_SLDB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  SLDB %02X", Processor.savedPC, alpha));

			Opcode0xx.SLDn(alpha);
		}
	}

	// 0166 SGDB
	@RegisterOpcode(SGDB)
	public static final class OP_SGDB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  SGDB %02X", Processor.savedPC, alpha));

			Memory.Write(Processor.GF + alpha + 1, Processor.Pop());
			Memory.Write(Processor.GF + alpha + 0, Processor.Pop());
		}
	}

	// 0167 LLKB
	@RegisterOpcode(LLKB)
	public static final class OP_LLKB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LLKB %02X", Processor.savedPC, alpha));

			Processor.PushLong(Memory.FetchLink(alpha));
		}
	}

	// 0170 RKIB
	@RegisterOpcode(RKIB)
	public static final class OP_RLKB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  RKIB %02X", Processor.savedPC, alpha));

			int ptr = Memory.FetchLink(alpha);
			Processor.Push(Memory.Read(ptr));
		}
	}

	// 0171 RKDIB
	@RegisterOpcode(RKDIB)
	public static final class OP_RKDIB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  RKDIB %02X", Processor.savedPC, alpha));

			int ptr = Memory.FetchLink(alpha);
			Processor.Push(Memory.Read(ptr + 0));
			Processor.Push(Memory.Read(ptr + 1));
		}
	}

	// 0172 LKB
	@RegisterOpcode(LKB)
	public static final class OP_LKB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  v %02X", Processor.savedPC, alpha));

			Processor.Recover();
			int link = Util.asInt(Processor.Pop());
			Memory.WriteMDS(Processor.LF, (short)(link - alpha));
		}
	}

	// 0173 SHIFT
	@RegisterOpcode(SHIFT)
	public static final class OP_SHIFT implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  SHIFT", Processor.savedPC));

			// shift is signed
			int   shift = Processor.Pop();
			short u     = Processor.Pop();
			
			Processor.Push(Util.Shift(u, shift));
		}
	}
	
	// 0174 SHIFTSB
	@RegisterOpcode(SHIFTSB)
	public static final class OP_SHIFTSB implements Runnable {
		@Override
		public void run() {
			byte alpha = (byte)Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  SHIFTSB %d", Processor.savedPC, alpha));

			if (alpha < -15 || 15 < alpha) throw new MesaAssertionError();
			// shift is signed
			short u = Processor.Pop();
			Processor.Push(Util.Shift(u, alpha));
		}
	}
}
