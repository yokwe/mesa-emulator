package mesa.courier.program;

public class ConstantSizeOf extends Constant {
	public final ReferencedConstant value;
	
	public ConstantSizeOf(String program, String name) {
		super(Kind.REFERENCE);
		value = new ReferencedConstant(program, name);
	}
	
	public ConstantSizeOf(String name) {
		super(Kind.REFERENCE);
		value = new ReferencedConstant(name);
	}
	
	public String toString() {
		return String.format("[Const SIZEOF %s]", value.toString());
	}
}
