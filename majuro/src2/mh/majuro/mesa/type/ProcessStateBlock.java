package mh.majuro.mesa.type;

import mh.majuro.mesa.Type.*;

public final class ProcessStateBlock {
    public static final int SIZE = 8;

    // offset    0  size    1  type PsbLink   name link
    public static final class link {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  0;

        public static @LONG_POINTER int offset(@LONG_POINTER int base) {
            return base + OFFSET;
        }
        //   PsbLink  priority
        public static final class priority {
            public static @CARD16 int get(@LONG_POINTER int base) {
                return PsbLink.priority.get(offset(base));
            }
            public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
                PsbLink.priority.set(offset(base), newValue);
            }
        }
        //   PsbLink  next
        public static final class next {
            public static @CARD16 int get(@LONG_POINTER int base) {
                return PsbLink.next.get(offset(base));
            }
            public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
                PsbLink.next.set(offset(base), newValue);
            }
        }
        //   PsbLink  failed
        public static final class failed {
            public static boolean get(@LONG_POINTER int base) {
                return PsbLink.failed.get(offset(base));
            }
            public static void set(@LONG_POINTER int base, boolean newValue) {
                PsbLink.failed.set(offset(base), newValue);
            }
        }
        //   PsbLink  permanent
        public static final class permanent {
            public static boolean get(@LONG_POINTER int base) {
                return PsbLink.permanent.get(offset(base));
            }
            public static void set(@LONG_POINTER int base, boolean newValue) {
                PsbLink.permanent.set(offset(base), newValue);
            }
        }
        //   PsbLink  preempted
        public static final class preempted {
            public static boolean get(@LONG_POINTER int base) {
                return PsbLink.preempted.get(offset(base));
            }
            public static void set(@LONG_POINTER int base, boolean newValue) {
                PsbLink.preempted.set(offset(base), newValue);
            }
        }
    }
    // offset    1  size    1  type PsbFlags  name flags
    public static final class flags {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  1;

        public static @LONG_POINTER int offset(@LONG_POINTER int base) {
            return base + OFFSET;
        }
        //   PsbFlags  cleanup
        public static final class cleanup {
            public static @CARD16 int get(@LONG_POINTER int base) {
                return PsbFlags.cleanup.get(offset(base));
            }
            public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
                PsbFlags.cleanup.set(offset(base), newValue);
            }
        }
        //   PsbFlags  waiting
        public static final class waiting {
            public static boolean get(@LONG_POINTER int base) {
                return PsbFlags.waiting.get(offset(base));
            }
            public static void set(@LONG_POINTER int base, boolean newValue) {
                PsbFlags.waiting.set(offset(base), newValue);
            }
        }
        //   PsbFlags  abort
        public static final class abort {
            public static boolean get(@LONG_POINTER int base) {
                return PsbFlags.abort.get(offset(base));
            }
            public static void set(@LONG_POINTER int base, boolean newValue) {
                PsbFlags.abort.set(offset(base), newValue);
            }
        }
    }
    // offset    2  size    1  type Context   name conext
    public static final class conext {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  2;

        public static @LONG_POINTER int offset(@LONG_POINTER int base) {
            return base + OFFSET;
        }
        //   Context  frame
        public static final class frame {
            public static @CARD16 int get(@LONG_POINTER int base) {
                return Context.frame.get(offset(base));
            }
            public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
                Context.frame.set(offset(base), newValue);
            }
        }
        //   Context  state
        public static final class state {
            public static @CARD16 int get(@LONG_POINTER int base) {
                return Context.state.get(offset(base));
            }
            public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
                Context.state.set(offset(base), newValue);
            }
        }
    }
    // offset    3  size    1  type CARD16    name timeout
    public static final class timeout {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  3;

        public static @LONG_POINTER int offset(@LONG_POINTER int base) {
            return base + OFFSET;
        }
        public static @CARD16 int get(@LONG_POINTER int base) {
            return RecordBase.get(ProcessStateBlock.timeout::offset, base);
        }
        public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
            RecordBase.set(ProcessStateBlock.timeout::offset, base, newValue);
        }
    }
    // offset    4  size    1  type CARD16    name mds
    public static final class mds {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  4;

        public static @LONG_POINTER int offset(@LONG_POINTER int base) {
            return base + OFFSET;
        }
        public static @CARD16 int get(@LONG_POINTER int base) {
            return RecordBase.get(ProcessStateBlock.mds::offset, base);
        }
        public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
            RecordBase.set(ProcessStateBlock.mds::offset, base, newValue);
        }
    }
    // offset    5  size    1  type CARD16    name data
    public static final class data {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  5;

        public static @LONG_POINTER int offset(@LONG_POINTER int base) {
            return base + OFFSET;
        }
        public static @CARD16 int get(@LONG_POINTER int base) {
            return RecordBase.get(ProcessStateBlock.data::offset, base);
        }
        public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
            RecordBase.set(ProcessStateBlock.data::offset, base, newValue);
        }
    }
    // offset    6  size    2  type CARD32    name sticky
    public static final class sticky {
        public static final         int SIZE       =  2;
        public static final         int OFFSET     =  6;

        public static @LONG_POINTER int offset(@LONG_POINTER int base) {
            return base + OFFSET;
        }
        public static @CARD32 int get(@LONG_POINTER int base) {
            return RecordBase.getDbl(ProcessStateBlock.sticky::offset, base);
        }
        public static void set(@LONG_POINTER int base, @CARD32 int newValue) {
            RecordBase.setDbl(ProcessStateBlock.sticky::offset, base, newValue);
        }
    }
}
