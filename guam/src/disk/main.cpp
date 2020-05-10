#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("disk");

#include "../mesa/MesaBasic.h"
#include "../agent/DiskFile.h"

#include "../util/ByteBuffer.h"

#include <QtCore>

class MyDisk {
public:
	MyDisk(const char* path) {
		diskFile.attach(path);
	}
	~MyDisk() {
		diskFile.detach();
	}

	void readSector(int sector, ByteBuffer& bb, quint32 count) {
		if (sector < 0) {
			logger.fatal("sector = %d", sector);
			ERROR();
		}
		quint8* data = bb.getData();
		for(quint32 i = 0; i < count; i++) {
			diskFile.readPage(sector + i, (CARD16*)(data), PageSize);
			data += Environment::bytesPerPage;
		}

		bb.setPos(Environment::bytesPerPage * count);
		bb.rewind();
	}
	void readSector(int sector, ByteBuffer& bb) {
		readSector(sector, bb, 1);
	}

private:
	DiskFile   diskFile;
};


//
//
//
struct My {
	//
	// From APilot/15.0.1/Pilot/Public/System.mesa
	//
	struct System {
		// UniversalID: TYPE [5];
		struct UniversalID {
			CARD16 a, b, c, d, e;
		};
	};

	//
	// From APilot/15.0.1/Pilot/Friends/PilotDisk.mesa
	//
	struct Disk {
		//FileID: TYPE = MACHINE DEPENDENT RECORD [
		//  name(0): SELECT OVERLAID * FROM
		//    volumeRelative => [
		//      fileID(0): File.ID, pad(2): ARRAY [0..3) OF WORD � ALL[0]],
		//    unique => [id(0): System.UniversalID]
		//    ENDCASE];
		union FileID {
			struct {
				CARD32 fileID;
				CARD16 pad[3];
			} __attribute__((packed));
			System::UniversalID id;
		};
	};

	//
	// From APilot/15.3/Faces/Friends/PilotDiskFace.mesa
	//
	struct PilotDiskFace {
		//DiskAddress: TYPE = MACHINE DEPENDENT RECORD [
		//  cylinder(0): CARDINAL, head(1:0..7): [0..256), sector(1:8..15): [0..256)];
		struct DiskAddress {
			CARD16 cylinder;
			union {
				struct {
					CARD16 sector : 8;
					CARD16 head   : 8;
				};
				CARD16 u1;
			};
		};
	};

	//
	// From APilot/15.0.1/Pilot/Friends/Boot.mesa
	//
	struct Boot {
		//DiskFileID: TYPE = MACHINE DEPENDENT RECORD [
		//  fID (0): PilotDisk.FileID,
		//  firstPage (5): PilotDisk.FilePageNumber,
		//  da (7): PilotDiskFace.DiskAddress];
		struct DiskFileID {
			Disk::FileID               fID;
			CARD32                     firstPage;
			PilotDiskFace::DiskAddress da;
		};

		//-- Types of boot files pointed to from root pages of physical and logical
		//-- volumes, and delivered by boot servers:
		//-- The following cannot be changed without invalidating all Pilot volumes.
		//-- Known to the initial microcode.
		//BootFileType: TYPE = MACHINE DEPENDENT{
		//  hardMicrocode (0), softMicrocode (1), germ (2), pilot (3), debugger (4),
		//  debuggee (5), initialMicrocode (6)};
		static const CARD16 BFT_hardMicrocode    = 0;
		static const CARD16 BFT_softMicrocode    = 1;
		static const CARD16 BFT_germ             = 2;
		static const CARD16 BFT_pilot            = 3;
		static const CARD16 BFT_debugger         = 4;
		static const CARD16 BFT_debuggee         = 5;
		static const CARD16 BFT_initialMicrocode = 6;

		//PVBootFiles: TYPE = ARRAY BootFileType [hardMicrocode..pilot] OF DiskFileID;
		struct PVBootFiles {
			DiskFileID bootFile[4];  // 0..3 -- hardMicrocode..pilot
		};
	};

	//
	// APilot/15.0.1/Pilot/Private/PhysicalVolumeFormat.mesa
	//
	struct PhysicalVolume {
		//  seal, Seal: CARDINAL = 121212B;
		static const CARD16 seal = 0121212;

		//  currentVersion: CARDINAL = 7;
		static const CARD16 currentVersion = 7;

		//  maxSubVols: CARDINAL [10..10] = 10;
		static const CARD16 maxSubVols = 10;

		//  -- SubVolumeDesc as stored on disk in physical volume root page and in
		//  -- marker pages.
		//  SubVolumeDesc: TYPE = MACHINE DEPENDENT RECORD [
		//    lvID(0): System.VolumeID,
		//    lvSize(5): PageCount,
		//    lvPage(7): PageNumber,
		//    pvPage(11B): PageNumber,
		//    nPages(13B): PageCount];
		struct SubVolumeDesc {
			System::UniversalID lvID;
			CARD32 lvSize;
			CARD32 lvPage;
			CARD32 pvPage;
			CARD32 nPages;
		};

		//  physicalVolumeLabelLength: CARDINAL = 40;
		//
		//  LabelArray: TYPE = PACKED ARRAY [0..physicalVolumeLabelLength) OF CHARACTER;
		static const CARD16 physicalVolumeLabelLength = 40;

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
		struct Descriptor {
			CARD16              seal;
			CARD16              version;
			CARD16              labelLength;
			System::UniversalID pvID;
			Boot::PVBootFiles   bootingInfo;
			CARD8               label[physicalVolumeLabelLength];
			CARD16              subVolumeCount;
			System::UniversalID subVolumeMarkerID;
			CARD32              badPageCount;
			CARD32              maxBadPages;
			CARD32              dataLostPageCount;
			CARD32              maxDataLostPages;
			CARD16              onLineCount;
			SubVolumeDesc       subVolumes[maxSubVols];
			CARD16              fill[43];
			CARD16              localTimeParametersValid;
			CARD32              localTimeParameters;
			CARD16              checksum;
		};
	};
};

static void read(ByteBuffer& bb, CARD8& value) {
	value = bb.get8();
}
static void read(ByteBuffer& bb, CARD16& value) {
	value = bb.get16();
}
static void read(ByteBuffer& bb, CARD32& value) {
	value = bb.get32();
}

static void read(ByteBuffer& bb, My::System::UniversalID& value) {
	read(bb, value.a);
	read(bb, value.b);
	read(bb, value.c);
	read(bb, value.d);
	read(bb, value.e);
}

static void read(ByteBuffer& bb, My::Disk::FileID& value) {
	read(bb, value.id);
}
static void read(ByteBuffer& bb, My::PilotDiskFace::DiskAddress& value) {
	read(bb, value.cylinder);
	read(bb, value.u1);
}

static void read(ByteBuffer& bb, My::Boot::DiskFileID& value) {
	read(bb, value.fID);
	read(bb, value.firstPage);
	read(bb, value.da);
}

static void read(ByteBuffer& bb, My::Boot::PVBootFiles& value) {
	for(CARD32 i = 0; i < ELEMENTSOF(value.bootFile); i++) {
		read(bb, value.bootFile[i]);
	}
}

static void read(ByteBuffer& bb, My::PhysicalVolume::SubVolumeDesc& value) {
	read(bb, value.lvID);
	read(bb, value.lvSize);
	read(bb, value.lvPage);
	read(bb, value.pvPage);
	read(bb, value.nPages);
}

static void read(ByteBuffer& bb, My::PhysicalVolume::Descriptor& value) {
	read(bb, value.seal);
	read(bb, value.version);
	read(bb, value.labelLength);
	read(bb, value.pvID);
	read(bb, value.bootingInfo);
	for(CARD32 i = 0; i < ELEMENTSOF(value.label); i++) {
		read(bb, value.label[i]);
	}
	read(bb, value.subVolumeCount);
	read(bb, value.subVolumeMarkerID);
	read(bb, value.badPageCount);
	read(bb, value.maxBadPages);
	read(bb, value.dataLostPageCount);
	read(bb, value.maxDataLostPages);
	read(bb, value.onLineCount);
	for(CARD32 i = 0; i < ELEMENTSOF(value.subVolumes); i++) {
		read(bb, value.subVolumes[i]);
	}
	for(CARD32 i = 0; i < ELEMENTSOF(value.fill); i++) {
		read(bb, value.fill[i]);
	}
	read(bb, value.localTimeParametersValid);
	read(bb, value.localTimeParameters);
	read(bb, value.checksum);

	// Sanity check
	if (bb.getPos() != 512) {
		logger.fatal("pos expect %6o  actual %6o", 512, bb.getPos());
		ERROR();
	}

	if (value.seal != My::PhysicalVolume::seal) {
		logger.fatal("seal expect %6o  actual %6o", My::PhysicalVolume::seal, value.seal);
		ERROR();
	}
	if (value.version != My::PhysicalVolume::currentVersion) {
		logger.fatal("version expect %6o  actual %6o", My::PhysicalVolume::currentVersion, value.version);
		ERROR();
	}
}

static QString toQString(My::System::UniversalID& value) {
	return QString("%1-%2-%3-%4-%5").
			arg(value.a, 4, 16, QChar('0')).
			arg(value.b, 4, 16, QChar('0')).
			arg(value.c, 4, 16, QChar('0')).
			arg(value.d, 4, 16, QChar('0')).
			arg(value.e, 4, 16, QChar('0')).toUpper();
}
static QString toQString(My::Disk::FileID& value) {
	if (value.pad[0] == 0 && value.pad[1] == 0 && value.pad[2] == 0 && value.fileID != 0) {
		return QString("%1").arg(value.fileID, 8, 16, QChar('0')).toUpper();
	} else {
		return toQString(value.id);
	}
}
static QString toQString(My::PilotDiskFace::DiskAddress& value) {
	return QString("%1-%2-%3").
			arg(value.cylinder, 4, 16, QChar('0')).
			arg(value.head,     2, 16, QChar('0')).
			arg(value.sector,   2, 16, QChar('0')).toUpper();
}
static bool isNull(My::System::UniversalID& value) {
	return value.a == 0 && value.b == 0 && value.c == 0 && value.d == 0 && value.e == 0;
}
static bool isNull(My::Disk::FileID& value) {
	return isNull(value.id);
}
static bool isNull(My::PilotDiskFace::DiskAddress& value) {
	return value.cylinder == 0 && value.head == 0 && value.sector == 0;
}
static bool isNull(My::Boot::DiskFileID& value) {
	return isNull(value.fID) && value.firstPage == 0 && isNull(value.da);
}
QString toQString(My::Boot::DiskFileID& value) {
	if (isNull(value)) {
		return QString("NULL");
	} else {
		QString fID       = toQString(value.fID);
		QString firstPage = QString::number(value.firstPage, 16);
		QString da        = toQString(value.da);

		return QString("{%1 %2 %3}").arg(fID, firstPage, da);
	}
}
QString toQString(My::PhysicalVolume::SubVolumeDesc& value) {
	return QString("{%1 %2 %3 %4 %5}").
			arg(toQString(value.lvID)).
			arg(value.lvSize, 6).
			arg(value.lvPage, 6).
			arg(value.pvPage, 6).
			arg(value.nPages, 6).toUpper();
}

static void dump(My::PhysicalVolume::Descriptor& value) {
	logger.info("seal                     %o", value.seal);
	logger.info("version                  %d", value.version);
	logger.info("pvID                     %s", toQString(value.pvID).toLocal8Bit().constData());

	logger.info("bootinInfo");
	if (!isNull(value.bootingInfo.bootFile[My::Boot::BFT_hardMicrocode])) {
		logger.info("  hardMicrocode          %s", toQString(value.bootingInfo.bootFile[My::Boot::BFT_hardMicrocode]).toLocal8Bit().data());
	}
	if (!isNull(value.bootingInfo.bootFile[My::Boot::BFT_softMicrocode])) {
		logger.info("  softMicrocode          %s", toQString(value.bootingInfo.bootFile[My::Boot::BFT_softMicrocode]).toLocal8Bit().data());
	}
	if (!isNull(value.bootingInfo.bootFile[My::Boot::BFT_germ])) {
		logger.info("  germ                   %s", toQString(value.bootingInfo.bootFile[My::Boot::BFT_germ]).toLocal8Bit().data());
	}
	if (!isNull(value.bootingInfo.bootFile[My::Boot::BFT_pilot])) {
		logger.info("  pilot                  %s", toQString(value.bootingInfo.bootFile[My::Boot::BFT_pilot]).toLocal8Bit().data());
	}

	QString label;
	{
		for(CARD16 i = 0; i < value.labelLength; i++) {
			label.append(value.label[i]);
		}
	}
	logger.info("label                    %s", label.toLocal8Bit().data());

	logger.info("subVolumeCount           %d", value.subVolumeCount);
	logger.info("subVolumeMarkerID        %s", toQString(value.subVolumeMarkerID).toLocal8Bit().constData());
	logger.info("badPageCount             %d", value.badPageCount);
	logger.info("maxBadPages              %d", value.maxBadPages);
	logger.info("dataLostPageCount        %d", value.dataLostPageCount);
	logger.info("maxDataLostPages         %d", value.maxDataLostPages);
	logger.info("onLineCount              %d", value.onLineCount);

	logger.info("subVolume");
	for(CARD16 i = 0; i < value.subVolumeCount; i++) {
		logger.info("                         %s", toQString(value.subVolumes[i]).toLocal8Bit().data());
	}

	logger.info("localTimeParametersValid %d", value.localTimeParametersValid);
	logger.info("localTimeParameters      %d", value.localTimeParameters);
	logger.info("checksum                 %d", value.checksum);
}

int main(int /*argc*/, char** /*argv*/) {
	const char* pilotDiskImagePath   = "data/GVWin/GVWIN001.DSK";

	logger.info("pilotDiskImagePath   = %s", pilotDiskImagePath);
	MyDisk disk(pilotDiskImagePath);

	My::PhysicalVolume::Descriptor descriptor;

	{
		LittleEndianByteBuffer bb(512);
		disk.readSector(0, bb);
		read(bb, descriptor);
	}

	dump(descriptor);

	logger.info("checksum %04X", descriptor.checksum);



	return 0;
}

