package mesa.courier.compiler;

import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.ArrayList;
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
import mesa.courier.program.TypeArray;
import mesa.courier.program.TypeChoice;
import mesa.courier.program.TypeChoice.Candidate;
import mesa.courier.program.TypeEnum;
import mesa.courier.program.TypeError;
import mesa.courier.program.TypeProcedure;
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
			return String.format("ARRAY<%s,%d>", toTypeString(typeArray.type), typeArray.size);
		}
		case SEQUENCE:
		{
			TypeSequence typeSequence = (TypeSequence)type;
			if (typeSequence.size == TypeSequence.MAX_SIZE) {
				return String.format("SEQUENCE<%s>", toTypeString(typeSequence.type));
			} else {
				return String.format("SEQUENCE<%s,%d>", toTypeString(typeSequence.type), typeSequence.size);
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
	
	private void genDeclType(LinePrinter outh, LinePrinter outc, String namePrefix, DeclType declType,
			List<EnumInfo> enumInfoList, List<RecordInfo> recordInfoList, List<ChoiceInfo> choiceInfoList) {
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
			outh.format("using %s = %s;", name, type);
			break;
		// constructed
		case ENUM:
			genTypeDeclEnum(outh, outc, (TypeEnum)type, name, namePrefix, enumInfoList, recordInfoList, choiceInfoList);
			break;
		case ARRAY:
			outh.format("using %s = %s;", name, toTypeString(type));
			break;
		case SEQUENCE:
			outh.format("using %s = %s;", name, toTypeString(type));
			break;
		case RECORD:
			genTypeDeclRecord(outh, outc, (TypeRecord)type, name, namePrefix, enumInfoList, recordInfoList, choiceInfoList);
			break;
		case CHOICE:
			genTypeDeclChoice(outh, outc, (TypeChoice)type, name, namePrefix, enumInfoList, recordInfoList, choiceInfoList);
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
	private void genDeclConstError(LinePrinter outh, LinePrinter outc, String namePrefix, TypeError typeError, String name, Constant constant,
			List<EnumInfo> enumInfoList, List<RecordInfo> recordInfoList, List<ChoiceInfo> choiceInfoList) {
		//
		// Output declaration of class
		//
		List<String> c1 = new ArrayList<>();
		List<String> c2 = new ArrayList<>();
		for(Field field: typeError.paramList) {
			c1.add(toTypeString(field.type));
			c2.add(String.format("%s;", Util.sanitizeSymbol(field.name)));
		}
		
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
		outh.format("static const quint16 CODE = %d;", code);
		outh.line();
		for(String line: ColumnLayout.layoutStringString(c1, c2)) {
			outh.line(line);
		}
		
		outh.line("};");
		
		// Build recordInfo
		{
			TypeRecord typeRecord = new TypeRecord();
			for(Field field: typeError.paramList) {
				typeRecord.addField(field);
			}
			recordInfoList.add(new RecordInfo(typeRecord, name, namePrefix));
		}
	}
	private void genDeclConstProcedure(LinePrinter outh, LinePrinter outc, String namePrefix, TypeProcedure typeProcedure, String name, Constant constant,
			List<EnumInfo> enumInfoList, List<RecordInfo> recordInfoList, List<ChoiceInfo> choiceInfoList) {
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
		outh.line("struct Param {");
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			for(Field field: typeProcedure.paramList) {
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
				default:
					fieldType = toTypeString(field.type);
					fieldName = field.name;
					break;
				}
				
				c1.add(fieldType);
				c2.add(String.format("%s;", Util.sanitizeSymbol(fieldName)));
			}
			for(String line: ColumnLayout.layoutStringString(c1, c2)) {
				outh.line(line);
			}
		}
		outh.line("};");
		
		outh.line("struct Result {");
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			for(Field field: typeProcedure.resultList) {
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
				default:
					fieldType = toTypeString(field.type);
					fieldName = field.name;
					break;
				}
				
				c1.add(fieldType);
				c2.add(String.format("%s;", Util.sanitizeSymbol(fieldName)));
			}
			for(String line: ColumnLayout.layoutStringString(c1, c2)) {
				outh.line(line);
			}
		}
		outh.line("};");
		
		outh.line();
		outh.format("static const quint16 CODE = %d;", code);
		outh.line();
		outh.format("void (*call)(Param& param, Result& result) throw (%s);", String.join(", ", typeProcedure.errroList));
		outh.line("};");
		
		// Build recordInfo
		{
			TypeRecord typeRecord = new TypeRecord();
			for(Field field: typeProcedure.paramList) {
				typeRecord.addField(field);
			}
			recordInfoList.add(new RecordInfo(typeRecord, "Param", String.format("%s::%s", namePrefix, name)));
		}
		{
			TypeRecord typeRecord = new TypeRecord();
			for(Field field: typeProcedure.resultList) {
				typeRecord.addField(field);
			}
			recordInfoList.add(new RecordInfo(typeRecord, "Result", String.format("%s::%s", namePrefix, name)));
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
	private void genDeclConst(LinePrinter outh, LinePrinter outc, String namePrefix, DeclConst declConst,
			List<EnumInfo> enumInfoList, List<RecordInfo> recordInfoList, List<ChoiceInfo> choiceInfoList) {
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
				outh.format("%s %s = %s::%s;", typeString, name, typeString, constString);
			} else {
				throw new CompilerException(String.format("Unexpected type %s", type.toString()));
			}
		}
			break;
		case ERROR:
			genDeclConstError(outh, outc, namePrefix, (TypeError)type, name, constant, enumInfoList, recordInfoList, choiceInfoList);
			break;
		case PROCEDURE:
			genDeclConstProcedure(outh, outc, namePrefix, (TypeProcedure)type, name, constant, enumInfoList, recordInfoList, choiceInfoList);
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
	private void genDecl(LinePrinter outh, LinePrinter outc, String namePrefix,
			List<EnumInfo> enumInfoList, List<RecordInfo> recordInfoList, List<ChoiceInfo> choiceInfoList) {
		outh.line("",
				  "//",
				  "// Declaration",
				  "//");

		for(Decl decl: program.declList) {
			outh.line();
			logDecl(outh, decl);
			
			switch(decl.kind) {
			case TYPE:
				genDeclType(outh, outc, namePrefix, (DeclType)decl, enumInfoList, recordInfoList, choiceInfoList);
				break;
			case CONST:
				genDeclConst(outh, outc, namePrefix, (DeclConst)decl, enumInfoList, recordInfoList, choiceInfoList);
				break;
			default:
				throw new CompilerException(String.format("Unexpected decl.kind %s", decl.kind));
			}
		}
	}

	private void logDecl(LinePrinter out, Decl decl) {
		Type   type         = decl.type;
		String name         = decl.name;
		Type   concreteType = type.getConcreteType();

		out.format("// %s  %s", decl.kind, name);
		out.format("//   %s", type);
		if (type.isReference()) {
			out.format("//   %s", concreteType);
		}
		if (decl.isConstant()) {
			Constant constant = decl.getConstant();
			out.format("//   %s", constant);
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

	private void genTypeDeclRecord(LinePrinter outh, LinePrinter outc, TypeRecord typeRecord, String name, String namePrefix,
			List<EnumInfo> enumInfoList, List<RecordInfo> recordInfoList, List<ChoiceInfo> choiceInfoList) {
		//
		// Output declaration of class
		//
		List<String> c1 = new ArrayList<>();
		List<String> c2 = new ArrayList<>();
		for(Field field: typeRecord.fields) {
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
		outh.line("};");
		
		// build recordInfoList
		{
			recordInfoList.add(new RecordInfo(typeRecord, name, namePrefix));
		}
	}
	
	private void genTypeDeclEnum(LinePrinter outh, LinePrinter outc, TypeEnum type, String name, String namePrefix,
			List<EnumInfo> enumInfoList, List<RecordInfo> recordInfoList, List<ChoiceInfo> choiceInfoList) {
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
		
		// build enumInfoList
		enumInfoList.add(new EnumInfo((TypeEnum)type, name, namePrefix));
	}
	
	private void genTypeDeclChoice(LinePrinter outh, LinePrinter outc, TypeChoice typeChoice, String name, String namePrefix,
			List<EnumInfo> enumInfoList, List<RecordInfo> recordInfoList, List<ChoiceInfo> choiceInfoList) {
		if (typeChoice instanceof TypeChoice.Typed) {
			genTypeDeclChoiceTyped(outh, outc, (TypeChoice.Typed)typeChoice, name, namePrefix, enumInfoList, recordInfoList, choiceInfoList);
		} else if (typeChoice instanceof TypeChoice.Anon) {
			genTypeDeclChoiceAnon(outh, outc, (TypeChoice.Anon)typeChoice, name, namePrefix, enumInfoList, recordInfoList, choiceInfoList);
		} else {
			throw new CompilerException(String.format("Unexpected typeChoice %s", typeChoice.toString()));
		}
	}
	private void genTypeDeclChoiceTyped(LinePrinter outh, LinePrinter outc, TypeChoice.Typed typed, String name, String namePrefix,
			List<EnumInfo> enumInfoList, List<RecordInfo> recordInfoList, List<ChoiceInfo> choiceInfoList) {
		outh.format("struct %s {", name);
		
		List<String>         choiceNameList  = new ArrayList<>(); // choice name list in appearance order
		Map<String, Integer> choiceMap       = new TreeMap<>();   // choice name => struct number
		int                  maxChoiceNumber = 0;
		for(Candidate<String> candidate: typed.candidates) {
			maxChoiceNumber++;
			Type candidateType = candidate.type;
			
			// build choiceMap
			for(String id: candidate.designators) {
				String choiceName = id;
				choiceNameList.add(choiceName);
				choiceMap.put(choiceName, maxChoiceNumber);
			}
			
			// generate choice struct
			if (candidateType.kind == Type.Kind.RECORD) {
				TypeRecord typeRecord = (TypeRecord)candidateType;
				String structName = String.format("CHOICE_%02d", maxChoiceNumber);
				genTypeDeclRecord(outh, outc, typeRecord, structName, String.format("%s::%s", namePrefix, name), enumInfoList, recordInfoList, choiceInfoList);
			} else {
				throw new CompilerException(String.format("Unexpected candidateType %s", candidateType.toString()));
			}
		}
		
		outh.line();
		outh.format("using CHOICE_TAG = %s;", toTypeString(typed.type));
		outh.line("CHOICE_TAG choiceTag;");
		outh.line();
		
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(String choiceName: choiceNameList) {
				int structNumber = choiceMap.get(choiceName);
				c1.add(String.format("CHOICE_%02d& %s()", structNumber, Util.sanitizeSymbol(choiceName)));
				c2.add("const;");
			}

			for(String line: ColumnLayout.layoutStringString(c1, c2)) {
				outh.line(line);
			}
		}

		outh.line("private:");
		for(int i = 1; i <= maxChoiceNumber; i++) {
			outh.format("mutable CHOICE_%02d  choice_%02d;", i, i);
		}
		
		outh.line("};");
		
		for(String choiceName: choiceNameList) {
			int structNumber = choiceMap.get(choiceName);
			outc.line();
			outc.format("%s::%s::CHOICE_%02d& %s::%s::%s() const {", namePrefix, name, structNumber, namePrefix, name, Util.sanitizeSymbol(choiceName));
			outc.format("if (choiceTag == CHOICE_TAG::%s) {", Util.sanitizeSymbol(choiceName));
			outc.format("return choice_%02d;", structNumber);
			outc.line("} else {");
			outc.format("logger.error(\"choiceTag  expect %s  actual %%s\", Courier::toString(choiceTag));", choiceName);
			outc.line("COURIER_ERROR();");
			outc.line("}");
			outc.line("}");
		}
		
		// Build choideInfoList
		{
			choiceInfoList.add(new ChoiceInfo(typed, name, namePrefix, choiceNameList));
		}
	}
	private void genTypeDeclChoiceAnon(LinePrinter outh, LinePrinter outc, TypeChoice.Anon anon, String name, String namePrefix,
			List<EnumInfo> enumInfoList, List<RecordInfo> recordInfoList, List<ChoiceInfo> choiceInfoList) {
		outh.format("struct %s {", name);
		
		List<String>         choiceNameList  = new ArrayList<>(); // choice name list in appearance order
		Map<String, Integer> choiceMap       = new TreeMap<>();   // choice name => struct number
		int                  maxChoiceNumber = 0;

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
			for(String line: ColumnLayout.layoutEnumElement(c1, c2)) {
				outh.line(line);
			}
			outh.line("};");
		}
		
		for(Candidate<Correspondence> candidate: anon.candidates) {
			maxChoiceNumber++;
			Type candidateType = candidate.type;
			
			// build choiceMap
			for(Correspondence correspondence: candidate.designators) {
				String choiceName = correspondence.id;
				choiceNameList.add(choiceName);
				choiceMap.put(choiceName, maxChoiceNumber);
			}
			
			// generate choice struct
			if (candidateType.kind == Type.Kind.RECORD) {
				TypeRecord typeRecord = (TypeRecord)candidateType;
				String structName = String.format("CHOICE_%02d", maxChoiceNumber);
				genTypeDeclRecord(outh, outc, typeRecord, structName, String.format("%s::%s", namePrefix, name), enumInfoList, recordInfoList, choiceInfoList);
			} else {
				throw new CompilerException(String.format("Unexpected candidateType %s", candidateType.toString()));
			}
		}
		
		outh.line();
		outh.line("CHOICE_TAG choiceTag;");
		outh.line();
		
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(String choiceName: choiceNameList) {
				int structNumber = choiceMap.get(choiceName);
				c1.add(String.format("CHOICE_%02d& %s()", structNumber, Util.sanitizeSymbol(choiceName)));
				c2.add("const;");
			}

			for(String line: ColumnLayout.layoutStringString(c1, c2)) {
				outh.line(line);
			}
		}

		outh.line("private:");
		for(int i = 1; i <= maxChoiceNumber; i++) {
			outh.format("mutable CHOICE_%02d  choice_%02d;", i, i);
		}
		
		outh.line("};");
		
		for(String choiceName: choiceNameList) {
			int structNumber = choiceMap.get(choiceName);
			outc.line();
			outc.format("%s::%s::CHOICE_%02d& %s::%s::%s() const {", namePrefix, name, structNumber, namePrefix, name, Util.sanitizeSymbol(choiceName));
			outc.format("if (choiceTag == CHOICE_TAG::%s) {", Util.sanitizeSymbol(choiceName));
			outc.format("return choice_%02d;", structNumber);
			outc.line("} else {");
			outc.format("logger.error(\"choiceTag  expect %s  actual %%s\", Courier::toString(choiceTag));", choiceName);
			outc.line("COURIER_ERROR();");
			outc.line("}");
			outc.line("}");
		}
		
		// Build enumInfoList
		{
			// Build typeEnum from anon
			TypeEnum typeEnum = new TypeEnum();
			for(Candidate<Correspondence> candidate: anon.candidates) {
				for(Correspondence correspondence: candidate.designators) {
					typeEnum.addCorrespondence(correspondence);
				}
			}
			
			enumInfoList.add(new EnumInfo(typeEnum, "CHOICE_TAG", String.format("%s::%s", namePrefix, name)));
		}
		
		// Build choideInfoList
		{
			choiceInfoList.add(new ChoiceInfo(anon, name, namePrefix, choiceNameList));
		}
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
	
	private void genEnumToString(LinePrinter outh, LinePrinter outc, List<EnumInfo> enumInfoList) {
		if (enumInfoList.isEmpty()) return;
		
		// Declaration
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(EnumInfo enumInfo: enumInfoList) {				
				c1.add(String.format("QString toString(const %s::%s", enumInfo.namePrefix, enumInfo.name));
				c2.add("value);");
			}
			
			outh.line("",
					  "//",
					  "// Enum toString Function Declaration",
					  "//");
			for(String line: ColumnLayout.layoutStringString(c1, c2)) {
				outh.line(line);
			}
		}
		
		// Definition
		{
			outc.line("",
					  "//",
					  "// Enum toString Function Definition",
					  "//");

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
	}
	private void genEnumSerialize(LinePrinter outh, LinePrinter outc, List<EnumInfo> enumInfoList) {
		if (enumInfoList.isEmpty()) return;
		
		// Declaration
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(EnumInfo enumInfo: enumInfoList) {				
				c1.add(String.format("void serialize(BLOCK& block, const %s::%s", enumInfo.namePrefix, enumInfo.name));
				c2.add("value);");
			}
			
			outh.line("",
					  "//",
					  "// Enum serialize Function Declaration",
					  "//");
			for(String line: ColumnLayout.layoutStringString(c1, c2)) {
				outh.line(line);
			}
		}
		
		// Definition
		{
			outc.line("",
					  "//",
					  "// Enum serialize Function Definition",
					  "//");
			for(EnumInfo enumInfo: enumInfoList) {							
				outc.line();
				outc.format("void %s::serialize(BLOCK& block, const %s::%s value) {", "Courier", enumInfo.namePrefix, enumInfo.name);
				outc.line("Courier::serialize(block, (quint16)value);");
				outc.line("}");
			}
		}
	}
	private void genEnumDeserialize(LinePrinter outh, LinePrinter outc, List<EnumInfo> enumInfoList) {
		if (enumInfoList.isEmpty()) return;
		
		// Declaration
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(EnumInfo enumInfo: enumInfoList) {				
				c1.add(String.format("void deserialize(BLOCK& block, %s::%s&", enumInfo.namePrefix, enumInfo.name));
				c2.add("value);");
			}
			
			outh.line("",
					  "//",
					  "// Enum deserialize Function Declaration",
					  "//");
			for(String line: ColumnLayout.layoutStringString(c1, c2)) {
				outh.line(line);
			}
		}
		
		// Definition
		{
			outc.line("",
					  "//",
					  "// Enum deserialize Function Definition",
					  "//");
			for(EnumInfo enumInfo: enumInfoList) {							
				outc.line();
				outc.format("void %s::deserialize(BLOCK& block, %s::%s& value) {", "Courier", enumInfo.namePrefix, enumInfo.name);
				outc.line("quint16 t;");
				outc.line("Courier::deserialize(block, t);");
				outc.format("value = (%s::%s)t;", enumInfo.namePrefix, enumInfo.name);
				outc.line("}");
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
	private void genRecordToString(LinePrinter outh, LinePrinter outc, List<RecordInfo> recordInfoList) {
		if (recordInfoList.isEmpty()) return;
		
		// Record toSring declaration
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(RecordInfo recordInfo: recordInfoList) {				
				c1.add(String.format("QString toString(const %s::%s&", recordInfo.namePrefix, recordInfo.name));
				c2.add("value);");
			}
			
			outh.line("",
					  "//",
					  "// Record toString Function Declaration",
					  "//");
			for(String line: ColumnLayout.layoutStringString(c1, c2)) {
				outh.line(line);
			}
		}
		
		// Record toString definition
		{
			outc.line("",
					  "//",
					  "// Record toSring Function Definition",
					  "//");
			for(RecordInfo recordInfo: recordInfoList) {
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
	}
	
	void genRecordSerialize(LinePrinter outh, LinePrinter outc, List<RecordInfo> recordInfoList) {
		if (recordInfoList.size() == 0) return;
		
		// Declaration
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(RecordInfo recordInfo: recordInfoList) {
				c1.add(String.format("void serialize(BLOCK& block, const %s::%s&", recordInfo.namePrefix, recordInfo.name));
				c2.add("value);");
			}
			
			outh.line("",
					  "//",
					  "// Record serialize method Declaration",
					  "//");
			for(String line: ColumnLayout.layoutStringString(c1, c2)) {
				outh.line(line);
			}
		}
		
		// Definition
		{
			outc.line("",
					  "//",
					  "// Record serialize Function Definition",
					  "//");
			for(RecordInfo recordInfo: recordInfoList) {
				if (recordInfo.typeRecord.fields.isEmpty()) {
					outc.format("void %s::serialize(BLOCK&, const %s::%s&) {", "Courier", recordInfo.namePrefix, recordInfo.name);
					outc.line("}");
				} else {
					outc.format("void %s::serialize(BLOCK& block, const %s::%s& value) {", "Courier", recordInfo.namePrefix, recordInfo.name);
					for(Field field: recordInfo.typeRecord.fields) {
						logField(outc, field.type, field.name);
						outc.format("Courier::serialize(block, value.%s);", field.name);
					}
					outc.line("}");
				}
			}
		}
	}
	void genRecordDeserialize(LinePrinter outh, LinePrinter outc, List<RecordInfo> recordInfoList) {
		if (recordInfoList.size() == 0) return;
		
		// Declaration
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(RecordInfo recordInfo: recordInfoList) {
				c1.add(String.format("void deserialize(BLOCK& block, %s::%s&", recordInfo.namePrefix, recordInfo.name));
				c2.add("value);");
			}
			
			outh.line("",
					  "//",
					  "// Record deserialize method Declaration",
					  "//");
			for(String line: ColumnLayout.layoutStringString(c1, c2)) {
				outh.line(line);
			}
		}
		
		// Definition
		{
			outc.line("",
					  "//",
					  "// Record deserialize Function Definition",
					  "//");
			for(RecordInfo recordInfo: recordInfoList) {
				if (recordInfo.typeRecord.fields.isEmpty()) {
					outc.format("void %s::deserialize(BLOCK&, %s::%s&) {", "Courier", recordInfo.namePrefix, recordInfo.name);
					outc.line("}");
				} else {
					outc.format("void %s::deserialize(BLOCK& block, %s::%s& value) {", "Courier", recordInfo.namePrefix, recordInfo.name);
					for(Field field: recordInfo.typeRecord.fields) {
						logField(outc, field.type, field.name);
						outc.format("Courier::deserialize(block, value.%s);", field.name);
					}
					outc.line("}");
				}
			}
		}
	}
	
	class ChoiceInfo {
		final TypeChoice           typeChoice;
		final String               name;
		final String               namePrefix;
		final List<String>         choiceNameList; // choice name list in appearance order

		ChoiceInfo(TypeChoice typeChoice, String name, String namePrefix, List<String> choiceNameList) {
			this.typeChoice     = typeChoice;
			this.name           = name;
			this.namePrefix     = namePrefix;
			this.choiceNameList = choiceNameList;
		}
	}
	
	private void genChoiceToString(LinePrinter outh, LinePrinter outc, List<ChoiceInfo> choiceInfoList) {
		if (choiceInfoList.isEmpty()) return;
		
		// Choice toSring declaration
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(ChoiceInfo choiceInfo: choiceInfoList) {				
				c1.add(String.format("QString toString(const %s::%s&", choiceInfo.namePrefix, choiceInfo.name));
				c2.add("value);");
			}
			
			outh.line("",
					  "//",
					  "// Choice toString Function Declaration",
					  "//");
			for(String line: ColumnLayout.layoutStringString(c1, c2)) {
				outh.line(line);
			}
		}
		
		// Choice toString definition
		{
			outc.line("",
					  "//",
					  "// Choice toSring Function Definition",
					  "//");

			for(ChoiceInfo choiceInfo: choiceInfoList) {
				outc.format("QString %s::toString(const %s::%s& value) {", "Courier", choiceInfo.namePrefix, choiceInfo.name);
				outc.line("switch(value.choiceTag) {");
				
				for(String choiceName: choiceInfo.choiceNameList) {
					outc.format("case %s::%s::CHOICE_TAG::%s:", choiceInfo.namePrefix, choiceInfo.name, Util.sanitizeSymbol(choiceName));
					outc.format("return QString(\"[%%1 %%2]\").arg(\"%s\").arg(Courier::toString(value.%s()));", choiceName, Util.sanitizeSymbol(choiceName));
				}

				outc.line("default:");
				outc.line("logger.error(\"Unexpected choiceTag = %%d\", (quint16)value.choiceTag);");
		        outc.line("COURIER_ERROR();");
		        outc.line("break;");
				outc.line("}"); // end of switch

				outc.line("}"); // end of method
			}
		}
	}

	private void genChoiceSerialize(LinePrinter outh, LinePrinter outc, List<ChoiceInfo> choiceInfoList) {
		if (choiceInfoList.isEmpty()) return;
		
		// Declaration
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(ChoiceInfo choiceInfo: choiceInfoList) {
				c1.add(String.format("void serialize(BLOCK& block, const %s::%s&", choiceInfo.namePrefix, choiceInfo.name));
				c2.add("value);");
			}
			
			outh.line("",
					  "//",
					  "// Choice serialize Function Declaration",
					  "//");
			for(String line: ColumnLayout.layoutStringString(c1, c2)) {
				outh.line(line);
			}
		}
		
		// Definition
		{
			outc.line("",
					  "//",
					  "// Choice serialize Function Definition",
					  "//");
			for(ChoiceInfo choiceInfo: choiceInfoList) {
				outc.format("void %s::serialize(BLOCK& block, const %s::%s& value) {", "Courier", choiceInfo.namePrefix, choiceInfo.name);
		        outc.line("block.serialize16((quint16)value.choiceTag);");
				outc.line("switch(value.choiceTag) {");
				
				for(String choiceName: choiceInfo.choiceNameList) {
					outc.format("case %s::%s::CHOICE_TAG::%s:", choiceInfo.namePrefix, choiceInfo.name, Util.sanitizeSymbol(choiceName));
					outc.format("Courier::serialize(block, value.%s());", Util.sanitizeSymbol(choiceName));
					outc.line("break;");
				}
	
				outc.line("default:");
				outc.line("logger.error(\"Unexpected choiceTag = %%d\", (quint16)value.choiceTag);");
		        outc.line("COURIER_ERROR();");
		        outc.line("break;");
				outc.line("}"); // end of switch
	
				outc.line("}"); // end of method
			}
		}
	}

	private void genChoiceDeserialize(LinePrinter outh, LinePrinter outc, List<ChoiceInfo> choiceInfoList) {
		if (choiceInfoList.isEmpty()) return;
		
		// Declaration
		{
			List<String> c1 = new ArrayList<>();
			List<String> c2 = new ArrayList<>();
			
			for(ChoiceInfo choiceInfo: choiceInfoList) {
				c1.add(String.format("void deserialize(BLOCK& block, %s::%s&", choiceInfo.namePrefix, choiceInfo.name));
				c2.add("value);");
			}
			
			outh.line("",
					  "//",
					  "// Choice deserialize Function Declaration",
					  "//");
			for(String line: ColumnLayout.layoutStringString(c1, c2)) {
				outh.line(line);
			}
		}
		
		// Definition
		{
			outc.line("",
					  "//",
					  "// Choice deserialize Function Definition",
					  "//");
			for(ChoiceInfo choiceInfo: choiceInfoList) {
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
				outc.line("logger.error(\"Unexpected choiceTag = %%d\", (quint16)value.choiceTag);");
		        outc.line("COURIER_ERROR();");
		        outc.line("break;");
				outc.line("}"); // end of switch
	
				outc.line("}"); // end of method
			}
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
			} else if ((value & 0xFFFF) == 0) {
				return String.format((value <= 0x7FFF_FFF) ? "0x%X" : "0x%XU", value);
			} else {
				return String.format((value <= 0x7FFF_FFF) ? "%d" : "%dU", value);
			}
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
			outc.line("#include \"../util/Util.h\"");
			outc.format("static log4cpp::Category& logger = Logger::getLogger(\"stub/%s\");", program.info.getProgramVersion());
			outc.line();
			outc.format("#include \"../stub/%s.h\"", program.info.getProgramVersion());
			outc.line();
			
//			List<EnumInfo>   enumInfoList   = getEnumInfoList(namePrefix);
//			List<RecordInfo> recordInfoList = getRecordInfo(namePrefix);
//			List<ChoiceInfo> choiceInfoList = getChoiceInfo(namePrefix);
			List<EnumInfo>   enumInfoList   = new ArrayList<>();
			List<RecordInfo> recordInfoList = new ArrayList<>();
			List<ChoiceInfo> choiceInfoList = new ArrayList<>();

			// generate type and constant declaration
			// and build enmInfoList, recordInfoList and choiceInfoList
			genDecl(outh, outc, namePrefix, enumInfoList, recordInfoList, choiceInfoList);
			
			// Close program namespace
			outh.line("}");

			// generate toString for enum using enumInfoList
			genEnumToString   (outh, outc, enumInfoList);
			genEnumSerialize  (outh, outc, enumInfoList);
			genEnumDeserialize(outh, outc, enumInfoList);
			
			// generate toString for Record using recordInfoList
			genRecordToString   (outh, outc, recordInfoList);
			// generate serialize for Record
			genRecordSerialize  (outh, outc, recordInfoList);
			// generate deserialize for Record
			genRecordDeserialize(outh, outc, recordInfoList);
			
			// generate toString for Choice using choiceInfoList
			genChoiceToString   (outh, outc, choiceInfoList);
			// generate serialize for Choice
			genChoiceSerialize  (outh, outc, choiceInfoList);
			// generate deserialize for Choice
			genChoiceDeserialize(outh, outc, choiceInfoList);
			
			// Close courier namespace
			outh.line("}");
			
			// #endif
			outh.line("#endif");
			
		} catch (FileNotFoundException e) {
			throw new CompilerException("FileNotFoundException", e);
		}
	}
}
