package mh.majuro.genType;

public class TypeInfo {
	public final String name;
	public final String type;
	public final int    minValue;
	public final int    maxValue;
	
	TypeInfo(String name, String type, int minValue, int maxValue) {
		this.name     = name;
		this.type     = type;
		this.minValue = minValue;
		this.maxValue = maxValue;
	}
	
	@Override
	public String toString() {
		return String.format("{%-16s %-8s %5d %5d}", name, type, minValue, maxValue);
	}
}