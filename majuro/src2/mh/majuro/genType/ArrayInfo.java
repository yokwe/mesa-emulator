package mh.majuro.genType;

public class ArrayInfo {
	final String indexType;
	final String type;
	final int    size;
	final int    length;
	
	public ArrayInfo(String indexType, String elementType, int elementSize, int elementLength) {
		this.indexType = indexType;
		this.type      = elementType;
		this.size      = elementSize;
		this.length    = elementLength;
	}
	
	@Override
	public String toString() {
		return String.format("{%s %s %d %d}", indexType, type, size, length);
	}
}