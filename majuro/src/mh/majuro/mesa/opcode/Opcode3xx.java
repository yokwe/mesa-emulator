package mh.majuro.mesa.opcode;

import static mh.majuro.mesa.Debug.TRACE_OPCODE;
import static mh.majuro.mesa.OpcodeInfo.*;
import mh.majuro.mesa.Memory;
import mh.majuro.mesa.MesaAssertionError;
import mh.majuro.mesa.Processor;
import mh.majuro.mesa.Opcode.RegisterOpcode;
import mh.majuro.mesa.Util;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public final class Opcode3xx {
	protected static final Logger logger = LoggerFactory.getLogger(Opcode3xx.class);

	static void LIn(final int n) {
		Processor.Push((short)n);
	}

	// 0300 LI0
	@RegisterOpcode(LI0)
	public static final class OP_LI0 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LI0", Processor.savedPC));
			LIn(0);
		}
	}
	
	// 0301 LI1
	@RegisterOpcode(LI1)
	public static final class OP_LI1 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LI1", Processor.savedPC));
			LIn(1);
		}
	}
	
	// 0302 LI2
	@RegisterOpcode(LI2)
	public static final class OP_LI2 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LI2", Processor.savedPC));
			LIn(2);
		}
	}
	
	// 0303 LI3
	@RegisterOpcode(LI3)
	public static final class OP_LI3 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LI3", Processor.savedPC));
			LIn(3);
		}
	}
	
	// 0304 LI4
	@RegisterOpcode(LI4)
	public static final class OP_LI4 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LI4", Processor.savedPC));
			LIn(4);
		}
	}
	
	// 0305 LI5
	@RegisterOpcode(LI5)
	public static final class OP_LI5 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LI5", Processor.savedPC));
			LIn(5);
		}
	}
	
	// 0306 LI6
	@RegisterOpcode(LI6)
	public static final class OP_LI6 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LI6", Processor.savedPC));
			LIn(6);
		}
	}
	
	// 0307 LI7
	@RegisterOpcode(LI7)
	public static final class OP_LI7 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LI7", Processor.savedPC));
			LIn(7);
		}
	}
	

	// 0310 LI8
	@RegisterOpcode(LI8)
	public static final class OP_LI8 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LI8", Processor.savedPC));
			LIn(8);
		}
	}
	
	// 0311 LI9
	@RegisterOpcode(LI9)
	public static final class OP_LI9 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LI9", Processor.savedPC));
			LIn(9);
		}
	}
	
	// 0312 LI10
	@RegisterOpcode(LI10)
	public static final class OP_LI10 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LI10", Processor.savedPC));
			LIn(10);
		}
	}
	
	// 0313 LIN1
	@RegisterOpcode(LIN1)
	public static final class OP_LIN1 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LIN1", Processor.savedPC));
			LIn(0xffff);
		}
	}
	
	// 0314 LINI
	@RegisterOpcode(LINI)
	public static final class OP_LINI implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LINI", Processor.savedPC));
			LIn(0x8000);
		}
	}

	// 0315 LIB
	@RegisterOpcode(LIB)
	public static final class OP_LIB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LIB %02X", Processor.savedPC, alpha));
			LIn(alpha);
		}
	}

	// 0316 LIW
	@RegisterOpcode(LIW)
	public static final class OP_LIW implements Runnable {
		@Override
		public void run() {
			short word = Memory.GetCodeWord();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LIB %04X", Processor.savedPC, word));
			LIn(word);
		}
	}
	
	// 0317 LINB
	@RegisterOpcode(LINB)
	public static final class OP_LINB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LINB %02X", Processor.savedPC, alpha));
			LIn(0xff00 | alpha);
		}
	}

	// 0320 LIHB
	@RegisterOpcode(LIHB)
	public static final class OP_LIHB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LIHB %02X", Processor.savedPC, alpha));
			LIn(alpha << 8);
		}
	}

	// 0321 LID0
	@RegisterOpcode(LID0)
	public static final class OP_LID0 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LID0", Processor.savedPC));
			Processor.PushLong(0);
		}
	}

	static void LAn(final int n) {
		Processor.Push((short)(Processor.LF + n));
	}

	// 0322 LA0
	@RegisterOpcode(LA0)
	public static final class OP_LA0 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LA0", Processor.savedPC));
			LAn(0);
		}
	}

	// 0323 LA1
	@RegisterOpcode(LA1)
	public static final class OP_LA1 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LA1", Processor.savedPC));
			LAn(1);
		}
	}

	// 0324 LA2
	@RegisterOpcode(LA2)
	public static final class OP_LA2 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LA2", Processor.savedPC));
			LAn(2);
		}
	}

	// 0325 LA3
	@RegisterOpcode(LA3)
	public static final class OP_LA3 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LA3", Processor.savedPC));
			LAn(3);
		}
	}

	// 0326 LA6
	@RegisterOpcode(LA6)
	public static final class OP_LA6 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LA6", Processor.savedPC));
			LAn(6);
		}
	}

	// 0327 LA8
	@RegisterOpcode(LA8)
	public static final class OP_LA8 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LA8", Processor.savedPC));
			LAn(8);
		}
	}


	// 0330 LAB
	@RegisterOpcode(LAB)
	public static final class OP_LAB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LAB %02X", Processor.savedPC, alpha));
			LAn(alpha);
		}
	}

	// 0331 LAW
	@RegisterOpcode(LAW)
	public static final class OP_LAW implements Runnable {
		@Override
		public void run() {
			int word = Util.asInt(Memory.GetCodeWord());
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LAW %04X", Processor.savedPC, word));
			LAn(word);
		}
	}

	static void GAn(int n) {
		Processor.Push((short)(Processor.GF + n));
	}

	// 0332 GA0
	@RegisterOpcode(GA0)
	public static final class OP_GA0 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  GA0", Processor.savedPC));
			GAn(0);
		}
	}

	// 0333 GA1
	@RegisterOpcode(GA1)
	public static final class OP_GA1 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  GA1", Processor.savedPC));
			GAn(1);
		}
	}

	// 0334 GAB
	@RegisterOpcode(GAB)
	public static final class OP_GAB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  GAB %02X", Processor.savedPC, alpha));
			GAn(alpha);
		}
	}

	// 0335 GAW
	@RegisterOpcode(GAW)
	public static final class OP_GAW implements Runnable {
		@Override
		public void run() {
			int word = Util.asInt(Memory.GetCodeWord());
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  GAW %04X", Processor.savedPC, word));
			GAn(word);
		}
	}

	//
	// 0337 EFC0
	// TODO implements EFC0

	// 0340 EFC1
	// TODO implements EFC1
	// 0341 EFC2
	// TODO implements EFC2
	// 0342 EFC3
	// TODO implements EFC3
	// 0343 EFC4
	// TODO implements EFC4
	// 0344 EFC5
	// TODO implements EFC5
	// 0345 EFC6
	// TODO implements EFC6
	// 0346 EFC7
	// TODO implements EFC7
	// 0347 EFC8
	// TODO implements EFC8

	// 0350 EFC9
	// TODO implements EFC9
	// 0351 EFC10
	// TODO implements EFC10
	// 0352 EFC11
	// TODO implements EFC11
	// 0353 EFC12
	// TODO implements EFC12
	// 0354 EFCB
	// TODO implements EFC8
	// 0355 LFC
	// TODO implements LFC
	// 0356 SFC
	// TODO implements SFC
	// 0357 RET
	// TODO implements RET

	// 0360 KFCB
	// TODO implements KFCB
	// 0361 ME
	// TODO implements ME
	// 0362 MX
	// TODO implements MX
	// 0363 BLT
	// TODO implements BLT
	// 0364 BLTL
	// TODO implements BLTL
	// 0365 BLTC
	// TODO implements BLTC
	// 0366 BLTCL
	// TODO implements BLTCL
	// 0367 LP
	@RegisterOpcode(LP)
	public static final class OP_LP implements Runnable {
		@Override
		public void run() {
			logger.error(String.format("TRACE %6o  LP", Processor.savedPC));
			short ptr = Processor.Pop();
			Processor.PushLong((ptr == 0) ? 0 : Memory.LengthenPointer(ptr));
		}
	}

	// 0370 ESC
	@RegisterOpcode(ESC)
	public static final class OP_ESC implements Runnable {
		@Override
		public void run() {
			logger.error(String.format("TRACE %6o  ESC", Processor.savedPC));
			throw new MesaAssertionError();
		}
	}
	
	// 0371 ESCL
	@RegisterOpcode(ESCL)
	public static final class OP_ESCL implements Runnable {
		@Override
		public void run() {
			logger.error(String.format("TRACE %6o  ESCL", Processor.savedPC));
			throw new MesaAssertionError();
		}
	}

	static void LGAn(int n) {
		Processor.PushLong(Processor.GF + n);
	}
	
	// 0372 LGA0
	@RegisterOpcode(LGA0)
	public static final class OP_LGA0 implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LGA0", Processor.savedPC));
			LGAn(0);
		}
	}
	
	// 0373 LGAB
	@RegisterOpcode(LGAB)
	public static final class OP_LGAB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LGAB %02X", Processor.savedPC, alpha));
			LGAn(alpha);
		}
	}
	
	// 0374 LGAW
	@RegisterOpcode(LGAW)
	public static final class OP_LGAW implements Runnable {
		@Override
		public void run() {
			int word = Util.asInt(Memory.GetCodeWord());
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LGAW %04X", Processor.savedPC, word));
			LGAn(word);
		}
	}
	
	// 0375 DESC
	@RegisterOpcode(DESC)
	public static final class OP_DESC implements Runnable {
		@Override
		public void run() {
			short word = Memory.GetCodeWord();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  DESC %04X", Processor.savedPC, word));
			Processor.Push((short)(Processor.GFI | 0x0003));
			Processor.Push(word);
		}
	}
	//
	// 0377 RESRVD
	@RegisterOpcode(RESRVD)
	public static final class OP_RESRVD implements Runnable {
		@Override
		public void run() {
			logger.error(String.format("TRACE %6o  RESRVD", Processor.savedPC));
			throw new MesaAssertionError();
		}
	}
}
