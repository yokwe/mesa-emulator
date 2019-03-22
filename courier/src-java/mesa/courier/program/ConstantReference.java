package mesa.courier.program;

public class ConstantReference extends Constant {
	public final ReferencedConstant value;
	
	public ConstantReference(String program, String name) {
		super(Kind.REFERENCE);
		value = new ReferencedConstant(program, name);
	}
	
	public ConstantReference(String name) {
		super(Kind.REFERENCE);
		value = new ReferencedConstant(name);
	}
	
	public String toString() {
		return String.format("[Const REF %s]", value.toString());
	}
}
