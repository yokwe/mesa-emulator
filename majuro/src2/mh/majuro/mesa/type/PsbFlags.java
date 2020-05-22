package mh.majuro.mesa.type;

import mh.majuro.mesa.Type.*;

public final class PsbFlags {
    public static final int SIZE = 1;

    // offset    0  size    1  type           name available
    //   bit startBit  0  stopBit  2
    // offset    0  size    1  type PsbIndex  name cleanup
    //   bit startBit  3  stopBit 12
    public static final class cleanup {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  0;
        public static final @CARD16 int MASK        = 0b0001_1111_1111_1000;
        public static final         int SHIFT       = 3;

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
            return RecordBase.getBitField(PsbFlags.cleanup::offset, PsbFlags.cleanup::getBit, base);
        }
        public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
            RecordBase.setBitField(PsbFlags.cleanup::offset, PsbFlags.cleanup::setBit, base, newValue);
        }
    }
    // offset    0  size    1  type           name reserved
    //   bit startBit 13  stopBit 13
    // offset    0  size    1  type boolean   name waiting
    //   bit startBit 14  stopBit 14
    public static final class waiting {
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
            return RecordBase.getBitField(PsbFlags.waiting::offset, PsbFlags.waiting::getBit, base) != 0;
        }
        public static void set(@LONG_POINTER int base, boolean newValue) {
            RecordBase.setBitField(PsbFlags.waiting::offset, PsbFlags.waiting::setBit, base, (newValue ? 1 : 0));
        }
    }
    // offset    0  size    1  type boolean   name abort
    //   bit startBit 15  stopBit 15
    public static final class abort {
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
            return RecordBase.getBitField(PsbFlags.abort::offset, PsbFlags.abort::getBit, base) != 0;
        }
        public static void set(@LONG_POINTER int base, boolean newValue) {
            RecordBase.setBitField(PsbFlags.abort::offset, PsbFlags.abort::setBit, base, (newValue ? 1 : 0));
        }
    }
}
