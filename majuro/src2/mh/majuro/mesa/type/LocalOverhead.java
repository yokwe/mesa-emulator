package mh.majuro.mesa.type;

import mh.majuro.mesa.Memory;
import mh.majuro.mesa.Type.*;

public final class LocalOverhead {
    public static final int SIZE = 4;

    // offset    0  size    1  type CARD16    name word
    public static final class word {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  0;
        public static @CARD16 int get(@LONG_POINTER int base) {
            return Memory.fetch(base + OFFSET);
        }
        public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
            Memory.store(base + OFFSET, newValue);
        }
    }
    // offset    1  size    1  type CARD16    name returnlink
    public static final class returnlink {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  1;
        public static @CARD16 int get(@LONG_POINTER int base) {
            return Memory.fetch(base + OFFSET);
        }
        public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
            Memory.store(base + OFFSET, newValue);
        }
    }
    // offset    2  size    1  type CARD16    name globallink
    public static final class globallink {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  2;
        public static @CARD16 int get(@LONG_POINTER int base) {
            return Memory.fetch(base + OFFSET);
        }
        public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
            Memory.store(base + OFFSET, newValue);
        }
    }
    // offset    3  size    1  type CARD16    name pc
    public static final class pc {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  3;
        public static @CARD16 int get(@LONG_POINTER int base) {
            return Memory.fetch(base + OFFSET);
        }
        public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
            Memory.store(base + OFFSET, newValue);
        }
    }
}
