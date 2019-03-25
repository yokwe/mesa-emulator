package mesa.courier.compiler;

import java.io.FileNotFoundException;
import java.io.PrintWriter;

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

public class Compiler {
	protected static final Logger logger = LoggerFactory.getLogger(Compiler.class);

	public static String STUB_DIR_PATH = "src/stub/";

	static final int USING_LEFT_WIDTH  = -22;
	static final int FIELD_TYPE_WIDTH  = -26;
	static final int ENUM_NAME_WIDTH   = -30;
	static final int ENUM_NUMBER_WIDTH = 4;


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
			String name = declType.name;
			
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
				outh.indent().format("using %s = %s;", pad(USING_LEFT_WIDTH, name), toTypeString(type)).println();
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
				outh.indent().format("using %s = %s;", pad(USING_LEFT_WIDTH, name), toTypeString(type)).println();
				break;
			default:
				throw new CompilerException(String.format("Unexpected type %s", type.toString()));
			}
		}
	}
	
	public void genTypeDeclRecord(IndentPrintWriter outh, IndentPrintWriter outc, TypeRecord type, String name) {
		outh.indent().format("struct %s {", name).println();
		outh.nest();
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
			
			outh.indent().format("%s %s;", pad(FIELD_TYPE_WIDTH, fieldType), fieldName).println();
		}
		outh.unnest();
		outh.indent().format("};").println();
	}
	
	public void genTypeDeclEnum(IndentPrintWriter outh, IndentPrintWriter outc, TypeEnum type, String name) {
		outh.indent().format("enum class %s : quint16 {", name).println();
		outh.nest();
		for(Correspondence correspondence: type.elements) {
			String enumName   = correspondence.id;
			String enumNumber = String.format("%d", correspondence.numericValue);
			outh.indent().format("%s = %s,", pad(ENUM_NAME_WIDTH, enumName), pad(ENUM_NUMBER_WIDTH, enumNumber)).println();
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
			
			// generate serialize/deserialize for Record
			// generate toString for enum
			
			
			genClosing(outh, outc);
		} catch (FileNotFoundException e) {
			throw new CompilerException("FileNotFoundException", e);
		}
	}
}
