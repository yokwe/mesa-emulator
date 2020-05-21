package mh.majuro.genType;

public class ArrayInfo {
	final String indexType;
	final String elementType;
	final int    elementSize;
	final int    elementLength;
	
	public ArrayInfo(String indexType, String elementType, int elementSize, int elementLength) {
		this.indexType     = indexType;
		this.elementType   = elementType;
		this.elementSize   = elementSize;
		this.elementLength = elementLength;
	}
	
	public boolean isEmpty() {
		return indexType.isEmpty() && elementType.isEmpty();
	}
}