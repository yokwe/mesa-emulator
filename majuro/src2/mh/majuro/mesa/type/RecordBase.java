package mh.majuro.mesa.type;

import mh.majuro.mesa.Memory;
import mh.majuro.mesa.Type.CARD16;
import mh.majuro.mesa.Type.CARD32;
import mh.majuro.mesa.Type.LONG_POINTER;

public class RecordBase {
	public interface ToIntBiIntFunction extends Memory.ToIntBiIntFunction {
		//
	}
	public interface ToIntIntFunction {
		int apply(int value);
	}

	public static final @CARD16 int get(ToIntIntFunction addressFunc, @LONG_POINTER int base) {
		final int address = addressFunc.apply(base);
		return Memory.fetch(address);
	}
	public static final void set(ToIntIntFunction addressFunc, @LONG_POINTER int base, @CARD16 int newValue) {
		final int address = addressFunc.apply(base);
		Memory.store(address, newValue);
	}
	
	public static final @CARD32 int getDbl(ToIntIntFunction addressFunc, @LONG_POINTER int base) {
		final int address = addressFunc.apply(base);
		return Memory.readDbl(address);
	}
	public static final void setDbl(ToIntIntFunction addressFunc, @LONG_POINTER int base, @CARD32 int newValue) {
		final int address = addressFunc.apply(base);
		Memory.writeDbl(address, newValue);
	}

	public static final @CARD16 int getBitField(ToIntIntFunction addressFunc, ToIntIntFunction getValueFunc, @LONG_POINTER int base) {
		final int address = addressFunc.apply(base);
		return getValueFunc.apply(Memory.fetch(address));
	}
	public static final void setBitField(ToIntIntFunction addressFunc, ToIntBiIntFunction setValueFunc, @LONG_POINTER int base, @CARD16 int newValue) {
		final int address = addressFunc.apply(base);
		Memory.modify(address, setValueFunc, newValue);
	}
	
	public static final @CARD32 int getBitFieldDbl(ToIntIntFunction addressFunc, ToIntIntFunction getValueFunc, @LONG_POINTER int base) {
		final int address = addressFunc.apply(base);
		return getValueFunc.apply(Memory.readDbl(address));
	}
	public static final void setBitFieldDbl(ToIntIntFunction addressFunc, ToIntBiIntFunction setValueFunc, @LONG_POINTER int base, @CARD32 int newValue) {
		final int address = addressFunc.apply(base);
		Memory.modifyDbl(address, setValueFunc, newValue);
	}
}
