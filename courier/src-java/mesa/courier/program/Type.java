package mesa.courier.program;

public abstract class Type {
	public static enum Kind {
		// predefined
		BOOLEAN, BYTE, CARDINAL, LONG_CARDINAL, /* INTEGER, LONG_INTEGER,*/ STRING,
		UNSPECIFIED, UNSPECIFIED2, UNSPECIFIED3,
		// constructed
		ENUM, ARRAY, SEQUENCE, RECORD, CHOICE, PROCEDURE, ERROR,
		// reference
		REFERENCE,
	};
	
	public final Kind kind;
	
	protected Type(Kind kind) {
		this.kind = kind;
	}
	
	public boolean isPredefine() {
		switch(kind) {
		case BOOLEAN:
		case BYTE:
		case CARDINAL:
		case LONG_CARDINAL:
		case STRING:
		case UNSPECIFIED:
		case UNSPECIFIED2:
		case UNSPECIFIED3:
			return true;
		default:
			return false;
		}
	}
	public boolean isReference() {
		return kind == Kind.REFERENCE;
	}
	public Type getConcreteType() {
		Type type;
		for(type = this; type.isReference(); type = Program.dereference((TypeReference)type)) ;
		return type;
	}
	
	public Type getBaseType() {
		if (isReference()) {
			TypeReference typeRef = (TypeReference)this;
			if (typeRef.getConcreteType().isPredefine()) {
				// expand reference to predefined type
				return typeRef.getConcreteType();
			} else {
				return typeRef.getLastReference();
			}
		} else {
			return this;
		}
	}


	
	//
	// common class regarding type definition
	//
	public static class ReferencedType {
		public final String program;
		public final String name;
		
		public ReferencedType(String program, String name) {
			this.program = program;
			this.name    = name;
		}
		@Override
		public String toString() {
			return String.format("%s::%s", program, name);
		}
		@Override
		public boolean equals(Object o) {
			if (o == null) return false;
			if (!(o instanceof ReferencedType)) return false;
			
			ReferencedType that = (ReferencedType)o;
			return this.program.equals(that.program) && this.name.equals(that.name);
		}
	}
	
	public static class Field {
		public final String name;
		public final Type   type;
		public Field(String name, Type type) {
			this.name = name;
			this.type = type;
		}
		public String toString() {
			return String.format("%s: %s", name, type.toString());
		}
	}
	
	public static class Correspondence {
		public final String  id;
		public final long    numericValue;
		
		public Correspondence(String id, long numericValue) {
			this.id           = id;
			this.numericValue = numericValue;
		}
		
		public String toString() {
			return String.format("[%s  %d]", id, numericValue);
		}
	}
}
