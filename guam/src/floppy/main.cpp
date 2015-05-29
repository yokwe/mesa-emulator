#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("floppy");

#include "../mesa/MesaBasic.h"
#include "../agent/DiskFile.h"

int main(int /*argc*/, char** /*argv*/) {
	const char* path = "tmp/floppy/floppy144";
	CARD16 page[PageSize];

	logger.info("path = %s", path);
	DiskFile diskFile;
	diskFile.attach(path);

	diskFile.readPage(8, page, SIZE(page));
	Util::fromBigEndian(page, page, SIZE(page));

	logger.info("page  %5o  %4X", page[0], page[1]);
	return 0;
}

