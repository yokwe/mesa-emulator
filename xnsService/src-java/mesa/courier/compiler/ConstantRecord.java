package mesa.courier.compiler;

import java.util.ArrayList;
import java.util.List;

public class ConstantRecord extends Constant {
	public final List<Component> values = new ArrayList<>();
	
	protected ConstantRecord() {
		super(Kind.RECORD);
	}

	public void addComponent(Component value) {
		values.add(value);
	}
	
	@Override
	public String toString() {
		StringBuilder t = new StringBuilder();
		for(Component e: values) {
			t.append(String.format("  %s", e.toString()));
		}
		return String.format("[%s (%d)[%s]", kind, values.size(), (t.length() != 0) ? t.substring(2) : "");
	}
}
