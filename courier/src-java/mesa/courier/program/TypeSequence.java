package mesa.courier.program;

public class TypeSequence extends Type {
	public static final long MAX_SIZE = 65535;
	public final long size;
	public final Type type;
	
	public TypeSequence(long size, Type type) {
		super(Kind.SEQUENCE);
		this.size = size;
		this.type = type;
	}
	
	public String toString() {
		return String.format("[%s (%d)%s]]", kind, size, type.toString());
	}
}
