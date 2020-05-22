package mh.majuro.mesa.type;

import mh.majuro.mesa.Type.*;

public final class FaultQueue {
    public static final int SIZE = 2;

    // offset    0  size    1  type Queue     name queue
    public static final class queue {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  0;

        public static @LONG_POINTER int offset(@LONG_POINTER int base) {
            return base + OFFSET;
        }
        //   Queue  tail
        public static final class tail {
            public static @CARD16 int get(@LONG_POINTER int base) {
                return Queue.tail.get(offset(base));
            }
            public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
                Queue.tail.set(offset(base), newValue);
            }
        }
    }
    // offset    1  size    1  type Condition  name condition
    public static final class condition {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  1;

        public static @LONG_POINTER int offset(@LONG_POINTER int base) {
            return base + OFFSET;
        }
        //   Condition  tail
        public static final class tail {
            public static @CARD16 int get(@LONG_POINTER int base) {
                return Condition.tail.get(offset(base));
            }
            public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
                Condition.tail.set(offset(base), newValue);
            }
        }
        //   Condition  abortable
        public static final class abortable {
            public static boolean get(@LONG_POINTER int base) {
                return Condition.abortable.get(offset(base));
            }
            public static void set(@LONG_POINTER int base, boolean newValue) {
                Condition.abortable.set(offset(base), newValue);
            }
        }
        //   Condition  wakeup
        public static final class wakeup {
            public static boolean get(@LONG_POINTER int base) {
                return Condition.wakeup.get(offset(base));
            }
            public static void set(@LONG_POINTER int base, boolean newValue) {
                Condition.wakeup.set(offset(base), newValue);
            }
        }
    }
}
