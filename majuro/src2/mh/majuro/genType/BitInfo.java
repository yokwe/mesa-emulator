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
		long bits  = (1 << (width + 1)) - 1;
		switch(size) {
		case 1:
			this.shift = 15 - stopBit;
			this.mask  = String.format("%16s", Long.toBinaryString((bits << shift))).replace(" ", "0").replaceAll("(.{4})(.{4})(.{4})(.{4})", "0b$1_$2_$3_$4");
			break;
		case 2:
			this.shift = 31 - stopBit;
			this.mask  = String.format("%32s", Long.toBinaryString(bits << shift)).replace(" ", "0").replaceAll("(.{4})(.{4})(.{4})(.{4})(.{4})(.{4})(.{4})(.{4})", "0b$1_$2_$3_$4_$5_$6_$7_$8");
			break;
		default:
			logger.error("size {}", size);
			throw new UnexpectedException();
		}
	}
	
	@Override
	public String toString() {
		return String.format("{%d %d}", startBit, stopBit);
	}
}