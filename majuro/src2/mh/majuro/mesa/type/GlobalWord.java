package mh.majuro.mesa.type;

import mh.majuro.mesa.Type.*;

public final class GlobalWord {
    public static final int SIZE = 1;

    // offset    0  size    1  type CARD16    name gfi
    //   bit startBit  0  stopBit 13
    public static final class gfi {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  0;
        public static final @CARD16 int MASK        = 0b1111_1111_1111_1100;
        public static final         int SHIFT       = 2;

        public static @LONG_POINTER int offset(@LONG_POINTER int base) {
            return base + OFFSET;
        }
        public static @CARD16 int getBit(@CARD16 int value) {
            return (value & MASK) >>> SHIFT;
        }
        public static @CARD16 int setBit(@CARD16 int value, @CARD16 int newValue) {
            return ((newValue << SHIFT) & MASK) | (value & ~MASK);
        }
        public static @CARD16 int get(@LONG_POINTER int base) {
            return RecordBase.getBitField(GlobalWord.gfi::offset, GlobalWord.gfi::getBit, base);
        }
        public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
            RecordBase.setBitField(GlobalWord.gfi::offset, GlobalWord.gfi::setBit, base, newValue);
        }
    }
    // offset    0  size    1  type boolean   name trapxfers
    //   bit startBit 14  stopBit 14
    public static final class trapxfers {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  0;
        public static final @CARD16 int MASK        = 0b0000_0000_0000_0010;
        public static final         int SHIFT       = 1;

        public static @LONG_POINTER int offset(@LONG_POINTER int base) {
            return base + OFFSET;
        }
        public static @CARD16 int getBit(@CARD16 int value) {
            return (value & MASK) >>> SHIFT;
        }
        public static @CARD16 int setBit(@CARD16 int value, @CARD16 int newValue) {
            return ((newValue << SHIFT) & MASK) | (value & ~MASK);
        }
        public static boolean get(@LONG_POINTER int base) {
            return RecordBase.getBitField(GlobalWord.trapxfers::offset, GlobalWord.trapxfers::getBit, base) != 0;
        }
        public static void set(@LONG_POINTER int base, boolean newValue) {
            RecordBase.setBitField(GlobalWord.trapxfers::offset, GlobalWord.trapxfers::setBit, base, (newValue ? 1 : 0));
        }
    }
    // offset    0  size    1  type boolean   name codelinks
    //   bit startBit 15  stopBit 15
    public static final class codelinks {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  0;
        public static final @CARD16 int MASK        = 0b0000_0000_0000_0001;
        public static final         int SHIFT       = 0;

        public static @LONG_POINTER int offset(@LONG_POINTER int base) {
            return base + OFFSET;
        }
        public static @CARD16 int getBit(@CARD16 int value) {
            return (value & MASK) >>> SHIFT;
        }
        public static @CARD16 int setBit(@CARD16 int value, @CARD16 int newValue) {
            return ((newValue << SHIFT) & MASK) | (value & ~MASK);
        }
        public static boolean get(@LONG_POINTER int base) {
            return RecordBase.getBitField(GlobalWord.codelinks::offset, GlobalWord.codelinks::getBit, base) != 0;
        }
        public static void set(@LONG_POINTER int base, boolean newValue) {
            RecordBase.setBitField(GlobalWord.codelinks::offset, GlobalWord.codelinks::setBit, base, (newValue ? 1 : 0));
        }
    }
}
