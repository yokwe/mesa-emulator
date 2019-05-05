package mesa.courier.program;

public class TypePredefined extends Type {
	public TypePredefined(Kind kind) {
		super(kind);
		
		// sanity check
		switch(kind) {
		case BLOCK:
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
			throw new ProgramException(String.format("Unknown type = %s", kind.toString()));
		}
	}
	
	@Override
	public String toString() {
		return kind.toString();
	}
}
