package mesa.courier.compiler;

import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

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
	
	static List<String> layoutStringString(List<String> leftList, List<String> rightList) {
		if (leftList.size() != rightList.size()) {
			throw new CompilerException(String.format("Unexpected size  leftList = %d  rightList = %d", leftList.size(), rightList.size()));
		}
		int size = leftList.size();
		
		int leftMaxLength = 0;
		for(String value: leftList) {
			int length = value.trim().length();
			if (leftMaxLength < length) leftMaxLength = length;
		}
		leftMaxLength  = -leftMaxLength;
		String format = String.format("%%%ds %%s", leftMaxLength);
		List<String> ret = new ArrayList<>();
		for(int i = 0; i < size; i++) {
			ret.add(String.format(format, leftList.get(i).trim(), rightList.get(i).trim()));
		}
		return ret;
	}
	static List<String> layoutEnumElement(List<String> leftList, List<Integer> rightList) {
		if (leftList.size() != rightList.size()) {
			throw new CompilerException(String.format("Unexpected size  leftList = %d  rightList = %d", leftList.size(), rightList.size()));
		}
		int size = leftList.size();
		
		int leftMaxLength = 0;
		for(String value: leftList) {
			int length = value.trim().length();
			if (leftMaxLength < length) leftMaxLength = length;
		}
		
		int rightMaxLength = 0;
		for(Integer value: rightList) {
			String string = String.format("%d", value);
			int length = string.length();
			if (rightMaxLength < length) rightMaxLength = length;
		}
		
		String format = String.format("%%%ds = %%%ds,", -leftMaxLength, rightMaxLength);
		List<String> ret = new ArrayList<>();
		for(int i = 0; i < size; i++) {
			ret.add(String.format(format, leftList.get(i).trim(), rightList.get(i).intValue()));
		}
		return ret;
	}


	private static class IndentPrintWriter implements AutoCloseable {
		private static final String INDENT = "    ";

		private final PrintWriter out;
		private int level = 0;

		public IndentPrintWriter(PrintWriter out) {
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
		public PrintWriter indent() {
			for(int i = 0; i < level; i++) out.print(INDENT);
			return out;
		}
	}
	
	static String pad(int width, String string) {
		String format = String.format("%%%ds", width);
		return String.format(format, string);
	}

	private final Program program;
	public Compiler(Program program) {
		this.program = program;
	}

	public void genOpening(IndentPrintWriter outh, IndentPrintWriter outc) {
		// for outh
		// write opening lines
		outh.indent().format("#ifndef STUB_%s_H__", program.info.getProgramVersion()).println();
		outh.indent().format("#define STUB_%s_H__", program.info.getProgramVersion()).println();
		outh.indent().println();

		// include courier header
		outh.indent().println("#include \"../courier/Courier.h\"");			
		// include depend module
		for(Program.Info info: program.depends) {
			outh.indent().format("#include \"../stub/%s.h\"", info.getProgramVersion()).println();
		}

		// output namespace
		outh.indent().println();
		outh.indent().println("namespace Courier {");

		// output main namespace
		outh.indent().format("namespace %s {", program.info.getProgramVersion()).println();
		if (program.info.version != 0) {
			outh.indent().println();
			outh.indent().format("const quint32 PROGRAM_NUMBER = %d;", program.info.program).println();
			outh.indent().format("const quint32 VERSION_NUMBER = %d;", program.info.version).println();
		}

		
		// for outc
		outc.indent().format("#include \"../../util/Util.h\"").println();
		outc.indent().format("static log4cpp::Category& logger = Logger::getLogger(\"stub/%s\");", program.info.getProgramVersion()).println();
		outc.indent().println();
		outc.indent().format("#include \"../stub/%s.h\"", program.info.getProgramVersion()).println();
		outc.indent().println();
	}
	public void genClosing(IndentPrintWriter outh, IndentPrintWriter outc) {
		// for outh
		outh.indent().println();
		outh.indent().println("}"); // for namespace for progaram
		outh.indent().println("}"); // for namespace Courier
		
		// for outc
		// nothing for now
	}
	
	String toTypeString(Type type) {
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
	
	public void genTypeDecl(IndentPrintWriter outh, IndentPrintWriter outc) {
		outh.indent().println("");
		outh.indent().println("//");
		outh.indent().println("// Type Declaration");
		outh.indent().println("//");

		for(Program.DeclType declType: program.typeList) {
			outh.indent().println();

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
				outh.indent().format("using %s = %s;", name, type).println();
				break;
			// constructed
			case ENUM:
				genTypeDeclEnum(outh, outc, (TypeEnum)type, name);
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
				genTypeDeclRecord(outh, outc, (TypeRecord)type, name);
				break;
			case CHOICE:
				genTypeDeclChoice(outh, outc, (TypeChoice)type, name);
				break;
			case PROCEDURE:
			case ERROR:
				outh.indent().format("// FIXME TypeDecl %s %s", name, type.toString()).println();
				break;
			// reference
			case REFERENCE:
				outh.indent().format("using %s = %s;", name, toTypeString(type)).println();
				break;
			default:
				throw new CompilerException(String.format("Unexpected type %s", type.toString()));
			}
		}
	}
	
	public void genTypeDeclRecord(IndentPrintWriter outh, IndentPrintWriter outc, TypeRecord type, String name) {
		List<String> leftList  = new ArrayList<>();
		List<String> rightList = new ArrayList<>();
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
			
			leftList.add(fieldType);
			rightList.add(String.format("%s;", Util.sanitizeSymbol(fieldName)));
		}

		outh.indent().format("struct %s {", name).println();
		outh.nest();
		for(String line: layoutStringString(leftList, rightList)) {
			outh.indent().println(line);
		}
		outh.unnest();
		outh.indent().format("};").println();
	}
	
	public void genTypeDeclEnum(IndentPrintWriter outh, IndentPrintWriter outc, TypeEnum type, String name) {
		List<String>  leftList  = new ArrayList<>();
		List<Integer> rightList = new ArrayList<>();
		for(Correspondence correspondence: type.elements) {
			leftList.add(Util.sanitizeSymbol(correspondence.id));
			rightList.add((int)correspondence.numericValue);
		}
		
		outh.indent().format("enum class %s : quint16 {", name).println();
		outh.nest();	
		for(String line: layoutEnumElement(leftList, rightList)) {
			outh.indent().println(line);
		}
		outh.unnest();
		outh.indent().format("};").println();
	}
	
	public void genTypeDeclChoice(IndentPrintWriter outh, IndentPrintWriter outc, TypeChoice type, String name) {
		outh.indent().format("// FIXME TypeDecl %s %s", name, type.toString()).println();
		outh.indent().format("struct %s {", name).println();
		outh.nest();
		
		if (type instanceof TypeChoice.Typed) {
			TypeChoice.Typed typed = (TypeChoice.Typed)type;
			
			outh.indent().format("using CHOICE_TAG = %s;", toTypeString(typed.type)).println();
			outh.indent().format("CHOICE_TAG choice_tag;").println();

			for(Candidate<String> candidate: typed.candidates) {
				Type candidateType = candidate.type;
				
				for(String id: candidate.designators) {
//					outh.indent().format("// %s => %s", id, candidate.type.toString()).println();
					String candidateTypeName = String.format("CHOICE_%s", id);
					String candidateVarName  = String.format("choice_%s", id);
					if (candidateType.kind == Type.Kind.RECORD) {
						TypeRecord typeRecord = (TypeRecord)candidateType;
						if (typeRecord.fields.size() == 0) {
							//
						} else {
							outh.indent().println();
							genTypeDeclRecord(outh, outc, typeRecord, candidateTypeName);
							outh.indent().format("%s %s;", candidateTypeName, candidateVarName).println();
						}
					} else {
						throw new CompilerException(String.format("Unexpected type %s", type.toString()));
					}
				}
			}
		
		} else if (type instanceof TypeChoice.Anon) {
			TypeChoice.Anon anon = (TypeChoice.Anon)type;
			outh.indent().format("enum class CHOICE_TAG {").println();
			outh.nest();
			for(Candidate<Correspondence> candidate: anon.candidates) {
				for(Correspondence correspondence: candidate.designators) {
					outh.indent().format("%s = %s,", correspondence.id, correspondence.numericValue).println();
				}
//				outh.indent().format("// %s", candidate.toString()).println();
			}
			outh.unnest();
			outh.indent().format("};").println();
			outh.indent().format("CHOICE_TAG choice_tag;").println();
			
			for(Candidate<Correspondence> candidate: anon.candidates) {
				Type candidateType = candidate.type;
				
				for(Correspondence correspondence: candidate.designators) {
//					outh.indent().format("// %s => %s", correspondence.id, candidate.type.toString()).println();
					String candidateTypeName = String.format("CHOICE_%s", correspondence.id);
					String candidateVarName  = String.format("choice_%s", correspondence.id);
					if (candidateType.kind == Type.Kind.RECORD) {
						TypeRecord typeRecord = (TypeRecord)candidateType;
						if (typeRecord.fields.size() == 0) {
							//
						} else {
							outh.indent().println();
							genTypeDeclRecord(outh, outc, typeRecord, candidateTypeName);
							outh.indent().format("%s %s;", candidateTypeName, candidateVarName).println();
						}
					} else {
						throw new CompilerException(String.format("Unexpected type %s", type.toString()));
					}
				}
			}

		} else {
			throw new CompilerException(String.format("Unexpected type %s", type.toString()));
		}
		
		outh.unnest();
		outh.indent().format("};").println();
	}
	
	void genEnumFunc(IndentPrintWriter outh, IndentPrintWriter outc) {
		List<Program.DeclType> declTypeList = new ArrayList<>();
		for(Program.DeclType declType: program.typeList) {
			if (declType.type.kind == Type.Kind.ENUM) {
				declTypeList.add(declType);
			};
		}
		if (declTypeList.isEmpty()) return;
		
		{
			List<String> leftList  = new ArrayList<>();
			List<String> rightList = new ArrayList<>();
			
			for(Program.DeclType declType: declTypeList) {				
				String   name        = declType.name;
				String   programName = program.info.getProgramVersion();
				
				leftList.add(String.format("QString toString(const Courier::%s::%s", programName, name));
				rightList.add("value);");
			}
			
			outh.indent().println("");
			outh.indent().println("//");
			outh.indent().println("// Enum toString Declaration");
			outh.indent().println("//");
			for(String line: layoutStringString(leftList, rightList)) {
				outh.indent().println(line);
			}
		}
		
		outc.indent().println("");
		outc.indent().println("//");
		outc.indent().println("// Enum Function Definition");
		outc.indent().println("//");
		for(Program.DeclType declType: declTypeList) {			
			TypeEnum type = (TypeEnum)declType.type;
			String   name        = declType.name;
			String   programName = program.info.getProgramVersion();
			
			List<String> leftList  = new ArrayList<>();
			List<String> rightList = new ArrayList<>();

			for(Correspondence correspondence: type.elements) {
				String enumName   = correspondence.id;
				leftList. add(String.format("{Courier::%s::%s::%s,", programName, name, Util.sanitizeSymbol(enumName)));
				rightList.add(String.format("\"%s\"},", enumName));
//				outc.indent().format("{Courier::%s::%s::%s,  \"%s\"},", programName, name, Util.sanitizeSymbol(enumName), enumName).println();
			}

			outc.indent().println();
			outc.indent().format("QString toString(const Courier::%s::%s value) {", programName, name).println();
			outc.nest();
			outc.indent().format("static QMap<Courier::%s::%s, QString> map = {", programName, name).println();
			outc.nest();
			
			for(String line: layoutStringString(leftList, rightList)) {
				outc.indent().println(line);
			}
			
			outc.unnest();
			outc.indent().format("};").println();
			outc.indent().println();
			outc.indent().format("if (map.contains(value)) {").println();
			outc.nest();
			outc.indent().format("return map[value];").println();
			outc.unnest();
			outc.indent().format("} else {").println();
			outc.nest();
			outc.indent().format("return QString(\"%%1\").arg((quint16)value);").println();
			outc.unnest();
			outc.indent().format("}").println();
			outc.unnest();
			outc.indent().format("}").println();
		}
	}

	void genSerializeFunc(IndentPrintWriter outh, IndentPrintWriter outc) {
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
		
		outh.indent().println("");
		outh.indent().println("//");
		outh.indent().println("// Serialize Function Declaration");
		outh.indent().println("//");
		{
			List<String> leftList  = new ArrayList<>();
			List<String> rightList = new ArrayList<>();
			
			for(Program.DeclType declType: declTypeList) {				
				String   name        = declType.name;
				String   programName = program.info.getProgramVersion();
				
				// void serialize(Courier::BLOCK& block, const Courier::BYTE&          value);
				leftList.add(String.format("void serialize(Courier::BLOCK& block, const Courier::%s::%s&", programName, name));
				rightList.add("value);");
			}
			
			for(String line: layoutStringString(leftList, rightList)) {
				outh.indent().println(line);
			}
		}
		
		outh.indent().println("");
		outh.indent().println("//");
		outh.indent().println("// Deserizlize Function Declaration");
		outh.indent().println("//");
		{
			List<String> leftList  = new ArrayList<>();
			List<String> rightList = new ArrayList<>();
			
			for(Program.DeclType declType: declTypeList) {				
				String   name        = declType.name;
				String   programName = program.info.getProgramVersion();
				
				// void deserialize(Courier::BLOCK& block, Courier::BYTE&          value);
				leftList.add(String.format("void deserialize(Courier::BLOCK& block, Courier::%s::%s&", programName, name));
				rightList.add("value);");
			}
			
			for(String line: layoutStringString(leftList, rightList)) {
				outh.indent().println(line);
			}
		}

		// TODO output serialization function definition
		outc.indent().println("");
		outc.indent().println("//");
		outc.indent().println("// Serialize Function Definition");
		outc.indent().println("//");
		{
			for(Program.DeclType declType: declTypeList) {
				String   name        = declType.name;
				String   programName = program.info.getProgramVersion();

				if (declType.type.kind == Type.Kind.RECORD) {
					outc.indent().format("void serialize(Courier::BLOCK& block, const Courier::%s::%s& value) {", programName, name).println();
					outc.nest();
//					outc.indent().format("block.clear();").println();

					TypeRecord typeRecord = (TypeRecord)declType.type;
					for(Field field: typeRecord.fields) {
						String fieldName = Util.sanitizeSymbol(field.name);
						outc.indent().format("serialize(block, value.%s);", fieldName).println();
					}
					
//					outc.indent().format("block.rewind();").println();
					outc.unnest();
					outc.indent().format("}").println();
				}
				if (declType.type.kind == Type.Kind.CHOICE) {
					outc.indent().format("void serialize(Courier::BLOCK& block, const Courier::%s::%s& value) {", programName, name).println();
					outc.nest();
//					outc.indent().format("block.clear();").println();

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
					
//					outc.indent().format("block.rewind();").println();
					outc.unnest();
					outc.indent().format("}").println();
				}
				if (declType.type.kind == Type.Kind.ENUM) {
					outc.indent().format("void serialize(Courier::BLOCK& block, const Courier::%s::%s& value) {", programName, name).println();
					outc.nest();
					outc.indent().format("Courier::UNSPECIFIED u = (quint16)value;").println();
					outc.indent().format("serialize(block, u);").println();
					outc.unnest();
					outc.indent().format("}").println();
				}
				
			}
		}

		// TODO output deserialization function definition

	}
	public void genStub() {
		String pathc = String.format("%s%s.cpp", STUB_DIR_PATH, program.info.getProgramVersion());
		String pathh = String.format("%s%s.h",   STUB_DIR_PATH, program.info.getProgramVersion());
		logger.info(String.format("pathc = %s", pathc));
		logger.info(String.format("pathh = %s", pathh));
		
		try (
				IndentPrintWriter outc = new IndentPrintWriter(new PrintWriter(pathc));
				IndentPrintWriter outh = new IndentPrintWriter(new PrintWriter(pathh));) {
			genOpening(outh, outc);
			
			// generate type declaration (exclude error and procedure)
			genTypeDecl(outh, outc);
			
			// generate constant declaration
			
			genClosing(outh, outc);
			
			// generate serialize/deserialize for Record
			genSerializeFunc(outh, outc);

			// generate toString for enum
			genEnumFunc(outh, outc);
			
		} catch (FileNotFoundException e) {
			throw new CompilerException("FileNotFoundException", e);
		}
	}
}
