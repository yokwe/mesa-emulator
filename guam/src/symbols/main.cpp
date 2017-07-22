/*
Copyright (c) 2017, Yasuhiro Hasegawa
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

#include "BCDFile.h"
#include "BCD.h"
#include "Symbols.h"
#include "DumpSymbol.h"

int main(int argc, char** argv) {
	logger.info("START");

	QString outDirPath("tmp/symbol");
	logger.info("outDirPath %s", outDirPath.toLocal8Bit().constData());
	if (!QFile::exists(outDirPath)) {
		logger.fatal("outDirPath does not exist. outDirPath = %s", outDirPath.toLocal8Bit().constData());
		ERROR();
	}

	for(int i = 1; i < argc; i++) {
		QString filePath = argv[i];
		logger.info("path = %s", filePath.toLocal8Bit().constData());

		DumpSymbol::dumpSymbol(filePath, outDirPath);
	}
	
	logger.info("STOP");
	return 0;
}


//int main(int argc, char** argv) {
//	logger.info("START");
//
//	for(int i = 1; i < argc; i++) {
//		QString path = argv[i];
//		logger.info("path = %s", path.toLocal8Bit().constData());
//		BCDFile* file = BCDFile::getInstance(path);
//		BCD* bcd = new BCD(file);
//
//		int symbolBase = -1;
//		if (Symbols::isSymbolsFile(bcd)) {
//			logger.info("file is symbol file");
//			symbolBase = 2;
//		} else {
//		   for (SGRecord* p : bcd->sg.values()) {
//				if (p->segClass != SGRecord::SegClass::SYMBOLS)
//					continue;
//				if (!p->file->isSelf())
//					continue;
//
//				logger.info("found symbol segment  %s", p->toString().toLocal8Bit().constData());
//				symbolBase = p->base;
//				break;
//			}
//		}
//		if (symbolBase == -1) {
//			logger.fatal("No Symbol Segment");
//			ERROR();
//		}
//
//		Symbols symbols(bcd, symbolBase);
//		ShowType::dump(&symbols);
//	}
//
//	logger.info("STOP");
//}
