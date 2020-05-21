package mh.majuro.mesa.type;

import mh.majuro.mesa.Type.*;

public final class ProcessDataArea {
    public static final int SIZE = 8196;

    // offset    0  size    1  type Queue     name ready
    public static final class ready {
        public static final         int SIZE   =  1;
        public static final         int OFFSET =  0;

        public static @LONG_POINTER int offset(@LONG_POINTER int base) {
            return base + OFFSET;
        }
        public static @CARD16 int get(@LONG_POINTER int base) {
            return RecordBase.get(ProcessDataArea.ready::offset, base);
        }
        public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
            RecordBase.set(ProcessDataArea.ready::offset, base, newValue);
        }
    }
    // offset    1  size    1  type CARD16    name count
    public static final class count {
        public static final         int SIZE   =  1;
        public static final         int OFFSET =  1;

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
        public static final         int SIZE   = 32;
        public static final         int OFFSET = 16;
        // FIXME ARRAY {ARRAY    interrupt                                   16 32}
        // FIXME ARRAY {ARRAY    interrupt                                   16 32}
    }
    // offset   48  size   16  type           name fault
    //   array  index FaultIndex        size    2  length   8  element FaultQueue
    public static final class fault {
        public static final         int SIZE   = 16;
        public static final         int OFFSET = 48;
        // FIXME ARRAY {ARRAY    fault                                       48 16}
        // FIXME ARRAY {ARRAY    fault                                       48 16}
    }
    // offset    0  size 8196  type           name block
    //   array  index PsbIndex          size    8  length 1024  element ProcessStateBlock
    public static final class block {
        public static final         int SIZE   = 8196;
        public static final         int OFFSET =  0;
        // FIXME ARRAY {ARRAY    block                                        0 8196}
        // FIXME ARRAY {ARRAY    block                                        0 8196}
    }
}
