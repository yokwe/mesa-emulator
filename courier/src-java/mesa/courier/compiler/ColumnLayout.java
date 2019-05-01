package mesa.courier.compiler;

import java.util.List;

public class ColumnLayout {
	public static void layoutStringString(LinePrinter out, List<String> c1, List<String> c2) {
		if (c1.size() != c2.size()) {
			throw new CompilerException(String.format("Unexpected size  c1 = %d  c2 = %d", c1.size(), c2.size()));
		}
		int size = c1.size();
		
		int w1 = 0;
		for(String value: c1) {
			int length = value.trim().length();
			if (w1 < length) w1 = length;
		}
		String format = String.format("%%%ds %%s", -w1);
		
		for(int i = 0; i < size; i++) {
			out.line(String.format(format, c1.get(i).trim(), c2.get(i).trim()));
		}
	}

	public static void layoutStringString(LinePrinter out, List<String> c1, List<String> c2, List<String> c3) {
		if (c1.size() != c2.size()) {
			throw new CompilerException(String.format("Unexpected size  c1 = %d  c2 = %d", c1.size(), c2.size()));
		}
		if (c1.size() != c3.size()) {
			throw new CompilerException(String.format("Unexpected size  c1 = %d  c3 = %d", c1.size(), c3.size()));
		}
		int size = c1.size();
		
		int w1 = 0;
		for(String value: c1) {
			int length = value.trim().length();
			if (w1 < length) w1 = length;
		}
		int w2 = 0;
		for(String value: c2) {
			int length = value.trim().length();
			if (w2 < length) w2 = length;
		}
		String format = String.format("%%%ds %%%ds %%s", -w1, -w2);
		
		for(int i = 0; i < size; i++) {
			out.line(String.format(format, c1.get(i).trim(), c2.get(i).trim(), c3.get(i).trim()));
		}
	}

	public static void layoutStringStringRight(LinePrinter out, List<String> c1, List<String> c2, List<String> c3) {
		if (c1.size() != c2.size()) {
			throw new CompilerException(String.format("Unexpected size  c1 = %d  c2 = %d", c1.size(), c2.size()));
		}
		if (c1.size() != c3.size()) {
			throw new CompilerException(String.format("Unexpected size  c1 = %d  c3 = %d", c1.size(), c3.size()));
		}
		int size = c1.size();
		
		int w1 = 0;
		for(String value: c1) {
			int length = value.trim().length();
			if (w1 < length) w1 = length;
		}
		int w2 = 0;
		for(String value: c2) {
			int length = value.trim().length();
			if (w2 < length) w2 = length;
		}
		int w3 = 0;
		for(String value: c3) {
			int length = value.trim().length();
			if (w3 < length) w3 = length;
		}
		String format = String.format("%%%ds %%%ds %%%ds", -w1, -w2, w3);
		
		for(int i = 0; i < size; i++) {
			out.line(String.format(format, c1.get(i).trim(), c2.get(i).trim(), c3.get(i).trim()));
		}
	}

	public static void layoutStringString(LinePrinter out, List<String> c1, List<String> c2, List<String> c3, List<String> c4) {
		if (c1.size() != c2.size()) {
			throw new CompilerException(String.format("Unexpected size  c1 = %d  c2 = %d", c1.size(), c2.size()));
		}
		if (c1.size() != c3.size()) {
			throw new CompilerException(String.format("Unexpected size  c1 = %d  c3 = %d", c1.size(), c3.size()));
		}
		if (c1.size() != c4.size()) {
			throw new CompilerException(String.format("Unexpected size  c1 = %d  c4 = %d", c1.size(), c3.size()));
		}
		int size = c1.size();
		
		int w1 = 0;
		for(String value: c1) {
			int length = value.trim().length();
			if (w1 < length) w1 = length;
		}
		int w2 = 0;
		for(String value: c2) {
			int length = value.trim().length();
			if (w2 < length) w2 = length;
		}
		int w3 = 0;
		for(String value: c3) {
			int length = value.trim().length();
			if (w3 < length) w3 = length;
		}
		String format = String.format("%%%ds %%%ds %%%ds %%s", -w1, -w2, -w3);
		
		for(int i = 0; i < size; i++) {
			out.line(String.format(format, c1.get(i).trim(), c2.get(i).trim(), c3.get(i).trim(), c4.get(i).trim()));
		}
	}

	public static void layoutEnumElement(LinePrinter out, List<String> c1, List<Integer> c2) {
		if (c1.size() != c2.size()) {
			throw new CompilerException(String.format("Unexpected size  c1 = %d  c2 = %d", c1.size(), c2.size()));
		}
		int size = c1.size();
		
		int w1 = 0;
		for(String value: c1) {
			int length = value.trim().length();
			if (w1 < length) w1 = length;
		}
		int w2 = 0;
		for(Integer value: c2) {
			String string = String.format("%d", value);
			int length = string.length();
			if (w2 < length) w2 = length;
		}
		String format = String.format("%%%ds = %%%ds,", -w1, w2);
		
		for(int i = 0; i < size; i++) {
			out.line(String.format(format, c1.get(i).trim(), c2.get(i).intValue()));
		}
	}
}