package mesa.courier.compiler;

import java.io.PrintWriter;

public class LinePrinter implements AutoCloseable {
	public static final String INDENT = "    ";

	private final PrintWriter out;
	private int level = 0;

	public LinePrinter(PrintWriter out) {
		this.out = out;
	}
	public void close() {
		out.close();
	}

	public LinePrinter format(String format, Object... args) {
		return line(String.format(format, args));
	}
	public LinePrinter line(String message) {
		if (message.length() == 0) {
			out.println();
		} else if (
				// for class or struct
				message.equals("public:") || message.equals("private:") ||
				// for switch
				(message.startsWith("case ") && message.endsWith(":")) || message.equals("default:")) {
			for(int i = 1; i < level; i++) out.print(INDENT);
			out.println(message);
		} else {
			if (message.startsWith("}")) {
				level--;
				if (level < 0) throw new CompilerException("level < 0");
			}
			for(int i = 0; i < level; i++) out.print(INDENT);
			out.println(message);
			if (message.endsWith("{")) {
				level++;
			}
		}
		return this;
	}
	public LinePrinter line(String... args) {
		for(String message: args) {
			line(message);
		}
		return this;
	}
	public LinePrinter line() {
		out.println();
		return this;
	}
}