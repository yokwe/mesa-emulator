/*
Copyright (c) 2020, Yasuhiro Hasegawa
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
  3. The name of the author may not be used to endorse or promote products derived
     from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/


//
// disk.h
//

#ifndef DISK_H__
#define DISK_H__

#include "../util/Util.h"
#include "../mesa/MesaBasic.h"

#include "../util/ByteBuffer.h"

#include <QtCore>

// To prevent conflict class defined in Pilot.h, declare class in disk namespace.
namespace disk {

class Disk {
public:
	Disk(const char* path);
	~Disk();

	void readSector(int sector, ByteBuffer& bb, quint32 count);
	void readSector(int sector, ByteBuffer& bb);

private:
	DiskFile   diskFile;
};


//
// From APilot/15.0.1/Pilot/Private/PilotFileTypes.mesa
//
enum class PilotFileType : CARD16 {
	tUnasstPhysicalVolumeRootPageigned =  1,
	tBadPageList                       =  2,
	tBadPage                           =  3,
	tSubVolumeMarkerPage               =  4,
	tLogicalVolumeRootPage             =  5,
	tFreePage                          =  6,
	tVolumeAllocationMap               =  7,
	tVolumeFileMap                     =  8,
	tScavengerLog                      =  9,
	tTempFileList                      = 10,
	tTransactionStateFile              = 11,
	tVMBackingFile                     = 12,
	tBeingMoved                        = 13,
	tBeingReplicated                   = 14,
	tAnonymousFile                     = 15,
	tTransactionLog                    = 16,
	tScavengerLogOtherVolume           = 17,
	tRootDirectory                     = 18,
};
QString toQString(PilotFileType value);
const char* toString(PilotFileType value) {
	return toQString(value).toLocal8Bit().constData();
}

class PilotFileTypes {
public:
//		  PilotFileType: TYPE = CARDINAL [0..256);
//		  tUnassigned: File.Type = [PilotFileType[0]];
	static const CARD16 tUnassigned = 0;

//		  -- Beginning of volume file types (which are not root file types):
//		  PilotVFileType: TYPE = PilotFileType [1..8];
//		  tPhysicalVolumeRootPage: File.Type = [PilotVFileType[1]];
//		  tBadPageList: File.Type = [PilotVFileType[2]];
//		  tBadPage: File.Type = [PilotVFileType[3]];
//		  tSubVolumeMarkerPage: File.Type = [PilotVFileType[4]];
//		  tLogicalVolumeRootPage: File.Type = [PilotVFileType[5]];
	static const CARD16 tUnasstPhysicalVolumeRootPageigned = 1;
	static const CARD16 tBadPageList                       = 2;
	static const CARD16 tBadPage                           = 3;
	static const CARD16 tSubVolumeMarkerPage               = 4;
	static const CARD16 tLogicalVolumeRootPage             = 5;


//		  -- Beginning of types which are both volume files and root files :
//		  PilotRootFileType: TYPE = PilotFileType [6..14];
//		  tFreePage: File.Type = [PilotRootFileType[6]];
//		  tVolumeAllocationMap: File.Type = [PilotRootFileType[7]];
//		  tVolumeFileMap: File.Type = [PilotRootFileType[8]];
	static const CARD16 tFreePage            = 6;
	static const CARD16 tVolumeAllocationMap = 7;
	static const CARD16 tVolumeFileMap       = 8;

//		  -- Beginning of root file types (which are not volume file types):
//		  tScavengerLog: File.Type = [PilotRootFileType[9]];
//		  -- scavenger log describing its own volume
//		  tTempFileList: File.Type = [PilotRootFileType[10]];
//		  tTransactionStateFile: File.Type = [PilotRootFileType[11]];
//		  tVMBackingFile: File.Type = [PilotRootFileType[12]];
//		  tBeingMoved: File.Type = [PilotRootFileType[13]];
//		  tBeingReplicated: File.Type = [PilotRootFileType[14]];
//		  -- (Additional root file types would go here.)
	static const CARD16 tScavengerLog         =  9;
	static const CARD16 tTempFileList         = 10;
	static const CARD16 tTransactionStateFile = 11;
	static const CARD16 tVMBackingFile        = 12;
	static const CARD16 tBeingMoved           = 13;
	static const CARD16 tBeingReplicated      = 14;

//		  -- Beginning of types which are neither volume files nor root files :
//		  tAnonymousFile: File.Type = [PilotFileType[15]];
//		  tTransactionLog: File.Type = [PilotFileType[16]];
//		  tScavengerLogOtherVolume: File.Type = [PilotFileType[17]];
//		  -- scavenger log describing other than its own volume
//		  tRootDirectory: File.Type = [PilotFileType[18]];
	static const CARD16 tAnonymousFile           = 15;
	static const CARD16 tTransactionLog          = 16;
	static const CARD16 tScavengerLogOtherVolume = 17;
	static const CARD16 tRootDirectory           = 18;

	static QString toQString(CARD16 value);

	CARD16 value;

	PilotFileTypes(CARD16 value_) : value(value_) {}

	void    read(ByteBuffer& bb);
	QString toQString() {
		return PilotFileTypes::toQString(value);
	}
	const char* toString() {
		return toQString().toLocal8Bit().constData();
	}
};



//
// From APilot/15.3/Faces/Friends/HostNumbers.mesa
//
//-- Host numbers
//
//HostNumber: TYPE = MACHINE DEPENDENT RECORD [
//  a, b, c, d, e, f: [0..256)];
//nullHostNumber: HostNumber = [0, 0, 0, 0, 0, 0];
//  -- Not assigned as either a physical or multicast address.
//
//
//-- Processor identifiers (physical addresses)
//
//ProcessorID: TYPE = RECORD [HostNumber];
//
//-- A processor identifier p MUST have p.a even:
//IsProcessorID: PROC [hn: LONG POINTER TO HostNumber] RETURNS[BOOLEAN] =
//  INLINE {RETURN[hn.a MOD 2=0]};
//
//nullProcessorID: ProcessorID = [nullHostNumber];

class ProcessorID {
public:
	CARD8 a, b, c, d, e, f;

	void    read(ByteBuffer& bb);
	bool    isNull();
	QString toQString();
	const char* toString() {
		return toQString().toLocal8Bit().constData();
	}
};

//
// From APilot/15.0.1/Pilot/Private/SystemInternal.mesa
//
//UniversalID: TYPE = MACHINE DEPENDENT RECORD [
//  processor(0): SpecialSystem.ProcessorID, sequence(3): LONG CARDINAL];
//-- Implementation of the exported type in System.
//-- NOTE: The fields of a UniversalID should NOT be used as hints as
//-- to the location or age of the associated entity (e.g. file, volume).
//
//nullID: UniversalID = [SpecialSystem.nullProcessorID, 0];
//   -- assert = LOOPHOLE[System.nullID]

class UniversalID {
public:
	ProcessorID processor;
	CARD32      sequence;

	void    read(ByteBuffer& bb);
	bool    isNull();
	QString toQString();
	const char* toString() {
		return toQString().toLocal8Bit().constData();
	}
};

//
// From APilot/15.0.1/Pilot/Friends/PilotDisk.mesa
//
//FileID: TYPE = MACHINE DEPENDENT RECORD [
//  name(0): SELECT OVERLAID * FROM
//    volumeRelative => [
//      fileID(0): File.ID, pad(2): ARRAY [0..3) OF WORD � ALL[0]],
//    unique => [id(0): System.UniversalID]
//    ENDCASE];
class FileID {
public:
	CARD32 fileID;
	CARD16 pad0, pad1, pad2;
	//
	UniversalID id;

	void    read(ByteBuffer& bb);
	bool    isNull();
	QString toQString();
	const char* toString() {
		return toQString().toLocal8Bit().constData();
	}
};

//
// From APilot/15.3/Faces/Friends/PilotDiskFace.mesa
//
//DiskAddress: TYPE = MACHINE DEPENDENT RECORD [
//  cylinder(0): CARDINAL, head(1:0..7): [0..256), sector(1:8..15): [0..256)];
class DiskAddress {
public:
	CARD16 cylinder;
	union {
		struct {
			CARD16 sector : 8;
			CARD16 head   : 8;
		};
		CARD16 u1;
	};

	void    read(ByteBuffer& bb);
	bool    isNull();
	QString toQString();
	const char* toString() {
		return toQString().toLocal8Bit().constData();
	}
};

//
// From APilot/15.0.1/Pilot/Friends/Boot.mesa
//
//DiskFileID: TYPE = MACHINE DEPENDENT RECORD [
//  fID (0): PilotDisk.FileID,
//  firstPage (5): PilotDisk.FilePageNumber,
//  da (7): PilotDiskFace.DiskAddress];
class DiskFileID {
public:
	FileID      fID;
	CARD32      firstPage;
	DiskAddress da;


	void    read(ByteBuffer& bb);
	bool    isNull();
	QString toQString();
	const char* toString() {
		return toQString().toLocal8Bit().constData();
	}
};
//-- Types of boot files pointed to from root pages of physical and logical
//-- volumes, and delivered by boot servers:
//-- The following cannot be changed without invalidating all Pilot volumes.
//-- Known to the initial microcode.
//BootFileType: TYPE = MACHINE DEPENDENT{
//  hardMicrocode (0), softMicrocode (1), germ (2), pilot (3), debugger (4),
//  debuggee (5), initialMicrocode (6)};
//PVBootFiles: TYPE = ARRAY BootFileType [hardMicrocode..pilot] OF DiskFileID;
//LVBootFiles: TYPE = ARRAY BootFileType [hardMicrocode..debuggee] OF DiskFileID;
class PVBootFiles {
public:
	DiskFileID hardMicrocode;
	DiskFileID softMicrocode;
	DiskFileID germ;
	DiskFileID pilot;

	void    read(ByteBuffer& bb);
	QString toQString();
	const char* toString() {
		return toQString().toLocal8Bit().constData();
	}
};
class LVBootFiles {
public:
	DiskFileID hardMicrocode;
	DiskFileID softMicrocode;
	DiskFileID germ;
	DiskFileID pilot;
	DiskFileID debugger;
	DiskFileID debuggee;

	void    read(ByteBuffer& bb);
	QString toQString();
	const char* toString() {
		return toQString().toLocal8Bit().constData();
	}
};

//
// APilot/15.0.1/Pilot/Private/PhysicalVolumeFormat.mesa
//
class PhysicalVolume {
public:
	//  seal, Seal: CARDINAL = 121212B;
	static const CARD16 seal = 0121212;

	//  currentVersion: CARDINAL = 7;
	static const CARD16 currentVersion = 7;

	//  maxSubVols: CARDINAL [10..10] = 10;
	static const CARD16 maxSubVols = 10;

	//  physicalVolumeLabelLength: CARDINAL = 40;
	//
	//  LabelArray: TYPE = PACKED ARRAY [0..physicalVolumeLabelLength) OF CHARACTER;
	static const CARD16 physicalVolumeLabelLength = 40;

	//  -- SubVolumeDesc as stored on disk in physical volume root page and in
	//  -- marker pages.
	//  SubVolumeDesc: TYPE = MACHINE DEPENDENT RECORD [
	//    lvID(0): System.VolumeID,
	//    lvSize(5): PageCount,
	//    lvPage(7): PageNumber,
	//    pvPage(11B): PageNumber,
	//    nPages(13B): PageCount];
	class SubVolumeDesc {
	public:
		UniversalID lvID;
		CARD32      lvSize;
		CARD32      lvPage;
		CARD32      pvPage;
		CARD32      nPages;

		void    read(ByteBuffer& bb);
		QString toQString();
		const char* toString() {
			return toQString().toLocal8Bit().constData();
		}
	};

	//  Descriptor: TYPE = MACHINE DEPENDENT RECORD [
	//    -- the following information is relatively static
	//    seal(0): CARDINAL � Seal,  -- must be 1st field
	//    version(1): CARDINAL � currentVersion,  -- must be 2nd field
	//    labelLength(2): CARDINAL [0..physicalVolumeLabelLength] � 0,
	//    pvID(3): System.PhysicalVolumeID,
	//    bootingInfo(10B): Boot.PVBootFiles � nullPVBootFiles,
	//    -- must be at this offset as the microcode knows where to find it.
	//    label(54B): LabelArray � nullLabel | NULL,
	//    subVolumeCount(100B): CARDINAL [0..maxSubVols],
	//    subVolumeMarkerID(101B): MarkerID � [System.nullID],
	//    badPageCount(106B): PageCount � 0,
	//    maxBadPages(110B): PageCount � maxNumberBadPages,
	//    dataLostPageCount(112B): PageCount � 0,
	//    maxDataLostPages(114B): PageCount � maxNumberDataLostPages,
	//    onLineCount(116B): CARDINAL � 0,  -- TEMPORARILY unused.
	//    subVolumes(117B): ARRAY [0..maxSubVols) OF SubVolumeDesc,
	//    fill1(321B): ARRAY [0..374B - 321B) OF WORD � ALL[0],  -- fill to whole page
	//    localTimeParametersValid(374B:0..15): BOOLEAN � FALSE,
	//    localTimeParameters(375B): System.LocalTimeParameters � LOOPHOLE[LONG[0]],
	//    checksum(377B): CARDINAL � 0,  -- MUST be the last field of this page
	//
	//    -- followed, on immediately following pages, by a BadPageList with maxBadPages entries
	//
	//    badPageList(400B): BadPageListArray];
	class Descriptor {
	public:
		CARD16        seal;
		CARD16        version;
		CARD16        labelLength;
		UniversalID   pvID;
		PVBootFiles   bootingInfo;
		CARD8         label[physicalVolumeLabelLength];
		CARD16        subVolumeCount;
		UniversalID   subVolumeMarkerID;
		CARD32        badPageCount;
		CARD32        maxBadPages;
		CARD32        dataLostPageCount;
		CARD32        maxDataLostPages;
		CARD16        onLineCount;
		SubVolumeDesc subVolumes[maxSubVols];
		CARD16        fill[43];
		CARD16        localTimeParametersValid;
		CARD32        localTimeParameters;
		CARD16        checksum;

		void read(ByteBuffer& bb);
		void dump();
	};
};

//
//
//
class Volume {
public:
//	  Type: TYPE = MACHINE DEPENDENT{
//	    normal(0), debugger(1), debuggerDebugger(2), nonPilot(3)};
	class Type {
	public:
		static const CARD16 normal           = 0;
		static const CARD16 debugger         = 1;
		static const CARD16 debuggerDebugger = 2;
		static const CARD16 nonPilot         = 3;

		CARD16 value;

		void    read(ByteBuffer& bb);
		QString toQString();
		const char* toString() {
			return toQString().toLocal8Bit().constData();
		}
	};
};
//
// APilot/15.0.1/Pilot/Private/LogicalVolumeFormat.mesa
//
class LogicalVolume {
public:
	//  maxLogicalVolumeLabelLength: CARDINAL = 40;  -- DO NOT EVER CHANGE THIS VALUE!
	static const CARD16 maxLogicalVolumeLabelLength = 40;

	//  lvRootSeal: CARDINAL = CARDINAL[131313B];  -- DO NOT EVER CHANGE THIS VALUE!
	static const CARD16 lvRootSeal = 0131313;

	//  currentVersion: CARDINAL = 9;
	static const CARD16 currentVersion = 9;

//	  LockState: TYPE = MACHINE DEPENDENT {unlocked(0), write(1), read(2), spare(3)};
//	  VolumeLock: TYPE = MACHINE DEPENDENT RECORD [
//	    owner (0): Volume.ID � Volume.nullID,
//	    lock (5:0..15): SELECT stateTag (5:0..1): LockState FROM
//	      unlocked => [null (5:2..15): [0..16384) � 0],
//	      write => [null (5:2..15): [0..16384) � 0],
//	      read => [count (5:2..5): [0..16) � 0, null (5:6..15): [0..1024) � 0],
//	    ENDCASE];
	class VolumeLock {
	public:
		UniversalID owner;
		CARD16      lock;

		void    read(ByteBuffer& bb);
		QString toQString();
		const char* toString() {
			return toQString().toLocal8Bit().constData();
		}
	};


//	class RootFileID {
//	public:
//		CARD32
//	};


//	  Descriptor: TYPE = MACHINE DEPENDENT RECORD [
//
//	    --========    Header:    ========
//	    -- DO NOT CHANGE THE FORMAT OR MEANING OF THESE FIELDS!
//	    seal(0):          CARDINAL  -- absolutely must be 1st field
//	      -- First word of a valid logical volume root page.
//	      -- If Descriptor.seal # lvRootSeal, the Descriptor is TRASH.
//	      � lvRootSeal,
//	    version(1):       CARDINAL  -- absolutely must be 2nd field
//	      -- Indicates the format and meaning of the Body part of a Descriptor.
//	      � currentVersion,
//	    vID(2):           Volume.ID,
//	    labelLength(7):   CARDINAL [0..maxLogicalVolumeLabelLength] � 0,
//	    label(10B):       LabelArray � nullName,
//	    type(34B):        Volume.Type,
//	    volumeSize(35B):  Volume.PageCount,
//	    bootingInfo(37B): Boot.LVBootFiles � nullBoot,
//	    pad(125B):	      CARDINAL � 0,
//	    pad1(126B:0..14):  [0..77777B] � 0,
//	    changing(126B:15..15): BOOLEAN � TRUE,  -- see documentation for semantics.
//
//	    --========    Body:    ========
//	    -- The format and/or meaning of these fields may change
//	    -- in different versions of Pilot.
//	    freePageCount(127B):       Volume.PageCount � 0,
//	    vamStart(131B):            Volume.PageNumber � 1,
//	    primaryETableStartPage(133B): Volume.PageNumber � 0,
//	    copyETableStartPage(135B): Volume.PageNumber � 0,
//	    lowerBound(137B):          Volume.PageNumber
//	       -- maintained by allocater; first page looked at by allocator
//	      � 2,
//	    volumeRootDirectory(141B): File.ID � File.nullID,
//	    rootFileID(143B):          ARRAY PilotFileTypes.PilotRootFileType OF File.ID
//	      � nullRootFileIDs,
//	    lastIDAllocated(165B):     LONG CARDINAL
//	      -- Highest numbered File.ID given out on this volume.
//	      -- We reserve the first set of IDs for Pilot's own use.
//	      -- In particular, files of type IN PilotRootFileType may have their ID
//	      --   the same as their File.Type.
//	      � LAST[FileTypes.PilotFileType],
//	    scavengerLogVolume(167B):  Volume.ID
//	      -- ID of volume containing the Scavenger log for THIS volume
//	      � Volume.nullID,
//	    lastTimeOpendForWrite(174B):         System.GreenwichMeanTime �
//	      System.gmtEpoch,
//	    statusPrimaryETableFile(176B:0..7):  [0..400B) � 0,
//	    statusCopyETableFile(176B:8..15):    [0..400B) � 0,
//	    numberOfFile(177B):        LONG CARDINAL � 0,
//	    lock(201B): VolumeLock,
//	    numberOfBadPagesLastTime(207B): Volume.PageCount � 0,
//	    fill(211B):                ARRAY [0..377B - 211B) OF WORD � ALL[0],
//	      -- fill to whole page
//	    checksum(377B):            CARDINAL � 0];  -- MUST be the last field

	class Descriptor {
	public:
		CARD16        seal;
		CARD16        version;
		UniversalID   vID;
		CARD16        labelLength;
		CARD8         label[maxLogicalVolumeLabelLength];
		Volume::Type  type; // Volume.Type
		CARD32        volumeSize; // Volume.PageCount
		LVBootFiles   bootingInfo;
		CARD16        pad;
		CARD16        changing; // changing(126B:15..15): BOOLEAN � TRUE,  -- see documentation for semantics.

		CARD32        freePageCount;
		CARD32        vamStart;
		CARD32        primaryETableStartPage;
		CARD32        copyETableStartPage;
		CARD32        lowerBound;
		CARD32        volumeRootDirectory;
		CARD32        nootFileID[9]; // PilotRootFileType: TYPE = PilotFileType [6..14];
		CARD32        lastIDAllocated;
		UniversalID   scavengerLogVolume;
		CARD32        lastTimeOpendForWrite;
		//	    statusPrimaryETableFile(176B:0..7):  [0..400B) � 0,
		//	    statusCopyETableFile(176B:8..15):    [0..400B) � 0,
		CARD16        statusPrimaryETableFile;
		CARD16        statusCopyETableFile;
		CARD32        numberOfFile;
		VolumeLock    lock;
		CARD32        numberOfBadPagesLastTime;
		//	    fill(211B):                ARRAY [0..377B - 211B) OF WORD � ALL[0],
		CARD16        fill[0377 - 0211];
		CARD16        checksum;

		void read(ByteBuffer& bb);
		void dump();
	};

};


}; // end of namesapce

#endif

