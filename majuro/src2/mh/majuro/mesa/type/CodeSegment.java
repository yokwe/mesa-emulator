package mh.majuro.mesa.type;

import mh.majuro.mesa.Type.*;

public final class CodeSegment {
    public static final int SIZE = 4;

    // offset    0  size    1  type CARD8     name globalFsi
    //   bit startBit  0  stopBit  7
    public static final class globalFsi {
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
            return RecordBase.getBitField(CodeSegment.globalFsi::offset, CodeSegment.globalFsi::getBit, base);
        }
        public static void set(@LONG_POINTER int base, @CARD8 int newValue) {
            RecordBase.setBitField(CodeSegment.globalFsi::offset, CodeSegment.globalFsi::setBit, base, newValue);
        }
    }
    // offset    0  size    1  type CARD8     name nlinks
    //   bit startBit  8  stopBit 15
    public static final class nlinks {
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
            return RecordBase.getBitField(CodeSegment.nlinks::offset, CodeSegment.nlinks::getBit, base);
        }
        public static void set(@LONG_POINTER int base, @CARD8 int newValue) {
            RecordBase.setBitField(CodeSegment.nlinks::offset, CodeSegment.nlinks::setBit, base, newValue);
        }
    }
    // offset    1  size    1  type boolean   name stops
    //   bit startBit  0  stopBit  0
    public static final class stops {
        public static final         int SIZE   =  1;
        public static final         int OFFSET =  1;
        public static final @CARD16 int MASK    = 0b1000_0000_0000_0000;
        public static final         int SHIFT   = 15;

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
            return RecordBase.getBitField(CodeSegment.stops::offset, CodeSegment.stops::getBit, base) != 0;
        }
        public static void set(@LONG_POINTER int base, boolean newValue) {
            RecordBase.setBitField(CodeSegment.stops::offset, CodeSegment.stops::setBit, base, (newValue ? 1 : 0));
        }
    }
    // offset    1  size    1  type           name available
    //   bit startBit  1  stopBit 15
    // offset    2  size    1  type CARD16    name mainBody
    public static final class mainBody {
        public static final         int SIZE   =  1;
        public static final         int OFFSET =  2;

        public static @LONG_POINTER int offset(@LONG_POINTER int base) {
            return base + OFFSET;
        }
        public static @CARD16 int get(@LONG_POINTER int base) {
            return RecordBase.get(CodeSegment.mainBody::offset, base);
        }
        public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
            RecordBase.set(CodeSegment.mainBody::offset, base, newValue);
        }
    }
    // offset    3  size    1  type CARD16    name catchCode
    public static final class catchCode {
        public static final         int SIZE   =  1;
        public static final         int OFFSET =  3;

        public static @LONG_POINTER int offset(@LONG_POINTER int base) {
            return base + OFFSET;
        }
        public static @CARD16 int get(@LONG_POINTER int base) {
            return RecordBase.get(CodeSegment.catchCode::offset, base);
        }
        public static void set(@LONG_POINTER int base, @CARD16 int newValue) {
            RecordBase.set(CodeSegment.catchCode::offset, base, newValue);
        }
    }
}
