package mesa.courier.program;

public abstract class Constant {
	public static enum Kind {
		// predefined
		BOOLEAN, NUMBER, STRING,
		// constructed
		ARRAY, RECORD, EMPTY, CHOICE,
		// reference
		REFERENCE,
	};

	public final Kind kind;
	
	protected Constant(Kind kind) {
		this.kind = kind;
	}
	
	//
	// common class regarding constant definition
	//
	public static class ReferencedConstant {
		public final String program;
		public final String name;
		
		public ReferencedConstant(String program, String name) {
			this.program = program;
			this.name    = name;
		}
		public ReferencedConstant(String name) {
			this.program = null;
			this.name    = name;
		}
		public String toString() {
			if (program == null) return name;
			else return String.format("%s::%s", program, name);
		}
	}
	
	public static class Component {
		public final String   name;
		public final Constant constant;
		public Component(String name, Constant constant) {
			this.name = name;
			this.constant = constant;
		}
		public String toString() {
			return String.format("%s: %s", name, constant.toString());
		}
	}
	

}
