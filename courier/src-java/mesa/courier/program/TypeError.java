package mesa.courier.program;

import java.util.ArrayList;
import java.util.List;

public class TypeError extends Type {
	// argumentList resultList errorList
	public final List<Field>  paramList;
	
	public TypeError() {
		super(Kind.ERROR);
		paramList = new ArrayList<>();
	}
	public void addParam(Field field) {
		paramList.add(field);
	}

	@Override
	public String toString() {
		StringBuilder param = new StringBuilder();
		
		for(Field e: paramList) {
			param.append(String.format(", %s", e.toString()));
		}
		
		return String.format("[%s %s]", kind, ((param.length() == 0) ? "" : param.substring(2)));
	}
}
