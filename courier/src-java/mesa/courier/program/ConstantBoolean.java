package mesa.courier.program;

public class ConstantBoolean extends Constant {
	public final boolean value;
	protected ConstantBoolean(boolean value) {
		super(Kind.BOOLEAN);
		this.value = value;
	}
	@Override
	public String toString() {
		return String.format("%s", value ? "true" : "false");
	}
}
