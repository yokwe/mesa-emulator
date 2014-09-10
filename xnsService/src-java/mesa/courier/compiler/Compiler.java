package mesa.courier.compiler;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;

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
			// include depend module
			for(Program.Info info: program.depends) {
				outh.indent().format("#include \"%s.h\"", info.getProgramVersion()).println();
			}

			// output namespace
			outh.indent().println();
			outh.indent().println("namespace Courier {");
			outh.nest();

			// output main namespace
			outh.indent().format("namespace %s {", program.info.getProgramVersion()).println();
			outh.nest();
			if (program.info.version != 0) {
				outh.indent().format("const quint32 PROGRAM_NUMBER = %d;", program.info.program).println();
				outh.indent().format("const quint32 VERSION_NUMBER = %d;", program.info.version).println();
				outh.indent().println();
			}
			
			// output type (choice, enum, record as declared in appearance order
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
					outh.indent().format("// IGNORE predefined type.  %s: TYPE = %s", name, type.kind).println();
					break;
				case ENUM:
					genEnumDef(outh, name, (TypeEnum)type);
					break;
				case ARRAY:
					outh.indent().format("// IGNORE top level ARRAY type.  %s", name).println();
					break;
				case SEQUENCE:
					outh.indent().format("// IGNORE top level SEQUENCE type.  %s", name).println();
					break;
				case RECORD:
					// generate record class definition
					genRecordDef(outh, name, (TypeRecord)type);
					break;
				case CHOICE:
					// generate choice class definition
					genChoiceDef(outh, name, (TypeChoice)type);
					break;
				case PROCEDURE:
					// generate procedure class definition
					genProcedureDef(outh, name, (TypeProcedure)type);
					break;
				case ERROR:
					// generate error class definition
					genErrorDef(outh, name, (TypeError)type);
					break;
				case REFERENCE:
					outh.indent().format("// IGNORE reference.  %s: TYPE = %s", name, ((TypeReference)type).value.toString()).println();
					break;
				default:
					break;
				}
			}
			
			// output const -- constatnt
			for(Program.DeclConst declConst: program.constList) {
				String   name     = declConst.name;
				Type     type     = declConst.type;
				Constant constant = declConst.constant;
				Type     baseType = type.getBaseType();
				
				switch (baseType.kind) {
				case BOOLEAN:
				case CARDINAL:
				case LONG_CARDINAL:
				case STRING:
				case UNSPECIFIED:
				case UNSPECIFIED2:
				case UNSPECIFIED3:
					if (type.kind == Type.Kind.REFERENCE) outh.indent().format("// EXPAND %s as %s", ((TypeReference)type).value, baseType.kind).println();
					genPredefinedConst(outh, name, (TypePredefined)baseType, constant);
					break;
				case REFERENCE: {
					TypeReference refType = (TypeReference)baseType;
					Type concreteType = baseType.getConcreteType();
					switch(concreteType.kind) {
					case ENUM:
						genEnumConst(outh, name, refType, constant);
						break;
					default:
						outh.indent().format("// TODO need to implment concreteType.kind = %s", concreteType.kind).println();
						throw new CompilerException("NO IMPL");
					}
				}
					break;
				default:
					outh.indent().format("// TODO need to implement baseType.kind = %s", baseType.kind).println();
					throw new CompilerException("NO IMPL");
				}
			}

			// output include of header
			outc.indent().format("#include \"%s.h\"", program.info.getProgramVersion()).println();
			
			// close main namespace
			outh.unnest();
			outh.indent().println("}");

			// output implementation of getEnumName for this module
			for(Program.DeclType declType: program.typeList) {
				String name = declType.name;
				Type   type = declType.type;

				switch (type.kind) {
				case ENUM:
					outh.indent().println();
					outh.indent().format("const char* getName(%s::%s value);", program.info.getProgramVersion(), name).println();
					outh.indent().format("void serialize  (ByteBuffer* buffer, const %s::%s& value);", program.info.getProgramVersion(), name).println();
					outh.indent().format("void deserialize(ByteBuffer* buffer, %s::%s& value);", program.info.getProgramVersion(), name).println();
					
					outc.indent().println();
					outc.indent().format("static QMap<Courier::%s::%s, const char*>map%s = {", program.info.getProgramVersion(), name, name).println();
					outc.nest();
					for(Type.Correspondence e: ((TypeEnum)type).elements) {
						String enumName = Util.toJavaConstName(e.id);
						outc.indent().format("{Courier::%s::%s::%s, \"%s\"},", program.info.getProgramVersion(), name, enumName, enumName).println();
					}
					outc.unnest();
					outc.indent().println("};");
					outc.indent().format("const char* Courier::getName(%s::%s value) {", program.info.getProgramVersion(), name).println();
					outc.nest();
					outc.indent().format("return map%s.value(value, 0);", name).println();
					outc.unnest();
					outc.indent().println("}");
					outc.indent().format("void serialize  (ByteBuffer* buffer, const Courier::%s::%s& value) {", program.info.getProgramVersion(), name).println();
					outc.nest();
					outc.indent().format("buffer->put16((quint16)value);").println();
					outc.unnest();
					outc.indent().println("}");
					outc.indent().format("void deserialize(ByteBuffer* buffer, Courier::%s::%s& value) {", program.info.getProgramVersion(), name).println();
					outc.nest();
					outc.indent().format("value = (Courier::%s::%s)buffer->get16();", program.info.getProgramVersion(), name).println();
					outc.unnest();
					outc.indent().println("}");
					
					break;
				default:
					break;
				}
			}
			// output serialization and deserialization for this module
			for(Program.DeclType declType: program.typeList) {
				String name = declType.name;
				Type   type = declType.type;

				switch (type.kind) {
				case CHOICE:
					outh.indent().println();
					outh.indent().format("void serialize  (ByteBuffer* buffer, const %s::%s& value);", program.info.getProgramVersion(), name).println();
					outh.indent().format("void deserialize(ByteBuffer* buffer, %s::%s& value);", program.info.getProgramVersion(), name).println();

					outc.indent().println();
					outc.indent().format("void Courier::serialize  (ByteBuffer* buffer, const %s::%s& value) {", program.info.getProgramVersion(), name).println();
					outc.nest();
					outc.indent().format("Courier::serialize(buffer, value.tag);").println();
					outc.indent().format("switch(value.tag) {").println();
					
					{
						TypeChoice typeChoice = (TypeChoice)type;
						if (typeChoice instanceof TypeChoice.Typed) {
							TypeChoice.Typed typed = (TypeChoice.Typed)typeChoice;
							TypeReference typeReference = (TypeReference)typed.type;
							for(TypeChoice.Candidate<String> candidate: typed.candidates) {
								Type candicatType = candidate.type;
								for(String designator: candidate.designators) {
									outc.indent().format("case %s::%s::%s:", typeReference.value.program, typeReference.value.name, Util.toJavaConstName(designator)).println();
									outc.nest();
									TypeRecord typeRecord = (TypeRecord)candicatType;
									for(Type.Field field: typeRecord.fields) {
										outc.indent().format("Courier::serialize(buffer, value.%s.%s);", Util.toJavaConstName(designator), field.name).println();
									}
									outc.indent().println("break;");
									outc.unnest();
								}
							}
						} else if (typeChoice instanceof TypeChoice.Anon) {
							throw new CompilerException(String.format("NO IMPL typeChoice = %s", typeChoice.toString()));
						} else {
							throw new CompilerException(String.format("Unexpected typeChoice = %s", typeChoice.toString()));
						}
					}
					
					outc.indent().format("}").println();
					
					outc.unnest();
					outc.indent().println("}");
					
					outc.indent().println();
					outc.indent().format("void Courier::deserialize(ByteBuffer* buffer, %s::%s& value) {", program.info.getProgramVersion(), name).println();
					outc.nest();
					outc.indent().format("Courier::deserialize(buffer, value.tag);").println();
					outc.indent().format("switch(value.tag) {").println();
					
					{
						TypeChoice typeChoice = (TypeChoice)type;
						if (typeChoice instanceof TypeChoice.Typed) {
							TypeChoice.Typed typed = (TypeChoice.Typed)typeChoice;
							TypeReference typeReference = (TypeReference)typed.type;
							for(TypeChoice.Candidate<String> candidate: typed.candidates) {
								Type candicatType = candidate.type;
								for(String designator: candidate.designators) {
									outc.indent().format("case %s::%s::%s:", typeReference.value.program, typeReference.value.name, Util.toJavaConstName(designator)).println();
									outc.nest();
									TypeRecord typeRecord = (TypeRecord)candicatType;
									for(Type.Field field: typeRecord.fields) {
										outc.indent().format("Courier::deserialize(buffer, value.%s.%s);", Util.toJavaConstName(designator), field.name).println();
									}
									outc.indent().println("break;");
									outc.unnest();
								}
							}
						} else if (typeChoice instanceof TypeChoice.Anon) {
							throw new CompilerException(String.format("NO IMPL typeChoice = %s", typeChoice.toString()));
						} else {
							throw new CompilerException(String.format("Unexpected typeChoice = %s", typeChoice.toString()));
						}
					}
					
					outc.indent().format("}").println();
					outc.unnest();
					outc.indent().println("}");
					break;
				case RECORD:
					outh.indent().println();
					outh.indent().format("void serialize  (ByteBuffer* buffer, const %s::%s& value);", program.info.getProgramVersion(), name).println();
					outh.indent().format("void deserialize(ByteBuffer* buffer, %s::%s& value);", program.info.getProgramVersion(), name).println();
					
					outc.indent().println();
					outc.indent().format("void Courier::serialize  (ByteBuffer* buffer, const %s::%s& value) {", program.info.getProgramVersion(), name).println();
					outc.nest();
					for(Type.Field field: ((TypeRecord)type).fields) {
						String fieldName = field.name;
						outc.indent().format("Courier::serialize(buffer, value.%s);", fieldName).println();
					}
					outc.unnest();
					outc.indent().println("}");
					
					outc.indent().println();
					outc.indent().format("void Courier::deserialize(ByteBuffer* buffer, %s::%s& value) {", program.info.getProgramVersion(), name).println();
					outc.nest();
					for(Type.Field field: ((TypeRecord)type).fields) {
						String fieldName = field.name;
						outc.indent().format("Courier::deserialize(buffer, value.%s);", fieldName).println();
					}
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
	
	private void genEnumConst(IndentPrintWriter out, String name, TypeReference refType, Constant constant) {
		String constName = Util.toJavaConstName(name);
		if (constant.kind == Constant.Kind.REFERENCE) {
			ConstantReference constRef = (ConstantReference)constant;
			String enumName = program.getLocalRefName(refType);
			out.indent().format("const %s %s = %s::%s;", enumName, constName, enumName, program.getLocalRefName(constRef)).println();
		} else {
			throw new CompilerException(String.format("Unexpected type of constant.  constant = %s", constant));
		}
	}

	
	private void genPredefinedConst(IndentPrintWriter out, String name, TypePredefined type, Constant constant) {
		String constName = Util.toJavaConstName(name);
		switch(type.kind) {
		case BOOLEAN:
			if (constant.kind == Constant.Kind.BOOLEAN) {
				ConstantBoolean constBool = (ConstantBoolean)constant;
				out.indent().format("const quint16 %s = %s;", constName, constBool.value).println();
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
					out.indent().format("const quint16 %s = 0x%X;", constName, value).println();
				} else {
					out.indent().format("const quint16 %s = %dU;", constName, value).println();
				}
			} else {
				throw new CompilerException(String.format("Unexpected type of constant.  constant = %s", constant));
			}
			break;
		case LONG_CARDINAL:
		case UNSPECIFIED2:
			if (constant.kind == Constant.Kind.NUMBER) {
				ConstantNumber constNum = (ConstantNumber)constant;
				long value = constNum.value;
				if ((value & 0xff) == 0xff || (value & 0xff) == 0) {
					out.indent().format("const quint32 %s = 0x%X;", constName, value).println();
				} else {
					out.indent().format("const quint32 %s = %dU;", constName, value).println();
				}
			} else if (constant.kind == Constant.Kind.REFERENCE) {
				ConstantReference constRef = (ConstantReference)constant;
				out.indent().format("const quint32 %s = %s;", constName, program.getLocalRefName(constRef)).println();
			} else {
				throw new CompilerException(String.format("Unexpected type of constant.  constant = %s", constant));
			}
			break;
		case UNSPECIFIED3:
			if (constant.kind == Constant.Kind.NUMBER) {
				ConstantNumber constNum = (ConstantNumber)constant;
				long value = constNum.value;
				if ((value & 0xff) == 0xff || (value & 0xff) == 0) {
					out.indent().format("const quint48 %s = 0x%XLL;", constName, value).println();
				} else {
					out.indent().format("const quint48 %s = %dULL;", constName, value).println();
				}
			} else if (constant.kind == Constant.Kind.REFERENCE) {
				ConstantReference constRef = (ConstantReference)constant;
				out.indent().format("const quint48 %s = %s;", constName, program.getLocalRefName(constRef)).println();
			} else {
				throw new CompilerException(String.format("Unexpected type of constant.  constant = %s", constant));
			}
			break;
		case STRING:
			throw new CompilerException("NO IMPL STRING");
		default:
			throw new CompilerException(String.format("Unexpected type.  type.kind = %s", type.kind));
		}
	}
	
	private void genEnumDef(IndentPrintWriter out, String name, TypeEnum type) {
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
	
	private void genRecordDef(IndentPrintWriter out, String name, TypeRecord type) {
		out.indent().format("struct %s {", name).println();
		out.nest();
		for(Type.Field field: type.fields) {
			String fieldName = field.name;
			String fieldType = getCPPType(field.type);
			
			out.indent().format("%s %s;", fieldType, fieldName).println();
		}
		out.unnest();
		out.indent().println("};");
	}
	private void genChoiceDef(IndentPrintWriter out, String name, TypeChoice type) {
		out.indent().format("struct %s {", name).println();
		out.nest();
		
		if (type instanceof TypeChoice.Anon) {
			throw new CompilerException("NO IMPL");
		} else if (type instanceof TypeChoice.Typed) {
			TypeChoice.Typed typed = (TypeChoice.Typed)type;
			Type choiceType = typed.type.getBaseType();
			out.indent().format("%s %s;", program.getLocalRefName((TypeReference)choiceType), "tag").println();
			
			out.indent().println("union {");
			out.nest();
			for(TypeChoice.Candidate<String> candidate: typed.candidates) {
				Type candidateType = candidate.type;
				for(String designator: candidate.designators) {
					TypeRecord typeRecord = (TypeRecord)candidateType;
					
					out.indent().println("struct {");
					out.nest();
					for(Type.Field field: typeRecord.fields) {
						String fieldName = field.name;
						String fieldType = getCPPType(field.type);
						
						out.indent().format("%s %s;", fieldType, fieldName).println();
					}
					out.unnest();
					out.indent().format("} %s;", Util.toJavaConstName(designator)).println();
				}
			}
			out.unnest();
			out.indent().println("};");
		} else {
			throw new CompilerException(String.format("Unexpected kind = %s", type.kind));
		}
			
		out.unnest();
		out.indent().println("};");
	}
	
	
	private void genProcedureDef(IndentPrintWriter out, String name, TypeProcedure type) {
		throw new CompilerException(String.format("Unexpected kind = %s", type.kind));
	}
	private void genErrorDef(IndentPrintWriter out, String name, TypeError type) {
		throw new CompilerException(String.format("Unexpected kind = %s", type.kind));
	}
	
	private String getCPPType(Type type) {
		switch(type.kind) {
		case BOOLEAN:
			return "quint16";
		case CARDINAL:
			return "quint16";
		case LONG_CARDINAL:
			return "quint32";
		case STRING:
			break;
		case UNSPECIFIED:
			return "quint16";
		case UNSPECIFIED2:
			return "quint32";
		case UNSPECIFIED3:
			return "quint48";
		// constructed
		case ENUM:
		case ARRAY:
		case SEQUENCE:
		case RECORD:
		case CHOICE:
		case PROCEDURE:
		case ERROR:
			break;
		// reference
		case REFERENCE:
		{
			TypeReference refType = (TypeReference)type;
			Type concreteType = type.getConcreteType();
			switch(concreteType.kind) {
			case BOOLEAN:
				return "quint16";
			case CARDINAL:
				return "quint16";
			case LONG_CARDINAL:
				return "quint32";
			case STRING:
				break;
			case UNSPECIFIED:
				return "quint16";
			case UNSPECIFIED2:
				return "quint32";
			case UNSPECIFIED3:
				return "quint48";
			// constructed
			case ENUM:
			case ARRAY:
			case SEQUENCE:
			case RECORD:
			case CHOICE:
				return program.getLocalRefName(refType.getLastReference());
			case PROCEDURE:
			case ERROR:
			case REFERENCE:
				break;
			default:
				break;
			}
			throw new CompilerException(String.format("Unexpected concreteType.ref kind = %s", concreteType.kind));
		}
		default:
			break;
		}
		throw new CompilerException(String.format("Unexpected kind = %s", type.kind));
	}
}
