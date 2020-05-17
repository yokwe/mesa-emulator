package mh.majuro.mesa;

import mh.majuro.UnexpectedException;
import mh.majuro.mesa.Type.CARD16;
import mh.majuro.mesa.Type.CARD32;
import mh.majuro.mesa.Type.CARD8;
import mh.majuro.mesa.Type.LONG_POINTER;

public final class Processor {
	public static final int StackDepth = Constant.cSS;
	
	// 3.3.2 Evaluation Stack
	public static final @CARD16 int[] stack = new int[StackDepth];
	public static       @CARD16 int   SP; // [0..StackDepth)
	
	// 3.3.2 Evaluation Stack
	public static int stackCount() {
		return SP;
	}
	public static void push(@CARD16 int data) {
		if (SP == StackDepth) ControlTransfers.stackError();
		stack[SP++] = data;
	}
	public static @CARD16 int pop() {
		if (SP == 0) ControlTransfers.stackError();
		return stack[--SP];
	}
	// Note that double-word quantities are placed on the stack so that
	// the least significant word is at the lower-numbered stack index.
	public static void pushLong(@CARD32 int data) {
//		Long t = {data};
//		Push(t.low);
//		Push(t.high);
		push(Type.lowHalf(data));
		push(Type.highHalf(data));
	}
	public static @CARD32 int popLong() {
//		Long t;
//		t.high = Pop();
//		t.low = Pop();
//		return t.u;
		int t0 = pop();
		int t1 = pop();
		return Type.makeLong(t0, t1);
	}
	public static void minimalStack() {
		if (SP != 0) ControlTransfers.stackError();
	}

	public static void recover() {
		if (SP == StackDepth) ControlTransfers.stackError();
		SP++;
	}
	public static void discard() {
		if (SP == 0) ControlTransfers.stackError();
		SP--;
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
	
	
	//
	//
	//
	@SuppressWarnings("serial")
	public static final class AbortRuntimeException extends RuntimeException {
		AbortRuntimeException() {
			//    message
			//          cause
			//                enableSuppression
			//                       writableStackTrace
			super(null, null, true, Debug.ENABLE_ABORT_STACK_TRACE);
		}
	};
	
	public static void abort() {
		throw new AbortRuntimeException();
	}
	
	public static void error() {
		throw new UnexpectedException("ERROR");
	}
}
