package mh.majuro.genType;

import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.TreeSet;

import org.slf4j.LoggerFactory;

import mh.majuro.UnexpectedException;
import mh.majuro.util.AutoIndentPrintWriter;
import mh.majuro.util.CSVUtil;

public class GenerateType {
	static final org.slf4j.Logger logger = LoggerFactory.getLogger(GenerateType.class);

	static Map<String, RecordInfo> buildRecordInfoMap(List<Record> recordList) {
		// Sanity check
		{
			for(Record record: recordList) {
				if (record.isEmpty()) continue;
				
				if (record.name.isEmpty()) {
					logger.error("name is empty");
					throw new UnexpectedException();
				}
				if (record.field.isEmpty()) {
					logger.error("field is empty");
					throw new UnexpectedException();
				}
				if (record.offset.isEmpty()) {
					logger.error("offset is empty");
					throw new UnexpectedException();
				}
				if (record.size.isEmpty()) {
					logger.error("size is empty");
					throw new UnexpectedException();
				}
				if (record.startBit.isEmpty()) {
					if (!record.stopBit.isEmpty()) {
						logger.error("startBit is empty but stopBit is not emtpty");
						throw new UnexpectedException();
					}
				}
				if (record.stopBit.isEmpty()) {
					if (!record.startBit.isEmpty()) {
						logger.error("stopBit is empty but startBit is not emtpty");
						throw new UnexpectedException();
					}
				}
				if (record.elementSize.isEmpty()) {
					if (!record.elementLength.isEmpty()) {
						logger.error("elementSize is empty but elementLength is not emtpty");
						throw new UnexpectedException();
					}
				}
				if (record.elementLength.isEmpty()) {
					if (!record.elementSize.isEmpty()) {
						logger.error("elementLength is empty but elementSize is not emtpty");
						throw new UnexpectedException();
					}
				}
			}
		}
			
		Map<String, RecordInfo> ret = new TreeMap<>();
		
		String name = null;
		List<Record> list = new ArrayList<>();
		
		for(Record record: recordList) {
			if (record.isEmpty()) {
				if (name != null) {
					Record last = list.get(list.size() - 1);
					int size = Integer.parseInt(last.offset) + Integer.parseInt(last.size);
					
					List<FieldInfo> fields = new ArrayList<>();
					
					for(Record e: list) {
						if (!e.startBit.isEmpty() && e.elementType.isEmpty()) {
							// BIT
							fields.add(new BitFieldInfo(
								e.field, e.type, Integer.parseInt(e.offset), Integer.parseInt(e.size),
								Integer.parseInt(e.startBit), Integer.parseInt(e.stopBit)));
						} else if (e.startBit.isEmpty() && !e.elementType.isEmpty()) {
							// ARRAY
							fields.add(new ArrayFieldInfo(
								e.field, e.type, Integer.parseInt(e.offset), Integer.parseInt(e.size),
								e.indexType, e.elementType, Integer.parseInt(e.elementSize), Integer.parseInt(e.elementLength)));
						} else if (e.startBit.isEmpty() && e.elementType.isEmpty()) {
							// NORMAL
							fields.add(new FieldInfo(
								e.field, e.type, Integer.parseInt(e.offset), Integer.parseInt(e.size)));
						} else {
							logger.error("Unexpected field type");
							throw new UnexpectedException("Unexpected field type");
						}
					}
					
					ret.put(name, new RecordInfo(name, size, fields));
				}
				//
				list = new ArrayList<>();
				name = null;
			} else {
				if (name == null) {
					name = record.name;
				}
				list.add(record);
			}
		}
		if (name != null) {
			Record last = list.get(list.size() - 1);
			int size = Integer.parseInt(last.offset) + Integer.parseInt(last.size);
			
			List<FieldInfo> fields = new ArrayList<>();
			
			for(Record e: list) {
				if (!e.startBit.isEmpty() && e.elementType.isEmpty()) {
					// BIT
					fields.add(new BitFieldInfo(
						e.field, e.type, Integer.parseInt(e.offset), Integer.parseInt(e.size),
						Integer.parseInt(e.startBit), Integer.parseInt(e.stopBit)));
				} else if (e.startBit.isEmpty() && !e.elementType.isEmpty()) {
					// ARRAY
					fields.add(new ArrayFieldInfo(
						e.field, e.type, Integer.parseInt(e.offset), Integer.parseInt(e.size),
						e.indexType, e.elementType, Integer.parseInt(e.elementSize), Integer.parseInt(e.elementLength)));
				} else if (e.startBit.isEmpty() && e.elementType.isEmpty()) {
					// NORMAL
					fields.add(new FieldInfo(
						e.field, e.type, Integer.parseInt(e.offset), Integer.parseInt(e.size)));
				} else {
					logger.error("Unexpected field type");
					throw new UnexpectedException("Unexpected field type");
				}
			}
			
			ret.put(name, new RecordInfo(name, size, fields));
		}
		return ret;
	}
	
	static Map<String, TypeInfo> buildTypeInfoMap(List<Type> typeList) {
		Map<String, TypeInfo> ret = new TreeMap<>();

		for(Type type: typeList) {
			if (type.isEmpty() && type.type.isEmpty() && type.minValue.isEmpty() && type.maxValue.isEmpty()) continue;
			
			// Sanity check
			{
				if (type.name.isEmpty()) {
					logger.error("name is empty");
					throw new UnexpectedException();
				}
				if (type.type.isEmpty()) {
					logger.error("type is empty");
					throw new UnexpectedException();
				}
				if (type.minValue.isEmpty()) {
					logger.error("minValue is empty");
					throw new UnexpectedException();
				}
				if (type.maxValue.isEmpty()) {
					logger.error("maxValue is empty");
					throw new UnexpectedException();
				}
			}
			
			ret.put(type.name, new TypeInfo(type.name, type.type, Integer.parseInt(type.minValue), Integer.parseInt(type.maxValue)));
		}
		
		return ret;
	}

	static final String PATH_DIR = "src2/mh/majuro/mesa/type";
	
	private static String getType(Map<String, RecordInfo> recordInfoMap, Map<String, TypeInfo> typeInfoMap, String type) {
		switch(type) {
		case "boolean":
		case "CARD8":
		case "CARD16":
		case "CARD32":
			break;
		default:
			if (typeInfoMap.containsKey(type)) {
				type = typeInfoMap.get(type).type;
				switch(type) {
				case "boolean":
				case "CARD8":
				case "CARD16":
				case "CARD32":
					break;
				default:
					logger.error("type {}", type);
					throw new UnexpectedException();
				}
			} else if (recordInfoMap.containsKey(type)) {
				RecordInfo refRecordInfo = recordInfoMap.get(type);
				if (refRecordInfo.size == 1) {
					type = "CARD16";
				} else {
					logger.error("size {}", refRecordInfo.size);
					throw new UnexpectedException();
				}
			} else {
				logger.error("type {}", type);
				throw new UnexpectedException();
			}
		}
		return type;
	}
	
	static void generateRecordClass(Map<String, RecordInfo> recordInfoMap, Map<String, TypeInfo> typeInfoMap, RecordInfo recordInfo) {
		String path = String.format("%s/%s.java", PATH_DIR, recordInfo.name);
		logger.info("path {}",path);
		try (AutoIndentPrintWriter out = new AutoIndentPrintWriter(new PrintWriter(path))) {
			out.println("package mh.majuro.mesa.type;");
			out.println();
			out.println("import mh.majuro.mesa.Type.*;");
			out.println();
			out.println("public final class %s {", recordInfo.name);
			out.println("public static final int SIZE = %d;", recordInfo.size);
			out.println();
			
			for(FieldInfo fieldInfo: recordInfo.fieldList) {
				out.println("// offset %4d  size %4d  type %-8s  name %s", fieldInfo.offset, fieldInfo.size, fieldInfo.type, fieldInfo.name);
				if (fieldInfo.fieldType == FieldType.ARRAY) {
					ArrayFieldInfo arrayFieldInfo = (ArrayFieldInfo)fieldInfo;
					ArrayInfo arrayInfo = arrayFieldInfo.arrayInfo;;
					out.println("//   array  index %-16s  size %4d  length %3d  element %s", arrayInfo.indexType, arrayInfo.size, arrayInfo.length, arrayInfo.type);
				}
				if (fieldInfo.fieldType == FieldType.BIT) {
					BitFieldInfo bitFieldInfo = (BitFieldInfo)fieldInfo;
					BitInfo bitInfo = bitFieldInfo.bitInfo;
					out.println("//   bit startBit %2d  stopBit %2d", bitInfo.startBit, bitInfo.stopBit);
				}
				if (fieldInfo.isEmpty()) continue;

				// 	public static int WORD_OFFSET       = 0;
				out.println("public static final class %s {", fieldInfo.name);
				out.println("public static final         int SIZE   = %2d;", fieldInfo.size);
				out.println("public static final         int OFFSET = %2d;", fieldInfo.offset);

				switch (fieldInfo.fieldType) {
				case NORMAL:
				{
					out.println();
					out.println("public static @LONG_POINTER int offset(@LONG_POINTER int base) {");
					out.println("return base + OFFSET;");
					out.println("}");
				}
					break;
				case BIT:
				{
					BitFieldInfo bitFieldInfo = (BitFieldInfo)fieldInfo;
					switch(bitFieldInfo.size) {
					case 1:
						out.println("public static final @CARD16 int MASK    = %s;", bitFieldInfo.bitInfo.mask);
						out.println("public static final         int SHIFT   = %d;", bitFieldInfo.bitInfo.shift);
						out.println();
						
						out.println("public static @LONG_POINTER int offset(@LONG_POINTER int base) {");
						out.println("return base + OFFSET;");
						out.println("}");
						out.println("public static @CARD16 int getBit(@CARD16 int value) {");
						out.println("return (value & MASK) >>> SHIFT;");
						out.println("}");
						out.println("public static @CARD16 int setBit(@CARD16 int value, @CARD16 int newValue) {");
						out.println("return ((newValue << SHIFT) & MASK) | (value & ~MASK);");
						out.println("}");
						break;
					case 2:
						out.println("public static final @CARD32 int MASK    = %s;", bitFieldInfo.bitInfo.mask);
						out.println("public static final         int SHIFT   = %d;", bitFieldInfo.bitInfo.shift);
						out.println();
						
						out.println("public static @LONG_POINTER int offset(@LONG_POINTER int base) {");
						out.println("return base + OFFSET;");
						out.println("}");
						out.println("public static @CARD32 int getBit(@CARD32 int value) {");
						out.println("return (value & MASK) >>> SHIFT;");
						out.println("}");
						out.println("public static @CARD32 int setBit(@CARD32 int value, @CARD32 int newValue) {");
						out.println("return ((newValue << SHIFT) & MASK) | (value & ~MASK);");
						out.println("}");
						break;
					default:
						logger.error("size {}", bitFieldInfo.size);
						throw new UnexpectedException();
					}
				}
					break;
				case ARRAY:
					out.println("// FIXME ARRAY %s", fieldInfo);
					break;
				default:
					throw new UnexpectedException();
				}
				
				// get set methods
				{
					String qClassName = String.format("%s.%s", recordInfo.name, fieldInfo.name);
					switch(fieldInfo.fieldType) {
					case NORMAL:
					{
						String type = getType(recordInfoMap, typeInfoMap, fieldInfo.type);						switch(type) {
						case "boolean":
							out.println("public static boolean get(@LONG_POINTER int base) {");
							out.println("return RecordBase.get(%s::offset, base) != 0;", qClassName);
							out.println("}");
							out.println("public static void set(@LONG_POINTER int base, boolean newValue) {");
							out.println("RecordBase.set(%s::offset, base, (newValue ? 1 : 0));", qClassName);
							out.println("}");
							break;
						case "CARD8":
						case "CARD16":
							out.println("public static @%s int get(@LONG_POINTER int base) {", type);
							out.println("return RecordBase.get(%s::offset, base);", qClassName);
							out.println("}");
							out.println("public static void set(@LONG_POINTER int base, @%s int newValue) {", type);
							out.println("RecordBase.set(%s::offset, base, newValue);", qClassName);
							out.println("}");
							break;
						case "CARD32":
							out.println("public static @%s int get(@LONG_POINTER int base) {", type);
							out.println("return RecordBase.getDbl(%s::offset, base);", qClassName);
							out.println("}");
							out.println("public static void set(@LONG_POINTER int base, @%s int newValue) {", type);
							out.println("RecordBase.setDbl(%s::offset, base, newValue);", qClassName);
							out.println("}");
							break;
						default:
							logger.error("type {}", type);
							throw new UnexpectedException();
						}
					}
						break;
					case BIT:
					{
						String type = getType(recordInfoMap, typeInfoMap, fieldInfo.type);
						switch(type) {
						case "boolean":
							// public static final int getBitField(ToIntIntFunction addressFunc, ToIntIntFunction getValueFunc, @LONG_POINTER int base)
							out.println("public static boolean get(@LONG_POINTER int base) {");
							out.println("return RecordBase.getBitField(%1$s::offset, %1$s::getBit, base) != 0;", qClassName);
							out.println("}");
							out.println("public static void set(@LONG_POINTER int base, boolean newValue) {");
							out.println("RecordBase.setBitField(%1$s::offset, %1$s::setBit, base, (newValue ? 1 : 0));", qClassName);
							out.println("}");
							break;
						case "CARD8":
						case "CARD16":
							out.println("public static @%s int get(@LONG_POINTER int base) {", type);
							out.println("return RecordBase.getBitField(%1$s::offset, %1$s::getBit, base);", qClassName);
							out.println("}");
							out.println("public static void set(@LONG_POINTER int base, @%s int newValue) {", type);
							out.println("RecordBase.setBitField(%1$s::offset, %1$s::setBit, base, newValue);", qClassName);
							out.println("}");
							break;
						case "CARD32":
							out.println("public static @%s int get(@LONG_POINTER int base) {", type);
							out.println("return RecordBase.getBitFieldDbl(%1$s::offset, %1$s::getBit, base);", qClassName);
							out.println("}");
							out.println("public static void set(@LONG_POINTER int base, @%s int newValue) {", type);
							out.println("RecordBase.setBitFieldDbl(%1$s::offset, %1$s::setBit, base, newValue);", qClassName);
							out.println("}");
							break;
						default:
							logger.error("type {}", type);
							throw new UnexpectedException();
						}
					}
						break;
					case ARRAY:
						// FIXME
						out.println("// FIXME ARRAY %s", fieldInfo);
						break;
					default:
						throw new UnexpectedException();
					}
				}
				out.println("}");
			}
			out.println("}");
		} catch (FileNotFoundException e) {
			String exceptionName = e.getClass().getSimpleName();
			logger.error("{} {}", exceptionName, e);
			throw new UnexpectedException(exceptionName, e);
		}
	}

	public static void main(String[] args) {
		logger.info("START");
		
		List<Record> recordList = CSVUtil.read(Record.class).file("tmp/Record.csv");
		List<Type>   typeList   = CSVUtil.read(Type.class).file("tmp/Type.csv");
		
		logger.info("typeList   {}", typeList.size());
		logger.info("recordList {}", recordList.size());
		
		Map<String, TypeInfo> typeInfoMap = buildTypeInfoMap(typeList);
		logger.info("typeInfoMap  {} {}", typeInfoMap.size(), typeInfoMap.keySet());

		Map<String, RecordInfo> recordInfoMap = buildRecordInfoMap(recordList);
		logger.info("recordInfoMap {} {}", recordInfoMap.size(), recordInfoMap.keySet());
		
		for(TypeInfo e: typeInfoMap.values()) {
			logger.info("typeInfo {}", e);
		}
		for(RecordInfo e: recordInfoMap.values()) {
			logger.info("recordInfo {}", e);
		}
		
		// Sanity check
		{
			Set<String> knownTypeSet = new TreeSet<>();
			{
				knownTypeSet.add("boolean");
				knownTypeSet.add("CARD8");
				knownTypeSet.add("CARD16");
				knownTypeSet.add("CARD32");
				
				typeInfoMap.values().stream().forEach(o -> knownTypeSet.add(o.name));
				recordInfoMap.values().stream().forEach(o -> knownTypeSet.add(o.name));
			}

			for(TypeInfo e: typeInfoMap.values()) {
				if (!knownTypeSet.contains(e.type)) {
					logger.error("unknown type");
					logger.error("  type {}", e.type);
					throw new UnexpectedException("unknown type");
				}
				if (e.minValue < 0) {
					logger.error("minValue is negative");
					logger.error("  minValue {}", e.minValue);
					throw new UnexpectedException("minValue is negative");
				}
				if (e.maxValue < e.minValue) {
					logger.error("minValue is greater than maxValue");
					logger.error("  minValue {}", e.minValue);
					logger.error("  maxValue {}", e.maxValue);
					throw new UnexpectedException("minValue is greater than maxValue");
				}
			}
			
			for(RecordInfo e: recordInfoMap.values()) {
				if (e.size <= 0) {
					logger.error("size <= 0");
					logger.error("  size {}", e.size);
					throw new UnexpectedException("size <= 0");
				}
				if (e.fieldList.size() == 0) {
					logger.error("fieldList.size() == 0");
					logger.error("  fieldList {}", e.fieldList.size());
					throw new UnexpectedException("fieldList.size() == 0");
				}
				for(FieldInfo fe: e.fieldList) {
					if (fe.type.isEmpty()) continue;
					
					if (!knownTypeSet.contains(fe.type)) {
						logger.error("unknown type");
						logger.error("  type {}", fe.type);
						throw new UnexpectedException("unknown type");
					}
				}
			}
		}
		
		for(RecordInfo e: recordInfoMap.values()) {
			generateRecordClass(recordInfoMap, typeInfoMap, e);
		}
		
		logger.info("STOP");
	}

}
