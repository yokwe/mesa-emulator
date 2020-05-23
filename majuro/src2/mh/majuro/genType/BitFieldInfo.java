package mh.majuro.genType;

public class BitFieldInfo extends FieldInfo {
	final BitInfo bitInfo;
	
	public BitFieldInfo(String name, String type, int offset, int size, int startBit, int stopBit) {
		super(FieldType.BIT, name, type, offset, size);
		
		bitInfo = new BitInfo(size, startBit, stopBit);
	}
	
	@Override
	public String toString() {
		return toStringInternal(bitInfo.toString());
	}
}