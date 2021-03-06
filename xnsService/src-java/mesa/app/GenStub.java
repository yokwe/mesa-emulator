package mesa.app;

import mesa.courier.compiler.*;
import mesa.courier.compiler.Compiler;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

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
				ProgramBuilder.buildConst(program);
			}
			
			Compiler compiler = new Compiler(program);
			compiler.genStub();
		}
		
		logger.debug("STOP");
	}
}