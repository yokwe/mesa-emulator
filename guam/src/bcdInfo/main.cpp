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

// Define PageFault and WriteProtectFault for BCDFile
void PageFault(CARD32 ptr) {
	logger.fatal("%s %X", __FUNCTION__, ptr);
	ERROR();
}
void WriteProtectFault(CARD32 ptr) {
	logger.fatal("%s %X", __FUNCTION__, ptr);
	ERROR();
}


void processFile(const QDir& outDir, const QFileInfo& fileInfo) {
	BCD bcd(fileInfo.filePath());

	// If it is not BCD file, just return
	if (!bcd.isBCDFile()) return;

	logger.info("%s %s", __FUNCTION__, fileInfo.filePath().toLocal8Bit().constData());

	QJsonObject jsonObject;

	//  Build jsonObject from bcd
	{
		BCDInfo bcdInfo(bcd);
		bcdInfo.setJsonValue(jsonObject);
	}

	// Sanity check of BCDInfo <-> QJsonObject conversion
	{

		// Create jsonObject1 from bcd -- BCD -> BCDInfo -> json
		QJsonObject jsonObject1;
		{
			BCDInfo bcdInfo1(bcd);
			bcdInfo1.setJsonValue(jsonObject1);
		}

		// Create jsonObject2 from jsonObject1 -- json -> BCDInfo -> json
		QJsonObject jsonObject2;
		{
			BCDInfo bcdInfo2(jsonObject1);
			bcdInfo2.setJsonValue(jsonObject2);
		}

		// Create sting representation of jsonObject1
		QJsonDocument jsonDoc1(jsonObject1);
		QByteArray fileContents1 = jsonDoc1.toJson(QJsonDocument::JsonFormat::Indented); // Compact or Indented

		// Create sting representation of jsonObject2
		QJsonDocument jsonDoc2(jsonObject2);
		QByteArray fileContents2 = jsonDoc2.toJson(QJsonDocument::JsonFormat::Indented); // Compact or Indented

		// Compare fileContents1 and fileContents2.  These value should be same.
		if (!(fileContents1.size() == fileContents2.size() || qstricmp((const char*)fileContents1, (const char*)fileContents2) == 0)) {
			logger.fatal("Unexpected");
			logger.fatal("fileContents1 = (%d) %s!", fileContents1.size(), fileContents1.constData());
			logger.fatal("fileContents2 = (%d) %s!", fileContents2.size(), fileContents2.constData());
			ERROR();
		}
	}

	// Create outDir if not exists.
	{
		if (!outDir.exists()) {
			QFileInfo outDirFileInfo(outDir.absolutePath());
			QString parentPath(outDirFileInfo.path());
			QString dirName(outDirFileInfo.fileName());

			logger.info("mkpath %s %s", parentPath.toLocal8Bit().constData(), dirName.toLocal8Bit().constData());
			QDir parentDir(parentPath);
			parentDir.mkpath(dirName);
		}
	}

	// Output jsonObect to file
	{
		QJsonDocument jsonDoc(jsonObject);
		QByteArray fileContents = jsonDoc.toJson(QJsonDocument::JsonFormat::Indented); // Compact or Indented

		QFileInfo outFileInfo(outDir, QString("%1.json").arg(fileInfo.fileName()));
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

			QFileInfo childOutFileInfo(outDir, childFileName);
			QDir childOutDir(childOutFileInfo.filePath());
			processDir(childOutDir, childFileInfo);
		}
		if (childFileInfo.isFile()) {
			if (childFileName.endsWith(".bcd") || childFileName.endsWith(".symbols")) {
				processFile(outDir, childFileInfo);
			}
		}
	}
}


int main(int argc, char** argv) {
	logger.info("START");

	setSignalHandler();

	try {
		QString outDirPath("tmp/bcdInfo");
		logger.info("outDirPath %s", outDirPath.toLocal8Bit().constData());

		QDir outDir(outDirPath);
		if (!outDir.exists()) {
			logger.fatal("outDir does not exist. outDir = %s", outDir.absolutePath().toLocal8Bit().constData());
			ERROR();
		}

		for(int i = 1; i < argc; i++) {
			QString path = argv[i];
			logger.info("path = %s", path.toLocal8Bit().constData());
			QFileInfo fileInfo(path);

			// Sanity check
			if (!fileInfo.exists()) {
				logger.fatal("path doesn't exist = %s", fileInfo.filePath().toLocal8Bit().constData());
				ERROR();
			}

			if (fileInfo.isFile()) {
				processFile(outDir, fileInfo);
			}
			if (fileInfo.isDir()) {
				QString fileName(fileInfo.fileName());
				// Special case : trailing slash
				if (fileName.length() == 0) {
					fileInfo = QFileInfo(fileInfo.path());
					fileName = fileInfo.fileName();
				}

				QFileInfo rootOutDirFileInfo(outDir, fileName);
				QDir rootOutDir(rootOutDirFileInfo.filePath());

				logger.info("rootOutDir = %s", rootOutDirFileInfo.filePath().toLocal8Bit().constData());
				processDir(rootOutDir, fileInfo);
			}
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

