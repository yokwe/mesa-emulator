package mh.majuro.genType;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import org.slf4j.LoggerFactory;

import mh.majuro.UnexpectedException;

public class Context {
	static final org.slf4j.Logger logger = LoggerFactory.getLogger(Context.class);

	final Map<String, RecordInfo> recordMap;
	final Map<String, TypeInfo>   typeMap;
	
	Context(Map<String, RecordInfo> recordMap, Map<String, TypeInfo> typeMap) {
		this.recordMap = recordMap;
		this.typeMap   = typeMap;
	}
	
	boolean isPrimitive(String name) {
		switch(name) {
		case "boolean":
		case "CARD8":
		case "CARD16":
		case "CARD32":
			return true;
		default:
			return false;
		}
	}
	boolean isRecord(String name) {
		return recordMap.containsKey(name);
	}
	boolean isType(String name) {
		return typeMap.containsKey(name);
	}
	RecordInfo getRecordInfo(String name) {
		if (recordMap.containsKey(name)) {
			return recordMap.get(name);
		} else {
			logger.error("name {}", name);
			throw new UnexpectedException();
		}
	}
	int getSize(String name) {
		String type = name;
		for(;;) {
			switch(type) {
			case "boolean":
			case "CARD8":
			case "CARD16":
				return 1;
			case "CARD32":
				return 2;
			default:
				if (typeMap.containsKey(type)) {
					type = typeMap.get(type).type;
					continue;
				}
				if (recordMap.containsKey(name)) {
					return recordMap.get(name).size;
				} else {
					logger.error("name {}", name);
					throw new UnexpectedException();
				}
			}
		}
	}
	String getBaseType(String name) {
		String type = name;
		for(;;) {
			switch(type) {
			case "boolean":
			case "CARD8":
			case "CARD16":
			case "CARD32":
				return type;
			default:
				if (typeMap.containsKey(type)) {
					type = typeMap.get(type).type;
					continue;
				}
				if (recordMap.containsKey(type)) {
					return type;
				} else {
					logger.error("name {}", name);
					logger.error("type {}", type);
					logger.error("recordMap {}", recordMap.keySet());
					throw new UnexpectedException();
				}
			}
		}
	}

	void sanityCheck() {
		boolean hasProblem = false;
		for(TypeInfo e: typeMap.values()) {
			{
				String type = getBaseType(e.type);
				if (isPrimitive(type)) {
					//
				} else {
					logger.error("TYPE type is not primitive");
					logger.error("  type {}", e);
					hasProblem = true;
				}
			}
			{
				int minValue = e.minValue;
				int maxValue = e.maxValue;
				if (minValue < 0) {
					logger.error("TYPE minValue is negative");
					logger.error("  type {}", e);
					hasProblem = true;
				}
				if (maxValue < minValue) {
					logger.error("TYPE minValue is greather than maxValue");
					logger.error("  type {}", e);
					hasProblem = true;
				}
			}
		}
		for(RecordInfo e: recordMap.values()) {
			if (e.size <= 0) {
				logger.error("RECORD size is less than or equals to zero");
				logger.error("  record {}", e);
				hasProblem = true;
			}
			if (e.fieldList.isEmpty()) {
				logger.error("RECORD fieldList is empty");
				logger.error("  record {}", e);
				hasProblem = true;
			}
			
			List<BitFieldInfo> bitFieldInfoList = new ArrayList<>();
			for(FieldInfo ee: e.fieldList) {
				if (ee.type.isEmpty()) {
					//
				} else {
					String type = getBaseType(ee.type);
					if (isPrimitive(type)) {
						// OK
					} else if (isRecord(type)) {
						// OK
					} else if (isType(type)) {
						// OK
					} else {
						logger.error("RECORD unknown type");
						logger.error("  record {}", e);
						logger.error("  field  {}", ee);
						hasProblem = true;
					}
				}
				if (ee.fieldType == FieldType.ARRAY) {
					ArrayFieldInfo arrayFieldInfo = (ArrayFieldInfo)ee;
					ArrayInfo arrayInfo = arrayFieldInfo.arrayInfo;
					if (arrayFieldInfo.isEmpty()) {
						//
					} else {
						{
							String type = getBaseType(arrayInfo.indexType);
							if (isPrimitive(type)) {
								// OK
							} else if (isRecord(type)) {
								// OK
							} else if (isType(type)) {
								// OK
							} else {
								logger.error("RECORD unknown array indexType");
								logger.error("  record {}", e);
								logger.error("  field  {}", ee);
								hasProblem = true;
							}
						}
						{
							String type = getBaseType(arrayInfo.type);
							if (isPrimitive(type)) {
								// OK
							} else if (isRecord(type)) {
								// OK
							} else if (isType(type)) {
								// OK
							} else {
								logger.error("RECORD unknown array type");
								logger.error("  record {}", e);
								logger.error("  field  {}", ee);
								hasProblem = true;
							}
						}
						{
							if (arrayInfo.size <= 0) {
								logger.error("RECORD array size is less than or equals to zero");
								logger.error("  record {}", e);
								logger.error("  field  {}", ee);
								hasProblem = true;
							}
						}
						{
							if (arrayInfo.length <= 0) {
								logger.error("RECORD array length is less than or equals to zero");
								logger.error("  record {}", e);
								logger.error("  field  {}", ee);
								hasProblem = true;
							}
						}
					}
				}
				if (ee.fieldType == FieldType.BIT) {
					BitFieldInfo bitFieldInfo = (BitFieldInfo)ee;
					BitInfo bitInfo = bitFieldInfo.bitInfo;
					// save for later sanity check
					bitFieldInfoList.add(bitFieldInfo);

					switch(bitFieldInfo.size) {
					case 1:
						if (0 <= bitInfo.startBit && bitInfo.startBit <= 15) {
							//
						} else {
							logger.error("RECORD bit startBit is out of bound");
							logger.error("  record {}", e);
							logger.error("  field  {}", ee);
							hasProblem = true;
						}
						if (0 <= bitInfo.stopBit && bitInfo.stopBit <= 15) {
							//
						} else {
							logger.error("RECORD bit stopBit is out of bound");
							logger.error("  record {}", e);
							logger.error("  field  {}", ee);
							hasProblem = true;
						}
						if (bitInfo.startBit <= bitInfo.stopBit) {
							//
						} else {
							logger.error("RECORD bit startBit is greather than stopBit");
							logger.error("  record {}", e);
							logger.error("  field  {}", ee);
							hasProblem = true;
						}
						break;
					case 2:
						if (0 <= bitInfo.startBit && bitInfo.startBit <= 31) {
							//
						} else {
							logger.error("RECORD bit startBit is out of bound");
							logger.error("  record {}", e);
							logger.error("  field  {}", ee);
							hasProblem = true;
						}
						if (0 <= bitInfo.stopBit && bitInfo.stopBit <= 31) {
							//
						} else {
							logger.error("RECORD bit stopBit is out of bound");
							logger.error("  record {}", e);
							logger.error("  field  {}", ee);
							hasProblem = true;
						}
						if (bitInfo.startBit <= bitInfo.stopBit) {
							//
						} else {
							logger.error("RECORD bit startBit is greather than stopBit");
							logger.error("  record {}", e);
							logger.error("  field  {}", ee);
							hasProblem = true;
						}
						break;
					default:
						throw new UnexpectedException();
					}
				}
			}
			
			if (!bitFieldInfoList.isEmpty()) {
				// check bit field
				Map<Integer, List<BitFieldInfo>> map = new TreeMap<>();
				for(BitFieldInfo ee: bitFieldInfoList) {
					List<BitFieldInfo> list;
					if (map.containsKey(ee.offset)) {
						list = map.get(ee.offset);
					} else {
						list = new ArrayList<>();
						map.put(ee.offset, list);
					}
					list.add(ee);
				}
				
				for(List<BitFieldInfo> ee: map.values()) {
					int size = ee.get(0).size;
					// check size
					{
						boolean hasInconsistent = false;
						for(BitFieldInfo eee: ee) {
							if (eee.size != size) hasInconsistent = true;
						}
						if (hasInconsistent) {
							logger.error("RECORD bit size is incosistent");
							logger.error("  size   {}", size);
							logger.error("  record {}", e);
							for(BitFieldInfo eee: ee) {
								logger.error("  field {}", eee);
							}
							hasProblem = true;
						}
					}
					// check bit pattern hole
					{
						switch(size) {
						case 1:
						case 2:
							break;
						default:
							throw new UnexpectedException();
						}
						boolean bitArray[] = new boolean[size * 16];
						for(int i = 0; i < bitArray.length; i++) bitArray[i] = false;
						
						for(BitFieldInfo eee: ee) {
							for(int i = eee.bitInfo.startBit; i <= eee.bitInfo.stopBit; i++) {
								bitArray[i] = true;
							}
						}
						boolean hasHole = false;
						for(int i = 0; i < bitArray.length; i++) {
							if (!bitArray[i]) hasHole = true;
						}
						if (hasHole) {
							StringBuilder pattern = new StringBuilder();
							for(int i = 0; i < bitArray.length; i++) {
								pattern.append(bitArray[i] ? "1" : "0");
							}
							logger.error("RECORD bit has hole");
							logger.error("  pattern {}", pattern);
							logger.error("  record  {}", e);
							for(BitFieldInfo eee: ee) {
								logger.error("  field {}", eee);
							}
							hasProblem = true;
						}
					}
				}
			}
		}
		
		if (hasProblem) throw new UnexpectedException("hasProblem");
	}
}