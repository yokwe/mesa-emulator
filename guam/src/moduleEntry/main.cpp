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
// main.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("main");

#include "../symbols/BCDFile.h"
#include "../symbols/BCD.h"
#include "../symbols/Symbols.h"

#include "../symbols/BTIndex.h"
#include "../symbols/SEIndex.h"
#include "../symbols/HTIndex.h"


void PageFault(CARD32 ptr) {
	logger.fatal("%s %X", __FUNCTION__, ptr);
	ERROR();
}
void WriteProtectFault(CARD32 ptr) {
	logger.fatal("%s %X", __FUNCTION__, ptr);
	ERROR();
}


class Module {
public:
	typedef Symbols::TransferMode TransferMode;

	static void dumpEntry(QString outDirPath, QString filePath);
	static void dumpEntry(QString outDirPath, Symbols& symbols);

private:
	class Entry {
	public:
		const CARD16       entryIndex;
		const QString      name;
		const TransferMode mode;

		Entry(CARD16 entryIndex_, QString name_, TransferMode mode_) : entryIndex(entryIndex_), name(name_), mode(mode_) {}
	};
};

void Module::dumpEntry(QString outDirPath, QString filePath) {
	// Check existence of file
	if (!QFile::exists(filePath)) {
		logger.fatal("File does not exist. pathFile = %s", filePath.toLocal8Bit().constData());
		ERROR();
	}
	BCDFile* file = BCDFile::getInstance(filePath);

	// Read bcd file
	BCD* bcd = new BCD(file);

	// Locate target segment
	if (Symbols::isSymbolsFile(bcd)) {
		logger.info("symbol file");
		Symbols symbols(bcd, 2);
		dumpEntry(outDirPath, symbols);
	} else {
	   for (SGRecord* p : bcd->sg.values()) {
			if (p->segClass == SGRecord::SegClass::SYMBOLS) {
				if (p->file->isSelf()) {
					logger.info("symbol segment  %s %s", p->file->version->toString().toLocal8Bit().constData(), p->toString().toLocal8Bit().constData());
					Symbols symbols(bcd, p->base);
					dumpEntry(outDirPath, symbols);
				}
			}
		}
	}
}


void Module::dumpEntry(QString outDirPath, Symbols& symbols) {
	quint64 version = symbols.version->value;

	QMap<CARD16, Entry*> entryMap;

	for(BTRecord* bt: symbols.bt.values()) {
		if (bt->info->tag == BTRecord::BodyInfo::Tag::EXTERNAL) {
			if (bt->tag == BTRecord::Tag::CALLABLE) {
				const BTRecord::Callable& callable = bt->getCallable();
				if (callable.tag == BTRecord::Callable::Tag::OUTER || callable.tag == BTRecord::Callable::Tag::INNER) {
					const CARD16        entryIndex = callable.entryIndex;
					const SERecord::Id& id         = callable.id->getValue().getId();
					const QString&      name       = id.hash->getValue().value;
					const SERecord&     ioType     = callable.ioType->getValue();
					const TransferMode  mode       = ioType.getCons().getTransfer().mode;

					if (mode == TransferMode::PROGRAM || mode == TransferMode::PROC) {
						Entry* entry = new Entry(entryIndex, name, mode);
						if (entryMap.contains(entryIndex)) {
							logger.fatal("Unexpected duplicate entryIndex %d", entryIndex);
							ERROR();
						}
						entryMap.insert(entryIndex, entry);
					} else {
						logger.fatal("Unexpected mode %s", SERecord::Cons::Transfer::toString(mode));
						ERROR();
					}
				}
			}
		}
	}

	// Sanity check - there is only one program
	QString moduleName;
	{
		int programCount = 0;
		for(Entry* entry: entryMap.values()) {
			if (entry->mode == TransferMode::PROGRAM) {
				if (programCount == 0) {
					programCount++;
					moduleName = entry->name;
				} else {
					logger.fatal("Unexpected duplicate program %s %s", moduleName.toLocal8Bit().constData(), entry->name.toLocal8Bit().constData());
					ERROR();
				}
			}
		}
	}
	// Sanity check - entryIndex in [0..size(entryMap))
	{
		CARD16 mapSize = (CARD16)entryMap.size();
		for(Entry* entry: entryMap.values()) {
			CARD16 entryIndex = entry->entryIndex;
			if (mapSize <= entryIndex) {
				logger.fatal("Unexpected entryIndex %d <= %d", mapSize, entryIndex);
				ERROR();
			}
		}
	}

	// Prepare output file
	QString outFilePath;
	{
		QDir outDir(outDirPath);
		if (!outDir.exists()) {
			logger.fatal("outDirPath does not exist. outDirPath = %s", outDir.absolutePath().toLocal8Bit().constData());
			ERROR();
		}

		outFilePath = outDir.absoluteFilePath(QString("%1-%2").arg(version).arg(moduleName));
		logger.info("outFilePath = %s", outFilePath.toLocal8Bit().constData());
	}

	QFile outFile(outFilePath);
	{
		bool result = outFile.open(QIODevice::OpenModeFlag::WriteOnly | QIODevice::OpenModeFlag::Truncate);
		if (!result) {
			logger.fatal("File open error %s", outFile.errorString().toLocal8Bit().constData());
			ERROR();
		}
	}

	QTextStream out(&outFile);
	for(Entry* entry: entryMap.values()) {
		out << version << " " << moduleName << " " << entry->entryIndex << " " << entry->name << endl;
	}
}

int main(int argc, char** argv) {
	logger.info("START");

	setSignalHandler();

	try {
		QString outDirPath("tmp/moduleEntry");
		logger.info("outDirPath %s", outDirPath.toLocal8Bit().constData());
		if (!QFile::exists(outDirPath)) {
			logger.fatal("outDirPath does not exist. outDirPath = %s", outDirPath.toLocal8Bit().constData());
			ERROR();
		}

		for(int i = 1; i < argc; i++) {
			QString filePath = argv[i];
			logger.info("path = %s", filePath.toLocal8Bit().constData());

			Module::dumpEntry(outDirPath, filePath);
		}
	} catch (Error& e) {
		logger.info("Error %s %d %s", e.file, e.line, e.func);
	} catch (...) {
		logger.info("Unknown exception");
	}
	
	logger.info("STOP");
	return 0;
}
