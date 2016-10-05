package mh.majuro.mesa.opcode;

import static mh.majuro.mesa.Debug.TRACE_OPCODE;
import static mh.majuro.mesa.OpcodeInfo.*;
import mh.majuro.mesa.Memory;
import mh.majuro.mesa.MesaAssertionError;
import mh.majuro.mesa.Processor;
import mh.majuro.mesa.Util;
import mh.majuro.mesa.Opcode.RegisterOpcode;
import mh.majuro.mesa.Type;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class OpcodeEsc0xx {
	protected static final Logger logger = LoggerFactory.getLogger(OpcodeEsc0xx.class);

	// 02 MW
	// TODO implements MW
	// 03 MR
	// TODO implements MR
	// 04 NC
	// TODO implements NC
	// 05 BC
	// TODO implements BC
	// 06 REQ
	// TODO implements REQ
	// 07 SM
	@RegisterOpcode(SM)
	public static final class OP_SM implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  SM", Processor.savedPC));
			short mf = Processor.Pop();
			int   rp = Processor.PopLong();
			int   vp = Processor.PopLong();
			
			Memory.WriteFlag(vp, mf);
			Memory.WriteMap(vp, rp);
		}
	}

	// 010 SMF
	@RegisterOpcode(SMF)
	public static final class OP_SMF implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  SMF", Processor.savedPC));
			short newMF = Processor.Pop();
			int   vp    = Processor.PopLong();
			
			short mf = Memory.ReadFlag(vp);
			int   rp = Memory.ReadMap(vp);
			Processor.Push(mf);
			Processor.PushLong(rp);
			
			if (!Type.MapFlags.isVacant(mf)) {
				Memory.WriteMap(vp, rp);
				Memory.WriteFlag(vp, newMF);
			}
		}
	}
	// 011 GMF
	@RegisterOpcode(GMF)
	public static final class OP_GMF implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  GMF", Processor.savedPC));
			int   vp = Processor.PopLong();

			short mf = Memory.ReadFlag(vp);
			int   rp = Memory.ReadMap(vp);
			Processor.Push(mf);
			Processor.PushLong(rp);
		}
	}
	// 012 AF
	// TODO implements AF
	// 013 FF
	// TODO implements FF
	// 014 PI
	// TODO implements PI
	// 015 PO
	// TODO implements PO
	// 016 POR
	// TODO implements POR
	// 017 SPP
	// TODO implements SPP

	// 020 DI
	// TODO implements DI
	// 021 EI
	// TODO implements EI
	// 022 XOR
	@RegisterOpcode(XOR)
	public static final class OP_XOR implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  XOR", Processor.savedPC));
			
			short v = Processor.Pop();
			short u = Processor.Pop();
			Processor.Push((short)(u ^ v));
		}
	}

	// 023 DAND
	@RegisterOpcode(DAND)
	public static final class OP_DAND implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  DAND", Processor.savedPC));
			
			int v = Processor.PopLong();
			int u = Processor.PopLong();
			Processor.PushLong((u & v));
		}
	}

	// 024 DIOR
	@RegisterOpcode(DIOR)
	public static final class OP_DIOR implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  DIOR", Processor.savedPC));
			
			int v = Processor.PopLong();
			int u = Processor.PopLong();
			Processor.PushLong((u | v));
		}
	}

	// 025 DXOR
	@RegisterOpcode(DXOR)
	public static final class OP_DXOR implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  DXOR", Processor.savedPC));
			
			int v = Processor.PopLong();
			int u = Processor.PopLong();
			Processor.PushLong((u ^ v));
		}
	}

	// 026 ROTATE
	@RegisterOpcode(ROTATE)
	public static final class OP_ROTATE implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  ROTATE", Processor.savedPC));
			
			short rotate = Processor.Pop();
			short u      = Processor.Pop();
			Processor.Push(Util.Rotate(u, rotate));
		}
	}
	// 027 DSHIFT
	@RegisterOpcode(DSHIFT)
	public static final class OP_DSHIFT implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  DSHIFT", Processor.savedPC));
			
			short shift = Processor.Pop();
			int   u     = Processor.PopLong();
			Processor.PushLong(Util.LongShift(u, shift));
		}
	}

	// 030 LINT
	@RegisterOpcode(LINT)
	public static final class OP_LINT implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LINT", Processor.savedPC));
			
			short i = Processor.Pop();
			Processor.Push(i);
			Processor.Push((i < 0) ? (short)-1 : (short)0);
		}
	}

	// 031 JS
	@RegisterOpcode(JS)
	public static final class OP_JS implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  JS", Processor.savedPC));
			
			Processor.PC = Processor.Pop();
		}
	}
	
	// 032 RCFS
	@RegisterOpcode(RCFS)
	public static final class OP_RCFS implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  RCFS", Processor.savedPC));
			
			short desc = Processor.Pop();
			int   offset = Util.asInt(Processor.Pop());

			Processor.Push(Memory.ReadField(Memory.ReadCode(offset + Util.HighByte(desc)), (byte)desc));
		}
	}
	// 033 RC
	@RegisterOpcode(RC)
	public static final class OP_RC implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  RC %02X", Processor.savedPC, alpha));
			
			int offset = Util.asInt(Processor.Pop());

			Processor.Push(Memory.ReadCode(offset + alpha));
		}
	}
	// 034 UDIV
	@RegisterOpcode(UDIV)
	public static final class OP_UDIV implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  UDIV", Processor.savedPC));
			
			// calculate as unsigned number
			int k = Util.asInt(Processor.Pop());
			int j = Util.asInt(Processor.Pop());
			
			if (k == 0) Processor.DivZeroTrap();
			Processor.Push((short)(j / k));
			Processor.Push((short)(j % k));
		}
	}
	// 035 LUDIV
	@RegisterOpcode(LUDIV)
	public static final class OP_LUDIV implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  LUDIV", Processor.savedPC));
			
			// calculate as unsigned number
			long t = Util.asInt(Processor.Pop());
			long s = Util.asLong(Processor.PopLong());
			
			if (t == 0) Processor.DivZeroTrap();
			if (t < Util.HighHalf((int)s)) Processor.DivCheckTrap();
			Processor.Push((short)(s / t));
			Processor.Push((short)(s % t));
			Processor.Discard();
		}
	}
	// 036 ROB
	@RegisterOpcode(ROB)
	public static final class OP_ROB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  ROB %02X", Processor.savedPC, alpha));
			
			int ptr = Util.asInt(Processor.Pop());
			if (alpha < 1 || 4 < alpha) throw new MesaAssertionError();

			Processor.Push(Memory.ReadMDS((short)(ptr - alpha)));
		}
	}
	// 037 WOB
	@RegisterOpcode(WOB)
	public static final class OP_WOB implements Runnable {
		@Override
		public void run() {
			int alpha = Memory.GetCodeByte();
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  WOB %02X", Processor.savedPC, alpha));
			
			int ptr = Util.asInt(Processor.Pop());
			if (alpha < 1 || 4 < alpha) throw new MesaAssertionError();
			short newValue = Processor.Pop();
			
			Memory.WriteMDS((short)(ptr - alpha), newValue);
		}
	}

	// 040 DSK
	// TODO implements DSK
	// 041 XE
	// TODO implements XE
	// 042 XF
	// TODO implements XF
	// 043 LSK
	// TODO implements LSK
	// 044 BNDCKL
	@RegisterOpcode(BNDCKL)
	public static final class OP_BNDCKL implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  BNDCKL", Processor.savedPC));
			int range = Processor.PopLong();
			int index = Processor.PopLong();
			Processor.PushLong(index);
			if (Util.asLong(range) <= Util.asLong(index)) Processor.BoundsTrap();
		}
	}
	// 045 NILCK
	@RegisterOpcode(NILCK)
	public static final class OP_NILCK implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  NILCK", Processor.savedPC));
			short ptr = Processor.Pop();
			Processor.Push(ptr);
			if (ptr == 0) Processor.PointerTrap();
		}
	}
	// 046 NILCKL
	@RegisterOpcode(NILCKL)
	public static final class OP_NILCKL implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  NILCKL", Processor.savedPC));
			int ptr = Processor.PopLong();
			Processor.PushLong(ptr);
			if (ptr == 0) Processor.PointerTrap();
		}
	}
	// 047 BLTLR
	// TODO implements BLTLR
	
	// 050 BLEL
	// TODO implements BLEL
	// 051 BLECL
	// TODO implements BLECL
	// 052 CKSUM
	// TODO implements CKSUM
	// 053 BITBLT
	// TODO implements BITBLT
	// 054 TXTBLT
	// TODO implements TXTBLT
	// 055 BYTBLT
	// TODO implements BYTBLT
	// 056 BYTBLTR
	// TODO implements BYTBLTR
	// 057 VERSION
	@RegisterOpcode(VERSION)
	public static final class OP_VERSION implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.error(String.format("TRACE %6o  VERSION", Processor.savedPC));
			short word0 = 0;			
			word0 = Type.ProcessorFaceExtras.VersionResult.machineType.setData(word0, Type.ProcessorFaceExtras.daybreak);
			word0 = Type.ProcessorFaceExtras.VersionResult.majorVersion.setData(word0, 0);
			word0 = Type.ProcessorFaceExtras.VersionResult.unused.setData(word0, 0);
			word0 = Type.ProcessorFaceExtras.VersionResult.floatingPoint.setData(word0, 0);
			word0 = Type.ProcessorFaceExtras.VersionResult.cedar.setData(word0, 0);
			
			short word1 = (short)40908; // days since 1901-01-01 to 2013-01-01

			Processor.Push(word0);
			Processor.Push(word1);
		}
	}

	// 060 DMUL
	@RegisterOpcode(DMUL)
	public static final class OP_DMUL implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  DMUL", Processor.savedPC));
			
			// calculate as unsigned number
			long t = Util.asLong(Processor.PopLong());
			long s = Util.asLong(Processor.PopLong());
			Processor.PushLong((int)(t * s));
		}
	}
	// 061 SDIV
	@RegisterOpcode(SDIV)
	public static final class OP_SDIV implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  SDIV", Processor.savedPC));
			
			// calculate as signed number
			short k = Processor.Pop();
			short j = Processor.Pop();
			
			if (k == 0) Processor.DivZeroTrap();
			Processor.Push((short)(j / k));
			Processor.Push((short)(j % k));
		}
	}
	// 062 SDDIV
	@RegisterOpcode(SDDIV)
	public static final class OP_SDDIV implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  SDDIV", Processor.savedPC));
			
			// calculate as signed number
			int k = Processor.PopLong();
			int j = Processor.PopLong();
			
			if (k == 0) Processor.DivZeroTrap();
			Processor.PushLong(j / k);
			Processor.PushLong(j % k);
			Processor.Discard();
			Processor.Discard();
		}
	}
	// 063 UDDIV
	@RegisterOpcode(UDDIV)
	public static final class OP_UDDIV implements Runnable {
		@Override
		public void run() {
			if (TRACE_OPCODE) logger.debug(String.format("TRACE %6o  UDDIV", Processor.savedPC));
			
			// calculate as unsigned number
			long t = Util.asLong(Processor.PopLong());
			long s = Util.asLong(Processor.PopLong());
			
			if (t == 0) Processor.DivZeroTrap();
			Processor.PushLong((int)(s / t));
			Processor.PushLong((int)(s % t));
			Processor.Discard();
			Processor.Discard();
		}
	}
}
