package mh.majuro.mesa.type;

import mh.majuro.mesa.Memory;
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

        public static @CARD16 int getBit(@CARD16 int value) {
            return (value & MASK) >>> SHIFT;
        }
        public static @CARD16 int setBit(@CARD16 int value, @CARD16 int newValue) {
            return ((newValue << SHIFT) & MASK) | (value & ~MASK);
        }
        public static @CARD16 int get(@LONG_POINTER int base) {
            return getBit(Memory.fetch(base + OFFSET));
        }
        public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
            Memory.modify(base + OFFSET, GlobalWord.gfi::setBit, newValue);
        }
    }
    // offset    0  size    1  type boolean   name trapxfers
    //   bit startBit 14  stopBit 14
    public static final class trapxfers {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  0;
        public static final @CARD16 int MASK        = 0b0000_0000_0000_0010;
        public static final         int SHIFT       = 1;

        public static @CARD16 int getBit(@CARD16 int value) {
            return (value & MASK) >>> SHIFT;
        }
        public static @CARD16 int setBit(@CARD16 int value, @CARD16 int newValue) {
            return ((newValue << SHIFT) & MASK) | (value & ~MASK);
        }
        public static boolean get(@LONG_POINTER int base) {
            return getBit(Memory.fetch(base + OFFSET)) != 0;
        }
        public static void set(@LONG_POINTER int base, boolean newValue) {
            Memory.modify(base + OFFSET, GlobalWord.trapxfers::setBit, (newValue ? 1 : 0));
        }
    }
    // offset    0  size    1  type boolean   name codelinks
    //   bit startBit 15  stopBit 15
    public static final class codelinks {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  0;
        public static final @CARD16 int MASK        = 0b0000_0000_0000_0001;
        public static final         int SHIFT       = 0;

        public static @CARD16 int getBit(@CARD16 int value) {
            return (value & MASK) >>> SHIFT;
        }
        public static @CARD16 int setBit(@CARD16 int value, @CARD16 int newValue) {
            return ((newValue << SHIFT) & MASK) | (value & ~MASK);
        }
        public static boolean get(@LONG_POINTER int base) {
            return getBit(Memory.fetch(base + OFFSET)) != 0;
        }
        public static void set(@LONG_POINTER int base, boolean newValue) {
            Memory.modify(base + OFFSET, GlobalWord.codelinks::setBit, (newValue ? 1 : 0));
        }
    }
}
