package mh.majuro.mesa;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class Perf {
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
	
	public static long fetchMDS          = 0;
	public static long storeMDS          = 0;
	public static long readDblMDS        = 0;

	public static long readCode          = 0;
	
	public static long codeCacheHit      = 0;
	public static long codeCacheMiss     = 0;
	
	public static void initialize() {
		fetchPage         = 0;
		storePage         = 0;

		cacheHit          = 0;
		cacheMissConflict = 0;
		cacheMissEmpty    = 0;
		
		fetch             = 0;
		store             = 0;
		readDbl           = 0;
		
		fetchMDS          = 0;
		storeMDS          = 0;
		readDblMDS        = 0;

		readCode          = 0;
		
		codeCacheHit      = 0;
		codeCacheMiss     = 0;
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
		logger.info("{}", String.format("cacheRatio         %10.2f%%", (double)cacheHit / (double)cacheTotal));

		logger.info("{}", String.format("fetch              %10d", fetch));
		logger.info("{}", String.format("store              %10d", store));
		logger.info("{}", String.format("readDbl            %10d", readDbl));

		logger.info("{}", String.format("fetchMDS           %10d", fetchMDS));
		logger.info("{}", String.format("storeMDS           %10d", storeMDS));
		logger.info("{}", String.format("readDblMDS         %10d", readDblMDS));
		
		logger.info("{}", String.format("readCode           %10d", readCode));
		
		long codeCacheTotal = codeCacheHit + codeCacheMiss;
		logger.info("{}", String.format("codeCacheHit       %10d", codeCacheHit));
		logger.info("{}", String.format("codeCacheMiss      %10d", codeCacheMiss));
		logger.info("{}", String.format("coddCacheRatio     %10.2f%%", (double)(codeCacheHit / codeCacheTotal)));
	}
}
