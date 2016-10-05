package mh.majuro.mesa;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;
import java.lang.reflect.Field;
import java.util.List;

import mh.majuro.mesa.OpcodeInfo.OpcodeType;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


public final class Opcode {
	protected static final Logger logger = LoggerFactory.getLogger(Opcode.class);
	
	@Retention(RetentionPolicy.RUNTIME)
	@Target({ElementType.TYPE, ElementType.FIELD})
	public @interface RegisterOpcode {
		OpcodeInfo value();
	}
	
	static class Info {
		final OpcodeType     type;
		final int      code;
		final String   name;
		final Runnable runnable;
		//
		long     count;
		
		Info(OpcodeInfo info, Runnable runnable) {
			this.type     = info.type;
			this.code     = info.code;
			this.name     = info.name;
			this.runnable = runnable;
			//
			this.count    = 0;
		}
		
		public String toString() {
			return String.format("%-6s %03o %s", type, code, name);
		}
	}

	private static final int zESC  = 0370;
	private static final int zESCL = 0371;
	
	private static final int TABLE_SIZE = 256;
	private static Info infoTable[]    = new Info[TABLE_SIZE];
	private static Info infoTableEsc[] = new Info[TABLE_SIZE];
	
	static {
		for(int i = 0; i < TABLE_SIZE; i++) {
			infoTable   [i] = null;
			infoTableEsc[i] = null;
		}
	}
	
	public static void dispatch(int code) {
		Info info;
		switch(code) {
		case zESC:
		case zESCL:
			code = Memory.GetCodeByte();
			info = infoTableEsc[code];
			if (info == null) Processor.EscOpcodeTrap(code);
			break;
		default:
			info = infoTable[code];
			if (info == null) Processor.OpcodeTrap(code);
			break;
		}
		info.count++;
		info.runnable.run();
	}
	public static void execute() {
		Processor.savedPC = Processor.PC;
		Processor.savedSP = Processor.SP;
		dispatch(Memory.GetCodeByte());
	}
	
	private static void register(Info info) {
		if (Debug.TRACE_REGISTER_OPCODE) logger.info("Register {}", info.toString());
		switch(info.type) {
		case NORMAL:
			if (infoTable[info.code] != null) {
				logger.error(String.format("opcode is already registered.  old = %s  new = %s", infoTable[info.code].toString(), info.toString()));
				throw new MesaAssertionError();
			}
			infoTable[info.code] = info;
			break;
		case ESC:
		case ESCL:
			if (infoTableEsc[info.code] != null) {
				logger.error(String.format("opcode is already registered.  old = %s  new = %s", infoTableEsc[info.code].toString(), info.toString()));
				throw new MesaAssertionError();
			}
			infoTableEsc[info.code] = info;
			break;
		default:
			throw new MesaAssertionError();
		}
	}

	public static void stats() {
		logger.debug("Opcode stats START");
		logger.debug("====");
		for(int i = 0; i < TABLE_SIZE; i++) {
			if (infoTable[i] != null) {
				Info info = infoTable[i];
				if (info == null) continue;
				logger.info(String.format("%-20s %10d", info.toString(), info.count));
			}
		}
		logger.debug("====");
		for(int i = 0; i < TABLE_SIZE; i++) {
			if (infoTableEsc[i] != null) {
				Info info = infoTableEsc[i];
				if (info == null) continue;
				logger.info(String.format("%-20s %10d", info.toString(), info.count));
			}
		}
		logger.debug("====");
		logger.debug("Opcode stats STOP");
	}
	
	private static boolean initialized = false;
	private static String PACKAGE_NAME_FOR_SCAN = "mh.majuro.mesa";
	public static void initialize() {
		if (initialized) return;
		initialized = true;
		
		int count = 0;
		
		logger.info("Start initialize opcode  package = {}", PACKAGE_NAME_FOR_SCAN);
		List<Class<?>> classes = Util.findClass(PACKAGE_NAME_FOR_SCAN);
		for(Class<?> clazz : classes) {
			if (clazz.isAnnotationPresent(RegisterOpcode.class)) {
				RegisterOpcode registerOpcode = clazz.getAnnotation(RegisterOpcode.class);
				try {
					Object o = clazz.newInstance();
					if (o instanceof Runnable) {
						Info info = new Info(registerOpcode.value(), (Runnable)o);
						register(info);
						count++;
					} else {
						logger.error("class {} is not instance of Runnable", clazz.getName());
						throw new MesaAssertionError();
					}
				} catch (InstantiationException | IllegalAccessException e) {
					logger.error("Exception during clazz.newInstance()", e);
					throw new MesaAssertionError();
				}
			} else {
				for(Field field: clazz.getDeclaredFields()) {
					if (field.isAnnotationPresent(RegisterOpcode.class)) {
						RegisterOpcode registerOpcode = field.getAnnotation(RegisterOpcode.class);
						try {
							Object o = field.get(null);
							if (o instanceof Runnable) {
								Info info = new Info(registerOpcode.value(), (Runnable)o);
								register(info);
								count++;
							} else {
								logger.error("field {} is not instance of Runnable", field.getName());
								throw new MesaAssertionError();
							}
						} catch (IllegalArgumentException | IllegalAccessException e) {
							logger.error("Exception during field.get()", e);
							throw new MesaAssertionError();
						}
					}
				}
			}
		}
		logger.info(String.format("Stop  initialize opcode  count = %d / %d", count, OpcodeInfo.values().length));
	}
	
	public static void main(String[] args) {
		logger.info("START");
		
		Opcode.initialize();
		
		Opcode.dispatch(0);
		//Opcode.dispatch(0377);
		
		Opcode.stats();
		
		logger.info("STOP");
	}

	
}

