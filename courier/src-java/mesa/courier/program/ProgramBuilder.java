package mesa.courier.program;

import java.io.File;
import java.io.FilenameFilter;
import java.io.IOException;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import mesa.courier.antlr.CourierBaseVisitor;
import mesa.courier.antlr.CourierLexer;
import mesa.courier.antlr.CourierParser;
import mesa.courier.antlr.CourierParser.*;
import mesa.courier.program.Constant.*;
import mesa.courier.program.Program.Info;
import mesa.courier.program.Type.*;

import org.antlr.v4.runtime.ANTLRFileStream;
import org.antlr.v4.runtime.ANTLRInputStream;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.misc.NotNull;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class ProgramBuilder {
	protected static final Logger logger = LoggerFactory.getLogger(ProgramBuilder.class);
	
	private static String COURIER_FILE_SUFFIX = ".cr";
	private static FilenameFilter courierFileFilter = new FilenameFilter() {
		@Override
		public boolean accept(File dir, String name) {
			return name.endsWith(COURIER_FILE_SUFFIX);
		}
	};
	
	private static class Cache {
		public final Program               program;
		
		public Cache(Program program) {
			this.program  = program;
		}
	}

	private static Map<String, Cache> programCache = new TreeMap<>();
	public static void initialize(String directoryPath) {
		boolean hasError = false;

		File directory = new File(directoryPath);
		if (!directory.isDirectory()) throw new ProgramException(String.format("no such directory. directoryPath = %s", directoryPath));
		File[] courierFiles = directory.listFiles(courierFileFilter);
		if (courierFiles.length == 0) throw new ProgramException(String.format("no courier file. directoryPath = %s", directoryPath));

		for(File courierFile: courierFiles) {
			String fileName = courierFile.getName();
			int length = fileName.length();
			
			String key = fileName.substring(0, length - COURIER_FILE_SUFFIX.length());
			
			String path = String.format("%s%c%s", directoryPath, File.separatorChar, fileName);
			CourierProgramContext tree     = getTree(path);
			Program               program  = ProgramVisitor.getProgram(tree);
			ProgramBuilder        builder = new ProgramBuilder(program);
			
			// build declaration of program
			builder.buildDecl(tree);
			
			if (key.equals(program.info.getProgramVersion())) {
				// Name with version number if version number is not zero
			} else if (key.equals(String.format("%s%d", program.info.name, program.info.version))) {
				// Name with version number
			} else {
				hasError = true;
				logger.error(String.format("Filename and program name and version is inconsistent.  key = %s  info = %s", key, program.info.toString()));
			}

			programCache.put(key, new Cache(program));
		}
		if (hasError) throw new ProgramException("hasError");
	}
	public static Program getProgram(String name) {
		if (!programCache.containsKey(name)) throw new ProgramException(String.format("Unknown program  name = %s", name));
		
		Cache cache = programCache.get(name);
		return cache.program;
	}

	private static CourierProgramContext getTree(String path) {
		ANTLRInputStream input;
		try {
			input = new ANTLRFileStream(path);
		} catch (IOException e) {
			throw new ProgramException("IOException", e);
		}
		CourierLexer          lexer   = new CourierLexer(input);
		CommonTokenStream     tokens  = new CommonTokenStream(lexer);
		CourierParser         parser  = new CourierParser(tokens);
		
		return parser.courierProgram();
	}	
	
	
	
	private final Program               program;
	private final TypeVisitor           typeVisitor;
	private final ReferencedTypeVisitor referencedTypeVisitor;
	private final ConstVisitor          constVisitor;

	private ProgramBuilder(Program program_) {
		program               = program_;
		typeVisitor           = new TypeVisitor();
		referencedTypeVisitor = new ReferencedTypeVisitor();
		constVisitor          = new ConstVisitor();
	}
	
	// need to be instance method to access instance variable
	private void buildDecl(CourierProgramContext tree) {
		if (tree.programBody().declarationList() != null) {
			// First process type
			for(DeclarationContext declarationContext: tree.programBody().declarationList().elements) {
				if (declarationContext instanceof DeclTypeContext) {
					DeclTypeContext context = (DeclTypeContext)declarationContext;
					String name = context.name.getText();
					Type   type = typeVisitor.visit(context.type());
					program.addType(name, type);
				} else if (declarationContext instanceof DeclConstContext) {
					DeclConstContext context = (DeclConstContext)declarationContext;
					String name = context.name.getText();
					Type   type = typeVisitor.visit(context.type());
					Constant constant = constVisitor.visit(context.constant());
					program.addConstant(name, type, constant);
				} else {
					String msg = String.format("Unknonw declarationContext = %s", declarationContext.getText());
					logger.error(msg);
					throw new ProgramException(msg);
				}
			}
		}
	}	

	private static class ProgramVisitor extends CourierBaseVisitor<Void> {
		private Program program = null;
		
		public static Program getProgram(CourierProgramContext tree) {
			ProgramVisitor visitor = new ProgramVisitor();
			visitor.visit(tree);
			if (visitor.program == null) throw new ProgramException("program == null");
			return visitor.program;
		}
		
		// don't visit in declarationList
		@Override
		public Void visitDeclarationList(@NotNull DeclarationListContext context) {
			return null;
		}
		
		@Override
		public Void visitProgramHeader(@NotNull ProgramHeaderContext context) {
			String name      = context.name.getText();
			int    programNo = (int)Util.parseLong(context.program.getText());
			int    versionNo = (int)Util.parseLong(context.version.getText());
			program = new Program(new Info(name, programNo, versionNo));
			return null;
		}

		@Override
		public Void visitReferencedProgram(@NotNull ReferencedProgramContext context) {
			String name    = context.program.getText();
			int    number  = (int)Util.parseLong(context.number.getText());
			int    version = (int)Util.parseLong(context.version.getText());
			program.addDepend(new Info(name, number, version));
			return null;
		}
	}

	// To access program field of ProgramBuilder, XXXVisitor is not static class
	private class TypeVisitor extends CourierBaseVisitor<Type> {
		// Primitive
		@Override
		public Type visitTypeBoolean(@NotNull TypeBooleanContext context) {
			return new TypePredefined(Type.Kind.BOOLEAN);
		}
		@Override
		public Type visitTypeByte(@NotNull TypeByteContext context) {
			return new TypePredefined(Type.Kind.BYTE);
		}
		@Override
		public Type visitTypeCardinal(@NotNull TypeCardinalContext context) {
			return new TypePredefined(Type.Kind.CARDINAL);
		}
		@Override
		public Type visitTypeLongCardinal(@NotNull TypeLongCardinalContext context) {
			return new TypePredefined(Type.Kind.LONG_CARDINAL);
		}
//		@Override
//		public Type visitTypeInteger(@NotNull TypeIntegerContext context) {
//			return new TypePredefined(Type.Kind.INTEGER);
//		}
//		@Override
//		public Type visitTypeLongInteger(@NotNull TypeLongIntegerContext context) {
//			return new TypePredefined(Type.Kind.LONG_INTEGER);
//		}
		@Override
		public Type visitTypeString(@NotNull TypeStringContext context) {
			return new TypePredefined(Type.Kind.STRING);
		}
		@Override
		public Type visitTypeUnspecified(@NotNull TypeUnspecifiedContext context) {
			return new TypePredefined(Type.Kind.UNSPECIFIED);
		}
		@Override
		public Type visitTypeUnspecified2(@NotNull TypeUnspecified2Context context) {
			return new TypePredefined(Type.Kind.UNSPECIFIED2);
		}
		@Override
		public Type visitTypeUnspecified3(@NotNull TypeUnspecified3Context context) {
			return new TypePredefined(Type.Kind.UNSPECIFIED3);
		}

		// Constructed
		@Override
		public Type visitTypeEmptyEnum(@NotNull TypeEmptyEnumContext context) {
			TypeEnum typeEnum = new TypeEnum(new TypePredefined(Type.Kind.UNSPECIFIED));

			for(CorrespondenceContext e: context.correspondenceList().elements) {
				String id           = e.ID().getText();
				long   numericValue = Util.parseLong(e.numericValue().getText());
				typeEnum.addCorrespondence(new TypeEnum.Correspondence(id, numericValue));
			}
			return typeEnum;
		}
		@Override
		public Type visitTypeEnum(@NotNull TypeEnumContext context) {
			EnumTypeContext enumType = context.enumType();
			
			Type type;
			if (enumType.BYTE() != null) {
				type = new TypePredefined(Type.Kind.BYTE);
			} else if (enumType.UNSPECIFIED() != null) {
				type = new TypePredefined(Type.Kind.UNSPECIFIED);
			} else if (enumType.UNSPECIFIED2() != null) {
				type = new TypePredefined(Type.Kind.UNSPECIFIED2);
			} else if (enumType.UNSPECIFIED3() != null) {
				type = new TypePredefined(Type.Kind.UNSPECIFIED3);
			} else {
				throw new ProgramException(String.format("unexpected enumType enumType = %s", enumType.toString()));
			}
			TypeEnum typeEnum = new TypeEnum(type);
			
			for(CorrespondenceContext e: context.correspondenceList().elements) {
				String id           = e.ID().getText();
				long   numericValue = Util.parseLong(e.numericValue().getText());
				typeEnum.addCorrespondence(new TypeEnum.Correspondence(id, numericValue));
			}
			return typeEnum;
		}
		@Override
		public Type visitTypeArray(@NotNull TypeArrayContext context) {
			long  numericValue = Util.parseLong(context.numericValue().getText());
			Type  type         = typeVisitor.visit(context.type());
			return new TypeArray(numericValue, type);
		}
		@Override
		public Type visitTypeSequence(@NotNull TypeSequenceContext context) {
			long  numericValue;
			if (context.maximumNumber().getChildCount() == 0) {
				numericValue = TypeSequence.MAX_SIZE;
			} else {
				numericValue = Util.parseLong(context.maximumNumber().getText());
			}
			
			Type  type         = typeVisitor.visit(context.type());
			return new TypeSequence(numericValue, type);
		}

		@Override
		public Type visitTypeRecord(@NotNull TypeRecordContext context) {
			TypeRecord ret = new TypeRecord();
			for(FieldContext field: context.fieldList().elements) {
				Type type = typeVisitor.visit(field.type());
				List<Token> nameList = field.nameList().elements;
				for(Token name: nameList) {
					ret.addField(new Field(name.getText(), type));
				}
			}
			return ret;
		}
		@Override
		public Type visitTypeEmptyRecord(@NotNull TypeEmptyRecordContext context) {
			return new TypeRecord();
		}
		@Override
		public Type visitTypeChoiceTyped(@NotNull TypeChoiceTypedContext context) {
			TypeChoice.Typed ret;
			
			{
				TypeReference referencedType = referencedTypeVisitor.visit(context.referencedType());
				ret = new TypeChoice.Typed(referencedType);
			}

			List<TypedCandidateContext> candicateContextList = context.typedCandidateList().elements;
			for(TypedCandidateContext candidateContext: candicateContextList) {
				TypeChoice.Candidate<String> candidate;
				{
					Type candidateType = typeVisitor.visit(candidateContext.type());
					candidate = new TypeChoice.Candidate<String>(candidateType);
				}
				
				for(Token designator: candidateContext.nameList().elements) {
					candidate.addDesignator(designator.getText());
				}
				ret.addCandidate(candidate);
			}
			return ret;
		}
		@Override
		public Type visitTypeChoiceAnon(@NotNull TypeChoiceAnonContext context) {
			TypeChoice.Anon ret = new TypeChoice.Anon();
			
			for(AnonCandidateContext candidateContext: context.anonCandidateList().elements) {
				TypeChoice.Candidate<Correspondence> candidate;
				{
					Type candidateType = typeVisitor.visit(candidateContext.type());
					candidate = new TypeChoice.Candidate<Correspondence>(candidateType);
				}
				
				for(CorrespondenceContext designator: candidateContext.correspondenceList().elements) {
					String  id           = designator.ID().getText();
					long    numericValue = Util.parseLong(designator.numericValue().getText());

					candidate.addDesignator(new Correspondence(id, numericValue));
				}
				ret.addCandidate(candidate);
			}
			return ret;
		}
		@Override
		public Type visitTypeProcedure(@NotNull TypeProcedureContext context) {
			TypeProcedure ret = new TypeProcedure();
			
			if (context.argumentList().getChildCount() != 0) {
				for(FieldContext field: context.argumentList().fieldList().elements) {
					Type type = typeVisitor.visit(field.type());
					for(Token name: field.nameList().elements) {
						ret.addParam(new Field(name.getText(), type));
					}
				}
			}
			if (context.resultList().getChildCount() != 0) {
				for(FieldContext field: context.resultList().fieldList().elements) {
					Type type = typeVisitor.visit(field.type());
					for(Token name: field.nameList().elements) {
						ret.addResult(new Field(name.getText(), type));
					}
				}
			}
			if (context.errorList().getChildCount() != 0) {
				for(Token name: context.errorList().nameList().elements) {
					ret.addError(name.getText());
				}
			}
			return ret;
		}
		@Override
		public Type visitTypeError(@NotNull TypeErrorContext context) {
			TypeError ret = new TypeError();
			
			if (context.argumentList().getChildCount() != 0) {
				for(FieldContext field: context.argumentList().fieldList().elements) {
					Type type = typeVisitor.visit(field.type());
					for(Token name: field.nameList().elements) {
						ret.addParam(new Field(name.getText(), type));
					}
				}
			}
			return ret;
		}

		// Referenced
		public Type visitTypeRef(@NotNull TypeRefContext context) {
			// Use program name with version number
			return new TypeReference(program.info.getProgramVersion(), context.name.getText());
		}
		public Type visitTypeRefQ(@NotNull TypeRefQContext context) {
			// Use program name with version number
			Info info = program.getDepend(context.program.getText());
			return new TypeReference(info.getProgramVersion(), context.name.getText());
		}
	}
	private class ReferencedTypeVisitor extends CourierBaseVisitor<TypeReference> {
		// Referenced
		public TypeReference visitTypeRef(@NotNull TypeRefContext context) {
			// Use program name with version number
			return new TypeReference(program.info.getProgramVersion(), context.name.getText());
		}
		public TypeReference visitTypeRefQ(@NotNull TypeRefQContext context) {
			// Use program name with version number
			Info info = program.getDepend(context.program.getText());
			return new TypeReference(info.getProgramVersion(), context.name.getText());
		}
	}

	private class ConstVisitor extends CourierBaseVisitor<Constant> {
		// predefinedConstant
		public Constant visitConstTrue(@NotNull ConstTrueContext context) {
			return new ConstantBoolean(true);
		}
		public Constant visitConstFalse(@NotNull ConstFalseContext context) {
			return new ConstantBoolean(false);
		}
		public Constant visitConstNumber(@NotNull ConstNumberContext context) {
			return new ConstantNumber(Util.parseLong(context.NUMBER().getText()));
		}
		public Constant visitConstNumberNegative(@NotNull ConstNumberNegativeContext context) {
			return new ConstantNumber((Util.parseLong(context.NUMBER().getText())) * -1L);
		}
		public Constant visitConstString(@NotNull ConstStringContext context) {
			return new ConstantString(context.STR().getText());
		}
		// constructedConstant
		public Constant visitConstArray(@NotNull ConstArrayContext context) {
			ConstantArray ret = new ConstantArray();
			for(ConstantContext e: context.elementList().elements) {
				Constant constant = constVisitor.visit(e);
				ret.addConstant(constant);
			}

			return ret;
		}
		public Constant visitConstRecord(@NotNull ConstRecordContext context) {
			ConstantRecord ret = new ConstantRecord();
			for(ComponentContext e: context.componentList().elements) {
				Constant constant = constVisitor.visit(e.constant());
				for(Token ee: e.nameList().elements) {
					Component component = new Component(ee.getText(), constant);
					ret.addComponent(component);
				}
			}
			return ret;
		}
		public Constant visitConstEmpty(@NotNull ConstEmptyContext context) {
			ConstantArray ret = new ConstantArray();
			return ret;
		}
		public Constant visitConstChoice(@NotNull ConstChoiceContext context) {
			String   id       = context.ID().getText();
			Constant constant = constVisitor.visit(context.constant());
			return new ConstantChoice(id, constant);
		}
		// referencedConstant
		public Constant visitConstRef(@NotNull ConstRefContext context) {
			// Assume local reference to constant or enum element.
			// Cannot distinguish local reference and enum element. So don't supply current program name as program
			return new ConstantReference(context.name.getText());
		}
		public Constant visitConstRefQ(@NotNull ConstRefQContext context) {
			// Use program name with version number
			Info info = program.getDepend(context.program.getText());
			return new ConstantReference(info.getProgramVersion(), context.name.getText());
		}
	}
}
