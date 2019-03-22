package mesa.courier.program;

import java.util.ArrayList;
import java.util.List;

public class TypeEnum extends Type {
	public List<Correspondence> elements;

	public TypeEnum() {
		super(Kind.ENUM);
		elements = new ArrayList<>();
	}

	public void addCorrespondence(Correspondence element) {
		elements.add(element);
	}
	
	public String toString() {
		StringBuilder buf = new StringBuilder();
		int count = 0;
		for(Correspondence e: elements) {
			if (count++ != 0) buf.append(", ");
			buf.append(e.toString());
		}
		return String.format("%s [%s]", kind, buf.toString());
	}
}
