package mh.majuro.mesa.opcode;
//
//

import static mh.majuro.mesa.Debug.TRACE_OPCODE;
import static mh.majuro.mesa.OpcodeInfo.BNDCK;
import static mh.majuro.mesa.OpcodeInfo.BRK;
import static mh.majuro.mesa.OpcodeInfo.LG0;
import static mh.majuro.mesa.OpcodeInfo.LG1;
import static mh.majuro.mesa.OpcodeInfo.LG2;
import static mh.majuro.mesa.OpcodeInfo.LGB;
import static mh.majuro.mesa.OpcodeInfo.LGD0;
import static mh.majuro.mesa.OpcodeInfo.LGD2;
import static mh.majuro.mesa.OpcodeInfo.LGDB;
import static mh.majuro.mesa.OpcodeInfo.LL0;
import static mh.majuro.mesa.OpcodeInfo.LL1;
import static mh.majuro.mesa.OpcodeInfo.LL10;
import static mh.majuro.mesa.OpcodeInfo.LL11;
import static mh.majuro.mesa.OpcodeInfo.LL2;
import static mh.majuro.mesa.OpcodeInfo.LL3;
import static mh.majuro.mesa.OpcodeInfo.LL4;
import static mh.majuro.mesa.OpcodeInfo.LL5;
import static mh.majuro.mesa.OpcodeInfo.LL6;
import static mh.majuro.mesa.OpcodeInfo.LL7;
import static mh.majuro.mesa.OpcodeInfo.LL8;
import static mh.majuro.mesa.OpcodeInfo.LL9;
import static mh.majuro.mesa.OpcodeInfo.LLB;
import static mh.majuro.mesa.OpcodeInfo.LLD0;
import static mh.majuro.mesa.OpcodeInfo.LLD1;
import static mh.majuro.mesa.OpcodeInfo.LLD10;
import static mh.majuro.mesa.OpcodeInfo.LLD2;
import static mh.majuro.mesa.OpcodeInfo.LLD3;
import static mh.majuro.mesa.OpcodeInfo.LLD4;
import static mh.majuro.mesa.OpcodeInfo.LLD5;
import static mh.majuro.mesa.OpcodeInfo.LLD6;
import static mh.majuro.mesa.OpcodeInfo.LLD7;
import static mh.majuro.mesa.OpcodeInfo.LLD8;
import static mh.majuro.mesa.OpcodeInfo.LLDB;
import static mh.majuro.mesa.OpcodeInfo.NOOP;
import static mh.majuro.mesa.OpcodeInfo.PL0;
import static mh.majuro.mesa.OpcodeInfo.PL1;
import static mh.majuro.mesa.OpcodeInfo.PL2;
import static mh.majuro.mesa.OpcodeInfo.PL3;
import static mh.majuro.mesa.OpcodeInfo.PLB;
import static mh.majuro.mesa.OpcodeInfo.PLD0;
import static mh.majuro.mesa.OpcodeInfo.PLDB;
import static mh.majuro.mesa.OpcodeInfo.SGB;
import static mh.majuro.mesa.OpcodeInfo.SL0;
import static mh.majuro.mesa.OpcodeInfo.SL1;
import static mh.majuro.mesa.OpcodeInfo.SL10;
import static mh.majuro.mesa.OpcodeInfo.SL2;
import static mh.majuro.mesa.OpcodeInfo.SL3;
import static mh.majuro.mesa.OpcodeInfo.SL4;
import static mh.majuro.mesa.OpcodeInfo.SL5;
import static mh.majuro.mesa.OpcodeInfo.SL6;
import static mh.majuro.mesa.OpcodeInfo.SL7;
import static mh.majuro.mesa.OpcodeInfo.SL8;
import static mh.majuro.mesa.OpcodeInfo.SL9;
import static mh.majuro.mesa.OpcodeInfo.SLB;
import static mh.majuro.mesa.OpcodeInfo.SLD0;
import static mh.majuro.mesa.OpcodeInfo.SLD1;
import static mh.majuro.mesa.OpcodeInfo.SLD2;
import static mh.majuro.mesa.OpcodeInfo.SLD3;
import static mh.majuro.mesa.OpcodeInfo.SLD4;
import static mh.majuro.mesa.OpcodeInfo.SLD5;
import static mh.majuro.mesa.OpcodeInfo.SLD6;
import static mh.majuro.mesa.OpcodeInfo.SLD8;
import mh.majuro.mesa.Memory;
import mh.majuro.mesa.MesaAssertionError;
import mh.majuro.mesa.Opcode;
import mh.majuro.mesa.Opcode.RegisterOpcode;
import mh.majuro.mesa.Processor;
import mh.majuro.mesa.Util;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public final class Opcode0xx {
	protected static final Logger logger = LoggerFactory.getLogger(Opcode0xx.class);
	
	// 00 NOOP
	@RegisterOpcode(NOOP)
	public static final class OP_NOOP implements Runnable {
		@Override
		public void run() {
			logger.error(String.format("TRACE %6o  NOOP", Processor.savedPC));
			throw new MesaAssertionError();
		}
	}
	
	// Anonymous class version
//	@RegisterOpcode(NOOP)
//	public static Runnable OP_NOOP = new Runnable() {
//		@Override
//		public void run() {
//			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  NOOP"));
//			//throw new MesaAssertionError();
//			logger.debug("OP_NOOP Anonymous");
//		}
//	};
	
	static void LLn(final int n) {
		Processor.Push(Memory.ReadMDS(Processor.LF, n));
	}

	// 01 LL0
	@RegisterOpcode(LL0)
	public static final class OP_LL0 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LL0", Processor.savedPC));
			LLn(0);
		}
	}

	// 02 LL1
	@RegisterOpcode(LL1)
	public static final class OP_LL1 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LL1", Processor.savedPC));
			LLn(1);
		}
	}

	// 03 LL2
	@RegisterOpcode(LL2)
	public static final class OP_LL2 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LL2", Processor.savedPC));
			LLn(2);
		}
	}

	// 04 LL3
	@RegisterOpcode(LL3)
	public static final class OP_LL3 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LL3", Processor.savedPC));
			LLn(3);
		}
	}

	// 05 LL4
	@RegisterOpcode(LL4)
	public static final class OP_LL4 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LL4", Processor.savedPC));
			LLn(4);
		}
	}

	// 06 LL5
	@RegisterOpcode(LL5)
	public static final class OP_LL5 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LL5", Processor.savedPC));
			LLn(5);
		}
	}

	// 07 LL6
	@RegisterOpcode(LL6)
	public static final class OP_LL6 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LL6", Processor.savedPC));
			LLn(6);
		}
	}

	// 010 LL7
	@RegisterOpcode(LL7)
	public static final class OP_LL7 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LL7", Processor.savedPC));
			LLn(7);
		}
	}

	// 011 LL8
	@RegisterOpcode(LL8)
	public static final class OP_LL8 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LL8", Processor.savedPC));
			LLn(8);
		}
	}

	// 012 LL9
	@RegisterOpcode(LL9)
	public static final class OP_LL9 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LL9", Processor.savedPC));
			LLn(9);
		}
	}

	// 013 LL10
	@RegisterOpcode(LL10)
	public static final class OP_LL10 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LL10", Processor.savedPC));
			LLn(10);
		}
	}

	// 014 LL11
	@RegisterOpcode(LL11)
	public static final class OP_LL11 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LL11", Processor.savedPC));
			LLn(11);
		}
	}

	// 015 LLB
	@RegisterOpcode(LLB)
	public static final class OP_LLB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LLB %02X", Processor.savedPC, alpha));
			LLn(alpha);
		}
	}
	
	static void LLDn(final int n) {
		Processor.Push(Memory.ReadMDS(Processor.LF, n + 0));
		Processor.Push(Memory.ReadMDS(Processor.LF, n + 1));
	}

	// 016 LLD0
	@RegisterOpcode(LLD0)
	public static final class OP_LLD0 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LLD0", Processor.savedPC));
			LLDn(0);
		}
	}

	// 017 LLD1
	@RegisterOpcode(LLD1)
	public static final class OP_LLD1 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LLD1", Processor.savedPC));
			LLDn(1);
		}
	}

	// 020 LLD2
	@RegisterOpcode(LLD2)
	public static final class OP_LLD2 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LLD2", Processor.savedPC));
			LLDn(2);
		}
	}

	// 021 LLD3
	@RegisterOpcode(LLD3)
	public static final class OP_LLD3 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LLD3", Processor.savedPC));
			LLDn(3);
		}
	}

	// 022 LLD4
	@RegisterOpcode(LLD4)
	public static final class OP_LLD4 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LLD4", Processor.savedPC));
			LLDn(4);
		}
	}

	// 023 LLD5
	@RegisterOpcode(LLD5)
	public static final class OP_LLD5 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LLD5", Processor.savedPC));
			LLDn(5);
		}
	}

	// 024 LLD6
	@RegisterOpcode(LLD6)
	public static final class OP_LLD6 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LLD6", Processor.savedPC));
			LLDn(6);
		}
	}

	// 025 LLD7
	@RegisterOpcode(LLD7)
	public static final class OP_LLD7 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LLD7", Processor.savedPC));
			LLDn(7);
		}
	}

	// 026 LLD8
	@RegisterOpcode(LLD8)
	public static final class OP_LLD8 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LLD8", Processor.savedPC));
			LLDn(8);
		}
	}

	// 027 LLD10
	@RegisterOpcode(LLD10)
	public static final class OP_LLD10 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LLD10", Processor.savedPC));
			LLDn(10);
		}
	}


	// 030 LLDB
	@RegisterOpcode(LLDB)
	public static final class OP_LLDB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LLDB %02X", Processor.savedPC, alpha));
			LLDn(alpha);
		}
	}

	static void SLn(final int n) {
		Memory.WriteMDS(Processor.LF, n, Processor.Pop());
	}

	// 031 SL0
	@RegisterOpcode(SL0)
	public static final class OP_SL0 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  SL0", Processor.savedPC));
			SLn(0);
		}
	}
	
	// 032 SL1
	@RegisterOpcode(SL1)
	public static final class OP_SL1 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  SL1", Processor.savedPC));
			SLn(1);
		}
	}
	
	// 033 SL2
	@RegisterOpcode(SL2)
	public static final class OP_SL2 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  SL2", Processor.savedPC));
			SLn(2);
		}
	}
	
	// 034 SL3
	@RegisterOpcode(SL3)
	public static final class OP_SL3 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  SL3", Processor.savedPC));
			SLn(3);
		}
	}
	
	// 035 SL4
	@RegisterOpcode(SL4)
	public static final class OP_SL4 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  SL4", Processor.savedPC));
			SLn(4);
		}
	}
	
	// 036 SL5
	@RegisterOpcode(SL5)
	public static final class OP_SL5 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  SL5", Processor.savedPC));
			SLn(5);
		}
	}
	
	// 037 SL6
	@RegisterOpcode(SL6)
	public static final class OP_SL6 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  SL6", Processor.savedPC));
			SLn(6);
		}
	}
	

	// 040 SL7
	@RegisterOpcode(SL7)
	public static final class OP_SL7 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  SL7", Processor.savedPC));
			SLn(7);
		}
	}
	
	// 041 SL8
	@RegisterOpcode(SL8)
	public static final class OP_SL8 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  SL8", Processor.savedPC));
			SLn(8);
		}
	}
	
	// 042 SL9
	@RegisterOpcode(SL9)
	public static final class OP_SL9 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  SL9", Processor.savedPC));
			SLn(9);
		}
	}
	
	// 043 SL10
	@RegisterOpcode(SL10)
	public static final class OP_SL10 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  SL10", Processor.savedPC));
			SLn(10);
		}
	}
	
	// 044 SLB
	@RegisterOpcode(SLB)
	public static final class OP_SLB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  SLB %02X", Processor.savedPC, alpha));
			SLn(alpha);
		}
	}
	
	static void SLDn(final int n) {
		Memory.WriteMDS(Processor.LF, n + 1, Processor.Pop());
		Memory.WriteMDS(Processor.LF, n + 0, Processor.Pop());
	}
	
	// 045 SLD0
	@RegisterOpcode(SLD0)
	public static final class OP_SLD0 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  SLD0", Processor.savedPC));
			SLDn(0);
		}
	}

	// 046 SLD1
	@RegisterOpcode(SLD1)
	public static final class OP_SLD1 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  SLD1", Processor.savedPC));
			SLDn(1);
		}
	}

	// 047 SLD2
	@RegisterOpcode(SLD2)
	public static final class OP_SLD2 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  SLD2", Processor.savedPC));
			SLDn(2);
		}
	}


	// 050 SLD3
	@RegisterOpcode(SLD3)
	public static final class OP_SLD3 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  SLD3", Processor.savedPC));
			SLDn(3);
		}
	}

	// 051 SLD4
	@RegisterOpcode(SLD4)
	public static final class OP_SLD4 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  SLD4", Processor.savedPC));
			SLDn(4);
		}
	}

	// 052 SLD5
	@RegisterOpcode(SLD5)
	public static final class OP_SLD5 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  SLD5", Processor.savedPC));
			SLDn(5);
		}
	}

	// 053 SLD6
	@RegisterOpcode(SLD6)
	public static final class OP_SLD6 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  SLD6", Processor.savedPC));
			SLDn(6);
		}
	}

	// 054 SLD8
	@RegisterOpcode(SLD8)
	public static final class OP_SLD8 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  SLD8", Processor.savedPC));
			SLDn(8);
		}
	}

	static void PLn(final int n) {
		SLn(n);
		//Processor.Recover();
		Processor.SP++;
	}

	// 055 PL0
	@RegisterOpcode(PL0)
	public static final class OP_PL0 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  PL0", Processor.savedPC));
			PLn(0);
		}
	}
	
	// 056 PL1
	@RegisterOpcode(PL1)
	public static final class OP_PL1 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  PL1", Processor.savedPC));
			PLn(1);
		}
	}
	
	// 057 PL2
	@RegisterOpcode(PL2)
	public static final class OP_PL2 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  PL2", Processor.savedPC));
			PLn(2);
		}
	}

	// 060 PL3
	@RegisterOpcode(PL3)
	public static final class OP_PL3 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  PL3", Processor.savedPC));
			PLn(3);
		}
	}
	
	// 061 PLB
	@RegisterOpcode(PLB)
	public static final class OP_PLB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  PLB %02X", Processor.savedPC, alpha));
			PLn(alpha);
		}
	}

	static void PLDn(final int n) {
		SLDn(n);
		//Processor.Recover();
		//Processor.Recover();
		Processor.SP += 2;
	}
	
	// 062 PLD0
	@RegisterOpcode(PLD0)
	public static final class OP_PLD0 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  PLD0", Processor.savedPC));
			PLDn(0);
		}
	}
	
	// 063 PLDB
	@RegisterOpcode(PLDB)
	public static final class OP_PLDB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  PLDB %02X", Processor.savedPC, alpha));
			PLDn(alpha);
		}
	}

	static void LGn(final int n) {
		Processor.Push(Memory.Read(Processor.GF + n));
	}
	
	// 064 LG0
	@RegisterOpcode(LG0)
	public static final class OP_LG0 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LG0", Processor.savedPC));
			LGn(0);
		}
	}
	
	// 065 LG1
	@RegisterOpcode(LG1)
	public static final class OP_LG1 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LG1", Processor.savedPC));
			LGn(1);
		}
	}
	
	// 066 LG2
	@RegisterOpcode(LG2)
	public static final class OP_LG2 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LG2", Processor.savedPC));
			LGn(2);
		}
	}
	
	// 067 LGB
	@RegisterOpcode(LGB)
	public static final class OP_LGB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LGB %02X", Processor.savedPC, alpha));
			LGn(alpha);
		}
	}

	static void LGDn(final int n) {
		Processor.Push(Memory.Read(Processor.GF + n + 0));
		Processor.Push(Memory.Read(Processor.GF + n + 1));
	}
	
	// 070 LGD0
	@RegisterOpcode(LGD0)
	public static final class OP_LGD0 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LGD0", Processor.savedPC));
			LGDn(0);
		}
	}

	// 071 LGD2
	@RegisterOpcode(LGD2)
	public static final class OP_LGD2 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LGD2", Processor.savedPC));
			LGDn(2);
		}
	}

	// 072 LGDB
	@RegisterOpcode(LGDB)
	public static final class OP_LGDB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LGDB %02X", Processor.savedPC, alpha));
			LGDn(alpha);
		}
	}

	// 073 SGB
	@RegisterOpcode(SGB)
	public static final class OP_SGB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  SGB %02X", Processor.savedPC, alpha));
			
			Memory.Write(Processor.GF + alpha, Processor.Pop());
		}
	}

	// 074 BNDCK
	@RegisterOpcode(BNDCK)
	public static final class OP_BNDCK implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  BNDCK", Processor.savedPC));
			int range = Util.asInt(Processor.Pop());
			int index = Util.asInt(Processor.Pop());
			Processor.Push((short)index);
			if (range <= index) Processor.BoundsTrap();
		}
	}

	// 075 BRK
	@RegisterOpcode(BRK)
	public static final class OP_BRK implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  BRK", Processor.savedPC));
			if (Processor.breakByte == 0) Processor.BreakTrap();
			Opcode.dispatch(Processor.breakByte);
			Processor.breakByte = 0;
		}
	}
}
