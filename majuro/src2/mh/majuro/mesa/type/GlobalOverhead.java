package mh.majuro.mesa.type;

import mh.majuro.mesa.Type.*;

public final class GlobalOverhead {
    public static final int SIZE = 2;

    // offset    0  size    1  type CARD16    name available
    public static final class available {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  0;

        public static @LONG_POINTER int offset(@LONG_POINTER int base) {
            return base + OFFSET;
        }
        public static @CARD16 int get(@LONG_POINTER int base) {
            return RecordBase.get(GlobalOverhead.available::offset, base);
        }
        public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
            RecordBase.set(GlobalOverhead.available::offset, base, newValue);
        }
    }
    // offset    1  size    1  type GlobalWord  name word
    public static final class word {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  1;

        public static @LONG_POINTER int offset(@LONG_POINTER int base) {
            return base + OFFSET;
        }
        //   GlobalWord  gfi
        public static final class gfi {
            public static @CARD16 int get(@LONG_POINTER int base) {
                return GlobalWord.gfi.get(offset(base));
            }
            public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
                GlobalWord.gfi.set(offset(base), newValue);
            }
        }
        //   GlobalWord  trapxfers
        public static final class trapxfers {
            public static boolean get(@LONG_POINTER int base) {
                return GlobalWord.trapxfers.get(offset(base));
            }
            public static void set(@LONG_POINTER int base, boolean newValue) {
                GlobalWord.trapxfers.set(offset(base), newValue);
            }
        }
        //   GlobalWord  codelinks
        public static final class codelinks {
            public static boolean get(@LONG_POINTER int base) {
                return GlobalWord.codelinks.get(offset(base));
            }
            public static void set(@LONG_POINTER int base, boolean newValue) {
                GlobalWord.codelinks.set(offset(base), newValue);
            }
        }
    }
}
