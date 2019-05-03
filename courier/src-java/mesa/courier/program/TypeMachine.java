package mesa.courier.program;

import java.util.ArrayList;
import java.util.List;

public class TypeMachine extends Type {
	public final Type type;
	public final List<MDField>mdFields;
	public TypeMachine(Type type) {
		super(Kind.MACHINE);
		this.type     = type;
		this.mdFields = new ArrayList<>();
	}
	public void addField(MDField mdField) {
		mdFields.add(mdField);
	}
	@Override
	public String toString() {
		if (mdFields.isEmpty()) return String.format("%s []", kind);
		
		StringBuilder t = new StringBuilder();
		for(MDField e: mdFields) {
			t.append(String.format(", %s", e.toString()));
		}
		return String.format("%s %s [%s]", kind, type, t.substring(2));
	}
}
