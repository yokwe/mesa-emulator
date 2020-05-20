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
	
	static String bitMask(BitFieldInfo bitFieldInfo) {
		int width = bitFieldInfo.stopBit - bitFieldInfo.startBit;
		switch(bitFieldInfo.size) {
		case 1:
		{
			int shift = 15 - bitFieldInfo.stopBit;
			int bit   = ((1 << (width + 1)) - 1) & 0xFFFF;
			return String.format("0x%04X", (bit << shift) & 0xFFFF);
		}
		case 2:
		{
			int shift = 31 - bitFieldInfo.stopBit;
			int bit   = (1 << (width + 1)) - 1;
			return String.format("0x%08X", (bit << shift));
		}
		default:
			logger.error("size {}", bitFieldInfo.size);
			throw new UnexpectedException();
		}
	}
	
	private static String buildPrefixName(String prefix, String name) {
		return String.format("%s%s%s", prefix, name.substring(0, 1).toUpperCase(), name.substring(1));
	}
	
	private static String toTitleCase(String name) {
		return String.format("%s%s", name.substring(0, 1).toUpperCase(), name.substring(1));
	}
	
	static void generateRecordClass(Map<String, RecordInfo> recordInfoMap, Map<String, TypeInfo> typeInfoMap, RecordInfo recordInfo) {
		String path = String.format("%s/%s.java", PATH_DIR, recordInfo.name);
		logger.info("path {}",path);
		try (AutoIndentPrintWriter out = new AutoIndentPrintWriter(new PrintWriter(path))) {
			out.println("package mh.majuro.mesa.type;");
			out.println();
			out.println("import mh.majuro.mesa.Type.*;");
			out.println();
			out.println("public final class %s extends RecordBase {", recordInfo.name);
			out.println("public static final int SIZE = %d;", recordInfo.size);
			out.println();
			
			for(FieldInfo fieldInfo: recordInfo.fieldList) {
				// 	public static int WORD_OFFSET       = 0;
				out.println("public static final class %s {", toTitleCase(fieldInfo.name));
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
						out.println("public static final @CARD16 int MASK    = %s;", bitMask(bitFieldInfo));
						out.println("public static final         int SHIFT   = %d;", 15 - bitFieldInfo.stopBit);
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
						out.println("public static final @CARD32 int MASK    = %s;", bitMask(bitFieldInfo));
						out.println("public static final         int SHIFT   = %d;", 31 - bitFieldInfo.stopBit);
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
					break;
				default:
					throw new UnexpectedException();
				}
					
				out.println("}");
			}
			out.println();

			
//			// field get
			out.println("// field get");
			for(FieldInfo fieldInfo: recordInfo.fieldList) {
				if (fieldInfo.isEmpty()) continue;
				
				switch(fieldInfo.fieldType) {
				case NORMAL:
				{
					switch(fieldInfo.type) {
					case "boolean":
						out.println("public static boolean %s(@LONG_POINTER int base) {", buildPrefixName("get", fieldInfo.name));
						out.println("return get(%s.%s::offset, base) != 0;", recordInfo.name, toTitleCase(fieldInfo.name));
						out.println("}");
						out.println("public static void %s(@LONG_POINTER int base, boolean newValue) {", buildPrefixName("set", fieldInfo.name));
						out.println("set(%s.%s::offset, base, (newValue ? 1 : 0));", recordInfo.name, toTitleCase(fieldInfo.name));
						out.println("}");
						break;
					case "CARD8":
					case "CARD16":
						out.println("public static @%s int %s(@LONG_POINTER int base) {", fieldInfo.type, buildPrefixName("get", fieldInfo.name));
						out.println("return get(%s.%s::offset, base);", recordInfo.name, toTitleCase(fieldInfo.name));
						out.println("}");
						out.println("public static void %s(@LONG_POINTER int base, @%s int newValue) {", buildPrefixName("set", fieldInfo.name), fieldInfo.type);
						out.println("set(%s.%s::offset, base, newValue);", recordInfo.name, toTitleCase(fieldInfo.name));
						out.println("}");
						break;
					case "CARD32":
						out.println("public static @%s int %s(@LONG_POINTER int base) {", fieldInfo.type, buildPrefixName("get", fieldInfo.name));
						out.println("return getDbl(%s.%s::offset, base);", recordInfo.name, toTitleCase(fieldInfo.name));
						out.println("}");
						out.println("public static void %s(@LONG_POINTER int base, @%s int newValue) {", buildPrefixName("set", fieldInfo.name), fieldInfo.type);
						out.println("setDbl(%s.%s::offset, base, newValue);", recordInfo.name, toTitleCase(fieldInfo.name));
						out.println("}");
						break;
					default:
					{
						String type = fieldInfo.type;
						if (typeInfoMap.containsKey(type)) {
							String realType = typeInfoMap.get(type).type;
							out.println("public static @%s int %s(@LONG_POINTER int base) {", realType, buildPrefixName("get", fieldInfo.name));
							out.println("return get(%s.%s::offset, base);", recordInfo.name, toTitleCase(fieldInfo.name));
							out.println("}");
							out.println("public static void %s(@LONG_POINTER int base, @%s int newValue) {", buildPrefixName("set", fieldInfo.name), realType);
							out.println("set(%s.%s::offset, base, newValue);", recordInfo.name, toTitleCase(fieldInfo.name));
							out.println("}");
						} else if (recordInfoMap.containsKey(type)) {
							RecordInfo refRecordInfo = recordInfoMap.get(type);
							if (refRecordInfo.size == 1) {
								out.println("public static @CARD16 int %s(@LONG_POINTER int base) {", buildPrefixName("get", fieldInfo.name));
								out.println("return get(%s.%s::offset, base);", recordInfo.name, toTitleCase(fieldInfo.name));
								out.println("}");
								out.println("public static void get%s(@LONG_POINTER int base, @CARD16 int newValue) {", buildPrefixName("set", fieldInfo.name));
								out.println("set(%s.%s::offset, base, newValue);", recordInfo.name, toTitleCase(fieldInfo.name));
								out.println("}");
							} else {
								logger.error("refRecordInfo {}", refRecordInfo);
								throw new UnexpectedException();
							}
						} else {
							logger.error("type {}", type);
							throw new UnexpectedException();
						}
					}
						break;
					}
				}
					break;
				case BIT:
				{
					switch(fieldInfo.type) {
					case "boolean":
						// public static final int getBitField(ToIntIntFunction addressFunc, ToIntIntFunction getValueFunc, @LONG_POINTER int base)
						out.println("public static boolean %s(@LONG_POINTER int base) {", buildPrefixName("get", fieldInfo.name));
						out.println("return getBitField(%s.%s::offset, %s.%s::getBit, base) != 0;", recordInfo.name, toTitleCase(fieldInfo.name), recordInfo.name, toTitleCase(fieldInfo.name));
						out.println("}");
						out.println("public static void %s(@LONG_POINTER int base, boolean newValue) {", buildPrefixName("set", fieldInfo.name));
						out.println("setBitField(%s.%s::offset, %s.%s::setBit, base, (newValue ? 1 : 0));", recordInfo.name, toTitleCase(fieldInfo.name), recordInfo.name, toTitleCase(fieldInfo.name));
						out.println("}");
						break;
					case "CARD8":
					case "CARD16":
						out.println("public static @%s int %s(@LONG_POINTER int base) {", fieldInfo.type, buildPrefixName("get", fieldInfo.name));
						out.println("return getBitField(%s.%s::offset, %s.%s::getBit, base);", recordInfo.name, toTitleCase(fieldInfo.name), recordInfo.name, toTitleCase(fieldInfo.name));
						out.println("}");
						out.println("public static void %s(@LONG_POINTER int base, @%s int newValue) {", buildPrefixName("set", fieldInfo.name), fieldInfo.type);
						out.println("setBitField(%s.%s::offset, %s.%s::setBit, base, newValue);", recordInfo.name, toTitleCase(fieldInfo.name), recordInfo.name, toTitleCase(fieldInfo.name));
						out.println("}");
						break;
					case "CARD32":
						out.println("public static @%s int %s(@LONG_POINTER int base) {", fieldInfo.type, buildPrefixName("get", fieldInfo.name));
						out.println("return getBitFieldDbl(%s.%s::offset, %s.%s::getBit, base);", recordInfo.name, toTitleCase(fieldInfo.name), recordInfo.name, toTitleCase(fieldInfo.name));
						out.println("}");
						out.println("public static void %s(@LONG_POINTER int base, @%s int newValue) {", buildPrefixName("set", fieldInfo.name), fieldInfo.type);
						out.println("setBitFieldDbl(%s.%s::offset, %s.%s::setBit, base, newValue);", recordInfo.name, toTitleCase(fieldInfo.name), recordInfo.name, toTitleCase(fieldInfo.name));
						out.println("}");
						break;
					default:
					{
						String type = fieldInfo.type;
						if (typeInfoMap.containsKey(type)) {
							String realType = typeInfoMap.get(type).type;
							switch(realType) {
							case "CARD8":
							case "CARD16":
								out.println("public static @%s int %s(@LONG_POINTER int base) {", realType, buildPrefixName("get", fieldInfo.name));
								out.println("return getBitField(%s.%s::offset, %s.%s::getBit, base);", recordInfo.name, toTitleCase(fieldInfo.name), recordInfo.name, toTitleCase(fieldInfo.name));
								out.println("}");
								out.println("public static void %s(@LONG_POINTER int base, @%s int newValue) {", buildPrefixName("set", fieldInfo.name), realType);
								out.println("setBitField(%s.%s::offset, %s.%s::setBit, base, newValue);", recordInfo.name, toTitleCase(fieldInfo.name), recordInfo.name, toTitleCase(fieldInfo.name));
								out.println("}");
								break;
							case "CARD32":
								out.println("public static @%s int %s(@LONG_POINTER int base) {", realType, buildPrefixName("get", fieldInfo.name));
								out.println("return getBitFieldDbl(%s.%s::offset, %s.%s::getBit, base);", recordInfo.name, toTitleCase(fieldInfo.name), recordInfo.name, toTitleCase(fieldInfo.name));
								out.println("}");
								out.println("public static void %s(@LONG_POINTER int base, @%s int newValue) {", buildPrefixName("set", fieldInfo.name), realType);
								out.println("setBitFieldDbl(%s.%s::offset, %s.%s::setBit, base, newValue);", recordInfo.name, toTitleCase(fieldInfo.name), recordInfo.name, toTitleCase(fieldInfo.name));
								out.println("}");
								break;
							default:
								// FIXME
								out.println("// FIXME %s", fieldInfo.toString());
							}
						} else if (recordInfoMap.containsKey(type)) {
							// FIXME
							out.println("// FIXME %s", fieldInfo.toString());
						} else {
							// FIXME
							out.println("// FIXME %s", fieldInfo.toString());
						}
					}
						break;
					}
				}
					break;
				case ARRAY:
					// FIXME
					out.println("// FIXME %s", fieldInfo.toString());
					break;
				default:
					throw new UnexpectedException();
				}
			}
			out.println();



			
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
