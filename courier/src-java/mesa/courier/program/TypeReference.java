package mesa.courier.program;

import java.util.ArrayList;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class TypeReference extends Type {
	protected static final Logger logger = LoggerFactory.getLogger(TypeReference.class);

	public static List<TypeReference> all = new ArrayList<>();
	
	public final ReferencedType value;
	public TypeReference(String program, String name) {
		super(Kind.REFERENCE);
		value = new ReferencedType(program, name);
		//
		all.add(this);
	}
	
	@Override
	public String toString() {
		return String.format("[REF %s]", value.toString());
	}
	
	public TypeReference getLastReference() {
		TypeReference ret = this;
		for(;;) {
			Type derefType = Program.dereference(ret);
			if (!derefType.isReference()) break;
			ret = (TypeReference)derefType;
		}
		return ret;
	}
}
