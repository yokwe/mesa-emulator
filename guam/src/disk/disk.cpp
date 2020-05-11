#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("disk");

#include "../mesa/MesaBasic.h"
#include "../agent/DiskFile.h"

#include "../util/ByteBuffer.h"

#include "disk.h"

#define Q_COMPILER_INITIALIZER_LISTS

#include <QtCore>

namespace disk {

Disk::Disk(const char* path) {
	diskFile.attach(path);
}
Disk::~Disk() {
	diskFile.detach();
}

void Disk::readSector(int sector, ByteBuffer& bb, quint32 count) {
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
void Disk::readSector(int sector, ByteBuffer& bb) {
	readSector(sector, bb, 1);
}

//
// PilotFileTypes
//
QString toQString(PilotFileType value) {
    static QMap<PilotFileType, QString> map = {
    		{PilotFileType::tUnasstPhysicalVolumeRootPageigned,  "tUnasstPhysicalVolumeRootPageigned"},
    		{PilotFileType::tBadPageList,                        "tBadPageList"},
    		{PilotFileType::tBadPage,                            "tBadPage"},
    		{PilotFileType::tSubVolumeMarkerPage,                "tSubVolumeMarkerPage"},
    		{PilotFileType::tLogicalVolumeRootPage,              "tLogicalVolumeRootPage"},
    		{PilotFileType::tFreePage,                           "tFreePage"},
    		{PilotFileType::tVolumeAllocationMap,                "tVolumeAllocationMap"},
    		{PilotFileType::tVolumeFileMap,                      "tVolumeFileMap"},
    		{PilotFileType::tScavengerLog,                       "tScavengerLog"},
    		{PilotFileType::tTempFileList,                       "tTempFileList"},
    		{PilotFileType::tTransactionStateFile,               "tTransactionStateFile"},
    		{PilotFileType::tVMBackingFile,                      "tVMBackingFile"},
    		{PilotFileType::tBeingMoved,                         "tBeingMoved"},
    		{PilotFileType::tBeingReplicated,                    "tBeingReplicated"},
    		{PilotFileType::tAnonymousFile,                      "tAnonymousFile"},
    		{PilotFileType::tTransactionLog,                     "tTransactionLog"},
    		{PilotFileType::tScavengerLogOtherVolume,            "tScavengerLogOtherVolume"},
    		{PilotFileType::tRootDirectory,                      "tRootDirectory"},
    };

    if (map.contains(value)) {
        return map[value];
    } else {
        return QString("%1").arg((quint16)value);
    }
}

//
// Processor
//
void ProcessorID::read(ByteBuffer& bb) {
	bb.read(a).read(b).read(c).read(d).read(e).read(f);
}
bool ProcessorID::isNull() {
	return a == 0 && b == 0 && c == 0 && d == 0&& e == 0 && f == 0;
}
QString ProcessorID::toQString() {
	if (isNull()) {
		return "NULL";
	} else {
		return QString("%1%2%3%4%5%6").
				arg(a, 2, 16, QChar('0')).
				arg(b, 2, 16, QChar('0')).
				arg(c, 2, 16, QChar('0')).
				arg(d, 2, 16, QChar('0')).
				arg(e, 2, 16, QChar('0')).
				arg(f, 2, 16, QChar('0')).
				toUpper();
	}
}

//
// UniversalID
//
void UniversalID::read(ByteBuffer& bb) {
	processor.read(bb);
	bb.read(sequence);
}
bool UniversalID::isNull() {
	return processor.isNull() && sequence == 0;
}
QString UniversalID::toQString() {
	if (isNull()) {
		return "NULL";
	} else {
		return QString("{%1-%2}").
				arg(processor.toQString()).
				arg(sequence, 8, 16, QChar('0')).toUpper();
	}
}

//
// FileID
//
void FileID::read(ByteBuffer& bb) {
	quint32 pos = bb.getPos();
	// volumeRelative
	bb.read(fileID).read(pad0).read(pad1).read(pad2);
	quint32 pos1 = bb.getPos();
	// unique
	bb.setPos(pos);
	id.read(bb);
	quint32 pos2 = bb.getPos();
	if (pos1 != pos2) {
		logger.fatal("pos2 expect %6o  actual %6o", pos1, pos2);
		ERROR();
	}
}
bool FileID::isNull() {
	return fileID == 0 && pad0 == 0 && pad1 == 0 && pad2 == 0;
}
QString FileID::toQString() {
	if (isNull()) {
		return "NULL";
	} else if (pad0 == 0 && pad1 == 0 && pad2 == 0 && fileID != 0) {
		return QString("{FILEID %1}").arg(fileID, 8, 16, QChar('0')).toUpper();
	} else {
		return QString("{ID %1}").arg(id.toQString()).toUpper();
	}
}

//
// DiskAddress
//
void DiskAddress::read(ByteBuffer& bb) {
	bb.read(cylinder).read(u1);
}
bool DiskAddress::isNull() {
	return cylinder == 0 && sector == 0 && head == 0;
}
QString DiskAddress::toQString() {
	return QString("{%1-%2-%3}").
			arg(cylinder, 4, 16, QChar('0')).
			arg(head,     2, 16, QChar('0')).
			arg(sector,   2, 16, QChar('0')).
			toUpper();
}

//
// DiskFileID
//
void DiskFileID::read(ByteBuffer& bb) {
	fID.read(bb);
	bb.read(firstPage);
	da.read(bb);
}
bool DiskFileID::isNull() {
	return fID.isNull() && firstPage == 0 && da.isNull();
}
QString DiskFileID::toQString() {
	if (isNull()) {
		return QString("NULL");
	} else {
		return QString("{%1 %2 %3}").
				arg(fID.toQString()).
				arg(QString::number(firstPage, 16)).
				arg(da.toQString()).
				toUpper();
	}
}

//
// PVBootFiles
//
void PVBootFiles::read(ByteBuffer& bb) {
	hardMicrocode.read(bb);
	softMicrocode.read(bb);
	germ.read(bb);
	pilot.read(bb);
}
QString PVBootFiles::toQString() {
	return QString("{hard %1, soft %2, germ %3, pilot %4}").
			arg(hardMicrocode.toQString()).
			arg(softMicrocode.toQString()).
			arg(germ.toQString()).
			arg(pilot.toQString()).
			toUpper();
}

//
// LVBootFiles
//
void LVBootFiles::read(ByteBuffer& bb) {
	hardMicrocode.read(bb);
	softMicrocode.read(bb);
	germ.read(bb);
	pilot.read(bb);
	debugger.read(bb);
	debuggee.read(bb);
}
QString LVBootFiles::toQString() {
	return QString("{hard %1, soft %2, germ %3, pilot %4, debugger %5, debugee %6}").
			arg(hardMicrocode.toQString()).
			arg(softMicrocode.toQString()).
			arg(germ.toQString()).
			arg(pilot.toQString()).
			arg(debugger.toQString()).
			arg(debuggee.toQString()).
			toUpper();
}

//
// PhysicalVolume::SubVolumeDesc
//
void PhysicalVolume::SubVolumeDesc::read(ByteBuffer& bb) {
	lvID.read(bb);
	bb.read(lvSize).read(lvPage).read(pvPage).read(nPages);
}
QString PhysicalVolume::SubVolumeDesc::toQString() {
	return QString("{%1 %2 %3 %4 %5}").
			arg(lvID.toQString()).
			arg(lvSize, 6).
			arg(lvPage, 6).
			arg(pvPage, 6).
			arg(nPages, 6).toUpper();
}

//
// PhysicalVolume::Descriptor
//
void PhysicalVolume::Descriptor::read(ByteBuffer& bb) {
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
	if (seal != PhysicalVolume::seal) {
		logger.fatal("seal expect %6o  actual %6o", PhysicalVolume::seal, seal);
		ERROR();
	}
	if (version != PhysicalVolume::currentVersion) {
		logger.fatal("version expect %6o  actual %6o", PhysicalVolume::currentVersion, version);
		ERROR();
	}
}

void PhysicalVolume::Descriptor::dump() {
	logger.info("seal                     %o", seal);
	logger.info("version                  %d", version);
	logger.info("pvID                     %s", pvID.toString());

	logger.info("bootinInfo");
	if (!bootingInfo.hardMicrocode.isNull()) {
		logger.info("  hardMicrocode          %s", bootingInfo.hardMicrocode.toString());
	}
	if (!bootingInfo.softMicrocode.isNull()) {
		logger.info("  softMicrocode          %s", bootingInfo.softMicrocode.toString());
	}
	if (!bootingInfo.germ.isNull()) {
		logger.info("  germ                   %s", bootingInfo.germ.toString());
	}
	if (!bootingInfo.pilot.isNull()) {
		logger.info("  pilot                  %s", bootingInfo.pilot.toString());
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
		logger.info("                         %s", subVolumes[i].toString());
	}

	logger.info("localTimeParametersValid %d", localTimeParametersValid);
	logger.info("localTimeParameters      %d", localTimeParameters);
	logger.info("checksum                 %d", checksum);
}

//
// Volume::Type
//
void Volume::Type::read(ByteBuffer& bb) {
	bb.read(value);
}
QString Volume::Type::toQString() {
	switch(value) {
	case normal:
		return QString("normal");
	case debugger:
		return QString("debugger");
	case debuggerDebugger:
		return QString("debuggerDebugger");
	case nonPilot:
		return QString("nonPilot");
	default:
		logger.fatal("Unexpected value = %X", value);
		ERROR();
		break;
	}
}

//
// LogicalVolume::VolumeLock
//
void LogicalVolume::VolumeLock::read(ByteBuffer& bb) {
	owner.read(bb);
	bb.read(lock);
}
QString LogicalVolume::VolumeLock::toQString() {
	return QString("{%1 %2}").arg(owner.toQString()).arg(lock, 4, 16, QChar('0'));
}

//
// LogicalVolume::Descriptor
//
void LogicalVolume::Descriptor::read(ByteBuffer& bb) {
	bb.read(seal).read(version);
	vID.read(bb);
	bb.read(labelLength);
	for(CARD32 i = 0; i < ELEMENTSOF(label); i++) {
		bb.read(label[i]);
	}
	type.read(bb);
	bb.read(volumeSize);
	bootingInfo.read(bb);
	bb.read(pad).read(changing);

	bb.read(freePageCount).read(vamStart).read(primaryETableStartPage).read(copyETableStartPage).read(lowerBound);
	bb.read(volumeRootDirectory);
	for(CARD32 i = 0; i < ELEMENTSOF(nootFileID); i++) {
		bb.read(nootFileID[i]);
	}
	bb.read(lastIDAllocated);
	scavengerLogVolume.read(bb);

	CARD16 status;
	bb.read(lastTimeOpendForWrite).read(status).read(numberOfFile);
	statusPrimaryETableFile = (status >> 8) & 0xFF;
	statusCopyETableFile    = status & 0xFF;

	lock.read(bb);
	bb.read(numberOfBadPagesLastTime);
	for(CARD32 i = 0; i < ELEMENTSOF(fill); i++) {
		bb.read(fill[i]);
	}
	bb.read(checksum);

	// Sanity check
	if (bb.getPos() != 512) {
		logger.fatal("pos expect %6o  actual %6o", 512, bb.getPos());
		ERROR();
	}
	if (seal != LogicalVolume::lvRootSeal) {
		logger.fatal("seal expect %6o  actual %6o", LogicalVolume::lvRootSeal, seal);
		ERROR();
	}
	if (version != LogicalVolume::currentVersion) {
		logger.fatal("version expect %6o  actual %6o", LogicalVolume::currentVersion, version);
		ERROR();
	}
}
void LogicalVolume::Descriptor::dump() {
	logger.info("seal                     %o", seal);
	logger.info("version                  %d", version);
	logger.info("labelLength              %d", labelLength);
	QString labelString;
	{
		for(CARD16 i = 0; i < labelLength; i++) {
			labelString.append(label[i]);
		}
	}
	logger.info("label                    %s", labelString.toLocal8Bit().constData());
	logger.info("type                     %s", type.toString());
	logger.info("volumeSize               %d", volumeSize);
	logger.info("bootingInfo              %s", bootingInfo.toString());
	logger.info("changing                 %d", changing);

	logger.info("freePageCount            %d", freePageCount);
	logger.info("vamStart                 %d", vamStart);
	logger.info("primaryETableStartPage   %d", primaryETableStartPage);
	logger.info("copyETableStartPage      %d", copyETableStartPage);
	logger.info("lowerBound               %d", lowerBound);
	logger.info("volumeRootDirectory      %d", volumeRootDirectory);

	for(CARD32 i = 0; i < ELEMENTSOF(nootFileID); i++) {
		logger.info("nootFileID               %d", nootFileID[i]);
	}
	logger.info("lastIDAllocated          %d", lastIDAllocated);

}



}
