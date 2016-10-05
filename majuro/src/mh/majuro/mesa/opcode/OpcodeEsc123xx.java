package mh.majuro.mesa.opcode;

import static mh.majuro.mesa.Debug.TRACE_OPCODE;
import static mh.majuro.mesa.OpcodeInfo.*;
import mh.majuro.mesa.Processor;
import mh.majuro.mesa.Opcode.RegisterOpcode;
import mh.majuro.mesa.Util;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class OpcodeEsc123xx {
	protected static final Logger logger = LoggerFactory.getLogger(OpcodeEsc123xx.class);

	// Floating Point (100B-137B are reserved)
	// 0100 FADD
	// 0101 FSUB
	// 0102 FMUL
	// 0103 FDIV
	// 0104 FCOMP
	// 0105 FIX
	// 0106 FLOAT
	// 0107 FIXI
	
	// 0110 FIXC
	// 0111 FSTICKY
	// 0112 FREM
	// 0113 ROUND
	// 0114 ROUNDI
	// 0115 ROUNDC
	// 0116 FSQRT
	//

	//  Read / Write Registers
	// 0160 WRPSB
	// TODO implements WRPSB
	// 0161 WRMDS
	@RegisterOpcode(WRMDS)
	public static final class OP_WRMDS implements Runnable {
		@Override
		public void run() {
			short value = Processor.Pop();
			if (TRACE_OPCODE) logger.error(String.format("TRACE %6o  WRMDS %04X", Processor.savedPC, value));
			Processor.MDS = Util.asInt(value, (short)0);
		}
	}
	// 0162 WRWP
	// TODO implements WRWP
	// 0163 WRWDC
	// TODO implements WRWDC
	// 0164 WRPTC
	// TODO implements WRPTC
	// 0165 WRIT
	// TODO implements WRIT
	// 0166 WRXTS
	@RegisterOpcode(WRXTS)
	public static final class OP_WRXTS implements Runnable {
		@Override
		public void run() {
			Processor.XTS = Processor.Pop();
			if (TRACE_OPCODE) logger.error(String.format("TRACE %6o  WRXTS %04X", Processor.savedPC, Processor.XTS));
		}
	}
	// 0167 WRMP
	// TODO implements WRMP
	
	// 0170 RRPSB
	// TODO implements RRPSB
	// 0171 RRMDS
	@RegisterOpcode(RRMDS)
	public static final class OP_RRMDS implements Runnable {
		@Override
		public void run() {
			short value = Util.HighHalf(Processor.MDS);
			if (TRACE_OPCODE) logger.error(String.format("TRACE %6o  RRMDS %04X", Processor.savedPC, value));
			Processor.Push(value);
		}
	}
	// 0172 RRWP
	// TODO implements RRWP
	// 0173 RRWDC
	// TODO implements RRWDC
	// 0174 RRPTC
	// TODO implements RRPTC
	// 0175 RRIT
	// TODO implements RRIT
	// 0176 RRXTS
	@RegisterOpcode(RRXTS)
	public static final class OP_RRXTS implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.error(String.format("TRACE %6o  RRXTS %04X", Processor.savedPC, Processor.XTS));
			Processor.Push(Processor.XTS);
		}
	}

	
	// Guam
	//   GuamInputOutput.mesa
	// 0211 CALLAGENT
	// TODO implements CALLAGENT
	// 0212 MAPDISPLAY
	// TODO implements MAPDISPLAY
	// 0213 STOPEMULATOR
	// TODO implements STOPEMULATOR
	// 0214 A214
	@RegisterOpcode(A214)
	public static final class OP_A214 implements Runnable {
		@Override
		public void run() {
			// TODO Implements OP_A214
			if (TRACE_OPCODE) logger.error(String.format("TRACE %6o  A214", Processor.savedPC));
			Processor.PopLong();
			Processor.PopLong();
			Processor.PopLong();
			Processor.Push((short)0);
		}
	}


	// ColorBlt.mesa
	// 0300 COLORBLT
	// TODO implements COLORBLT
	// 0301 WRITEPIXEL
	// TODO implements WRITEPIXEL
	// 0302 BITBLTX
	// TODO implements BITBLTX
	//
	//
	// 0305 A305
	@RegisterOpcode(A305)
	public static final class OP_A305 implements Runnable {
		@Override
		public void run() {
			// TODO Implements OP_A305
			if (TRACE_OPCODE) logger.error(String.format("TRACE %6o  A305", Processor.savedPC));
		}
	}

	// 0306 A306
	@RegisterOpcode(A306)
	public static final class OP_A306 implements Runnable {
		@Override
		public void run() {
			// TODO Implements OP_A306
			short n = Processor.Pop();
			if (TRACE_OPCODE) logger.error(String.format("TRACE %6o  A306　%d", Processor.savedPC, n));
			Processor.Push((short)0);
		}
	}
}
