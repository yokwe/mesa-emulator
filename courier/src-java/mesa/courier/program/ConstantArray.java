package mesa.courier.program;

import java.util.ArrayList;
import java.util.List;

public class ConstantArray extends Constant {
	public final List<Constant> values = new ArrayList<>();
	
	protected ConstantArray() {
		super(Kind.ARRAY);
	}

	public void addConstant(Constant constant) {
		values.add(constant);
	}
	
	@Override
	public String toString() {
		StringBuilder t = new StringBuilder();
		for(Constant e: values) {
			t.append(String.format("  %s", e.toString()));
		}
		return String.format("[%s (%d)[%s]", kind, values.size(), (t.length() != 0) ? t.substring(2) : "");
	}
}
