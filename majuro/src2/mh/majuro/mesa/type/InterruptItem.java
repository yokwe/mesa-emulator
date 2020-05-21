package mh.majuro.mesa.type;

import mh.majuro.mesa.Type.*;

public final class InterruptItem {
    public static final int SIZE = 2;

    // offset    0  size    1  type Condition  name condition
    public static final class condition {
        public static final         int SIZE   =  1;
        public static final         int OFFSET =  0;

        public static @LONG_POINTER int offset(@LONG_POINTER int base) {
            return base + OFFSET;
        }
        public static @CARD16 int get(@LONG_POINTER int base) {
            return RecordBase.get(InterruptItem.condition::offset, base);
        }
        public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
            RecordBase.set(InterruptItem.condition::offset, base, newValue);
        }
    }
    // offset    1  size    1  type           name available
}
