package mesa.courier.program;

public class TypeBlock extends Type {
	public final long size;
	
	public TypeBlock(long size) {
		super(Kind.BLOCK);
		this.size = size;
	}
	
	@Override
	public String toString() {
		return String.format("[%s (%d)]]", kind, size);
	}
}
