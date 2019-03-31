package mesa.courier.compiler;

import java.util.ArrayList;
import java.util.List;

public class ColumnLayout {
	public static List<String> layoutStringString(List<String> c1, List<String> c2) {
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
		
		List<String> ret = new ArrayList<>();
		for(int i = 0; i < size; i++) {
			ret.add(String.format(format, c1.get(i).trim(), c2.get(i).trim()));
		}
		return ret;
	}

	public static List<String> layoutStringString(List<String> c1, List<String> c2, List<String> c3) {
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
		
		List<String> ret = new ArrayList<>();
		for(int i = 0; i < size; i++) {
			ret.add(String.format(format, c1.get(i).trim(), c2.get(i).trim(), c3.get(i).trim()));
		}
		return ret;
	}

	public static List<String> layoutEnumElement(List<String> c1, List<Integer> c2) {
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
		
		List<String> ret = new ArrayList<>();
		for(int i = 0; i < size; i++) {
			ret.add(String.format(format, c1.get(i).trim(), c2.get(i).intValue()));
		}
		return ret;
	}
}