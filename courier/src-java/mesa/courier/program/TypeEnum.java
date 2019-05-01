package mesa.courier.program;

import java.util.ArrayList;
import java.util.List;

public class TypeEnum extends Type {
	public final Type                 type;
	public final List<Correspondence> elements;

	public TypeEnum(Type type) {
		super(Kind.ENUM);
		this.type = type;
		elements = new ArrayList<>();
	}

	public void addCorrespondence(Correspondence element) {
		elements.add(element);
	}
	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder();
		int count = 0;
		for(Correspondence e: elements) {
			if (count++ != 0) buf.append(", ");
			buf.append(e.toString());
		}
		return String.format("%s [%s [%s]]", kind, type.toString(), buf.toString());
	}
}
