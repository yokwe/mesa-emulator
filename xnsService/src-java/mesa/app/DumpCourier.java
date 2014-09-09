package mesa.app;

import mesa.courier.compiler.Program;
import mesa.courier.compiler.ProgramBuilder;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class DumpCourier {
	protected static final Logger logger = LoggerFactory.getLogger(DumpCourier.class);
	
	public static void main(String[] args) throws Exception {
		logger.debug("START");
		ProgramBuilder.initialize("data/courier");
		
		for(int i = 0; i < args.length; i++) {
			String name = args[i];
			logger.debug(String.format("name = %s", name));
			Program program = ProgramBuilder.getProgram(name);
			ProgramBuilder.buildConst(program);
			logger.debug("program = {}", program.toString());
		}
		logger.debug("STOP");
	}
}
