package mh.majuro;


@SuppressWarnings("serial")
public class UnexpectedException extends RuntimeException {
	public UnexpectedException() {
		super();
	}
	public UnexpectedException(String message) {
		super(message);
	}
	public UnexpectedException(String message, Throwable throwable) {
		super(message, throwable);
	}
}
