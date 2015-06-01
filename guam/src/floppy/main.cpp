#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("floppy");

#include "../mesa/MesaBasic.h"
#include "../agent/DiskFile.h"

#include "../util/ByteBuffer.h"

class FloppyDisk {
public:
	FloppyDisk(const char* path) {
		diskFile.attach(path);
	}

	void readSector(int sector, ByteBuffer& bb) {
		if (sector <= 0) {
			logger.fatal("sector = %d", sector);
			ERROR();
		}
		diskFile.readPage((sector - 1), (CARD16*)(bb.getData()), PageSize);
		bb.setPos(PageSize * Environment::bytesPerWord);
		bb.rewind();
	}

private:
	DiskFile   diskFile;
};

int main(int /*argc*/, char** /*argv*/) {
	const char* path = "tmp/floppy/floppy144";

	logger.info("path = %s", path);
	FloppyDisk floppyDisk(path);

	LittleEndianByteBuffer bb(512);

	floppyDisk.readSector(9, bb);

	logger.info("page  seal                %6o", bb.get16());
	logger.info("page  version             %6X", bb.get16());
	logger.info("page  cylinders           %6d", bb.get16());
	logger.info("page  tracksPerCylinder   %6d", bb.get16());
	logger.info("page  sectorsPerTrack     %6d", bb.get16());
	logger.info("page  fileList            %6d", bb.get16());
	logger.info("page  fileListID          %6X", bb.get32());
	logger.info("page  fileListSize        %6d", bb.get16());
	logger.info("page  rootFile            %6X", bb.get32());
	logger.info("page  alternateMicrocode  %6d", bb.get16());
	logger.info("page  pilotMicrocode      %6d", bb.get16());
	logger.info("page  diagnosticMicrocode %6d", bb.get16());
	logger.info("page  germ                %6d", bb.get16());
	logger.info("page  pilotBootFile       %6d", bb.get16());
	logger.info("page  firstAlternateSector%6d", bb.get16());
	logger.info("page  countBadSectors     %6d", bb.get16());
	logger.info("page  nextUnusedFileID    %6X", bb.get32());
	logger.info("page  changing            %6X", bb.get16());
	int labelSize = bb.get16();
	logger.info("page  labelSize           %6d", labelSize);
	QByteArray label;
	for(int i = 0; i < labelSize; i++) {
		quint8 c = bb.get8_();
		label.append(c);
	}
	logger.info("page  label               %s",  label.constData());

	return 0;
}

