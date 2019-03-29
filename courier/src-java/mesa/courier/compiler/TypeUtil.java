package mesa.courier.compiler;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import mesa.courier.program.Type;
import mesa.courier.program.TypeArray;
import mesa.courier.program.TypeSequence;

public class TypeUtil {
	interface TypeNameToString {
		String[] process(Type type, String name);
	}
	
	static class ToString {
		ToString(Type.Kind kind) {
			this.kind = kind;
			genSerialize   = (t, n) -> {throw new CompilerException(String.format("Unexpeced %s", kind));};
			genDeserialize = (t, n) -> {throw new CompilerException(String.format("Unexpeced %s", kind));};
			genToString    = (t, n) -> {throw new CompilerException(String.format("Unexpeced %s", kind));};
		}
		
		final Type.Kind        kind;
		
		TypeNameToString genSerialize;
		TypeNameToString genDeserialize;
		TypeNameToString genToString;
	}

	static Map<Type.Kind, ToString> toStringMap = new TreeMap<>();
	static {
		// predefined
		toStringMap.put(Type.Kind.BOOLEAN,       new ToStringBOOLEAN());
		toStringMap.put(Type.Kind.BYTE,          new ToStringBYTE());
		toStringMap.put(Type.Kind.CARDINAL,      new ToStringCARDINAL());
		toStringMap.put(Type.Kind.LONG_CARDINAL, new ToStringLONG_CARDINAL());
		toStringMap.put(Type.Kind.STRING,        new ToStringSTRING());
		toStringMap.put(Type.Kind.UNSPECIFIED,   new ToStringUNSPECIFIED());
		toStringMap.put(Type.Kind.UNSPECIFIED2,  new ToStringUNSPECIFIED2());
		toStringMap.put(Type.Kind.UNSPECIFIED3,  new ToStringUNSPECIFIED3());
		toStringMap.put(Type.Kind.ENUM,          new ToStringENUM());
		// constructed
		toStringMap.put(Type.Kind.ARRAY,         new ToStringARRAY());
		toStringMap.put(Type.Kind.BLOCK,         new ToStringBLOCK());
		toStringMap.put(Type.Kind.SEQUENCE,      new ToStringSEQUENCE());
		toStringMap.put(Type.Kind.RECORD,        new ToStringRECORD());
		toStringMap.put(Type.Kind.CHOICE,        new ToStringCHOICE());
		toStringMap.put(Type.Kind.PROCEDURE,     new ToStringPROCEDURE());
		toStringMap.put(Type.Kind.ERROR,         new ToStringERROR());
		// reference
		toStringMap.put(Type.Kind.REFERENCE,     new ToStringREFERENCE());
	}


	
	static ToString getToString(Type type) {
		ToString toString = toStringMap.get(type.kind);
		if (toString == null) {
			throw new CompilerException(String.format("Unexpected type %s", type.toString()));
		}
		return toString;
	}
	
	public static String[] genSerialize(Type type, String name) {
		if (type.isReference()) type = type.getConcreteType();
		return getToString(type).genSerialize.process(type, name);
	}
	public static String[] genDeserialize(Type type, String name) {
		if (type.isReference()) type = type.getConcreteType();
		return getToString(type).genDeserialize.process(type, name);
	}
	public static String[] genToString(Type type, String name) {
		if (type.isReference()) type = type.getConcreteType();
		return getToString(type).genToString.process(type, name);
	}
	
	static String[] toArray(String...args) {
		return args;
	}
	
	
	static class ToStringBOOLEAN extends ToString {
		ToStringBOOLEAN() {
			super(Type.Kind.BOOLEAN);
			genSerialize   = (type, name) -> {
				return toArray(
						"{",
						String.format("quint16 t = (%s ? 1 : 0);", name),
						String.format("block.serialize16(t);"),
						"}"
					);
			};
			genDeserialize = (type, name) -> {
				return toArray(
						"{",
						"quint16 t;",
						"block.deserialize16(t);",
						String.format("%s = (t != 0);", name),
						"}"
						);
			};
		}
	}
	static class ToStringBYTE extends ToString {
		ToStringBYTE() {
			super(Type.Kind.BYTE);
			genSerialize   = (type, name) -> {
				return toArray(String.format("block.serialize8(%s);", name));
			};
			genDeserialize = (type, name) -> {
				return toArray(String.format("block.deserialize8(%s);", name));
			};
		}
	}
	static class ToStringCARDINAL extends ToString {
		ToStringCARDINAL() {
			super(Type.Kind.CARDINAL);
			genSerialize   = (type, name) -> {
				return toArray(String.format("block.serialize16(%s);", name));
			};
			genDeserialize = (type, name) -> {
				return toArray(String.format("block.deserialize16(%s);", name));
			};
		}
	}
	static class ToStringLONG_CARDINAL extends ToString {
		ToStringLONG_CARDINAL() {
			super(Type.Kind.LONG_CARDINAL);
			genSerialize   = (type, name) -> {
				return toArray(String.format("block.serialize32(%s);", name));
			};
			genDeserialize = (type, name) -> {
				return toArray(String.format("block.deserialize32(%s);", name));
			};
		}
	}
	static class ToStringUNSPECIFIED extends ToString {
		ToStringUNSPECIFIED() {
			super(Type.Kind.UNSPECIFIED);
			genSerialize   = (type, name) -> {
				return toArray(String.format("block.serialize16(%s);", name));
			};
			genDeserialize = (type, name) -> {
				return toArray(String.format("block.deserialize16(%s);", name));
			};
		}
	}
	static class ToStringUNSPECIFIED2 extends ToString {
		ToStringUNSPECIFIED2() {
			super(Type.Kind.UNSPECIFIED2);
			genSerialize   = (type, name) -> {
				return toArray(String.format("block.serialize32(%s);", name));
			};
			genDeserialize = (type, name) -> {
				return toArray(String.format("block.deserialize32(%s);", name));
			};
		}
	}
	static class ToStringUNSPECIFIED3 extends ToString {
		ToStringUNSPECIFIED3() {
			super(Type.Kind.UNSPECIFIED3);
			genSerialize   = (type, name) -> {
				return toArray(String.format("block.serialize48(%s);", name));
			};
			genDeserialize = (type, name) -> {
				return toArray(String.format("block.deserialize48(%s);", name));
			};
		}
	}
	static class ToStringENUM extends ToString {
		ToStringENUM() {
			super(Type.Kind.ENUM);
			genSerialize   = (type, name) -> {
				return toArray(
						"{",
						String.format("quint16 t = %s;", name),
						String.format("block.serialize16(t);"),
						"}"
					);
			};
			genDeserialize = (type, name) -> {
				return toArray(
						"{",
						"quint16 t;",
						"block.deserialize16(t);",
						String.format("%s = t;", name),
						"}"
						);
			};
		}
	}
	static class ToStringSTRING extends ToString {
		ToStringSTRING() {
			super(Type.Kind.STRING);
			genSerialize   = (type, name) -> {
				return toArray(String.format("block.serialize(%s);", name));
			};
			genDeserialize = (type, name) -> {
				return toArray(String.format("block.deserialize(%s);", name));
			};
		}
	}
	static class ToStringBLOCK extends ToString {
		ToStringBLOCK() {
			super(Type.Kind.BLOCK);
			genSerialize   = (type, name) -> {
				return toArray(String.format("block.serialize(%s);", name));
			};
			genDeserialize = (type, name) -> {
				return toArray(String.format("block.deserialize(%s);", name));
			};
		}
	}
	static class ToStringARRAY extends ToString {
		ToStringARRAY() {
			super(Type.Kind.ARRAY);			
			genSerialize   = (type, name) -> {
				List<String> lines = new ArrayList<String>();
				TypeArray typeArray = (TypeArray)type;
				Type elementType = typeArray.type.getConcreteType();
				String elementName = String.format("%s[i]", name);
				lines.add("{");
				lines.add(String.format("for(quint16 i = 0; i < %s.maxSize; i++) {", name));
				lines.addAll(Arrays.asList(TypeUtil.genSerialize(elementType, elementName)));
				lines.add("}");
				lines.add("}");
				return lines.toArray(new String[0]);
			};
			genDeserialize = (type, name) -> {
				List<String> lines = new ArrayList<String>();
				TypeArray typeArray = (TypeArray)type;
				Type elementType = typeArray.type.getConcreteType();
				String elementName = String.format("%s[i]", name);
				lines.add("{");
				lines.add(String.format("for(quint16 i = 0; i < %s.maxSize; i++) {", name));
				lines.addAll(Arrays.asList(TypeUtil.genDeserialize(elementType, elementName)));
				lines.add("}");
				lines.add("}");
				return lines.toArray(new String[0]);
			};
		}
	}
	static class ToStringSEQUENCE extends ToString {
		ToStringSEQUENCE() {
			super(Type.Kind.SEQUENCE);
			genSerialize   = (type, name) -> {
				List<String> lines = new ArrayList<String>();
				TypeSequence typeSequence = (TypeSequence)type;
				Type elementType = typeSequence.type.getConcreteType();
				String elementName = String.format("%s[i]", name);
				lines.add("{");
				lines.add(String.format("quint16 size = %s.getSize();", name));
				lines.add("block.serialize16(size);");
				lines.add("for(quint16 i = 0; i < size; i++) {");
				lines.addAll(Arrays.asList(TypeUtil.genSerialize(elementType, elementName)));
				lines.add("}");
				lines.add("}");
				return lines.toArray(new String[0]);
			};
			genDeserialize = (type, name) -> {
				List<String> lines = new ArrayList<String>();
				TypeSequence typeSequence = (TypeSequence)type;
				Type elementType = typeSequence.type.getConcreteType();
				String elementName = String.format("%s[i]", name);
				lines.add("{");
				lines.add("quint16 size;");
				lines.add("block.deserialize16(size);");
				lines.add(String.format("%s.setSize(size);", name));
				lines.add("for(quint16 i = 0; i < size; i++) {");
				lines.addAll(Arrays.asList(TypeUtil.genDeserialize(elementType, elementName)));
				lines.add("}");
				lines.add("}");
				return lines.toArray(new String[0]);
			};
		}
	}
	static class ToStringRECORD extends ToString {
		ToStringRECORD() {
			super(Type.Kind.RECORD);
			genSerialize   = (type, name) -> {
				return toArray(String.format("block.serialize(%s);", name));
			};
			genDeserialize = (type, name) -> {
				return toArray(String.format("block.deserialize(%s);", name));
			};
		}
	}
	static class ToStringCHOICE extends ToString {
		ToStringCHOICE() {
			super(Type.Kind.CHOICE);
			genSerialize   = (type, name) -> {
				return toArray(String.format("block.serialize(%s);", name));
			};
			genDeserialize = (type, name) -> {
				return toArray(String.format("block.deserialize(%s);", name));
			};
		}
	}
	static class ToStringPROCEDURE extends ToString {
		ToStringPROCEDURE() {
			super(Type.Kind.PROCEDURE);
		}
	}
	static class ToStringERROR extends ToString {
		ToStringERROR() {
			super(Type.Kind.ERROR);
		}
	}
	static class ToStringREFERENCE extends ToString {
		ToStringREFERENCE() {
			super(Type.Kind.REFERENCE);
		}
	}
}
