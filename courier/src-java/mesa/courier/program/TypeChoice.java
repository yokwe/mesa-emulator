package mesa.courier.program;

import java.util.ArrayList;
import java.util.List;

public abstract class TypeChoice extends Type {
	public static class Candidate<T> {
		public final List<T> designators;
		public final Type    type;
		
		public Candidate(Type type) {
			this.type = type;
			this.designators = new ArrayList<>();
		}
		public void addDesignator(T designator) {
			designators.add(designator);
		}
		public String toString() {
			StringBuilder t = new StringBuilder();
			for(T designator: designators) {
				t.append(String.format(", %s", designator.toString()));
			}
			return String.format("[[%s] => %s]", t.substring(2), type.toString());
		}
	}
	
	public TypeChoice() {
		super(Kind.CHOICE);
	}
	
	public static class Typed extends TypeChoice {
		public final Type type;
		public final List<Candidate<String>> candidates = new ArrayList<>();
		
		public Typed(Type type) {
			this.type = type;
		}
		public void addCandidate(Candidate<String> candidate) {
			candidates.add(candidate);
		}
		@Override
		public String toString() {
			StringBuilder t = new StringBuilder();
			for(Candidate<String> candidate: candidates) {
				t.append(String.format(", %s", candidate.toString()));
			}
			return String.format("[%s %s [%s]]", kind, type, (t.length() == 0) ? "" : t.substring(2));
		}
	}
	public static class Anon extends TypeChoice {
		public final List<Candidate<Correspondence>> candidates = new ArrayList<>();
		public void addCandidate(Candidate<Correspondence> candidate) {
			candidates.add(candidate);
		}
		@Override
		public String toString() {
			StringBuilder t = new StringBuilder();
			for(Candidate<Correspondence> candidate: candidates) {
				t.append(String.format(", %s", candidate.toString()));
			}
			return String.format("[%s [%s]]", kind, (t.length() == 0) ? "" : t.substring(2));
		}
	}
}
