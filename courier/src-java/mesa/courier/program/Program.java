package mesa.courier.program;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import mesa.courier.compiler.CompilerException;

public final class Program {
	protected static final Logger logger = LoggerFactory.getLogger(Program.class);
	
	public static Map<String, Program>all = new TreeMap<>();
	
	public static class Info {
		public final String name;
		public final int    program;
		public final int    version;
		
		public Info(String name, int program, int version) {
			this.name = name;
			this.program  = program;
			this.version = version;
		}
		
		// returns program name with version number
		public String getProgramVersion() {
			if (version == 0) return name;
			else return String.format("%s%d", name, version);
		}
		
		@Override
		public String toString() {
			return String.format("[%s %d %d]", name, program, version);
		}
		
		@Override
		public boolean equals(Object object) {
			if (object == null) return false;
			else {
				if (object instanceof Info) {
					Info that = (Info)object;
					return this.name.equals(that.name) && (this.program == that.program) && (this.version == that.version);
				} else {
					return false;
				}
			}
		}
	}
	
	public static class Decl {
		public enum Kind {TYPE, CONST};
		
		public final Kind   kind;
		public final String name;
		public final Type   type;
		public Decl(Kind kind, String name, Type type) {
			this.kind = kind;
			this.name = name;
			this.type = type;
		}
		
		public boolean isType() {
			return kind == Kind.TYPE;
		}
		public boolean isConstant() {
			return kind == Kind.CONST;
		}

		public Constant getConstant() {
			switch(kind) {
			case CONST:
				return ((Program.DeclConst)this).constant;
			case TYPE:
			default:
				throw new CompilerException(String.format("Unexpected kind %s", kind.toString()));
			}
		}
	}
	public static class DeclType extends Decl {
		public DeclType(String name, Type type) {
			super(Kind.TYPE, name, type);
		}
	}
	public static class DeclConst extends Decl {
		public final Constant constant;
		public DeclConst(String name, Type type, Constant constant) {
			super(Kind.CONST, name, type);
			this.constant = constant;
		}
	}

	// self
	public final Info              info;
	// depends
	public final List<Info>        depends;
	// declared types
	public final List<Decl>        declList;
	public final Map<String, Decl> declMap;
	
	public Program(Info info) {
		this.info      = info;
		this.depends   = new ArrayList<>();
		this.declList  = new ArrayList<>();
		this.declMap   = new TreeMap<>();
		
		all.put(this.info.getProgramVersion(), this);
	}
		
	public void addDepend(Info info) {
		depends.add(info);
	}
	public void addType(String name, Type type) {
		DeclType declType = new DeclType(name, type);
		declList.add(declType);
		
		if (declMap.containsKey(name)) {
			throw new ProgramException(String.format("Duplicate name.  name = %s", name));
		} else {
			declMap.put(name, declType);
		}
	}
	public void addConstant(String name, Type type, Constant constant) {
		DeclConst declConst = new DeclConst(name, type, constant);
		declList.add(declConst);

		if (declMap.containsKey(name)) {
			throw new ProgramException(String.format("Duplicate name.  name = %s", name));
		} else {
			declMap.put(name, declConst);
		}
	}
	public Info getDepend(String program) {
		for(Info e: depends) {
			if (program.equals(e.name)) {
				return e;
			}
		}
		throw new ProgramException(String.format("Unknown program = %s", program));
	}
	public boolean isLocalReference(TypeReference typeRef) {
		return  typeRef.value.program.equals(info.getProgramVersion());
	}
	public String getLocalRefName(TypeReference typeRef) {
		if (isLocalReference(typeRef)) {
			return typeRef.value.name;
		} else {
			return typeRef.value.toString();
		}
	}
	public boolean isLocalReference(ConstantReference constRef) {
		return  constRef.value.program.equals(info.getProgramVersion());
	}
	public String getLocalRefName(ConstantReference constRef) {
		if (constRef.value.program == null) {
			return constRef.value.name;
		} else {
			if (isLocalReference(constRef)) {
				return constRef.value.name;
			} else {
				return constRef.value.toString();
			}
		}
	}
	
	public String toString() {
		StringBuilder ret = new StringBuilder();
		ret.append(String.format("Program: [info: %s", info.toString()));
		{
			ret.append(String.format("  depends: (%d)[", depends.size()));
			if (!depends.isEmpty()) {
				StringBuilder t = new StringBuilder();
				for(Info e: depends) {
					t.append(String.format("  %s", e.toString()));
				}
				ret.append(t.substring(2));
			}
			ret.append("]");
		}
		{
			ret.append(String.format("  decl: (%d)[", declList.size()));
			if (!declList.isEmpty()) {
				StringBuilder t = new StringBuilder();
				for(Decl e: declList) {
					switch(e.kind) {
					case TYPE:
					{
						DeclType declType = (DeclType)e;
						String   name     = declType.name;
						Type     type     = declType.type;
						t.append(String.format("  [%s %s]", name, (type == null) ? "null" : type.toString()));
					}
						break;
					case CONST:
					{
						DeclConst declConst = (DeclConst)e;
						String    name      = declConst.name;
						Type      type      = declConst.type;
						Constant  constant  = declConst.constant;
						t.append(String.format("  [%s %s %s]", name, type.toString(), (constant == null) ? "null" : constant.toString()));
					}
						break;
					default:
						throw new ProgramException(String.format("Unknown kind.  kind = %s", e.kind));
					}
				}
				ret.append(t.substring(2));
			}
			ret.append("]");
		}
		ret.append("]");
		return ret.toString();
	}
	public static Program getProgram(String programName) {
		if (Program.all.containsKey(programName)) {
			return Program.all.get(programName);
		} else {
			throw new ProgramException(String.format("Unknown program.  programName = %s", programName));
		}
	}
	public Decl getDecl(String name) {
		if (declMap.containsKey(name)) {
			return declMap.get(name);
		} else {
			throw new ProgramException(String.format("Unknown decl.  name = %s", name));
		}
	}
	public Type getType(String name) {
		Decl decl = getDecl(name);
		switch (decl.kind) {
		case TYPE:
			return ((DeclType)decl).type;
		case CONST:
		default:
			throw new ProgramException(String.format("Unexpected kind.  kind = %s", decl.kind));
		}
	}
	public static Type dereference(TypeReference typeRef) {
		Program program = getProgram(typeRef.value.program);
		return program.getType(typeRef.value.name);
	}
	
	public boolean isRemoteType(Type type) {
		if (type.isReference()) {
			TypeReference typeReference = (TypeReference)type;
			Type baseType = typeReference.getBaseType();
			if (baseType.isReference()) {
				TypeReference baseTypeReference = (TypeReference)baseType;
				return baseTypeReference.value.program.compareTo(info.getProgramVersion()) != 0;
			} else {
				return false;
			}
		} else {
			return false;
		}
	}

}
