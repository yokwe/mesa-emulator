package mesa.courier.compiler;

@SuppressWarnings("serial")
public class CompilerException extends RuntimeException {
	public CompilerException(String msg, Exception e) {
		super(msg, e);
	}
	public CompilerException(String msg) {
		super(msg);
	}
}
