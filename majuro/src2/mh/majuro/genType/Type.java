package mh.majuro.genType;

public class Type {
	public String name;
	public String type;
	public String minValue;
	public String maxValue;
	
	public Type() {
	}
	
	public boolean isEmpty() {
		return name.isEmpty();
	}
}