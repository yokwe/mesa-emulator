package mh.majuro.mesa.opcode;

import static mh.majuro.mesa.Debug.TRACE_OPCODE;
import static mh.majuro.mesa.OpcodeInfo.*;
import mh.majuro.mesa.MesaAssertionError;
import mh.majuro.mesa.Opcode.RegisterOpcode;
import mh.majuro.mesa.Memory;
import mh.majuro.mesa.Processor;
import mh.majuro.mesa.Type;
import mh.majuro.mesa.Util;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public final class Opcode2xx {
	protected static final Logger logger = LoggerFactory.getLogger(Opcode2xx.class);

	// 0200 CATCH
	@RegisterOpcode(CATCH)
	public static final class OP_CATCH implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  CATCH", Processor.savedPC));
			Memory.GetCodeByte();
		}
	}
	
	static void Jn(int n) {
		Processor.PC = (short)(Processor.savedPC + n);
	}
	
	// 0201 J2
	@RegisterOpcode(J2)
	public static final class OP_J2 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  J2", Processor.savedPC));
			Jn(2);
		}
	}

	// 0202 J3
	@RegisterOpcode(J3)
	public static final class OP_J3 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  J3", Processor.savedPC));
			Jn(3);
		}
	}

	// 0203 J4
	@RegisterOpcode(J4)
	public static final class OP_J4 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  J4", Processor.savedPC));
			Jn(4);
		}
	}

	// 0204 J5
	@RegisterOpcode(J5)
	public static final class OP_J5 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  J5", Processor.savedPC));
			Jn(5);
		}
	}

	// 0205 J6
	@RegisterOpcode(J6)
	public static final class OP_J6 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  J6", Processor.savedPC));
			Jn(6);
		}
	}

	// 0206 J7
	@RegisterOpcode(J7)
	public static final class OP_J7 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  J7", Processor.savedPC));
			Jn(7);
		}
	}

	// 0207 J8
	@RegisterOpcode(J8)
	public static final class OP_J8 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  J8", Processor.savedPC));
			Jn(8);
		}
	}


	// 0210 JB
	@RegisterOpcode(JB)
	public static final class OP_JB implements Runnable {
		@Override
		public void run() {
			// disp is signed byte
			int disp = (byte)Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  JB %d", Processor.savedPC, disp));
			Jn(disp);
		}
	}

	// 0211 JW
	@RegisterOpcode(JW)
	public static final class OP_JW implements Runnable {
		@Override
		public void run() {
			// disp is signed short
			int disp = Memory.GetCodeWord();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  JW %d", Processor.savedPC, disp));
			Jn(disp);
		}
	}

	// 0212 JEP
	@RegisterOpcode(JEP)
	public static final class OP_JEP implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  JEP %02X", Processor.savedPC, alpha));
			
			short data = Processor.Pop();
			if (data == Util.LeftNibble(alpha)) Processor.PC = (short)(Processor.savedPC + Util.RightNibble(alpha) + 4);
		}
	}

	// 0213 JEB
	@RegisterOpcode(JEB)
	public static final class OP_JEB implements Runnable {
		@Override
		public void run() {
			// disp is signed byte
			int disp = (byte)Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  JEB %d", Processor.savedPC, disp));
			
			short v = Processor.Pop();
			short u = Processor.Pop();
			if (u == v) Processor.PC = (short)(Processor.savedPC + disp);
		}
	}

	// 0214 JEBB
	@RegisterOpcode(JEBB)
	public static final class OP_JEBB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			// disp is signed byte
			int disp = (byte)Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  JEBB %d %d", Processor.savedPC, alpha, disp));
			
			short data = Processor.Pop();
			if (data == alpha) Processor.PC = (short)(Processor.savedPC + disp);
		}
	}

	// 0215 JNEP
	@RegisterOpcode(JNEP)
	public static final class OP_JNEP implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  JNEP %02X", Processor.savedPC, alpha));
			
			short data = Processor.Pop();
			if (data != Util.LeftNibble(alpha)) Processor.PC = (short)(Processor.savedPC + Util.RightNibble(alpha) + 4);
		}
	}

	// 0216 JNEB
	@RegisterOpcode(JNEB)
	public static final class OP_JNEB implements Runnable {
		@Override
		public void run() {
			// disp is signed byte
			int disp = (byte)Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  JNEB %d", Processor.savedPC, disp));
			
			short v = Processor.Pop();
			short u = Processor.Pop();
			if (u != v) Processor.PC = (short)(Processor.savedPC + disp);
		}
	}

	// 0217 JNEBB
	@RegisterOpcode(JNEBB)
	public static final class OP_JNEBB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			// disp is signed byte
			int disp = (byte)Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  JNEBB %d %d", Processor.savedPC, alpha, disp));
			
			short data = Processor.Pop();
			if (data != alpha) Processor.PC = (short)(Processor.savedPC + disp);
		}
	}


	// 0220 JLB
	@RegisterOpcode(JLB)
	public static final class OP_JLB implements Runnable {
		@Override
		public void run() {
			// disp is signed byte
			int disp = (byte)Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  JLB %d", Processor.savedPC, disp));
			
			short k = Processor.Pop();
			short j = Processor.Pop();
			if (j < k) Processor.PC = (short)(Processor.savedPC + disp);
		}
	}

	// 0221 JGEB
	@RegisterOpcode(JGEB)
	public static final class OP_JGEB implements Runnable {
		@Override
		public void run() {
			// disp is signed byte
			int disp = (byte)Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  JGEB %d", Processor.savedPC, disp));
			
			short k = Processor.Pop();
			short j = Processor.Pop();
			if (j >= k) Processor.PC = (short)(Processor.savedPC + disp);
		}
	}

	// 0222 JGB
	@RegisterOpcode(JGB)
	public static final class OP_JGB implements Runnable {
		@Override
		public void run() {
			// disp is signed byte
			int disp = (byte)Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  JGB %d", Processor.savedPC, disp));
			
			short k = Processor.Pop();
			short j = Processor.Pop();
			if (j > k) Processor.PC = (short)(Processor.savedPC + disp);
		}
	}

	// 0223 JLEB
	@RegisterOpcode(JLEB)
	public static final class OP_JLEB implements Runnable {
		@Override
		public void run() {
			// disp is signed byte
			int disp = (byte)Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  JLEB %d", Processor.savedPC, disp));
			
			short k = Processor.Pop();
			short j = Processor.Pop();
			if (j <= k) Processor.PC = (short)(Processor.savedPC + disp);
		}
	}

	// 0224 JULB
	@RegisterOpcode(JULB)
	public static final class OP_JULB implements Runnable {
		@Override
		public void run() {
			// disp is signed byte
			int disp = (byte)Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  JULB %d", Processor.savedPC, disp));
			
			// u and v are unsigned
			int v = Util.asInt(Processor.Pop());
			int u = Util.asInt(Processor.Pop());
			if (u < v) Processor.PC = (short)(Processor.savedPC + disp);
		}
	}

	// 0225 JUGEB
	@RegisterOpcode(JUGEB)
	public static final class OP_JUGEB implements Runnable {
		@Override
		public void run() {
			// disp is signed byte
			int disp = (byte)Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  JUGEB %d", Processor.savedPC, disp));
			
			// u and v are unsigned
			int v = Util.asInt(Processor.Pop());
			int u = Util.asInt(Processor.Pop());
			if (u >= v) Processor.PC = (short)(Processor.savedPC + disp);
		}
	}

	// 0226 JUGB
	@RegisterOpcode(JUGB)
	public static final class OP_JUGB implements Runnable {
		@Override
		public void run() {
			// disp is signed byte
			int disp = (byte)Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  JUGB %d", Processor.savedPC, disp));
			
			// u and v are unsigned
			int v = Util.asInt(Processor.Pop());
			int u = Util.asInt(Processor.Pop());
			if (u > v) Processor.PC = (short)(Processor.savedPC + disp);
		}
	}

	// 0227 JULEB
	@RegisterOpcode(JULEB)
	public static final class OP_JULEB implements Runnable {
		@Override
		public void run() {
			// disp is signed byte
			int disp = (byte)Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  JULEB %d", Processor.savedPC, disp));
			
			// u and v are unsigned
			int v = Util.asInt(Processor.Pop());
			int u = Util.asInt(Processor.Pop());
			if (u <= v) Processor.PC = (short)(Processor.savedPC + disp);
		}
	}


	// 0230 JZ3
	@RegisterOpcode(JZ3)
	public static final class OP_JZ3 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  JZ3", Processor.savedPC));
			
			short u = Processor.Pop();
			if (u == 0) Processor.PC = (short)(Processor.savedPC + 3);
		}
	}
	
	// 0231 JZ4
	@RegisterOpcode(JZ4)
	public static final class OP_JZ4 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  JZ4", Processor.savedPC));
			
			short u = Processor.Pop();
			if (u == 0) Processor.PC = (short)(Processor.savedPC + 4);
		}
	}
	
	// 0232 JZB
	@RegisterOpcode(JZB)
	public static final class OP_JZB implements Runnable {
		@Override
		public void run() {
			// disp is signed byte
			int disp = (byte)Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  JZB %d", Processor.savedPC, disp));
			
			short u = Processor.Pop();
			if (u == 0) Processor.PC = (short)(Processor.savedPC + 4);
		}
	}
	
	// 0233 JNZ3
	@RegisterOpcode(JNZ3)
	public static final class OP_JNZ3 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  JNZ3", Processor.savedPC));
			
			short u = Processor.Pop();
			if (u != 0) Processor.PC = (short)(Processor.savedPC + 3);
		}
	}
	
	// 0234 JNZ4
	@RegisterOpcode(JNZ4)
	public static final class OP_JNZ4 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  JNZ4", Processor.savedPC));
			
			short u = Processor.Pop();
			if (u != 0) Processor.PC = (short)(Processor.savedPC + 4);
		}
	}
	
	// 0235 JNZB
	@RegisterOpcode(JNZB)
	public static final class OP_JNZB implements Runnable {
		@Override
		public void run() {
			// disp is signed byte
			int disp = (byte)Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  JNZB %d", Processor.savedPC, disp));
			
			short u = Processor.Pop();
			if (u != 0) Processor.PC = (short)(Processor.savedPC + 4);
		}
	}

	// 0236 JDEB
	@RegisterOpcode(JDEB)
	public static final class OP_JDEB implements Runnable {
		@Override
		public void run() {
			// disp is signed byte
			int disp = (byte)Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  JDEB %d", Processor.savedPC, disp));
			
			int v = Processor.PopLong();
			int u = Processor.PopLong();
			if (u == v) Processor.PC = (short)(Processor.savedPC + disp);
		}
	}

	// 0237 JDNEB
	@RegisterOpcode(JDNEB)
	public static final class OP_JDNEB implements Runnable {
		@Override
		public void run() {
			// disp is signed byte
			int disp = (byte)Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  JDNEB %d", Processor.savedPC, disp));
			
			int v = Processor.PopLong();
			int u = Processor.PopLong();
			if (u != v) Processor.PC = (short)(Processor.savedPC + disp);
		}
	}
	

	// 0240 JIB
	@RegisterOpcode(JIB)
	public static final class OP_JIB implements Runnable {
		@Override
		public void run() {
			// disp is signed byte
			int base = (byte)Memory.GetCodeWord();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  JIB %04X", Processor.savedPC, base));
			
			int limit = Util.asInt(Processor.Pop());
			int index = Util.asInt(Processor.Pop());
			if (index < limit) {
				short disp = Memory.ReadCode(base + index / 2);
				Processor.PC = (short)(Processor.savedPC + ((index & 1) == 0 ? Util.HighHalfAsInt(disp) : Util.LowHalfAsInt(disp)));
			}
		}
	}

	// 0241 JIW
	@RegisterOpcode(JIW)
	public static final class OP_JIW implements Runnable {
		@Override
		public void run() {
			// disp is signed byte
			int base = (byte)Memory.GetCodeWord();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  JIW %04X", Processor.savedPC, base));
			
			int limit = Util.asInt(Processor.Pop());
			int index = Util.asInt(Processor.Pop());
			if (index < limit) {
				short disp = Memory.ReadCode(base + index);
				Processor.PC = (short)(Processor.savedPC + Util.asInt(disp));
			}
		}
	}

	// 0242 REC
	@RegisterOpcode(REC)
	public static final class OP_REC implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  REC", Processor.savedPC));
			
			Processor.Recover();
		}
	}

	// 0243 REC2
	@RegisterOpcode(REC2)
	public static final class OP_REC2 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  REC2", Processor.savedPC));
			
			Processor.Recover();
			Processor.Recover();
		}
	}

	// 0244 DIS
	@RegisterOpcode(DIS)
	public static final class OP_DIS implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  DIS", Processor.savedPC));
			
			Processor.Discard();
		}
	}

	// 0245 DIS2
	@RegisterOpcode(DIS2)
	public static final class OP_DIS2 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  DIS2", Processor.savedPC));
			
			Processor.Discard();
			Processor.Discard();
		}
	}

	// 0246 EXCH
	@RegisterOpcode(EXCH)
	public static final class OP_EXCH implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  EXCH", Processor.savedPC));
			
			short v = Processor.Pop();
			short u = Processor.Pop();
			Processor.Push(v);
			Processor.Push(u);
		}
	}

	// 0247 DEXCH
	@RegisterOpcode(DEXCH)
	public static final class OP_DEXCH implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  DEXCH", Processor.savedPC));
			
			int v = Processor.PopLong();
			int u = Processor.PopLong();
			Processor.PushLong(v);
			Processor.PushLong(u);
		}
	}

	// 0250 DUP
	@RegisterOpcode(DUP)
	public static final class OP_DUP implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  DUP", Processor.savedPC));
			
			short u = Processor.Pop();
			Processor.Push(u);
			Processor.Push(u);
		}
	}

	// 0251 DDUP
	@RegisterOpcode(DDUP)
	public static final class OP_DDUP implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  DDUP", Processor.savedPC));
			
			int u = Processor.PopLong();
			Processor.PushLong(u);
			Processor.PushLong(u);
		}
	}

	// 0252 EXDIS
	@RegisterOpcode(EXDIS)
	public static final class OP_EXDIS implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  EXDIS", Processor.savedPC));
			
			short u = Processor.Pop();
			Processor.Pop();
			Processor.Push(u);
		}
	}

	// 0253 NEG
	@RegisterOpcode(NEG)
	public static final class OP_NEG implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  NEG", Processor.savedPC));
			
			short i = Processor.Pop();
			Processor.Push((short)-i);
		}
	}

	// 0254 INC
	@RegisterOpcode(INC)
	public static final class OP_INC implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  INC", Processor.savedPC));
			
			short i = Processor.Pop();
			Processor.Push((short)(i + 1));
		}
	}

	// 0255 DEC
	@RegisterOpcode(DEC)
	public static final class OP_DEC implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  DEC", Processor.savedPC));
			
			short i = Processor.Pop();
			Processor.Push((short)(i - 1));
		}
	}

	// 0256 DINC
	@RegisterOpcode(DINC)
	public static final class OP_DINC implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  DINC", Processor.savedPC));
			
			int i = Processor.PopLong();
			Processor.PushLong(i + 1);
		}
	}

	// 0257 DBL
	@RegisterOpcode(DBL)
	public static final class OP_DBL implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  DBL", Processor.savedPC));
			
			short i = Processor.Pop();
			Processor.Push((short)(i << 1));
		}
	}


	// 0260 DDBL
	@RegisterOpcode(DDBL)
	public static final class OP_DDBL implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  DDBL", Processor.savedPC));
			
			int i = Processor.PopLong();
			Processor.PushLong(i << 1);
		}
	}
	
	// 0261 TRPL
	@RegisterOpcode(TRPL)
	public static final class OP_TRPL implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  TRPL", Processor.savedPC));
			
			int i = Util.asInt(Processor.Pop());
			Processor.Push((short)(i * 3));
		}
	}

	// 0262 AND
	@RegisterOpcode(AND)
	public static final class OP_AND implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  AND", Processor.savedPC));
			
			short v = Processor.Pop();
			short u = Processor.Pop();
			Processor.Push((short)(u & v));
		}
	}

	// 0263 IOR
	@RegisterOpcode(IOR)
	public static final class OP_IOR implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  IOR", Processor.savedPC));
			
			short v = Processor.Pop();
			short u = Processor.Pop();
			Processor.Push((short)(u | v));
		}
	}
	
	// 0264 ADDSB
	@RegisterOpcode(ADDSB)
	public static final class OP_ADDSB implements Runnable {
		@Override
		public void run() {
			byte alpha = (byte)Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  ADDSB %d", Processor.savedPC, alpha));
			
			short i = Processor.Pop();
			Processor.Push((short)(i + alpha));
		}
	}
	
	// 0265 ADD
	@RegisterOpcode(ADD)
	public static final class OP_ADD implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  ADD", Processor.savedPC));
			
			short t = Processor.Pop();
			short s = Processor.Pop();
			Processor.Push((short)(s + t));
		}
	}

	// 0266 SUB
	@RegisterOpcode(SUB)
	public static final class OP_SUB implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  SUB", Processor.savedPC));
			
			short t = Processor.Pop();
			short s = Processor.Pop();
			Processor.Push((short)(s - t));
		}
	}
	// 0267 DADD
	@RegisterOpcode(DADD)
	public static final class OP_DADD implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  DADD", Processor.savedPC));
			
			int t = Processor.PopLong();
			int s = Processor.PopLong();
			Processor.PushLong(s + t);
		}
	}


	// 0270 DSUB
	@RegisterOpcode(DSUB)
	public static final class OP_DSUB implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  DSUB", Processor.savedPC));
			
			int t = Processor.PopLong();
			int s = Processor.PopLong();
			Processor.PushLong(s - t);
		}
	}
	
	// 0271 ADC
	@RegisterOpcode(ADC)
	public static final class OP_ADC implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  ADC", Processor.savedPC));
			
			int t = Util.asInt(Processor.Pop());
			int s = Processor.PopLong();
			Processor.PushLong(s + t);
		}
	}
	
	// 0272 ACD
	@RegisterOpcode(ACD)
	public static final class OP_ACD implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  ACD", Processor.savedPC));
			
			int t = Processor.PopLong();
			int s = Util.asInt(Processor.Pop());
			Processor.PushLong(s + t);
		}
	}
	
	// 0273 AL0IB
	@RegisterOpcode(AL0IB)
	public static final class OP_AL0IB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  AL0IB %d", Processor.savedPC, alpha));
			
			Processor.Push(Memory.ReadMDS(Processor.LF, alpha));
		}
	}
	
	// 0274 MUL
	@RegisterOpcode(MUL)
	public static final class OP_MUL implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  MUL", Processor.savedPC));
			
			int t = Util.asInt(Processor.Pop());
			int s = Util.asInt(Processor.Pop());
			Processor.PushLong(s * t);
			Processor.Discard();
		}
	}
	
	// 0275 DCMP
	@RegisterOpcode(DCMP)
	public static final class OP_DCMP implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  DCMP", Processor.savedPC));
			
			// k and j need to be signed for comparison
			int k = Processor.PopLong();
			int j = Processor.PopLong();
			
			Processor.Push((k == j) ? (short)0 : ((j > k) ? (short)1 : (short)-1));
		}
	}
	
	// 0276 UDCMP
	@RegisterOpcode(UDCMP)
	public static final class OP_UDCMP implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  UDCMP", Processor.savedPC));
			
			// k and j need to be unsigned for comparison
			long k = Util.asLong(Processor.PopLong());
			long j = Util.asLong(Processor.PopLong());
			
			Processor.Push((k == j) ? (short)0 : ((j > k) ? (short)1 : (short)-1));
		}
	}
	
	// 0277 VMFIND
	@RegisterOpcode(VMFIND)
	public static final class OP_VMFIND implements Runnable {
		@Override
		public void run() {
//			CARD16 pRunTop = Pop();
//			CARD32 rBase   = PopLong();
//			CARD32 page    = PopLong();
			int pRunTop = Util.asInt(Processor.Pop());
			int rBase   = Processor.PopLong();
			int page    = Processor.PopLong();
//			if (DEBUG_TRACE_OPCODE) logger.debug("TRACE %6o  zVMFIND  %06X  %08X  %04X", savedPC, page, rBase, pRunTop);
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  zVMFIND  %06X  %08X  %04X", Processor.savedPC, page, rBase, pRunTop));
//			CARD16 found = 0;
//			CARD16 pRun  = 0;
			short found = 0;
			int   pRun  = 0;
//			//pageTop: VM.PageNumber = FIRST[VM.PageNumber] + StoragePrograms.countVM;  --end of VM
//			CARD32 pageTop = 0 + Memory::getVPSize();
			int pageTop = 0 + Memory.GetVMPage();
			
//			//indexRunFirst: CARDINAL = (pRunFirst - FIRST[PRun])/SIZE[Run];
//			const CARD16 indexRunFirst = (VMDataInternal::pRunFirst - 0) / SIZE(VMDataInternal::Run);
			int indexRunFirst = (Type.VMDataInternal.pRunFirst - 0) / Type.VMDataInternal.Run.SIZE;
	
//			//IF page >= pageTop THEN Bug[beyondVM];
//			if (pageTop <= page) ERROR();
			if (pageTop <= page) throw new MesaAssertionError();

			{
//				//indexRunLow = indexRunFirst;
//				CARD16 indexRunLow = indexRunFirst;
				int indexRunLow = indexRunFirst;

//				//indexRunHigh = (pRunTop - FIRST[PRun])/SIZE[Run];
//				CARD16 indexRunHigh = (pRunTop - 0) / SIZE(VMDataInternal::Run);
				int indexRunHigh = (pRunTop - 0) / Type.VMDataInternal.Run.SIZE;

//				//DO  --UNTIL search terminates--
				for(;;) {
//					//  indexRun = (indexRunLow + indexRunHigh)/2;
//					CARD16 indexRun = (indexRunLow + indexRunHigh) / 2;
					int indexRun = (indexRunLow + indexRunHigh) / 2;

//					//pageComp = rBase[FIRST[PRun] + indexRun*SIZE[Run]].interval.page;
//					CARD32 pageComp  = ReadDbl(rBase + 0 + (indexRun * SIZE(VMDataInternal::Run)) + OFFSET(VMDataInternal::Run, interval.page));
					int pageComp = Type.VMDataInternal.Run.intervalPage.get(rBase + 0 + (indexRun * Type.VMDataInternal.Run.SIZE));

					if (page < pageComp)
						indexRunHigh = indexRun - 1;
					else
						if (pageComp < page) indexRunLow = indexRun + 1;
						else {
//							// Exact => {pRun = FIRST[PRun] + indexRun*SIZE[Run]; found = TRUE};
//							pRun = 0 + indexRun * SIZE(VMDataInternal::Run);
							pRun = 0 + indexRun * Type.VMDataInternal.Run.SIZE;
							found = 1;
							break;
						}

//					//  IF indexRunHigh < indexRunLow THEN GO TO NotExact;
					if (indexRunHigh < indexRunLow) {
//						//  NotExact =>
//						//    -- Assert: page>"indexRunHigh".page AND page<"indexRunHigh+1".page AND indexRunHigh+1 = indexRunLow.
//						//    IF indexRunLow = indexRunFirst THEN {pRun = pRunFirst; found = FALSE}
//						//    ELSE
//						//      BEGIN
//						//      pRun = FIRST[PRun] + indexRunHigh*SIZE[Run];
//						//      IF page < rBase[pRun].interval.page + rBase[pRun].interval.count THEN
//						//        found = TRUE
//						//      ELSE {pRun = pRun + SIZE[Run]; found = FALSE};
//						//      END;
						if (indexRunLow == indexRunFirst) {
//							pRun = VMDataInternal::pRunFirst;
							pRun = Type.VMDataInternal.pRunFirst;
							found = 0;
						} else {
//							pRun = 0 + indexRunHigh * SIZE(VMDataInternal::Run);
//							const CARD32 intervalPage  = ReadDbl(rBase + pRun + OFFSET(VMDataInternal::Run, interval.page));
//							const CARD32 intervalCount = ReadDbl(rBase + pRun + OFFSET(VMDataInternal::Run, interval.count));
							pRun = 0 + indexRunHigh * Type.VMDataInternal.Run.SIZE;
							int intervalPage  = Type.VMDataInternal.Run.intervalPage.get(rBase + pRun);
							int intervalCount = Type.VMDataInternal.Run.intervalCount.get(rBase + pRun);
							if (page < (intervalPage + intervalCount)) {
								found = 1;
							} else {
//								pRun = pRun + SIZE(VMDataInternal::Run);
								pRun = pRun + Type.VMDataInternal.Run.SIZE;
								found = 0;
							}
						}
						break;
					}
				}
			}
//			//ENDLOOP;  --DO UNTIL search terminates--
//			//EXITS
//			//  Exact => {pRun = FIRST[PRun] + indexRun*SIZE[Run]; found = TRUE};
//			//  NotExact =>
//			//    -- Assert: page>"indexRunHigh".page AND page<"indexRunHigh+1".page AND indexRunHigh+1 = indexRunLow.
//			//    IF indexRunLow = indexRunFirst THEN {pRun = pRunFirst; found = FALSE}
//			//    ELSE
//			//      BEGIN
//			//      pRun = FIRST[PRun] + indexRunHigh*SIZE[Run];
//			//      IF page < rBase[pRun].interval.page + rBase[pRun].interval.count THEN
//			//        found = TRUE
//			//      ELSE {pRun = pRun + SIZE[Run]; found = FALSE};
//			//      END;
//			//END;  --scope of SameAsLastTime--
//	
//			Push(found);
//			Push(pRun);
			Processor.Push(found);
			Processor.Push((short)pRun);
		}
	}
}
