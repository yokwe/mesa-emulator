package mh.majuro.util;

import java.io.PrintWriter;

import org.slf4j.LoggerFactory;

import mh.majuro.UnexpectedException;

public class AutoIndentPrintWriter implements AutoCloseable {
	static final org.slf4j.Logger logger = LoggerFactory.getLogger(AutoIndentPrintWriter.class);

	private static final String INDENT = "    ";

	private final PrintWriter out;
	private int level = 0;

	public AutoIndentPrintWriter(PrintWriter out) {
		this.out = out;
	}
	public void close() {
		out.close();
	}
	public void println() {
		out.println();
	}
	
	private String stripString(String string) {
		StringBuffer ret = new StringBuffer();
		
		boolean insideDoubleQuote = false;
		
		int length = string.length();
		for(int i = 0; i < length; i++) {
			char c1 = string.charAt(i);
			char c2 = (i == length - 1) ? '\0' : string.charAt(i + 1);
			
			// line comment
			if ((!insideDoubleQuote) && c1 == '/' && c2 == '/') {
				break;
			}
			// ordinary comment
			if ((!insideDoubleQuote) && c1 == '/' && c2 == '*') {
				logger.error("Unexpected java comment");
				logger.error("  string {}!", string);
				throw new UnexpectedException("level < 0");
			}
			
			if (insideDoubleQuote) {
				//
			} else {
				// line comment
				if (c1 == '/' && c2 == '/') {
					break;
				}
				// ordinary comment
				if (c1 == '/' && c2 == '*') {
					logger.error("Unexpected java comment");
					logger.error("  string {}!", string);
					throw new UnexpectedException("level < 0");
				}
			}
			
			// \" can be appeared inside double quote
			if (c1 == '\\' && c2 == '"') {
				i++; // advance i to skip backslash
				continue;
			}
			
			if (c1 == '"') {
				insideDoubleQuote = !insideDoubleQuote;
				continue;
			}
			
			if (!insideDoubleQuote) {
				switch(c1) {
				case '{':
				case '}':
				case '(':
				case ')':
					ret.append(c1);
					break;
				default:
					break;
				}
			}
		}
		
		return ret.toString();
	}
	public void println(String string) {
		String strippedString = stripString(string);
		
		// adjust level
		for(char c: strippedString.toCharArray()) {
			switch (c) {
			case '}':
				level--;
				if (level < 0) {
					logger.error("level < 0");
					logger.error("  string {}!", string);
					logger.error("  strippedString {}!", strippedString);
					throw new UnexpectedException("level < 0");
				}
				break;
			default:
				break;
			}
		}

		for(int i = 0; i < level; i++) out.print(INDENT);
		out.println(string);
		
		// adjust level
		for(char c: strippedString.toCharArray()) {
			switch (c) {
			case ')':
				level--;
				if (level < 0) {
					logger.error("level < 0");
					logger.error("  string {}!", string);
					logger.error("  strippedString {}!", strippedString);
					throw new UnexpectedException("level < 0");
				}
				break;
			case '{':
			case '(':
				level++;
				break;
			default:
				break;
			}
		}
	}
	public void println(String format, Object... args) {
		String string = String.format(format, args);
		println(string);
	}
}