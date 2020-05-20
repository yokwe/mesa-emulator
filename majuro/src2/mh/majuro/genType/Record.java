package mh.majuro.genType;

public class Record {
	public String name;
	public String field;
	public String type;
	public String offset;
	public String size;
	public String startBit;
	public String stopBit;
	public String indexType;
	public String elementType;
	public String elementSize;
	public String elementLength;
	
	public Record() {
	}
	
	public boolean isEmpty() {
		return name.isEmpty();
	}
}