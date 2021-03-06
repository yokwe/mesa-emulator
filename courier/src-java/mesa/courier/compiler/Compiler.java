package mesa.courier.compiler;

import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.time.LocalDateTime;
import java.time.ZoneId;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import mesa.courier.program.Constant;
import mesa.courier.program.Constant.Component;
import mesa.courier.program.ConstantArray;
import mesa.courier.program.ConstantBoolean;
import mesa.courier.program.ConstantNumber;
import mesa.courier.program.ConstantRecord;
import mesa.courier.program.ConstantReference;
import mesa.courier.program.ConstantString;
import mesa.courier.program.Program;
import mesa.courier.program.Program.Decl;
import mesa.courier.program.Program.DeclConst;
import mesa.courier.program.Program.DeclType;
import mesa.courier.program.Type;
import mesa.courier.program.Type.Correspondence;
import mesa.courier.program.Type.Field;
import mesa.courier.program.Type.Kind;
import mesa.courier.program.TypeArray;
import mesa.courier.program.TypeChoice;
import mesa.courier.program.TypeChoice.Candidate;
import mesa.courier.program.TypeEnum;
import mesa.courier.program.TypeError;
import mesa.courier.program.TypeMachine;
import mesa.courier.program.TypePredefined;
import mesa.courier.program.TypeProcedure;
import mesa.courier.program.TypeRecord;
import mesa.courier.program.TypeReference;
import mesa.courier.program.TypeSequence;
import mesa.courier.program.Util;

public class Compiler {
	protected static final Logger logger = LoggerFactory.getLogger(Compiler.class);

	public static String STUB_DIR_PATH = "src/stub/";
	
	public static class EnumInfo {
		public final TypeEnum typeEnum;
		public final String   name;
		public final String   namePrefix;
		
		EnumInfo(TypeEnum typeEnum, String name, String namePrefix) {
			this.typeEnum   = typeEnum;
			this.name       = name;
			this.namePrefix = namePrefix;
		}
	}
	
	public static class ChoiceInfo {
		public final TypeChoice   typeChoice;
		public final String       name;
		public final String       namePrefix;
		public final List<String> choiceNameList; // choice name list in appearance order

		ChoiceInfo(TypeChoice typeChoice, String name, String namePrefix, List<String> choiceNameList) {
			this.typeChoice     = typeChoice;
			this.name           = name;
			this.namePrefix     = namePrefix;
			this.choiceNameList = choiceNameList;
		}
	}
	
	public static class RecordInfo {
		public final TypeRecord typeRecord;
		public final String     name;
		public final String     namePrefix;
		
		RecordInfo(TypeRecord typeRecord, String name, String namePrefix) {
			this.typeRecord = typeRecord;
			this.name       = name;
			this.namePrefix = namePrefix;
		}
	}
	
	public static class ErrorInfo {
		public final TypeError typeError;
		public final String    name;
		public final String    namePrefix;
		public final long      code;
		
		ErrorInfo(TypeError typeError, String name, String namePrefix, long code) {
			this.typeError  = typeError;
			this.name       = name;
			this.namePrefix = namePrefix;
			this.code       = code;
		}
	}
	
	public static class ProcedureInfo {
		public final TypeProcedure typeProcedure;
		public final String        name;
		public final String        namePrefix;
		public final long          code;

		ProcedureInfo(TypeProcedure typeProcedure, String name, String namePrefix, long code) {
			this.typeProcedure = typeProcedure;
			this.name          = name;
			this.namePrefix    = namePrefix;
			this.code          = code;
		}
	}
	
	public static class SequenceInfo {
		public final TypeSequence typeSequence;
		public final String       name;
		public final String       namePrefix;
		
		SequenceInfo(TypeSequence typeSequence, String name, String namePrefix) {
			this.typeSequence = typeSequence;
			this.name         = name;
			this.namePrefix   = namePrefix;
		}
	}
	
	public static class ArrayInfo {
		public final TypeArray typeArray;
		public final String    name;
		public final String    namePrefix;
		
		ArrayInfo(TypeArray typeArray, String name, String namePrefix) {
			this.typeArray  = typeArray;
			this.name       = name;
			this.namePrefix = namePrefix;
		}
	}
	
	public static class MachineInfo {
		public final TypeMachine typeMachine;
		public final String      name;
		public final String      namePrefix;
		
		MachineInfo(TypeMachine typeMachine, String name, String namePrefix) {
			this.typeMachine = typeMachine;
			this.name        = name;
			this.namePrefix  = namePrefix;
		}
	}

	public static class Context {
		public final Program              program;
		public final List<EnumInfo>       enumInfoList;
		public final List<RecordInfo>     recordInfoList;
		public final List<ChoiceInfo>     choiceInfoList;
		public final List<ErrorInfo>      errorInfoList;
		public final List<ProcedureInfo>  procedureInfoList;
		public final List<SequenceInfo>   sequenceInfoList;
		public final List<ArrayInfo>      arrayInfoList;
		public final List<MachineInfo>    machineInfoList;
		
		Context(Program program) {
			this.program           = program;
			this.enumInfoList      = new ArrayList<>();
			this.recordInfoList    = new ArrayList<>();
			this.choiceInfoList    = new ArrayList<>();
			this.errorInfoList     = new ArrayList<>();
			this.procedureInfoList = new ArrayList<>();
			this.sequenceInfoList  = new ArrayList<>();
			this.arrayInfoList     = new ArrayList<>();
			this.machineInfoList   = new ArrayList<>();
			
			contextMap.put(program.info.getProgramVersion(), this);
		}
	}

	public static Map<String, Context> contextMap = new TreeMap<>();
	
	public  final Context context;
	private final Program program;
	public Compiler(Program program) {
		this.context = new Context(program);
		this.program = program;
	}

	private String toTypeString(Type type) {
		return toTypeString(type, true);
	}
	private String toTypeStringNotLocal(Type type) {
		return toTypeString(type, false);
	}
	private String toTypeString(Type type, boolean useLocalName) {
		switch(type.kind) {
		// predefined
		case BLOCK:
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
			return String.format("ARRAY_N<%s,%d>", toTypeString(typeArray.type, useLocalName), typeArray.size);
		}
		case SEQUENCE:
		{
			TypeSequence typeSequence = (TypeSequence)type;
			if (typeSequence.size == TypeSequence.MAX_SIZE) {
				return String.format("SEQUENCE_N<%s>", toTypeString(typeSequence.type, useLocalName));
			} else {
				return String.format("SEQUENCE_N<%s,%d>", toTypeString(typeSequence.type, useLocalName), typeSequence.size);
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
			return useLocalName ? program.getLocalRefName(typeReference) : typeReference.value.toString();
		}
		default:
			break;
		}
		throw new CompilerException(String.format("Unexpected type %s", type.toString()));
	}

	private void genDeclType(LinePrinter outh, LinePrinter outc, String namePrefix, DeclType declType) {
		genDeclType(outh, outc, namePrefix, declType.type, declType.name);
	}
	private void genDeclType(LinePrinter outh, LinePrinter outc, String namePrefix, Type type, String name) {
		switch(type.kind) {
		// predefined
		case BLOCK:
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
			genDeclTypeEnum(outh, outc, (TypeEnum)type, name, namePrefix);
			break;
		case ARRAY:
			genDeclTypeArray(outh, outc, (TypeArray)type, name, namePrefix);
			break;
		case SEQUENCE:
			genDeclTypeSequence(outh, outc, (TypeSequence)type, name, namePrefix);
			break;
		case RECORD:
			genDeclTypeRecord(outh, outc, (TypeRecord)type, name, namePrefix);
			break;
		case CHOICE:
			genDeclTypeChoice(outh, outc, (TypeChoice)type, name, namePrefix);
			break;
		case MACHINE:
			genDeclTypeMachine(outh, outc, (TypeMachine)type, name, namePrefix);
			break;
		// reference
		case REFERENCE:
			outh.format("using %s = %s;", name, toTypeString(type));
			break;
		case PROCEDURE:
		case ERROR:
		default:
			throw new CompilerException(String.format("Unexpected type %s", type.toString()));
		}
	}
	private void genDeclConstError(LinePrinter outh, LinePrinter outc, String namePrefix, TypeError typeError, String name, Constant constant) {
		//
		// Output declaration of class
		//
		long code;
		{
			if (constant.kind == Constant.Kind.NUMBER) {
				ConstantNumber constantNumber = (ConstantNumber)constant;
				code = constantNumber.value;
			} else {
				throw new CompilerException(String.format("Unexpected constant.kind %s", constant.toString()));
			}
		}
		
		outh.format("struct %s : public Service::Abort {", name);
		{
			StringBuffer initList = new StringBuffer();
			for(Field field: typeError.paramList) {
				initList.append(String.format(", %s()", field.name));
			}
			outh.format("%s() : Service::Abort(programName, programCode, versionCode, \"%s\", %d)%s {}", name, name, code, initList.toString());
		}
		
		if (typeError.paramList.isEmpty()) {
			//
		} else {
			outh.line();
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			for(Field field: typeError.paramList) {
				c1.add(toTypeString(field.type));
				c2.add(String.format("%s;", Util.sanitizeSymbol(field.name)));
			}
			ColumnLayout.layoutStringString(outh, c1, c2);
		}
		
		outh.line("};");
		
		// Build recordInfo
		{
			TypeRecord typeRecord = new TypeRecord();
			for(Field field: typeError.paramList) {
				typeRecord.addField(field);
			}
			context.recordInfoList.add(new RecordInfo(typeRecord, name, namePrefix));
		}
		
		// Build errorInfo
		{
			context.errorInfoList.add(new ErrorInfo(typeError, name, namePrefix, code));
		}
	}
	private void genDeclConstProcedure(LinePrinter outh, LinePrinter outc, String namePrefix, TypeProcedure typeProcedure, String name, Constant constant) {
		//
		// Output declaration of class
		//
		long code;
		{
			if (constant.kind == Constant.Kind.NUMBER) {
				ConstantNumber constantNumber = (ConstantNumber)constant;
				code = constantNumber.value;
			} else {
				throw new CompilerException(String.format("Unexpected constant.kind %s", constant.toString()));
			}
		}

		outh.format("struct %s {", name);
		outh.format("static constexpr const char*   NAME = \"%s\";", name);
		outh.format("static constexpr const quint16 CODE = %d;", code);
		outh.line();
		outh.line("struct Param {");
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			for(Field field: typeProcedure.paramList) {
				String fieldType = toTypeString(field.type);
				String fieldName = field.name;
				c1.add(fieldType);
				c2.add(String.format("%s;", Util.sanitizeSymbol(fieldName)));
			}
			ColumnLayout.layoutStringString(outh, c1, c2);
		}
		outh.line("};");
		
		outh.line("struct Result {");
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			for(Field field: typeProcedure.resultList) {
				String fieldType = toTypeString(field.type);
				String fieldName = field.name;
				c1.add(fieldType);
				c2.add(String.format("%s;", Util.sanitizeSymbol(fieldName)));
			}
			ColumnLayout.layoutStringString(outh, c1, c2);
		}
		outh.line("};");
		
		outh.line();
		outh.line("using call = void (*)(Param& param, Result& result);");
		if (typeProcedure.errroList.isEmpty()) {
			outh.line("// nothrow");
		} else {
			outh.format("// throw %s", String.join(", ", typeProcedure.errroList));
		}
		outh.line("};");
		
		// Build recordInfo
		{
			TypeRecord typeRecord = new TypeRecord();
			for(Field field: typeProcedure.paramList) {
				typeRecord.addField(field);
			}
			context.recordInfoList.add(new RecordInfo(typeRecord, "Param", String.format("%s::%s", namePrefix, name)));
		}
		{
			TypeRecord typeRecord = new TypeRecord();
			for(Field field: typeProcedure.resultList) {
				typeRecord.addField(field);
			}
			context.recordInfoList.add(new RecordInfo(typeRecord, "Result", String.format("%s::%s", namePrefix, name)));
		}
		// Build procedureInfo
		{
			context.procedureInfoList.add(new ProcedureInfo(typeProcedure, name, namePrefix, code));
		}
	}
	private void genDeclConstRecord(LinePrinter outh, LinePrinter outc, String namePrefix, Type type, String name, Constant constant) {
        //const Ordering defaultOrdering = {AttributeType::name, true, Interpretation::string};

		if (type.kind == Type.Kind.REFERENCE) {
			TypeReference typeReference = (TypeReference)type;
			String typeName = program.getLocalRefName(typeReference);
			List<String> values = new ArrayList<>();
			//   [RECORD (3)[key: [Const REF name]  ascending: true  interpretation: [Const REF string]]
			if (constant.kind == Constant.Kind.RECORD) {
				Type concreteType = type.getConcreteType();
				if (concreteType.kind == Type.Kind.RECORD) {
					TypeRecord typeRecord = (TypeRecord)concreteType;
					Map<String, Type> fieldMap = new TreeMap<>();
					for(Field field: typeRecord.fields) {
						fieldMap.put(field.name, field.type);
					}
					
					ConstantRecord constantRecord = (ConstantRecord)constant;
					for(Component component: constantRecord.values) {
						String componentName = component.name;
						if (fieldMap.containsKey(componentName)) {
							Type componentType = fieldMap.get(componentName);
							switch(componentType.kind) {
							case REFERENCE:
								values.add(String.format("%s::%s", program.getLocalRefName((TypeReference)componentType), toConstantString(component.constant)));
								break;
							default:
								values.add(toConstantString(component.constant));
								break;
							}
						} else {
							throw new CompilerException(String.format("Unexpected componentName %s", componentName.toString()));
						}
					}
				} else {
					throw new CompilerException(String.format("Unexpected concreteType %s", concreteType.toString()));
				}
			} else if (constant.kind == Constant.Kind.ARRAY) {
				ConstantArray constantArray = (ConstantArray)constant;
				for(Constant constantArrayElement: constantArray.values) {
					values.add(toConstantString(constantArrayElement));
				}
			} else {
				throw new CompilerException(String.format("Unexpected constant %s", constant.toString()));
			}

			outh.format("const %s %s = {%s};", typeName, name, String.join(", ", values));
		} else {
			throw new CompilerException(String.format("Unexpected type %s", type.toString()));
		}
	}
	private void genDeclConstSequence(LinePrinter outh, LinePrinter outc, String namePrefix, Type type, String name, Constant constant) {
        // CONST  firstPosition
        //   [REF Filing4::Position]
        //   [SEQUENCE (100)UNSPECIFIED]]
        //   [ARRAY (1)[0]
        //     UNSPECIFIED

		if (type.kind == Type.Kind.REFERENCE) {
			String typeName = program.getLocalRefName((TypeReference)type);
			List<String> values = new ArrayList<>();
			if (constant.kind == Constant.Kind.ARRAY) {
				ConstantArray constantArray = (ConstantArray)constant;
				for(Constant constantArrayElement: constantArray.values) {
					values.add(toConstantString(constantArrayElement));
				}
			} else {
				throw new CompilerException(String.format("Unexpected constant %s", constant.toString()));
			}
			outh.format("const %s %s = {%s};", typeName, name, String.join(", ", values));
		} else {
			throw new CompilerException(String.format("Unexpected type %s", type.toString()));
		}
	}
	private void genDeclConstArray(LinePrinter outh, LinePrinter outc, String namePrefix, Type type, String name, Constant constant) {
        // CONST  nullFileID
        //   [REF Filing4::FileID]
        //   [ARRAY (5)UNSPECIFIED]]
        //   [ARRAY (5)[0  0  0  0  0]
        //     UNSPECIFIED

		if (type.kind == Type.Kind.REFERENCE) {
			String typeName = program.getLocalRefName((TypeReference)type);
			List<String> values = new ArrayList<>();
			if (constant.kind == Constant.Kind.ARRAY) {
				ConstantArray constantArray = (ConstantArray)constant;
				for(Constant constantArrayElement: constantArray.values) {
					values.add(toConstantString(constantArrayElement));
				}
			} else {
				throw new CompilerException(String.format("Unexpected constant %s", constant.toString()));
			}
			outh.format("const %s %s = {%s};", typeName, name, String.join(", ", values));
		} else {
			throw new CompilerException(String.format("Unexpected type %s", type.toString()));
		}
	}
	private void genDeclConst(LinePrinter outh, LinePrinter outc, String namePrefix, DeclConst declConst) {
		Type     type         = declConst.type;
		String   name         = Util.sanitizeSymbol(declConst.name);
		Constant constant     = declConst.constant;
		Type     concreteType = type.getConcreteType();
		
		switch(concreteType.kind) {
		// predefined
		case BOOLEAN:
		case BYTE:
		case CARDINAL:
		case LONG_CARDINAL:
		case STRING:
		case UNSPECIFIED:
		case UNSPECIFIED2:
		case UNSPECIFIED3:
			outh.format("const %s %s = %s;", toTypeString(type), name, toConstantString(constant));
			break;
		// constructed
		case ENUM:
		{
			if (type.kind == Type.Kind.REFERENCE && constant.kind == Constant.Kind.REFERENCE) {
				String typeString = program.getLocalRefName((TypeReference)type);
				String constString = program.getLocalRefName((ConstantReference)constant);
				outh.format("const %s %s = %s::%s;", typeString, name, typeString, constString);
			} else {
				throw new CompilerException(String.format("Unexpected type %s", type.toString()));
			}
		}
			break;
		case ERROR:
			genDeclConstError(outh, outc, namePrefix, (TypeError)type, name, constant);
			break;
		case PROCEDURE:
			genDeclConstProcedure(outh, outc, namePrefix, (TypeProcedure)type, name, constant);
			break;
		case RECORD:
			genDeclConstRecord(outh, outc, namePrefix, type, name, constant);
			break;
		case ARRAY:
			genDeclConstArray(outh, outc, namePrefix, type, name, constant);
			break;
		case SEQUENCE:
			genDeclConstSequence(outh, outc, namePrefix, type, name, constant);
			break;
		case CHOICE:
		// reference
		case REFERENCE:
			outh.format("// FIXME CONST %s", concreteType.kind);
			break;
		default:
			throw new CompilerException(String.format("Unexpected type %s", type.toString()));
		}
	}
	private void genDecl(LinePrinter outh, LinePrinter outc, String namePrefix) {
		for(Decl decl: program.declList) {
			outh.line();
			logDecl(outh, decl);
			
			switch(decl.kind) {
			case TYPE:
				genDeclType(outh, outc, namePrefix, (DeclType)decl);
				break;
			case CONST:
				genDeclConst(outh, outc, namePrefix, (DeclConst)decl);
				break;
			default:
				throw new CompilerException(String.format("Unexpected decl.kind %s", decl.kind));
			}
		}
	}

	private void logDecl(LinePrinter out, Decl decl) {
		Type   type         = decl.type;
		String name         = decl.name;

		switch(decl.kind) {
		case TYPE:
			out.format("// %s  %s  %s", decl.kind, name, type);
			break;
		case CONST:
			out.format("// %s  %s  %s  %s", decl.kind, name, decl.getConstant(), type);
			break;
		default:
			throw new CompilerException(String.format("Unexpected decl %s", decl));
		}
		if (type.toString().compareTo(type.getTrueConcreteType().toString()) != 0) {
			out.format("//   %s", type.getTrueConcreteType());
		}
	}

	private void logField(LinePrinter out, Type type, String name) {
		out.format("// %s  %s", name, type);
		
		if (type.toString().compareTo(type.getTrueConcreteType().toString()) != 0) {
			out.format("//   %s", type.getTrueConcreteType());
		}
	}

	private void genDeclTypeRecord(LinePrinter outh, LinePrinter outc, TypeRecord typeRecord, String name, String namePrefix) {
		//
		// Output declaration of class
		//
		List<String> c1 = new ArrayList<>();
		List<String> c2 = new ArrayList<>();
		for(Field field: typeRecord.fields) {
			String fieldType = toTypeString(field.type);
			String fieldName = field.name;
			c1.add(fieldType);
			
			// FIXME Should create default constructor of BLOCK?
			if (field.type.kind == Type.Kind.BLOCK) {
				c2.add(String.format("%s{nullptr, 0};", Util.sanitizeSymbol(fieldName)));
			} else {
				c2.add(String.format("%s;", Util.sanitizeSymbol(fieldName)));
			}
		}

		outh.format("struct %s {", name);
		ColumnLayout.layoutStringString(outh, c1, c2);
		outh.line("};");
		
		// build context.recordInfoList
		{
			context.recordInfoList.add(new RecordInfo(typeRecord, name, namePrefix));
		}
	}
	
	private void genDeclTypeEnum(LinePrinter outh, LinePrinter outc, TypeEnum type, String name, String namePrefix) {
		String typeString;
		switch(type.type.kind) {
		case BYTE:
			typeString = "quint8";
			break;
		case UNSPECIFIED:
			typeString = "quint16";
			break;
		case UNSPECIFIED2:
			typeString = "quint32";
			break;
		case UNSPECIFIED3:
			typeString = "quint64";
			break;
		default:
			throw new CompilerException(String.format("Unexpected type %s", type.type.toString()));
		}
				
		List<String> c1 = new ArrayList<>();
		List<String> c2 = new ArrayList<>();
		List<String> c3 = new ArrayList<>();
		for(Correspondence correspondence: type.elements) {
			c1.add(Util.sanitizeSymbol(correspondence.id));
			c2.add("=");
			c3.add(String.format("%s,", toNumberString(correspondence.numericValue)));
		}
		
		outh.format("enum class %s : %s {", name, typeString);
		ColumnLayout.layoutStringStringRight(outh, c1, c2, c3);
		outh.line("};");
		
		// build context.enumInfoList
		context.enumInfoList.add(new EnumInfo((TypeEnum)type, name, namePrefix));
	}
	
	
	private void genDeclTypeArray(LinePrinter outh, LinePrinter outc, TypeArray typeArray, String name, String namePrefix) {
		outh.format("using %s = %s;", name, toTypeString(typeArray));
		
		Type elementType = typeArray.type;
		if (program.isRemoteType(elementType)) {
			//
		} else {
			switch(elementType.getConcreteType().kind) {
			case SEQUENCE:
			case ARRAY:
				context.arrayInfoList.add(new ArrayInfo(typeArray, name, namePrefix));
				break;
			default:
				break;
			}
		}
	}
	private void genDeclTypeSequence(LinePrinter outh, LinePrinter outc, TypeSequence typeSequence, String name, String namePrefix) {
		outh.format("using %s = %s;", name, toTypeString(typeSequence));
		
		Type elementType = typeSequence.type;
		if (program.isRemoteType(elementType)) {
			//
		} else {
			switch(elementType.getConcreteType().kind) {
			case SEQUENCE:
			case ARRAY:
				context.sequenceInfoList.add(new SequenceInfo(typeSequence, name, namePrefix));
				break;
			default:
				break;
			}
		}
	}

	private void genDeclTypeChoice(LinePrinter outh, LinePrinter outc, TypeChoice typeChoice, String name, String namePrefix) {
		if (typeChoice instanceof TypeChoice.Typed) {
			genDeclTypeChoiceTyped(outh, outc, (TypeChoice.Typed)typeChoice, name, namePrefix);
		} else if (typeChoice instanceof TypeChoice.Anon) {
			genDeclTypeChoiceAnon(outh, outc, (TypeChoice.Anon)typeChoice, name, namePrefix);
		} else {
			throw new CompilerException(String.format("Unexpected typeChoice %s", typeChoice.toString()));
		}
	}
	
	// FIXME If CHOICE contains CHOICE directly, nest struct CHOICE_01 contains declaration of struct CHOICE_01.
	//       To fix this problem, prepend struct name to choice type name.
	private String getCandidateName(String name, int structNo) {
		return String.format("%s_CHOICE_%02d", name, structNo);
	}
	private void genDeclTypeChoiceTyped(LinePrinter outh, LinePrinter outc, TypeChoice.Typed typed, String name, String namePrefix) {
		outh.format("struct %s {", name);
		
		List<String>         choiceNameList  = new ArrayList<>(); // choice name list in appearance order
		Map<String, Integer> choiceMap       = new TreeMap<>();   // choice name => struct number
		
		{
			int structNumber = 0;
			for(Candidate<String> candidate: typed.candidates) {
				structNumber++;
				Type candidateType = candidate.type;
				
				// build choiceMape
				String candidateName = getCandidateName(name, structNumber);
				for(String id: candidate.designators) {
					String choiceName = id;
					choiceNameList.add(choiceName);
					choiceMap.put(choiceName, structNumber);
				}
				// generate choice type declaration
				genDeclType(outh, outc, String.format("%s::%s", namePrefix, name), candidateType, candidateName);
			}
			
			// Provide alias of type for code readability
			{
				List<String> c1 = new ArrayList<>();
				List<String> c2 = new ArrayList<>();
				for(String choiceName: choiceNameList) {
					int n = choiceMap.get(choiceName);
					c1.add(String.format("using %s_%s",  name, choiceName));
					c2.add(String.format("= %s;",  getCandidateName(name, n)));
				}
				outh.line("// Provide alias for code readability");
				ColumnLayout.layoutStringString(outh, c1, c2);
			}
		}
		
		outh.line();
		outh.format("using CHOICE_TAG = %s;", toTypeString(typed.type));
		outh.line("CHOICE_TAG choiceTag;");
		outh.line();
		
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			List<String> c3 = new ArrayList<>();
			
			for(String choiceName: choiceNameList) {
				c1.add(String.format("%s_%s&",  name, choiceName));
				c2.add(String.format("%s()", Util.sanitizeSymbol(choiceName)));
				c3.add("const;");
			}
			ColumnLayout.layoutStringString(outh, c1, c2, c3);
		}

		outh.line("private:");
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(String choiceName: choiceNameList) {
				c1.add(String.format("mutable %s_%s", name, choiceName));
				c2.add(String.format("choice_%s;", choiceName));
			}
			ColumnLayout.layoutStringString(outh, c1, c2);
		}
		
		outh.line("};");
		
		for(String choiceName: choiceNameList) {
			outc.line();
			outc.format("%s::%s::%s_%s& %s::%s::%s() const {", namePrefix, name, name, choiceName, namePrefix, name, Util.sanitizeSymbol(choiceName));
			outc.format("if (choiceTag == CHOICE_TAG::%s) {", Util.sanitizeSymbol(choiceName));
			outc.format("return choice_%s;", choiceName);
			outc.line("} else {");
			outc.format("logger.error(\"choiceTag  expect %s  actual %%s\", Courier::toString(choiceTag));", choiceName);
			outc.line("COURIER_FATAL_ERROR();");
			outc.line("}");
			outc.line("}");
		}
		
		// Build choideInfoList
		{
			context.choiceInfoList.add(new ChoiceInfo(typed, name, namePrefix, choiceNameList));
		}
	}
	private void genDeclTypeChoiceAnon(LinePrinter outh, LinePrinter outc, TypeChoice.Anon anon, String name, String namePrefix) {
		// Check for StreamOf special case
		{
			if (name.startsWith("StreamOf") && anon.candidates.size() == 2) {
				Candidate<Correspondence> nextSegment = anon.candidates.get(0);
				Candidate<Correspondence> lastSegment = anon.candidates.get(1);
				
				String nextSegmentName = nextSegment.designators.get(0).id;
				String lastSegmentName = lastSegment.designators.get(0).id;
				
				if (nextSegmentName.compareTo("nextSegment") == 0 && lastSegmentName.compareTo("lastSegment") == 0) {
					Type lastSegmentType = lastSegment.type;
					if (lastSegmentType.kind == Type.Kind.SEQUENCE) {
						TypeSequence typeSequence = (TypeSequence)lastSegmentType;
						Type elementType = typeSequence.type;
						
						outh.line("// FIXME");
						outh.format("using %s = Courier::StreamOf<%s>;", name, toTypeString(elementType));
						return;
					}
				}
			}
		}

		outh.format("struct %s {", name);
		
		List<String>         choiceNameList  = new ArrayList<>(); // choice name list in appearance order
		Map<String, Integer> choiceMap       = new TreeMap<>();   // choice name => struct number

		{
			List<String>  c1 = new ArrayList<>();
			List<Integer> c2 = new ArrayList<>();
			for(Candidate<Correspondence> candidate: anon.candidates) {
				for(Correspondence correspondence: candidate.designators) {
					c1.add(Util.sanitizeSymbol(correspondence.id));
					c2.add((int)correspondence.numericValue);
				}
			}
			
			outh.format("enum class %s : quint16 {", "CHOICE_TAG");
			ColumnLayout.layoutEnumElement(outh, c1, c2);
			outh.line("};");
		}
		
		{
			int structNumber = 0;
			for(Candidate<Correspondence> candidate: anon.candidates) {
				structNumber++;
				Type candidateType = candidate.type;
				
				// build choiceMape
				String candidateName = getCandidateName(name, structNumber);
				for(Correspondence correspondence: candidate.designators) {
					String choiceName = correspondence.id;
					choiceNameList.add(choiceName);
					choiceMap.put(choiceName, structNumber);
				}
				// generate choice type declaration
				genDeclType(outh, outc, String.format("%s::%s", namePrefix, name), candidateType, candidateName);
			}
			
			// Provide alias of type for code readability
			{
				List<String> c1 = new ArrayList<>();
				List<String> c2 = new ArrayList<>();
				for(String choiceName: choiceNameList) {
					int n = choiceMap.get(choiceName);
					c1.add(String.format("using %s_%s",  name, choiceName));
					c2.add(String.format("= %s;",  getCandidateName(name, n)));
				}
				outh.line("// Provide alias for code readability");
				ColumnLayout.layoutStringString(outh, c1, c2);
			}
		}
		
		outh.line();
		outh.line("CHOICE_TAG choiceTag;");
		outh.line("");
		
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			List<String> c3 = new ArrayList<>();
			
			for(String choiceName: choiceNameList) {
				c1.add(String.format("%s_%s&",  name, choiceName));
				c2.add(String.format("%s()", Util.sanitizeSymbol(choiceName)));
				c3.add("const;");
			}
			ColumnLayout.layoutStringString(outh, c1, c2, c3);
		}

		outh.line("private:");
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(String choiceName: choiceNameList) {
				c1.add(String.format("mutable %s_%s", name, choiceName));
				c2.add(String.format("choice_%s;", choiceName));
			}
			ColumnLayout.layoutStringString(outh, c1, c2);
		}
		
		outh.line("};");
		
		for(String choiceName: choiceNameList) {
			outc.line();
			outc.format("%s::%s::%s_%s& %s::%s::%s() const {", namePrefix, name, name, choiceName, namePrefix, name, Util.sanitizeSymbol(choiceName));
			outc.format("if (choiceTag == CHOICE_TAG::%s) {", Util.sanitizeSymbol(choiceName));
			outc.format("return choice_%s;", choiceName);
			outc.line("} else {");
			outc.format("logger.error(\"choiceTag  expect %s  actual %%s\", Courier::toString(choiceTag));", choiceName);
			outc.line("COURIER_FATAL_ERROR();");
			outc.line("}");
			outc.line("}");
		}
		
		// Build context.enumInfoList
		{
			// Build typeEnum from anon
			TypeEnum typeEnum = new TypeEnum(new TypePredefined(Type.Kind.UNSPECIFIED));
			for(Candidate<Correspondence> candidate: anon.candidates) {
				for(Correspondence correspondence: candidate.designators) {
					typeEnum.addCorrespondence(correspondence);
				}
			}
			
			context.enumInfoList.add(new EnumInfo(typeEnum, "CHOICE_TAG", String.format("%s::%s", namePrefix, name)));
		}
		
		// Build choideInfoList
		{
			context.choiceInfoList.add(new ChoiceInfo(anon, name, namePrefix, choiceNameList));
		}
	}
	
	private void genDeclTypeMachine(LinePrinter outh, LinePrinter outc, TypeMachine typeMachine, String name, String namePrefix) {
		//
		// Output declaration of class
		//
		List<String> c1 = new ArrayList<>();
		List<String> c2 = new ArrayList<>();
		List<String> c3 = new ArrayList<>();
		for(Type.MDField mdField: typeMachine.mdFields) {
			String fieldType = toTypeString(mdField.type);
			String fieldName = mdField.name;
			c1.add(fieldType);
			c2.add(String.format("%s;", Util.sanitizeSymbol(fieldName)));
			c3.add(String.format("// %d .. %d", mdField.start, mdField.stop));
		}

		outh.format("struct %s /* %s */ {", name, typeMachine.type);
		ColumnLayout.layoutStringString(outh, c1, c2, c3);
		outh.line("};");
		
		// Build choideInfoList
		{
			context.machineInfoList.add(new MachineInfo(typeMachine, name, namePrefix));
		}
	}
	
	private void genEnumToString(LinePrinter outh, LinePrinter outc) {
		if (context.enumInfoList.isEmpty()) return;

		// Declaration
		outh.line("",
				  "//",
				  "// Enum Function Declaration",
				  "//");

		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(EnumInfo enumInfo: context.enumInfoList) {
				c1.add(String.format("QString toString(const %s::%s", enumInfo.namePrefix, enumInfo.name));
				c2.add("value);");
			}
			ColumnLayout.layoutStringString(outh, c1, c2);
		}
		
		// Definition
		outc.line("",
				  "//",
				  "// Enum Function Definition",
				  "//");

		for(EnumInfo enumInfo: context.enumInfoList) {
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
			
			ColumnLayout.layoutStringString(outc, c1, c2);
			
			outc.line("};"); // end of static QMap
			
			outc.line("",
					  "if (map.contains(value)) {",
					      "return map[value];",
					  "} else {",
					      "return QString(\"%1\").arg((quint16)value);",
					  "}");
			
			outc.line("}"); // end of toString
		}
	}
	private static String toEnumTypeString(Type type) {
		switch(type.getConcreteType().kind) {
		case BYTE:
			return "quint8";
		case UNSPECIFIED:
			return "quint16";
		case UNSPECIFIED2:
			return "quint32";
		case UNSPECIFIED3:
			return "quint64";
		default:
			throw new CompilerException(String.format("Unexpected type %s", type));
		}
	}
	private void genEnumSerialize(LinePrinter outh, LinePrinter outc) {
		if (context.enumInfoList.isEmpty()) return;
		outh.line();
		outc.line();
		
		// Declaration
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(EnumInfo enumInfo: context.enumInfoList) {
				c1.add(String.format("void serialize(BLOCK& block, const %s::%s", enumInfo.namePrefix, enumInfo.name));
				c2.add("value);");
			}
			ColumnLayout.layoutStringString(outh, c1, c2);
		}
		
		// Definition
		for(EnumInfo enumInfo: context.enumInfoList) {
			outc.line();
			outc.format("void %s::serialize(BLOCK& block, const %s::%s value) {", "Courier", enumInfo.namePrefix, enumInfo.name);
			outc.format("Courier::serialize(block, (%s)value);", toEnumTypeString(enumInfo.typeEnum.type));
			outc.line("}");
		}
	}
	private void genEnumDeserialize(LinePrinter outh, LinePrinter outc) {
		if (context.enumInfoList.isEmpty()) return;
		outh.line();
		outc.line();
		
		// Declaration
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(EnumInfo enumInfo: context.enumInfoList) {
				c1.add(String.format("void deserialize(BLOCK& block, %s::%s&", enumInfo.namePrefix, enumInfo.name));
				c2.add("value);");
			}
			ColumnLayout.layoutStringString(outh, c1, c2);
		}
		
		// Definition
		for(EnumInfo enumInfo: context.enumInfoList) {
			outc.line();
			outc.format("void %s::deserialize(BLOCK& block, %s::%s& value) {", "Courier", enumInfo.namePrefix, enumInfo.name);
			outc.format("%s t;", toEnumTypeString(enumInfo.typeEnum.type));
			outc.line("Courier::deserialize(block, t);");
			outc.format("value = (%s::%s)t;", enumInfo.namePrefix, enumInfo.name);
			outc.line("}");
		}
	}

	private void genRecordToString(LinePrinter outh, LinePrinter outc) {
		if (context.recordInfoList.isEmpty()) return;
		outh.line();
		outc.line();
		
		outh.line("//",
				  "// Record Function Declaration",
				  "//");

		outc.line("//",
				  "// Record Function Definition",
				  "//");

		// Record toSring declaration
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(RecordInfo recordInfo: context.recordInfoList) {
				c1.add(String.format("QString toString(const %s::%s&", recordInfo.namePrefix, recordInfo.name));
				c2.add("value);");
			}
			ColumnLayout.layoutStringString(outh, c1, c2);
		}
		
		// Record toString definition
		for(RecordInfo recordInfo: context.recordInfoList) {
			if (recordInfo.typeRecord.fields.isEmpty()) {
				outc.format("QString %s::toString(const %s::%s&) {", "Courier", recordInfo.namePrefix, recordInfo.name);
				outc.line("return \"[]\";");
				outc.line("}");
			} else {
				outc.format("QString %s::toString(const %s::%s& value) {", "Courier", recordInfo.namePrefix, recordInfo.name);
				outc.line("QStringList list;");
				
				for(Field field: recordInfo.typeRecord.fields) {
					logField(outc, field.type, field.name);
					outc.format("list << QString(\"[%%1 %%2]\").arg(\"%s\").arg(Courier::toString(value.%s));", field.name, field.name);
				}
				
				outc.line("return QString(\"[%1]\").arg(list.join(\" \"));");
				outc.line("}");
			}
		}
	}
	
	void genRecordSerialize(LinePrinter outh, LinePrinter outc) {
		if (context.recordInfoList.isEmpty()) return;
		outh.line();
		outc.line();
		
		// Declaration
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(RecordInfo recordInfo: context.recordInfoList) {
				c1.add(String.format("void serialize(BLOCK& block, const %s::%s&", recordInfo.namePrefix, recordInfo.name));
				c2.add("value);");
			}
			ColumnLayout.layoutStringString(outh, c1, c2);
		}
		
		// Definition
		for(RecordInfo recordInfo: context.recordInfoList) {
			if (recordInfo.typeRecord.fields.isEmpty()) {
				outc.format("void %s::serialize(BLOCK&, const %s::%s&) {", "Courier", recordInfo.namePrefix, recordInfo.name);
				outc.line("}");
			} else {
				outc.format("void %s::serialize(BLOCK& block, const %s::%s& value) {", "Courier", recordInfo.namePrefix, recordInfo.name);
				for(Field field: recordInfo.typeRecord.fields) {
					logField(outc, field.type, field.name);
					// FIXME
					if (field.type.kind == Kind.BLOCK) {
						outc.format("// Courier::serialize(block, value.%s);", field.name);
					} else {
						outc.format("Courier::serialize(block, value.%s);", field.name);
					}
				}
				outc.line("}");
			}
		}
	}
	void genRecordDeserialize(LinePrinter outh, LinePrinter outc) {
		if (context.recordInfoList.isEmpty()) return;
		outh.line();
		outc.line();
		
		// Declaration
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(RecordInfo recordInfo: context.recordInfoList) {
				c1.add(String.format("void deserialize(BLOCK& block, %s::%s&", recordInfo.namePrefix, recordInfo.name));
				c2.add("value);");
			}
			ColumnLayout.layoutStringString(outh, c1, c2);
		}
		
		// Definition
		for(RecordInfo recordInfo: context.recordInfoList) {
			if (recordInfo.typeRecord.fields.isEmpty()) {
				outc.format("void %s::deserialize(BLOCK&, %s::%s&) {", "Courier", recordInfo.namePrefix, recordInfo.name);
				outc.line("}");
			} else {
				outc.format("void %s::deserialize(BLOCK& block, %s::%s& value) {", "Courier", recordInfo.namePrefix, recordInfo.name);
				int count = 0;
				int size = recordInfo.typeRecord.fields.size();
				for(Field field: recordInfo.typeRecord.fields) {
					count++;
					logField(outc, field.type, field.name);
					if (field.type.kind == Type.Kind.BLOCK) {
						// assume this is the last field
						if (count == size) {
							// special for BLOCK
							outc.format("value.%s = block.remainder();", field.name);
						} else {
							throw new CompilerException(String.format("Unexpected count = %d  size = %d", count, size));
						}
					} else {
						outc.format("Courier::deserialize(block, value.%s);", field.name);
					}
				}
				outc.line("}");
			}
		}
	}
	
	private void genChoiceToString(LinePrinter outh, LinePrinter outc) {
		if (context.choiceInfoList.isEmpty()) return;
		outh.line();
		outc.line();
		
		outh.line("//",
				  "// Choice Function Declaration",
				  "//");

		outc.line("//",
				  "// Choice Function Definition",
				  "//");

		// Choice toSring declaration
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(ChoiceInfo choiceInfo: context.choiceInfoList) {
				c1.add(String.format("QString toString(const %s::%s&", choiceInfo.namePrefix, choiceInfo.name));
				c2.add("value);");
			}
			ColumnLayout.layoutStringString(outh, c1, c2);
		}
		
		// Choice toString definition
		for(ChoiceInfo choiceInfo: context.choiceInfoList) {
			outc.format("QString %s::toString(const %s::%s& value) {", "Courier", choiceInfo.namePrefix, choiceInfo.name);
			outc.line("switch(value.choiceTag) {");
			
			for(String choiceName: choiceInfo.choiceNameList) {
				outc.format("case %s::%s::CHOICE_TAG::%s:", choiceInfo.namePrefix, choiceInfo.name, Util.sanitizeSymbol(choiceName));
				outc.format("return QString(\"[%%1 %%2]\").arg(\"%s\").arg(Courier::toString(value.%s()));", choiceName, Util.sanitizeSymbol(choiceName));
			}

			outc.line("default:");
			outc.line("logger.error(\"Unexpected choiceTag = %d\", (quint16)value.choiceTag);");
	        outc.line("COURIER_FATAL_ERROR();");
	        outc.line("break;");
			outc.line("}"); // end of switch

			outc.line("}"); // end of method
		}
	}

	private void genChoiceSerialize(LinePrinter outh, LinePrinter outc) {
		if (context.choiceInfoList.isEmpty()) return;
		outh.line();
		outc.line();
		
		// Declaration
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(ChoiceInfo choiceInfo: context.choiceInfoList) {
				c1.add(String.format("void serialize(BLOCK& block, const %s::%s&", choiceInfo.namePrefix, choiceInfo.name));
				c2.add("value);");
			}
			ColumnLayout.layoutStringString(outh, c1, c2);
		}
		
		// Definition
		for(ChoiceInfo choiceInfo: context.choiceInfoList) {
			outc.format("void %s::serialize(BLOCK& block, const %s::%s& value) {", "Courier", choiceInfo.namePrefix, choiceInfo.name);
	        outc.line("block.serialize16((quint16)value.choiceTag);");
			outc.line("switch(value.choiceTag) {");
			
			for(String choiceName: choiceInfo.choiceNameList) {
				outc.format("case %s::%s::CHOICE_TAG::%s:", choiceInfo.namePrefix, choiceInfo.name, Util.sanitizeSymbol(choiceName));
				outc.format("Courier::serialize(block, value.%s());", Util.sanitizeSymbol(choiceName));
				outc.line("break;");
			}

			outc.line("default:",
					 "logger.error(\"Unexpected choiceTag = %d\", (quint16)value.choiceTag);",
					 "COURIER_FATAL_ERROR();",
					 "break;",
					 "}"); // end of switch

			outc.line("}"); // end of method
		}
	}

	private void genChoiceDeserialize(LinePrinter outh, LinePrinter outc) {
		if (context.choiceInfoList.isEmpty()) return;
		outh.line();
		outc.line();

		// Declaration
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(ChoiceInfo choiceInfo: context.choiceInfoList) {
				c1.add(String.format("void deserialize(BLOCK& block, %s::%s&", choiceInfo.namePrefix, choiceInfo.name));
				c2.add("value);");
			}
			ColumnLayout.layoutStringString(outh, c1, c2);
		}
		
		// Definition
		for(ChoiceInfo choiceInfo: context.choiceInfoList) {
			outc.format("void %s::deserialize(BLOCK& block, %s::%s& value) {", "Courier", choiceInfo.namePrefix, choiceInfo.name);
			
			outc.line("quint16 choiceTag_;",
				      "block.deserialize16(choiceTag_);");
			outc.format("value.choiceTag = (%s::%s::CHOICE_TAG)choiceTag_;", choiceInfo.namePrefix, choiceInfo.name);
			
			outc.line("switch(value.choiceTag) {");
			
			for(String choiceName: choiceInfo.choiceNameList) {
				outc.format("case %s::%s::CHOICE_TAG::%s:", choiceInfo.namePrefix, choiceInfo.name, Util.sanitizeSymbol(choiceName));
				outc.format("Courier::deserialize(block, value.%s());", Util.sanitizeSymbol(choiceName));
				outc.line("break;");
			}

			outc.line("default:");
			outc.line("logger.error(\"Unexpected choiceTag = %d\", (quint16)value.choiceTag);");
	        outc.line("COURIER_FATAL_ERROR();");
	        outc.line("break;");
			outc.line("}"); // end of switch

			outc.line("}"); // end of method
		}
	}
	private String toNumberString(long value) {
		if (value == 0) {
			return "0";
		} else if (0xF0 <= value && value <= 0xFF) {
			return String.format("0x%X", value);
		} else if (0xFFF0 <= value && value <= 0xFFFF) {
			return String.format("0x%X", value);
		} else if (0xFFFF_FFF0L <= value && value <= 0xFFFF_FFFFL) {
			return String.format("0x%XU", value);
		} else if (0xFFFF_FFFF_FFF0L <= value && value <= 0xFFFF_FFFF_FFFFL) {
			return String.format("0x%XU", value);
		} else if ((value & 0xFF) == 0) {
			return String.format((value <= 0x7FFF_FFF) ? "0x%X" : "0x%XU", value);
		} else {
			return String.format((value <= 0x7FFF_FFF) ? "%d" : "%dU", value);
		}
	}
	private String toConstantString(Constant constant) {
		switch(constant.kind) {
		case BOOLEAN:
		{
			ConstantBoolean constantBoolean = (ConstantBoolean)constant;
			return constantBoolean.value ? "true" : "false";
		}
		case NUMBER:
		{
			ConstantNumber constantNumber = (ConstantNumber)constant;
			long value = constantNumber.value;
			return toNumberString(value);
		}
		case STRING:
		{
			ConstantString constantString = (ConstantString)constant;
			return constantString.value;
		}
		case ARRAY:
		case RECORD:
		case EMPTY:
		case CHOICE:
			throw new CompilerException(String.format("Unexpected type %s", constant));
		case REFERENCE:
		{
			ConstantReference constantReference = (ConstantReference)constant;
			return program.getLocalRefName(constantReference);
		}

		default:
				return constant.toString();
		}
	}
	
	private void genSequenceToString(LinePrinter outh, LinePrinter outc) {
		if (context.sequenceInfoList.isEmpty()) return;
		outh.line();
		outc.line();
		
		outh.line("//",
				  "// Sequence Function Declaration",
				  "//");

		outc.line("//",
				  "// Sequence Function Definition",
				  "//");

		// Declaration
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(SequenceInfo sequenceInfo: context.sequenceInfoList) {
				TypeSequence typeSequence = new TypeSequence(sequenceInfo.typeSequence.size, sequenceInfo.typeSequence.type.getConcreteType());
				String       typeString   = toTypeStringNotLocal(typeSequence);
				
				c1.add(String.format("QString toString(const %s&", typeString));
				c2.add("value);");
			}
			ColumnLayout.layoutStringString(outh, c1, c2);
		}
		// Definition
		for(SequenceInfo sequenceInfo: context.sequenceInfoList) {
			TypeSequence typeSequence = new TypeSequence(sequenceInfo.typeSequence.size, sequenceInfo.typeSequence.type.getConcreteType());
			String       typeString   = toTypeStringNotLocal(typeSequence);

			outc.format("QString %s::toString(const %s& value) {", "Courier", typeString);
			
			outc.line("QStringList list;",
					  "quint16 size = value.getSize();",
					  "for(quint16 i = 0; i < size; i++) {",
					  "list << Courier::toString(value[i]);",
					  "}",
					  "return QString(\"(%1)[%2]\").arg(size).arg(list.join(\" \"));");

			outc.line("}");
		}
	}
	private void genSequenceSerialize(LinePrinter outh, LinePrinter outc) {
		if (context.sequenceInfoList.isEmpty()) return;
		outh.line();
		outc.line();
		
		// Declaration
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(SequenceInfo sequenceInfo: context.sequenceInfoList) {
				TypeSequence typeSequence = new TypeSequence(sequenceInfo.typeSequence.size, sequenceInfo.typeSequence.type.getConcreteType());
				String       typeString   = toTypeStringNotLocal(typeSequence);
				
				c1.add(String.format("void serialize(BLOCK& block, const %s&", typeString));
				c2.add("value);");
			}
			ColumnLayout.layoutStringString(outh, c1, c2);
		}
		// Definition
		for(SequenceInfo sequenceInfo: context.sequenceInfoList) {
			TypeSequence typeSequence = new TypeSequence(sequenceInfo.typeSequence.size, sequenceInfo.typeSequence.type.getConcreteType());
			String       typeString   = toTypeStringNotLocal(typeSequence);
			
			outc.format("void %s::serialize(BLOCK& block, const %s& value) {", "Courier", typeString);
			
			outc.line("quint16 size = value.getSize();",
					  "Courier::serialize(block, size);",
					  "for(quint16 i = 0; i < size; i++) {",
					  "Courier::serialize(block, value[i]);",
					  "}");

			outc.line("}");
		}
	}
	private void genSequenceDeserialize(LinePrinter outh, LinePrinter outc) {
		if (context.sequenceInfoList.isEmpty()) return;
		outh.line();
		outc.line();
		
		// Declaration
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(SequenceInfo sequenceInfo: context.sequenceInfoList) {
				TypeSequence typeSequence = new TypeSequence(sequenceInfo.typeSequence.size, sequenceInfo.typeSequence.type.getConcreteType());
				String       typeString   = toTypeStringNotLocal(typeSequence);
				
				c1.add(String.format("void deserialize(BLOCK& block, %s&", typeString));
				c2.add("value);");
			}
			ColumnLayout.layoutStringString(outh, c1, c2);
		}
		// Definition
		for(SequenceInfo sequenceInfo: context.sequenceInfoList) {
			TypeSequence typeSequence = new TypeSequence(sequenceInfo.typeSequence.size, sequenceInfo.typeSequence.type.getConcreteType());
			String       typeString   = toTypeStringNotLocal(typeSequence);

			outc.format("void %s::deserialize(BLOCK& block, %s& value) {", "Courier", typeString);
			
			outc.line("quint16 size = value.getSize();",
					  "Courier::deserialize(block, size);",
					  "for(quint16 i = 0; i < size; i++) {",
					  "Courier::deserialize(block, value[i]);",
					  "}");

			outc.line("}");
		}
	}
	
	
	private void genArrayToString(LinePrinter outh, LinePrinter outc) {
		if (context.arrayInfoList.isEmpty()) return;
		outh.line();
		outc.line();

		outh.line("//",
				  "// Array Function Declaration",
				  "//");

		outc.line("//",
				  "// Array Function Definition",
				  "//");


		// Declaration
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(ArrayInfo arrayInfo: context.arrayInfoList) {
				TypeArray typeArray  = new TypeArray(arrayInfo.typeArray.size, arrayInfo.typeArray.type.getConcreteType());
				String    typeString = toTypeStringNotLocal(typeArray);
				
				c1.add(String.format("QString toString(const %s&", typeString));
				c2.add("value);");
			}
			ColumnLayout.layoutStringString(outh, c1, c2);
		}
		// Definition
		for(ArrayInfo arrayInfo: context.arrayInfoList) {
			TypeArray typeArray  = new TypeArray(arrayInfo.typeArray.size, arrayInfo.typeArray.type.getConcreteType());
			String    typeString = toTypeStringNotLocal(typeArray);

			outc.format("QString %s::toString(const %s& value) {", "Courier", typeString);
			
			outc.line("QStringList list;",
					  "quint16 size = value.getSize();",
					  "for(quint16 i = 0; i < size; i++) {",
					  "list << Courier::toString(value[i]);",
					  "}",
					  "return QString(\"(%1)[%2]\").arg(size).arg(list.join(\" \"));");

			outc.line("}");
		}
	}
	private void genArraySerialize(LinePrinter outh, LinePrinter outc) {
		if (context.arrayInfoList.isEmpty()) return;
		outh.line();
		outc.line();

		// Declaration
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(ArrayInfo arrayInfo: context.arrayInfoList) {
				TypeArray typeArray  = new TypeArray(arrayInfo.typeArray.size, arrayInfo.typeArray.type.getConcreteType());
				String    typeString = toTypeStringNotLocal(typeArray);
				
				c1.add(String.format("void serialize(BLOCK& block, const %s&", typeString));
				c2.add("value);");
			}
			ColumnLayout.layoutStringString(outh, c1, c2);
		}
		// Definition
		for(ArrayInfo arrayInfo: context.arrayInfoList) {
			TypeArray typeArray  = new TypeArray(arrayInfo.typeArray.size, arrayInfo.typeArray.type.getConcreteType());
			String    typeString = toTypeStringNotLocal(typeArray);

			outc.format("void %s::serialize(BLOCK& block, const %s& value) {", "Courier", typeString);
			
			outc.line("quint16 size = value.getSize();",
					  "for(quint16 i = 0; i < size; i++) {",
					  "Courier::serialize(block, value[i]);",
					  "}");

			outc.line("}");
		}
	}
	private void genArrayDeserialize(LinePrinter outh, LinePrinter outc) {
		if (context.arrayInfoList.isEmpty()) return;
		outh.line();
		outc.line();

		// Declaration
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(ArrayInfo arrayInfo: context.arrayInfoList) {
				TypeArray typeArray  = new TypeArray(arrayInfo.typeArray.size, arrayInfo.typeArray.type.getConcreteType());
				String    typeString = toTypeStringNotLocal(typeArray);
				
				c1.add(String.format("void deserialize(BLOCK& block, %s&", typeString));
				c2.add("value);");
			}
			ColumnLayout.layoutStringString(outh, c1, c2);
		}
		// Definition
		for(ArrayInfo arrayInfo: context.arrayInfoList) {
			TypeArray typeArray  = new TypeArray(arrayInfo.typeArray.size, arrayInfo.typeArray.type.getConcreteType());
			String    typeString = toTypeStringNotLocal(typeArray);

			outc.format("void %s::deserialize(BLOCK& block, %s& value) {", "Courier", typeString);
			
			outc.line("quint16 size = value.getSize();",
					  "for(quint16 i = 0; i < size; i++) {",
					  "Courier::deserialize(block, value[i]);",
					  "}");

			outc.line("}");
		}
	}

	private void genMachineToString(LinePrinter outh, LinePrinter outc) {
		if (context.machineInfoList.isEmpty()) return;
		outh.line();
		outc.line();
		
		outh.line("//",
				  "// Machine Function Declaration",
				  "//");

		outc.line("//",
				  "// Machine Function Definition",
				  "//");

		// Machine toSring declaration
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(MachineInfo machineInfo: context.machineInfoList) {
				c1.add(String.format("QString toString(const %s::%s&", machineInfo.namePrefix, machineInfo.name));
				c2.add("value);");
			}
			ColumnLayout.layoutStringString(outh, c1, c2);
		}
		
		// Record toString definition
		for(MachineInfo machineInfo: context.machineInfoList) {
			if (machineInfo.typeMachine.mdFields.isEmpty()) {
				throw new CompilerException(String.format("Unexpected empty field %s::%s", machineInfo.namePrefix, machineInfo.name));
			} else {
				outc.format("QString %s::toString(const %s::%s& value) {", "Courier", machineInfo.namePrefix, machineInfo.name);
				outc.line("QStringList list;");
				
				for(Type.MDField mdField: machineInfo.typeMachine.mdFields) {
					logField(outc, mdField.type, mdField.name);
					outc.format("list << QString(\"[%%1 %%2]\").arg(\"%s\").arg(Courier::toString(value.%s));", mdField.name, mdField.name);
				}
				
				outc.line("return QString(\"[%1]\").arg(list.join(\" \"));");
				outc.line("}");
			}
		}
	}
	// generate serialize for Record
	private void genMachineSerialize(LinePrinter outh, LinePrinter outc) {
		if (context.machineInfoList.isEmpty()) return;
		outh.line();
		outc.line();
		
		// Declaration
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(MachineInfo machineInfo: context.machineInfoList) {
				c1.add(String.format("void serialize(BLOCK& block, const %s::%s&", machineInfo.namePrefix, machineInfo.name));
				c2.add("value);");
			}
			ColumnLayout.layoutStringString(outh, c1, c2);
		}
		
		// Definition
		for(MachineInfo machineInfo: context.machineInfoList) {
			if (machineInfo.typeMachine.mdFields.isEmpty()) {
				throw new CompilerException(String.format("Unexpected empty field %s::%s", machineInfo.namePrefix, machineInfo.name));
			} else {
				String typeString = toTypeString(machineInfo.typeMachine.type);

				outc.format("void %s::serialize(BLOCK& block, const %s::%s& value) {", "Courier", machineInfo.namePrefix, machineInfo.name);
				outc.format("%s v = 0;", typeString);
				for(Type.MDField mdField: machineInfo.typeMachine.mdFields) {
					outc.format("v = setBits(v, %d, %d, (%s)value.%s);", mdField.start, mdField.stop, typeString, mdField.name);
				}
				outc.line("Courier::serialize(block, v);");
				outc.line("}");
			}
		}
	}
	// generate deserialize for Record
	private void genMachineDeserialize(LinePrinter outh, LinePrinter outc) {
		if (context.machineInfoList.isEmpty()) return;
		outh.line();
		outc.line();
		
		// Declaration
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(MachineInfo machineInfo: context.machineInfoList) {
				c1.add(String.format("void deserialize(BLOCK& block, %s::%s&", machineInfo.namePrefix, machineInfo.name));
				c2.add("value);");
			}
			ColumnLayout.layoutStringString(outh, c1, c2);
		}
		
		// Definition
		for(MachineInfo machineInfo: context.machineInfoList) {
			if (machineInfo.typeMachine.mdFields.isEmpty()) {
				throw new CompilerException(String.format("Unexpected empty field %s::%s", machineInfo.namePrefix, machineInfo.name));
			} else {
				outc.format("void %s::deserialize(BLOCK& block, %s::%s& value) {", "Courier", machineInfo.namePrefix, machineInfo.name);
				outc.format("%s v;", toTypeString(machineInfo.typeMachine.type));
				outc.line("Courier::deserialize(block, v);");
				for(Type.MDField mdField: machineInfo.typeMachine.mdFields) {
					outc.format("value.%s = (%s)getBits(v, %d, %d);", mdField.name, toTypeString(mdField.type), mdField.start, mdField.stop);
				}
				outc.line("}");
			}
		}
	}

	
	private static final boolean SHOW_BUILD_TIME;
	private static final String BUILD_TIME;
	static {
		SHOW_BUILD_TIME = false;
		LocalDateTime now = LocalDateTime.now();
		BUILD_TIME = String.format("%04d-%02d-%02d %02d:%02d:%02d %s",
				now.getYear(), now.getMonth().getValue(), now.getDayOfMonth(),
				now.getHour(), now.getMinute(), now.getSecond(),
				ZoneId.systemDefault().getId());
	}
	
	public void genStub() {
		String programName = program.info.getProgramVersion();
		String pathc = String.format("%s%s.cpp", STUB_DIR_PATH, programName);
		String pathh = String.format("%s%s.h",   STUB_DIR_PATH, programName);
		logger.info(String.format("pathc = %s", pathc));
		logger.info(String.format("pathh = %s", pathh));
		
		try (
			LinePrinter outc = new LinePrinter(new PrintWriter(pathc));
			LinePrinter outh = new LinePrinter(new PrintWriter(pathh));) {
			// for outh
			// write opening lines
			if (SHOW_BUILD_TIME) {
				outh.line("//",
						String.format("// File  %s.h", programName),
						String.format("// Build %s",   BUILD_TIME),
						"//",
						"");
			}
			
			outh.format("#ifndef STUB_%s_H__", program.info.getProgramVersion());
			outh.format("#define STUB_%s_H__", program.info.getProgramVersion());
			outh.line();
			
			boolean hasError = false;
			for(Decl decl: program.declList) {
				if (decl instanceof DeclConst) {
					DeclConst declConst = (DeclConst)decl;
					Type concreteType = declConst.type.getConcreteType();
					if (concreteType.kind == Type.Kind.ERROR) {
						hasError = true;
						break;
					}
				}
			}
			
			// include courier header
			outh.line("#include \"../courier/Courier.h\"");
			outh.line("#include \"../courier/StreamOf.h\""); // for StreaOf<T>
			if (hasError) outh.line("#include \"../courier/Service.h\"");  // for Service::Abort
			
			// include depend module
			if (!program.depends.isEmpty()) {
				outh.line();
				for(Program.Info info: program.depends) {
					outh.format("#include \"../stub/%s.h\"", info.getProgramVersion());
				}
			}

			// output namespace
			outh.line();
			outh.format("namespace %s {", "Courier");
			outh.format("namespace %s {", "Stub");

			String namespace = "Courier::Stub";
			String namePrefix = String.format("%s::%s", namespace, programName);
			
			// output main namespace
			outh.format("namespace %s {", program.info.getProgramVersion());
			if (program.info.version != 0) {
				outh.line();
				outh.format("constexpr const char*   PROGRAM_NAME = \"%s\";", program.info.name);
				outh.format("constexpr const quint32 PROGRAM_CODE = %d;", program.info.program);
				outh.format("constexpr const quint32 VERSION_CODE = %d;", program.info.version);
			}

			// for outc
			if (SHOW_BUILD_TIME) {
				outc.line("//",
						String.format("// File  %s.cpp", programName),
						String.format("// Build %s",     BUILD_TIME),
						"//",
						"");
			}
			outc.line("#include \"../util/Util.h\"");
			outc.format("static log4cpp::Category& logger = Logger::getLogger(\"stub/%s\");", program.info.getProgramVersion());
			outc.line();
			outc.format("#include \"../stub/%s.h\"", program.info.getProgramVersion());
			outc.line();
			outc.line("#include \"../courier/Last.h\"");
			outc.line();
			
			// generate type and constant declaration
			// and build enmInfoList, context.recordInfoList and context.choiceInfoList
			genDecl(outh, outc, namePrefix);
			
			// Close program namespace
			outh.line("}");

			// Close Stub namespace
			outh.line("}");

			// generate toString for enum using context.enumInfoList
			genEnumToString   (outh, outc);
			genEnumSerialize  (outh, outc);
			genEnumDeserialize(outh, outc);
			
			// generate toString for Record using context.recordInfoList
			genRecordToString   (outh, outc);
			// generate serialize for Record
			genRecordSerialize  (outh, outc);
			// generate deserialize for Record
			genRecordDeserialize(outh, outc);
			
			// generate toString for Choice using context.choiceInfoList
			genChoiceToString   (outh, outc);
			// generate serialize for Choice
			genChoiceSerialize  (outh, outc);
			// generate deserialize for Choice
			genChoiceDeserialize(outh, outc);

			// generate toString for Sequence using context.sequenceInfoList
			genSequenceToString   (outh, outc);
			// generate serialize for Record
			genSequenceSerialize  (outh, outc);
			// generate deserialize for Record
			genSequenceDeserialize(outh, outc);
			
			// generate toString for Array using context.arrayInfoList
			genArrayToString   (outh, outc);
			// generate serialize for Record
			genArraySerialize  (outh, outc);
			// generate deserialize for Record
			genArrayDeserialize(outh, outc);
			
			// generate toString for Machine using context.machineInfoList
			genMachineToString   (outh, outc);
			// generate serialize for Record
			genMachineSerialize  (outh, outc);
			// generate deserialize for Record
			genMachineDeserialize(outh, outc);
			
			// Close Courier namespace
			outh.line("}");
			
			// #endif
			outh.line("#endif");
			
			// Sort procedureInfoList and errorInfoList by code
			Collections.sort(context.procedureInfoList, (a, b) -> (int)(a.code - b.code));
			Collections.sort(context.errorInfoList, (a, b) -> (int)(a.code - b.code));
		} catch (FileNotFoundException e) {
			throw new CompilerException("FileNotFoundException", e);
		}
	}
}
