package mh.majuro.mesa.type;

import mh.majuro.mesa.Memory;
import mh.majuro.mesa.Type.*;

public final class ProcessDataArea {
    public static final int SIZE = 8196;

    // offset    0  size    1  type Queue     name ready
    public static final class ready {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  0;
        //   Queue  tail
        public static final class tail {
            public static final int OFFSET = ready.OFFSET +  0;

            public static @CARD16 int get(@LONG_POINTER int base) {
                return Queue.tail.get(base + OFFSET);
            }
            public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
                Queue.tail.set(base + OFFSET, newValue);
            }
        }
    }
    // offset    1  size    1  type CARD16    name count
    public static final class count {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  1;
        public static @CARD16 int get(@LONG_POINTER int base) {
            return Memory.fetch(base + OFFSET);
        }
        public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
            Memory.store(base + OFFSET, newValue);
        }
    }
    // offset    2  size    1  type           name unused
    // offset    3  size    5  type           name available
    // offset    8  size    8  type           name state
    //   array  index CARD16            size    1  length   8  element CARD16
    public static final class state {
        public static final         int SIZE       =  8;
        public static final         int OFFSET     =  8;
        public static final         int ARRAY_SIZE =  1;
        public static final         int ARRAY_LEN  =  8;

        public static @CARD16 int get(@LONG_POINTER int base, int index) {
            return Memory.fetch(base + OFFSET + (ARRAY_SIZE * index));
        }
        public static void set(@LONG_POINTER int base, int index, @CARD16 int newValue) {
            Memory.store(base + OFFSET + (ARRAY_SIZE * index), newValue);
        }
    }
    // offset   16  size   32  type           name interrupt
    //   array  index InterruptLevle    size    2  length  16  element InterruptItem
    public static final class interrupt {
        public static final         int SIZE       = 32;
        public static final         int OFFSET     = 16;
        public static final         int ARRAY_SIZE =  2;
        public static final         int ARRAY_LEN  = 16;

        // InterruptItem  condition  Condition
        public static final class condition {
            public static final int OFFSET = interrupt.OFFSET +  0;

            // Condition  tail  CARD16
            public static final class tail {
                public static final int OFFSET = interrupt.condition.OFFSET +  0;

                public static @CARD16 int get(@LONG_POINTER int base, int index) {
                    return Condition.tail.get(base + OFFSET + (ARRAY_SIZE * index));
                }
                public static void set(@LONG_POINTER int base, int index, @CARD16 int newValue) {
                    Condition.tail.set(base + OFFSET + (ARRAY_SIZE * index), newValue);
                }
            }
            // Condition  abortable  boolean
            public static final class abortable {
                public static final int OFFSET = interrupt.condition.OFFSET +  0;

                public static boolean get(@LONG_POINTER int base, int index) {
                    return Condition.abortable.get(base + OFFSET + (ARRAY_SIZE * index));
                }
                public static void set(@LONG_POINTER int base, int index, boolean newValue) {
                    Condition.abortable.set(base + OFFSET + (ARRAY_SIZE * index), newValue);
                }
            }
            // Condition  wakeup  boolean
            public static final class wakeup {
                public static final int OFFSET = interrupt.condition.OFFSET +  0;

                public static boolean get(@LONG_POINTER int base, int index) {
                    return Condition.wakeup.get(base + OFFSET + (ARRAY_SIZE * index));
                }
                public static void set(@LONG_POINTER int base, int index, boolean newValue) {
                    Condition.wakeup.set(base + OFFSET + (ARRAY_SIZE * index), newValue);
                }
            }
        }
    }
    // offset   48  size   16  type           name fault
    //   array  index FaultIndex        size    2  length   8  element FaultQueue
    public static final class fault {
        public static final         int SIZE       = 16;
        public static final         int OFFSET     = 48;
        public static final         int ARRAY_SIZE =  2;
        public static final         int ARRAY_LEN  =  8;

        // FaultQueue  queue  Queue
        public static final class queue {
            public static final int OFFSET = fault.OFFSET +  0;

            // Queue  tail  CARD16
            public static final class tail {
                public static final int OFFSET = fault.queue.OFFSET +  0;

                public static @CARD16 int get(@LONG_POINTER int base, int index) {
                    return Queue.tail.get(base + OFFSET + (ARRAY_SIZE * index));
                }
                public static void set(@LONG_POINTER int base, int index, @CARD16 int newValue) {
                    Queue.tail.set(base + OFFSET + (ARRAY_SIZE * index), newValue);
                }
            }
        }
        // FaultQueue  condition  Condition
        public static final class condition {
            public static final int OFFSET = fault.OFFSET +  1;

            // Condition  tail  CARD16
            public static final class tail {
                public static final int OFFSET = fault.condition.OFFSET +  0;

                public static @CARD16 int get(@LONG_POINTER int base, int index) {
                    return Condition.tail.get(base + OFFSET + (ARRAY_SIZE * index));
                }
                public static void set(@LONG_POINTER int base, int index, @CARD16 int newValue) {
                    Condition.tail.set(base + OFFSET + (ARRAY_SIZE * index), newValue);
                }
            }
            // Condition  abortable  boolean
            public static final class abortable {
                public static final int OFFSET = fault.condition.OFFSET +  0;

                public static boolean get(@LONG_POINTER int base, int index) {
                    return Condition.abortable.get(base + OFFSET + (ARRAY_SIZE * index));
                }
                public static void set(@LONG_POINTER int base, int index, boolean newValue) {
                    Condition.abortable.set(base + OFFSET + (ARRAY_SIZE * index), newValue);
                }
            }
            // Condition  wakeup  boolean
            public static final class wakeup {
                public static final int OFFSET = fault.condition.OFFSET +  0;

                public static boolean get(@LONG_POINTER int base, int index) {
                    return Condition.wakeup.get(base + OFFSET + (ARRAY_SIZE * index));
                }
                public static void set(@LONG_POINTER int base, int index, boolean newValue) {
                    Condition.wakeup.set(base + OFFSET + (ARRAY_SIZE * index), newValue);
                }
            }
        }
    }
    // offset    0  size 8196  type           name block
    //   array  index PsbIndex          size    8  length 1024  element ProcessStateBlock
    public static final class block {
        public static final         int SIZE       = 8196;
        public static final         int OFFSET     =  0;
        public static final         int ARRAY_SIZE =  8;
        public static final         int ARRAY_LEN  = 1024;

        // ProcessStateBlock  link  PsbLink
        public static final class link {
            public static final int OFFSET = block.OFFSET +  0;

            // PsbLink  priority  CARD16
            public static final class priority {
                public static final int OFFSET = block.link.OFFSET +  0;

                public static @CARD16 int get(@LONG_POINTER int base, int index) {
                    return PsbLink.priority.get(base + OFFSET + (ARRAY_SIZE * index));
                }
                public static void set(@LONG_POINTER int base, int index, @CARD16 int newValue) {
                    PsbLink.priority.set(base + OFFSET + (ARRAY_SIZE * index), newValue);
                }
            }
            // PsbLink  next  CARD16
            public static final class next {
                public static final int OFFSET = block.link.OFFSET +  0;

                public static @CARD16 int get(@LONG_POINTER int base, int index) {
                    return PsbLink.next.get(base + OFFSET + (ARRAY_SIZE * index));
                }
                public static void set(@LONG_POINTER int base, int index, @CARD16 int newValue) {
                    PsbLink.next.set(base + OFFSET + (ARRAY_SIZE * index), newValue);
                }
            }
            // PsbLink  failed  boolean
            public static final class failed {
                public static final int OFFSET = block.link.OFFSET +  0;

                public static boolean get(@LONG_POINTER int base, int index) {
                    return PsbLink.failed.get(base + OFFSET + (ARRAY_SIZE * index));
                }
                public static void set(@LONG_POINTER int base, int index, boolean newValue) {
                    PsbLink.failed.set(base + OFFSET + (ARRAY_SIZE * index), newValue);
                }
            }
            // PsbLink  permanent  boolean
            public static final class permanent {
                public static final int OFFSET = block.link.OFFSET +  0;

                public static boolean get(@LONG_POINTER int base, int index) {
                    return PsbLink.permanent.get(base + OFFSET + (ARRAY_SIZE * index));
                }
                public static void set(@LONG_POINTER int base, int index, boolean newValue) {
                    PsbLink.permanent.set(base + OFFSET + (ARRAY_SIZE * index), newValue);
                }
            }
            // PsbLink  preempted  boolean
            public static final class preempted {
                public static final int OFFSET = block.link.OFFSET +  0;

                public static boolean get(@LONG_POINTER int base, int index) {
                    return PsbLink.preempted.get(base + OFFSET + (ARRAY_SIZE * index));
                }
                public static void set(@LONG_POINTER int base, int index, boolean newValue) {
                    PsbLink.preempted.set(base + OFFSET + (ARRAY_SIZE * index), newValue);
                }
            }
        }
        // ProcessStateBlock  flags  PsbFlags
        public static final class flags {
            public static final int OFFSET = block.OFFSET +  1;

            // PsbFlags  cleanup  CARD16
            public static final class cleanup {
                public static final int OFFSET = block.flags.OFFSET +  0;

                public static @CARD16 int get(@LONG_POINTER int base, int index) {
                    return PsbFlags.cleanup.get(base + OFFSET + (ARRAY_SIZE * index));
                }
                public static void set(@LONG_POINTER int base, int index, @CARD16 int newValue) {
                    PsbFlags.cleanup.set(base + OFFSET + (ARRAY_SIZE * index), newValue);
                }
            }
            // PsbFlags  waiting  boolean
            public static final class waiting {
                public static final int OFFSET = block.flags.OFFSET +  0;

                public static boolean get(@LONG_POINTER int base, int index) {
                    return PsbFlags.waiting.get(base + OFFSET + (ARRAY_SIZE * index));
                }
                public static void set(@LONG_POINTER int base, int index, boolean newValue) {
                    PsbFlags.waiting.set(base + OFFSET + (ARRAY_SIZE * index), newValue);
                }
            }
            // PsbFlags  abort  boolean
            public static final class abort {
                public static final int OFFSET = block.flags.OFFSET +  0;

                public static boolean get(@LONG_POINTER int base, int index) {
                    return PsbFlags.abort.get(base + OFFSET + (ARRAY_SIZE * index));
                }
                public static void set(@LONG_POINTER int base, int index, boolean newValue) {
                    PsbFlags.abort.set(base + OFFSET + (ARRAY_SIZE * index), newValue);
                }
            }
        }
        // ProcessStateBlock  conext  Context
        public static final class conext {
            public static final int OFFSET = block.OFFSET +  2;

            // Context  frame  CARD16
            public static final class frame {
                public static final int OFFSET = block.conext.OFFSET +  0;

                public static @CARD16 int get(@LONG_POINTER int base, int index) {
                    return Context.frame.get(base + OFFSET + (ARRAY_SIZE * index));
                }
                public static void set(@LONG_POINTER int base, int index, @CARD16 int newValue) {
                    Context.frame.set(base + OFFSET + (ARRAY_SIZE * index), newValue);
                }
            }
            // Context  state  CARD16
            public static final class state {
                public static final int OFFSET = block.conext.OFFSET +  0;

                public static @CARD16 int get(@LONG_POINTER int base, int index) {
                    return Context.state.get(base + OFFSET + (ARRAY_SIZE * index));
                }
                public static void set(@LONG_POINTER int base, int index, @CARD16 int newValue) {
                    Context.state.set(base + OFFSET + (ARRAY_SIZE * index), newValue);
                }
            }
        }
        // ProcessStateBlock  timeout  CARD16
        public static final class timeout {
            public static final int OFFSET = block.OFFSET +  3;

            public static @CARD16 int get(@LONG_POINTER int base, int index) {
                return ProcessStateBlock.timeout.get(base + OFFSET + (ARRAY_SIZE * index));
            }
            public static void set(@LONG_POINTER int base, int index, @CARD16 int newValue) {
                ProcessStateBlock.timeout.set(base + OFFSET + (ARRAY_SIZE * index), newValue);
            }
        }
        // ProcessStateBlock  mds  CARD16
        public static final class mds {
            public static final int OFFSET = block.OFFSET +  4;

            public static @CARD16 int get(@LONG_POINTER int base, int index) {
                return ProcessStateBlock.mds.get(base + OFFSET + (ARRAY_SIZE * index));
            }
            public static void set(@LONG_POINTER int base, int index, @CARD16 int newValue) {
                ProcessStateBlock.mds.set(base + OFFSET + (ARRAY_SIZE * index), newValue);
            }
        }
        // ProcessStateBlock  data  CARD16
        public static final class data {
            public static final int OFFSET = block.OFFSET +  5;

            public static @CARD16 int get(@LONG_POINTER int base, int index) {
                return ProcessStateBlock.data.get(base + OFFSET + (ARRAY_SIZE * index));
            }
            public static void set(@LONG_POINTER int base, int index, @CARD16 int newValue) {
                ProcessStateBlock.data.set(base + OFFSET + (ARRAY_SIZE * index), newValue);
            }
        }
        // ProcessStateBlock  sticky  CARD32
        public static final class sticky {
            public static final int OFFSET = block.OFFSET +  6;

            public static @CARD32 int get(@LONG_POINTER int base, int index) {
                return ProcessStateBlock.sticky.get(base + OFFSET + (ARRAY_SIZE * index));
            }
            public static void set(@LONG_POINTER int base, int index, @CARD32 int newValue) {
                ProcessStateBlock.sticky.set(base + OFFSET + (ARRAY_SIZE * index), newValue);
            }
        }
    }
}
