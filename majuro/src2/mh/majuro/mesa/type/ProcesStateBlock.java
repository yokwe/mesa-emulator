package mh.majuro.mesa.type;

import mh.majuro.mesa.Type.*;

public final class ProcesStateBlock {
    public static final int SIZE = 8;

    // offset    0  size    1  type PsbLink   name link
    public static final class link {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  0;

        public static @LONG_POINTER int offset(@LONG_POINTER int base) {
            return base + OFFSET;
        }
        // FIXME RECORD
        // FIXME  ProcesStateBlock.link.priority
        // FIXME  ProcesStateBlock.link.next
        // FIXME  ProcesStateBlock.link.failed
        // FIXME  ProcesStateBlock.link.permanent
        // FIXME  ProcesStateBlock.link.preempted
    }
    // offset    1  size    1  type PsbFlags  name flags
    public static final class flags {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  1;

        public static @LONG_POINTER int offset(@LONG_POINTER int base) {
            return base + OFFSET;
        }
        // FIXME RECORD
        // FIXME  ProcesStateBlock.flags.available
        // FIXME  ProcesStateBlock.flags.cleanup
        // FIXME  ProcesStateBlock.flags.reserved
        // FIXME  ProcesStateBlock.flags.waiting
        // FIXME  ProcesStateBlock.flags.abort
    }
    // offset    2  size    1  type Context   name conext
    public static final class conext {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  2;

        public static @LONG_POINTER int offset(@LONG_POINTER int base) {
            return base + OFFSET;
        }
        // FIXME RECORD
        // FIXME  ProcesStateBlock.conext.frame
        // FIXME  ProcesStateBlock.conext.state
    }
    // offset    3  size    1  type CARD16    name timeout
    public static final class timeout {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  3;

        public static @LONG_POINTER int offset(@LONG_POINTER int base) {
            return base + OFFSET;
        }
        public static @CARD16 int get(@LONG_POINTER int base) {
            return RecordBase.get(ProcesStateBlock.timeout::offset, base);
        }
        public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
            RecordBase.set(ProcesStateBlock.timeout::offset, base, newValue);
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
            return RecordBase.get(ProcesStateBlock.mds::offset, base);
        }
        public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
            RecordBase.set(ProcesStateBlock.mds::offset, base, newValue);
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
            return RecordBase.get(ProcesStateBlock.data::offset, base);
        }
        public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
            RecordBase.set(ProcesStateBlock.data::offset, base, newValue);
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
            return RecordBase.getDbl(ProcesStateBlock.sticky::offset, base);
        }
        public static void set(@LONG_POINTER int base, @CARD32 int newValue) {
            RecordBase.setDbl(ProcesStateBlock.sticky::offset, base, newValue);
        }
    }
}
