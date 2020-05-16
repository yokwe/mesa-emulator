package mh.majuro.mesa.type;

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
	
	public void checkFetch(@LONG_POINTER int va) {
		// check vacant
		if (flag == VACANT) {
			// FIXME PageFault
		}
	}
	public boolean isReferenced() {
		return (flag & REFERENCED) == REFERENCED;
	}
	public void updateFlagFetch() {
		flag = (~REFERENCED & flag) | REFERENCED;
	}
	public boolean isVacant() {
		return flag == VACANT;
	}
	
	public void checkStore(@LONG_POINTER int va) {
		// check vacant
		if (flag == VACANT) {
			// FIXME PageFault
		}
		// check protect
		if ((flag & PROTECT) != 0) {
			// FIXME WriteProtectFault
		}
	}
	public boolean isReferencedDirty() {
		return (flag & REFERENCED_AND_DIRTY) == REFERENCED_AND_DIRTY;
	}
	public void updateFlagStore() {
		flag = (~REFERENCED_AND_DIRTY & flag) | REFERENCED_AND_DIRTY;
	}
}