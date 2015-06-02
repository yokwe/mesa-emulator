#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("floppy");

#include "../mesa/MesaBasic.h"
#include "../agent/DiskFile.h"
#include "../agent/AgentProcessor.h"

#include "../util/ByteBuffer.h"

#include <QtCore>

class FloppyDisk {
public:
	// APilot/15.0.1/Pilot/Public/FileTypes.emsa
	//  FileTypes.mesa  CommonSoftwareFileType: TYPE = CARDINAL [2048..3072);
	static const quint16 CommonSoftwareFileType    = 2048;

	// APilot/15.0.1/ComSoft/Public/CommonSoftwareFileTypes.emsa
	static const quint16 COMMON_SOFTWARE_FILE_TYPE = 2048;
	static const quint16 tUnassigned               = CommonSoftwareFileType + 0;
	static const quint16 tDirectory                = CommonSoftwareFileType + 1;
	static const quint16 tBackstopLog              = CommonSoftwareFileType + 3;
	static const quint16 tCarryVolumeDirectory     = CommonSoftwareFileType + 4;
	static const quint16 tClearingHouseBackupFile  = CommonSoftwareFileType + 5;
	static const quint16 tFilelist                 = CommonSoftwareFileType + 6;
	static const quint16 tBackstopDebugger         = CommonSoftwareFileType + 7;
	static const quint16 tBackstopDebuggee         = CommonSoftwareFileType + 8;

	// AMesa/14.0/Floppy/Friends/AccessFloppy.mesa
	static const quint16 tFloppyLeaderPage         = tCarryVolumeDirectory;

	FloppyDisk(const char* path) {
		diskFile.attach(path);
	}

	void readSector(int sector, ByteBuffer& bb, quint32 count) {
		if (sector <= 0) {
			logger.fatal("sector = %d", sector);
			ERROR();
		}
		quint8* data = bb.getData();
		for(quint32 i = 0; i < count; i++) {
			diskFile.readPage((sector - 1) + i, (CARD16*)(data), PageSize);
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

class SectorNine {
public:
	static const quint16 SEAL    = 0141414;
	static const quint16 VERSION =       1;

	quint16 seal;
	quint16 version;
	quint16 cylinders;
	quint16 tracksPerCylinder;
	quint16 sectorsPerTrack;
	quint16 fileList;
	quint32 fileListID;
	quint16 fileListSize;
	quint32 rootFile;
	quint16 alternateMicrocode;
	quint16 pilotMicrocode;
	quint16 diagnosticMicrocode;
	quint16 germ;
	quint16 pilotBootFile;
	quint16 firstAlternateSector;
	quint16 countBadSectors;
	quint32 nextUnusedFileID;
	quint16 changing;
	quint16 labelSize;
	QByteArray label;

	SectorNine(FloppyDisk& floppyDisk) {
		LittleEndianByteBuffer bb(512);

		floppyDisk.readSector(9, bb);
		seal                 = bb.get16();
		if (seal != SEAL) {
			logger.fatal("seal = %6o  SEAL = %6o", seal, SEAL);
			ERROR();
		}
		version              = bb.get16();
		if (version != VERSION) {
			logger.fatal("version = %6d  VERSIO = %6d", version, VERSION);
			ERROR();
		}
		cylinders            = bb.get16();
		if (cylinders != 80) {
			logger.fatal("cylinders = %6d", cylinders);
			ERROR();
		}
		tracksPerCylinder    = bb.get16();
		if (tracksPerCylinder != DiskFile::FLOPPY_NUMBER_OF_HEADS) {
			logger.fatal("tracksPerCylinder = %6d  FLOPPY_NUMBER_OF_HEADS = %6d", tracksPerCylinder, DiskFile::FLOPPY_NUMBER_OF_HEADS);
			ERROR();
		}
		sectorsPerTrack      = bb.get16();
		if (sectorsPerTrack != DiskFile::FLOPPY_SECTORS_PER_TRACK) {
			logger.fatal("sectorsPerTrack = %6d  FLOPPY_SECTORS_PER_TRACK = %6d", tracksPerCylinder, DiskFile::FLOPPY_SECTORS_PER_TRACK);
			ERROR();
		}
		fileList             = bb.get16();
		fileListID           = bb.get32();
		fileListSize         = bb.get16();
		rootFile             = bb.get32();
		alternateMicrocode   = bb.get16();
		pilotMicrocode       = bb.get16();
		diagnosticMicrocode  = bb.get16();
		germ                 = bb.get16();
		pilotBootFile        = bb.get16();
		firstAlternateSector = bb.get16();
		countBadSectors      = bb.get16();
		nextUnusedFileID     = bb.get32();
		changing             = bb.get16();
		labelSize            = bb.get16();
		for(int i = 0; i < labelSize; i++) {
			label.append(bb.get8());
		}
	}
	void dump() {
		logger.info("seal                  %6o", seal);
		logger.info("version               %6d", version);
		logger.info("cylinders             %6d", cylinders);
		logger.info("tracksPerCylinder     %6d", tracksPerCylinder);
		logger.info("sectorsPerTrack       %6d", sectorsPerTrack);
		logger.info("fileList              %6d", fileList);
		logger.info("fileListID            %6d", fileListID);
		logger.info("fileListSize          %6d", fileListSize);
		logger.info("rootFile              %6d", rootFile);
		logger.info("alternateMicrocode    %6d", alternateMicrocode);
		logger.info("pilotMicrocode        %6d", pilotMicrocode);
		logger.info("diagnosticMicrocode   %6d", diagnosticMicrocode);
		logger.info("germ                  %6d", germ);
		logger.info("pilotBootFile         %6d", pilotBootFile);
		logger.info("firstAlternateSector  %6d", firstAlternateSector);
		logger.info("countBadSectors       %6d", countBadSectors);
		logger.info("nextUnusedFileID      %6d", nextUnusedFileID);
		logger.info("changing              %6d", changing);
		logger.info("labelSize             %6d", labelSize);
		logger.info("label                     \"%s\"", label.constData());
	}
};

class FileList {
public:
	static const quint16 SEAL    = 0131313;
	static const quint16 VERSION =       1;

	class Entry {
	public:
		quint32 file;
		quint16 type;
		quint16 location;
		quint16 size;

		Entry(ByteBuffer& bb) {
			file     = bb.get32();
			type     = bb.get16();
			location = bb.get16();
			size     = bb.get16();
		}
		Entry(const Entry& that) {
			file     = that.file;
			type     = that.type;
			location = that.location;
			size     = that.type;
		}
		void dump() const {
			logger.info("file      %6d", file);
			logger.info("type      %6d", type);
			logger.info("location  %6d", location);
			logger.info("size      %6d", size);
		}
	};

	quint16         seal;
	quint16         version;
	quint16         count;
	quint16         maxEntries;
	QVector<Entry*> files;

	FileList(FloppyDisk& floppyDisk, quint32 fileList, quint32 fileListSize) {
		quint32 dataSize = Environment::bytesPerPage * fileListSize;
		LittleEndianByteBuffer bb(dataSize);
		floppyDisk.readSector(fileList, bb, fileListSize);

		seal       = bb.get16();
		if (seal != SEAL) {
			logger.fatal("seal = %6o  SEL = %6o", seal, SEAL);
			ERROR();
		}
		version    = bb.get16();
		if (version != VERSION) {
			logger.fatal("version = %6d  VERSIO = %6d", version, VERSION);
			ERROR();
		}
		count      = bb.get16();
		maxEntries = bb.get16();
		for(int i = 0; i < count; i++) {
			Entry* entry = new Entry(bb);
			files.append(entry);
		}
	}
	void dump() {
		logger.info("seal        %6o", seal);
		logger.info("version     %6d", version);
		logger.info("count       %6d", count);
		logger.info("maxEntries  %6d", maxEntries);
		for(int i = 0; i < count; i++) {
			const Entry* entry = files.at(i);
			logger.info("----");
			entry->dump();
		}
		logger.info("----");
	}
};

//
// AMesa/14.0/Floppy/Friends/AccessFloppy.mesa
//
class FloppyLeaderPage {
public:
	static const int SEAL    = 0125252;
	static const int VERSION =       1;

	// Identity attributes
	quint16 seal;     // used to check consistency of a file.
	quint16 version;  // version of attributes record type.
	quint16 type;     // file type of containing file.

	// Activity attributes
	QDateTime createData;       // the creation data of the disk file.
	QDateTime lastWrittenData;  // the date the disk file was last modified prior to copying to the floppy.

	// File attributes
	quint32 size;              // number of pages in the floppy file not including the leader page.
	quint32 offset;            // page number in the disk file corresponding to the first page in the floppy file piece.
	quint32 totalSize;         // number of pages in the disk file.
	quint32 totalSizeInBytes;  // the number of bytes in the disk file

	//Name attributes
	//  -- name attributes
	//  length: CARDINAL _ 0,
	//  maxLength: CARDINAL _ maxNameLength, -- so that @length is STRING.
	//  name: PACKED ARRAY [0..maxNameLength) OF Environment.Byte,
	quint16 length;
	quint16 maxLength;
	QByteArray name;

	//Client attributes
	//  -- client attributes
	//  clientDataLength: CARDINAL _ 0 ,-- number of component in client's private data.
	//  clientData: SEQUENCE maxlength: CARDINAL OF UNSPECIFIED
	quint16 clientDataLength;  // number of component in client's private data.
	QByteArray clientData;
	quint32    contentsPos;
	QByteArray contents;

	FloppyLeaderPage(FloppyDisk& floppyDisk, const FileList::Entry* entry) {
		quint32 dataSize = Environment::bytesPerPage * entry->size;
		LittleEndianByteBuffer bb(dataSize);
		floppyDisk.readSector(entry->location, bb, entry->size);

		seal             = bb.get16();
		if (seal != SEAL) {
			logger.fatal("seal = %6o  SEAL = %6o", seal, SEAL);
			ERROR();
		}
		version          = bb.get16();
		if (version != VERSION) {
			logger.fatal("version = %6d  VERSIO = %6d", version, VERSION);
			ERROR();
		}
		type             = bb.get16();

		createData.setTime_t(AgentProcessor::toUnixTime(bb.get32()));
		lastWrittenData.setTime_t(AgentProcessor::toUnixTime(bb.get32()));

		size             = bb.get32();
		offset           = bb.get32();
		totalSize        = bb.get32();
		totalSizeInBytes = bb.get32();

		length           = bb.get16();
		maxLength        = bb.get16();
		for(quint16 i = 0; i < length; i++) {
			name.append(bb.get8());
		}
		bb.skipByte(maxLength - length);

		clientDataLength = bb.get16();
		quint16 clientDataMaxLength = bb.get16();
		for(quint16 i = 0; i < clientDataLength; i++) {
			clientData.append(bb.get16());
		}
		bb.skipWord(clientDataMaxLength - clientDataLength);

		contentsPos = bb.getPos();

		for(quint32 i = 0; i < totalSizeInBytes; i++) {
			contents.append(bb.get8());
		}
	}

	void dump() {
		logger.info("seal             %6o", seal);
		logger.info("version          %6d", version);
		logger.info("size             %6d", size);
		logger.info("offset           %6d", offset);
		logger.info("totalSize        %6d", totalSize);
		logger.info("totalSizeInBytes %6d", totalSizeInBytes);
		logger.info("createData      %s", createData.toString("yyyy-MM-dd HH:mm:ss").toLocal8Bit().constData());
		logger.info("lastWrittenData %s", lastWrittenData.toString("yyyy-MM-dd HH:mm:ss").toLocal8Bit().constData());
		logger.info("name            \"%s\"", name.constData());
		logger.info("clientDataLength %6d", clientDataLength);
		logger.info("clientDataMax    %6d", clientData.size());
		logger.info("contentsPos      %6d", contentsPos);
		logger.info("contents         %6d", contents.size());
	}

};

int main(int /*argc*/, char** /*argv*/) {
	const char* path = "tmp/floppy/floppy144";

	logger.info("path = %s", path);
	FloppyDisk floppyDisk(path);

	SectorNine sectorNine(floppyDisk);
	sectorNine.dump();
	logger.info("");

	FileList fileList(floppyDisk, sectorNine.fileList, sectorNine.fileListSize);
	fileList.dump();

	for(int i = 0; i < fileList.count; i++) {
		const FileList::Entry* entry = fileList.files.at(i);
		if (entry->type != FloppyDisk::tFloppyLeaderPage) continue;
		FloppyLeaderPage leaderPage(floppyDisk, entry);
		logger.info("----");
		leaderPage.dump();
	}

	return 0;
}

