package mh.majuro.genType;

import java.util.List;

public class RecordInfo {
	public final String      name;
	public final int         size;
	
	public final List<FieldInfo> fieldList;
	
	public RecordInfo(String name, int size, List<FieldInfo> fieldList) {
		this.name      = name;
		this.size      = size;
		this.fieldList = fieldList;
	}
	
	@Override
	public String toString() {
		return String.format("{%-20s %4d %2d}", name, size, fieldList.size());
	}
}