package mh.majuro.mesa.type;

import mh.majuro.mesa.Memory;
import mh.majuro.mesa.Type.*;

public final class CodeSegment {
    public static final int SIZE = 4;

    // offset    0  size    1  type CARD8     name globalFsi
    //   bit startBit  0  stopBit  7
    public static final class globalFsi {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  0;
        public static final @CARD16 int MASK        = 0b1111_1111_0000_0000;
        public static final         int SHIFT       = 8;

        public static @CARD16 int getBit(@CARD16 int value) {
            return (value & MASK) >>> SHIFT;
        }
        public static @CARD16 int setBit(@CARD16 int value, @CARD16 int newValue) {
            return ((newValue << SHIFT) & MASK) | (value & ~MASK);
        }
        public static @CARD8 int get(@LONG_POINTER int base) {
            return getBit(Memory.fetch(base + OFFSET));
        }
        public static void set(@LONG_POINTER int base, @CARD8 int newValue) {
            Memory.modify(base + OFFSET, CodeSegment.globalFsi::setBit, newValue);
        }
    }
    // offset    0  size    1  type CARD8     name nlinks
    //   bit startBit  8  stopBit 15
    public static final class nlinks {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  0;
        public static final @CARD16 int MASK        = 0b0000_0000_1111_1111;
        public static final         int SHIFT       = 0;

        public static @CARD16 int getBit(@CARD16 int value) {
            return (value & MASK) >>> SHIFT;
        }
        public static @CARD16 int setBit(@CARD16 int value, @CARD16 int newValue) {
            return ((newValue << SHIFT) & MASK) | (value & ~MASK);
        }
        public static @CARD8 int get(@LONG_POINTER int base) {
            return getBit(Memory.fetch(base + OFFSET));
        }
        public static void set(@LONG_POINTER int base, @CARD8 int newValue) {
            Memory.modify(base + OFFSET, CodeSegment.nlinks::setBit, newValue);
        }
    }
    // offset    1  size    1  type boolean   name stops
    //   bit startBit  0  stopBit  0
    public static final class stops {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  1;
        public static final @CARD16 int MASK        = 0b1000_0000_0000_0000;
        public static final         int SHIFT       = 15;

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
            Memory.modify(base + OFFSET, CodeSegment.stops::setBit, (newValue ? 1 : 0));
        }
    }
    // offset    1  size    1  type           name available
    //   bit startBit  1  stopBit 15
    // offset    2  size    1  type CARD16    name mainBody
    public static final class mainBody {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  2;
        public static @CARD16 int get(@LONG_POINTER int base) {
            return Memory.fetch(base + OFFSET);
        }
        public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
            Memory.store(base + OFFSET, newValue);
        }
    }
    // offset    3  size    1  type CARD16    name catchCode
    public static final class catchCode {
        public static final         int SIZE       =  1;
        public static final         int OFFSET     =  3;
        public static @CARD16 int get(@LONG_POINTER int base) {
            return Memory.fetch(base + OFFSET);
        }
        public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
            Memory.store(base + OFFSET, newValue);
        }
    }
}
