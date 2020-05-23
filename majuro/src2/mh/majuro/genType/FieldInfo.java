package mh.majuro.genType;

import mh.majuro.UnexpectedException;

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
		this(FieldType.SIMPLE, name, type, offset, size);
	}
	
	public boolean isEmpty() {
		if (fieldType == FieldType.ARRAY) {
			ArrayFieldInfo arrayFieldInfo = (ArrayFieldInfo)this;
			return arrayFieldInfo.isEmpty();
		} else {
			return type.isEmpty();
		}
	}
	
	@Override
	public String toString() {
		switch(fieldType) {
		case SIMPLE:
			return toStringInternal();
		case ARRAY:
			return ((ArrayFieldInfo)this).toString();
		case BIT:
			return ((BitFieldInfo)this).toString();
		default:
			throw new UnexpectedException();
		}
	}
	private String toStringInternal() {
		return String.format("{%s %s %s %d %d}", fieldType, name, type, offset, size);
	}
	protected String toStringInternal(String extra) {
		return String.format("{%s %s %s %d %d %s}", fieldType, name, type, offset, size, extra);
	}
}