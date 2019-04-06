package mesa.app;

import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import mesa.courier.compiler.Compiler;
import mesa.courier.program.Program;
import mesa.courier.program.ProgramBuilder;

public class GenStub {
	protected static final Logger logger = LoggerFactory.getLogger(GenStub.class);
	
	public static void main(String[] args) throws Exception {
		logger.debug("START");
		ProgramBuilder.initialize("data/courier");
		
		for(int i = 0; i < args.length; i++) {
			Program program;
			{
				String name = args[i];
				logger.debug(String.format("name = %s", name));
				program = ProgramBuilder.getProgram(name);
			}
			
			Compiler compiler = new Compiler(program);
			compiler.genStub();
		}
		
		// TODO generate program version procedure mapping table
		for(Map.Entry<String, Compiler.Context> entry: Compiler.contextMap.entrySet()) {
			String           programVersion = entry.getKey();
			Compiler.Context context        = entry.getValue();
			
			if (context.procedureInfoList.isEmpty()) continue;
						
			for(Compiler.ProcedureInfo procedureInfo: context.procedureInfoList) {
				logger.info("{}  PROC  {}  {}", programVersion, procedureInfo.code, procedureInfo.name);
			}

			for(Compiler.ErrorInfo errorInfo: context.errorInfoList) {
				logger.info("{}  ERROR {}  {}", programVersion, errorInfo.code, errorInfo.name);
			}
}
		
		logger.debug("STOP");
	}
}