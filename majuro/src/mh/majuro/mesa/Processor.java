package mh.majuro.mesa;

public final class Processor {
	public static final int qFrameFault        = 0;
	public static final int qPageFault         = 1;
	public static final int qWriteProtectFault = 2;

	public static final int sBoot            = 001; //  1
	public static final int sBoundsTrap      = 016; // 14
	public static final int sBreakTrap       = 000; //  0
	public static final int sCodeTrap        = 007; //  7
	public static final int sControlTrap     = 006; //  6
	public static final int sDivCheckTrap    = 013; // 11
	public static final int sDivZeroTrap     = 012; // 10
	public static final int sInterruptError  = 014; // 12
	public static final int sOpcodeTrap      = 005; //  5
	public static final int sPointerTrap     = 017; // 15
	public static final int sProcessTrap     = 015; // 13
	public static final int sRescheduleError = 003; //  3
	public static final int sStackError      = 002; //  2
	public static final int sUnboundTrap     = 011; //  9
	public static final int sXferTrap        = 004; //  4
	public static final int sHardwareError   = 010; //  8

	//
	// Stack related declaration
	//
	public  static final int StackDepth = 14;
	public  static short     stack[] = new short[StackDepth];
	public  static short     SP;
	
	public static void Push(short value) {
		if (SP == StackDepth) Processor.StackError();
		stack[SP++] = value;
	}
	public static short Pop() {
		if (SP == 0) StackError();
		return stack[--SP];
	}
	public static void PushLong(int value) {
		Push(Util.LowHalf(value));
		Push(Util.HighHalf(value));
	}
	public static int PopLong() {
		short highHalf = Pop();
		short lowHalf  = Pop();
		return Util.asInt(highHalf, lowHalf);
	}
	public static void MinimalStack() {
		if (SP != 0) StackError();
	}
	public static void Recover() {
		if (SP == StackDepth) StackError();
		SP++;
	}
	public static void Discard() {
		if (SP == 0) StackError();
		SP--;
	}


	//
	// Processor registers
	//
	public static short PID[] = new short[4];
	public static short MP;
	public static short XTS;
	public static short PSB;
	public static int   MDS;
	public static short LF; 
	public static int   GF;
	public static int   CB;
	public static short PC;
	public static short GFI;

	// Processor related variables
	// Memory.GetCodeByte returns int. So breakByte should be int.
	public static int   breakByte;
	public static short savedPC;
	public static short savedSP;
	public static int   running;
	
	// Initialize registers and fields
	public static void initialize() {
		for(int i = 0; i < stack.length; i++) stack[i] = 0;
		SP = 0;
		for(int i = 0; i < PID.length; i++) PID[i] = 0;
		MP  = 0;
		XTS = 0;
		PSB = 0;
		MDS = 0;
		LF  = 0;
		GF  = 0;
		CB  = 0;
		PC  = 0;
		GFI = 0;
		
		breakByte = 0;
		savedPC   = 0;
		savedSP   = 0;
		running   = 0;
	}

	public static void PageFault(int virtualAddress) {
		// TODO implement this
		throw new MesaAbort("No Implementation");
	}
	public static void WriteProtectFault(int virtualAddress) {
		// TODO implement this
		throw new MesaAbort("No Implementation");
	}
	public static void StackError() {
		// TODO implement this
		throw new MesaAbort("No Implementation");
	}
	public static void OpcodeTrap(int opcode) {
		// TODO implement this
		throw new MesaAbort("No Implementation");
	}
	public static void EscOpcodeTrap(int opcode) {
		// TODO implement this
		throw new MesaAbort("No Implementation");
	}
	public static void BoundsTrap() {
		// TODO implement this
		throw new MesaAbort("No Implementation");
	}
	public static void BreakTrap() {
		// TODO implement this
		throw new MesaAbort("No Implementation");
	}
	public static void PointerTrap() {
		// TODO implement this
		throw new MesaAbort("No Implementation");
	}
	public static void DivZeroTrap() {
		// TODO implement this
		throw new MesaAbort("No Implementation");
	}
	public static void DivCheckTrap() {
		// TODO implement this
		throw new MesaAbort("No Implementation");
	}

}
