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

static QMap<QString, BCDInfo> bcdMap;
void processSymbols(const QDir& outDir, const QString& name, const QString& version, CARD16 base) {
	if (bcdMap.contains(version)) {
		QString path = bcdMap[version].path;
		BCD bcd(path);

		if (Symbols::isSymbolsSegment(&bcd, base)) {
			// Found symbols segment
			Symbols symbols(&bcd, base);

			QJsonObject jsonObject;
			SymInfo symInfo(symbols);
			symInfo.setJsonValue(jsonObject);

			// Output jsonObect to file
			{
				QJsonDocument jsonDoc(jsonObject);
				QByteArray fileContents = jsonDoc.toJson(QJsonDocument::JsonFormat::Indented); // Compact or Indented

				QFileInfo outFileInfo(outDir, QString("%1-%2-%3.json").arg(name).arg(version).arg(base));
				logger.info("outFile %s", outFileInfo.fileName().toLocal8Bit().constData());

				QString outFilePath = outFileInfo.filePath();
				QFile outFile(outFilePath);
				if (!outFile.open(QIODevice::OpenModeFlag::WriteOnly)) {
					logger.fatal("File open error %s", outFile.errorString().toLocal8Bit().constData());
					ERROR();
				}
				int fileLength = outFile.write(fileContents);
				if (fileLength != fileContents.size()) {
					logger.fatal("File write error %s  fileLength = %d  fileContents = %d", outFile.errorString().toLocal8Bit().constData(), fileLength, fileContents.size());
					ERROR();
				}
				outFile.close();
			}
		} else {
			logger.fatal("Unexpected not symbol segment");
			logger.fatal("path = %s  base = %d", path.toLocal8Bit().constData(), base);
			ERROR();
		}
	} else {
		logger.warn("Not in bcdMap  %llu", version);
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

		// Check symbols
		int countBCD      = 0;
		int countMT       = 0;
		int countCodeSelf = 0;
		int countCode     = 0;
		int countCodeMiss = 0;
		int countSym      = 0;
		int countSymSelf  = 0;
		int countSymMiss = 0;
		for(BCDInfo bcdInfo: bcdMap.values()) {
			countBCD++;
			logger.info("bcdInfo %s", bcdInfo.path.toLocal8Bit().constData());
			for(MTInfo mt: bcdInfo.mtList) {
				countMT++;
				SGInfo code = mt.code.sg;
				SGInfo sym  = mt.sseg;

				if (code.file.isNull()) {
					//
				} else if (code.file.isSelf()) {
					countCodeSelf++;
				} else {
					countCode++;
					if (bcdMap.contains(code.file.version)) {
						//
					} else {
						countCodeMiss++;
						logger.warn("No code %llu %-32s  %s", code.file.version, mt.name.toLocal8Bit().constData(), bcdInfo.path.toLocal8Bit().constData());
					}
				}

				if (sym.file.isNull()) {
					//
				} else if (sym.file.isSelf()) {
					countSymSelf++;
					logger.info("bcdInfo sym S %s %llu, %d", bcdInfo.path.toLocal8Bit().constData(), bcdInfo.version, sym.base);
					processSymbols(outDir, mt.name, bcdInfo.version, sym.base);
				} else {
					countSym++;
					logger.info("bcdInfo sym E %s %llu, %d", bcdInfo.path.toLocal8Bit().constData(), sym.file.version, sym.base);
					logger.info("XX  %llu  %lld  %s", sym.file.version, sym.file.version, QString("%1").arg(sym.file.version).toLocal8Bit().constData());
					processSymbols(outDir, mt.name, sym.file.version, sym.base);
				}
			}
		}
		logger.info("countBCD      = %5d  countMT   = %5d", countBCD, countMT);
		logger.info("countCodeSelf = %5d  countCode = %5d  countCodeMiss = %5d", countCodeSelf, countCode, countCodeMiss);
		logger.info("countSymSelf  = %5d  countSym  = %5d  countSymMiss  = %5d", countSymSelf,  countSym,  countSymMiss);


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

