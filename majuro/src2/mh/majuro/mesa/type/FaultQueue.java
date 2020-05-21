package mh.majuro.mesa.type;

import mh.majuro.mesa.Type.*;

public final class FaultQueue {
    public static final int SIZE = 2;

    // offset    0  size    1  type Queue     name queue
    public static final class queue {
        public static final         int SIZE   =  1;
        public static final         int OFFSET =  0;

        public static @LONG_POINTER int offset(@LONG_POINTER int base) {
            return base + OFFSET;
        }
        public static @CARD16 int get(@LONG_POINTER int base) {
            return RecordBase.get(FaultQueue.queue::offset, base);
        }
        public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
            RecordBase.set(FaultQueue.queue::offset, base, newValue);
        }
    }
    // offset    1  size    1  type Condition  name condition
    public static final class condition {
        public static final         int SIZE   =  1;
        public static final         int OFFSET =  1;

        public static @LONG_POINTER int offset(@LONG_POINTER int base) {
            return base + OFFSET;
        }
        public static @CARD16 int get(@LONG_POINTER int base) {
            return RecordBase.get(FaultQueue.condition::offset, base);
        }
        public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
            RecordBase.set(FaultQueue.condition::offset, base, newValue);
        }
    }
}
