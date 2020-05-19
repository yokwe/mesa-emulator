package mh.majuro.mesa;

import mh.majuro.mesa.Type.LONG_POINTER;

public final class MapFlags {
	//MapFlags: TYPE = MACHINE DEPENDENT RECORD (
	//  reserved (0:0..12) : UNSPEClFIED[0..17777B],
	//  protected (0:13..13) : BOOLEAN,
	//  dirty (0:14..14): BOOLEAN,
	//  referenced (0:15..15): BOOLEAN];
	public static final short CLEAR      = 0;
	public static final short REFERENCED = 0x0001;
	public static final short DIRYT      = 0x0002;
	public static final short PROTECT    = 0x0004;
	public static final short VACANT     = DIRYT | PROTECT;
	public static final short REFERENCED_AND_DIRTY = REFERENCED | DIRYT;
	
	public int rmPage;
	public int flag;
	
	public MapFlags() {
		rmPage = 0;
		flag   = VACANT;
	}
	public void set(int rmPage, int flag) {
		this.rmPage = rmPage;
		this.flag   = flag;
	}
	
	public boolean isVacant() {
		return flag == VACANT;
	}
	private boolean isProtect() {
		return (flag & PROTECT) != 0;
	}

	public void checkFetch(@LONG_POINTER int va) {
		// check vacant
		if (isVacant()) {
			Processes.pageFault(va);
		}
	}
	public void updateFlagFetch() {
		// set referenced flag
		flag = (~REFERENCED & flag) | REFERENCED;
	}
	
	public void checkStore(@LONG_POINTER int va) {
		// check vacant
		if (isVacant()) {
			Processes.pageFault(va);
		}
		// check protect
		if (isProtect()) {
			Processes.writeProtectFault(va);
		}
	}
	public boolean isReferencedDirty() {
		return (flag & REFERENCED_AND_DIRTY) == REFERENCED_AND_DIRTY;
	}
	public void updateFlagStore() {
		// set referenced and dirty flag
		flag = (~REFERENCED_AND_DIRTY & flag) | REFERENCED_AND_DIRTY;
	}
}