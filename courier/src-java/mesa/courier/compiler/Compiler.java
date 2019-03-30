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

public class Compiler {
	protected static final Logger logger = LoggerFactory.getLogger(Compiler.class);

	public static String STUB_DIR_PATH = "src/stub/";
	
	private final Program program;
	public Compiler(Program program) {
		this.program = program;
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
	
	private void genTypeDecl(LinePrinter outh, LinePrinter outc, String namePrefix) {
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

	private void logField(LinePrinter out, Type type, String name) {
		Type concreteType = type.getConcreteType();

		out.format("// %s", name);
		out.format("//   %s", type);
		if (type.isReference()) {
			out.format("//   %s", concreteType);
		}
						
		switch(concreteType.kind) {
		case ARRAY:
		{
			TypeArray typeArray = (TypeArray)concreteType;
			out.format("//     %s", typeArray.type);
			if (typeArray.type.isReference()) {
				Type elementType = typeArray.type.getConcreteType();
				out.format("//       %s", elementType);
			}
		}
			break;
		case SEQUENCE:
		{
			TypeSequence typeSequence = (TypeSequence)concreteType;
			out.format("//     %s", typeSequence.type);
			if (typeSequence.type.isReference()) {
				Type elementType = typeSequence.type.getConcreteType();
				out.format("//       %s", elementType);
			}
		}
			break;
		default:
			break;
		}
	}

	private void genTypeDeclRecord(LinePrinter outh, LinePrinter outc, TypeRecord type, String name, String namePrefix) {
		genRecordDecl       (outh, outc, type, name, namePrefix);
		genRecordSerialize  (outh, outc, type, name, namePrefix);
		genRecordDeserialize(outh, outc, type, name, namePrefix);
	}
	private void genRecordDecl(LinePrinter outh, LinePrinter outc, TypeRecord type, String name, String namePrefix) {
		// Output declaration of class
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
		for(String line: ColumnLayout.layoutStringString(c1, c2)) {
			outh.line(line);
		}
		
		outh.line(
			"",
			"// Standard methods",
			"void serialize  (BLOCK& block) const;",
			"void deserialize(BLOCK& block);"
			);

		outh.line("};");
	}
	private void genRecordSerialize(LinePrinter outh, LinePrinter outc, TypeRecord type, String name, String namePrefix) {
		// Output definition of serialize()
		outc.format("void %s::%s::serialize(BLOCK& block) const {", namePrefix, name);
		
		for(Field field: type.fields) {
			Type concreteType = field.type.getConcreteType();
			logField(outc, field.type, field.name);
			outc.line(TypeUtil.genSerialize(concreteType, field.name));
		}
		outc.line("}");
	}
	private void genRecordDeserialize(LinePrinter outh, LinePrinter outc, TypeRecord type, String name, String namePrefix) {
		// Output definition of deserialize()
		outc.format("void %s::%s::deserialize(BLOCK& block) {", namePrefix, name);
		
		for(Field field: type.fields) {
			Type concreteType = field.type.getConcreteType();
			logField(outc, field.type, field.name);
			outc.line(TypeUtil.genDeserialize(concreteType, field.name));
		}
		outc.line("}");
	}
	
	private void genTypeDeclEnum(LinePrinter outh, LinePrinter outc, TypeEnum type, String name, String namePrefix) {
		List<String>  c1 = new ArrayList<>();
		List<Integer> c2 = new ArrayList<>();
		for(Correspondence correspondence: type.elements) {
			c1.add(Util.sanitizeSymbol(correspondence.id));
			c2.add((int)correspondence.numericValue);
		}
		
		outh.format("enum class %s : quint16 {", name);
		for(String line: ColumnLayout.layoutEnumElement(c1, c2)) {
			outh.line(line);
		}
		outh.line("};");
	}
	
	private void genTypeDeclChoice(LinePrinter outh, LinePrinter outc, TypeChoice type, String name, String namePrefix) {
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
					"void deserialize(BLOCK& block);"
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
					"void deserialize(BLOCK& block);"
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
	
	
	class EnumInfo {
		final TypeEnum typeEnum;
		final String   name;
		final String   namePrefix;
		
		EnumInfo(TypeEnum typeEnum, String name, String namePrefix) {
			this.typeEnum   = typeEnum;
			this.name       = name;
			this.namePrefix = namePrefix;
		}
	}
	
	private void genEnumToString(LinePrinter outh, LinePrinter outc, String namePrefix) {
		List<EnumInfo> enumInfoList = new ArrayList<>();
		for(Program.DeclType declType: program.typeList) {
			if (declType.type.kind == Type.Kind.ENUM) {
				TypeEnum typeEnum = (TypeEnum)declType.type;
				enumInfoList.add(new EnumInfo(typeEnum, declType.name, namePrefix));
			};
		}
		if (enumInfoList.isEmpty()) return;
		
		// Declaration
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(EnumInfo enumInfo: enumInfoList) {				
				c1.add(String.format("QString toString(const %s::%s", enumInfo.namePrefix, enumInfo.name));
				c2.add("value);");
			}
			
			outh.line(
					"",
					"//",
					"// Enum toString Declaration",
					"//"
					);
			for(String line: ColumnLayout.layoutStringString(c1, c2)) {
				outh.line(line);
			}
		}
		
		// Definition
		{
			outc.line(
					"",
					"//",
					"// Enum Function Definition",
					"//"
					);

			for(EnumInfo enumInfo: enumInfoList) {							
				List<String> c1 = new ArrayList<>();
				List<String> c2 = new ArrayList<>();

				for(Correspondence correspondence: enumInfo.typeEnum.elements) {
					String enumName = correspondence.id;
					c1.add(String.format("{%s::%s::%s,", enumInfo.namePrefix, enumInfo.name, Util.sanitizeSymbol(enumName)));
					c2.add(String.format("\"%s\"},", enumName));
				}

				outc.line();
				outc.format("QString %s::toString(const %s::%s value) {", "Courier", enumInfo.namePrefix, enumInfo.name);
				outc.format("static QMap<%s::%s, QString> map = {", enumInfo.namePrefix, enumInfo.name);
				
				for(String line: ColumnLayout.layoutStringString(c1, c2)) {
					outc.line(line);
				}
				
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
	}
	
	class RecordInfo {
		final TypeRecord typeRecord;
		final String     name;
		final String     namePrefix;
		
		RecordInfo(TypeRecord typeRecord, String name, String namePrefix) {
			this.typeRecord = typeRecord;
			this.name       = name;
			this.namePrefix = namePrefix;
		}
	}
	
	private void genRecordToString(LinePrinter outh, LinePrinter outc, String namePrefix) {
		List<RecordInfo> recordInfoList = new ArrayList<>();
		for(Program.DeclType declType: program.typeList) {
			String name = declType.name;
			if (declType.type.kind == Type.Kind.RECORD) {
				TypeRecord typeRecord = (TypeRecord)declType.type;
				recordInfoList.add(new RecordInfo(typeRecord, name, namePrefix));
			} else if (declType.type.kind == Type.Kind.CHOICE) {
				TypeChoice typeChoice = (TypeChoice)declType.type;
				if (typeChoice instanceof TypeChoice.Typed) {
					TypeChoice.Typed typed = (TypeChoice.Typed)typeChoice;
					int              maxStructNumber = 0;
					for(Candidate<String> candidate: typed.candidates) {
						maxStructNumber++;
						Type candidateType = candidate.type;
						if (candidateType.kind == Type.Kind.RECORD) {
							TypeRecord typeRecord = (TypeRecord)candidateType;
							String structName = String.format("CHOICE_%02d", maxStructNumber);
							recordInfoList.add(new RecordInfo(typeRecord, structName, String.format("%s::%s", namePrefix, name)));
						} else {
							throw new CompilerException(String.format("Unexpected candidateType %s", candidateType.toString()));
						}
					}
				} else if (typeChoice instanceof TypeChoice.Anon) {
					TypeChoice.Anon anon = (TypeChoice.Anon)typeChoice;
					int             maxStructNumber = 0;
					for(Candidate<Correspondence> candidate: anon.candidates) {
						maxStructNumber++;
						Type candidateType = candidate.type;
						if (candidateType.kind == Type.Kind.RECORD) {
							TypeRecord typeRecord = (TypeRecord)candidateType;
							String structName = String.format("CHOICE_%02d", maxStructNumber);
							recordInfoList.add(new RecordInfo(typeRecord, structName, String.format("%s::%s", namePrefix, name)));
						} else {
							throw new CompilerException(String.format("Unexpected candidateType %s", candidateType.toString()));
						}
					}
				} else {
					throw new CompilerException(String.format("Unexpected type %s", typeChoice.getClass().getName()));
				}
			}
		}
		if (recordInfoList.isEmpty()) return;
		
		// Record toSring declaration
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(RecordInfo recordInfo: recordInfoList) {				
				c1.add(String.format("QString toString(const %s::%s", recordInfo.namePrefix, recordInfo.name));
				c2.add("value);");
			}
			
			outh.line(
					"",
					"//",
					"// Record toString Declaration",
					"//"
					);
			for(String line: ColumnLayout.layoutStringString(c1, c2)) {
				outh.line(line);
			}
		}
		
		// Record toString definition
		{
			for(RecordInfo recordInfo: recordInfoList) {
				outc.format("QString %s::toString(const %s::%s value) {", "Courier", recordInfo.namePrefix, recordInfo.name);
				outc.line("QStringList list;");
				
				for(Field field: recordInfo.typeRecord.fields) {
					logField(outc, field.type, field.name);

					outc.line("{");
					outc.format("list << QString(\"[%%1 %%2]\").arg(\"%s\").arg(toString(value.%s));", field.name, field.name);
					outc.line("}");
				}
				
				outc.line("return list.join(\" \");");
				outc.line("}");
			}
		}
	}
	
	public void genStub() {
		String programName = program.info.getProgramVersion();
		String pathc = String.format("%s%s.cpp", STUB_DIR_PATH, programName);
		String pathh = String.format("%s%s.h",   STUB_DIR_PATH, programName);
		logger.info(String.format("pathc = %s", pathc));
		logger.info(String.format("pathh = %s", pathh));
		
		String namespace = "Courier";
		String namePrefix = String.format("%s::%s", namespace, programName);
		
		try (
				LinePrinter outc = new LinePrinter(new PrintWriter(pathc));
				LinePrinter outh = new LinePrinter(new PrintWriter(pathh));) {
			// for outh
			// write opening lines
			outh.format("#ifndef STUB_%s_H__", program.info.getProgramVersion());
			outh.format("#define STUB_%s_H__", program.info.getProgramVersion());
			outh.line();

			// include courier header
			outh.line("#include \"../courier/Courier.h\"");			
			// include depend module
			for(Program.Info info: program.depends) {
				outh.format("#include \"../stub/%s.h\"", info.getProgramVersion());
			}

			// output namespace
			outh.line();
			outh.format("namespace %s {", namespace);

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
			
			// generate type declaration (exclude error and procedure)
			// and generate serialize/deserialize/toString() for record and choice
			genTypeDecl(outh, outc, namePrefix);
			
			// TODO generate constant declaration
			
			// Close program namespace
			outh.line("}");
			
			// generate toString for enum
			genEnumToString(outh, outc, namePrefix);
			
			genRecordToString(outh, outc, namePrefix);
			
			// Close courier namespace
			outh.line("}");
		} catch (FileNotFoundException e) {
			throw new CompilerException("FileNotFoundException", e);
		}
	}
}
