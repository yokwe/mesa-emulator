package mh.majuro.mesa.type;

import mh.majuro.mesa.Memory;
import mh.majuro.mesa.Type.*;

public final class GlobalOverhead {
    public static final int SIZE = 2;

    // offset    0  size    1  type CARD16    name available
    public static final class available {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  0;
        public static @CARD16 int get(@LONG_POINTER int base) {
            return Memory.fetch(base + OFFSET);
        }
        public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
            Memory.store(base + OFFSET, newValue);
        }
    }
    // offset    1  size    1  type GlobalWord  name word
    public static final class word {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  1;
        //   GlobalWord  gfi
        public static final class gfi {
            public static final int OFFSET = word.OFFSET +  0;

            public static @CARD16 int get(@LONG_POINTER int base) {
                return GlobalWord.gfi.get(base + OFFSET);
            }
            public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
                GlobalWord.gfi.set(base + OFFSET, newValue);
            }
        }
        //   GlobalWord  trapxfers
        public static final class trapxfers {
            public static final int OFFSET = word.OFFSET +  0;

            public static boolean get(@LONG_POINTER int base) {
                return GlobalWord.trapxfers.get(base + OFFSET);
            }
            public static void set(@LONG_POINTER int base, boolean newValue) {
                GlobalWord.trapxfers.set(base + OFFSET, newValue);
            }
        }
        //   GlobalWord  codelinks
        public static final class codelinks {
            public static final int OFFSET = word.OFFSET +  0;

            public static boolean get(@LONG_POINTER int base) {
                return GlobalWord.codelinks.get(base + OFFSET);
            }
            public static void set(@LONG_POINTER int base, boolean newValue) {
                GlobalWord.codelinks.set(base + OFFSET, newValue);
            }
        }
    }
}
