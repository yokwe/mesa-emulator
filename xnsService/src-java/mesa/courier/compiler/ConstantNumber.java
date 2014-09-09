package mesa.courier.compiler;

public class ConstantNumber extends Constant {
	public final long value;
	protected ConstantNumber(long value) {
		super(Kind.NUMBER);
		this.value = value;
	}
	@Override
	public String toString() {
		return String.format("%d", value);
	}
}
