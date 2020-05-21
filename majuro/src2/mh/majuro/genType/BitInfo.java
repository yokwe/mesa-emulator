package mh.majuro.genType;

import org.slf4j.LoggerFactory;

import mh.majuro.UnexpectedException;

public class BitInfo {
	static final org.slf4j.Logger logger = LoggerFactory.getLogger(BitInfo.class);

	final int startBit;
	final int stopBit;
	
	final int    shift;
	final String mask;
	
	public BitInfo(int size, int startBit, int stopBit) {
		this.startBit = startBit;
		this.stopBit  = stopBit;
		
		int width = stopBit - startBit;
		int bits  = (1 << (width + 1)) - 1;
		switch(size) {
		case 1:
			this.shift = 15 - stopBit;
			this.mask  = String.format("0x%04X", (bits << shift) & 0xFFFF);
			break;
		case 2:
			this.shift = 31 - stopBit;
			this.mask  = String.format("0x%08X", (bits << shift));
			break;
		default:
			logger.error("size {}", size);
			throw new UnexpectedException();
		}
	}
}