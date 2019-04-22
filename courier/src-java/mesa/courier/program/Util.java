package mesa.courier.program;

import java.util.Map;
import java.util.TreeMap;

public class Util {
	public static long parseLong(String text) {
		int length = text.length();
		
		try {
			switch(text.charAt(length - 1)) {
			case 'b':
			case 'B':
				return Long.parseLong(text.substring(0, length - 1), 8);
			case 'x':
			case 'X':
				return Long.parseLong(text.substring(0, length - 1), 16);
			case 'd':
			case 'D':
				return Long.parseLong(text.substring(0, length - 1), 10);
			default:
				return Long.parseLong(text, 10);
			}
		} catch (NumberFormatException e) {
			throw new ProgramException(String.format("text = %s", text), e);
		}
	}
	public static String toJavaConstName(String name) {
		StringBuilder ret = new StringBuilder();
		boolean lastCharIsUpper = false;
		for(int i = 0; i < name.length(); i++) {
			char c = name.charAt(i);

			if (Character.isLowerCase(c)) {
				ret.append(Character.toUpperCase(c));
				lastCharIsUpper = false;
			} else if (Character.isDigit(c)) {
				ret.append(c);
				lastCharIsUpper = false;
			} else if (Character.isUpperCase(c)) {
				if (lastCharIsUpper) {
					ret.append(c);
				} else {
					ret.append('_').append(c);
				}
				lastCharIsUpper = true;
			} else {
				throw new ProgramException(String.format("Unknown character type = %c - %04X", c, c));
			}
		}
		return ret.toString();
	}
	
	private static Map<String, String> sanitizeMap = new TreeMap<>();
	static {
		sanitizeMap.put("abort",  "abort__");
		sanitizeMap.put("and",    "and__");
		sanitizeMap.put("not",    "not__");
		sanitizeMap.put("or",     "or__");
		sanitizeMap.put("return", "return__");
	}

	public static String sanitizeSymbol(String symbol) {
		if (sanitizeMap.containsKey(symbol)) {
			return sanitizeMap.get(symbol);
		}
		return symbol;
	}

}
