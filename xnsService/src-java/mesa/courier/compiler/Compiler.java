package mesa.courier.compiler;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.List;

import mesa.courier.compiler.Type.Correspondence;
import mesa.courier.compiler.Type.Field;
import mesa.courier.compiler.TypeChoice.Candidate;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class Compiler {
	protected static final Logger logger = LoggerFactory.getLogger(Compiler.class);

	public static String STUB_DIR_PATH = "src/courier/stub";

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

	private final Program     program;
	public Compiler(Program program) {
		this.program = program;
	}

	public void genStub() {
		String pathc = String.format("%s%c%s.cpp", STUB_DIR_PATH, File.separatorChar, program.info.getProgramVersion());
		String pathh = String.format("%s%c%s.h",   STUB_DIR_PATH, File.separatorChar, program.info.getProgramVersion());
		logger.info(String.format("pathc = %s", pathc));
		logger.info(String.format("pathh = %s", pathh));
		
		try (
				IndentPrintWriter outc = new IndentPrintWriter(new PrintWriter(pathc));
				IndentPrintWriter outh = new IndentPrintWriter(new PrintWriter(pathh));) {
			// output pacage
			outh.indent().format("#ifndef COURIER_%s", program.info.getProgramVersion()).println();
			outh.indent().format("#define COURIER_%s", program.info.getProgramVersion()).println();
			outh.indent().println();

			// include courier header
			outh.indent().println("#include \"../Courier.h\"");
			outh.indent().println();

			// output namespace
			outh.indent().println("namespace Courier {");
			outh.nest();

			// output main class
			outh.indent().format("namespace %s {", program.info.getProgramVersion()).println();
			outh.nest();
			if (program.info.version != 0) {
				outh.indent().format("static const quint32 PROGRAM_NUMBER = %d;", program.info.program).println();
				outh.indent().format("static const quint32 VERSION_NUMBER = %d;", program.info.version).println();
				outh.indent().println();
			}

			// output type
			for(Program.DeclType declType: program.typeList) {
				String name = declType.name;
				Type   type = declType.type;

				switch (type.kind) {
				case BOOLEAN:
				case CARDINAL:
				case LONG_CARDINAL:
				case STRING:
				case UNSPECIFIED:
				case UNSPECIFIED2:
				case UNSPECIFIED3:
					// we will expand reference to predefined type.
					// We will ignore these definition. Because we don't generate class these definition.
					outh.indent().format("// IGNORE predefined type.  %s: TYPE = %s", name, type.kind).println();
					break;
				case ENUM:
					// generate enum class definition
					genDefEnum(outh, name, (TypeEnum)type);
					break;
				case ARRAY:
					// we will expand reference to array type.
					// We will ignore these definition. Because we don't generate class these definition.
					outh.indent().format("// IGNORE top level ARRAY type.  %s", name).println();
					break;
				case SEQUENCE:
					// we will expand reference to sequence type.
					// We will ignore these definition. Because we don't generate class these definition.
					outh.indent().format("// IGNORE top level SEQUENCE type.  %s", name).println();
					break;
				case RECORD:
					// generate record class definition
					genDefRecord(outh, name, (TypeRecord)type);
					break;
				case CHOICE:
					// generate choice class definition
					genDefChoice(outh, name, (TypeChoice)type);
					break;
				case PROCEDURE:
					// generate procedure class definition
					genDefProcedure(outh, name, (TypeProcedure)type);
					break;
				case ERROR:
					// generate error class definition
					genDefError(outh, name, (TypeError)type);
					break;
				case REFERENCE:
					outh.indent().format("// IGNORE reference.  %s: TYPE = %s", name, ((TypeReference)type).value.toString()).println();
					break;
				default:
					throw new CompilerException(String.format("Unknown type = %s", type));
				}
			}


			// output const
			for(Program.DeclConst declConst: program.constList) {
				String   name     = declConst.name;
				Type     type     = declConst.type;
				Constant constant = declConst.constant;
				Type     baseType = type.getBaseType();
				switch(baseType.kind) {
				case BOOLEAN:
				case CARDINAL:
				case LONG_CARDINAL:
				case STRING:
				case UNSPECIFIED:
				case UNSPECIFIED2:
				case UNSPECIFIED3:
					if (type.kind == Type.Kind.REFERENCE) outh.indent().format("// EXPAND %s as %s", ((TypeReference)type).value, baseType.kind).println();
					genConstPredefined(outh, name, (TypePredefined)baseType, constant);
					break;
				case PROCEDURE:
					genConstProcedure(outh, name, (TypeProcedure) baseType, constant);
					break;
				case ERROR:
					genConstError(outh, name, (TypeError) baseType, constant);
					break;
				case REFERENCE: {
					TypeReference refType = (TypeReference)baseType;
					Type concreteType = baseType.getConcreteType();
					switch(concreteType.kind) {
					case ENUM:
						genConstEnum(outh, name, refType, constant);
						break;
					case ARRAY:
						genConstArray(outh, name, refType, constant);
						break;
					case SEQUENCE:
						genConstSequence(outh, name, refType, constant);
						break;
					case RECORD:
						genConstRecord(outh, name, refType, constant);
						break;
					case CHOICE:
						genConstChoice(outh, name, refType, constant);
						break;
					default:
						throw new CompilerException(String.format("Unexpect baseType = %s  concreteType = %s", baseType, baseType.getConcreteType()));
					}
				}
					break;
				default:
					throw new CompilerException(String.format("Unexpect baseType = %s  concreteType = %s", baseType, baseType.getConcreteType()));
				}
			}

			// output include of header
			outc.indent().format("#include \"%s.h\"", program.info.getProgramVersion()).println();
			outc.indent().println();
			
			// close main class
			outh.unnest();
			outh.indent().println("};");

			//
			// ENUM
			//
			outh.indent().println("// getEnumMap");
			for(Program.DeclType declType: program.typeList) {
				String name = declType.name;
				Type   type = declType.type;

				switch (type.kind) {
				case ENUM:
					// generate record class definition
					//genDefRecord(outh, name, (TypeRecord)type);
					
					// output serialize
					outh.indent().format("const char* getEnumName(%s::%s value);", program.info.getProgramVersion(), name).println();
					
					
					outc.indent().println();
					
					
					outc.indent().format("static QMap<Courier::%s::%s, const char*>map%s = {", program.info.getProgramVersion(), name, name).println();
					outc.nest();
					//{Courier::Time2::PacketType::REQUEST, 1},
					
					{
						TypeEnum typeEnum = (TypeEnum)type;
						for(Type.Correspondence e: typeEnum.elements) {
							String enumName = Util.toJavaConstName(e.id);
							int value = (int)e.numericValue;
							outc.indent().format("{Courier::%s::%s::%s, %d},", program.info.getProgramVersion(), name, enumName, value).println();
						}
					}
					outc.unnest();
					outc.indent().println("};");
					
					outc.indent().format("const char* Courier::getEnumName(Courier::%s::%s value) {", program.info.getProgramVersion(), name).println();
					outc.nest();
					// TODO output implementation of deserialize to outc
					outc.indent().format("return map%s.value(value, 0);", name).println();
					outc.unnest();
					outc.indent().println("}");
					break;
				default:
					break;
				}
			}

			//
			// RECORD
			//
			outh.indent().println("// serialize and deserialize");
			for(Program.DeclType declType: program.typeList) {
				String name = declType.name;
				Type   type = declType.type;

				switch (type.kind) {
				case RECORD:
					// generate record class definition
					//genDefRecord(outh, name, (TypeRecord)type);
					
					// output serialize
					outh.indent().format("void serialize  (ByteBuffer* buffer, %s::%s* data);", program.info.getProgramVersion(), name).println();
					outh.indent().format("void deserialize(ByteBuffer* buffer, %s::%s* data);", program.info.getProgramVersion(), name).println();
					
					outc.indent().println();
					outc.indent().format("void Courier::serialize  (ByteBuffer* buffer, %s::%s* data) {", program.info.getProgramVersion(), name).println();
					outc.nest();
					// TODO output implementation of serialize to outc
					outc.indent().println("// TODO implements serialize");
					outc.unnest();
					outc.indent().println("}");
					
					outc.indent().println();
					outc.indent().format("void Courier::deserialize(ByteBuffer* buffer, %s::%s* data) {", program.info.getProgramVersion(), name).println();
					outc.nest();
					// TODO output implementation of deserialize to outc
					outc.indent().println("// TODO implements deserialize");
					outc.unnest();
					outc.indent().println("}");
					break;
				default:
					break;
				}
			}
			
			
			
			
			// close namespace
			outh.unnest();
			outh.indent().println("}");
			outh.indent().println("#endif");
			
			
		} catch (FileNotFoundException e) {
			throw new CompilerException("FileNotFoundException", e);
		}
	}

	// enum definition
	private void genDefEnum(IndentPrintWriter out, String name, TypeEnum type) {
		out.indent().format("enum class %s : quint16 {", name).println();
		out.nest();
			for(Type.Correspondence e: type.elements) {
				String enumName = Util.toJavaConstName(e.id);
				int value = (int)e.numericValue;
				out.indent().format("%s = %d,", enumName, value).println();
			}
		out.unnest();
		out.indent().println("};");
	}
	private void genDefArray(IndentPrintWriter out, String name, TypeArray type) {
		Type baseType = type.type.getBaseType();
		String javaTypeName;
		switch(baseType.kind) {
		case BOOLEAN:
			javaTypeName = "quint32";
			break;
		case CARDINAL:
			javaTypeName = "quint16";
			break;
		case LONG_CARDINAL:
			javaTypeName = "quint32";
			break;
		case STRING:
			javaTypeName = "quint8";
			break;
		case UNSPECIFIED:
			javaTypeName = "quint16";
			break;
		case REFERENCE:
			javaTypeName = program.getLocalRefName((TypeReference)baseType);
			break;
		default:
			throw new CompilerException(String.format("Unexpected type = %s", baseType));
		}

		out.indent().format("%s %s[%d];", javaTypeName, name, type.size).println();
	}
	private void genDefSequence(IndentPrintWriter out, String name, TypeSequence type) {
		Type baseType = type.type.getBaseType();
		String javaTypeName;
		switch(baseType.kind) {
		case BOOLEAN:
			javaTypeName = "quint32";
			break;
		case CARDINAL:
			javaTypeName = "quint16";
			break;
		case LONG_CARDINAL:
			javaTypeName = "quint32";
			break;
		case STRING:
			javaTypeName = "quint8";
			break;
		case UNSPECIFIED:
			javaTypeName = "quint16";
			break;
		case REFERENCE: {
			// courierTypeName can be RECORD, ENUM or CHOICE
			Type concreteType = baseType.getConcreteType();
			switch(concreteType.kind) {
			case RECORD:
			case ENUM:
			case CHOICE:
				break;
			default:
				throw new CompilerException(String.format("Unexpected concreteType = %s", concreteType));
			}
			javaTypeName = program.getLocalRefName((TypeReference)baseType);
		}
			break;
		default:
			throw new CompilerException(String.format("Unexpected type = %s", baseType));
		}

		out.indent().format("public %s* %s;", javaTypeName, name).println();
	}
	private void genDefRecord(IndentPrintWriter out, String name, TypeRecord type) {
		out.indent().format("struct %s {", name).println();
		out.nest();
		genFieldList(out, type.fields);
		out.unnest();
		out.indent().format("};").println();
	}
	private void genDefChoice(IndentPrintWriter out, String name, TypeChoice type) {
		if (type instanceof TypeChoice.Anon) {
			TypeChoice.Anon typeAnon = (TypeChoice.Anon)type;

			// class header
			out.indent().format("union %s {", name).println();
			out.nest();

			// output tag enum
			out.indent().format("enum Tag : quint16 {").println();
			out.nest();
			for(Candidate<Correspondence> candidate: typeAnon.candidates) {
				for(Correspondence correspondence: candidate.designators) {
					String enumName = Util.toJavaConstName(correspondence.id);
					int value = (int)correspondence.numericValue;
					out.indent().format("%s = %d,", enumName, value).println();
				}
			}
			out.unnest();
			out.indent().format("}").println();
			
			// output field
			out.indent().format("Tag tag;").println();

			// output sub record
			for(Candidate<Correspondence> candidate: typeAnon.candidates) {
				Type choiceType = candidate.type;
				switch(choiceType.kind) {
				case RECORD:
					// Output ChoiceValue
					for(Correspondence correspondence: candidate.designators) {
						String choiceName = correspondence.id;
						
						// TODO add union field name in struct
						genDefRecord(out, toClassName(choiceName), (TypeRecord)choiceType);
					}
					break;
				default:
					throw new CompilerException(String.format("Unexpected choiceType = %s", choiceType));
				}
			}
			out.indent().println("}");
		} else if (type instanceof TypeChoice.Typed) {
			TypeChoice.Typed typeTyped = (TypeChoice.Typed)type;

			// class header
			out.indent().format("union %s {", name).println();
			out.nest();

			{
				String tagJavaClassName;
				// Sanity check
				switch(typeTyped.type.kind) {
				case REFERENCE: {
					Type concreteType = typeTyped.type.getConcreteType();
					switch(concreteType.kind) {
					case ENUM: {
						TypeReference refType = (TypeReference)typeTyped.type;
						tagJavaClassName = program.getLocalRefName(refType.getLastReference());
					}
						break;
					default:
						throw new CompilerException(String.format("Unexpected concreteType = %s", concreteType));
					}
				}
					break;
				default:
					throw new CompilerException(String.format("Unexpected typeTyped.type = %s", typeTyped.type));
				}
			}
			
			// output sub record
			for(Candidate<String> candidate: typeTyped.candidates) {
				Type choiceType = candidate.type;
				switch(choiceType.kind) {
				case RECORD:
					// Output ChoiceValue
					for(String choiceName: candidate.designators) {
						// TODO append choice name as instance name of record
						genDefRecord(out, toClassName(choiceName), (TypeRecord)choiceType);
					}
					break;
				default:
					throw new CompilerException(String.format("Unexpected choiceType = %s", choiceType));
				}
			}

			out.unnest();
			out.indent().println("};");
		} else {
			throw new CompilerException(String.format("Unknown constant = %s", type.toString()));
		}
	}
	private void genDefProcedure(IndentPrintWriter out, String name, TypeProcedure type) {
		out.indent().format("// TODO Type Proc %s", name, type.toString()).println();
	}
	private void genDefError(IndentPrintWriter out, String name, TypeError type) {
		out.indent().format("// TODO Type Error %s", name, type.toString()).println();
	}


	private void genConstPredefined(IndentPrintWriter out, String name, TypePredefined type, Constant constant) {
		String constName = Util.toJavaConstName(name);
		switch(type.kind) {
		case BOOLEAN:
			if (constant.kind == Constant.Kind.BOOLEAN) {
				ConstantBoolean constBool = (ConstantBoolean)constant;
				out.indent().format("static const quitn32 %s = %s;", constName, constBool.value).println();
			} else {
				throw new CompilerException(String.format("Unexpected type of constant.  constant = %s", constant));
			}
			break;
		case CARDINAL:
		case UNSPECIFIED:
			if (constant.kind == Constant.Kind.NUMBER) {
				ConstantNumber constNum = (ConstantNumber)constant;
				long value = constNum.value;
				if ((value & 0xff) == 0xff || (value & 0xff) == 0) {
					out.indent().format("static const quint16 %s = 0x%X;", constName, value).println();
				} else {
					out.indent().format("static const quint16 %s = %d;", constName, value).println();
				}
			} else {
				throw new CompilerException(String.format("Unexpected type of constant.  constant = %s", constant));
			}
			break;
		case LONG_CARDINAL:
		case UNSPECIFIED2:
		case UNSPECIFIED3:
			if (constant.kind == Constant.Kind.NUMBER) {
				ConstantNumber constNum = (ConstantNumber)constant;
				long value = constNum.value;
				if ((value & 0xff) == 0xff || (value & 0xff) == 0) {
					out.indent().format("static const quint32 %s = 0x%XL;", constName, value).println();
				} else {
					out.indent().format("static const quint32 %s = %dL;", constName, value).println();
				}
			} else if (constant.kind == Constant.Kind.REFERENCE) {
				ConstantReference constRef = (ConstantReference)constant;
				out.indent().format("static const quint32 %s = %s;", constName, program.getLocalRefName(constRef)).println();
			} else {
				throw new CompilerException(String.format("Unexpected type of constant.  constant = %s", constant));
			}
			break;
		case STRING:
			if (constant.kind == Constant.Kind.STRING) {
				ConstantString constStr = (ConstantString)constant;
				out.indent().format("static const quint8* %s = %s;", constName, constStr.value).println();
			} else if (constant.kind == Constant.Kind.REFERENCE) {
				ConstantReference constRef = (ConstantReference)constant;
				out.indent().format("static const quint8* %s = %s;", constName, program.getLocalRefName(constRef)).println();
			} else {
				throw new CompilerException(String.format("Unexpected type of constant.  constant = %s", constant));
			}
			break;
		default:
			out.indent().format("// TODO Const Predefined %s", name, type.toString()).println();
			break;
		}
	}
	private void genConstEnum(IndentPrintWriter out, String name, TypeReference refType, Constant constant) {
		String constName = Util.toJavaConstName(name);
		if (constant.kind == Constant.Kind.REFERENCE) {
			ConstantReference constRef = (ConstantReference)constant;
			String enumName = program.getLocalRefName(refType);
			out.indent().format("static const %s %s = %s.%s;", enumName, constName, enumName, program.getLocalRefName(constRef)).println();
		} else {
			throw new CompilerException(String.format("Unexpected type of constant.  constant = %s", constant));
		}
	}
	private void genConstArray(IndentPrintWriter out, String name, TypeReference refType, Constant constant) {
		String constName = Util.toJavaConstName(name);
		Type concreteType = refType.getConcreteType();
		TypeArray arrayType = (TypeArray)concreteType;

		if (arrayType.type.isPredefine()) {
			switch(arrayType.type.kind) {
			case UNSPECIFIED: {
				// array of unspecified
				ConstantArray constArray = (ConstantArray)constant;
				StringBuilder initList = new StringBuilder();
				for(Constant e: constArray.values) {
					switch(e.kind) {
					case NUMBER: {
						ConstantNumber constNum = (ConstantNumber)e;
						if ((constNum.value & 0xffff) == 0xffff) {
							initList.append(String.format(", 0x%X", constNum.value));
						} else {
							initList.append(String.format(", %d", constNum.value));
						}
					}
						break;
					default:
						throw new CompilerException(String.format("Unexpected refType.  refType = %s  concreteType = %s", refType, concreteType));
					}
				}
				out.indent().format("static const quint16 %s[] = new int[]{%s};", constName, initList.substring(2)).println();
			}
				break;
			default:
				throw new CompilerException(String.format("Unexpected arrayType.  arrayType = %s", arrayType));
			}
		} else {
			out.indent().format("// TODO Const REF Array %s = %s(%s) = %s", name, refType, concreteType, constant).println();
		}
	}
	private void genConstSequence(IndentPrintWriter out, String name, TypeReference refType, Constant constant) {
		String constName = Util.toJavaConstName(name);
		Type concreteType = refType.getConcreteType();
		TypeSequence sequenceType = (TypeSequence)concreteType;
		Type sequenceElementConcreteType = sequenceType.type.getConcreteType();
		if (sequenceElementConcreteType.isPredefine()) {
			switch(sequenceElementConcreteType.kind) {
			case UNSPECIFIED: {
				// sequence of unspecified
				ConstantArray constArray = (ConstantArray)constant;
				StringBuilder initList = new StringBuilder();
				for(Constant e: constArray.values) {
					switch(e.kind) {
					case NUMBER: {
						ConstantNumber constNum = (ConstantNumber)e;
						if ((constNum.value & 0xffff) == 0xffff) {
							initList.append(String.format(", 0x%X", constNum.value));
						} else {
							initList.append(String.format(", %d", constNum.value));
						}
					}
						break;
					default:
						throw new CompilerException(String.format("Unexpected refType.  refType = %s  concreteType = %s", refType, concreteType));
					}
				}
				out.indent().format("satic const quint16 %s[] = {%s};", constName, initList.substring(2)).println();
			}
				break;
			case LONG_CARDINAL: {
				// sequence of unspecified
				ConstantArray constArray = (ConstantArray)constant;
				StringBuilder initList = new StringBuilder();
				for(Constant e: constArray.values) {
					switch(e.kind) {
					case NUMBER: {
						ConstantNumber constNum = (ConstantNumber)e;
						if ((constNum.value & 0xffff) == 0xffff) {
							initList.append(String.format(", 0x%XL", constNum.value));
						} else {
							initList.append(String.format(", %dL", constNum.value));
						}
					}
						break;
					default:
						throw new CompilerException(String.format("Unexpected refType.  refType = %s  concreteType = %s", refType, concreteType));
					}
				}
				out.indent().format("satic const quint32 %s[] = {%s};", constName, initList.substring(2)).println();
			}
				break;
			default:
				throw new CompilerException(String.format("Unexpected arrayType.  sequenceType = %s", sequenceType));
			}
		} else {
			out.indent().format("// TODO Const REF Array %s = %s(%s) = %s", name, refType, concreteType, constant).println();
		}
	}
	private void genConstRecord(IndentPrintWriter out, String name, TypeReference refType, Constant constant) {
		out.indent().format("// TODO Const REF Record %s", name, refType).println();
	}
	private void genConstChoice(IndentPrintWriter out, String name, TypeReference refType, Constant constant) {
		out.indent().format("// TODO Const REF Choice %s", name, refType).println();
	}
	private void genConstProcedure(IndentPrintWriter out, String name, TypeProcedure type, Constant constant) {
		throw new CompilerException(String.format("genConstProcedure name = %s", name));
	}
	private void genConstError(IndentPrintWriter out, String name, TypeError type, Constant constant) {
		throw new CompilerException(String.format("genConstError name = %s", name));
	}

	private void genFieldList(IndentPrintWriter out, List<Field> fieldList) {
		for(Field field: fieldList) {
			String fieldName = field.name;
			Type fieldType = field.type;
			//Type baseType = getBaseType(fieldType);
			switch(fieldType.kind) {
			case BOOLEAN:
				out.indent().format("quint32 %s;", fieldName).println();
				break;
			case CARDINAL:
				out.indent().format("quint16 %s;", fieldName).println();
				break;
			case LONG_CARDINAL:
				out.indent().format("quint32 %s;", fieldName).println();
				break;
			case STRING:
				out.indent().format("quint8* %s;", fieldName).println();
				break;
			case UNSPECIFIED:
				out.indent().format("quint16 %s;", fieldName).println();
				break;
			case UNSPECIFIED2:
				out.indent().format("quint32 %s;", fieldName).println();
				break;
			case UNSPECIFIED3:
				out.indent().format("quint48 %s;", fieldName).println();
				break;
			case ARRAY:
				genDefArray(out, fieldName, (TypeArray)fieldType);
				break;
			case SEQUENCE:
				genDefSequence(out, fieldName, (TypeSequence)fieldType);
				break;
			case REFERENCE: {
				TypeReference refType = (TypeReference)fieldType;
				Type concreteType = fieldType.getConcreteType();
				switch(concreteType.kind) {
				case BOOLEAN:
					out.indent().format("// EXPAND %s as %s", refType.value, concreteType.kind).println();
					out.indent().format("quint32 %s;", fieldName).println();
					break;
				case CARDINAL:
					out.indent().format("// EXPAND %s as %s", refType.value, concreteType.kind).println();
					out.indent().format("quint16 %s;", fieldName).println();
					break;
				case LONG_CARDINAL:
					out.indent().format("// EXPAND %s as %s", refType.value, concreteType.kind).println();
					out.indent().format("quint32 %s;", fieldName).println();
					break;
				case STRING:
					out.indent().format("// EXPAND %s as %s", refType.value, concreteType.kind).println();
					out.indent().format("quint8* %s;", fieldName).println();
					break;
				case UNSPECIFIED:
					out.indent().format("// EXPAND %s as %s", refType.value, concreteType.kind).println();
					out.indent().format("quint16 %s;", fieldName).println();
					break;
				case UNSPECIFIED2:
					out.indent().format("// EXPAND %s as %s", refType.value, concreteType.kind).println();
					out.indent().format("quint32 %s;", fieldName).println();
					break;
				case UNSPECIFIED3:
					out.indent().format("// EXPAND %s as %s", refType.value, concreteType.kind).println();
					out.indent().format("quint48 %s;", fieldName).println();
					break;
				case ARRAY:
					out.indent().format("// EXPAND %s as %s", refType.value, concreteType.kind).println();
					genDefArray(out, fieldName, (TypeArray)concreteType);
					break;
				case SEQUENCE:
					out.indent().format("// EXPAND %s as %s", refType.value, concreteType.kind).println();
					genDefSequence(out, fieldName, (TypeSequence)concreteType);
					break;
				case CHOICE:
					out.indent().format("// TODO CHOICE public %s %s = new %s();", program.getLocalRefName(refType.getLastReference()), fieldName, program.getLocalRefName(refType.getLastReference())).println();
					break;
				case ENUM:
					out.indent().format("%s %s;", program.getLocalRefName(refType.getLastReference()), fieldName).println();
					break;
				case RECORD:
					out.indent().format("%s %s;", program.getLocalRefName(refType.getLastReference()), fieldName).println();
					break;
				default:
					throw new CompilerException(String.format("Unexpected fieldType.getConcreteType() = %s", fieldType.getConcreteType()));
				}
			}
				break;
			default:
				throw new CompilerException(String.format("Unexpected fieldType = %s", fieldType));
			}
		}
	}
	
	private String toClassName(String s) {
		StringBuilder ret = new StringBuilder();
		ret.append(Character.toUpperCase(s.charAt(0)));
		ret.append(s.substring(1));
		return ret.toString();
	}
}
