package mh.majuro.genType;

import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import org.slf4j.LoggerFactory;

import mh.majuro.UnexpectedException;
import mh.majuro.util.AutoIndentPrintWriter;
import mh.majuro.util.CSVUtil;

public class GenerateType {
	static final org.slf4j.Logger logger = LoggerFactory.getLogger(GenerateType.class);

	static Map<String, RecordInfo> buildRecordMap(List<Record> recordList) {
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
			
		Map<String, RecordInfo> map = new TreeMap<>();
		
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
					
					map.put(name, new RecordInfo(name, size, fields));
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
			
			map.put(name, new RecordInfo(name, size, fields));
		}
		return map;
	}
	
	static Map<String, TypeInfo> buildTypeMap(List<Type> typeList) {
		Map<String, TypeInfo> map = new TreeMap<>();

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
			
			map.put(type.name, new TypeInfo(type.name, type.type, Integer.parseInt(type.minValue), Integer.parseInt(type.maxValue)));
		}
		
		return map;
	}

	static final String PATH_DIR = "src2/mh/majuro/mesa/type";
	
	static void generateInnerFieldGetSet(AutoIndentPrintWriter out, Context context, String type, String prefix) {
		RecordInfo recordInfo = context.getRecordInfo(type);
		for(FieldInfo fieldInfo: recordInfo.fieldList) {
			if (fieldInfo.isEmpty()) continue;
			String fieldType  = context.getBaseType(fieldInfo.type);
			String qClassName = String.format("%s.%s", recordInfo.name, fieldInfo.name);

			out.println("//   %s  %s", type, fieldInfo.name);
			
			out.println("public static final class %s {", fieldInfo.name);
			out.println("public static final int OFFSET = %s.OFFSET + %2d;", prefix, fieldInfo.offset);
			out.println();

			switch(fieldType) {
			case "boolean":
				out.println("public static boolean get(@LONG_POINTER int base) {");
				out.println("return %s.get(base + OFFSET);", qClassName);
				out.println("}");
				out.println("public static void set(@LONG_POINTER int base, boolean newValue) {");
				out.println("%s.set(base + OFFSET, newValue);", qClassName);
				out.println("}");
				break;
			case "CARD8":
			case "CARD16":
			case "CARD32":
				out.println("public static @%s int get(@LONG_POINTER int base) {", fieldType);
				out.println("return %s.get(base + OFFSET);", qClassName);
				out.println("}");
				out.println("public static void set(@LONG_POINTER int base, @%s int newValue) {", fieldType);
				out.println("%s.set(base + OFFSET, newValue);", qClassName);
				out.println("}");
				break;
			default:
				if (context.isRecord(fieldType)) {
					generateInnerFieldGetSet(out, context, fieldType, String.format("%s.%s", prefix, fieldInfo.name));
				} else {
					logger.error("nestedType {}", fieldType);
					throw new UnexpectedException();
				}
			}
			out.println("}");
		}
	}
	
	static void generateInnerArrayFieldGetSet(AutoIndentPrintWriter out, Context context, String type, String prefix) {
		RecordInfo recordInfo = context.getRecordInfo(type);
		for(FieldInfo fieldInfo: recordInfo.fieldList) {
			if (fieldInfo.isEmpty()) continue;
			String qClassName = String.format("%s.%s", recordInfo.name, fieldInfo.name);
			String fieldType  = context.getBaseType(fieldInfo.type);
			out.println("// %s  %s  %s", type, fieldInfo.name, fieldType);
			
			out.println("public static final class %s {", fieldInfo.name);
			out.println("public static final int OFFSET = %s.OFFSET + %2d;", prefix, fieldInfo.offset);
			out.println();
			
			switch(fieldType) {
			case "boolean":
				out.println("public static boolean get(@LONG_POINTER int base, int index) {");
				out.println("return %s.get(base + OFFSET + (ARRAY_SIZE * index));", qClassName);
				out.println("}");
				out.println("public static void set(@LONG_POINTER int base, int index, boolean newValue) {");
				out.println("%s.set(base + OFFSET + (ARRAY_SIZE * index), newValue);", qClassName);
				out.println("}");
				break;
			case "CARD8":
			case "CARD16":
			case "CARD32":
				out.println("public static @%s int get(@LONG_POINTER int base, int index) {", fieldType);
				out.println("return %s.get(base + OFFSET + (ARRAY_SIZE * index));", qClassName);
				out.println("}");
				out.println("public static void set(@LONG_POINTER int base, int index, @%s int newValue) {", fieldType);
				out.println("%s.set(base + OFFSET + (ARRAY_SIZE * index), newValue);", qClassName);
				out.println("}");
				break;
			default:
				if (context.isRecord(fieldType)) {
					generateInnerArrayFieldGetSet(out, context, fieldType, String.format("%s.%s", prefix, fieldInfo.name));
				} else {
					logger.error("nestedType {}", fieldType);
					throw new UnexpectedException();
				}
			}
			out.println("}");
		}
	}
	
	static boolean useMemoryClass(Context context, RecordInfo recordInfo) {
		for(FieldInfo fieldInfo: recordInfo.fieldList) {
			if (fieldInfo.isEmpty()) continue;
			switch(fieldInfo.fieldType) {
			case SIMPLE:
			case BIT:
			{
				String type = context.getBaseType(fieldInfo.type);
				switch(type) {
				case "boolean":
				case "CARD8":
				case "CARD16":
				case "CARD32":
					return true;
				default:
					break;
				}
			}
				break;
			case ARRAY:
			{
				ArrayFieldInfo arrayFieldInfo = (ArrayFieldInfo)fieldInfo;
				ArrayInfo arrayInfo = arrayFieldInfo.arrayInfo;
				String type = context.getBaseType(arrayInfo.type);
				switch(type) {
				case "CARD16":
					return true;
				case "boolean":
				case "CARD8":
				case "CARD32":
					throw new UnexpectedException();
				default:
					break;
				}
			}
				break;
			default:
				throw new UnexpectedException();
			}
		}
		return false;
	}

	static void generateRecordClass(Context context, RecordInfo recordInfo) {
		String path = String.format("%s/%s.java", PATH_DIR, recordInfo.name);
		logger.info("path {}",path);
		try (AutoIndentPrintWriter out = new AutoIndentPrintWriter(new PrintWriter(path))) {			
			out.println("package mh.majuro.mesa.type;");
			out.println();
			
			if (useMemoryClass(context, recordInfo)) {
				out.println("import mh.majuro.mesa.Memory;");
			}
			
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
			}
			out.println();
			
			for(FieldInfo fieldInfo: recordInfo.fieldList) {
				if (fieldInfo.isEmpty()) continue;

				out.println("public static final class %s {", fieldInfo.name);
				out.println("public static final         int SIZE       = %2d;", fieldInfo.size);
				out.println("public static final         int OFFSET     = %2d;", fieldInfo.offset);

				{
					switch (fieldInfo.fieldType) {
					case SIMPLE:
						break;
					case BIT:
					{
						BitFieldInfo bitFieldInfo = (BitFieldInfo)fieldInfo;
						switch(bitFieldInfo.size) {
						case 1:
							out.println("public static final         int SHIFT      = %2d;", bitFieldInfo.bitInfo.shift);
							out.println("public static final @CARD16 int MASK       = %s;", bitFieldInfo.bitInfo.mask);
							out.println();
							
							out.println("public static @CARD16 int getBit(@CARD16 int value) {");
							out.println("return (value & MASK) >>> SHIFT;");
							out.println("}");
							out.println("public static @CARD16 int setBit(@CARD16 int value, @CARD16 int newValue) {");
							out.println("return ((newValue << SHIFT) & MASK) | (value & ~MASK);");
							out.println("}");
							break;
						case 2:
							out.println("public static final @CARD32 int MASK       = %s;", bitFieldInfo.bitInfo.mask);
							out.println("public static final         int SHIFT      = %d;", bitFieldInfo.bitInfo.shift);
							out.println();
							
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
					{
						ArrayFieldInfo arrayFieldInfo = (ArrayFieldInfo)fieldInfo;

						out.println("public static final         int ARRAY_SIZE = %2d;", arrayFieldInfo.arrayInfo.size);
						out.println("public static final         int ARRAY_LEN  = %2d;", arrayFieldInfo.arrayInfo.length);
						out.println();
					}
						break;
					default:
						throw new UnexpectedException();
					}

				}
				
				//
				// get set methods
				//
				{
					String qClassName = String.format("%s.%s", recordInfo.name, fieldInfo.name);
					switch(fieldInfo.fieldType) {
					case SIMPLE:
					{
						String type = context.getBaseType(fieldInfo.type);
						switch(type) {
						case "boolean":
							out.println("public static boolean get(@LONG_POINTER int base) {");
							out.println("return Memory.fetch(base + OFFSET) != 0;");
							out.println("}");
							out.println("public static void set(@LONG_POINTER int base, boolean newValue) {");
							out.println("Memory.store(base + OFFSET, (newValue ? 1 : 0));");
							out.println("}");
							break;
						case "CARD8":
						case "CARD16":
							out.println("public static @%s int get(@LONG_POINTER int base) {", type);
							out.println("return Memory.fetch(base + OFFSET);");
							out.println("}");
							out.println("public static void set(@LONG_POINTER int base, @%s int newValue) {", type);
							out.println("Memory.store(base + OFFSET, newValue);");
							out.println("}");
							break;
						case "CARD32":
							out.println("public static @%s int get(@LONG_POINTER int base) {", type);
							out.println("return Memory.readDbl(base + OFFSET);");
							out.println("}");
							out.println("public static void set(@LONG_POINTER int base, @%s int newValue) {", type);
							out.println("Memory.writeDbl(base + OFFSET, newValue);");
							out.println("}");
							break;
						default:
							if (context.isRecord(type)) {
								generateInnerFieldGetSet(out, context, type, fieldInfo.name);
							} else {
								logger.error("type {}", type);
								throw new UnexpectedException();
							}
						}
					}
						break;
					case BIT:
					{
						String type = context.getBaseType(fieldInfo.type);
						switch(type) {
						case "boolean":
							out.println("public static boolean get(@LONG_POINTER int base) {");
							out.println("return getBit(Memory.fetch(base + OFFSET)) != 0;");
							out.println("}");
							out.println("public static void set(@LONG_POINTER int base, boolean newValue) {");
							out.println("Memory.modify(base + OFFSET, %1$s::setBit, (newValue ? 1 : 0));", qClassName);
							out.println("}");
							break;
						case "CARD8":
						case "CARD16":
							out.println("public static @%s int get(@LONG_POINTER int base) {", type);
							out.println("return getBit(Memory.fetch(base + OFFSET));");
							out.println("}");
							out.println("public static void set(@LONG_POINTER int base, @%s int newValue) {", type);
							out.println("Memory.modify(base + OFFSET, %1$s::setBit, newValue);", qClassName);
							out.println("}");
							break;
						case "CARD32":
							out.println("public static @%s int get(@LONG_POINTER int base) {", type);
							out.println("return getBit(Memory.readDbl(base + OFFSET));");
							out.println("}");
							out.println("public static void set(@LONG_POINTER int base, @%s int newValue) {", type);
							out.println("Memory.modifyDbl(base + OFFSET, %1$s::setBit, newValue);", qClassName);
							out.println("}");
							break;
						default:
							logger.error("type {}", type);
							throw new UnexpectedException();
						}
					}
						break;
					case ARRAY:
					{
						ArrayFieldInfo arrayFieldInfo = (ArrayFieldInfo)fieldInfo;
						ArrayInfo arrayInfo = arrayFieldInfo.arrayInfo;
						String type = context.getBaseType(arrayInfo.type);
						
						switch(type) {
						case "CARD16":
							out.println("public static @%s int get(@LONG_POINTER int base, int index) {", type);
							out.println("return Memory.fetch(base + OFFSET + (ARRAY_SIZE * index));");
							out.println("}");
							out.println("public static void set(@LONG_POINTER int base, int index, @%s int newValue) {", type);
							out.println("Memory.store(base + OFFSET + (ARRAY_SIZE * index), newValue);");
							out.println("}");
							break;
						default:
							if (context.isRecord(type)) {
								generateInnerArrayFieldGetSet(out, context, type, fieldInfo.name);
							} else {
								logger.error("type {}", type);
								throw new UnexpectedException();
							}
						}
					}
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
		
		Map<String, TypeInfo> typeMap = buildTypeMap(typeList);
		logger.info("typeInfoMap  {} {}", typeMap.size(), typeMap.keySet());

		Map<String, RecordInfo> recordMap = buildRecordMap(recordList);
		logger.info("recordInfoMap {} {}", recordMap.size(), recordMap.keySet());
		
		Context context = new Context(recordMap, typeMap);
		// Sanity check
		context.sanityCheck();
		
		for(RecordInfo e: recordMap.values()) {
			generateRecordClass(context, e);
		}
		
		logger.info("STOP");
	}

}
