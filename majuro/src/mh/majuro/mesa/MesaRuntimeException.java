package mh.majuro.mesa;

@SuppressWarnings("serial")
public class MesaRuntimeException extends RuntimeException {
	public MesaRuntimeException() {
		super();
	}
	
	public MesaRuntimeException(String msg) {
		super(msg);
	}
}
