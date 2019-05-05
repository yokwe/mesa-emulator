package mesa.courier.program;

import mesa.courier.compiler.CompilerException;
import mesa.courier.program.TypeChoice.Candidate;

public abstract class Type {
	public static enum Kind {
		// predefined
		BLOCK, BOOLEAN, BYTE, CARDINAL, LONG_CARDINAL, /* INTEGER, LONG_INTEGER,*/ STRING,
		UNSPECIFIED, UNSPECIFIED2, UNSPECIFIED3,
		// constructed
		ENUM, ARRAY, SEQUENCE, RECORD, CHOICE, PROCEDURE, ERROR, MACHINE,
		// reference
		REFERENCE,
	};
	
	public final Kind kind;
	
	protected Type(Kind kind) {
		this.kind = kind;
	}
	
	public boolean isPredefine() {
		switch(kind) {
		case BLOCK:
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
	public Type getTrueConcreteType() {
		Type type = getConcreteType();
		switch(type.kind) {
		case BLOCK:
		case BOOLEAN:
		case BYTE:
		case CARDINAL:
		case LONG_CARDINAL:
		case STRING:
		case UNSPECIFIED:
		case UNSPECIFIED2:
		case UNSPECIFIED3:
		case ENUM:
		case MACHINE:
			return type;
		case ARRAY:
		{
			TypeArray typeArray = (TypeArray)type;
			return new TypeArray(typeArray.size, typeArray.type.getConcreteType());
		}
		case SEQUENCE:
		{
			TypeSequence typeSequence = (TypeSequence)type;
			return new TypeSequence(typeSequence.size, typeSequence.type.getConcreteType());
		}
		case RECORD:
		{
			TypeRecord newType = new TypeRecord();
			TypeRecord typeRecord = (TypeRecord)type;			
			for(Field field: typeRecord.fields) {
				newType.addField(new Field(field.name, field.type.getConcreteType()));
			}
			return newType;
		}
		case CHOICE:
		{
			TypeChoice typeChoice = (TypeChoice)type;
			if (typeChoice instanceof TypeChoice.Anon) {
				TypeChoice.Anon anon = (TypeChoice.Anon)typeChoice;
				TypeChoice.Anon newType = new TypeChoice.Anon();
				for(Candidate<Correspondence> candidate: anon.candidates) {
					Candidate<Correspondence> newCandidate = new Candidate<>(candidate.type.getConcreteType());
					for(Correspondence designator: candidate.designators) {
						newCandidate.addDesignator(designator);
					}
					newType.addCandidate(candidate);
				}
				return newType;
			}
			if (typeChoice instanceof TypeChoice.Typed) {
				TypeChoice.Typed typed = (TypeChoice.Typed)typeChoice;
				TypeChoice.Typed newType = new TypeChoice.Typed(typed.type.getConcreteType());
				for(Candidate<String> candidate: typed.candidates) {
					Candidate<String> newCandidate = new Candidate<>(candidate.type.getConcreteType());
					for(String designator: candidate.designators) {
						newCandidate.addDesignator(designator);
					}
					newType.addCandidate(candidate);
				}
				return newType;
			}
		}
			break;
		case PROCEDURE:
		{
			TypeProcedure newType = new TypeProcedure();
			TypeProcedure typeProcedure = (TypeProcedure)type;
			
			for(Field field: typeProcedure.paramList) {
				newType.addParam(new Field(field.name, field.type.getConcreteType()));
			}
			for(Field field: typeProcedure.resultList) {
				newType.addResult(new Field(field.name, field.type.getConcreteType()));
			}
			return newType;
		}
		case ERROR:
		{
			TypeError newType = new TypeError();
			TypeError typeError = (TypeError)type;
			for(Field field: typeError.paramList) {
				newType.addParam(new Field(field.name, field.type.getConcreteType()));
			}
			return newType;
		}
		default:
			break;
		}
		throw new CompilerException(String.format("Unexpected type %s", type));
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
	
	public static class MDField {
		public final String name;
		public final int    start;
		public final int    stop;
		public final Type   type;
		public MDField(String name, int start, int stop, Type type) {
			this.name  = name;
			this.start = start;
			this.stop  = stop;
			this.type  = type;
		}
		public String toString() {
			return String.format("%s: (%d..%d) %s", name, start, stop, type.toString());
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
