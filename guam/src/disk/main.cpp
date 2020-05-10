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
	class System {
	public:
		// UniversalID: TYPE [5];
		class UniversalID {
		public:
			CARD16 a, b, c, d, e;

			void read(ByteBuffer& bb) {
				bb.read(a).read(b).read(c).read(d).read(e);
			}
			bool isNull() {
				return a == 0 && b == 0 && c == 0 && d == 0;
			}
			QString toQString() {
				if (isNull()) {
					return "NULL";
				} else {
					return QString("{%1-%2-%3-%4-%5}").
							arg(a, 4, 16, QChar('0')).
							arg(b, 4, 16, QChar('0')).
							arg(c, 4, 16, QChar('0')).
							arg(d, 4, 16, QChar('0')).
							arg(e, 4, 16, QChar('0')).toUpper();
				}
			}
		};
	};

	//
	// From APilot/15.0.1/Pilot/Friends/PilotDisk.mesa
	//
	class Disk {
	public:
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

			void read(ByteBuffer& bb) {
				id.read(bb);
			}
			bool inNull() {
				return id.isNull();
			}
			QString toQString() {
				if (pad[0] == 0 && pad[1] == 0 && pad[2] == 0 && fileID != 0) {
					return QString("%1").arg(fileID, 8, 16, QChar('0')).toUpper();
				} else {
					return id.toQString();
				}
			}
		};
	};

	//
	// From APilot/15.3/Faces/Friends/PilotDiskFace.mesa
	//
	class PilotDiskFace {
	public:
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

			void read(ByteBuffer& bb) {
				bb.read(cylinder).read(u1);
			}
			bool inNull() {
				return cylinder == 0 && sector == 0 && head == 0;
			}
			QString toQString() {
				return QString("{%1-%2-%3}").
						arg(cylinder, 4, 16, QChar('0')).
						arg(head,     2, 16, QChar('0')).
						arg(sector,   2, 16, QChar('0')).toUpper();
			}

		};
	};

	//
	// From APilot/15.0.1/Pilot/Friends/Boot.mesa
	//
	class Boot {
	public:
		//DiskFileID: TYPE = MACHINE DEPENDENT RECORD [
		//  fID (0): PilotDisk.FileID,
		//  firstPage (5): PilotDisk.FilePageNumber,
		//  da (7): PilotDiskFace.DiskAddress];
		class DiskFileID {
		public:
			Disk::FileID               fID;
			CARD32                     firstPage;
			PilotDiskFace::DiskAddress da;

			void read(ByteBuffer& bb) {
				fID.read(bb);
				bb.read(firstPage);
				da.read(bb);
			}
			bool isNull() {
				return fID.inNull() && firstPage == 0 && da.inNull();
			}
			QString toQString() {
				if (isNull()) {
					return QString("NULL");
				} else {
					return QString("{%1 %2 %3}").
							arg(fID.toQString()).
							arg(QString::number(firstPage, 16)).
							arg(da.toQString()).toUpper();
				}
			}
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
		class PVBootFiles {
		public:
			DiskFileID hardMicrocode;
			DiskFileID softMicrocode;
			DiskFileID germ;
			DiskFileID pilot;

			void read(ByteBuffer& bb) {
				hardMicrocode.read(bb);
				softMicrocode.read(bb);
				germ.read(bb);
				pilot.read(bb);
			}
			QString toQString() {
				return QString("{hard %1, soft %2, germ %3, pilot %4}").
						arg(hardMicrocode.toQString()).
						arg(softMicrocode.toQString()).
						arg(germ.toQString()).
						arg(pilot.toQString()).toUpper();
			}
		};
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
			System::UniversalID lvID;
			CARD32 lvSize;
			CARD32 lvPage;
			CARD32 pvPage;
			CARD32 nPages;

			void read(ByteBuffer& bb) {
				lvID.read(bb);
				bb.read(lvSize).read(lvPage).read(pvPage).read(nPages);
			}
			QString toQString() {
				return QString("{%1 %2 %3 %4 %5}").
						arg(lvID.toQString()).
						arg(lvSize, 6).
						arg(lvPage, 6).
						arg(pvPage, 6).
						arg(nPages, 6).toUpper();
			}
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
		class Descriptor {
		public:
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

			void read(ByteBuffer& bb) {
				bb.read(seal).read(version).read(labelLength);
				pvID.read(bb);
				bootingInfo.read(bb);
				for(CARD32 i = 0; i < ELEMENTSOF(label); i++) {
					bb.read(label[i]);
				}
				bb.read(subVolumeCount);
				subVolumeMarkerID.read(bb);
				bb.read(badPageCount).read(maxBadPages).read(dataLostPageCount).read(maxDataLostPages).read(onLineCount);

				for(CARD32 i = 0; i < ELEMENTSOF(subVolumes); i++) {
					subVolumes[i].read(bb);
				}
				for(CARD32 i = 0; i < ELEMENTSOF(fill); i++) {
					bb.read(fill[i]);
				}
				bb.read(localTimeParametersValid).read(localTimeParameters).read(checksum);

				// Sanity check
				if (bb.getPos() != 512) {
					logger.fatal("pos expect %6o  actual %6o", 512, bb.getPos());
					ERROR();
				}
				if (seal != My::PhysicalVolume::seal) {
					logger.fatal("seal expect %6o  actual %6o", My::PhysicalVolume::seal, seal);
					ERROR();
				}
				if (version != My::PhysicalVolume::currentVersion) {
					logger.fatal("version expect %6o  actual %6o", My::PhysicalVolume::currentVersion, version);
					ERROR();
				}
			}

			void dump() {
				logger.info("seal                     %o", seal);
				logger.info("version                  %d", version);
				logger.info("pvID                     %s", pvID.toQString().toLocal8Bit().constData());

				logger.info("bootinInfo");
				if (!bootingInfo.hardMicrocode.isNull()) {
					logger.info("  hardMicrocode          %s", bootingInfo.hardMicrocode.toQString().toLocal8Bit().data());
				}
				if (!bootingInfo.softMicrocode.isNull()) {
					logger.info("  softMicrocode          %s", bootingInfo.softMicrocode.toQString().toLocal8Bit().data());
				}
				if (!bootingInfo.germ.isNull()) {
					logger.info("  germ                   %s", bootingInfo.germ.toQString().toLocal8Bit().data());
				}
				if (!bootingInfo.pilot.isNull()) {
					logger.info("  pilot                  %s", bootingInfo.pilot.toQString().toLocal8Bit().data());
				}

				QString label;
				{
					for(CARD16 i = 0; i < labelLength; i++) {
						label.append(label[i]);
					}
				}
				logger.info("label                    %s", label.toLocal8Bit().data());

				logger.info("subVolumeCount           %d", subVolumeCount);
				logger.info("subVolumeMarkerID        %s", subVolumeMarkerID.toQString().toLocal8Bit().constData());
				logger.info("badPageCount             %d", badPageCount);
				logger.info("maxBadPages              %d", maxBadPages);
				logger.info("dataLostPageCount        %d", dataLostPageCount);
				logger.info("maxDataLostPages         %d", maxDataLostPages);
				logger.info("onLineCount              %d", onLineCount);

				logger.info("subVolume");
				for(CARD16 i = 0; i < subVolumeCount; i++) {
					logger.info("                         %s", subVolumes[i].toQString().toLocal8Bit().data());
				}

				logger.info("localTimeParametersValid %d", localTimeParametersValid);
				logger.info("localTimeParameters      %d", localTimeParameters);
				logger.info("checksum                 %d", checksum);
			}
		};
	};
};

int main(int /*argc*/, char** /*argv*/) {
	const char* pilotDiskImagePath   = "data/GVWin/GVWIN001.DSK";

	logger.info("pilotDiskImagePath   = %s", pilotDiskImagePath);
	MyDisk disk(pilotDiskImagePath);

	My::PhysicalVolume::Descriptor descriptor;

	{
		LittleEndianByteBuffer bb(512);
		disk.readSector(0, bb);
		descriptor.read(bb);
	}

	descriptor.dump();

	return 0;
}

