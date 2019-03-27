package mesa.courier.compiler;

import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import mesa.courier.program.Program;
import mesa.courier.program.Type;
import mesa.courier.program.Type.Correspondence;
import mesa.courier.program.Type.Field;
import mesa.courier.program.TypeChoice.Candidate;
import mesa.courier.program.TypeArray;
import mesa.courier.program.TypeBlock;
import mesa.courier.program.TypeChoice;
import mesa.courier.program.TypeEnum;
import mesa.courier.program.TypeRecord;
import mesa.courier.program.TypeReference;
import mesa.courier.program.TypeSequence;
import mesa.courier.program.Util;

public class Compiler {
	protected static final Logger logger = LoggerFactory.getLogger(Compiler.class);

	public static String STUB_DIR_PATH = "src/stub/";
	
	private static List<String> layoutStringString(List<String> c1, List<String> c2) {
		if (c1.size() != c2.size()) {
			throw new CompilerException(String.format("Unexpected size  c1 = %d  c2 = %d", c1.size(), c2.size()));
		}
		int size = c1.size();
		
		int w1 = 0;
		for(String value: c1) {
			int length = value.trim().length();
			if (w1 < length) w1 = length;
		}
		String format = String.format("%%%ds %%s", -w1);
		
		List<String> ret = new ArrayList<>();
		for(int i = 0; i < size; i++) {
			ret.add(String.format(format, c1.get(i).trim(), c2.get(i).trim()));
		}
		return ret;
	}
	private static List<String> layoutStringString(List<String> c1, List<String> c2, List<String> c3) {
		if (c1.size() != c2.size()) {
			throw new CompilerException(String.format("Unexpected size  c1 = %d  c2 = %d", c1.size(), c2.size()));
		}
		if (c1.size() != c3.size()) {
			throw new CompilerException(String.format("Unexpected size  c1 = %d  c3 = %d", c1.size(), c3.size()));
		}
		int size = c1.size();
		
		int w1 = 0;
		for(String value: c1) {
			int length = value.trim().length();
			if (w1 < length) w1 = length;
		}
		int w2 = 0;
		for(String value: c2) {
			int length = value.trim().length();
			if (w2 < length) w2 = length;
		}
		String format = String.format("%%%ds %%%ds %%s", -w1, -w2);
		
		List<String> ret = new ArrayList<>();
		for(int i = 0; i < size; i++) {
			ret.add(String.format(format, c1.get(i).trim(), c2.get(i).trim(), c3.get(i).trim()));
		}
		return ret;
	}
	private static List<String> layoutEnumElement(List<String> c1, List<Integer> c2) {
		if (c1.size() != c2.size()) {
			throw new CompilerException(String.format("Unexpected size  c1 = %d  c2 = %d", c1.size(), c2.size()));
		}
		int size = c1.size();
		
		int w1 = 0;
		for(String value: c1) {
			int length = value.trim().length();
			if (w1 < length) w1 = length;
		}
		int w2 = 0;
		for(Integer value: c2) {
			String string = String.format("%d", value);
			int length = string.length();
			if (w2 < length) w2 = length;
		}
		String format = String.format("%%%ds = %%%ds,", -w1, w2);
		
		List<String> ret = new ArrayList<>();
		for(int i = 0; i < size; i++) {
			ret.add(String.format(format, c1.get(i).trim(), c2.get(i).intValue()));
		}
		return ret;
	}


	public static class Printer implements AutoCloseable {
		public static final String INDENT = "    ";
		public static final String NEST   = "!!<<NEST>>!!";
		public static final String UNNEST = "!!<<UNNEST>>!!";

		private final PrintWriter out;
		
		private int level = 0;

		public Printer(PrintWriter out) {
			this.out = out;
		}
		public void close() {
			out.close();
		}
		public void nest() {
			level++;
		}
		public void unnest() {
			level--;
			if (level < 0) throw new CompilerException("level < 0");
		}

		public void println(String format, Object... args) {
			for(int i = 0; i < level; i++) out.print(INDENT);
			out.format(format, args);
			out.println();
		}
		public void println(String[] stringArray) {
			for(String string: stringArray) {
				if (string.equals(NEST)) {
					nest();
				} else if (string.equals(UNNEST)) {
					unnest();
				} else {
					for(int i = 0; i < level; i++) out.print(INDENT);
					out.println(string);
				}
			}
		}
		public void println() {
			out.println();
		}
	}
	
	private final Program program;
	public Compiler(Program program) {
		this.program = program;
	}

	private void genOpening(Printer outh, Printer outc) {
		// for outh
		// write opening lines
		outh.println("#ifndef STUB_%s_H__", program.info.getProgramVersion());
		outh.println("#define STUB_%s_H__", program.info.getProgramVersion());
		outh.println();

		// include courier header
		outh.println("#include \"../courier/Courier.h\"");			
		// include depend module
		for(Program.Info info: program.depends) {
			outh.println("#include \"../stub/%s.h\"", info.getProgramVersion());
		}

		// output namespace
		outh.println();
		outh.println("namespace Courier {");

		// output main namespace
		outh.println("namespace %s {", program.info.getProgramVersion());
		if (program.info.version != 0) {
			outh.println();
			outh.println("const quint32 PROGRAM_NUMBER = %d;", program.info.program);
			outh.println("const quint32 VERSION_NUMBER = %d;", program.info.version);
		}

		
		// for outc
		outc.println("#include \"../../util/Util.h\"");
		outc.println("static log4cpp::Category& logger = Logger::getLogger(\"stub/%s\");", program.info.getProgramVersion());
		outc.println();
		outc.println("#include \"../stub/%s.h\"", program.info.getProgramVersion());
		outc.println();
	}
	private void genClosing(Printer outh, Printer outc) {
		// for outh
		outh.println();
		outh.println("}"); // for namespace for progaram
		outh.println("}"); // for namespace Courier
		
		// for outc
		// nothing for now
	}
	
	private String toTypeString(Type type) {
		switch(type.kind) {
		// predefined
		case BOOLEAN:
		case BYTE:
		case CARDINAL:
		case LONG_CARDINAL:
		case STRING:
		case UNSPECIFIED:
		case UNSPECIFIED2:
		case UNSPECIFIED3:
			return String.format("%s", type.kind.toString());
		// constructed
		case ENUM:
			break;
		case ARRAY:
		{
			TypeArray typeArray = (TypeArray)type;
			return String.format("ARRAY<%s>", toTypeString(typeArray.type));
		}
		case BLOCK:
			return String.format("%s", type.kind.toString());
		case SEQUENCE:
		{
			TypeSequence typeSequence = (TypeSequence)type;
			
			if (typeSequence.size == TypeSequence.MAX_SIZE) {
				return String.format("SEQUENCE<%s>", toTypeString(typeSequence.type));
			} else {
				return String.format("SEQUENCE<%s>", toTypeString(typeSequence.type));
			}
		}
		case RECORD:
			break;
		case CHOICE:
			break;
		case PROCEDURE:
		case ERROR:
			break;
		// reference
		case REFERENCE:
		{
			TypeReference typeReference = (TypeReference)type;
			Type realType = typeReference.getConcreteType();
			if (realType.kind == Type.Kind.ENUM) {
				return String.format("ENUM<%s>", program.getLocalRefName(typeReference));
			} else {
				return program.getLocalRefName(typeReference);
			}
		}
		default:
			break;
		}
		throw new CompilerException(String.format("Unexpected type %s", type.toString()));
	}
	
	private void genTypeDecl(Printer outh, Printer outc, String namePrefix) {
		outh.println("");
		outh.println("//");
		outh.println("// Type Declaration");
		outh.println("//");

		for(Program.DeclType declType: program.typeList) {
			outh.println();

			Type   type = declType.type;
			String name = Util.sanitizeSymbol(declType.name);
			
			switch(type.kind) {
			// predefined
			case BOOLEAN:
			case BYTE:
			case CARDINAL:
			case LONG_CARDINAL:
			case STRING:
			case UNSPECIFIED:
			case UNSPECIFIED2:
			case UNSPECIFIED3:
				outh.println("using %s = %s;", name, type);
				break;
			// constructed
			case ENUM:
				genTypeDeclEnum(outh, outc, (TypeEnum)type, name, namePrefix);
				break;
			case ARRAY:
				logger.error("Rewrite \"ARRAY N OF T\" to \"RECORD [value: ARRAY N OF T]\"");
				logger.error("  {}  {}", name, type.toString());
				throw new CompilerException(String.format("Unexpected type %s", type.toString()));
			case BLOCK:
				logger.error("Rewrite \"RECORD N OF BYTE\" to \"RECORD [value: RECORD N OF BYTE]\"");
				logger.error("  {}  {}", name, type.toString());
				throw new CompilerException(String.format("Unexpected type %s", type.toString()));
			case SEQUENCE:
				logger.error("Rewrite \"SEQUENCE N OF T\" to \"RECORD [value: SEQUENCE N OF T]\"");
				logger.error("  {}  {}", name, type.toString());
				throw new CompilerException(String.format("Unexpected type %s", type.toString()));
			case RECORD:
				genTypeDeclRecord(outh, outc, (TypeRecord)type, name, namePrefix);
				break;
			case CHOICE:
				genTypeDeclChoice(outh, outc, (TypeChoice)type, name, namePrefix);
				break;
			case PROCEDURE:
			case ERROR:
				outh.println("// FIXME TypeDecl %s %s", name, type.toString());
				break;
			// reference
			case REFERENCE:
				outh.println("using %s = %s;", name, toTypeString(type));
				break;
			default:
				throw new CompilerException(String.format("Unexpected type %s", type.toString()));
			}
		}
	}
	
	private void genTypeDeclRecord(Printer outh, Printer outc, TypeRecord type, String name, String namePrefix) {
		List<String> c1 = new ArrayList<>();
		List<String> c2 = new ArrayList<>();
		for(Field field: type.fields) {
			String fieldType;
			String fieldName;
			switch(field.type.kind) {
			case ARRAY:
			{
				TypeArray typeArray = (TypeArray)field.type;
				fieldType = toTypeString(field.type);
				fieldName = String.format("%s{%d}", field.name, typeArray.size);	
			}
				break;
			case SEQUENCE:
			{
				TypeSequence typeSequence = (TypeSequence)field.type;
				fieldType = toTypeString(field.type);
				if (typeSequence.size == TypeSequence.MAX_SIZE) {
					fieldName = field.name;
				} else {
					fieldName = String.format("%s{%d}", field.name, typeSequence.size);	
				}
			}
				break;
			case BLOCK:
			{
				TypeBlock typeBlock = (TypeBlock)field.type;
				fieldType = toTypeString(field.type);
				fieldName = String.format("%s{%d}", field.name, typeBlock.size);	
			}
				break;
			default:
				fieldType = toTypeString(field.type);
				fieldName = field.name;
				break;
			}
			
			c1.add(fieldType);
			c2.add(String.format("%s;", Util.sanitizeSymbol(fieldName)));
		}

		outh.println("struct %s {", name);
		outh.nest();
		for(String line: layoutStringString(c1, c2)) {
			outh.println(line);
		}
		
		outh.println();
		outh.println(new String[] {
			"void serialize  (BLOCK& block) const;",
			"void deserialize(BLOCK& block);",
			"QString toString();"
			});

		outh.unnest();
		outh.println("};");
	}
	
	private void genTypeDeclEnum(Printer outh, Printer outc, TypeEnum type, String name, String namePrefix) {
		List<String>  c1 = new ArrayList<>();
		List<Integer> c2 = new ArrayList<>();
		for(Correspondence correspondence: type.elements) {
			c1.add(Util.sanitizeSymbol(correspondence.id));
			c2.add((int)correspondence.numericValue);
		}
		
		outh.println("enum class %s : quint16 {", name);
		outh.nest();	
		for(String line: layoutEnumElement(c1, c2)) {
			outh.println(line);
		}
		outh.unnest();
		outh.println("};");
	}
	
	private void genTypeDeclChoice(Printer outh, Printer outc, TypeChoice type, String name, String namePrefix) {
		outh.println("// FIXME TypeDecl %s %s", name, type.toString());
		outh.println("struct %s {", name);
		outh.nest();
		
		if (type instanceof TypeChoice.Typed) {
			TypeChoice.Typed typed = (TypeChoice.Typed)type;
			
			outh.println("using CHOICE_TAG = %s;", toTypeString(typed.type));
			
			List<String>         choiceNameList  = new ArrayList<>(); // choice name list in appearance order
			Map<String, Integer> choiceMap       = new TreeMap<>();   // choice name => struct number
			int                  maxStructNumber = 0;
			{
				for(Candidate<String> candidate: typed.candidates) {
					maxStructNumber++;
					Type candidateType = candidate.type;
					
					// build choiceMap
					for(String id: candidate.designators) {
						String choiceName = id;
						choiceNameList.add(choiceName);
						choiceMap.put(choiceName, maxStructNumber);
					}
					
					// generate choice struct
					if (candidateType.kind == Type.Kind.RECORD) {
						TypeRecord typeRecord = (TypeRecord)candidateType;
						String structName = String.format("CHOICE_%02d", maxStructNumber);
						if (typeRecord.fields.size() == 0) {
							outh.println("struct %s {};", structName);
						} else {
							genTypeDeclRecord(outh, outc, typeRecord, structName, String.format("%s::%s", namePrefix, name));
						}
					} else {
						throw new CompilerException(String.format("Unexpected type %s", type.toString()));
					}
				}
			}
			
			outh.println();
			outh.println("CHOICE_TAG choiceTag;");
			
			for(String choiceName: choiceNameList) {
				int structNumber = choiceMap.get(choiceName);
				outh.println("CHOICE_%02d& %s() {", structNumber, Util.sanitizeSymbol(choiceName));
				outh.nest();
				outh.println("return choice_%02d;", structNumber);
				outh.unnest();
				outh.println("}");
			}
			
			outh.println();
			outh.println(new String[] {
					"void serialize  (BLOCK& block) const;",
					"void deserialize(BLOCK& block);",
					"QString toString();"
					});

			outh.unnest();
			outh.println("private:");
			outh.nest();
			for(int i = 1; i <= maxStructNumber; i++) {
				outh.println("CHOICE_%02d  choice_%02d;", i, i);
			}
		} else if (type instanceof TypeChoice.Anon) {
			TypeChoice.Anon anon = (TypeChoice.Anon)type;

			List<String>         choiceNameList  = new ArrayList<>(); // choice name list in appearance order
			Map<String, Integer> choiceMap       = new TreeMap<>();   // choice name => struct number
			int                  maxStructNumber = 0;

			outh.println("enum class CHOICE_TAG : quint16 {");
			outh.nest();
			for(Candidate<Correspondence> candidate: anon.candidates) {
				for(Correspondence correspondence: candidate.designators) {
					outh.println("%s = %s,", correspondence.id, correspondence.numericValue);
				}
			}
			outh.unnest();
			outh.println("};");
			
			for(Candidate<Correspondence> candidate: anon.candidates) {
				maxStructNumber++;
				Type candidateType = candidate.type;
				
				// build choiceMap
				for(Correspondence correspondence: candidate.designators) {
					String choiceName = correspondence.id;
					choiceNameList.add(choiceName);
					choiceMap.put(choiceName, maxStructNumber);
				}
				
				// generate choice struct
				if (candidateType.kind == Type.Kind.RECORD) {
					TypeRecord typeRecord = (TypeRecord)candidateType;
					String structName = String.format("CHOICE_%02d", maxStructNumber);
					if (typeRecord.fields.size() == 0) {
						outh.println("struct %s {};", structName);
					} else {
						genTypeDeclRecord(outh, outc, typeRecord, structName, String.format("%s::%s", namePrefix, name));

					}
				} else {
					throw new CompilerException(String.format("Unexpected type %s", type.toString()));
				}
			}
			outh.println();

			for(String choiceName: choiceNameList) {
				int structNumber = choiceMap.get(choiceName);
				outh.println("CHOICE_%02d& %s() {", structNumber, Util.sanitizeSymbol(choiceName));
				outh.nest();
				outh.println("return choice_%02d;", structNumber);
				outh.unnest();
				outh.println("}");
			}
			outh.println();

			outh.println();
			outh.println(new String[] {
					"void serialize  (BLOCK& block) const;",
					"void deserialize(BLOCK& block);",
					"QString toString();"
					});

			outh.unnest();
			outh.println("private:");
			outh.nest();
			for(int i = 1; i <= maxStructNumber; i++) {
				outh.println("CHOICE_%02d  choice_%02d;", i, i);
			}

		} else {
			throw new CompilerException(String.format("Unexpected type %s", type.toString()));
		}
		
		outh.unnest();
		outh.println("};");
	}
	
	private void genEnumFunc(Printer outh, Printer outc, String namePrefix) {
		List<Program.DeclType> declTypeList = new ArrayList<>();
		for(Program.DeclType declType: program.typeList) {
			if (declType.type.kind == Type.Kind.ENUM) {
				declTypeList.add(declType);
			};
		}
		if (declTypeList.isEmpty()) return;
		
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(Program.DeclType declType: declTypeList) {				
				String name        = declType.name;
				String programName = program.info.getProgramVersion();
				
				c1.add(String.format("QString toString(const %s::%s::%s", namePrefix, programName, name));
				c2.add("value);");
			}
			
			outh.println();
			outh.println(new String[] {
					"//",
					"// Enum toString Declaration",
					"//"
					});
			for(String line: layoutStringString(c1, c2)) {
				outh.println(line);
			}
		}
		
		outc.println();
		outh.println(new String[] {
				"//",
				"// Enum Function Definition",
				"//"
				});

		for(Program.DeclType declType: declTypeList) {			
			TypeEnum type = (TypeEnum)declType.type;
			String   name        = declType.name;
			String   programName = program.info.getProgramVersion();
			
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();

			for(Correspondence correspondence: type.elements) {
				String enumName   = correspondence.id;
				c1.add(String.format("{%s::%s::%s::%s,", namePrefix, programName, name, Util.sanitizeSymbol(enumName)));
				c2.add(String.format("\"%s\"},", enumName));
//				outc.println("{Courier::%s::%s::%s,  \"%s\"},", programName, name, Util.sanitizeSymbol(enumName), enumName);
			}

			outc.println();
			outc.println("QString toString(const %s::%s::%s value) {", namePrefix, programName, name);
			outc.nest();
			outc.println("static QMap<%s::%s::%s, QString> map = {", namePrefix, programName, name);
			outc.nest();
			
			for(String line: layoutStringString(c1, c2)) {
				outc.println(line);
			}
			
//			outc.unnest();
//			outc.println("};");
//			outc.println();
//			outc.println("if (map.contains(value)) {");
//			outc.nest();
//			outc.println("return map[value];");
//			outc.unnest();
//			outc.println("} else {");
//			outc.nest();
//			outc.println("return QString(\"%%1\").arg((quint16)value);");
//			outc.unnest();
//			outc.println("}");
//			outc.unnest();
//			outc.println("}");
			
			outc.println(new String[] {
					Printer.UNNEST,
					"};",
					"",
					"if (map.contains(value)) {",
					Printer.NEST,
						"return map[value];",
					Printer.UNNEST,
					"} else {",
					Printer.NEST,
						"return QString(\"%1\").arg((quint16)value);",
					Printer.UNNEST,
					"}",
					Printer.UNNEST,
				"}",
					});

		}
	}

/**/
	private void genSerializeFunc(Printer outh, Printer outc) {
		List<Program.DeclType> declTypeList = new ArrayList<>();
		for(Program.DeclType declType: program.typeList) {
			if (declType.type.kind == Type.Kind.RECORD) {
				declTypeList.add(declType);
			};
			if (declType.type.kind == Type.Kind.CHOICE) {
				declTypeList.add(declType);
			};
			if (declType.type.kind == Type.Kind.ENUM) {
				declTypeList.add(declType);
			};
		}
		if (declTypeList.isEmpty()) return;
		
		outh.println("");
		outh.println("//");
		outh.println("// Serialize Function Declaration");
		outh.println("//");
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(Program.DeclType declType: declTypeList) {				
				String   name        = declType.name;
				String   programName = program.info.getProgramVersion();
				
				// void serialize(Courier::BLOCK& block, const Courier::BYTE&          value);
				c1.add(String.format("void serialize(Courier::BLOCK& block, const Courier::%s::%s&", programName, name));
				c2.add("value);");
				
				if (declType.type.kind == Type.Kind.CHOICE) {
					// TODO output function for CHOICE_TAG
					// TODO output function for CHOICE_XX
				}
			}
			
			for(String line: layoutStringString(c1, c2)) {
				outh.println(line);
			}
		}
		
		outh.println("");
		outh.println("//");
		outh.println("// Deserizlize Function Declaration");
		outh.println("//");
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(Program.DeclType declType: declTypeList) {				
				String   name        = declType.name;
				String   programName = program.info.getProgramVersion();
				
				// void deserialize(Courier::BLOCK& block, Courier::BYTE&          value);
				c1.add(String.format("void deserialize(Courier::BLOCK& block, Courier::%s::%s&", programName, name));
				c2.add("value);");

				if (declType.type.kind == Type.Kind.CHOICE) {
					// TODO output function for CHOICE_TAG
					// TODO output function for CHOICE_XX
				}
			}
			
			for(String line: layoutStringString(c1, c2)) {
				outh.println(line);
			}
		}

		// TODO output serialization function definition
		outc.println("");
		outc.println("//");
		outc.println("// Serialize Function Definition");
		outc.println("//");
		{
			for(Program.DeclType declType: declTypeList) {
				String   name        = declType.name;
				String   programName = program.info.getProgramVersion();

				if (declType.type.kind == Type.Kind.RECORD) {
					outc.println("void serialize(Courier::BLOCK& block, const Courier::%s::%s& value) {", programName, name);
					outc.nest();
//					outc.println("block.clear();");

					TypeRecord typeRecord = (TypeRecord)declType.type;
					for(Field field: typeRecord.fields) {
						String fieldName = Util.sanitizeSymbol(field.name);
						outc.println("serialize(block, value.%s);", fieldName);
					}
					
//					outc.println("block.rewind();");
					outc.unnest();
					outc.println("}");
				}
				if (declType.type.kind == Type.Kind.CHOICE) {
					// TODO output function for struct for choice
					// TODO output function for CHOICE_TAG
					// TODO output function for CHOICE_XX

					outc.println("void serialize(Courier::BLOCK& block, const Courier::%s::%s& value) {", programName, name);
					outc.nest();
//					outc.println("block.clear();");

					TypeChoice typeChoice = (TypeChoice)declType.type;
					if (typeChoice instanceof TypeChoice.Anon) {
						TypeChoice.Anon typeAnon = (TypeChoice.Anon)typeChoice;
						for(Candidate<Correspondence> candidate: typeAnon.candidates) {
							for(Correspondence correspondence: candidate.designators) {
								// TODO
							}
						}
					} else if (typeChoice instanceof TypeChoice.Typed) {
						TypeChoice.Typed typeTyped = (TypeChoice.Typed)typeChoice;
						for(Candidate<String> candidate: typeTyped.candidates) {
							for(String choiceName: candidate.designators) {
								// TODO
							}
						}
					}
					
//					outc.println("block.rewind();");
					outc.unnest();
					outc.println("}");
				}
				if (declType.type.kind == Type.Kind.ENUM) {
					outc.println("void serialize(Courier::BLOCK& block, const Courier::%s::%s& value) {", programName, name);
					outc.nest();
					outc.println("Courier::UNSPECIFIED u = (quint16)value;");
					outc.println("serialize(block, u);");
					outc.unnest();
					outc.println("}");
				}
				
			}
		}

		// TODO output deserialization function definition
		outc.println("");
		outc.println("//");
		outc.println("// Deserialize Function Definition");
		outc.println("//");
		{
			for(Program.DeclType declType: declTypeList) {
				String   name        = declType.name;
				String   programName = program.info.getProgramVersion();

				if (declType.type.kind == Type.Kind.RECORD) {
					outc.println("void deserialize(Courier::BLOCK& block, Courier::%s::%s& value) {", programName, name);
					outc.nest();
//					outc.println("block.clear();");

					TypeRecord typeRecord = (TypeRecord)declType.type;
					for(Field field: typeRecord.fields) {
						String fieldName = Util.sanitizeSymbol(field.name);
						outc.println("deserialize(block, value.%s);", fieldName);
					}
					
//					outc.println("block.rewind();");
					outc.unnest();
					outc.println("}");
				}
				if (declType.type.kind == Type.Kind.CHOICE) {
					// TODO output function for struct for choice
					// TODO output function for CHOICE_TAG
					// TODO output function for CHOICE_XX

					outc.println("void deserialize(Courier::BLOCK& block, Courier::%s::%s& value) {", programName, name);
					outc.nest();
//					outc.println("block.clear();");

					TypeChoice typeChoice = (TypeChoice)declType.type;
					if (typeChoice instanceof TypeChoice.Anon) {
						TypeChoice.Anon typeAnon = (TypeChoice.Anon)typeChoice;
						for(Candidate<Correspondence> candidate: typeAnon.candidates) {
							for(Correspondence correspondence: candidate.designators) {
								// TODO
							}
						}
					} else if (typeChoice instanceof TypeChoice.Typed) {
						TypeChoice.Typed typeTyped = (TypeChoice.Typed)typeChoice;
						for(Candidate<String> candidate: typeTyped.candidates) {
							for(String choiceName: candidate.designators) {
								// TODO
							}
						}
					}
					
//					outc.println("block.rewind();");
					outc.unnest();
					outc.println("}");
				}
				if (declType.type.kind == Type.Kind.ENUM) {
					outc.println("void deserialize(Courier::BLOCK& block, Courier::%s::%s& value) {", programName, name);
					outc.nest();
					outc.println("Courier::UNSPECIFIED u = (quint16)value;");
					outc.println("deserialize(block, u);");
					outc.unnest();
					outc.println("}");
				}
				
			}
		}
	}
/**/
	
	public void genStub() {
		String pathc = String.format("%s%s.cpp", STUB_DIR_PATH, program.info.getProgramVersion());
		String pathh = String.format("%s%s.h",   STUB_DIR_PATH, program.info.getProgramVersion());
		logger.info(String.format("pathc = %s", pathc));
		logger.info(String.format("pathh = %s", pathh));
		
		String namePrefix = "Courier";
		
		try (
				Printer outc = new Printer(new PrintWriter(pathc));
				Printer outh = new Printer(new PrintWriter(pathh));) {
			genOpening(outh, outc);
			
			// generate type declaration (exclude error and procedure)
			genTypeDecl(outh, outc, namePrefix);
			
			// generate constant declaration
			
			genClosing(outh, outc);
			
			// generate serialize/deserialize for Record
			// genSerializeFunc(outh, outc);

			// generate toString for enum
			genEnumFunc(outh, outc, namePrefix);
			
		} catch (FileNotFoundException e) {
			throw new CompilerException("FileNotFoundException", e);
		}
	}
}
