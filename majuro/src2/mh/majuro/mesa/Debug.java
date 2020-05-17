package mh.majuro.mesa;

public final class Debug {
	// Procesor.AbortRuntimeException
	public static final boolean ENABLE_ABORT_STACK_TRACE = false;
		
	// Keyboard and Mouse
	public static final boolean SHOW_EVENT_KEY       = false;
	public static final boolean SHOW_EVENT_MOUSE     = false;

	// Network
	public static final boolean SHOW_NETWORK_PACKET  = false;
	public static final boolean TRACE_NETWORK_PACKET = false;

	// Debug emulator
	public static final boolean SHOW_DUMMY_IMPL_OPCODE = false;
	public static final boolean SHOW_RUNNING           = false;
	public static final boolean TRACE_OPCODE           = false;
	public static final boolean SHOW_OPCODE_STATS      = false;
	public static final boolean TRACE_XFER             = false;

	// Show Fault
	public static final boolean SHOW_FRAME_FAULT         = false;
	public static final boolean SHOW_PAGE_FAULT          = false;
	public static final boolean SHOW_WRITE_PROTECT_FAULT = true;

	// Show Trap
	public static final boolean SHOW_BOUNDS_TRAP     = false;
	public static final boolean SHOW_BREAK_TRAP      = false;
	public static final boolean SHOW_CODE_TRAP       = false;
	public static final boolean SHOW_CONTROL_TRAP    = false;
	public static final boolean SHOW_DIV_CHECK_TRAP  = true;
	public static final boolean SHOW_DIV_ZERO_TRAP   = true;
	public static final boolean SHOW_ESC_OPCODE_TRAP = false;
	public static final boolean SHOW_INTERRUPT_ERROR = true;
	public static final boolean SHOW_OPCODE_TRAP     = false;
	public static final boolean SHOW_POINTER_TRAP    = true;
	public static final boolean SHOW_PROCESS_TRAP    = true;
	public static final boolean SHOW_RESCHEDULE_TRAP = true;
	public static final boolean SHOW_STACK_ERROR     = true;
	public static final boolean SHOW_UNBOUND_TRAP    = true;
	public static final boolean SHOW_HARDWARE_ERROR  = true;
	public static final boolean SHOW_XFER_TRAP       = true;

	// Show Agent
	public static final boolean SHOW_AGENT_BEEP      = false;
	public static final boolean SHOW_AGENT_DISK      = false;
	public static final boolean SHOW_AGENT_DISPLAY   = false;
	public static final boolean SHOW_AGENT_FLOPPY    = false;
	public static final boolean SHOW_AGENT_MOUSE     = false;
	public static final boolean SHOW_AGENT_NETWORK   = false;
	public static final boolean SHOW_AGENT_PROCESSOR = false;
	public static final boolean SHOW_AGENT_STREAM    = false;

	// Stream
	public static final boolean SHOW_STREAM_BOOT     = true;
	public static final boolean SHOW_STREAM_COPYPASTE= false;
	public static final boolean SHOW_STREAM_PCFA     = false;
	public static final boolean SHOW_STREAM_TCP      = false;
	public static final boolean SHOW_STREAM_WWC      = false;

	// Stop emulator at
	public static final boolean STOP_AT_CONTROL_TRAP = true;
	public static final boolean STOP_AT_UNBOUND_TRAP = false;
	public static final boolean STOP_AT_OPCODE_TRAP  = false;
	public static final boolean STOP_AT_PAGE_FAULT   = false;
	public static final boolean STOP_AT_MP_915       = false;
	public static final boolean STOP_AT_STACK_ERROR  = true;
	public static final boolean STOP_AT_NOT_RUNNING  = false;

	public static final boolean STOP_MESSAGE_UNTIL_MP = false;

}
