package mesa.app;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import mesa.courier.compiler.CompilerRaw;
import mesa.courier.program.Program;
import mesa.courier.program.ProgramBuilder;

public class GenStubRaw {
	protected static final Logger logger = LoggerFactory.getLogger(GenStubRaw.class);
	
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
			
			CompilerRaw compiler = new CompilerRaw(program);
			compiler.genStub();
		}
		
		logger.debug("STOP");
	}
}