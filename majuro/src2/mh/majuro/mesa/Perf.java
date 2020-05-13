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


	public static void stats() {		
		logger.info("{}", String.format("fetchPage          %10u", fetchPage));
		logger.info("{}", String.format("storePage          %10u", storePage));

		long cacheMiss  = cacheMissEmpty + cacheMissConflict;
		long cacheTotal = cacheMiss + cacheHit;
		logger.info("{}", String.format("cacheHit           %10u", cacheHit));
		logger.info("{}", String.format("cacheMissEmpty     %10u", cacheMissEmpty));
		logger.info("{}", String.format("cacheMissConflict  %10u", cacheMissConflict));
		logger.info("{}", String.format("cacheMiss          %10u", cacheMiss));
		logger.info("{}", String.format("cacheRatio         %6.2f%%", (double)cacheHit / (double)cacheTotal));

		logger.info("{}", String.format("fetch              %10u", fetch));
		logger.info("{}", String.format("store              %10u", store));
		logger.info("{}", String.format("readDbl            %10u", readDbl));

		logger.info("{}", String.format("fetchMDS           %10u", fetchMDS));
		logger.info("{}", String.format("storeMDS           %10u", storeMDS));
		logger.info("{}", String.format("readDblMDS         %10u", readDblMDS));
		
		logger.info("{}", String.format("readCode           %10u", readCode));
		
		long codeCacheTotal = codeCacheHit + codeCacheMiss;
		logger.info("{}", String.format("codeCacheHit       %10u", codeCacheHit));
		logger.info("{}", String.format("codeCacheMiss      %10u", codeCacheMiss));
		logger.info("{}", String.format("coddCacheRatio     %6.2f%%", (double)(codeCacheHit / codeCacheTotal)));
	}
}
