package mh.majuro.mesa.opcode;

import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Modifier;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import mh.majuro.UnexpectedException;
import mh.majuro.mesa.CodeCache;
import mh.majuro.mesa.ControlTransfer;
import mh.majuro.mesa.Perf;
import mh.majuro.mesa.Processor;
import mh.majuro.mesa.Type.CARD8;
import mh.majuro.mesa.Util;
import mh.majuro.mesa.opcode.Opcode.Register;
import mh.majuro.mesa.opcode.Opcode.Type;

public class Interpreter {
	private static final Logger logger = LoggerFactory.getLogger(Interpreter.class);

	public static final boolean EnableStats = true;
	
	public static final Runnable[] tableMop = new Runnable[256];
	public static final Runnable[] tableEsc = new Runnable[256];
	
	public static final long[] countMop = new long[256];
	public static final long[] countEsc = new long[256];
	
	public static void execute() {
		Processor.savedPC = CodeCache.getPC();
		Processor.savedSP = Processor.SP;
		dispatchMop(CodeCache.getCodeByte());
	}
	
	public static void dispatchMop(@CARD8 int opcode) {
		if (Perf.ENABLE) Perf.dispatch++;
		tableMop[opcode].run();
		if (EnableStats) countMop[opcode]++;
	}
	public static void dispatchEsc(@CARD8 int opcode) {
		tableEsc[opcode].run();
		if (EnableStats) countEsc[opcode]++;
	}
	
	public static void stats() {
		for(Opcode info: Opcode.values()) {
			long count = (info.type == Type.MOP) ? countMop[info.code] : countEsc[info.code];
			if (count != 0) {
				logger.info("{}", String.format("%s %-8s  %10d", info.type, info.name, countMop[info.code]));
			}
		}
	}
	
	private static class OpcodeTrap implements Runnable {
		private final int code;
		OpcodeTrap(int code) {
			this.code = code;
		}
		public void run() {
			ControlTransfer.opcodeTrap(code);
		}
	}
	private static class EscOpcodeTrap implements Runnable {
		private final int code;
		EscOpcodeTrap(int code) {
			this.code = code;
		}
		public void run() {
			ControlTransfer.escOpcodeTrap(code);
		}
	}
	
	private static void register(Opcode info, Runnable runnable) {
		logger.info("register {} {} {}", info.type, String.format("%03o", info.code), info.name);
		switch(info.type) {
		case MOP:
			tableMop[info.code] = runnable;
			break;
		case ESC:
		case ESCL:
			tableEsc[info.code] = runnable;
			break;
		default:
			throw new UnexpectedException();
		}
	}
	
	private static String PACKAGE_NAME_FOR_SCAN = "mh.majuro.mesa.opcode";
	public static void initialize() {
		int count = 0;
		
		for(int i = 0; i < tableMop.length; i++) tableMop[i] = null;
		for(int i = 0; i < tableEsc.length; i++) tableEsc[i] = null;
		
		logger.info("Start initialize opcode  package = {}", PACKAGE_NAME_FOR_SCAN);
		List<Class<?>> classes = Util.findClass(PACKAGE_NAME_FOR_SCAN);
		for(Class<?> clazz : classes) {
			if (clazz.isAnnotationPresent(Register.class)) {
				Register registerOpcode = clazz.getAnnotation(Register.class);
				try {
					// Object o = clazz.newInstance();
					Object o = clazz.getDeclaredConstructor().newInstance();
					if (o instanceof Runnable) {
						register(registerOpcode.value(), (Runnable)o);
						count++;
					} else {
						logger.error("class {} is not instance of Runnable", clazz.getName());
						throw new UnexpectedException();
					}
				} catch (InstantiationException | IllegalAccessException | IllegalArgumentException | InvocationTargetException | NoSuchMethodException | SecurityException e) {
					logger.error("Exception during clazz.newInstance()", e);
					throw new UnexpectedException();
				}
			} else {
				for(Field field: clazz.getDeclaredFields()) {
					if (field.isAnnotationPresent(Register.class)) {
						if (Modifier.isStatic(field.getModifiers())) {
							Register registerOpcode = field.getAnnotation(Register.class);
							try {
								// Expect static field
								Object o = field.get(null);
								if (o instanceof Runnable) {
									register(registerOpcode.value(), (Runnable)o);
									count++;
								} else {
									logger.error("field {} is not instance of Runnable", field.getName());
									throw new UnexpectedException();
								}
							} catch (IllegalArgumentException | IllegalAccessException e) {
								logger.error("Exception during field.get()", e);
								throw new UnexpectedException();
							}
						} else {
							logger.error("field {} is not static field", field.getName());
							throw new UnexpectedException();
						}
					}
				}
			}
		}
		logger.info(String.format("Stop  initialize opcode  count = %d / %d", count, Opcode.values().length));
		
		// install opcode trap for empty entry of tableMop and tableEsc.
		for(Opcode info: Opcode.values()) {
			int code = info.code;
			switch (info.type) {
			case MOP:
				if (tableMop[code] == null) {
					tableMop[code] = new OpcodeTrap(code);
				}
				break;
			default:
				if (tableEsc[code] == null) {
					tableEsc[code] = new EscOpcodeTrap(code);
				}
				break;
			}
		}
	}

	public static void main(String[] args) {
    	logger.info("START");
    	initialize();
    	logger.info("STOP");
    }
}
