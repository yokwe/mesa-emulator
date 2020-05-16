package mh.majuro.mesa.opcode;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import mh.majuro.UnexpectedException;
import mh.majuro.mesa.Processor;

public class Opcode0xx {
	private static final Logger logger = LoggerFactory.getLogger(Opcode0xx.class);
	
	// 00 NOOP
	@Register(Info.NOOP)
	public static final class OP_NOOP implements Runnable {
		@Override
		public void run() {
			logger.error(String.format("TRACE %6o  NOOP", Processor.savedPC));
			throw new UnexpectedException();
		}
	}

}
