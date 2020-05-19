package mh.majuro.mesa;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public final class Perf {
	private static final Logger logger = LoggerFactory.getLogger(Perf.class);
	
	public static final boolean ENABLE = true;

	public static long fetchPage         = 0;
	public static long storePage         = 0;

	public static long cacheHit          = 0;
	public static long cacheMissConflict = 0;
	public static long cacheMissEmpty    = 0;
	
	public static long fetch             = 0;
	public static long store             = 0;
	public static long readDbl           = 0;
	public static long writeDbl          = 0;
	public static long modify            = 0;
	public static long modifyDbl         = 0;
	
	public static long fetchMDS          = 0;
	public static long storeMDS          = 0;
	public static long readDblMDS        = 0;
	public static long writeDblMDS       = 0;
	public static long modifyMDS         = 0;
	public static long modifyDblMDS      = 0;

	public static long readCode          = 0;
	
	public static long codeCacheHit      = 0;
	public static long codeCacheMiss     = 0;
	public static long codeCacheCodeByte = 0;
	
	public static long writeMap          = 0;
	
	public static long lfCacheHit        = 0;
	public static long lfCacheMiss       = 0;

	public static long fetchByte         = 0;
	public static long fetchWord         = 0;
	public static long storeByte         = 0;
	
	public static long readField         = 0;
	public static long writeField        = 0;

	public static long fetchPDA          = 0;
	public static long storePDA          = 0;
	
	public static long dispatch          = 0;
	
	public static long codeTrap          = 0;
	public static long escOpcodeTrap     = 0;
	public static long opcodeTrap        = 0;
	public static long unboundTrap       = 0;

	public static void initialize() {
		fetchPage         = 0;
		storePage         = 0;

		cacheHit          = 0;
		cacheMissConflict = 0;
		cacheMissEmpty    = 0;
		
		fetch             = 0;
		store             = 0;
		readDbl           = 0;
		writeDbl          = 0;
		modify            = 0;
		modifyDbl         = 0;
		
		fetchMDS          = 0;
		storeMDS          = 0;
		readDblMDS        = 0;
		writeDblMDS       = 0;
		modifyMDS         = 0;
		modifyDblMDS      = 0;

		readCode          = 0;
		
		codeCacheHit      = 0;
		codeCacheMiss     = 0;
		codeCacheCodeByte = 0;
		
		writeMap          = 0;
		
		lfCacheHit        = 0;
		lfCacheMiss       = 0;
		
		fetchByte         = 0;
		fetchWord         = 0;
		storeByte         = 0;
		
		readField         = 0;
		writeField        = 0;
		
		fetchPDA          = 0;
		storePDA          = 0;

		dispatch          = 0;
		
		codeTrap          = 0;
		escOpcodeTrap     = 0;
		opcodeTrap        = 0;
		unboundTrap       = 0;

	}


	public static void stats() {
		logger.info("{}", String.format("fetchPage          %10d", fetchPage));
		logger.info("{}", String.format("storePage          %10d", storePage));

		long cacheMiss  = cacheMissEmpty + cacheMissConflict;
		long cacheTotal = cacheMiss + cacheHit;
		logger.info("{}", String.format("cacheHit           %10d", cacheHit));
		logger.info("{}", String.format("cacheMissEmpty     %10d", cacheMissEmpty));
		logger.info("{}", String.format("cacheMissConflict  %10d", cacheMissConflict));
		logger.info("{}", String.format("cacheMiss          %10d", cacheMiss));
		
		if (0 < cacheTotal) {
			logger.info("{}", String.format("cacheRatio         %10.2f%%", ((double)cacheHit / (double)cacheTotal) * 100));
		}

		logger.info("{}", String.format("fetch              %10d", fetch));
		logger.info("{}", String.format("store              %10d", store));
		logger.info("{}", String.format("readDbl            %10d", readDbl));
		logger.info("{}", String.format("writeDbl           %10d", writeDbl));
		logger.info("{}", String.format("modify             %10d", modify));
		logger.info("{}", String.format("modifyDbl          %10d", modifyDbl));

		logger.info("{}", String.format("fetchMDS           %10d", fetchMDS));
		logger.info("{}", String.format("storeMDS           %10d", storeMDS));
		logger.info("{}", String.format("readDblMDS         %10d", readDblMDS));
		logger.info("{}", String.format("writeDblMDS        %10d", writeDblMDS));
		logger.info("{}", String.format("modifyMDS          %10d", modifyMDS));
		logger.info("{}", String.format("modifyDblMDS       %10d", modifyDblMDS));
		
		logger.info("{}", String.format("readCode           %10d", readCode));
		
		long codeCacheTotal = codeCacheHit + codeCacheMiss;
		logger.info("{}", String.format("codeCacheHit       %10d", codeCacheHit));
		logger.info("{}", String.format("codeCacheMiss      %10d", codeCacheMiss));
		logger.info("{}", String.format("codeCacheCodeByte  %10d", codeCacheCodeByte));
		
		if (0 < codeCacheTotal) {
			logger.info("{}", String.format("coddCacheRatio     %10.2f%%", ((double)(codeCacheHit / codeCacheTotal)) * 100));
		}

		logger.info("{}", String.format("writeMap           %10d", writeMap));

		logger.info("{}", String.format("lfCacheHit         %10d", lfCacheHit));
		logger.info("{}", String.format("lfCacheMiss        %10d", lfCacheMiss));
		
		logger.info("{}", String.format("fetchByte          %10d", fetchByte));
		logger.info("{}", String.format("fetchWord          %10d", fetchWord));
		logger.info("{}", String.format("storeByte          %10d", storeByte));

		logger.info("{}", String.format("readField          %10d", readField));
		logger.info("{}", String.format("writeField         %10d", writeField));
		
		logger.info("{}", String.format("fetchPDA           %10d", fetchPDA));
		logger.info("{}", String.format("storePDA           %10d", storePDA));

		logger.info("{}", String.format("dispatch           %10d", dispatch));
		
		logger.info("{}", String.format("codeTrap           %10d", codeTrap));
		logger.info("{}", String.format("escOpcodeTrap      %10d", escOpcodeTrap));
		logger.info("{}", String.format("opcodeTrap         %10d", opcodeTrap));
		logger.info("{}", String.format("unboundTrap        %10d", unboundTrap));

	}
}
