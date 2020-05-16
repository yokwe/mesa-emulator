package mh.majuro.mesa.opcode;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import mh.majuro.mesa.CodeCache;

public class Opcode3xx {
	private static final Logger logger = LoggerFactory.getLogger(Opcode3xx.class);
	
	// 0370 ESC
	@Register(Info.ESC)
	public static final class OP_ESC implements Runnable {
		@Override
		public void run() {
			Interpreter.dispatchEsc(CodeCache.getCodeByte());
		}
	}
	
	// 0371 ESCL
	@Register(Info.ESCL)
	public static final class OP_ESCL implements Runnable {
		@Override
		public void run() {
			Interpreter.dispatchEsc(CodeCache.getCodeByte());
		}
	}
}
