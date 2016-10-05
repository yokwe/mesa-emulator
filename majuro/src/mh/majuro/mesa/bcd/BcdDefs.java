package mh.majuro.mesa.bcd;

import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import mh.majuro.mesa.MesaAssertionError;
import mh.majuro.mesa.Util;

public class BcdDefs {
	protected static final Logger logger = LoggerFactory.getLogger(BcdDefs.class);

	//VersionID: CARDINAL = 6103
	public static final short VersionID = 6103;
	
	//Stamp: TYPE = RECORD [net, host: [0..377B], time: LONG CARDINAL];
	public static class VersionStamp {
		public static final int EPOCH_DIFF = 2114294400 + (731 * 60 * 60 * 24);
		public static final DateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss");

		public final byte net;
		public final byte host;
		public final int time;
		
		public VersionStamp(byte net, byte host, int time) {
			this.net = net;
			this.host = host;
			this.time = time;
		}
		public boolean isNull() {
			return net == 0 && host == 0 && time == 0;
		}
		public String toString() {
			if (isNull()) return "Null";
			int unixTime = time - EPOCH_DIFF;
			Date date = new Date((unixTime & 0xffffffffL) * 1000L);
			return String.format("[net %02X  host %02X  time %s]", net, host, dateFormat.format(date));
		}
	}
	
	//BCD: TYPE = RECORD [
	//  versionIdent: CARDINAL,
	//  version: VersionStamp,
	//  creator: VersionStamp,
	//  sourceFile: FTIndex,
	//  unpackagedFile: FTIndex,
	//  nConfigs, nModules: CARDINAL,
	//  nImports, nExports: CARDINAL,
	//  nPages: [0..256),
	//  definitions, repackaged, typeExported, tableCompiled: BOOLEAN,
	//  spare: PACKED ARRAY [0..4) OF BOOLEAN,
	//  firstdummy: GFIndex,
	//  nDummies: CARDINAL,
	//  ssOffset: CARDINAL, -- string table
	//  ssLimit: CARDINAL,
	//  ctOffset: CARDINAL, -- config table
	//  ctLimit: CTIndex,
	//  mtOffset: CARDINAL, -- module table
	//  mtLimit: MTIndex,
	//  impOffset: CARDINAL, -- import table
	//  impLimit: IMPIndex,
	//  expOffset: CARDINAL, -- export table
	//  expLimit: EXPIndex,
	//  enOffset: CARDINAL, -- entry table
	//  enLimit: ENIndex,
	//  sgOffset: CARDINAL, -- segment table
	//  sgLimit: SGIndex,
	//  ftOffset: CARDINAL, -- file table
	//  ftLimit: FTIndex,
	//  spOffset: CARDINAL, -- space table
	//  spLimit: SPIndex,
	//  ntOffset: CARDINAL, -- name table
	//  ntLimit: NTIndex,
	//  typOffset: CARDINAL, -- type table
	//  typLimit: TYPIndex,
	//  tmOffset: CARDINAL, -- type map table
	//  tmLimit: TMIndex,
	//  fpOffset: CARDINAL, -- frame pack table
	//  fpLimit: FPIndex,
	//  lfOffset: CARDINAL, -- link fragment table
	//  lfLimit: LFIndex,
	//  atOffset: CARDINAL, -- atom table
	//  atLimit: ATIndex,
	//  apOffset: CARDINAL, -- atom printname table
	//  apLimit: CARDINAL];
	
	public static final class BCD {
		public short        versionIdent;
		public VersionStamp version;
		public VersionStamp creator;
		
		public BCD(String path) {
			File file = new File(path);
			if (!file.exists())  throw new MesaAssertionError();
			if (!file.canRead()) throw new MesaAssertionError();
			
			try (DataInputStream dis = new DataInputStream(new FileInputStream(file))) {
				versionIdent = dis.readShort();
				logger.info("versionIdent = {}", versionIdent);
				{
					byte  net      = dis.readByte();
					byte  host     = dis.readByte();
					short timeLow  = dis.readShort();
					short timeHigh = dis.readShort();
					version = new VersionStamp(net, host, Util.asInt(timeHigh, timeLow));
					logger.info("version = {}", version);
				}
				{
					byte net       = dis.readByte();
					byte host      = dis.readByte();
					short timeLow  = dis.readShort();
					short timeHigh = dis.readShort();
					creator = new VersionStamp(net, host, Util.asInt(timeHigh, timeLow));
					logger.info("creator = {}", creator);
				}
			} catch (IOException e) {
				logger.error("IOException", e);
				throw new MesaAssertionError();
			}
			
		}
	}
	
	public static void main(String[] args) {
		new BCD("../Source/APilot/15.0.1/Mesa/Friends/BcdDefsExtras.bcd");
		// For ../Source/APilot/15.0.1/Mesa/Friends/BcdDefsExtras.bcd
		// version = [net 00  host AA  time 1986-06-14T08:08:42]
		// creator = [net 00  host 00  time 1986-01-15T04:13:35]
	}

}
