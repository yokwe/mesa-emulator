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
import mesa.courier.program.TypeArray;
import mesa.courier.program.TypeBlock;
import mesa.courier.program.TypeChoice;
import mesa.courier.program.TypeChoice.Candidate;
import mesa.courier.program.TypeEnum;
import mesa.courier.program.TypeRecord;
import mesa.courier.program.TypeReference;
import mesa.courier.program.TypeSequence;
import mesa.courier.program.Util;

public class CompilerRaw {
	protected static final Logger logger = LoggerFactory.getLogger(CompilerRaw.class);

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


	public static class IndentationPrinter implements AutoCloseable {
		public static final String INDENT = "    ";

		private final PrintWriter out;
		private int level = 0;

		public IndentationPrinter(PrintWriter out) {
			this.out = out;
		}
		public void close() {
			out.close();
		}

		public IndentationPrinter format(String format, Object... args) {
			return line(String.format(format, args));
		}
		public IndentationPrinter line(String message) {
			if (message.length() == 0) {
				out.println();
			} else if (message.equals("public:") || message.equals("private:")) {
				for(int i = 1; i < level; i++) out.print(INDENT);
				out.println(message);
			} else {
				if (message.startsWith("}")) {
					level--;
					if (level < 0) throw new CompilerException("level < 0");
				}
				for(int i = 0; i < level; i++) out.print(INDENT);
				out.println(message);
				if (message.endsWith("{")) {
					level++;
				}
			}
			return this;
		}
		public IndentationPrinter line(String arg0, String... args) {
			line(arg0);
			for(String message: args) {
				line(message);
			}
			return this;
		}
		public IndentationPrinter line() {
			out.println();
			return this;
		}
	}
	
	private final Program program;
	public CompilerRaw(Program program) {
		this.program = program;
	}

	private void genOpening(IndentationPrinter outh, IndentationPrinter outc) {
		// for outh
		// write opening lines
		outh.format("#ifndef STUB_%s_H__", program.info.getProgramVersion());
		outh.format("#define STUB_%s_H__", program.info.getProgramVersion());
		outh.line();

		// include courier header
		outh.line("#include \"../courier/CourierRaw.h\"");			
		// include depend module
		for(Program.Info info: program.depends) {
			outh.format("#include \"../stub/%s.h\"", info.getProgramVersion());
		}

		// output namespace
		outh.line();
		outh.line("namespace Courier {");

		// output main namespace
		outh.format("namespace %s {", program.info.getProgramVersion());
		if (program.info.version != 0) {
			outh.line();
			outh.format("const quint32 PROGRAM_NUMBER = %d;", program.info.program);
			outh.format("const quint32 VERSION_NUMBER = %d;", program.info.version);
		}

		// for outc
		outc.line("#include \"../../util/Util.h\"");
		outc.format("static log4cpp::Category& logger = Logger::getLogger(\"stub/%s\");", program.info.getProgramVersion());
		outc.line();
		outc.format("#include \"../stub/%s.h\"", program.info.getProgramVersion());
		outc.line();
	}
	private void genClosing(IndentationPrinter outh, IndentationPrinter outc) {
		// for outh
		outh.line();
		outh.line("}"); // for namespace for progaram
		outh.line("}"); // for namespace Courier
		
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
			return program.getLocalRefName(typeReference);
		}
		default:
			break;
		}
		throw new CompilerException(String.format("Unexpected type %s", type.toString()));
	}
	
	private void genTypeDecl(IndentationPrinter outh, IndentationPrinter outc, String namePrefix) {
		outh.line(
				"",
				"//",
				"// Type Declaration",
				"//"
				);

		for(Program.DeclType declType: program.typeList) {
			outh.line();

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
				outh.format("using %s = %s;", name, type);
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
				outh.format("// FIXME TypeDecl %s %s", name, type.toString());
				break;
			// reference
			case REFERENCE:
				outh.format("using %s = %s;", name, toTypeString(type));
				break;
			default:
				throw new CompilerException(String.format("Unexpected type %s", type.toString()));
			}
		}
	}
	
	private void genTypeDeclRecord(IndentationPrinter outh, IndentationPrinter outc, TypeRecord type, String name, String namePrefix) {
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

		outh.format("struct %s {", name);
		for(String line: layoutStringString(c1, c2)) {
			outh.line(line);
		}
		
		outh.line(
			"",
			"// Standard methods",
			"void serialize  (BLOCK& block) const;",
			"void deserialize(BLOCK& block);",
			"QString toString();"
			);

		outh.line("};");
	}
	
	private void genTypeDeclEnum(IndentationPrinter outh, IndentationPrinter outc, TypeEnum type, String name, String namePrefix) {
		List<String>  c1 = new ArrayList<>();
		List<Integer> c2 = new ArrayList<>();
		for(Correspondence correspondence: type.elements) {
			c1.add(Util.sanitizeSymbol(correspondence.id));
			c2.add((int)correspondence.numericValue);
		}
		
		outh.format("enum class %s : quint16 {", name);
		for(String line: layoutEnumElement(c1, c2)) {
			outh.line(line);
		}
		outh.line("};");
	}
	
	private void genTypeDeclChoice(IndentationPrinter outh, IndentationPrinter outc, TypeChoice type, String name, String namePrefix) {
		outh.format("// FIXME TypeDecl %s %s", name, type.toString());
		outh.format("struct %s {", name);
		
		if (type instanceof TypeChoice.Typed) {
			TypeChoice.Typed typed = (TypeChoice.Typed)type;
			
			outh.format("using CHOICE_TAG = %s;", toTypeString(typed.type));
			
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
							outh.format("struct %s {};", structName);
						} else {
							genTypeDeclRecord(outh, outc, typeRecord, structName, String.format("%s::%s", namePrefix, name));
						}
					} else {
						throw new CompilerException(String.format("Unexpected type %s", type.toString()));
					}
				}
			}
			
			outh.line();
			outh.line("CHOICE_TAG choiceTag;");
			
			for(String choiceName: choiceNameList) {
				int structNumber = choiceMap.get(choiceName);
				outh.format("CHOICE_%02d& %s() {", structNumber, Util.sanitizeSymbol(choiceName));
				outh.format("return choice_%02d;", structNumber);
				outh.line("}");
			}
			
			outh.line(
					"",
					"void serialize  (BLOCK& block) const;",
					"void deserialize(BLOCK& block);",
					"QString toString();"
					);

			outh.line("private:");
			for(int i = 1; i <= maxStructNumber; i++) {
				outh.format("CHOICE_%02d  choice_%02d;", i, i);
			}
		} else if (type instanceof TypeChoice.Anon) {
			TypeChoice.Anon anon = (TypeChoice.Anon)type;

			List<String>         choiceNameList  = new ArrayList<>(); // choice name list in appearance order
			Map<String, Integer> choiceMap       = new TreeMap<>();   // choice name => struct number
			int                  maxStructNumber = 0;

			outh.line("enum class CHOICE_TAG : quint16 {");
			for(Candidate<Correspondence> candidate: anon.candidates) {
				for(Correspondence correspondence: candidate.designators) {
					outh.format("%s = %s,", correspondence.id, correspondence.numericValue);
				}
			}
			outh.line("};");
			
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
						outh.format("struct %s {};", structName);
					} else {
						genTypeDeclRecord(outh, outc, typeRecord, structName, String.format("%s::%s", namePrefix, name));

					}
				} else {
					throw new CompilerException(String.format("Unexpected type %s", type.toString()));
				}
			}
			outh.line();

			for(String choiceName: choiceNameList) {
				int structNumber = choiceMap.get(choiceName);
				outh.format("CHOICE_%02d& %s() {", structNumber, Util.sanitizeSymbol(choiceName));
				outh.format("return choice_%02d;", structNumber);
				outh.line("}");
			}
			outh.line();

			outh.line();
			outh.line(
					"void serialize  (BLOCK& block) const;",
					"void deserialize(BLOCK& block);",
					"QString toString();"
					);

			outh.line("private:");
			for(int i = 1; i <= maxStructNumber; i++) {
				outh.format("CHOICE_%02d  choice_%02d;", i, i);
			}

		} else {
			throw new CompilerException(String.format("Unexpected type %s", type.toString()));
		}
		
		outh.line("};");
	}
	
	private void genEnumFunc(IndentationPrinter outh, IndentationPrinter outc, String namePrefix) {
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
			
			outh.line(
					"",
					"//",
					"// Enum toString Declaration",
					"//"
					);
			for(String line: layoutStringString(c1, c2)) {
				outh.line(line);
			}
		}
		
		outc.line(
				"",
				"//",
				"// Enum Function Definition",
				"//"
				);

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

			outc.line();
			outc.format("QString toString(const %s::%s::%s value) {", namePrefix, programName, name);
			outc.format("static QMap<%s::%s::%s, QString> map = {", namePrefix, programName, name);
			
			for(String line: layoutStringString(c1, c2)) {
				outc.line(line);
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
			
			outc.line(
					"};",
					"",
					"if (map.contains(value)) {",
						"return map[value];",
					"} else {",
						"return QString(\"%1\").arg((quint16)value);",
					"}",
				"}"
					);

		}
	}

/**/
	private void genSerializeFunc(IndentationPrinter outh, IndentationPrinter outc) {
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
		
		outh.line("");
		outh.line("//");
		outh.line("// Serialize Function Declaration");
		outh.line("//");
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
				outh.line(line);
			}
		}
		
		outh.line("");
		outh.line("//");
		outh.line("// Deserizlize Function Declaration");
		outh.line("//");
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
				outh.line(line);
			}
		}

		// TODO output serialization function definition
		outc.line("");
		outc.line("//");
		outc.line("// Serialize Function Definition");
		outc.line("//");
		{
			for(Program.DeclType declType: declTypeList) {
				String   name        = declType.name;
				String   programName = program.info.getProgramVersion();

				if (declType.type.kind == Type.Kind.RECORD) {
					outc.format("void serialize(Courier::BLOCK& block, const Courier::%s::%s& value) {", programName, name);
//					outc.println("block.clear();");

					TypeRecord typeRecord = (TypeRecord)declType.type;
					for(Field field: typeRecord.fields) {
						String fieldName = Util.sanitizeSymbol(field.name);
						outc.format("serialize(block, value.%s);", fieldName);
					}
					
//					outc.println("block.rewind();");
					outc.line("}");
				}
				if (declType.type.kind == Type.Kind.CHOICE) {
					// TODO output function for struct for choice
					// TODO output function for CHOICE_TAG
					// TODO output function for CHOICE_XX

					outc.format("void serialize(Courier::BLOCK& block, const Courier::%s::%s& value) {", programName, name);
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
					outc.line("}");
				}
				if (declType.type.kind == Type.Kind.ENUM) {
					outc.format("void serialize(Courier::BLOCK& block, const Courier::%s::%s& value) {", programName, name);
					outc.line("Courier::UNSPECIFIED u = (quint16)value;");
					outc.line("serialize(block, u);");
					outc.line("}");
				}
				
			}
		}

		// TODO output deserialization function definition
		outc.line("");
		outc.line("//");
		outc.line("// Deserialize Function Definition");
		outc.line("//");
		{
			for(Program.DeclType declType: declTypeList) {
				String   name        = declType.name;
				String   programName = program.info.getProgramVersion();

				if (declType.type.kind == Type.Kind.RECORD) {
					outc.format("void deserialize(Courier::BLOCK& block, Courier::%s::%s& value) {", programName, name);
//					outc.println("block.clear();");

					TypeRecord typeRecord = (TypeRecord)declType.type;
					for(Field field: typeRecord.fields) {
						String fieldName = Util.sanitizeSymbol(field.name);
						outc.format("deserialize(block, value.%s);", fieldName);
					}
					
//					outc.println("block.rewind();");
					outc.line("}");
				}
				if (declType.type.kind == Type.Kind.CHOICE) {
					// TODO output function for struct for choice
					// TODO output function for CHOICE_TAG
					// TODO output function for CHOICE_XX

					outc.format("void deserialize(Courier::BLOCK& block, Courier::%s::%s& value) {", programName, name);
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
					outc.line("}");
				}
				if (declType.type.kind == Type.Kind.ENUM) {
					outc.format("void deserialize(Courier::BLOCK& block, Courier::%s::%s& value) {", programName, name);
					outc.line(
							"Courier::UNSPECIFIED u = (quint16)value;",
							"deserialize(block, u);",
							"}");
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
				IndentationPrinter outc = new IndentationPrinter(new PrintWriter(pathc));
				IndentationPrinter outh = new IndentationPrinter(new PrintWriter(pathh));) {
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
