package mesa.app;

import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import mesa.courier.compiler.Compiler;
import mesa.courier.compiler.CompilerException;
import mesa.courier.compiler.LinePrinter;
import mesa.courier.program.Program;
import mesa.courier.program.ProgramBuilder;

public class GenStub {
	protected static final Logger logger = LoggerFactory.getLogger(GenStub.class);
	
	public static void main(String[] args) throws Exception {
		logger.debug("START");
		ProgramBuilder.initialize("data/courier/custom");
		ProgramBuilder.initialize("data/courier/XNSonUX");
		ProgramBuilder.initialize("data/courier/test");
		
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
		{
			String programName = "Programs";
			String pathc = String.format("%s%s.cpp", Compiler.STUB_DIR_PATH, "Programs");
			String pathh = String.format("%s%s.h",   Compiler.STUB_DIR_PATH, "Programs");
			logger.info(String.format("pathc = %s", pathc));
			logger.info(String.format("pathh = %s", pathh));
			
			try (LinePrinter outh = new LinePrinter(new PrintWriter(pathh));) {
				
				outh.format("#ifndef STUB_%s_H__", programName);
				outh.format("#define STUB_%s_H__", programName);
				outh.line();
				// include courier header
				outh.line("#include \"../courier/Programs.h\"");
				
				// output namespace
				outh.line();
				outh.format("namespace %s {", "Courier");
				outh.format("namespace %s {", "Stub");
				outh.format("namespace %s {", programName);
				
				outh.line("QList<Courier::Programs::Info> infoList = {");
				
				for(Map.Entry<String, Compiler.Context> entry: Compiler.contextMap.entrySet()) {
					Program program = entry.getValue().program;
					Compiler.Context context        = entry.getValue();
					
					for(Compiler.ProcedureInfo procedureInfo: context.procedureInfoList) {
						// 	static void     addProgram(int programCode, QString programName, int versionCode, int procudureCode, QString procedureName);
						outh.format(" {%2d, \"%s\", %2d, %2d, \"%s\"},", program.info.program, program.info.name, program.info.version, procedureInfo.code, procedureInfo.name);
					}
				}

				outh.line("};");

				// Close namespace
				outh.line("}");
				outh.line("}");
				outh.line("}");
				// #endif
				outh.line("#endif");
			} catch (FileNotFoundException e) {
				throw new CompilerException("FileNotFoundException", e);
			}
		}
		for(Map.Entry<String, Compiler.Context> entry: Compiler.contextMap.entrySet()) {
			Program myProgram = entry.getValue().program;
			
			String programName = myProgram.info.name;
			int    program     = myProgram.info.program;
			int    version     = myProgram.info.version;
					
			Compiler.Context context        = entry.getValue();
			
			for(Compiler.ProcedureInfo procedureInfo: context.procedureInfoList) {
				logger.info("{}  {}  {}  PROC  {}  {}", program, programName, version, procedureInfo.code, procedureInfo.name);
			}

			for(Compiler.ErrorInfo errorInfo: context.errorInfoList) {
				logger.info("{}  {}  {}  ERROR {}  {}", program, programName, version, errorInfo.code, errorInfo.name);
			}
		}
		
		logger.debug("STOP");
	}
}