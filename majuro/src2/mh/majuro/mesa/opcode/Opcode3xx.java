package mh.majuro.mesa.opcode;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import mh.majuro.mesa.CodeCache;
import mh.majuro.mesa.opcode.Opcode.Register;

public class Opcode3xx {
	private static final Logger logger = LoggerFactory.getLogger(Opcode3xx.class);
	
	// 0370 ESC
	@Register(Opcode.ESC)
	public static final void OP_ESC () {
		Interpreter.dispatchEsc(CodeCache.getCodeByte());
	};
	
	// 0371 ESCL
	@Register(Opcode.ESCL)
	public static final void OP_ESCL () {
		Interpreter.dispatchEsc(CodeCache.getCodeByte());
	};
}
