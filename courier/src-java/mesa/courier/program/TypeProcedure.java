package mesa.courier.program;

import java.util.ArrayList;
import java.util.List;

public class TypeProcedure extends Type {
	// argumentList resultList errorList
	public final List<Field>  paramList;
	public final List<Field>  resultList;
	public final List<String> errroList;
	
	public TypeProcedure() {
		super(Kind.PROCEDURE);
		paramList  = new ArrayList<>();
		resultList = new ArrayList<>();
		errroList  = new ArrayList<>();
	}
	public void addParam(Field field) {
		paramList.add(field);
	}
	public void addResult(Field field) {
		resultList.add(field);
	}
	public void addError(String name) {
		errroList.add(name);
	}
	public String toString() {
		StringBuilder param  = new StringBuilder();
		StringBuilder result = new StringBuilder();
		StringBuilder error  = new StringBuilder();
		
		for(Field e: paramList) {
			param.append(String.format(", %s", e.toString()));
		}
		for(Field e: resultList) {
			result.append(String.format(", %s", e.toString()));
		}
		for(String e: errroList) {
			error.append(String.format(", %s", e.toString()));
		}
		
		return String.format("[%s arg:[%s] result:[%s] error:[%s]]", kind,
				((param.length() == 0) ? "" : param.substring(2)),
				((result.length() == 0)   ? "" : result.substring(2)),
				((error.length() == 0)    ? "" : error.substring(2)));
	}
}
