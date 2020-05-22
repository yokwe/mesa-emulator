package mh.majuro.mesa.type;

import mh.majuro.mesa.Memory;
import mh.majuro.mesa.Type.*;

public final class Context {
    public static final int SIZE = 1;

    // offset    0  size    1  type CARD16    name frame
    public static final class frame {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  0;
        public static @CARD16 int get(@LONG_POINTER int base) {
            return Memory.fetch(base + OFFSET);
        }
        public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
            Memory.store(base + OFFSET, newValue);
        }
    }
    // offset    0  size    1  type CARD16    name state
    public static final class state {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  0;
        public static @CARD16 int get(@LONG_POINTER int base) {
            return Memory.fetch(base + OFFSET);
        }
        public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
            Memory.store(base + OFFSET, newValue);
        }
    }
}
