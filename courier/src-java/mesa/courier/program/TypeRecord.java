package mesa.courier.program;

import java.util.ArrayList;
import java.util.List;

public class TypeRecord extends Type {
	public final List<Field>fields = new ArrayList<>();
	public TypeRecord() {
		super(Kind.RECORD);
	}
	public void addField(Field field) {
		fields.add(field);
	}
	public String toString() {
		if (fields.isEmpty()) return String.format("%s []", kind);
		
		StringBuilder t = new StringBuilder();
		for(Field e: fields) {
			t.append(String.format(", %s", e.toString()));
		}
		return String.format("%s [%s]", kind, t.substring(2));
	}
}
