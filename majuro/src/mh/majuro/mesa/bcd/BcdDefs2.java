package mh.majuro.mesa.bcd;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.MappedByteBuffer;
import java.nio.ShortBuffer;
import java.nio.channels.FileChannel.MapMode;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class BcdDefs2 {
	protected static final Logger logger = LoggerFactory.getLogger(BcdDefs2.class);
	
	public static void main(String[] args) throws IOException {
		logger.info("START");
		
//		File file = new File("/home/hasegawa/workspace/Wildflower/Source/APilot/15.0.1/Mesa/Friends/BcdDefsExtras.bcd");
		File file = new File("/home/hasegawa/workspace/Wildflower/Source/APilot/15.0.1/Mesa/Friends/BcdDefsExtras.bcd");
		long length = file.length();
		
		RandomAccessFile raf = new RandomAccessFile(file, "r");
		
		MappedByteBuffer mbb = raf.getChannel().map(MapMode.READ_ONLY, 0, length);
		logger.info("mbb hasArray = {}", mbb.hasArray());
		logger.info("mbb isDirect = {}", mbb.isDirect());
		logger.info("mbb order    = {}", mbb.order());
		ShortBuffer sb = mbb.asShortBuffer();
		logger.info("sb hasArray = {}", sb.hasArray());
		logger.info("sb isDirect = {}", sb.isDirect());
		logger.info("sb order    = {}", sb.order());
		logger.info("isLoaded = {}", mbb.isLoaded());
		{
			short versionIdent = mbb.getShort();
			logger.info("versionIdent = {}", versionIdent);
		}
		{
			short versionIdent = sb.get();
			logger.info("versionIdent = {}", versionIdent);
		}
		
		logger.info("STOP");
	}

}
