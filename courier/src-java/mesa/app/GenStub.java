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
import mesa.courier.compiler.Compiler.Context;
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
		for(Map.Entry<String, Context> entry: Compiler.contextMap.entrySet()) {
			Program myProgram = entry.getValue().program;
			
			String programName = myProgram.info.name;
			int    program     = myProgram.info.program;
			int    version     = myProgram.info.version;
					
			Context context        = entry.getValue();
			
			for(ProcedureInfo procedureInfo: context.procedureInfoList) {
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
			
			for(Map.Entry<String, Context> entry: Compiler.contextMap.entrySet()) {
				Program program = entry.getValue().program;
				Context context = entry.getValue();
				
				for(ProcedureInfo procedureInfo: context.procedureInfoList) {
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
		for(Map.Entry<String, Context> entry: Compiler.contextMap.entrySet()) {
			Program program = entry.getValue().program;
			Context context = entry.getValue();
			
			if (context.procedureInfoList.isEmpty()) continue;
			
			String programName = program.info.getProgramVersion();
			String serviceName = String.format("Service_%s", programName);
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
				outh.line("#include \"../courier/Service.h\"");
				// include corresponding stub header
				outh.format("#include \"../stub/%s.h\"", program.info.getProgramVersion());
				
				// output namespace
				outh.line();
				outh.format("namespace %s {", "Courier");
				outh.format("namespace %s {", "Stub");
				
				// output class
				outh.format("class %s : public Service::ServiceBase {", serviceName);
				outh.line("public:");
				
				outh.format("%s() : Service::ServiceBase(\"%s\", %d, %d) {}", serviceName, program.info.name, program.info.program, program.info.version);
				outh.format("virtual ~%s() {}", serviceName, programName);


				outh.line();
				{
					List<String> c1 = new ArrayList<>();
					List<String> c2 = new ArrayList<>();
					
					c1.add("using CallMessage =");
					c2.add("Protocol::Protocol3::Protocol3_CALL;");
					for(ProcedureInfo procedureInfo: context.procedureInfoList) {
						c1.add(String.format("using %s =", procedureInfo.name));
						c2.add(String.format("%s::%s;", programName, procedureInfo.name));
					}
					ColumnLayout.layoutStringString(outh, c1, c2);
				}
				
				outh.line();
				outh.line("virtual void init()    = 0;",
						  "virtual void destroy() = 0;",
						  "");
				{
					List<String> c1 = new ArrayList<>();
					List<String> c2 = new ArrayList<>();
					List<String> c3 = new ArrayList<>();
					List<String> c4 = new ArrayList<>();
					
					for(ProcedureInfo procedureInfo: context.procedureInfoList) {
						String procedureName = procedureInfo.name.substring(0, 1).toLowerCase() + procedureInfo.name.substring(1, procedureInfo.name.length());
						
						c1.add(String.format("virtual void %s", procedureName));
						c2.add(String.format("(quint16 transaction, const %s::Param& ", procedureInfo.name));
						c3.add(String.format("param, %s::Result&", procedureInfo.name));
						c4.add("result) = 0;");
					}
					ColumnLayout.layoutStringString(outh, c1, c2, c3, c4);
				}
				
				outh.line("",
						  "bool        isProcedureValid    (int code);",
						  "const char* getProcedureName    (int code);",
						  "void        service             (const CallMessage& callMessage, Block& request, Block& response);");
				
				outh.line("",
						  "private:");
				
				{
					List<String> c1 = new ArrayList<>();
					List<String> c2 = new ArrayList<>();
					
					for(ProcedureInfo procedureInfo: context.procedureInfoList) {
						c1.add(String.format("void call%s", procedureInfo.name));
						c2.add("(const CallMessage& callMessage, Block& request, Block& response);");
		            }
					ColumnLayout.layoutStringString(outh, c1, c2);
				}

				outh.line("};"); // end of class
				
				// Close namespace
				outh.line("}");
				outh.line("}");
				// #endif
				outh.line("#endif");
				
				
				//
				// Generation of outc starts
				//
				outc.line("#include \"../util/Util.h\"");
				outc.format("static log4cpp::Category& logger = Logger::getLogger(\"stub-svc/%s\");", program.info.getProgramVersion());
				outc.line();
				outc.format("#include \"../stub/%s.h\"", program.info.getProgramVersion());
				outc.format("#include \"../stub/%s.h\"", serviceName);
				outc.line();
				outc.line("#include \"../courier/Last.h\"");
				outc.line();
				
				outc.format("bool        %s::%s::isProcedureValid(int code) {", "Courier::Stub", serviceName);
				outc.line("switch(code) {");
				for(ProcedureInfo procedureInfo: context.procedureInfoList) {
					outc.format("case %s::CODE:", procedureInfo.name);
					outc.line("return true;");
	            }
				outc.line("default:",
						  "return false;",
						  "}",
						  "}");

				outc.format("const char* %s::%s::getProcedureName(int code) {", "Courier::Stub", serviceName);
				outc.line("switch(code) {");
				for(ProcedureInfo procedureInfo: context.procedureInfoList) {
					outc.format("case %s::CODE:", procedureInfo.name);
					outc.format("return %s::NAME;", procedureInfo.name);
	            }
				outc.line("default:",
						  "logger.error(\"Unexpected code = %d\", code);",
						  "COURIER_FATAL_ERROR();",
						  "}",
						  "}");

				outc.format("void %s::%s::service(const CallMessage& callMessage, Block& request, Block& response) {", "Courier::Stub", serviceName);
				outc.line("switch(callMessage.procedure) {");
				for(ProcedureInfo procedureInfo: context.procedureInfoList) {
					outc.format("case %s::CODE:", procedureInfo.name);
					outc.format("call%s(callMessage, request, response);", procedureInfo.name);
					outc.line("break;");
	            }
				outc.line("default:",
						  "logger.error(\"Unexpected callMessage.procedure = %d\", callMessage.procedure);",
						  "COURIER_FATAL_ERROR();");
				outc.line("}"); // switch
				outc.line("}"); // call
				
				for(ProcedureInfo procedureInfo: context.procedureInfoList) {
					String procedureName = procedureInfo.name.substring(0, 1).toLowerCase() + procedureInfo.name.substring(1, procedureInfo.name.length());

					outc.format("void %s::%s::call%s(const CallMessage& callMessage, Block& request, Block& response) {", "Courier::Stub", serviceName, procedureInfo.name);
					
					outc.line("try {");
					outc.format("%s::Param  param;", procedureInfo.name);
					outc.format("%s::Result result;", procedureInfo.name);
					outc.line("Courier::deserialize(request, param);");
					outc.format("%s(callMessage.transaction, param, result);", procedureName);
					outc.line("",
							  "Protocol::Protocol3 protocol3 = Courier::Service::getProtocolReturn(callMessage);",
							  "Courier::serialize(response, protocol3);",
							  "Courier::serialize(response, result);");

					for(String errorName: procedureInfo.typeProcedure.errroList) {
						outc.format("} catch(const %s::%s& e) {", programName, errorName);
						outc.line("Protocol::Protocol3 protocol3 = Courier::Service::getProtocolAbort(callMessage, e.abortCode);");
						outc.line("Courier::serialize(response, protocol3);",
								  "Courier::serialize(response, e);");
					}
					outc.line("} catch(const Courier::Service::Abort& e) {",
							  "logger.error(\"Uncaught Protocol::Abort  %s%d  %s\", e.programName, e.versionCode, e.abortName);",
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