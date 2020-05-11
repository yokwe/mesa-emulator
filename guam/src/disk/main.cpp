#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("main");

#include "../agent/DiskFile.h"

#include "disk.h"


int main(int /*argc*/, char** /*argv*/) {
	const char* pilotDiskImagePath   = "data/GVWin/GVWIN001.DSK";

	logger.info("pilotDiskImagePath   = %s", pilotDiskImagePath);
	disk::Disk disk(pilotDiskImagePath);

	disk::PhysicalVolume::Descriptor pvDescriptor;
	{
		LittleEndianByteBuffer bb(512);
		disk.readSector(0, bb);
		pvDescriptor.read(bb);
		pvDescriptor.dump();
	}

	disk::LogicalVolume::Descriptor lvDescriptor;
	{
		LittleEndianByteBuffer bb(512);

		for(CARD16 i = 0; i < ELEMENTSOF(pvDescriptor.subVolumes); i++) {
			disk::PhysicalVolume::SubVolumeDesc& svd(pvDescriptor.subVolumes[i]);
			if (svd.lvID.isNull()) continue;

			logger.info("===== subVolume %s", svd.toString());

			disk.readSector(svd.pvPage, bb);
			lvDescriptor.read(bb);
			lvDescriptor.dump();
		}
	}


	return 0;
}

