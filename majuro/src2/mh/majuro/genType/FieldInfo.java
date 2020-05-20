package mh.majuro.genType;

public class FieldInfo {
	final FieldType fieldType;
	final String    name;
	final String    type;
	final int       offset;
	final int       size;
	
	protected FieldInfo(FieldType fieldType, String name, String type, int offset, int size) {
		this.fieldType = fieldType;
		this.name      = name;
		this.type      = type;
		this.offset    = offset;
		this.size      = size;
	}
	public FieldInfo(String name, String type, int offset, int size) {
		this(FieldType.NORMAL, name, type, offset, size);
	}
	
	public boolean isEmpty() {
		return type.isEmpty();
	}
	
	@Override
	public String toString() {
		return String.format("{%-8s %-20s %-20s %4d %2d}", fieldType, name, type, offset, size);
	}
}