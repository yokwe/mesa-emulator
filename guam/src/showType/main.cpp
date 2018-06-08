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

#include "../symbols/BCDFile.h"
#include "../symbols/BCD.h"
#include "../symbols/Symbols.h"
#include "../symbols/DumpSymbol.h"

void PageFault(CARD32 ptr) {
	logger.fatal("%s %X", __FUNCTION__, ptr);
}
void WriteProtectFault(CARD32 ptr) {
	logger.fatal("%s %X", __FUNCTION__, ptr);
}

int main(int argc, char** argv) {
	logger.info("START");

	setSignalHandler();

	try {
		for(int i = 1; i < argc; i++) {
			QString path = argv[i];
			logger.info("path = %s", path.toLocal8Bit().constData());
			BCDFile* file = BCDFile::getInstance(path);
			// Skip not bcd file
			if (!file->isBCDFile()) {
				logger.warn("File is not BCD. filePath = %s", path.toLocal8Bit().constData());
				continue;
			}

			BCD* bcd = new BCD(file);

			int symbolBase = -1;
			if (file->isSymbolsFile()) {
				logger.info("file is symbol file");
				symbolBase = 2;

				Symbols symbols(bcd, symbolBase);
				ShowType::dump(&symbols);
			} else {
			   for (SGRecord* p : bcd->sg.values()) {
				   if (p->isNull()) continue;
				   if (p->segClass == SGRecord::SegClass::SYMBOLS) {
				       if (p->file->isSelf()) {
				    	   logger.info("found internal symbol segment  %s", p->toString().toLocal8Bit().constData());
				    	   symbolBase = p->base;

				    	   Symbols symbols(bcd, symbolBase);
				    	   ShowType::dump(&symbols);
					   } else {
				    	   logger.info("found external symbol segment  %s", p->toString().toLocal8Bit().constData());
					   }
				   }
				}
			}
		}
	} catch (Error& e) {
		logger.info("Error %s %d %s", e.file, e.line, e.func);
	} catch (...) {
		logger.info("Unknown exception");
	}

	logger.info("STOP");
}
