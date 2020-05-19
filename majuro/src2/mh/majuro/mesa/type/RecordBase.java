package mh.majuro.mesa.type;

import mh.majuro.mesa.Memory;
import mh.majuro.mesa.Type.CARD16;
import mh.majuro.mesa.Type.LONG_POINTER;

public class RecordBase {
	public interface ToIntBiIntFunction extends Memory.ToIntBiIntFunction {
		//
	}
	public interface ToIntIntFunction {
		int apply(int value);
	}

	public static final int get(ToIntIntFunction addressFunc, @LONG_POINTER int base) {
		final int address = addressFunc.apply(base);
		return Memory.fetch(address);
	}
	public static final void set(ToIntIntFunction addressFunc, @LONG_POINTER int base, @CARD16 int newValue) {
		final int address = addressFunc.apply(base);
		Memory.store(address, newValue);
	}
	
	public static final int getBitField(ToIntIntFunction addressFunc, ToIntIntFunction getValueFunc, @LONG_POINTER int base) {
		final int address = addressFunc.apply(base);
		return getValueFunc.apply(Memory.fetch(address));
	}
	public static final void setBitField(ToIntIntFunction addressFunc, ToIntBiIntFunction setValueFunc, @LONG_POINTER int base, @CARD16 int newValue) {
		final int address = addressFunc.apply(base);
		Memory.store(address, setValueFunc.apply(Memory.fetch(address), newValue));
	}
	
	// array
	public static final int arrayAddress(int base, int elementSize, int elementIndex) {
		return base + (elementSize * elementIndex);
	}
	
	public static final int getArray(ToIntIntFunction addressFunc, @LONG_POINTER int base, int elementSize, int elementIndex) {
		final int address = arrayAddress(addressFunc.apply(base), elementSize, elementIndex);
		return Memory.fetch(address);
	}
	public static final void setArray(ToIntIntFunction addressFunc, @LONG_POINTER int base, int elementSize, int elementIndex, @CARD16 int newValue) {
		final int address = arrayAddress(addressFunc.apply(base), elementSize, elementIndex);
		Memory.store(address, newValue);
	}

	public static int getBitFieldArray(ToIntIntFunction addressFunc, ToIntIntFunction getValueFunc, @LONG_POINTER int base, int elementSize, int elementIndex) {
		final int address = arrayAddress(addressFunc.apply(base), elementSize, elementIndex);
		return getValueFunc.apply(Memory.fetch(address));
	}
	public static void setBitFieldArray(ToIntIntFunction addressFunc, ToIntBiIntFunction setValueFunc, @LONG_POINTER int base, int elementSize, int elementIndex, @CARD16 int newValue) {
		final int address = arrayAddress(addressFunc.apply(base), elementSize, elementIndex);
		Memory.modify(address, setValueFunc, newValue);
	}
}