package mesa.courier.program;

@SuppressWarnings("serial")
public class ProgramException extends RuntimeException {
	public ProgramException(String msg, Exception e) {
		super(msg, e);
	}
	public ProgramException(String msg) {
		super(msg);
	}
}
