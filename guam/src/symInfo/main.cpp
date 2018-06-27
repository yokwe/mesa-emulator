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

#include "../symbols/BCD.h"
#include "../symbols/Symbols.h"

#include "../bcd/BCDInfo.h"
#include "../bcd/SymInfo.h"

// Define PageFault and WriteProtectFault for BCDFile
void PageFault(CARD32 ptr) {
	logger.fatal("%s %X", __FUNCTION__, ptr);
	ERROR();
}
void WriteProtectFault(CARD32 ptr) {
	logger.fatal("%s %X", __FUNCTION__, ptr);
	ERROR();
}

static QList<BCDInfo> bcdAll;

void processFile(const QDir& /*outDir*/, const QFileInfo& fileInfo) {
	QFile jsonFile(fileInfo.filePath());

	if (!jsonFile.open(QIODevice::ReadOnly)) {
		logger.fatal("File open error - %s", jsonFile.errorString().toLocal8Bit().constData());
		logger.fatal("path = %s", fileInfo.filePath().toLocal8Bit().constData());
		ERROR();
	}

	QByteArray fileContents = jsonFile.readAll();
	QJsonParseError jsonParseError;
	QJsonDocument jsonDocument(QJsonDocument::fromJson(fileContents, &jsonParseError));
	if (jsonDocument.isNull()) {
		logger.fatal("Parse error %s", jsonParseError.errorString().toLocal8Bit().constData());
		logger.fatal("path = %s", fileInfo.filePath().toLocal8Bit().constData());
		ERROR();
	}
	if (!jsonDocument.isObject()) {
		logger.fatal("Not Object - array %d  empty %d  null %d  object %d", jsonDocument.isArray(), jsonDocument.isEmpty(), jsonDocument.isNull(), jsonDocument.isObject());
		logger.fatal("path = %s", fileInfo.filePath().toLocal8Bit().constData());
		ERROR();
	}
	QJsonObject jsonObject(jsonDocument.object());

	BCDInfo bcdInfo(jsonObject);

	bcdAll.append(bcdInfo);
}


void processDir(const QDir& outDir, const QFileInfo& parentFileInfo) {
	QDir parentDir(parentFileInfo.filePath());

	QList<QFileInfo> childFileInfoList = parentDir.entryInfoList(QDir::Filter::NoFilter, QDir::SortFlag::Name);
	for(QFileInfo childFileInfo: childFileInfoList) {
		QString childFileName(childFileInfo.fileName());
		if (childFileInfo.isDir()) {
			// Return if file is . or ..
			if (childFileName == ".") continue;
			if (childFileName == "..") continue;

			processDir(outDir, childFileInfo);
		}
		if (childFileInfo.isFile()) {
			if (childFileName.endsWith(".json")) {
				processFile(outDir, childFileInfo);
			}
		}
	}
}


int main(int /*argc*/, char** /*argv*/) {
	logger.info("START");

	setSignalHandler();

	try {
		QString outDirPath("tmp/symInfo");
		logger.info("outDirPath %s", outDirPath.toLocal8Bit().constData());

		// Sanity check
		{
			QFileInfo fileInfo(outDirPath);
			if (!fileInfo.exists()) {
				logger.fatal("outDirPath does not exist. outDirPath = %s", outDirPath.toLocal8Bit().constData());
				ERROR();
			}
			if (!fileInfo.isDir()) {
				logger.fatal("inDirPath is not directory. outDirPath = %s", outDirPath.toLocal8Bit().constData());
				ERROR();
			}
		}

		QDir outDir(outDirPath);

		QString inDirPath("tmp/bcdInfo");
		logger.info("inDirPath %s", inDirPath.toLocal8Bit().constData());

		// Sanity check
		{
			QFileInfo fileInfo(inDirPath);
			if (!fileInfo.exists()) {
				logger.fatal("inDirPath does not exist. outDirPath = %s", inDirPath.toLocal8Bit().constData());
				ERROR();
			}
			if (!fileInfo.isDir()) {
				logger.fatal("inDirPath is not directory. outDirPath = %s", inDirPath.toLocal8Bit().constData());
				ERROR();
			}
		}

		processDir(outDir, QFileInfo(inDirPath));
		logger.info("bcdAll %d", bcdAll.size());

		QMap<quint64, BCDInfo> bcdMap;
		{
			for(BCDInfo& bcd: bcdAll) {
				if (bcdMap.contains(bcd.version)) {
					// Sanity check
					BCDInfo old = bcdMap[bcd.version];
					if (old.hash != old.hash) {
						logger.fatal("Duplicate %llu", bcd.version);
						logger.fatal("          %s  %s", bcd.hash.toLocal8Bit().constData(), bcd.path.toLocal8Bit().constData());
						logger.fatal("          %s  %s", old.hash.toLocal8Bit().constData(), old.path.toLocal8Bit().constData());
						ERROR();
					}
				} else {
					bcdMap.insert(bcd.version, bcd);
				}
			}
			logger.info("bcdMap %d", bcdMap.size());
		}

	} catch (Error& e) {
		logger.info("Error %s %d %s", e.file, e.line, e.func);
		return -1;
	} catch (...) {
		logger.info("Unknown exception");
		return -1;
	}
	
	logger.info("STOP");
	return 0;
}

