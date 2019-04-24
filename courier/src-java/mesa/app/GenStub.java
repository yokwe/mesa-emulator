package mesa.app;

import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import mesa.courier.compiler.ColumnLayout;
import mesa.courier.compiler.Compiler;
import mesa.courier.compiler.Compiler.ProcedureInfo;
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
		
		// generate courier/stub/Programs.h
		genPrograms();
		
		genService();
		// TODO generate program version procedure mapping table
		for(Map.Entry<String, Compiler.Context> entry: Compiler.contextMap.entrySet()) {
			Program myProgram = entry.getValue().program;
			
			String programName = myProgram.info.name;
			int    program     = myProgram.info.program;
			int    version     = myProgram.info.version;
					
			Compiler.Context context        = entry.getValue();
			
			for(Compiler.ProcedureInfo procedureInfo: context.procedureInfoList) {
				logger.info("{}  {}  {}  PROC  {}  {}", program, programName, version, procedureInfo.code, procedureInfo.name);
			}
		}
		
		logger.debug("STOP");
	}
	
	private static void genPrograms() {
		String programName = "Programs";
		String pathc = String.format("%s%s.cpp", Compiler.STUB_DIR_PATH, programName);
		String pathh = String.format("%s%s.h",   Compiler.STUB_DIR_PATH, programName);
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
				Program          program = entry.getValue().program;
				Compiler.Context context = entry.getValue();
				
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
	
	private static void genService() {
		for(Map.Entry<String, Compiler.Context> entry: Compiler.contextMap.entrySet()) {
			Program          program = entry.getValue().program;
			Compiler.Context context = entry.getValue();
			
			if (context.procedureInfoList.isEmpty()) continue;
			
			String programName = program.info.getProgramVersion();
			String serviceName = String.format("service_%s", programName);
			String pathc = String.format("%s%s.cpp", Compiler.STUB_DIR_PATH, serviceName);
			String pathh = String.format("%s%s.h",   Compiler.STUB_DIR_PATH, serviceName);
			logger.info(String.format("pathc = %s", pathc));
			logger.info(String.format("pathh = %s", pathh));
			
			try (
				LinePrinter outc = new LinePrinter(new PrintWriter(pathc));
				LinePrinter outh = new LinePrinter(new PrintWriter(pathh));) {
				outh.format("#ifndef STUB_%s_H__", serviceName);
				outh.format("#define STUB_%s_H__", serviceName);
				outh.line();
				outh.line("#include \"../courier/Protocol.h\"");
				// include corresponding stub header
				outh.format("#include \"../stub/%s.h\"", program.info.getProgramVersion());
				
				// output namespace
				outh.line();
				outh.format("namespace %s {", "Courier");
				outh.format("namespace %s {", "Stub");
				
				// output class
				outh.format("class %s {", serviceName);
				outh.line("public:");
				outh.line("struct CallTable {");
				
				{
					List<String> c1 = new ArrayList<>();
					List<String> c2 = new ArrayList<>();
					List<String> c3 = new ArrayList<>();
					
					for(Compiler.ProcedureInfo procedureInfo: context.procedureInfoList) {
						c1.add(String.format("%s::%s::call", programName, procedureInfo.name));
						c2.add(String.format("call%s", procedureInfo.name));
						c3.add(String.format("= nullptr; // %2d", procedureInfo.code));
					}
					ColumnLayout.layoutStringString(outh, c1, c2, c3);
				}
				outh.line("};");
				
				outh.line();
				{
					List<String> c1 = new ArrayList<>();
					List<String> c2 = new ArrayList<>();
					List<String> c3 = new ArrayList<>();

					for(Compiler.ProcedureInfo procedureInfo: context.procedureInfoList) {
						c1.add(String.format("void setCallTable%s", procedureInfo.name));
						c2.add(String.format("(%s::%s::call", programName, procedureInfo.name));
						c3.add("newValue);");
					}
					ColumnLayout.layoutStringString(outh, c1, c2, c3);
				}
				
				outh.line();
				outh.line(
						"void setCallTable(CallTable callTable_) {",
						"callTable = callTable_;",
						"}");

				outh.line();
				outh.line("void call(Protocol::Protocol3::CallMessage& callMessage, Block& request, Block& response);");
				outh.line();
				
				outh.line("private:");
				outh.line("CallTable callTable;");
				
				outh.line();
				{
					List<String> c1 = new ArrayList<>();
					List<String> c2 = new ArrayList<>();
					
					for(Compiler.ProcedureInfo procedureInfo: context.procedureInfoList) {
						c1.add(String.format("void call%s", procedureInfo.name));
						c2.add("(Protocol::Protocol3::CallMessage& callMessage, Block& request, Block& response);");
		            }
					ColumnLayout.layoutStringString(outh, c1, c2);
				}

				outh.line("};");
				// Close namespace
				outh.line("}");
				outh.line("}");
				// #endif
				outh.line("#endif");
				
				
				outc.line("#include \"../util/Util.h\"");
				outc.format("static log4cpp::Category& logger = Logger::getLogger(\"stub-svc/%s\");", program.info.getProgramVersion());
				outc.line();
				outc.format("#include \"../stub/%s.h\"", program.info.getProgramVersion());
				outc.format("#include \"../stub/%s.h\"", serviceName);
				outc.line();
				outc.line("#include \"../courier/Last.h\"");
				outc.line();
				
				for(Compiler.ProcedureInfo procedureInfo: context.procedureInfoList) {
					outc.format("void %s::%s::setCallTable%s(%s::%s::call newValue) {", "Courier::Stub", serviceName, procedureInfo.name, programName, procedureInfo.name);
					outc.format("callTable.call%s = newValue;", procedureInfo.name);
					outc.line("}");
				}

				outc.format("void %s::%s::call(Protocol::Protocol3::CallMessage& callMessage, Block& request, Block& response) {", "Courier::Stub", serviceName);
				outc.line("switch(callMessage.procedure) {");

				for(Compiler.ProcedureInfo procedureInfo: context.procedureInfoList) {
					outc.format("case %s::%s::CODE:", programName, procedureInfo.name);
					outc.format("call%s(callMessage, request, response);", procedureInfo.name);
					outc.line("break;");
	            }
				outc.line("default:",
						  "logger.error(\"Unexpected callMessage.procedure = %d\", callMessage.procedure);",
						  "COURIER_FATAL_ERROR();");
				outc.line("}"); // switch
				outc.line("}"); // call
				
				for(Compiler.ProcedureInfo procedureInfo: context.procedureInfoList) {
					outc.format("void %s::%s::call%s(Protocol::Protocol3::CallMessage& callMessage, Block& request, Block& response) {", "Courier::Stub", serviceName, procedureInfo.name);

					outc.format("if (callTable.call%s == nullptr) {", procedureInfo.name);
					outc.line("throw Courier::Protocol::REJECT_noSuchProcedureValue;",
							  "}");

					outc.line("try {");
					outc.format("%s::%s::Param  param;", programName, procedureInfo.name);
					outc.format("%s::%s::Result result;", programName, procedureInfo.name);
					
					outc.line("Courier::deserialize(request, param);");
					outc.format("callTable.call%s(param, result);", procedureInfo.name);
					outc.line("Protocol::Protocol3::ReturnMessage returnMessage;",
							  "returnMessage.transaction = callMessage.transaction;",
							  "",
							  "Courier::serialize(response, returnMessage);",
							  "Courier::serialize(response, result);");

					for(String errorName: procedureInfo.typeProcedure.errroList) {
						outc.format("} catch(const %s::%s& e) {", programName, errorName);
						outc.line("Protocol::Protocol3::AbortMessage abortMessage;",
								  "abortMessage.transaction = callMessage.transaction;",
								  "abortMessage.abortCode   = e.CODE;",
								  "",
								  "Courier::serialize(response, abortMessage);",
								  "Courier::serialize(response, e);");
					}
					outc.line("} catch(const Protocol::Abort& e) {",
							  "logger.error(\"Protocol::Abort\");",
							  "COURIER_FATAL_ERROR();");
					outc.line("} catch(const std::runtime_error& e) {",
							  "logger.error(\"std::runtime_error %s\", e.what());",
							  "COURIER_FATAL_ERROR();");
					outc.line("} catch(const std::exception& e) {",
							  "logger.error(\"std::exception %s\", e.what());",
							  "COURIER_FATAL_ERROR();");
					outc.line("} catch(...) {",
							  "COURIER_FATAL_ERROR();");
					
					outc.line("}"); // try
					outc.line("}"); // callXXX
				}

			} catch (FileNotFoundException e) {
				throw new CompilerException("FileNotFoundException", e);
			}
			
		}
	}
}