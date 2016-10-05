package mh.majuro.mesa;

@SuppressWarnings("serial")
public class MesaAssertionError extends MesaRuntimeException {
	public MesaAssertionError() {
		super();
	}
	public MesaAssertionError(String msg) {
		super(msg);
	}
}
