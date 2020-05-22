package mh.majuro.mesa.type;

import mh.majuro.mesa.Memory;
import mh.majuro.mesa.Type.*;

public final class ProcessStateBlock {
    public static final int SIZE = 8;

    // offset    0  size    1  type PsbLink   name link
    public static final class link {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  0;
        //   PsbLink  priority
        public static final class priority {
            public static final int OFFSET = link.OFFSET +  0;

            public static @CARD16 int get(@LONG_POINTER int base) {
                return PsbLink.priority.get(base + OFFSET);
            }
            public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
                PsbLink.priority.set(base + OFFSET, newValue);
            }
        }
        //   PsbLink  next
        public static final class next {
            public static final int OFFSET = link.OFFSET +  0;

            public static @CARD16 int get(@LONG_POINTER int base) {
                return PsbLink.next.get(base + OFFSET);
            }
            public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
                PsbLink.next.set(base + OFFSET, newValue);
            }
        }
        //   PsbLink  failed
        public static final class failed {
            public static final int OFFSET = link.OFFSET +  0;

            public static boolean get(@LONG_POINTER int base) {
                return PsbLink.failed.get(base + OFFSET);
            }
            public static void set(@LONG_POINTER int base, boolean newValue) {
                PsbLink.failed.set(base + OFFSET, newValue);
            }
        }
        //   PsbLink  permanent
        public static final class permanent {
            public static final int OFFSET = link.OFFSET +  0;

            public static boolean get(@LONG_POINTER int base) {
                return PsbLink.permanent.get(base + OFFSET);
            }
            public static void set(@LONG_POINTER int base, boolean newValue) {
                PsbLink.permanent.set(base + OFFSET, newValue);
            }
        }
        //   PsbLink  preempted
        public static final class preempted {
            public static final int OFFSET = link.OFFSET +  0;

            public static boolean get(@LONG_POINTER int base) {
                return PsbLink.preempted.get(base + OFFSET);
            }
            public static void set(@LONG_POINTER int base, boolean newValue) {
                PsbLink.preempted.set(base + OFFSET, newValue);
            }
        }
    }
    // offset    1  size    1  type PsbFlags  name flags
    public static final class flags {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  1;
        //   PsbFlags  cleanup
        public static final class cleanup {
            public static final int OFFSET = flags.OFFSET +  0;

            public static @CARD16 int get(@LONG_POINTER int base) {
                return PsbFlags.cleanup.get(base + OFFSET);
            }
            public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
                PsbFlags.cleanup.set(base + OFFSET, newValue);
            }
        }
        //   PsbFlags  waiting
        public static final class waiting {
            public static final int OFFSET = flags.OFFSET +  0;

            public static boolean get(@LONG_POINTER int base) {
                return PsbFlags.waiting.get(base + OFFSET);
            }
            public static void set(@LONG_POINTER int base, boolean newValue) {
                PsbFlags.waiting.set(base + OFFSET, newValue);
            }
        }
        //   PsbFlags  abort
        public static final class abort {
            public static final int OFFSET = flags.OFFSET +  0;

            public static boolean get(@LONG_POINTER int base) {
                return PsbFlags.abort.get(base + OFFSET);
            }
            public static void set(@LONG_POINTER int base, boolean newValue) {
                PsbFlags.abort.set(base + OFFSET, newValue);
            }
        }
    }
    // offset    2  size    1  type Context   name conext
    public static final class conext {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  2;
        //   Context  frame
        public static final class frame {
            public static final int OFFSET = conext.OFFSET +  0;

            public static @CARD16 int get(@LONG_POINTER int base) {
                return Context.frame.get(base + OFFSET);
            }
            public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
                Context.frame.set(base + OFFSET, newValue);
            }
        }
        //   Context  state
        public static final class state {
            public static final int OFFSET = conext.OFFSET +  0;

            public static @CARD16 int get(@LONG_POINTER int base) {
                return Context.state.get(base + OFFSET);
            }
            public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
                Context.state.set(base + OFFSET, newValue);
            }
        }
    }
    // offset    3  size    1  type CARD16    name timeout
    public static final class timeout {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  3;
        public static @CARD16 int get(@LONG_POINTER int base) {
            return Memory.fetch(base + OFFSET);
        }
        public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
            Memory.store(base + OFFSET, newValue);
        }
    }
    // offset    4  size    1  type CARD16    name mds
    public static final class mds {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  4;
        public static @CARD16 int get(@LONG_POINTER int base) {
            return Memory.fetch(base + OFFSET);
        }
        public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
            Memory.store(base + OFFSET, newValue);
        }
    }
    // offset    5  size    1  type CARD16    name data
    public static final class data {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  5;
        public static @CARD16 int get(@LONG_POINTER int base) {
            return Memory.fetch(base + OFFSET);
        }
        public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
            Memory.store(base + OFFSET, newValue);
        }
    }
    // offset    6  size    2  type CARD32    name sticky
    public static final class sticky {
        public static final         int SIZE       =  2;
        public static final         int OFFSET     =  6;
        public static @CARD32 int get(@LONG_POINTER int base) {
            return Memory.readDbl(base + OFFSET);
        }
        public static void set(@LONG_POINTER int base, @CARD32 int newValue) {
            Memory.writeDbl(base + OFFSET, newValue);
        }
    }
}
