package mh.majuro.mesa.opcode;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import mh.majuro.UnexpectedException;
import mh.majuro.mesa.Processor;
import mh.majuro.mesa.opcode.Opcode.Register;

public class Opcode0xx {
	private static final Logger logger = LoggerFactory.getLogger(Opcode0xx.class);
	
	// 00 NOOP
	@Register(Opcode.NOOP)
	public static final void OP_NOOP() {
		logger.error(String.format("TRACE %6o  NOOP", Processor.savedPC));
		throw new UnexpectedException();
	};
}
