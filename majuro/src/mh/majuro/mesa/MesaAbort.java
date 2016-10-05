package mh.majuro.mesa;

@SuppressWarnings("serial")
public class MesaAbort extends MesaRuntimeException {
	public MesaAbort(String msg) {
		super(msg);
	}

	public MesaAbort() {
		super();
	}
}
