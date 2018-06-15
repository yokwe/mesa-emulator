/*
Copyright (c) 2018, Yasuhiro Hasegawa
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
// LoadState.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("loadState");

#include "Memory.h"
#include "LoadState.h"

#include "../symbols/BCD.h"
#include "../symbols/BCDFile.h"
#include "../symbols/Symbols.h"


class ModuleEntry {
public:
	const quint64 version;
	const QString moduleName;

	QVector<QString> entryName;

	ModuleEntry(quint64 version_, QString moduleName_) : version(version_), moduleName(moduleName_) {}

	QString toString() {
		return QString("%1 %2 %3").arg(version).arg(moduleName).arg(entryName.size());
	}
};
static QMap<quint64, ModuleEntry*> moduleEntryMap; // key is module file version

void initializeModuleEntryMap() {
	QString PATH_MODULE_ENTRY("tmp/moduleEntry/");
	QDirIterator i(PATH_MODULE_ENTRY);

	while(i.hasNext()) {
		QString path = i.next();
//		logger.info("path = %s", path.toLocal8Bit().constData());

		QFileInfo fileInfo(path);
		if (fileInfo.isDir()) continue;

		QFile file(path);
		{
			bool ok = file.open(QIODevice::ReadOnly | QIODevice::Text);
			if (!ok) {
				logger.fatal("path = %s", path.toLocal8Bit().constData());
				logger.fatal("Unexpected errorString = %s", file.errorString().toLocal8Bit().constData());
				ERROR();
			}
		}
		QTextStream text(&file);
		uint count = 0;
		ModuleEntry* entry = 0;
		for(;;) {
			if (text.atEnd()) break;
			QString line = text.readLine();
			if (line.isNull()) break;

			QStringList token = line.split(' ');
			if (token.size() != 4) {
				logger.fatal("path = %s", path.toLocal8Bit().constData());
				logger.fatal("Unexpected line = %s", line.toLocal8Bit().constData());
				ERROR();
			}

			bool ok;
			const quint64 version     = token.at(0).toULongLong(&ok);
			if (!ok) {
				logger.fatal("path = %s", path.toLocal8Bit().constData());
				logger.fatal("Unexpected line = %s", line.toLocal8Bit().constData());
				logger.fatal("version = %s!", token.at(0).toLocal8Bit().constData());
				ERROR();
			}
			const QString moduleName  = token.at(1);
			const uint    entryIndex  = token.at(2).toUInt(&ok);
			if (!ok) {
				logger.fatal("path = %s", path.toLocal8Bit().constData());
				logger.fatal("Unexpected line = %s", line.toLocal8Bit().constData());
				logger.fatal("entryIndex = %s!", token.at(2).toLocal8Bit().constData());
				ERROR();
			}
			const QString entryName   = token.at(3);

			if (count == 0) {
				entry = new ModuleEntry(version, moduleName);
			}
			if (count != entryIndex) {
				logger.fatal("path = %s", path.toLocal8Bit().constData());
				logger.fatal("Unexpected line = %s", line.toLocal8Bit().constData());
				logger.fatal("entryIndex = %s!", token.at(2).toLocal8Bit().constData());
				logger.fatal("count = %d", count);
				ERROR();
			}
			entry->entryName.append(entryName);

			//
			count++;
		}

		moduleEntryMap.insert(entry->version, entry);
	}

	logger.info("%s %s %d", __FUNCTION__, PATH_MODULE_ENTRY.toLocal8Bit().constData(), moduleEntryMap.size());
}


class BCDModuleInfo {
public:
	const CARD16 bcdIndex;
	const CARD16 moduleIndex;

	BCDModuleInfo(CARD16 bcdIndex_, CARD16 moduleIndex_) : bcdIndex(bcdIndex_), moduleIndex(moduleIndex_) {}

    bool operator==(const BCDModuleInfo& that) const {
        return (this->bcdIndex == that.bcdIndex) && (this->moduleIndex == that.moduleIndex);
    }
    bool operator<(const BCDModuleInfo& that) const {
    	return (this->bcdIndex != that.bcdIndex) ? (this->bcdIndex < that.bcdIndex) : (this->moduleIndex < that.moduleIndex);
    }

    QString toString() {
    	return QString("%1-%2").arg(bcdIndex).arg(moduleIndex);
    }
};
static QMap<BCDModuleInfo, CARD16>  gfiMap;    // scanModule add entries

class GFInfo {
public:
	CARD16  gfi;
	QString name;

	QString fileName;
	quint64 fileVersion;

	CARD32  codebase;
	bool    dummyEntryName;

	QMap<CARD16, QString> entryNameMap; // key = initialPC, value = name

	GFInfo(CARD16 gfi_, QString name_, QString fileName_, quint64 fileVersion_, CARD32 codebase_) :
		gfi(gfi_), name(name_), fileName(fileName_), fileVersion(fileVersion_), codebase(codebase_), dummyEntryName(0) {}

	QString toString() {
		return QString("%1 %2 %3 %4 %5 %6 %7").arg(gfi, 4, 16).arg(name, -32).arg(fileName, -32).arg(fileVersion).arg(codebase, 8, 16).arg(dummyEntryName ? "D" : " ").arg(entryNameMap.size(), 3);
	}
};
static QMap<BCDModuleInfo, GFInfo*> gfInfoMap; // scanBCD add entries


static void scanBCD(CARD32 loadStateAddress, LoadStateFormat::Object& loadState) {
//	static QSet<CARD16>       done;
	static QMap<CARD32, BCD*> bcdMap; // key is base address. Cache BCD* in case of re-scan

	for(CARD16 i = 0; i < loadState.nBcds; i++) {
		CARD32 bcdInfoBase = loadStateAddress + loadState.bcdInfo + SIZE(LoadStateFormat::BcdInfo) * i;

		LoadStateFormat::BcdInfo bcdInfo;
		bcdInfo.u0   = *Fetch(bcdInfoBase  + OFFSET(LoadStateFormat::BcdInfo, u0));
		bcdInfo.base = ReadDbl(bcdInfoBase + OFFSET(LoadStateFormat::BcdInfo, base));
		bcdInfo.id   = *Fetch(bcdInfoBase  + OFFSET(LoadStateFormat::BcdInfo, id));

		// Skip if bcdInfo is not mapped
		//   check first page
		if (Memory::isVacant(bcdInfo.base)) {
			logger.info("bcdInfo %4d %8X first bcd page is not mapped", bcdInfo.id, bcdInfo.base);
			continue;
		}

		CARD16 bcdIndex = bcdInfo.id;
		if (bcdIndex != i) {
			logger.fatal("bcdIndex = %d  i = %d", bcdIndex, i);
			ERROR();
		}

//		logger.info("bcdInfo %4d %8X+%4d %4d", bcdIndex, bcdInfo.base, bcdInfo.pages, bcdInfo.id);

		BCD* bcd;
		{
			CARD32 base = bcdInfo.base;
			if (bcdMap.contains(base)) {
				bcd = bcdMap[base];
			} else {
				BCDFile* bcdFile = BCDFile::getInstance(base);
				try {
					bcd = new BCD(bcdFile);
				} catch (Abort& e) {
					bcd = 0;
				}
				if (bcd == 0) {
					logger.fatal("BCD page is not mapped");
					ERROR();
				}
				bcdMap[base] = bcd;
			}
		}

		QList<MTRecord*> mtList = bcd->mt.values();

		CARD16 moduleIndex = 0;
		for(MTRecord* mt: mtList) {
			if (mt->isNull()) continue;

			// moduleIndex starts from 1
			moduleIndex++;

			const QString   name(mt->name);
			const FTRecord* file(mt->file);

			BCDModuleInfo key(bcdIndex, moduleIndex);

			// If there is no correspondence moduleInfo entry, skip this entry.
			if (!gfiMap.contains(key)) {
				logger.warn("No module info  key = %s  %s", key.toString().toLocal8Bit().constData(), name.toLocal8Bit().constData());
				continue;
			}

			CARD16 gfi = gfiMap[key];

			if (gfInfoMap.contains(key)) {
				GFInfo* gfInfo = gfInfoMap[key];
				if (gfInfo->gfi != gfi) {
					logger.fatal("Unexpected key = %s  gfi = %4X  gfInfo = %s", key.toString().toLocal8Bit().constData(), gfi, gfInfo->toString().toLocal8Bit().constData());
					ERROR();
				}
			} else {
				QString fileName    = file->name;
				quint64 fileVersion = file->version->value;
				CARD32  codebase    = ReadDbl(GFT_OFFSET(gfi, codebase)) & ~1;

				GFInfo* gfInfo = new GFInfo(gfi, name, (file->isSelf() ? name : file->name), file->version->value, codebase);
				gfInfoMap[key] = gfInfo;

				logger.info("gfInfo %-7s %s", key.toString().toLocal8Bit().constData(), gfInfo->toString().toLocal8Bit().constData());

				int initialPCSize = mt->entries->initialPC.size();
				if (initialPCSize != 0 && moduleEntryMap.contains(fileVersion)) {
					ModuleEntry* moduleEntry = moduleEntryMap[fileVersion];
					int moduleEntrySize = moduleEntry->entryName.size();
					if (moduleEntrySize == initialPCSize) {
						for(int j = 0; j < initialPCSize; j++) {
							CARD16  initialPC = mt->entries->initialPC[j];
							QString entryName = moduleEntry->entryName[j];
							QString name = QString("%1.%2").arg(fileName).arg(entryName);
							gfInfo->entryNameMap[initialPC] = name;
						}
					} else {
						logger.fatal("Unexpected  entrySize");
						logger.fatal("entrySize  initialPCSize = %d   moduleEntry = %d", initialPCSize, moduleEntrySize);
						logger.fatal("mt %s", mt->toString().toLocal8Bit().constData());
						logger.fatal("moduleEntry %s", moduleEntry->toString().toLocal8Bit().constData());
						logger.fatal("gfInfo %s", gfInfo->toString().toLocal8Bit().constData());
						ERROR();
					}
				} else {
					// Add dummy entry to entryName
					for(int j = 0; j < initialPCSize; j++) {
						CARD16  initialPC = mt->entries->initialPC[j];
						QString entryName = QString("%1").arg(j);
						QString name = QString("%1.%2").arg(fileName).arg(entryName);
						gfInfo->entryNameMap[initialPC] = name;
					}
					gfInfo->dummyEntryName = 1;
				}
			}
		}
	}
}


static void scanModule(CARD32 loadStateAddress, LoadStateFormat::Object& loadState) {
	// Order of module entry can be change.
	// So need scan all module entry each time.

	for(CARD16 i = 0; i < loadState.nModules; i++) {
		CARD32 moduleBase = loadStateAddress + loadState.moduleInfo + SIZE(LoadStateFormat::ModuleInfo) * i;

		LoadStateFormat::ModuleInfo moduleInfo;
		moduleInfo.u0          = *Fetch(moduleBase  + OFFSET(LoadStateFormat::ModuleInfo, u0));
		moduleInfo.index       = *Fetch(moduleBase  + OFFSET(LoadStateFormat::ModuleInfo, index));
		moduleInfo.globalFrame = *Fetch(moduleBase  + OFFSET(LoadStateFormat::ModuleInfo, globalFrame));

		CARD16 bcdIndex    = moduleInfo.index;
		CARD16 moduleIndex = moduleInfo.cgfi;
		CARD16 gfi         = moduleInfo.globalFrame;

		BCDModuleInfo key(bcdIndex, moduleIndex);
		if (gfiMap.contains(key)) {
			CARD16 oldValue = gfiMap[key];
			if (oldValue != moduleInfo.globalFrame) {
				logger.fatal("Unexpected %s moduleInfo.globalFrame =  %4X  oldValue %4X  %5d",
					key.toString().toLocal8Bit().constData(), moduleInfo.globalFrame, oldValue, i);
				ERROR();
			}
		} else {
			gfiMap[key] = moduleInfo.globalFrame;

//			logger.info("module %4d %d %5d %5d %4X  %8X", i, moduleInfo.resolved, moduleInfo.cgfi, moduleInfo.index, moduleInfo.globalFrame, codebase);
			logger.info("module %5d %5d %5d %4X", i, bcdIndex, moduleIndex, gfi);
		}
	}
}

void scanLoadState() {
	try {
		if (moduleEntryMap.size() == 0) {
			logger.fatal("Unexpected moduleEntryMap.size() == 0");
			ERROR();
		}

		// Sanity check
		CARD32 pesv = ReadDbl(CPSwapDefs::PSEV);
		if (pesv == 0) return;

		{
			CPSwapDefs::ExternalStateVector esv;
			esv.version = *Fetch(pesv + OFFSET(CPSwapDefs::ExternalStateVector, version));
			if (esv.version != CPSwapDefs::currentVersion) {
				logger.fatal("Wrong esv.version  %5d", esv.version);
				ERROR();
			}
			CARD16* p27 = Store(pesv + OFFSET(CPSwapDefs::ExternalStateVector, u27));

			esv.u27 = *p27;
			// Return if load state is changing
			if (esv.loadStateChanging) {
	  			// logger.warn("loadStateChanging");
				return;
			}
			// Return if load state is clean (not changed)
			if (!esv.loadStateDirty) {
				// logger.warn("loadStateClean");
				return;
			}

			esv.loadState = ReadDbl(pesv + OFFSET(CPSwapDefs::ExternalStateVector, loadState));
			// Return if load state is null
			if (esv.loadState == 0) {
				logger.fatal("loadState == 0");
				ERROR();
			}

			// Scan load state
			LoadStateFormat::Object loadState;

			CARD32 loadStateAddress = esv.loadState;
			loadState.versionident = *Fetch(loadStateAddress + OFFSET(LoadStateFormat::Object, versionident));
			loadState.nModules     = *Fetch(loadStateAddress + OFFSET(LoadStateFormat::Object, nModules));
			loadState.maxModules   = *Fetch(loadStateAddress + OFFSET(LoadStateFormat::Object, maxModules));
			loadState.nBcds        = *Fetch(loadStateAddress + OFFSET(LoadStateFormat::Object, nBcds));
			loadState.maxBcds      = *Fetch(loadStateAddress + OFFSET(LoadStateFormat::Object, maxBcds));
			loadState.nextID       = *Fetch(loadStateAddress + OFFSET(LoadStateFormat::Object, nextID));
			loadState.moduleInfo   = *Fetch(loadStateAddress + OFFSET(LoadStateFormat::Object, moduleInfo));
			loadState.bcdInfo      = *Fetch(loadStateAddress + OFFSET(LoadStateFormat::Object, bcdInfo));

			if (loadState.versionident != LoadStateFormat::VersionID) {
				logger.fatal("loadState.versionident %5d %5d", loadState.versionident, LoadStateFormat::VersionID);
				ERROR();
			}

			logger.info("loadState %5d %5d %5d", loadState.nModules, loadState.nBcds, loadState.nextID);
			scanModule(loadStateAddress, loadState);
			scanBCD(loadStateAddress, loadState);

			esv.loadStateDirty = false;
			*p27 = esv.u27;
		}
	} catch (Error& e) {
		logger.fatal("Error %s %d %s", e.file, e.line, e.func);
		throw;
	} catch (Abort& e) {
		logger.fatal("Abort %s %d %s", e.file, e.line, e.func);
		throw;
	} catch (...) {
		logger.fatal("Unknown error");
		throw;
	}
}
