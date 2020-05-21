package mh.majuro.genType;

public class ArrayFieldInfo extends FieldInfo {
	public final ArrayInfo arrayInfo;
	
	public ArrayFieldInfo(String name, String type, int offset, int size, String indexType, String elementType, int elementSize, int elementLength) {
		super(FieldType.ARRAY, name, type, offset, size);
		
		arrayInfo = new ArrayInfo(indexType, elementType, elementSize, elementLength);
	}
	
	public boolean isEmpty() {
		return arrayInfo.isEmpty() && arrayInfo.isEmpty();
	}
}