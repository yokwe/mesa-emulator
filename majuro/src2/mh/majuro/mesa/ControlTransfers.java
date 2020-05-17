package mh.majuro.mesa;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import mh.majuro.UnexpectedException;
import mh.majuro.mesa.Type.CARD16;
import mh.majuro.mesa.Type.CARD32;
import mh.majuro.mesa.Type.CARD8;
import mh.majuro.mesa.Type.LONG_POINTER;
import mh.majuro.mesa.Type.POINTER;

public final class ControlTransfers {
	private static final Logger logger = LoggerFactory.getLogger(ControlTransfers.class);

	// 9.5.3 Trap Handlers
	public static void SaveStack(@LONG_POINTER int state) {
		// FIXME
	}
	public static void LoadStack(@LONG_POINTER int state) {
		// FIXME
	}

	// 9.3 Control Transfer primitives
	// XferType: TYPE = MACHINE DEPENDENT {
	//   return(0), call(l), local(2), port(3),
	//   xfer(4), trap(5), processSwitch(6), unused(7));
	public static enum XferType {
		RETURN        (0),
		CALL          (1),
		LOCAL_CALL    (2),
		PORT          (3),
		XFER          (4),
		TRAP          (5),
		PROCESS_SWITCH(6),
		UNUSED        (7);
		
		public static XferType getInstance(int value) {
			for(XferType xferType: XferType.values()) {
				if (xferType.value == value) return xferType;
			}
			logger.error("Unexpected value = {}", value);
			throw new UnexpectedException();
		}
		
		public final @CARD8 int value;
		
		XferType(int value) {
			this.value = value;
		}
	}
	
	public static void XFER(@CARD32 int dst, @CARD16 int src, XferType type, boolean freeFlag) {
		// FIXME
	}

	// 9.5.1 Trap Routines
	public static void boundsTrap() {
		// FIXME
	}
	public static void breakTrap() {
		// FIXME
	}
	public static void codeTrap(@POINTER int gfi) {
		// FIXME
	}
	public static void controlTrap(@CARD16 int src) {
		// FIXME
	}
	public static void divCheckTrap() {
		// FIXME
	}
	public static void divZeroTrap() {
		// FIXME
	}
	public static void escOpcodeTrap(@CARD8 int opcode) {
		// FIXME
	}
	public static void interruptError() {
		// FIXME
	}
	public static void opcodeTrap(@CARD8 int opcode) {
		// FIXME
	}
	public static void pointerTrap() {
		// FIXME
	}
	public static void processTrap() {
		// FIXME
	}
	public static void rescheduleError() {
		// FIXME
	}
	public static void stackError() {
		logger.error("stackError");
		throw new UnexpectedException();
	}
	public static void unboundTrap(@CARD32 int dst) {
		// FIXME
	}
	public static void hardwareError() {
		logger.error("hardwareError");
		throw new UnexpectedException();
	}


}
