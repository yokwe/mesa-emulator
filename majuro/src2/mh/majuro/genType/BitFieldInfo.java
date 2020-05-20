package mh.majuro.genType;

public class BitFieldInfo extends FieldInfo {
	final int startBit;
	final int stopBit;
	
	public BitFieldInfo(String name, String type, int offset, int size, int startBit, int stopBit) {
		super(FieldType.BIT, name, type, offset, size);
		
		this.startBit = startBit;
		this.stopBit  = stopBit;
	}
}