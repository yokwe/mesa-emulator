package mh.majuro.genType;

public class ArrayFieldInfo extends FieldInfo {
	final String indexType;
	final String elementType;
	final int    elementSize;
	final int    elementLength;
	
	public ArrayFieldInfo(String name, String type, int offset, int size, String indexType, String elementType, int elementSize, int elementLength) {
		super(FieldType.ARRAY, name, type, offset, size);
		
		this.indexType     = indexType;
		this.elementType   = elementType;
		this.elementSize   = elementSize;
		this.elementLength = elementLength;
	}
	
	public boolean isEmpty() {
		return indexType.isEmpty() && elementType.isEmpty();
	}
}