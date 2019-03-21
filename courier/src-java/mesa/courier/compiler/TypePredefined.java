package mesa.courier.compiler;

public class TypePredefined extends Type {
	public TypePredefined(Kind kind) {
		super(kind);
		
		// sanity check
		switch(kind) {
		case BOOLEAN:
		case BYTE:
		case CARDINAL:
		case LONG_CARDINAL:
//		case INTEGER:
//		case LONG_INTEGER:
		case STRING:
		case UNSPECIFIED:
		case UNSPECIFIED2:
		case UNSPECIFIED3:
			break;
		default:
			throw new CompilerException(String.format("Unknown type = %s", kind.toString()));
		}
	}
	
	public String toString() {
		return kind.toString();
	}
}
