package mh.majuro.mesa.type;

import mh.majuro.mesa.Type.*;

public final class LocalWord {
    public static final int SIZE = 1;

    // offset    0  size    1  type CARD8     name available
    //   bit startBit  0  stopBit  7
    public static final class available {
        public static final         int SIZE   =  1;
        public static final         int OFFSET =  0;
        public static final @CARD16 int MASK    = 0b1111_1111_0000_0000;
        public static final         int SHIFT   = 8;

        public static @LONG_POINTER int offset(@LONG_POINTER int base) {
            return base + OFFSET;
        }
        public static @CARD16 int getBit(@CARD16 int value) {
            return (value & MASK) >>> SHIFT;
        }
        public static @CARD16 int setBit(@CARD16 int value, @CARD16 int newValue) {
            return ((newValue << SHIFT) & MASK) | (value & ~MASK);
        }
        public static @CARD8 int get(@LONG_POINTER int base) {
            return RecordBase.getBitField(LocalWord.available::offset, LocalWord.available::getBit, base);
        }
        public static void set(@LONG_POINTER int base, @CARD8 int newValue) {
            RecordBase.setBitField(LocalWord.available::offset, LocalWord.available::setBit, base, newValue);
        }
    }
    // offset    0  size    1  type CARD8     name fsi
    //   bit startBit  8  stopBit 15
    public static final class fsi {
        public static final         int SIZE   =  1;
        public static final         int OFFSET =  0;
        public static final @CARD16 int MASK    = 0b0000_0000_1111_1111;
        public static final         int SHIFT   = 0;

        public static @LONG_POINTER int offset(@LONG_POINTER int base) {
            return base + OFFSET;
        }
        public static @CARD16 int getBit(@CARD16 int value) {
            return (value & MASK) >>> SHIFT;
        }
        public static @CARD16 int setBit(@CARD16 int value, @CARD16 int newValue) {
            return ((newValue << SHIFT) & MASK) | (value & ~MASK);
        }
        public static @CARD8 int get(@LONG_POINTER int base) {
            return RecordBase.getBitField(LocalWord.fsi::offset, LocalWord.fsi::getBit, base);
        }
        public static void set(@LONG_POINTER int base, @CARD8 int newValue) {
            RecordBase.setBitField(LocalWord.fsi::offset, LocalWord.fsi::setBit, base, newValue);
        }
    }
}
