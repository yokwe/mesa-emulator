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


#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("testPilot");

#include "testBase.h"

#include "../mesa/Type.h"
#include "../mesa/Pilot.h"


class testPilot : public testBase {

	CPPUNIT_TEST_SUITE(testPilot);

	// ColorBlt
	CPPUNIT_TEST(testAddress_ColorBlt);
	CPPUNIT_TEST(testPatternParm_ColorBlt);
	CPPUNIT_TEST(testColorBltFlags);
	CPPUNIT_TEST(testColorMapping);
	CPPUNIT_TEST(testColorBltTable);

	// ProcessorFaceExtras
	CPPUNIT_TEST(testVersionResult);

	// VM
	CPPUNIT_TEST(testInterval_VM);

	// BackingStore
	CPPUNIT_TEST(testData_BackingStore);

	//VMDataInternal
	CPPUNIT_TEST(testRun);

	// TimeStamp
	CPPUNIT_TEST(testTimeStamp);

	// Ignore BcdDefs
	CPPUNIT_TEST(testNamee);
	CPPUNIT_TEST(testCTRecord);
	CPPUNIT_TEST(testFTRecord);
	CPPUNIT_TEST(testCodeDesc);
	CPPUNIT_TEST(testMTRecord);
	CPPUNIT_TEST(testENRecord);

	// System
	CPPUNIT_TEST(testSwitches);
	CPPUNIT_TEST(testUniversalID);
	CPPUNIT_TEST(testNetworkNumber);
	CPPUNIT_TEST(testNetworkNumber);
	CPPUNIT_TEST(testHostNumber);
	CPPUNIT_TEST(testSocketNumber);
	CPPUNIT_TEST(testNetworkAddress);

	// PilotDiskFace
	CPPUNIT_TEST(testDiskAddress);
	CPPUNIT_TEST(testOperation_PilotDiskFace);

	// PilotDisk
	CPPUNIT_TEST(testFileID);

	// Boot
	CPPUNIT_TEST(testDiskFileID);
	CPPUNIT_TEST(testEthernetRequest);
	CPPUNIT_TEST(testLocation);
	CPPUNIT_TEST(testRequest);
	CPPUNIT_TEST(testPVBootFiles);
	CPPUNIT_TEST(testLVBootFiles);

	// LogicalVolumeFormat
	CPPUNIT_TEST(testVolumeLock);
	CPPUNIT_TEST(testDescriptor_LogicalVolumeFormat);

	// PhysicalVolumeFormat
	CPPUNIT_TEST(testSubVolumeDesc);
	CPPUNIT_TEST(testDescriptor_PhysicalVolumeFormat);

	// GuamInputOutput
	CPPUNIT_TEST(testIORegionType);

	// BiParallelPortFace
	CPPUNIT_TEST(testOperation_BiParallelPortFace);

	// DisplayFace
	CPPUNIT_TEST(testCursor);

	// FloppyDiskFace
	CPPUNIT_TEST(testDiskAddress_Floppy);
	CPPUNIT_TEST(testOperation_Floppy);

	// RS232CEnvironment
	CPPUNIT_TEST(testFlowControl);

	// NewRS232CFace
	CPPUNIT_TEST(testParameterRecord);
	CPPUNIT_TEST(testOperation_NewRS232CFace);
	CPPUNIT_TEST(testControlRecord);
	CPPUNIT_TEST(testDeviceStatus);

	// BeepIOFaceGuam
	CPPUNIT_TEST(testBeepFCBType);

	// CoProcessorIOFaceGuam
	CPPUNIT_TEST(testCoProcessorFCBType);
	CPPUNIT_TEST(testTransferRec);
	CPPUNIT_TEST(testCoProcessorIOCBType);

	// DiskIOFaceGuam
	CPPUNIT_TEST(testDiskDCBType);
	CPPUNIT_TEST(testDiskFCBType);
	CPPUNIT_TEST(testDiskIOCBType);

	// DisplayIOFaceGuam
	CPPUNIT_TEST(testDisplayCoordinate);
	CPPUNIT_TEST(testDisplayRectangle);
	CPPUNIT_TEST(testLookupTableEntry);
	CPPUNIT_TEST(testDisplayFCBType);

	// EthernetIOFaceGuam
	CPPUNIT_TEST(testEthernetFCBType);
	CPPUNIT_TEST(testEthernetIOCBType);

	// FloppyIOFaceGuam
	CPPUNIT_TEST(testFloppyDCBType);
	CPPUNIT_TEST(testFloppyIOCBType);
	CPPUNIT_TEST(testFloppyFCBType);

	// KeyboardIOFaceGuam
	CPPUNIT_TEST(testKeyboardFCBType);

	// MouseIOFaceGuam
	CPPUNIT_TEST(testMousePositionType);
	CPPUNIT_TEST(testMouseFCBType);

	// ParallelIOFaceGuam
	CPPUNIT_TEST(testParallelFCBType);
	CPPUNIT_TEST(testParallelIOCBType);

	// ProcessorIOFaceGuam
	CPPUNIT_TEST(testProcessorFCBType);

	// SerialIOFaceGuam
	CPPUNIT_TEST(testSerialFaceFCBType);
	CPPUNIT_TEST(testSerialIOCBType);

	// TTYPortIOFAceGuam
	CPPUNIT_TEST(testTTYFCBType);

	// LoadStateFormat
	CPPUNIT_TEST(testModuleInfo);
	CPPUNIT_TEST(testBcdInfo);
	CPPUNIT_TEST(testObject);

	// CPSwapDefs
	CPPUNIT_TEST(testSwapInfo);
	CPPUNIT_TEST(testExternalStateVector);

	// PrincOpsExtras2
	CPPUNIT_TEST(testGFTItem);

	CPPUNIT_TEST_SUITE_END();


public:
	// BcdDefs
	void testNamee() {
		//Namee: TYPE = RECORD [
		//  SELECT type: * FROM
		//    config => [cti: CTIndex],
		//    module => [mti: MTIndex],
		//    import => [impi: IMPIndex],
		//    export => [expi: EXPIndex]
		//    ENDCASE];

		CPPUNIT_ASSERT_EQUAL((CARD32)2, SIZE(BcdDefs::Namee));
    	CPPUNIT_ASSERT_EQUAL((CARD32)0, OFFSET(BcdDefs::Namee, type));
       	CPPUNIT_ASSERT_EQUAL((CARD32)1, OFFSET(BcdDefs::Namee, u1));
	}
	void testCTRecord() {
		//CTRecord: TYPE = --MACHINE DEPENDENT-- RECORD [
		//  name: NameRecord,
		//  file: FTIndex,
		//  config: CTIndex,
		//  namedInstance: BOOLEAN,
		//  nControls: NATURAL,
		//  controls: ARRAY [0..0) OF Namee];  -- only config or module are valid
		CPPUNIT_ASSERT_EQUAL((CARD32)4, SIZE(BcdDefs::CTRecord));
    	CPPUNIT_ASSERT_EQUAL((CARD32)0, OFFSET(BcdDefs::CTRecord, name));
       	CPPUNIT_ASSERT_EQUAL((CARD32)1, OFFSET(BcdDefs::CTRecord, file));
       	CPPUNIT_ASSERT_EQUAL((CARD32)2, OFFSET(BcdDefs::CTRecord, config));
       	CPPUNIT_ASSERT_EQUAL((CARD32)3, OFFSET(BcdDefs::CTRecord, u3));
       	CPPUNIT_ASSERT_EQUAL((CARD32)4, OFFSET(BcdDefs::CTRecord, controls));

       	BcdDefs::CTRecord t;

       	t.u3 = 0;
       	t.namedInstance = ~t.namedInstance;
       	CPPUNIT_ASSERT_EQUAL((CARD16)0x8000, t.u3);

       	t.u3 = 0;
       	t.nControls = ~t.nControls;
       	CPPUNIT_ASSERT_EQUAL((CARD16)0x7FFF, t.u3);
	}
	void testFTRecord() {
		//FTRecord: TYPE = RECORD [
		//  name: NameRecord,
		//  version: VersionStamp];
		CPPUNIT_ASSERT_EQUAL((CARD32)4, SIZE(BcdDefs::FTRecord));
    	CPPUNIT_ASSERT_EQUAL((CARD32)0, OFFSET(BcdDefs::FTRecord, name));
    	CPPUNIT_ASSERT_EQUAL((CARD32)1, OFFSET(BcdDefs::FTRecord, version));
	}
	void testCodeDesc() {
		//CodeDesc: TYPE = RECORD [
		//  sgi: SGIndex,
		//  offset,
		//  length: CARDINAL];
		CPPUNIT_ASSERT_EQUAL((CARD32)3, SIZE(BcdDefs::CodeDesc));
    	CPPUNIT_ASSERT_EQUAL((CARD32)0, OFFSET(BcdDefs::CodeDesc, sgi));
    	CPPUNIT_ASSERT_EQUAL((CARD32)1, OFFSET(BcdDefs::CodeDesc, offset));
    	CPPUNIT_ASSERT_EQUAL((CARD32)2, OFFSET(BcdDefs::CodeDesc, length));
	}
	void testMTRecord() {
		//MTRecord: TYPE = --MACHINE DEPENDENT-- RECORD [
		//  name: NameRecord,
		//  file: FTIndex,
		//  config: CTIndex,
		//  code: CodeDesc,
		//  sseg: SGIndex,
		//  links: LFIndex,
		//  linkLoc: LinkLocation,
		//  namedInstance, initial: BOOLEAN,
		//  boundsChecks, nilChecks: BOOLEAN,
		//  tableCompiled, residentFrame, crossJumped, packageable: BOOLEAN,
		//  packed: BOOLEAN, linkspace: BOOLEAN,
		//  spare: PACKED ARRAY [0..4) OF BOOLEAN,
		//  framesize: [0..PrincOps.MaxFrameSize),
		//  entries: ENIndex,
		//  atoms: ATIndex];
		CPPUNIT_ASSERT_EQUAL((CARD32)12, SIZE(BcdDefs::MTRecord));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(BcdDefs::MTRecord, name));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 1, OFFSET(BcdDefs::MTRecord, file));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(BcdDefs::MTRecord, config));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 3, OFFSET(BcdDefs::MTRecord, code));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 6, OFFSET(BcdDefs::MTRecord, sseg));
       	CPPUNIT_ASSERT_EQUAL((CARD32) 7, OFFSET(BcdDefs::MTRecord, links));
       	CPPUNIT_ASSERT_EQUAL((CARD32) 8, OFFSET(BcdDefs::MTRecord, u6));
       	CPPUNIT_ASSERT_EQUAL((CARD32) 9, OFFSET(BcdDefs::MTRecord, framesize));
       	CPPUNIT_ASSERT_EQUAL((CARD32)10, OFFSET(BcdDefs::MTRecord, entries));
       	CPPUNIT_ASSERT_EQUAL((CARD32)11, OFFSET(BcdDefs::MTRecord, atoms));

       	BcdDefs::MTRecord t;

       	t.u6 = 0;
       	t.spare = ~t.spare;
       	CPPUNIT_ASSERT_EQUAL((CARD16)0x000f, t.u6);

       	t.u6 = 0;
       	t.linkspace = ~t.linkspace;
       	CPPUNIT_ASSERT_EQUAL((CARD16)0x0010, t.u6);

       	t.u6 = 0;
       	t.packed = ~t.packed;
       	CPPUNIT_ASSERT_EQUAL((CARD16)0x0020, t.u6);

       	t.u6 = 0;
       	t.packageable = ~t.packageable;
       	CPPUNIT_ASSERT_EQUAL((CARD16)0x0040, t.u6);

       	t.u6 = 0;
       	t.crossJumped = ~t.crossJumped;
       	CPPUNIT_ASSERT_EQUAL((CARD16)0x0080, t.u6);

       	t.u6 = 0;
       	t.residentFrame = ~t.residentFrame;
       	CPPUNIT_ASSERT_EQUAL((CARD16)0x0100, t.u6);

       	t.u6 = 0;
       	t.tableCompiled = ~t.tableCompiled;
       	CPPUNIT_ASSERT_EQUAL((CARD16)0x0200, t.u6);

       	t.u6 = 0;
       	t.nilChecks = ~t.nilChecks;
       	CPPUNIT_ASSERT_EQUAL((CARD16)0x0400, t.u6);

       	t.u6 = 0;
       	t.boundsChecks = ~t.boundsChecks;
       	CPPUNIT_ASSERT_EQUAL((CARD16)0x0800, t.u6);

       	t.u6 = 0;
       	t.initail = ~t.initail;
       	CPPUNIT_ASSERT_EQUAL((CARD16)0x1000, t.u6);

       	t.u6 = 0;
       	t.namedInstace = ~t.namedInstace;
       	CPPUNIT_ASSERT_EQUAL((CARD16)0x2000, t.u6);

       	t.u6 = 0;
       	t.linkLoc = ~t.linkLoc;
       	CPPUNIT_ASSERT_EQUAL((CARD16)0xc000, t.u6);
	}
	void testENRecord() {
		//ENRecord: TYPE = RECORD [
		//   nEntries: CARDINAL, initialPC: ARRAY [0..0) OF PrincOps.BytePC];
		CPPUNIT_ASSERT_EQUAL((CARD32)1, SIZE(BcdDefs::ENRecord));
      	CPPUNIT_ASSERT_EQUAL((CARD32)0, OFFSET(BcdDefs::ENRecord, nEntries));
      	CPPUNIT_ASSERT_EQUAL((CARD32)1, OFFSET(BcdDefs::ENRecord, ininialPC));
	}

	// TimeStamp
	void testTimeStamp() {
		//Stamp: TYPE = RECORD [net, host: [0..377B], time: LONG CARDINAL];

		CPPUNIT_ASSERT_EQUAL((CARD32)3, SIZE(TimeStamp::Stamp));
    	CPPUNIT_ASSERT_EQUAL((CARD32)0, OFFSET(TimeStamp::Stamp, u0));
       	CPPUNIT_ASSERT_EQUAL((CARD32)1, OFFSET(TimeStamp::Stamp, time));

       	TimeStamp::Stamp t;

    	t.u0 = 0;
       	t.net = ~t.net;
       	CPPUNIT_ASSERT_EQUAL((CARD16)0xFF00, t.u0);

    	t.u0 = 0;
       	t.host = ~t.host;
       	CPPUNIT_ASSERT_EQUAL((CARD16)0x00FF, t.u0);
	}

	// System
	void testSwitches() {
		//Switches: TYPE = PACKED ARRAY CHARACTER OF UpDown;
		//UpDown: TYPE = MACHINE DEPENDENT {up (0), down (1)};
		//defaultSwitches: Switches = ALL[up];

		CPPUNIT_ASSERT_EQUAL((CARD32)(256 / 16), SIZE(System::Switches));
	}
	void testUniversalID() {
		// UniversalID: TYPE [5];

		CPPUNIT_ASSERT_EQUAL((CARD32)5, SIZE(System::UniversalID));
	}
	void testNetworkNumber() {
		// NetworkNumber: TYPE[2];

		CPPUNIT_ASSERT_EQUAL((CARD32)2, SIZE(System::NetworkNumber));
	}
	void testHostNumber() {
		// HostNumber: TYPE[3];

		CPPUNIT_ASSERT_EQUAL((CARD32)3, SIZE(System::HostNumber));
	}
	void testSocketNumber() {
		// SocketNumber: TYPE[1];

		CPPUNIT_ASSERT_EQUAL((CARD32)1, SIZE(System::SocketNumber));
	}
	void testNetworkAddress() {
		//NetworkAddress: TYPE = MACHINE DEPENDENT RECORD [
		//   net: NetworkNumber,
		//   host: HostNumber,
		//   socket: SocketNumber];

		CPPUNIT_ASSERT_EQUAL((CARD32)6, SIZE(System::NetworkAddress));
    	CPPUNIT_ASSERT_EQUAL((CARD32)0, OFFSET(System::NetworkAddress, net));
    	CPPUNIT_ASSERT_EQUAL((CARD32)2, OFFSET(System::NetworkAddress, host));
    	CPPUNIT_ASSERT_EQUAL((CARD32)5, OFFSET(System::NetworkAddress, socket));
	}


	// PilotDiskFace
	void testDiskAddress() {
		//DiskAddress: TYPE = MACHINE DEPENDENT RECORD [
		//  cylinder(0): CARDINAL, head(1:0..7): [0..256), sector(1:8..15): [0..256)];
		CPPUNIT_ASSERT_EQUAL((CARD32)2, SIZE(PilotDiskFace::DiskAddress));
    	CPPUNIT_ASSERT_EQUAL((CARD32)0, OFFSET(PilotDiskFace::DiskAddress, cylinder));
       	CPPUNIT_ASSERT_EQUAL((CARD32)1, OFFSET(PilotDiskFace::DiskAddress, u1));

       	PilotDiskFace::DiskAddress t;

		t.cylinder = 0;
		t.head     = 0;
		t.sector   = 0;

		t.head     = ~t.head;
		CPPUNIT_ASSERT_EQUAL((CARD16)0xff00, t.u1);


		t.cylinder = 0;
		t.head     = 0;
		t.sector   = 0;

		t.sector   = ~t.sector;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x00ff, t.u1);
	}
	void testOperation_PilotDiskFace() {
		//Operation: TYPE = MACHINE DEPENDENT RECORD [
		//  clientHeader(0): DiskAddress,  -- address of first sector of request
		//  reserved1(2): LONG POINTER = NIL,
		//  -- reserved1 reserves space for backwards compatibility.  MUST BE NIL
		//  dataPtr(4): LONG POINTER,  -- first (page aligned) data address of operation
		//  incrementDataPtr(6:0..0): BOOLEAN,  -- if TRUE, add wordsPerPage to dataPtr
		//  -- after each successful data write
		//  enableTrackBuffer(6:1..1): BOOLEAN = FALSE,
		//  command(6:2..7): Command,  -- six bits for backward compatibility
		//  tries(6:8..15): Tries,
		//    -- Indication of how hard the Face should retry the operation in the presence
		//    -- of errors.  Increasing values of tries indicates that non-decreasing
		//    -- effort should be expended in retrying the failing operation.  Retries
		//    -- include device specific recovery (ECC, recalibrates, ...)
		//  pageCount(7): CARDINAL,  -- sectors remaining for this operation.
		//  deviceStatus(10B): RECORD [a, b: UNSPECIFIED] = NULL,
		//  diskHeader(12B): DiskAddress = NULL,  -- readHeader commands put header here.
		//  device(14B): DeviceHandle];

		CPPUNIT_ASSERT_EQUAL((CARD32)13, SIZE(PilotDiskFace::Operation));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(PilotDiskFace::Operation, clientHeader));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(PilotDiskFace::Operation, reserved1));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 4, OFFSET(PilotDiskFace::Operation, dataPtr));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 6, OFFSET(PilotDiskFace::Operation, u6));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 7, OFFSET(PilotDiskFace::Operation, pageCount));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 8, OFFSET(PilotDiskFace::Operation, deviceStatus));
    	CPPUNIT_ASSERT_EQUAL((CARD32)10, OFFSET(PilotDiskFace::Operation, diskHeader));
    	CPPUNIT_ASSERT_EQUAL((CARD32)12, OFFSET(PilotDiskFace::Operation, device));

    	PilotDiskFace::Operation t;

    	t.u6 = 0;
    	t.incrementDataPtr = ~t.incrementDataPtr;
    	CPPUNIT_ASSERT_EQUAL((CARD16)0x8000, t.u6);

    	t.u6 = 0;
    	t.enableTrackBuffer = ~t.enableTrackBuffer;
    	CPPUNIT_ASSERT_EQUAL((CARD16)0x4000, t.u6);

    	t.u6 = 0;
    	t.command = ~t.command;
    	CPPUNIT_ASSERT_EQUAL((CARD16)0x3f00, t.u6);

    	t.u6 = 0;
    	t.tries = ~t.tries;
    	CPPUNIT_ASSERT_EQUAL((CARD16)0x00ff, t.u6);
	}


	// PilotDisk
	void testFileID() {
		//FileID: TYPE = MACHINE DEPENDENT RECORD [
		//  name(0): SELECT OVERLAID * FROM
		//    volumeRelative => [
		//      fileID(0): File.ID, pad(2): ARRAY [0..3) OF WORD = ALL[0]],
		//    unique => [id(0): System.UniversalID]
		//    ENDCASE];

		CPPUNIT_ASSERT_EQUAL((CARD32) 5, SIZE(PilotDisk::FileID));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(PilotDisk::FileID, fileID));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(PilotDisk::FileID, pad[0]));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 3, OFFSET(PilotDisk::FileID, pad[1]));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 4, OFFSET(PilotDisk::FileID, pad[2]));
	}


	// Boot
	void testDiskFileID() {
		//DiskFileID: TYPE = MACHINE DEPENDENT RECORD [
		//  fID (0): PilotDisk.FileID,
		//  firstPage (5): PilotDisk.FilePageNumber,
		//  da (7): PilotDiskFace.DiskAddress];

		CPPUNIT_ASSERT_EQUAL((CARD32) 9, SIZE(Boot::DiskFileID));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(Boot::DiskFileID, fID));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 5, OFFSET(Boot::DiskFileID, firstPage));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 7, OFFSET(Boot::DiskFileID, da));
	}
	void testEthernetRequest() {
		//EthernetRequest: TYPE = MACHINE DEPENDENT RECORD [
		//  bfn(0): EthernetBootFileNumber, address(3): System.NetworkAddress];
		//
		//EthernetBootFileNumber: TYPE = RECORD [HostNumbers.HostNumber];
		//-- Ethernet boot file numbers are allocated from the same name space as
		//-- HostNumbers. Of course, these numbers do not represent hosts.

		CPPUNIT_ASSERT_EQUAL((CARD32) 9, SIZE(Boot::EthernetRequest));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(Boot::EthernetRequest, bfn));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 3, OFFSET(Boot::EthernetRequest, address));
	}
	void testLocation() {
		//Location: TYPE = MACHINE DEPENDENT RECORD [  -- format known by the initial microcode.
		//  -- Description of boot file location:
		//  deviceType (0): Device.Type,  -- e.g. sa4000, ethernet
		//  deviceOrdinal (1): CARDINAL,  -- position of device within all those of same type
		//  vp (2): SELECT OVERLAID * FROM
		//    disk => [diskFileID (2): DiskFileID],
		//    ethernetOne => [bootFileNumber (2): CARDINAL, net (3), host (4): CARDINAL = 0],
		//    ethernet => [ethernetRequest (2): EthernetRequest],
		//    any => [a (2), b (3), c (4), d (5), e (6), f (7), g (10B), h (11B): UNSPECIFIED],
		//    ENDCASE];
		CPPUNIT_ASSERT_EQUAL((CARD32)11, SIZE(Boot::Location));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(Boot::Location, deviceType));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 1, OFFSET(Boot::Location, deviceOrdinal));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(Boot::Location, diskFileID));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(Boot::Location, ethernetRequest));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(Boot::Location, a));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 3, OFFSET(Boot::Location, b));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 4, OFFSET(Boot::Location, c));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 5, OFFSET(Boot::Location, d));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 6, OFFSET(Boot::Location, e));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 7, OFFSET(Boot::Location, f));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 8, OFFSET(Boot::Location, g));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 9, OFFSET(Boot::Location, h));
	}
	void testRequest() {
		//Request: TYPE = MACHINE DEPENDENT RECORD [
		//  -- Basic portion of Request: (format known by the initial microcode)
		//  -- IF YOU CHANGE THE FORMAT OF THIS PORTION, YOU MUST INCREMENT currentRequestBasicVersion AND GENERATE NEW INITIAL MICROCODE!
		//  requestBasicVersion (0B): CARDINAL = currentRequestBasicVersion,
		//  action (1B): Action,
		//  location (2B): Location,
		//  switches (16B): System.Switches = System.defaultSwitches,  -- When calling OutLoad, these are the default switches to be used for system when InLoaded later.  When calling InLoad, these are switches to be used for system being InLoaded; defaultSwitches means use the ones built into the boot file.  When returning from InLoad, these are the switches passed from caller of InLoad or, if he passed defaultSwitches, the switches built into the boot file.
		//
		//  -- Extension portion of Request: (not used by the initial microcode)
		//  requestExtensionVersion (15B): CARDINAL = currentRequestExtensionVersion,
		//
		//   -- Extensions for InLoad - "results":
		//  pStartListHeader  (36B): StartList.Base,  -- only valid after inload of virgin boot file.
		//
		//  -- Extensions for OutLoad - "arguments":
		//  inLoadMode (40B): BootFile.InLoadMode,  -- real page numbers significant?
		//  -- As a side-effect of the cross-mds call mechanism, the entry point of the system being outLoaded is stored in pInitialLink= in the caller's mds and the mds of the system being outLoaded is passed to the Germ.
		//
		//  -- Extensions for OutLoad - "results":
		//  session (41B): Session ];  -- just finished OutLoad, or InLoaded later?
		CPPUNIT_ASSERT_EQUAL((CARD32)34, SIZE(Boot::Request));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(Boot::Request, requestBasicVersion));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 1, OFFSET(Boot::Request, action));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(Boot::Request, location));
    	CPPUNIT_ASSERT_EQUAL((CARD32)13, OFFSET(Boot::Request, requestExtensionVersion));
    	CPPUNIT_ASSERT_EQUAL((CARD32)14, OFFSET(Boot::Request, switches));
    	CPPUNIT_ASSERT_EQUAL((CARD32)30, OFFSET(Boot::Request, pStartListHeader));
    	CPPUNIT_ASSERT_EQUAL((CARD32)32, OFFSET(Boot::Request, inLoadMode));
    	CPPUNIT_ASSERT_EQUAL((CARD32)33, OFFSET(Boot::Request, session));
	}
	void testPVBootFiles() {
		//BootFileType: TYPE = MACHINE DEPENDENT{
		//  hardMicrocode (0), softMicrocode (1), germ (2), pilot (3), debugger (4),
		//  debuggee (5), initialMicrocode (6)};
		//PVBootFiles: TYPE = ARRAY BootFileType [hardMicrocode..pilot] OF DiskFileID;
		CPPUNIT_ASSERT_EQUAL((CARD32)36, SIZE(Boot::PVBootFiles)); // 9 x 4 = 36
	}
	void testLVBootFiles() {
		//BootFileType: TYPE = MACHINE DEPENDENT{
		//  hardMicrocode (0), softMicrocode (1), germ (2), pilot (3), debugger (4),
		//  debuggee (5), initialMicrocode (6)};
		//LVBootFiles: TYPE = ARRAY BootFileType [hardMicrocode..debuggee] OF DiskFileID;
		CPPUNIT_ASSERT_EQUAL((CARD32)54, SIZE(Boot::LVBootFiles)); // 9 x 6 = 54
	}


	// GuamInputOutput
	void testIORegionType() {
		// IORegionType: PRIVATE TYPE = MACHINE DEPENDENT RECORD [
		//   fcbPtrs(0): ARRAY AgentDeviceIndex OF LONG POINTER];
		CPPUNIT_ASSERT_EQUAL((CARD32)32, SIZE(GuamInputOutput::IORegionType));
 	}


	// BiParallelPortFace
	void testOperation_BiParallelPortFace() {
		//Operation: TYPE = RECORD [
		//  dataBufferPtr: LONG POINTER,	-- ptr to buffer that contains data to be transferred out
		//  					--	or will contain data transferred in
		//  bufferByteLength: CARDINAL		-- buffer size in bytes
		//  ];
		CPPUNIT_ASSERT_EQUAL((CARD32)3, SIZE(BiParallelPortFace::Operation));
    	CPPUNIT_ASSERT_EQUAL((CARD32)0, OFFSET(BiParallelPortFace::Operation, dataBufferPtr));
       	CPPUNIT_ASSERT_EQUAL((CARD32)2, OFFSET(BiParallelPortFace::Operation, bufferByteLength));
	}


	// FloppyDiskFace
	void testDiskAddress_Floppy() {
		//DiskAddress: TYPE = MACHINE DEPENDENT RECORD [
		//  cylinder(0):       CARDINAL,           --must be IN [0..numberOfCylinders)
		//  head(1:0..7):      Environment.Byte,   --must be IN [0..numberOfHeads)
		//  sector(1:8..15):   Environment.Byte];  --must be IN [1..maxSectorsPerTrack]
		CPPUNIT_ASSERT_EQUAL((CARD32)2, SIZE(FloppyDiskFace::DiskAddress));
    	CPPUNIT_ASSERT_EQUAL((CARD32)0, OFFSET(FloppyDiskFace::DiskAddress, cylinder));
       	CPPUNIT_ASSERT_EQUAL((CARD32)1, OFFSET(FloppyDiskFace::DiskAddress, u1));

       	FloppyDiskFace::DiskAddress t;

		t.cylinder = 0;
		t.head     = 0;
		t.sector   = 0;

		t.head     = ~t.head;
		CPPUNIT_ASSERT_EQUAL((CARD16)0xff00, t.u1);


		t.cylinder = 0;
		t.head     = 0;
		t.sector   = 0;

		t.sector   = ~t.sector;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x00ff, t.u1);
	}
	void testOperation_Floppy() {
		//Operation: TYPE = MACHINE DEPENDENT RECORD [
		//  device(0):                    DeviceHandle,
		//  function(1):                  Function,
		//  address(2):                   DiskAddress,  -- ignored if nop.  Sector is ignored in formatTrack operations.
		//  dataPtr(4):                   LONG POINTER, -- ignored if nop
		//  incrementDataPointer(6:0..0): BOOLEAN,      -- ignored if nop, formatTrack
		//  tries(6:1..15):               Tries,        -- Indication of how hard the Face should retry the operation in the presence of errors.  Increasing values of tries indicates that non-decreasing effort should be expended in retrying the failing operation.  Retries include device specific recovery (recalibrates, ...)
		//  count(7):                     CARDINAL];    -- ignored if nop, readID.  Number of tracks to format if formatTrack (must not exceed MaxTracksPerFormatOperation); number of sectors to transfer otherwise.
		CPPUNIT_ASSERT_EQUAL((CARD32)8, SIZE(FloppyDiskFace::Operation));
    	CPPUNIT_ASSERT_EQUAL((CARD32)0, OFFSET(FloppyDiskFace::Operation, device));
    	CPPUNIT_ASSERT_EQUAL((CARD32)1, OFFSET(FloppyDiskFace::Operation, function));
    	CPPUNIT_ASSERT_EQUAL((CARD32)2, OFFSET(FloppyDiskFace::Operation, address));
    	CPPUNIT_ASSERT_EQUAL((CARD32)4, OFFSET(FloppyDiskFace::Operation, dataPtr));
    	CPPUNIT_ASSERT_EQUAL((CARD32)6, OFFSET(FloppyDiskFace::Operation, u6));
    	CPPUNIT_ASSERT_EQUAL((CARD32)7, OFFSET(FloppyDiskFace::Operation, count));

       	FloppyDiskFace::Operation t;

       	t.u6 = 0;
       	t.incrementDataPointer = ~t.incrementDataPointer;
       	CPPUNIT_ASSERT_EQUAL((CARD16)0x8000, t.u6);

       	t.u6 = 0;
       	t.tries = ~t.tries;
       	CPPUNIT_ASSERT_EQUAL((CARD16)0x7fff, t.u6);
	}


	// BeepIOFaceGuam
	void testBeepFCBType() {
		//  BeepFCBType: TYPE = MACHINE DEPENDENT RECORD [
		//    frequency(0): CARDINAL]; -- 0 to disable beep
		CPPUNIT_ASSERT_EQUAL((CARD32) 1, SIZE(BeepIOFaceGuam::BeepFCBType));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(BeepIOFaceGuam::BeepFCBType, frequency));
	}

	// FloppyIOFaceGuam
	void testFloppyDCBType() {
		//  FloppyDCBType: TYPE = MACHINE DEPENDENT RECORD [
		//    deviceType(0): Device.Type,
		//    numberOfCylinders(1): CARDINAL,
		//    numberOfHeads(2): CARDINAL,
		//    sectorsPerTrack(3): CARDINAL,
		//    ready(4): BOOLEAN,
		//    diskChanged(5): BOOLEAN,
		//    twoSided(6): BOOLEAN,
		//    suggestedTries(7): CARDINAL];
		CPPUNIT_ASSERT_EQUAL((CARD32) 8, SIZE(FloppyIOFaceGuam::FloppyDCBType));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(FloppyIOFaceGuam::FloppyDCBType, deviceType));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 1, OFFSET(FloppyIOFaceGuam::FloppyDCBType, numberOfCylinders));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(FloppyIOFaceGuam::FloppyDCBType, numberOfHeads));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 3, OFFSET(FloppyIOFaceGuam::FloppyDCBType, sectorsPerTrack));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 4, OFFSET(FloppyIOFaceGuam::FloppyDCBType, ready));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 5, OFFSET(FloppyIOFaceGuam::FloppyDCBType, diskChanged));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 6, OFFSET(FloppyIOFaceGuam::FloppyDCBType, twoSided));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 7, OFFSET(FloppyIOFaceGuam::FloppyDCBType, suggestedTries));
	}

	// FloppyIOFaceGuam
	void testFloppyIOCBType() {
		//  FloppyIOCBType: TYPE = MACHINE DEPENDENT RECORD [
		//    operation(0): FloppyDiskFace.Operation,
		//    density(8): FloppyDiskFace.Density,
		//    sectorLength(9): CARDINAL, -- in words
		//    sectorsPerTrack(10): CARDINAL,
		//    status(11): FloppyDiskFace.Status = inProgress,
		//    nextIOCB(12): FloppyIOCBPtr = NIL,
		//    retries(14): CARDINAL = 0,
		//    logStatus(15): UNSPECIFIED = 0];
		CPPUNIT_ASSERT_EQUAL((CARD32)16, SIZE(FloppyIOFaceGuam::FloppyIOCBType));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(FloppyIOFaceGuam::FloppyIOCBType, operation));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 8, OFFSET(FloppyIOFaceGuam::FloppyIOCBType, density));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 9, OFFSET(FloppyIOFaceGuam::FloppyIOCBType, sectorLength));
    	CPPUNIT_ASSERT_EQUAL((CARD32)10, OFFSET(FloppyIOFaceGuam::FloppyIOCBType, sectorsPerTrack));
    	CPPUNIT_ASSERT_EQUAL((CARD32)11, OFFSET(FloppyIOFaceGuam::FloppyIOCBType, status));
    	CPPUNIT_ASSERT_EQUAL((CARD32)12, OFFSET(FloppyIOFaceGuam::FloppyIOCBType, nextIOCB));
    	CPPUNIT_ASSERT_EQUAL((CARD32)14, OFFSET(FloppyIOFaceGuam::FloppyIOCBType, retries));
    	CPPUNIT_ASSERT_EQUAL((CARD32)15, OFFSET(FloppyIOFaceGuam::FloppyIOCBType, logStatus));
	}

	// FloppyIOFaceGuam
	void testFloppyFCBType() {
		//  FloppyFCBType: TYPE = MACHINE DEPENDENT RECORD [
		//    nextIOCB(0): FloppyIOCBPtr,
		//    interruptSelector(2): GuamInputOutput.InterruptSelectorType,
		//    stopAgent(3): BOOLEAN,
		//    agentStopped(4): BOOLEAN,
		//    numberOfDCBs(5): CARDINAL,
		//    dcbs(6): SEQUENCE COMPUTED CARDINAL OF FloppyDCBType];
		CPPUNIT_ASSERT_EQUAL((CARD32) 6, SIZE(FloppyIOFaceGuam::FloppyFCBType));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(FloppyIOFaceGuam::FloppyFCBType, nextIOCB));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(FloppyIOFaceGuam::FloppyFCBType, interruptSelector));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 3, OFFSET(FloppyIOFaceGuam::FloppyFCBType, stopAgent));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 4, OFFSET(FloppyIOFaceGuam::FloppyFCBType, agentStopped));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 5, OFFSET(FloppyIOFaceGuam::FloppyFCBType, numberOfDCBs));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 6, OFFSET(FloppyIOFaceGuam::FloppyFCBType, dcbs));
	}

	// DiskIOFaceGuam
	void testDiskDCBType() {
		//  DiskDCBType: TYPE = MACHINE DEPENDENT RECORD [
		//    deviceType(0): Device.Type,
		//    numberOfCylinders(1): CARDINAL,
		//    numberOfHeads(2): CARDINAL,
		//    sectorsPerTrack(3): CARDINAL,
		//    agentDeviceData(4): ARRAY [0..6) OF WORD];

		CPPUNIT_ASSERT_EQUAL((CARD32)10, SIZE(DiskIOFaceGuam::DiskDCBType));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(DiskIOFaceGuam::DiskDCBType, deviceType));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 1, OFFSET(DiskIOFaceGuam::DiskDCBType, numberOfCylinders));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(DiskIOFaceGuam::DiskDCBType, numberOfHeads));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 3, OFFSET(DiskIOFaceGuam::DiskDCBType, sectorsPerTrack));
       	CPPUNIT_ASSERT_EQUAL((CARD32) 4, OFFSET(DiskIOFaceGuam::DiskDCBType, agentDeviceData));
	}

	// DiskIOFaceGuam
	void testDiskFCBType() {
		//  DiskFCBType: TYPE = MACHINE DEPENDENT RECORD [
		//    nextIOCB(0): DiskIOCBPtr,
		//    interruptSelector(2): GuamInputOutput.InterruptSelectorType,
		//    stopAgent(3): BOOLEAN,
		//    agentStopped(4): BOOLEAN,
		//    numberOfDCBs(5): CARDINAL,
		//    dcbs(6): SEQUENCE COMPUTED CARDINAL OF DiskDCBType];
		CPPUNIT_ASSERT_EQUAL((CARD32) 6, SIZE(DiskIOFaceGuam::DiskFCBType));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(DiskIOFaceGuam::DiskFCBType, nextIOCB));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(DiskIOFaceGuam::DiskFCBType, interruptSelector));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 3, OFFSET(DiskIOFaceGuam::DiskFCBType, stopAgent));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 4, OFFSET(DiskIOFaceGuam::DiskFCBType, agentStopped));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 5, OFFSET(DiskIOFaceGuam::DiskFCBType, numberOfDCBs));
       	CPPUNIT_ASSERT_EQUAL((CARD32) 6, OFFSET(DiskIOFaceGuam::DiskFCBType, dcbs));
	}

	// DiskIOFaceGuam
	void testDiskIOCBType() {
		//  DiskIOCBType: TYPE = MACHINE DEPENDENT RECORD [
		//    operation(0): PilotDiskFace.Operation,
		//    deviceIndex(13): CARDINAL,
		//    diskAddress(14): PilotDiskFace.DiskAddress,
		//    dataPtr(16): LONG POINTER,
		//    incrementDataPtr(18): BOOLEAN,
		//    command(19): PilotDiskFace.Command,
		//    pageCount(20): CARDINAL,
		//    status(21): PilotDiskFace.Status = inProgress,
		//    nextIOCB(22): DiskIOCBPtr = NIL,
		//    agentOperationData(24): ARRAY [0..10) OF WORD = ALL[0] ];

		CPPUNIT_ASSERT_EQUAL((CARD32)34, SIZE(DiskIOFaceGuam::DiskIOCBType));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(DiskIOFaceGuam::DiskIOCBType, operation));
    	CPPUNIT_ASSERT_EQUAL((CARD32)13, OFFSET(DiskIOFaceGuam::DiskIOCBType, deviceIndex));
    	CPPUNIT_ASSERT_EQUAL((CARD32)14, OFFSET(DiskIOFaceGuam::DiskIOCBType, diskAddress));
    	CPPUNIT_ASSERT_EQUAL((CARD32)16, OFFSET(DiskIOFaceGuam::DiskIOCBType, dataPtr));
    	CPPUNIT_ASSERT_EQUAL((CARD32)18, OFFSET(DiskIOFaceGuam::DiskIOCBType, incrementDataPtr));
    	CPPUNIT_ASSERT_EQUAL((CARD32)19, OFFSET(DiskIOFaceGuam::DiskIOCBType, command));
    	CPPUNIT_ASSERT_EQUAL((CARD32)20, OFFSET(DiskIOFaceGuam::DiskIOCBType, pageCount));
    	CPPUNIT_ASSERT_EQUAL((CARD32)21, OFFSET(DiskIOFaceGuam::DiskIOCBType, status));
    	CPPUNIT_ASSERT_EQUAL((CARD32)22, OFFSET(DiskIOFaceGuam::DiskIOCBType, nextIOCB));
    	CPPUNIT_ASSERT_EQUAL((CARD32)24, OFFSET(DiskIOFaceGuam::DiskIOCBType, agentOperationData));
 	}

	// PrcessorIOFaceGuam
	void testProcessorFCBType() {
		//  ProcessorFCBType: TYPE = MACHINE DEPENDENT RECORD [
		//    processorID(0): ProcessorFace.ProcessorID,
		//    microsecondsPerHundredPulses(3): CARDINAL,
		//    millisecondsPerTick(4): CARDINAL,
		//    alignmentFiller(5): WORD,
		//    realMemoryPageCount(6): LONG CARDINAL,
		//    virtualMemoryPageCount(8): LONG CARDINAL,
		//    gmt(10): System.GreenwichMeanTime,
		//    command(12): ProcessorCommand,
		//    status(13): ProcessorStatus];

		CPPUNIT_ASSERT_EQUAL((CARD32)14, SIZE(ProcessorIOFaceGuam::ProcessorFCBType));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(ProcessorIOFaceGuam::ProcessorFCBType, processorID));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 3, OFFSET(ProcessorIOFaceGuam::ProcessorFCBType, microsecondsPerHundredPulses));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 4, OFFSET(ProcessorIOFaceGuam::ProcessorFCBType, millisecondsPerTick));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 5, OFFSET(ProcessorIOFaceGuam::ProcessorFCBType, alignmentFiller));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 6, OFFSET(ProcessorIOFaceGuam::ProcessorFCBType, realMemoryPageCount));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 8, OFFSET(ProcessorIOFaceGuam::ProcessorFCBType, virtualMemoryPageCount));
    	CPPUNIT_ASSERT_EQUAL((CARD32)10, OFFSET(ProcessorIOFaceGuam::ProcessorFCBType, gmt));
    	CPPUNIT_ASSERT_EQUAL((CARD32)12, OFFSET(ProcessorIOFaceGuam::ProcessorFCBType, command));
    	CPPUNIT_ASSERT_EQUAL((CARD32)13, OFFSET(ProcessorIOFaceGuam::ProcessorFCBType, status));
	}

	// RS232CEnvironment
	void testFlowControl() {
		//FlowControl: TYPE = MACHINE DEPENDENT RECORD [
		//  type(0): {none, xOnXOff},
		//  xOn(1), xOff(2): UNSPECIFIED];
		CPPUNIT_ASSERT_EQUAL((CARD32) 3, SIZE(RS232CEnvironment::FlowControl));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(RS232CEnvironment::FlowControl, type));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 1, OFFSET(RS232CEnvironment::FlowControl, xOn));
    	CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(RS232CEnvironment::FlowControl, xOff));
	}

	// NewRS232CFace
	void testParameterRecord() {
		//ParameterRecord: TYPE = MACHINE DEPENDENT RECORD [
		//  charLength(0:8..10): RS232CEnvironment.CharLength,
		//  clientType(2:1..7): ClientType,
		//  correspondent(2:8..15): RS232CEnvironment.Correspondent,
		//  echo(2:0..0): BOOLEAN,
		//  flowControl(4:0..47): RS232CEnvironment.FlowControl,
		//  frameTimeout(3): CARDINAL,
		//  lineSpeed(0:3..7): RS232CEnvironment.LineSpeed,
		//  lineType(0:0..2): RS232CEnvironment.LineType,
		//  parity(0:11..13): RS232CEnvironment.Parity,
		//  stopBits(0:14..15): RS232CEnvironment.StopBits,
		//  syncChar(1:8..15): RS232CEnvironment.SyncChar,
		//  syncCount(1:0..7): RS232CEnvironment.SyncCount];
		CPPUNIT_ASSERT_EQUAL((CARD32) 7, SIZE(NewRS232CFace::ParameterRecord));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(NewRS232CFace::ParameterRecord, u0));
		CPPUNIT_ASSERT_EQUAL((CARD32) 1, OFFSET(NewRS232CFace::ParameterRecord, u1));
		CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(NewRS232CFace::ParameterRecord, u2));
		CPPUNIT_ASSERT_EQUAL((CARD32) 3, OFFSET(NewRS232CFace::ParameterRecord, frameTimeout));
		CPPUNIT_ASSERT_EQUAL((CARD32) 4, OFFSET(NewRS232CFace::ParameterRecord, flowControl));

		NewRS232CFace::ParameterRecord t;

		//  charLength(0:8..10): RS232CEnvironment.CharLength,
		t.u0 = 0;
		t.charLength = ~t.charLength;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x00e0, t.u0);

		//  clientType(2:1..7): ClientType,
		t.u2 = 0;
		t.clientType = ~t.clientType;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x7f00, t.u2);

		//  correspondent(2:8..15): RS232CEnvironment.Correspondent,
		t.u2 = 0;
		t.correspondent = ~t.correspondent;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x00ff, t.u2);

		//  echo(2:0..0): BOOLEAN,
		t.u2 = 0;
		t.echo = ~t.echo;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x8000, t.u2);

		//  flowControl(4:0..47): RS232CEnvironment.FlowControl,
		//  frameTimeout(3): CARDINAL,

		//  lineSpeed(0:3..7): RS232CEnvironment.LineSpeed,
		t.u0 = 0;
		t.lineSpeed = ~t.lineSpeed;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x1f00, t.u0);

		//  lineType(0:0..2): RS232CEnvironment.LineType,
		t.u0 = 0;
		t.lineType = ~t.lineType;
		CPPUNIT_ASSERT_EQUAL((CARD16)0xe000, t.u0);

		//  parity(0:11..13): RS232CEnvironment.Parity,
		t.u0 = 0;
		t.parity = ~t.parity;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x001c, t.u0);

		//  stopBits(0:14..15): RS232CEnvironment.StopBits,
		t.u0 = 0;
		t.stopBits = ~t.stopBits;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0003, t.u0);

		//  syncChar(1:8..15): RS232CEnvironment.SyncChar,
		t.u1 = 0;
		t.syncChar = ~t.syncChar;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x00ff, t.u1);

		//  syncCount(1:0..7): RS232CEnvironment.SyncCount];
		t.u1 = 0;
		t.syncCount = ~t.syncCount;
		CPPUNIT_ASSERT_EQUAL((CARD16)0xff00, t.u1);
	}
	void testOperation_NewRS232CFace() {
		//Operation: TYPE = MACHINE DEPENDENT RECORD [
		//  dataBuffer(0:0..31): LONG POINTER,
		//  bufferByteLength(2:0..15): CARDINAL];
		CPPUNIT_ASSERT_EQUAL((CARD32) 3, SIZE(NewRS232CFace::Operation));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(NewRS232CFace::Operation, dataBuffer));
		CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(NewRS232CFace::Operation, bufferByteLength));
	}
	void testControlRecord() {
		//ControlRecord: TYPE = MACHINE DEPENDENT RECORD [
		//  dataTerminalReady(0:0..0): BOOLEAN,
		//  requestToSend(0:1..1): BOOLEAN,
		//  unused(0:2..15): [0..37777B] _ 0];
		CPPUNIT_ASSERT_EQUAL((CARD32) 1, SIZE(NewRS232CFace::ControlRecord));

		NewRS232CFace::ControlRecord t;
		//  dataTerminalReady(0:0..0): BOOLEAN,
		t.u = 0;
		t.dataTerminalReady = ~t.dataTerminalReady;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x8000, t.u);

		//  requestToSend(0:1..1): BOOLEAN,
		t.u = 0;
		t.requestToSend = ~t.requestToSend;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x4000, t.u);

		//  unused(0:2..15): [0..37777B] _ 0];
		t.u = 0;
		t.unused = ~t.unused;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x3fff, t.u);
	}
	void testDeviceStatus() {
		//DeviceStatus: TYPE = MACHINE DEPENDENT RECORD [
		//  -- latched status bits
		CPPUNIT_ASSERT_EQUAL((CARD32) 1, SIZE(NewRS232CFace::DeviceStatus));

		NewRS232CFace::DeviceStatus t;

		//  breakDetected(0:0..0): BOOLEAN,
		t.u = 0;
		t.breakDetected = ~t.breakDetected;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x8000, t.u);

		//  dataLost(0:1..1): BOOLEAN,
		t.u = 0;
		t.dataLost = ~t.dataLost;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x4000, t.u);

		//  ringHeard(0:2..2): BOOLEAN,
		t.u = 0;
		t.ringHeard = ~t.ringHeard;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x2000, t.u);

		//  -- unlatched status bits
		//  carrierDetect(0:3..3): BOOLEAN,
		t.u = 0;
		t.carrierDetect = ~t.carrierDetect;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x1000, t.u);

		//  clearToSend(0:4..4): BOOLEAN,
		t.u = 0;
		t.clearToSend = ~t.clearToSend;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0800, t.u);

		//  dataSetReady(0:5..5): BOOLEAN,
		t.u = 0;
		t.dataSetReady = ~t.dataSetReady;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0400, t.u);

		//  ringIndicator(0:6..6): BOOLEAN,
		t.u = 0;
		t.ringIndicator = ~t.ringIndicator;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0200, t.u);

		//  unused(0:7..15): [0..777B] _ 0];
		t.u = 0;
		t.unused = ~t.unused;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x01ff, t.u);
	}

	// CoProcessorIOFaceGuam
	void testCoProcessorFCBType() {
		//CoProcessorFCBType: TYPE = MACHINE DEPENDENT RECORD [
		//  iocbHead(0): CoProcessorIOCBPtr,
		//  iocbNext(2): CoProcessorIOCBPtr,
		//  headCommand(4): CommandType,
		//  filler5(5): WORD,
		//  headResult(6): ResultType,
		//  filler7(7): WORD,
		//  interruptSelector(8): GuamInputOutput.InterruptSelectorType,
		//  stopAgent(9): BOOLEAN,
		//  agentStopped(10): BOOLEAN,
		//  streamWordSize(11): CARDINAL];
		CPPUNIT_ASSERT_EQUAL((CARD32)12, SIZE(CoProcessorIOFaceGuam::CoProcessorFCBType));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(CoProcessorIOFaceGuam::CoProcessorFCBType, iocbHead));
		CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(CoProcessorIOFaceGuam::CoProcessorFCBType, iocbNext));
		CPPUNIT_ASSERT_EQUAL((CARD32) 4, OFFSET(CoProcessorIOFaceGuam::CoProcessorFCBType, headCommand));
		CPPUNIT_ASSERT_EQUAL((CARD32) 5, OFFSET(CoProcessorIOFaceGuam::CoProcessorFCBType, filler5));
		CPPUNIT_ASSERT_EQUAL((CARD32) 6, OFFSET(CoProcessorIOFaceGuam::CoProcessorFCBType, headResult));
		CPPUNIT_ASSERT_EQUAL((CARD32) 7, OFFSET(CoProcessorIOFaceGuam::CoProcessorFCBType, filler7));
		CPPUNIT_ASSERT_EQUAL((CARD32) 8, OFFSET(CoProcessorIOFaceGuam::CoProcessorFCBType, interruptSelector));
		CPPUNIT_ASSERT_EQUAL((CARD32) 9, OFFSET(CoProcessorIOFaceGuam::CoProcessorFCBType, stopAgent));
		CPPUNIT_ASSERT_EQUAL((CARD32)10, OFFSET(CoProcessorIOFaceGuam::CoProcessorFCBType, agentStopped));
		CPPUNIT_ASSERT_EQUAL((CARD32)11, OFFSET(CoProcessorIOFaceGuam::CoProcessorFCBType, streamWordSize));
	}
	void testTransferRec() {
		//TransferRec: TYPE = MACHINE DEPENDENT RECORD [
		//  subSequence(0): INTEGER = 0,
		//  filler1(1): WORD = 0,
		//  filler2(2:0..12): [0..8191] = 0,
		//  endStream(2:13..13): BOOLEAN = FALSE,
		//  endRecord(2:14..14): BOOLEAN = FALSE,
		//  endSST(2:15..15): BOOLEAN = FALSE,
		//  filler3(3): WORD = 0,
		//  bytesWritten(4): CARDINAL = 0,
		//  filler5(5): WORD = 0,
		//  bytesRead(6): CARDINAL = 0,
		//  filler7(7): WORD = 0,
		//  hTask(8): UNSPECIFIED = 0,
		//  interruptMesa(9): BOOLEAN = FALSE,
		//  buffer(10): LONG POINTER = NIL,
		//  bufferSize(12): CARDINAL = 0,
		//  filler13(13): WORD = 0,
		//  writeLockedByMesa(14): BOOLEAN = FALSE,
		//  filler15(15): WORD = 0];
		CPPUNIT_ASSERT_EQUAL((CARD32)16, SIZE(CoProcessorIOFaceGuam::TransferRec));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(CoProcessorIOFaceGuam::TransferRec, subSequence));
		CPPUNIT_ASSERT_EQUAL((CARD32) 1, OFFSET(CoProcessorIOFaceGuam::TransferRec, filler1));
		CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(CoProcessorIOFaceGuam::TransferRec, u2));
		CPPUNIT_ASSERT_EQUAL((CARD32) 3, OFFSET(CoProcessorIOFaceGuam::TransferRec, filler3));
		CPPUNIT_ASSERT_EQUAL((CARD32) 4, OFFSET(CoProcessorIOFaceGuam::TransferRec, bytesWritten));
		CPPUNIT_ASSERT_EQUAL((CARD32) 5, OFFSET(CoProcessorIOFaceGuam::TransferRec, filler5));
		CPPUNIT_ASSERT_EQUAL((CARD32) 6, OFFSET(CoProcessorIOFaceGuam::TransferRec, bytesRead));
		CPPUNIT_ASSERT_EQUAL((CARD32) 7, OFFSET(CoProcessorIOFaceGuam::TransferRec, filler7));
		CPPUNIT_ASSERT_EQUAL((CARD32) 8, OFFSET(CoProcessorIOFaceGuam::TransferRec, hTask));
		CPPUNIT_ASSERT_EQUAL((CARD32) 9, OFFSET(CoProcessorIOFaceGuam::TransferRec, interruptMesa));
		CPPUNIT_ASSERT_EQUAL((CARD32)10, OFFSET(CoProcessorIOFaceGuam::TransferRec, buffer));
		CPPUNIT_ASSERT_EQUAL((CARD32)12, OFFSET(CoProcessorIOFaceGuam::TransferRec, bufferSize));
		CPPUNIT_ASSERT_EQUAL((CARD32)13, OFFSET(CoProcessorIOFaceGuam::TransferRec, filler13));
		CPPUNIT_ASSERT_EQUAL((CARD32)14, OFFSET(CoProcessorIOFaceGuam::TransferRec, writeLockedByMesa));
		CPPUNIT_ASSERT_EQUAL((CARD32)15, OFFSET(CoProcessorIOFaceGuam::TransferRec, filler15));

		CoProcessorIOFaceGuam::TransferRec t;

		//  filler2(2:0..12): [0..8191] = 0,
		t.u2 = 0;
		t.filler2 = ~t.filler2;
		CPPUNIT_ASSERT_EQUAL((CARD16)0xfff8, t.u2);

		//  endStream(2:13..13): BOOLEAN = FALSE,
		t.u2 = 0;
		t.endStream = ~t.endStream;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0004, t.u2);

		//  endRecord(2:14..14): BOOLEAN = FALSE,
		t.u2 = 0;
		t.endRecord = ~t.endRecord;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0002, t.u2);

		//  endSST(2:15..15): BOOLEAN = FALSE,
		t.u2 = 0;
		t.endSST = ~t.endSST;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0001, t.u2);
	}
	void testCoProcessorIOCBType() {
		//CoProcessorIOCBType: TYPE = MACHINE DEPENDENT RECORD [
		//  serverID(0): LONG CARDINAL,
		//  mesaConnectionState(2): ConnectionStateType = idle,
		//  filler3(3): WORD = 0,
		//  pcConnectionState(4): ConnectionStateType = idle,
		//  filler5(5): WORD = 0,
		//  mesaPut(6): TransferRec = [],
		//  mesaGet(22): TransferRec = [],
		//  nextIOCB(38): CoProcessorIOCBPtr = NIL,
		//  mesaIsServer(40): BOOLEAN = FALSE,
		//  filler41(41): WORD = 0];
		CPPUNIT_ASSERT_EQUAL((CARD32)42, SIZE(CoProcessorIOFaceGuam::CoProcessorIOCBType));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(CoProcessorIOFaceGuam::CoProcessorIOCBType, serverID));
		CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(CoProcessorIOFaceGuam::CoProcessorIOCBType, mesaConnectionState));
		CPPUNIT_ASSERT_EQUAL((CARD32) 3, OFFSET(CoProcessorIOFaceGuam::CoProcessorIOCBType, filler3));
		CPPUNIT_ASSERT_EQUAL((CARD32) 4, OFFSET(CoProcessorIOFaceGuam::CoProcessorIOCBType, pcConnectionState));
		CPPUNIT_ASSERT_EQUAL((CARD32) 5, OFFSET(CoProcessorIOFaceGuam::CoProcessorIOCBType, filler5));
		CPPUNIT_ASSERT_EQUAL((CARD32) 6, OFFSET(CoProcessorIOFaceGuam::CoProcessorIOCBType, mesaPut));
		CPPUNIT_ASSERT_EQUAL((CARD32)22, OFFSET(CoProcessorIOFaceGuam::CoProcessorIOCBType, mesaGet));
		CPPUNIT_ASSERT_EQUAL((CARD32)38, OFFSET(CoProcessorIOFaceGuam::CoProcessorIOCBType, nextIOCB));
		CPPUNIT_ASSERT_EQUAL((CARD32)40, OFFSET(CoProcessorIOFaceGuam::CoProcessorIOCBType, mesaIsServer));
		CPPUNIT_ASSERT_EQUAL((CARD32)41, OFFSET(CoProcessorIOFaceGuam::CoProcessorIOCBType, filler41));
	}

	// DisplayIOFaceGuam
	void testDisplayCoordinate() {
		//DisplayCoordinate: TYPE = MACHINE DEPENDENT RECORD [
		//  x(0): CARDINAL,
		//  y(1): CARDINAL];
		CPPUNIT_ASSERT_EQUAL((CARD32) 2, SIZE(DisplayIOFaceGuam::DisplayCoordinate));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(DisplayIOFaceGuam::DisplayCoordinate, x));
		CPPUNIT_ASSERT_EQUAL((CARD32) 1, OFFSET(DisplayIOFaceGuam::DisplayCoordinate, y));
	}
	void testDisplayRectangle() {
		//DisplayRectangle: TYPE = MACHINE DEPENDENT RECORD [
		//  origin(0): DisplayCoordinate,
		//  width(2): CARDINAL,
		//  height(3): CARDINAL];
		CPPUNIT_ASSERT_EQUAL((CARD32) 4, SIZE(DisplayIOFaceGuam::DisplayRectangle));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(DisplayIOFaceGuam::DisplayRectangle, origin));
		CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(DisplayIOFaceGuam::DisplayRectangle, width));
		CPPUNIT_ASSERT_EQUAL((CARD32) 3, OFFSET(DisplayIOFaceGuam::DisplayRectangle, height));
	}
	void testLookupTableEntry() {
		//LookupTableEntry: TYPE = MACHINE DEPENDENT RECORD [
		//  reserved(1:0..7): Environment.Byte,
		//  blue(1:8..15): Environment.Byte,
		//  green(0:0..7): Environment.Byte,
		//  red(0:8..15): Environment.Byte];
		CPPUNIT_ASSERT_EQUAL((CARD32) 2, SIZE(DisplayIOFaceGuam::LookupTableEntry));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(DisplayIOFaceGuam::LookupTableEntry, u0));
		CPPUNIT_ASSERT_EQUAL((CARD32) 1, OFFSET(DisplayIOFaceGuam::LookupTableEntry, u1));

		DisplayIOFaceGuam::LookupTableEntry t;

		//  reserved(1:0..7): Environment.Byte,
		t.u1 = 0;
		t.reserved = ~t.reserved;
		CPPUNIT_ASSERT_EQUAL((CARD16)0xff00, t.u1);

		//  blue(1:8..15): Environment.Byte,
		t.u1 = 0;
		t.blue = ~t.blue;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x00ff, t.u1);

		//  green(0:0..7): Environment.Byte,
		t.u0 = 0;
		t.green = ~t.green;
		CPPUNIT_ASSERT_EQUAL((CARD16)0xff00, t.u0);

		//  red(0:8..15): Environment.Byte];
		t.u0 = 0;
		t.red = ~t.red;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x00ff, t.u0);
	}
	void testDisplayFCBType() {
		//DisplayFCBType: TYPE = MACHINE DEPENDENT RECORD [
		//  -- initialization values
		//  displayType(37): DisplayType,
		//  displayWidth(38): CARDINAL,
		//  displayHeight(39): CARDINAL,
		//  displayMemoryAddress(2): LONG UNSPECIFIED,
		//  -- general display command information
		//  command(0): DisplayCommand,
		//  status(1): DisplayStatus,
		//  -- display data change parameters
		//  destRectangle(8): DisplayRectangle,
		//  sourceOrigin(12): DisplayCoordinate,
		//  colorMapping(6): ARRAY [0..1] OF CARDINAL,
		//  pattern(30): ARRAY [0..4) OF WORD,
		//  patternFillMode(34): PatternFillMode,
		//  -- display appearance parameters
		//  cursorPattern(14): DisplayFace.Cursor,
		//  complemented(35): BOOLEAN,
		//  -- color lookup table information
		//  colorIndex(36): CARDINAL,
		//  color(4): LookupTableEntry];
		CPPUNIT_ASSERT_EQUAL((CARD32)40, SIZE(DisplayIOFaceGuam::DisplayFCBType));
		CPPUNIT_ASSERT_EQUAL((CARD32)37, OFFSET(DisplayIOFaceGuam::DisplayFCBType, displayType));
		CPPUNIT_ASSERT_EQUAL((CARD32)38, OFFSET(DisplayIOFaceGuam::DisplayFCBType, displayWidth));
		CPPUNIT_ASSERT_EQUAL((CARD32)39, OFFSET(DisplayIOFaceGuam::DisplayFCBType, displayHeight));
		CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(DisplayIOFaceGuam::DisplayFCBType, displayMemoryAddress));

		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(DisplayIOFaceGuam::DisplayFCBType, command));
		CPPUNIT_ASSERT_EQUAL((CARD32) 1, OFFSET(DisplayIOFaceGuam::DisplayFCBType, status));

		CPPUNIT_ASSERT_EQUAL((CARD32) 8, OFFSET(DisplayIOFaceGuam::DisplayFCBType, destRectangle));
		CPPUNIT_ASSERT_EQUAL((CARD32)12, OFFSET(DisplayIOFaceGuam::DisplayFCBType, sourceOrigin));
		CPPUNIT_ASSERT_EQUAL((CARD32) 6, OFFSET(DisplayIOFaceGuam::DisplayFCBType, colorMapping));
		CPPUNIT_ASSERT_EQUAL((CARD32)30, OFFSET(DisplayIOFaceGuam::DisplayFCBType, pattern));
		CPPUNIT_ASSERT_EQUAL((CARD32)34, OFFSET(DisplayIOFaceGuam::DisplayFCBType, patternFillMode));

		CPPUNIT_ASSERT_EQUAL((CARD32)14, OFFSET(DisplayIOFaceGuam::DisplayFCBType, cursorPattern));
		CPPUNIT_ASSERT_EQUAL((CARD32)35, OFFSET(DisplayIOFaceGuam::DisplayFCBType, complemented));
		CPPUNIT_ASSERT_EQUAL((CARD32)36, OFFSET(DisplayIOFaceGuam::DisplayFCBType, colorIndex));
		CPPUNIT_ASSERT_EQUAL((CARD32) 4, OFFSET(DisplayIOFaceGuam::DisplayFCBType, color));
	}

	// EthernetIOFaceGuam
	void testEthernetFCBType() {
		//EthernetFCBType: TYPE = MACHINE DEPENDENT RECORD [
		//  receiveIOCB(0): EthernetIOCBPtr,
		//  transmitIOCB(2): EthernetIOCBPtr,
		//  receiveInterruptSelector(4): GuamInputOutput.InterruptSelectorType,
		//  transmitInterruptSelector(5): GuamInputOutput.InterruptSelectorType,
		//  stopAgent(6): BOOLEAN,
		//  receiveStopped(7): BOOLEAN,
		//  transmitStopped(8): BOOLEAN,
		//  hearSelf(9): BOOLEAN,
		//  processorID(10): HostNumbers.ProcessorID, -- set by head
		//  packetsMissed(13): CARDINAL,
		//  agentBlockSize(14): CARDINAL]; -- additional words in IOCB for agent
		CPPUNIT_ASSERT_EQUAL((CARD32)15, SIZE(EthernetIOFaceGuam::EthernetFCBType));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(EthernetIOFaceGuam::EthernetFCBType, receiveIOCB));
		CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(EthernetIOFaceGuam::EthernetFCBType, transmitIOCB));
		CPPUNIT_ASSERT_EQUAL((CARD32) 4, OFFSET(EthernetIOFaceGuam::EthernetFCBType, receiveInterruptSelector));
		CPPUNIT_ASSERT_EQUAL((CARD32) 5, OFFSET(EthernetIOFaceGuam::EthernetFCBType, transmitInterruptSelector));
		CPPUNIT_ASSERT_EQUAL((CARD32) 6, OFFSET(EthernetIOFaceGuam::EthernetFCBType, stopAgent));
		CPPUNIT_ASSERT_EQUAL((CARD32) 7, OFFSET(EthernetIOFaceGuam::EthernetFCBType, receiveStopped));
		CPPUNIT_ASSERT_EQUAL((CARD32) 8, OFFSET(EthernetIOFaceGuam::EthernetFCBType, transmitStopped));
		CPPUNIT_ASSERT_EQUAL((CARD32) 9, OFFSET(EthernetIOFaceGuam::EthernetFCBType, hearSelf));
		CPPUNIT_ASSERT_EQUAL((CARD32)10, OFFSET(EthernetIOFaceGuam::EthernetFCBType, processorID));
		CPPUNIT_ASSERT_EQUAL((CARD32)13, OFFSET(EthernetIOFaceGuam::EthernetFCBType, packetsMissed));
		CPPUNIT_ASSERT_EQUAL((CARD32)14, OFFSET(EthernetIOFaceGuam::EthernetFCBType, agentBlockSize));
	}
	void testEthernetIOCBType() {
		//EthernetIOCBType: TYPE = MACHINE DEPENDENT RECORD [
		//  bufferAddress(0): LONG POINTER,
		//  bufferLength(2): CARDINAL, -- in bytes
		//  actualLength(3): CARDINAL, -- of received packet, in bytes
		//  dequeued(4:0..0): BOOLEAN,
		//  packetType(4:1..7): {receive, transmit},
		//  status(4:8..15): EthernetIOCBStatus = inProgress,
		//  retries(5): CARDINAL = 0,
		//  nextIOCB(6): EthernetIOCBPtr = NIL];
		CPPUNIT_ASSERT_EQUAL((CARD32) 8, SIZE(EthernetIOFaceGuam::EthernetIOCBType));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(EthernetIOFaceGuam::EthernetIOCBType, bufferAddress));
		CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(EthernetIOFaceGuam::EthernetIOCBType, bufferLength));
		CPPUNIT_ASSERT_EQUAL((CARD32) 3, OFFSET(EthernetIOFaceGuam::EthernetIOCBType, actualLength));
		CPPUNIT_ASSERT_EQUAL((CARD32) 4, OFFSET(EthernetIOFaceGuam::EthernetIOCBType, u4));
		CPPUNIT_ASSERT_EQUAL((CARD32) 5, OFFSET(EthernetIOFaceGuam::EthernetIOCBType, retries));
		CPPUNIT_ASSERT_EQUAL((CARD32) 6, OFFSET(EthernetIOFaceGuam::EthernetIOCBType, nextIOCB));

		EthernetIOFaceGuam::EthernetIOCBType t;

		//  dequeued(4:0..0): BOOLEAN,
		t.u4 = 0;
		t.dequeued = ~t.dequeued;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x8000, t.u4);

		//  packetType(4:1..7): {receive, transmit},
		t.u4 = 0;
		t.packetType = ~t.packetType;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x7f00, t.u4);

		//  status(4:8..15): EthernetIOCBStatus = inProgress,
		t.u4 = 0;
		t.status = ~t.status;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x00ff, t.u4);
	}

	// KeyboardIOFaceGuam
	void testKeyboardFCBType() {
		CPPUNIT_ASSERT_EQUAL((CARD32) 7, SIZE(KeyboardIOFaceGuam::KeyboardFCBType));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(KeyboardIOFaceGuam::KeyboardFCBType, keyStates));
	}

	// MouseIOFaceGuam
	void testMousePositionType() {
		//MousePositionType: TYPE = MACHINE DEPENDENT RECORD [
		//  mouseXCoord(0): INTEGER,
		//  mouseYCoord(1): INTEGER];
		CPPUNIT_ASSERT_EQUAL((CARD32) 2, SIZE(MouseIOFaceGuam::MousePositionType));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(MouseIOFaceGuam::MousePositionType, mouseXCoord));
		CPPUNIT_ASSERT_EQUAL((CARD32) 1, OFFSET(MouseIOFaceGuam::MousePositionType, mouseYCoord));
	}
	void testMouseFCBType() {
		//MouseFCBType: TYPE = MACHINE DEPENDENT RECORD [
		//  currentMousePosition(0): MousePositionType,
		//  cursorOffset(2): MousePositionType,
		//  newValue(4): MousePositionType,
		//  command(6): MouseCommandType];
		CPPUNIT_ASSERT_EQUAL((CARD32) 7, SIZE(MouseIOFaceGuam::MouseFCBType));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(MouseIOFaceGuam::MouseFCBType, currentMousePosition));
		CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(MouseIOFaceGuam::MouseFCBType, cursorOffset));
		CPPUNIT_ASSERT_EQUAL((CARD32) 4, OFFSET(MouseIOFaceGuam::MouseFCBType, newValue));
		CPPUNIT_ASSERT_EQUAL((CARD32) 6, OFFSET(MouseIOFaceGuam::MouseFCBType, command));
	}

	// ParallelIOFaceGuam
	void testParallelFCBType() {
		//ParallelFCBType: TYPE = MACHINE DEPENDENT RECORD [
		//  nextIOCB(0): ParallelIOCBPtr,
		//  interruptSelector(2): GuamInputOutput.InterruptSelectorType,
		//  stopAgent(3): BOOLEAN,
		//  agentStopped(4): BOOLEAN,
		//  numberOfPorts(5): CARDINAL];
		CPPUNIT_ASSERT_EQUAL((CARD32) 6, SIZE(ParallelIOFaceGuam::ParallelFCBType));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(ParallelIOFaceGuam::ParallelFCBType, nextIOCB));
		CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(ParallelIOFaceGuam::ParallelFCBType, interruptSelector));
		CPPUNIT_ASSERT_EQUAL((CARD32) 3, OFFSET(ParallelIOFaceGuam::ParallelFCBType, stopAgent));
		CPPUNIT_ASSERT_EQUAL((CARD32) 4, OFFSET(ParallelIOFaceGuam::ParallelFCBType, agentStopped));
		CPPUNIT_ASSERT_EQUAL((CARD32) 5, OFFSET(ParallelIOFaceGuam::ParallelFCBType, numberOfPorts));
	}
	void testParallelIOCBType() {
		//ParallelIOCBType: TYPE = MACHINE DEPENDENT RECORD [
		//  operation(0): BiParallelPortFace.Operation,
		//  port(3): CARDINAL,
		//  transferMode(4): BiParallelPortFace.TransferMode,
		//  controlOrData(5): BiParallelPortFace.ControlOrData,
		//  nextIOCB(6): ParallelIOCBPtr = NIL,
		//  status(8): ParallelIOCBStatus = inProgress,
		//  bytesTransferred(9): CARDINAL];
		CPPUNIT_ASSERT_EQUAL((CARD32)10, SIZE(ParallelIOFaceGuam::ParallelIOCBType));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(ParallelIOFaceGuam::ParallelIOCBType, operation));
		CPPUNIT_ASSERT_EQUAL((CARD32) 3, OFFSET(ParallelIOFaceGuam::ParallelIOCBType, port));
		CPPUNIT_ASSERT_EQUAL((CARD32) 4, OFFSET(ParallelIOFaceGuam::ParallelIOCBType, transferMode));
		CPPUNIT_ASSERT_EQUAL((CARD32) 5, OFFSET(ParallelIOFaceGuam::ParallelIOCBType, controlOrData));
		CPPUNIT_ASSERT_EQUAL((CARD32) 6, OFFSET(ParallelIOFaceGuam::ParallelIOCBType, nextIOCB));
		CPPUNIT_ASSERT_EQUAL((CARD32) 8, OFFSET(ParallelIOFaceGuam::ParallelIOCBType, status));
		CPPUNIT_ASSERT_EQUAL((CARD32) 9, OFFSET(ParallelIOFaceGuam::ParallelIOCBType, bytesTransferred));
	}

	// SerialIOFaceGuam
	void testSerialFaceFCBType() {
		//SerialFaceFCBType: TYPE = MACHINE DEPENDENT RECORD [
		//  txIOCB(0): SerialIOCBPtr,
		//  rxIOCB(2): SerialIOCBPtr,
		//  stopAgent(4): BOOLEAN,
		//  agentStopped(5): BOOLEAN,
		//  interruptSelector(6): GuamInputOutput.InterruptSelectorType,
		//  command(7): SerialCommand,
		//  parameterBlock(8): NewRS232CFace.ParameterRecord,
		//  controlFlags(15): NewRS232CFace.ControlRecord,
		//  deviceStatus(16): NewRS232CFace.DeviceStatus,
		//  commandStatus(17): NewRS232CFace.ParameterStatus,
		//  agentBlockSize(18): CARDINAL]; -- additional words in IOCB for agent
		CPPUNIT_ASSERT_EQUAL((CARD32)19, SIZE(SerialIOFaceGuam::SerialFaceFCBType));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(SerialIOFaceGuam::SerialFaceFCBType, txIOCB));
		CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(SerialIOFaceGuam::SerialFaceFCBType, rxIOCB));
		CPPUNIT_ASSERT_EQUAL((CARD32) 4, OFFSET(SerialIOFaceGuam::SerialFaceFCBType, stopAgent));
		CPPUNIT_ASSERT_EQUAL((CARD32) 5, OFFSET(SerialIOFaceGuam::SerialFaceFCBType, agentStopped));
		CPPUNIT_ASSERT_EQUAL((CARD32) 6, OFFSET(SerialIOFaceGuam::SerialFaceFCBType, interruptSelector));
		CPPUNIT_ASSERT_EQUAL((CARD32) 7, OFFSET(SerialIOFaceGuam::SerialFaceFCBType, command));
		CPPUNIT_ASSERT_EQUAL((CARD32) 8, OFFSET(SerialIOFaceGuam::SerialFaceFCBType, parameterBlock));
		CPPUNIT_ASSERT_EQUAL((CARD32)15, OFFSET(SerialIOFaceGuam::SerialFaceFCBType, controlFlags));
		CPPUNIT_ASSERT_EQUAL((CARD32)16, OFFSET(SerialIOFaceGuam::SerialFaceFCBType, deviceStatus));
		CPPUNIT_ASSERT_EQUAL((CARD32)17, OFFSET(SerialIOFaceGuam::SerialFaceFCBType, commandStatus));
		CPPUNIT_ASSERT_EQUAL((CARD32)18, OFFSET(SerialIOFaceGuam::SerialFaceFCBType, agentBlockSize));
	}
	void testSerialIOCBType() {
		//SerialIOCBType: TYPE = MACHINE DEPENDENT RECORD [
		//  operation(0): NewRS232CFace.Operation,
		//  dequeued(3:0..0): BOOLEAN,
		//  iocbType(3:1..15): {receive, transmit},
		//  actualLength(4): CARDINAL = 0,
		//  status(5): NewRS232CFace.TransferStatus = inProgress,
		//  nextIOCB(6): SerialIOCBPtr = NIL];
		CPPUNIT_ASSERT_EQUAL((CARD32) 8, SIZE(SerialIOFaceGuam::SerialIOCBType));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(SerialIOFaceGuam::SerialIOCBType, operation));
		CPPUNIT_ASSERT_EQUAL((CARD32) 3, OFFSET(SerialIOFaceGuam::SerialIOCBType, u3));
		CPPUNIT_ASSERT_EQUAL((CARD32) 4, OFFSET(SerialIOFaceGuam::SerialIOCBType, actualLength));
		CPPUNIT_ASSERT_EQUAL((CARD32) 5, OFFSET(SerialIOFaceGuam::SerialIOCBType, status));
		CPPUNIT_ASSERT_EQUAL((CARD32) 6, OFFSET(SerialIOFaceGuam::SerialIOCBType, nextIOCB));

		SerialIOFaceGuam::SerialIOCBType t;

		//  dequeued(3:0..0): BOOLEAN,
		t.u3 = 0;
		t.dequeued = ~t.dequeued;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x8000, t.u3);

		//  iocbType(3:1..15): {receive, transmit},
		t.u3 = 0;
		t.iocbType = ~t.iocbType;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x7fff, t.u3);
	}

	// TTYPortIOFaceGuam
	void testTTYFCBType() {
		//TTYFCBType: TYPE = MACHINE DEPENDENT RECORD [
		//  ttyInterruptSelector(0):	GuamInputOutput.InterruptSelectorType,
		//  command(1):			TTYCommandType,
		//  status(2):			TTYStatusType,
		//  data(3):			CHARACTER,
		//  -- parameters from Head to Agent
		//  lineSpeed(4):		LineSpeedType,
		//  charLength(5):		CharLengthType,
		//  parity(6):			ParityType,
		//  stopBits(7):		StopBitsType,
		//  CTS(8):			BOOLEAN,
		//  DSR(9):			BOOLEAN,
		//  -- device status from Agent to Head
		//  RTS(10):			BOOLEAN,
		//  DTR(11):			BOOLEAN,
		//  readyToReceive(12):		BOOLEAN,
		//  readyToTransmit(13):	BOOLEAN];
		CPPUNIT_ASSERT_EQUAL((CARD32)14, SIZE(TTYPortIOFaceGuam::TTYFCBType));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(TTYPortIOFaceGuam::TTYFCBType, ttyInterruptSelector));
		CPPUNIT_ASSERT_EQUAL((CARD32) 1, OFFSET(TTYPortIOFaceGuam::TTYFCBType, command));
		CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(TTYPortIOFaceGuam::TTYFCBType, status));
		CPPUNIT_ASSERT_EQUAL((CARD32) 3, OFFSET(TTYPortIOFaceGuam::TTYFCBType, data));

		CPPUNIT_ASSERT_EQUAL((CARD32) 4, OFFSET(TTYPortIOFaceGuam::TTYFCBType, lineSpeed));
		CPPUNIT_ASSERT_EQUAL((CARD32) 5, OFFSET(TTYPortIOFaceGuam::TTYFCBType, charLength));
		CPPUNIT_ASSERT_EQUAL((CARD32) 6, OFFSET(TTYPortIOFaceGuam::TTYFCBType, parity));
		CPPUNIT_ASSERT_EQUAL((CARD32) 7, OFFSET(TTYPortIOFaceGuam::TTYFCBType, stopBits));
		CPPUNIT_ASSERT_EQUAL((CARD32) 8, OFFSET(TTYPortIOFaceGuam::TTYFCBType, CTS));
		CPPUNIT_ASSERT_EQUAL((CARD32) 9, OFFSET(TTYPortIOFaceGuam::TTYFCBType, DSR));

		CPPUNIT_ASSERT_EQUAL((CARD32)10, OFFSET(TTYPortIOFaceGuam::TTYFCBType, RTS));
		CPPUNIT_ASSERT_EQUAL((CARD32)11, OFFSET(TTYPortIOFaceGuam::TTYFCBType, DTR));
		CPPUNIT_ASSERT_EQUAL((CARD32)12, OFFSET(TTYPortIOFaceGuam::TTYFCBType, readyToReceive));
		CPPUNIT_ASSERT_EQUAL((CARD32)13, OFFSET(TTYPortIOFaceGuam::TTYFCBType, readyToTransmit));
	}

	// BackingStore
	void testData_BackingStore() {
		//Data: TYPE = RECORD [
		//  data: ARRAY [0..7) OF UNSPECIFIED];
		//-- Describes backing storage of a run of virtual memory pages.
		//-- (Multiple coexisting implementations of this interface preclude the use of
		//--  an opaque type.)
		CPPUNIT_ASSERT_EQUAL((CARD32)7, SIZE(BackingStore::Data));
	}

	// VMDataInternal
	void testRun() {
		//Run: TYPE = RECORD [
		//  interval: VM.Interval,
		//  -- Attributes of the map unit containing this Run:
		//  usage: Space.Usage,  -- 11 bits.
		//  class: Space.Class,  -- 5 bits.
		//  transferProcID: TransferProcID,
		//  -- Data for this Run:
		//  startOfMapUnit: BOOLEAN,
		//  maybePartlyIn: BOOLEAN,
		//  endOfMapUnit: BOOLEAN,
		//  backingData: BackingStore.Data,
		//  -- swap unit structure of this Run:
		//  swapUnitData: SELECT swapUnits: Space.SwapUnitType FROM
		//    unitary => [swapUnitState: SwapUnitState],  --data is stored in run entry
		//    uniform => [uniformSwapUnitData: PUniformSwapUnitData],
		//    irregular => [irregularSwapUnitData: PIrregularSwapUnitData],
		//    ENDCASE];

		CPPUNIT_ASSERT_EQUAL((CARD32)14, SIZE(VMDataInternal::Run));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(VMDataInternal::Run, interval));
		CPPUNIT_ASSERT_EQUAL((CARD32) 4, OFFSET(VMDataInternal::Run, u4));
		CPPUNIT_ASSERT_EQUAL((CARD32) 5, OFFSET(VMDataInternal::Run, u5));
		CPPUNIT_ASSERT_EQUAL((CARD32) 6, OFFSET(VMDataInternal::Run, backingData));
		CPPUNIT_ASSERT_EQUAL((CARD32)13, OFFSET(VMDataInternal::Run, swapUnitData));
	}

	// VM
	void testInterval_VM() {
		//Interval: TYPE = RECORD [page: Environment.PageNumber, count: PageCount];
		CPPUNIT_ASSERT_EQUAL((CARD32)4, SIZE(VM::Interval));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(VM::Interval, page));
		CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(VM::Interval, count));
	}

	// DisplayFace
	void testCursor() {
		//Cursor: TYPE = ARRAY [0..16) OF WORD;
		CPPUNIT_ASSERT_EQUAL((CARD32)16, SIZE(DisplayFace::Cursor));
	}

	// ProcessorFaceExtras
	void testVersionResult() {
		//VersionResult: TYPE = MACHINE DEPENDENT RECORD [
		//  machineType (0: 0..3): MachineType,
		//  majorVersion (0: 4..7): [0..17B],  -- incremented by incompatible changes
		//  unused (0: 8..13): [0..77B],
		//  floatingPoint (0: 14..14): BOOLEAN,
		//  cedar (0: 15..15): BOOLEAN,
		//  releaseDate (1): CARDINAL];  -- days since January 1, 1901

		CPPUNIT_ASSERT_EQUAL((CARD32) 2, SIZE(ProcessorFaceExtras::VersionResult));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(ProcessorFaceExtras::VersionResult, u0));
		CPPUNIT_ASSERT_EQUAL((CARD32) 1, OFFSET(ProcessorFaceExtras::VersionResult, releaseDate));

		ProcessorFaceExtras::VersionResult t;

		//  machineType (0: 0..3): MachineType,
		t.u0 = 0;
		t.machineType = ~t.machineType;
		CPPUNIT_ASSERT_EQUAL((CARD16)0xf000, t.u0);

		//  majorVersion (0: 4..7): [0..17B],  -- incremented by incompatible changes
		t.u0 = 0;
		t.majorVersion = ~t.majorVersion;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0f00, t.u0);

		//  unused (0: 8..13): [0..77B],
		t.u0 = 0;
		t.unused = ~t.unused;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x00fc, t.u0);

		//  floatingPoint (0: 14..14): BOOLEAN,
		t.u0 = 0;
		t.floatingPoint = ~t.floatingPoint;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0002, t.u0);

		//  cedar (0: 15..15): BOOLEAN,
		t.u0 = 0;
		t.cedar = ~t.cedar;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0001, t.u0);
	}

	// PhysicalVolumeFormat
	void testSubVolumeDesc() {
		//  -- SubVolumeDesc as stored on disk in physical volume root page and in
		//  -- marker pages.
		//  SubVolumeDesc: TYPE = MACHINE DEPENDENT RECORD [
		//    lvID(0): System.VolumeID,
		//    lvSize(5): PageCount,
		//    lvPage(7): PageNumber,
		//    pvPage(11B): PageNumber,
		//    nPages(13B): PageCount];
		CPPUNIT_ASSERT_EQUAL((CARD32)13, SIZE(PhysicalVolumeFormat::SubVolumeDesc));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(PhysicalVolumeFormat::SubVolumeDesc, lvID));
		CPPUNIT_ASSERT_EQUAL((CARD32) 5, OFFSET(PhysicalVolumeFormat::SubVolumeDesc, lvSize));
		CPPUNIT_ASSERT_EQUAL((CARD32) 7, OFFSET(PhysicalVolumeFormat::SubVolumeDesc, lvPage));
		CPPUNIT_ASSERT_EQUAL((CARD32) 9, OFFSET(PhysicalVolumeFormat::SubVolumeDesc, pvPage));
		CPPUNIT_ASSERT_EQUAL((CARD32)11, OFFSET(PhysicalVolumeFormat::SubVolumeDesc, nPages));
	}
	void testDescriptor_PhysicalVolumeFormat() {
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
		CPPUNIT_ASSERT_EQUAL((CARD32)  256, SIZE(PhysicalVolumeFormat::Descriptor));
		CPPUNIT_ASSERT_EQUAL((CARD32)    0, OFFSET(PhysicalVolumeFormat::Descriptor, seal));
		CPPUNIT_ASSERT_EQUAL((CARD32)    1, OFFSET(PhysicalVolumeFormat::Descriptor, version));
		CPPUNIT_ASSERT_EQUAL((CARD32)    2, OFFSET(PhysicalVolumeFormat::Descriptor, labelLength));
		CPPUNIT_ASSERT_EQUAL((CARD32)    3, OFFSET(PhysicalVolumeFormat::Descriptor, pvID));
		CPPUNIT_ASSERT_EQUAL((CARD32)  010, OFFSET(PhysicalVolumeFormat::Descriptor, bootingInfo));
		CPPUNIT_ASSERT_EQUAL((CARD32)  054, OFFSET(PhysicalVolumeFormat::Descriptor, label));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0100, OFFSET(PhysicalVolumeFormat::Descriptor, subVolumeCount));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0101, OFFSET(PhysicalVolumeFormat::Descriptor, subVolumeMarkerID));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0106, OFFSET(PhysicalVolumeFormat::Descriptor, badPageCount));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0110, OFFSET(PhysicalVolumeFormat::Descriptor, maxBadPages));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0112, OFFSET(PhysicalVolumeFormat::Descriptor, dataLostPageCount));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0114, OFFSET(PhysicalVolumeFormat::Descriptor, maxDataLostPages));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0116, OFFSET(PhysicalVolumeFormat::Descriptor, onLineCount));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0117, OFFSET(PhysicalVolumeFormat::Descriptor, subVolumes));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0321, OFFSET(PhysicalVolumeFormat::Descriptor, fill));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0374, OFFSET(PhysicalVolumeFormat::Descriptor, localTimeParametersValid));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0375, OFFSET(PhysicalVolumeFormat::Descriptor, localTimeParameters));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0377, OFFSET(PhysicalVolumeFormat::Descriptor, checksum));
	}

	// LogicalVolumeFormat
	void testVolumeLock() {
		//VolumeLock: TYPE = MACHINE DEPENDENT RECORD [
		//  owner (0): Volume.ID � Volume.nullID,
		//  lock (5:0..15): SELECT stateTag (5:0..1): LockState FROM
		//    unlocked => [null (5:2..15): [0..16384) � 0],
		//    write => [null (5:2..15): [0..16384) � 0],
		//    read => [count (5:2..5): [0..16) � 0, null (5:6..15): [0..1024) � 0],
		//  ENDCASE];
		CPPUNIT_ASSERT_EQUAL((CARD32)    6, SIZE(LogicalVolumeFormat::VolumeLock));
		CPPUNIT_ASSERT_EQUAL((CARD32)    0, OFFSET(LogicalVolumeFormat::VolumeLock, owner));
		CPPUNIT_ASSERT_EQUAL((CARD32)    5, OFFSET(LogicalVolumeFormat::VolumeLock, u5));

		LogicalVolumeFormat::VolumeLock t;

		//  lock (5:0..15): SELECT stateTag (5:0..1): LockState FROM
		t.u5 = 0;
		t.stateTag = ~t.stateTag;
		CPPUNIT_ASSERT_EQUAL((CARD16)0xc000, t.u5);

		//    read => [count (5:2..5): [0..16) � 0, null (5:6..15): [0..1024) � 0],
		t.u5 = 0;
		t.count = ~t.count;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x3c00, t.u5);

		t.u5 = 0;
		t.null = ~t.null;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x03ff, t.u5);
	}
	void testDescriptor_LogicalVolumeFormat() {
		//Descriptor: TYPE = MACHINE DEPENDENT RECORD [
		//  seal(0):          CARDINAL  -- absolutely must be 1st field
		//  version(1):       CARDINAL  -- absolutely must be 2nd field
		//  vID(2):           Volume.ID,
		//  labelLength(7):   CARDINAL [0..maxLogicalVolumeLabelLength] � 0,
		//  label(10B):       LabelArray � nullName,
		//  type(34B):        Volume.Type,
		//  volumeSize(35B):  Volume.PageCount,
		//  bootingInfo(37B): Boot.LVBootFiles � nullBoot,
		//  pad(125B):	      CARDINAL � 0,
		//  pad1(126B:0..14):  [0..77777B] � 0,
		//  changing(126B:15..15): BOOLEAN � TRUE,  -- see documentation for semantics.
		//  freePageCount(127B):       Volume.PageCount � 0,
		//  vamStart(131B):            Volume.PageNumber � 1,
		//  primaryETableStartPage(133B): Volume.PageNumber � 0,
		//  copyETableStartPage(135B): Volume.PageNumber � 0,
		//  lowerBound(137B):          Volume.PageNumber
		//  volumeRootDirectory(141B): File.ID � File.nullID,
		//  rootFileID(143B):          ARRAY PilotFileTypes.PilotRootFileType OF File.ID
		//  lastIDAllocated(165B):     LONG CARDINAL
		//  scavengerLogVolume(167B):  Volume.ID
		//  lastTimeOpendForWrite(174B):         System.GreenwichMeanTime �
		//  statusPrimaryETableFile(176B:0..7):  [0..400B) � 0,
		//  statusCopyETableFile(176B:8..15):    [0..400B) � 0,
		//  numberOfFile(177B):        LONG CARDINAL � 0,
		//  lock(201B): VolumeLock,
		//  numberOfBadPagesLastTime(207B): Volume.PageCount � 0,
		//  fill(211B):                ARRAY [0..377B - 211B) OF WORD � ALL[0],
		//  checksum(377B):            CARDINAL � 0];  -- MUST be the last field
		CPPUNIT_ASSERT_EQUAL((CARD32)  256, SIZE(LogicalVolumeFormat::Descriptor));
		CPPUNIT_ASSERT_EQUAL((CARD32)    0, OFFSET(LogicalVolumeFormat::Descriptor, seal));
		CPPUNIT_ASSERT_EQUAL((CARD32)    1, OFFSET(LogicalVolumeFormat::Descriptor, version));
		CPPUNIT_ASSERT_EQUAL((CARD32)    2, OFFSET(LogicalVolumeFormat::Descriptor, vID));
		CPPUNIT_ASSERT_EQUAL((CARD32)    7, OFFSET(LogicalVolumeFormat::Descriptor, labelLength));
		CPPUNIT_ASSERT_EQUAL((CARD32)  010, OFFSET(LogicalVolumeFormat::Descriptor, label));
		CPPUNIT_ASSERT_EQUAL((CARD32)  034, OFFSET(LogicalVolumeFormat::Descriptor, type));
		CPPUNIT_ASSERT_EQUAL((CARD32)  035, OFFSET(LogicalVolumeFormat::Descriptor, volumeSize));
		CPPUNIT_ASSERT_EQUAL((CARD32)  037, OFFSET(LogicalVolumeFormat::Descriptor, bootingInfo));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0125, OFFSET(LogicalVolumeFormat::Descriptor, pad));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0126, OFFSET(LogicalVolumeFormat::Descriptor, u126));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0127, OFFSET(LogicalVolumeFormat::Descriptor, freePageCount));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0131, OFFSET(LogicalVolumeFormat::Descriptor, vamStart));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0133, OFFSET(LogicalVolumeFormat::Descriptor, primaryETableStartPage));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0135, OFFSET(LogicalVolumeFormat::Descriptor, copyETableStartPage));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0137, OFFSET(LogicalVolumeFormat::Descriptor, lowerBound));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0141, OFFSET(LogicalVolumeFormat::Descriptor, volumeRootDirectory));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0143, OFFSET(LogicalVolumeFormat::Descriptor, rootFileID));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0165, OFFSET(LogicalVolumeFormat::Descriptor, lastIDAllocated));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0167, OFFSET(LogicalVolumeFormat::Descriptor, scavengerLogVolume));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0174, OFFSET(LogicalVolumeFormat::Descriptor, lastTimeOpendForWrite));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0176, OFFSET(LogicalVolumeFormat::Descriptor, u176));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0177, OFFSET(LogicalVolumeFormat::Descriptor, numberOfFile));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0201, OFFSET(LogicalVolumeFormat::Descriptor, lock));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0207, OFFSET(LogicalVolumeFormat::Descriptor, numberOfBadPagesLastTime));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0211, OFFSET(LogicalVolumeFormat::Descriptor, fill));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0377, OFFSET(LogicalVolumeFormat::Descriptor, checksum));

		LogicalVolumeFormat::Descriptor t;

		//  pad1(126B:0..14):  [0..77777B] � 0,
		t.u126 = 0;
		t.pad1 = ~t.pad1;
		CPPUNIT_ASSERT_EQUAL((CARD16)0xfffe, t.u126);

		//  changing(126B:15..15): BOOLEAN � TRUE,  -- see documentation for semantics.
		t.u126 = 0;
		t.changing = ~t.changing;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0001, t.u126);

		//  statusPrimaryETableFile(176B:0..7):  [0..400B) � 0,
		t.u176 = 0;
		t.statusPrimaryETableFile = ~t.statusPrimaryETableFile;
		CPPUNIT_ASSERT_EQUAL((CARD16)0xff00, t.u176);

		//  statusCopyETableFile(176B:8..15):    [0..400B) � 0,
		t.u176 = 0;
		t.statusCopyETableFile = ~t.statusCopyETableFile;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x00ff, t.u176);
	}

	// ColorBlt
	void testAddress_ColorBlt() {
		//Address: TYPE = MACHINE DEPENDENT RECORD [
		//  word: LONG POINTER,
		//  pixel: CARDINAL];
		CPPUNIT_ASSERT_EQUAL((CARD32)  3, SIZE(ColorBlt::Address));
		CPPUNIT_ASSERT_EQUAL((CARD32)  0, OFFSET(ColorBlt::Address, word));
		CPPUNIT_ASSERT_EQUAL((CARD32)  2, OFFSET(ColorBlt::Address, pixel));
	}
	void testPatternParm_ColorBlt() {
		//PatternParm: TYPE = MACHINE DEPENDENT RECORD [
		//  unpacked(0:0..3): NIBBLE = 0,
		//  yOffset(0:4..7): NIBBLE,
		//  widthMinusOne(0:8..11): NIBBLE,
		//  heightMinusOne(0:12..15): NIBBLE];
		CPPUNIT_ASSERT_EQUAL((CARD32)  1, SIZE(ColorBlt::PatternParm));

		ColorBlt::PatternParm t;

		t.u = 0;
		t.unpacked = ~t.unpacked;
		CPPUNIT_ASSERT_EQUAL((CARD16)0xf000, t.u);

		t.u = 0;
		t.yOffset = ~t.yOffset;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0f00, t.u);

		t.u = 0;
		t.widthMinusOne = ~t.widthMinusOne;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x00f0, t.u);

		t.u = 0;
		t.heightMinusOne = ~t.heightMinusOne;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x000f, t.u);
}
	void testColorBltFlags() {
		//ColorBltFlags: TYPE = MACHINE DEPENDENT RECORD [
		//  -- determines the ColorBlt function
		//  direction: Direction � forward,
		//  srcType: PixelType � bit,
		//  dstType: PixelType � bit,
		//  pattern: BOOLEAN � FALSE,
		//  srcFunc: SrcFunc � null,
		//  dstFunc: DstFunc � src,
		//  reserved: [0..255] � 0];
		CPPUNIT_ASSERT_EQUAL((CARD32)  1, SIZE(ColorBlt::ColorBltFlags));

		ColorBlt::ColorBltFlags t;

		t.u = 0;
		t.direction = ~t.direction;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x8000, t.u);

		t.u = 0;
		t.srcType = ~t.srcType;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x4000, t.u);

		t.u = 0;
		t.dstType = ~t.dstType;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x2000, t.u);

		t.u = 0;
		t.pattern = ~t.pattern;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x1000, t.u);

		t.u = 0;
		t.srcFunc = ~t.srcFunc;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0800, t.u);

		t.u = 0;
		t.dstFunc = ~t.dstFunc;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0700, t.u);

		t.u = 0;
		t.reserved = ~t.reserved;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x00ff, t.u);
	}
	void testColorMapping() {
		//ColorMapping: TYPE = ARRAY [0..1] OF Color;
		CPPUNIT_ASSERT_EQUAL((CARD32)  2, SIZE(ColorBlt::ColorMapping));
	}
	void testColorBltTable() {
		//SrcDesc: TYPE = MACHINE DEPENDENT RECORD [
		//  SELECT OVERLAID * FROM
		//    pattern => [pattern: PatternParm],
		//    srcPpl => [srcPpl: INTEGER],
		//    ENDCASE];

		//CBTable, ColorBltTable: TYPE = MACHINE DEPENDENT RECORD [
		//  dst: Address,
		//  dstPpl: INTEGER,
		//  src: Address,
		//  srcDesc: SrcDesc,
		//  width: CARDINAL,
		//  height: CARDINAL,
		//  flags: ColorBltFlags,
		//  colorMapping: ColorMapping];
		CPPUNIT_ASSERT_EQUAL((CARD32) 13, SIZE(ColorBlt::ColorBltTable));
		CPPUNIT_ASSERT_EQUAL((CARD32)  0, OFFSET(ColorBlt::ColorBltTable, dst));
		CPPUNIT_ASSERT_EQUAL((CARD32)  3, OFFSET(ColorBlt::ColorBltTable, dstPpl));
		CPPUNIT_ASSERT_EQUAL((CARD32)  4, OFFSET(ColorBlt::ColorBltTable, src));
		CPPUNIT_ASSERT_EQUAL((CARD32)  7, OFFSET(ColorBlt::ColorBltTable, srcPpl));
		CPPUNIT_ASSERT_EQUAL((CARD32)  8, OFFSET(ColorBlt::ColorBltTable, width));
		CPPUNIT_ASSERT_EQUAL((CARD32)  9, OFFSET(ColorBlt::ColorBltTable, height));
		CPPUNIT_ASSERT_EQUAL((CARD32) 10, OFFSET(ColorBlt::ColorBltTable, flags));
		CPPUNIT_ASSERT_EQUAL((CARD32) 11, OFFSET(ColorBlt::ColorBltTable, colorMapping));
	}

	void testModuleInfo() {
		//ModuleInfo: TYPE = MACHINE DEPENDENT RECORD [
		//  resolved(0:0..0): BOOLEAN,
		//  cgfi(0:1..15): NATURAL,
		//  index(1): Index,
		//  globalFrame(2): SELECT OVERLAID * FROM
		//    oldWorld => [gf(2): PrincOps.GlobalFrameHandle],
		//    newWorld => [gfi(2): PrincOpsExtras2.GFTHandle],
		//    ENDCASE];
		CPPUNIT_ASSERT_EQUAL((CARD32) 3, SIZE(LoadStateFormat::ModuleInfo));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(LoadStateFormat::ModuleInfo, u0));
		CPPUNIT_ASSERT_EQUAL((CARD32) 1, OFFSET(LoadStateFormat::ModuleInfo, index));
		CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(LoadStateFormat::ModuleInfo, globalFrame));

		LoadStateFormat::ModuleInfo t;

		//  resolved(0:0..0): BOOLEAN,
		t.u0 = 0;
		t.resolved = ~t.resolved;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x8000, t.u0);

		//  cgfi(0:1..15): NATURAL,
		t.u0 = 0;
		t.cgfi = ~t.cgfi;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x7FFF, t.u0);
	}
	void testBcdInfo() {
		//BcdInfo: TYPE = MACHINE DEPENDENT RECORD [
		//  exports(0:0..0), typeExported(0:1..1): BOOLEAN,
		//  pages(0:2..15): [1..256],
		//  base(1): LONG BASE POINTER TO BcdDefs.BCD,
		//  id(3): ID];
		CPPUNIT_ASSERT_EQUAL((CARD32) 4, SIZE(LoadStateFormat::BcdInfo));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(LoadStateFormat::BcdInfo, u0));
		CPPUNIT_ASSERT_EQUAL((CARD32) 1, OFFSET(LoadStateFormat::BcdInfo, base));
		CPPUNIT_ASSERT_EQUAL((CARD32) 3, OFFSET(LoadStateFormat::BcdInfo, id));

		LoadStateFormat::BcdInfo t;

		//  exports(0:0..0),
		t.u0 = 0;
		t.exports = ~t.exports;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x8000, t.u0);

		//  typeExported(0:1..1),
		t.u0 = 0;
		t.typeExported = ~t.typeExported;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x4000, t.u0);

		//  pages(0:2..15): [1..256]
		t.u0 = 0;
		t.pages = ~t.pages;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x3FFF, t.u0);
	}
	void testObject() {
		//Object: TYPE = MACHINE DEPENDENT RECORD [
		//  versionident: CARDINAL,
		//  nModules: CARDINAL,
		//  maxModules: CARDINAL,
		//  nBcds: CARDINAL,
		//  maxBcds: CARDINAL,
		//  nextID: ID,
		//  moduleInfo: ModuleInfoIndex,
		//  bcdInfo: BcdInfoIndex];
		CPPUNIT_ASSERT_EQUAL((CARD32) 8, SIZE(LoadStateFormat::Object));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(LoadStateFormat::Object, versionident));
		CPPUNIT_ASSERT_EQUAL((CARD32) 1, OFFSET(LoadStateFormat::Object, nModules));
		CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(LoadStateFormat::Object, maxModules));
		CPPUNIT_ASSERT_EQUAL((CARD32) 3, OFFSET(LoadStateFormat::Object, nBcds));
		CPPUNIT_ASSERT_EQUAL((CARD32) 4, OFFSET(LoadStateFormat::Object, maxBcds));
		CPPUNIT_ASSERT_EQUAL((CARD32) 5, OFFSET(LoadStateFormat::Object, nextID));
		CPPUNIT_ASSERT_EQUAL((CARD32) 6, OFFSET(LoadStateFormat::Object, moduleInfo));
		CPPUNIT_ASSERT_EQUAL((CARD32) 7, OFFSET(LoadStateFormat::Object, bcdInfo));
	}
	void testSwapInfo() {
		//SwapInfo: TYPE = MACHINE DEPENDENT RECORD [
		//  -- This is the actual usage of PSB.PDA.available.
		//  externalStateVector(0): LONG POINTER TO ExternalStateVector,
		//  availableC(2), availableD(3), availableE(4): WORD];
		CPPUNIT_ASSERT_EQUAL((CARD32) 5, SIZE(CPSwapDefs::SwapInfo));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(CPSwapDefs::SwapInfo, externalStateVector));
		CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(CPSwapDefs::SwapInfo, availableC));
		CPPUNIT_ASSERT_EQUAL((CARD32) 3, OFFSET(CPSwapDefs::SwapInfo, availableD));
		CPPUNIT_ASSERT_EQUAL((CARD32) 4, OFFSET(CPSwapDefs::SwapInfo, availableE));
	}
	void testExternalStateVector() {
		//ExternalStateVector: TYPE = MACHINE DEPENDENT RECORD [
		//	-- Items describing the boot session in general:
		//	version(0): CARDINAL,  -- version of CPSwapDefs. MUST BE FIRST.
		//	bootSession(1): LONG CARDINAL,  -- random number identifying boot session.
		//	    loadState(3): LONG POINTER TO LoadStateFormat.Object,
		//	    mapLog(5): LONG POINTER TO VMMapLog.Descriptor,
		//	    patchTable(7): LONG POINTER TO PatchTable,
		//	    breakBlocks(9): LONG POINTER TO BBArray,
		//	    breakpointHandlers(11): LONG POINTER TO BreakpointHandlerItem,
		//	    mds(13): CARDINAL,  -- MDS of *all* processes.
		//	    faultsBeingProcessed(14): ARRAY FaultType OF FaultQueuePtr,
		//	    systemVolumeID(20): Volume.ID,  -- (nullID if none or not known yet)
		//	    virtualMemoryCount(25): Environment.PageCount,
		//	    patchTableInUse(27:0..12): BOOLEAN,  -- (in use by debuggee)
		//	    breakBlocksInUse(27:13..13): BOOLEAN,  -- (in use by debuggee)
		//	    loadStateChanging(27:14..14): BOOLEAN,
		//	    loadStateDirty(27:15..15): BOOLEAN,  -- set by debuggee, reset by debugger
		//	    -- Items describing the current trip to or from the debugger:
		//	    swapData(28): SwapData,
		//	    spareA(48): WORD,
		//	    spareB(49): WORD];
		CPPUNIT_ASSERT_EQUAL((CARD32) 50, SIZE(CPSwapDefs::ExternalStateVector));
		CPPUNIT_ASSERT_EQUAL((CARD32)  0, OFFSET(CPSwapDefs::ExternalStateVector, version));
		CPPUNIT_ASSERT_EQUAL((CARD32)  1, OFFSET(CPSwapDefs::ExternalStateVector, bootSession));
		CPPUNIT_ASSERT_EQUAL((CARD32)  3, OFFSET(CPSwapDefs::ExternalStateVector, loadState));
		CPPUNIT_ASSERT_EQUAL((CARD32)  5, OFFSET(CPSwapDefs::ExternalStateVector, mapLog));
		CPPUNIT_ASSERT_EQUAL((CARD32)  7, OFFSET(CPSwapDefs::ExternalStateVector, patchTable));
		CPPUNIT_ASSERT_EQUAL((CARD32)  9, OFFSET(CPSwapDefs::ExternalStateVector, breakBlocks));
		CPPUNIT_ASSERT_EQUAL((CARD32) 11, OFFSET(CPSwapDefs::ExternalStateVector, breakpointHandlers));
		CPPUNIT_ASSERT_EQUAL((CARD32) 13, OFFSET(CPSwapDefs::ExternalStateVector, mds));
		CPPUNIT_ASSERT_EQUAL((CARD32) 14, OFFSET(CPSwapDefs::ExternalStateVector, faultsBeingProcessed));
		CPPUNIT_ASSERT_EQUAL((CARD32) 20, OFFSET(CPSwapDefs::ExternalStateVector, systemVolumeID));
		CPPUNIT_ASSERT_EQUAL((CARD32) 25, OFFSET(CPSwapDefs::ExternalStateVector, virtualMemoryCount));
		CPPUNIT_ASSERT_EQUAL((CARD32) 27, OFFSET(CPSwapDefs::ExternalStateVector, u27));
		CPPUNIT_ASSERT_EQUAL((CARD32) 28, OFFSET(CPSwapDefs::ExternalStateVector, swapData));
		CPPUNIT_ASSERT_EQUAL((CARD32) 48, OFFSET(CPSwapDefs::ExternalStateVector, spareA));
		CPPUNIT_ASSERT_EQUAL((CARD32) 49, OFFSET(CPSwapDefs::ExternalStateVector, spareB));

		CPSwapDefs::ExternalStateVector t;

		//  patchTableInUse(27:0..12): BOOLEAN
		t.u27 = 0;
		t.patchTableInUse = ~t.patchTableInUse;
		CPPUNIT_ASSERT_EQUAL((CARD16)0xFFF8, t.u27);

		//  breakBlocksInUse(27:13..13): BOOLEAN
		t.u27 = 0;
		t.breakBlocksInUse = ~t.breakBlocksInUse;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0004, t.u27);

		//  loadStateChanging(27:14..14): BOOLEAN,
		t.u27 = 0;
		t.loadStateChanging = ~t.loadStateChanging;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0002, t.u27);

		//  loadStateDirty(27:15..15): BOOLEAN
		t.u27 = 0;
		t.loadStateDirty = ~t.loadStateDirty;
		CPPUNIT_ASSERT_EQUAL((CARD16)0x0001, t.u27);
	}
	void testGFTItem() {
		//  GFTItem: TYPE = MACHINE DEPENDENT RECORD [
		//	 SELECT OVERLAID * FROM
		//	   inuse => [
		//		 globalFrame: LongGlobalFrameHandle,
		//		 codebase: PrincOps.GlobalCodebase],
		//	   free => [
		//		 next: GFTHandle,
		//	     fill0: CARDINAL � 0,
		//	     nullCodebase: LONG CARDINAL � 0]
		//	   ENDCASE];
		CPPUNIT_ASSERT_EQUAL((CARD32) 4, SIZE(PrincOpsExtras2::GFTItem));
		CPPUNIT_ASSERT_EQUAL((CARD32) 0, OFFSET(PrincOpsExtras2::GFTItem, globalFrame));
		CPPUNIT_ASSERT_EQUAL((CARD32) 2, OFFSET(PrincOpsExtras2::GFTItem, codebase));
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(testPilot);
