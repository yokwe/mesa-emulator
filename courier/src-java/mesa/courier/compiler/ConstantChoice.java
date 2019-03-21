package mesa.courier.compiler;

public class ConstantChoice extends Constant {
	public final String   id;
	public final Constant constant;
	
	protected ConstantChoice(String id, Constant constant) {
		super(Kind.CHOICE);
		this.id       = id;
		this.constant = constant;
	}
	@Override
	public String toString() {
		return String.format("%s %s", id, constant.toString());
	}
}
