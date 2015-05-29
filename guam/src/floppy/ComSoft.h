/*
Copyright (c) 2014, Yasuhiro Hasegawa
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
// ComSoft.h
//

#ifndef COMSOFT_H__
#define COMSOFT_H__ 1

#include "../mesa/MesaBasic.h"
#include "../mesa/Pilot.h"


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// APilot/15.0.1/ComSoft/Public/Floppy.mesa
//
class Floppy {
public:

	//	-- Volume references
	//
	//VolumeHandle: TYPE [2];
	//nullVolumeHandle: READONLY VolumeHandle;
	struct VolumeHandle {
		CARD32 value;
	};

	//	-- File identifiers
	//
	//FileID: TYPE  = PRIVATE MACHINE DEPENDENT RECORD [a (0), b (1): WORD];
	//nullFileID: FileID = [0, 0];
	struct FileID {
		CARD16 a;
		CARD16 b;
	};
	//FileHandle: TYPE = RECORD [volume: VolumeHandle, file: FileID];
	struct FileHandle {
		VolumeHandle volume;
		FileID       file;
	};


};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// APilot/15.0.1/ComSoft/Private/FloppyFormat.mesa
//
class FloppyFormat {
public:

//	AsciiToEbcdic: ARRAY CHARACTER [' ..'Z] OF Byte = [
//	    -- !"#$%&'--  64, 126,  90, 123,  91, 108, 124, 125,
//	    --()*+,-./--  77,  93,  92,  78, 107,  96,  75,  97,
//	    --01234567-- 240, 241, 242, 243, 244, 245, 246, 247,
//	    --89:;<=>?-- 248, 249, 122,  94,  76, 126, 110, 111,
//	    --@ABCDEFG-- 124, 193, 194, 195, 196, 197, 198, 199,
//	    --HIJKLMNO-- 200, 201, 209, 210, 211, 212, 213, 214,
//	    --PQRSTUVW-- 215, 216, 217, 226, 227, 228, 229, 230,
//	    --XYZ     -- 231, 232, 233];
	static CARD8 AsciiToEbcdic[59]; // ' ' .. 'Z'

//	SectorToDiskAddress: PROCEDURE [
//	   sector: Sector, cylinders: CARDINAL, tracksPerCylinder: CARDINAL,
//	   sectorsPerTrack: CARDINAL]
//	   RETURNS [diskAddress: FloppyChannel.DiskAddress] = INLINE
//	   BEGIN
//	   temp: CARDINAL;
//	   -- Remember that disk sectors are numbered [1..sectors per track] and that
//	   -- Sector 0 is unused while Sector 1 is address [sector: 1, head: 0, cyl: 0]
//	   [quotient: temp, remainder: diskAddress.sector] �
//	     Inline.DIVMOD[sector-1, sectorsPerTrack];
//	   diskAddress.sector � diskAddress.sector+1; -- sectors begin at 1
//	   [quotient: diskAddress.cylinder, remainder: diskAddress.head] �
//	      Inline.DIVMOD[temp, tracksPerCylinder];
//	   END;
	FloppyDiskFace::DiskAddress SectorToDiskAddress(CARD32 sector, CARD32 cylinders, CARD32 tracksPerCylinder, CARD32 sectorsPerTrack) {
		FloppyDiskFace::DiskAddress ret;
		CARD32 tracks  = (sector - 1) / sectorsPerTrack;
		ret.sector   = (sector - 1) % sectorsPerTrack;
		ret.cylinder = tracks / tracksPerCylinder;
		ret.head     = tracks % tracksPerCylinder;
		return ret;
	}

//	DiskAddressToSector: PROCEDURE [
//	   diskAddress: FloppyChannel.DiskAddress, cylinders: CARDINAL,
//	   tracksPerCylinder: CARDINAL, sectorsPerTrack: CARDINAL]
//	   RETURNS [sector: Sector] = INLINE
//	   {RETURN[
//	      diskAddress.sector + sectorsPerTrack*
//		(diskAddress.head + tracksPerCylinder * diskAddress.cylinder)]};
	CARD32 DiskAddressToSector(FloppyDiskFace::DiskAddress diskAddress, CARD32 cylinders, CARD32 tracksPerCylinder, CARD32 sectorsPerTrack) {
		return diskAddress.sector + sectorsPerTrack * (diskAddress.head + tracksPerCylinder * diskAddress.cylinder);
	}

//	FloppySeal: CARDINAL = 141414B; -- Must never change!
	static const CARD16 FloppySeal = 0141414;

//	FloppyVersion: CARDINAL = 1;
//	   -- Must be incremented every time the format of track zero or track one changes
	static const CARD16 FloppyVersion = 1;


//	SectorNine: TYPE = MACHINE DEPENDENT RECORD [
//
//	   -- This is the root of the floppy file system.  It must be reconstructable from
//	   -- information stored redundantly on the floppy in the marker pages.  At some
//	   -- point it may be desirable to have a backup copy of (some) of this
//	   -- information in some other part of the disk.
//	   -- This sector deviates from IBM standard(s).
//
//	   seal (0): CARDINAL � FloppySeal,
//	   version (1): CARDINAL � FloppyVersion,
//
//	   cylinders (2): CARDINAL,
//	   tracksPerCylinder (3): CARDINAL,
//	   sectorsPerTrack (4): CARDINAL,
//
//	   fileList (5): Sector � nullSector,
//	   fileListID (6): Floppy.FileID � Floppy.nullFileID,
//	   fileListSize (8): ImplementedFileSize � 0,
//
//	   rootFile (9): Floppy.FileID � Floppy.nullFileID,
//
//	   -- The initial sector is specified for all boot files
//
//	   alternateMicrocode(11): Sector � nullSector,
//	   pilotMicrocode (12): Sector � nullSector,
//	   diagnosticMicrocode (13): Sector � nullSector,
//	   germ (14): Sector � nullSector,
//	   pilotBootFile (15): Sector � nullSector,
//
//	   firstAlternateSector (16): Sector,
//	   countBadSectors (17): CARDINAL � 0,
//
//	   nextUnusedFileID (18): LONG CARDINAL � 1,
//
//	   changing (20: 0..0): BOOLEAN � FALSE,
//	   pad (20: 1..15): CARDINAL [0..77777B) � 0,
//
//	   labelSize (21) : CARDINAL � 0,
//	   label (22): PACKED ARRAY [0..Floppy.maxCharactersInLabel) OF CHARACTER �
//	      ALL[' ],
//
//	   sectorOnePad (42): ARRAY [0..64-42) OF WORD � ALL[0]
//	      -- MUST be zero!  Reserved for future expansion.
//	   ];

};




