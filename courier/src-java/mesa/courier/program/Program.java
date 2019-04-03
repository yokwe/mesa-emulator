package mesa.courier.program;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

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
	
	public static class DeclType {
		public final String name;
		public final Type   type;
		public DeclType(String name, Type type) {
			this.name = name;
			this.type = type;
		}
	}
	public static class DeclConst {
		public final String   name;
		public final Type     type;
		public final Constant constant;
		public DeclConst(String name, Type type, Constant constant) {
			this.name     = name;
			this.type     = type;
			this.constant = constant;
		}
	}
	// self
	public final Info                   info;
	// depends
	public final List<Info>             depends;
	// declared types
	public final List<DeclType>         typeList;
	public final Map<String, DeclType>  typeMap;
	// declared constants
	public final List<DeclConst>        constList;
	public final Map<String, DeclConst> constMap;
	
	public Program(Info info) {
		this.info      = info;
		this.depends   = new ArrayList<>();
		this.typeList  = new ArrayList<>();
		this.typeMap   = new TreeMap<>();
		this.constList = new ArrayList<>();
		this.constMap  = new TreeMap<>();
		
		all.put(this.info.getProgramVersion(), this);
	}
		
	public void addDepend(Info info) {
		depends.add(info);
	}
	public void addType(String name, Type type) {
		DeclType declType = new DeclType(name, type);
		typeList.add(declType);
		typeMap.put(name, declType);
	}
	public void addConstant(String name, Type type, Constant constant) {
		DeclConst declConst = new DeclConst(name, type, constant);
		constList.add(declConst);
		constMap.put(name, declConst);
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
			ret.append(String.format("  types: (%d)[", typeList.size()));
			if (!typeList.isEmpty()) {
				StringBuilder t = new StringBuilder();
				for(DeclType e: typeList) {
					String name = e.name;
					Type type   = e.type;
					t.append(String.format("  [%s %s]", name, (type == null) ? "null" : type.toString()));
				}
				ret.append(t.substring(2));
			}
			ret.append("]");
		}
		{
			ret.append(String.format("  constants: (%d)[", constList.size()));
			if (!constList.isEmpty()) {
				StringBuilder t = new StringBuilder();
				for(DeclConst e: constList) {
					String   name     = e.name;
					Type     type     = e.type;
					Constant constant = e.constant;
					t.append(String.format("  [%s %s %s]", name, type.toString(), (constant == null) ? "null" : constant.toString()));
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
	public Type getType(String typeName) {
		if (typeMap.containsKey(typeName)) {
			return typeMap.get(typeName).type;
		} else {
			throw new ProgramException(String.format("Unknown type.  typeName = %s", typeName));
		}
	}
	public static Type dereference(TypeReference typeRef) {
		Program program = getProgram(typeRef.value.program);
		return program.getType(typeRef.value.name);
	}
}
