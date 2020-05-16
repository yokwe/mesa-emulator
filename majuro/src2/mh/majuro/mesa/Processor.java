package mh.majuro.mesa;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import mh.majuro.UnexpectedException;
import mh.majuro.mesa.Type.*;
import mh.majuro.mesa.Type.LONG_POINTER;
import mh.majuro.mesa.type.Long;

public final class Processor {
	private static final Logger logger = LoggerFactory.getLogger(Processor.class);

	public static final int StackDepth = Constant.cSS;
	
	// 3.3.2 Evaluation Stack
	private static final @CARD16 int[] stack = new int[StackDepth];
	private static               int   SP; // [0..StackDepth)
	
	// 3.3.2 Evaluation Stack
	public static void stackError() {
		logger.error("StackError");
		throw new UnexpectedException();
	}
	public static int stackCount() {
		return SP;
	}
	public static void push(@CARD16 int data) {
		if (SP == StackDepth) stackError();
		stack[SP++] = data;
	}
	public static @CARD16 int pop() {
		if (SP == 0) stackError();
		return stack[--SP];
	}
	// Note that double-word quantities are placed on the stack so that
	// the least significant word is at the lower-numbered stack index.
	public static void pushLong(@CARD32 int data) {
//		Long t = {data};
//		Push(t.low);
//		Push(t.high);
		push(Long.low(data));
		push(Long.high(data));
	}
	public static @CARD32 int popLong() {
//		Long t;
//		t.high = Pop();
//		t.low = Pop();
//		return t.u;
		int t0 = pop();
		int t1 = pop();
		return Long.make(t0, t1);
	}
	public static void minimalStack() {
		if (SP != 0) stackError();
	}

	public static void recover() {
		if (SP == StackDepth) stackError();
		SP++;
	}
	public static void discard() {
		if (SP == 0) stackError();
		SP--;
	}

	// 9.5.3 Trap Handlers
	public static void SaveStack(@LONG_POINTER int state) {
		// FIXME
	}
	public static void LoadStack(@LONG_POINTER int state) {
		// FIXME
	}

	
	// 3.3.3 Data and Status Registers
	// Processor ID
	public static final short[] PID = new short[4];
	// Maintenance Panel
	public static @CARD16 int MP;
	// Xfer trap status - 9.5.5
	public static @CARD16 int XTS;
	
	// 3.3.1 Control Registers
	// PsbIndex - 10.1.1
	public static @CARD16       int PSB;
	public static @LONG_POINTER int GF;  // LONG POINTER TO GlobalVarables
	public static @CARD16       int GFI;

	// 4.5 Instruction Execution
	public static @CARD8  int breakByte;
	public static @CARD16 int savedPC;
	public static @CARD16 int savedSP;
}
