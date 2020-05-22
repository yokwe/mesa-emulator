package mh.majuro.mesa.type;

import mh.majuro.mesa.Type.*;

public final class ProcessDataArea {
    public static final int SIZE = 8196;

    // offset    0  size    1  type Queue     name ready
    public static final class ready {
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
    // offset    1  size    1  type CARD16    name count
    public static final class count {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  1;

        public static @LONG_POINTER int offset(@LONG_POINTER int base) {
            return base + OFFSET;
        }
        public static @CARD16 int get(@LONG_POINTER int base) {
            return RecordBase.get(ProcessDataArea.count::offset, base);
        }
        public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
            RecordBase.set(ProcessDataArea.count::offset, base, newValue);
        }
    }
    // offset    2  size    1  type           name unused
    // offset    3  size    5  type           name available
    // offset    8  size    8  type           name state
    // offset   16  size   32  type           name interrupt
    //   array  index InterruptLevle    size    2  length  16  element InterruptItem
    public static final class interrupt {
        public static final         int SIZE       = 32;
        public static final         int OFFSET     = 16;
        public static final         int ARRAY_SIZE = 2;
        public static final         int ARRAY_LEN  = 16;

        public static @LONG_POINTER int offset(@LONG_POINTER int base, int index) {
            return base + OFFSET + (ARRAY_SIZE * index);
        }
        // InterruptItem  condition  Condition
        public static final class condition {
            // Condition  tail  CARD16
            public static final class tail {
                public static @CARD16 int get(@LONG_POINTER int base, int index) {
                    return Condition.tail.get(offset(base, index));
                }
                public static void set(@LONG_POINTER int base, int index, @CARD16 int newValue) {
                    Condition.tail.set(offset(base, index), newValue);
                }
            }
            // Condition  abortable  boolean
            public static final class abortable {
                public static boolean get(@LONG_POINTER int base, int index) {
                    return Condition.abortable.get(offset(base, index));
                }
                public static void set(@LONG_POINTER int base, int index, boolean newValue) {
                    Condition.abortable.set(offset(base, index), newValue);
                }
            }
            // Condition  wakeup  boolean
            public static final class wakeup {
                public static boolean get(@LONG_POINTER int base, int index) {
                    return Condition.wakeup.get(offset(base, index));
                }
                public static void set(@LONG_POINTER int base, int index, boolean newValue) {
                    Condition.wakeup.set(offset(base, index), newValue);
                }
            }
        }
    }
    // offset   48  size   16  type           name fault
    //   array  index FaultIndex        size    2  length   8  element FaultQueue
    public static final class fault {
        public static final         int SIZE       = 16;
        public static final         int OFFSET     = 48;
        public static final         int ARRAY_SIZE = 2;
        public static final         int ARRAY_LEN  = 8;

        public static @LONG_POINTER int offset(@LONG_POINTER int base, int index) {
            return base + OFFSET + (ARRAY_SIZE * index);
        }
        // FaultQueue  queue  Queue
        public static final class queue {
            // Queue  tail  CARD16
            public static final class tail {
                public static @CARD16 int get(@LONG_POINTER int base, int index) {
                    return Queue.tail.get(offset(base, index));
                }
                public static void set(@LONG_POINTER int base, int index, @CARD16 int newValue) {
                    Queue.tail.set(offset(base, index), newValue);
                }
            }
        }
        // FaultQueue  condition  Condition
        public static final class condition {
            // Condition  tail  CARD16
            public static final class tail {
                public static @CARD16 int get(@LONG_POINTER int base, int index) {
                    return Condition.tail.get(offset(base, index));
                }
                public static void set(@LONG_POINTER int base, int index, @CARD16 int newValue) {
                    Condition.tail.set(offset(base, index), newValue);
                }
            }
            // Condition  abortable  boolean
            public static final class abortable {
                public static boolean get(@LONG_POINTER int base, int index) {
                    return Condition.abortable.get(offset(base, index));
                }
                public static void set(@LONG_POINTER int base, int index, boolean newValue) {
                    Condition.abortable.set(offset(base, index), newValue);
                }
            }
            // Condition  wakeup  boolean
            public static final class wakeup {
                public static boolean get(@LONG_POINTER int base, int index) {
                    return Condition.wakeup.get(offset(base, index));
                }
                public static void set(@LONG_POINTER int base, int index, boolean newValue) {
                    Condition.wakeup.set(offset(base, index), newValue);
                }
            }
        }
    }
    // offset    0  size 8196  type           name block
    //   array  index PsbIndex          size    8  length 1024  element ProcessStateBlock
    public static final class block {
        public static final         int SIZE       = 8196;
        public static final         int OFFSET     =  0;
        public static final         int ARRAY_SIZE = 8;
        public static final         int ARRAY_LEN  = 1024;

        public static @LONG_POINTER int offset(@LONG_POINTER int base, int index) {
            return base + OFFSET + (ARRAY_SIZE * index);
        }
        // ProcessStateBlock  link  PsbLink
        public static final class link {
            // PsbLink  priority  CARD16
            public static final class priority {
                public static @CARD16 int get(@LONG_POINTER int base, int index) {
                    return PsbLink.priority.get(offset(base, index));
                }
                public static void set(@LONG_POINTER int base, int index, @CARD16 int newValue) {
                    PsbLink.priority.set(offset(base, index), newValue);
                }
            }
            // PsbLink  next  CARD16
            public static final class next {
                public static @CARD16 int get(@LONG_POINTER int base, int index) {
                    return PsbLink.next.get(offset(base, index));
                }
                public static void set(@LONG_POINTER int base, int index, @CARD16 int newValue) {
                    PsbLink.next.set(offset(base, index), newValue);
                }
            }
            // PsbLink  failed  boolean
            public static final class failed {
                public static boolean get(@LONG_POINTER int base, int index) {
                    return PsbLink.failed.get(offset(base, index));
                }
                public static void set(@LONG_POINTER int base, int index, boolean newValue) {
                    PsbLink.failed.set(offset(base, index), newValue);
                }
            }
            // PsbLink  permanent  boolean
            public static final class permanent {
                public static boolean get(@LONG_POINTER int base, int index) {
                    return PsbLink.permanent.get(offset(base, index));
                }
                public static void set(@LONG_POINTER int base, int index, boolean newValue) {
                    PsbLink.permanent.set(offset(base, index), newValue);
                }
            }
            // PsbLink  preempted  boolean
            public static final class preempted {
                public static boolean get(@LONG_POINTER int base, int index) {
                    return PsbLink.preempted.get(offset(base, index));
                }
                public static void set(@LONG_POINTER int base, int index, boolean newValue) {
                    PsbLink.preempted.set(offset(base, index), newValue);
                }
            }
        }
        // ProcessStateBlock  flags  PsbFlags
        public static final class flags {
            // PsbFlags  cleanup  CARD16
            public static final class cleanup {
                public static @CARD16 int get(@LONG_POINTER int base, int index) {
                    return PsbFlags.cleanup.get(offset(base, index));
                }
                public static void set(@LONG_POINTER int base, int index, @CARD16 int newValue) {
                    PsbFlags.cleanup.set(offset(base, index), newValue);
                }
            }
            // PsbFlags  waiting  boolean
            public static final class waiting {
                public static boolean get(@LONG_POINTER int base, int index) {
                    return PsbFlags.waiting.get(offset(base, index));
                }
                public static void set(@LONG_POINTER int base, int index, boolean newValue) {
                    PsbFlags.waiting.set(offset(base, index), newValue);
                }
            }
            // PsbFlags  abort  boolean
            public static final class abort {
                public static boolean get(@LONG_POINTER int base, int index) {
                    return PsbFlags.abort.get(offset(base, index));
                }
                public static void set(@LONG_POINTER int base, int index, boolean newValue) {
                    PsbFlags.abort.set(offset(base, index), newValue);
                }
            }
        }
        // ProcessStateBlock  conext  Context
        public static final class conext {
            // Context  frame  CARD16
            public static final class frame {
                public static @CARD16 int get(@LONG_POINTER int base, int index) {
                    return Context.frame.get(offset(base, index));
                }
                public static void set(@LONG_POINTER int base, int index, @CARD16 int newValue) {
                    Context.frame.set(offset(base, index), newValue);
                }
            }
            // Context  state  CARD16
            public static final class state {
                public static @CARD16 int get(@LONG_POINTER int base, int index) {
                    return Context.state.get(offset(base, index));
                }
                public static void set(@LONG_POINTER int base, int index, @CARD16 int newValue) {
                    Context.state.set(offset(base, index), newValue);
                }
            }
        }
        // ProcessStateBlock  timeout  CARD16
        public static final class timeout {
            public static @CARD16 int get(@LONG_POINTER int base, int index) {
                return ProcessStateBlock.timeout.get(offset(base, index));
            }
            public static void set(@LONG_POINTER int base, int index, @CARD16 int newValue) {
                ProcessStateBlock.timeout.set(offset(base, index), newValue);
            }
        }
        // ProcessStateBlock  mds  CARD16
        public static final class mds {
            public static @CARD16 int get(@LONG_POINTER int base, int index) {
                return ProcessStateBlock.mds.get(offset(base, index));
            }
            public static void set(@LONG_POINTER int base, int index, @CARD16 int newValue) {
                ProcessStateBlock.mds.set(offset(base, index), newValue);
            }
        }
        // ProcessStateBlock  data  CARD16
        public static final class data {
            public static @CARD16 int get(@LONG_POINTER int base, int index) {
                return ProcessStateBlock.data.get(offset(base, index));
            }
            public static void set(@LONG_POINTER int base, int index, @CARD16 int newValue) {
                ProcessStateBlock.data.set(offset(base, index), newValue);
            }
        }
        // ProcessStateBlock  sticky  CARD32
        public static final class sticky {
            public static @CARD32 int get(@LONG_POINTER int base, int index) {
                return ProcessStateBlock.sticky.get(offset(base, index));
            }
            public static void set(@LONG_POINTER int base, int index, @CARD32 int newValue) {
                ProcessStateBlock.sticky.set(offset(base, index), newValue);
            }
        }
    }
}
