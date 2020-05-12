package mh.majuro.mesa;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class Perf {
	private static final Logger logger = LoggerFactory.getLogger(Perf.class);
	
	public static final boolean PERF_ENABLE = true;

	public static long cacheHit = 0;
	public static long cacheMissConflict = 0;
	public static long cacheMissEmpty = 0;

	public static void stats() {
		long cacheMiss = Perf.cacheMissEmpty + Perf.cacheMissConflict;
		long cacheTotal = cacheMiss + Perf.cacheHit;
		
		logger.info(String.format("cacheHit           %10u", cacheHit));
		logger.info(String.format("cacheMissEmpty     %10u", cacheMissEmpty));
		logger.info(String.format("cacheMissConflict  %10u", cacheMissConflict));
		logger.info(String.format("cacheMiss          %10u", cacheMiss));
		logger.info(String.format("cacheRatio         %6.2f%%", (double)cacheHit / (double)cacheTotal));

	}
}
