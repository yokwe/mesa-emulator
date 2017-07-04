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

int main(int, char** argv) {
	logger.info("START");
	QString path = argv[1];
	logger.info("path = %s", path.toLocal8Bit().constData());
	BCDFile* file = BCDFile::getInstance(path);
	BCD bcd(*file);

	int symbolBase = -1;
	if (Symbols::isSymbolsFile(bcd)) {
		logger.info("file is symbol file");
		symbolBase = 2;
	} else {
	   for (SGRecord e : bcd.sg.values()) {
			if (e.segClass != SGRecord::SegClass::SYMBOLS)
				continue;
			if (!e.file.isSelf())
				continue;

			logger.info("found symbol segment  %s", e.toString().toLocal8Bit().constData());
			symbolBase = e.base;
			break;
		}
	}
	if (symbolBase == -1) {
		logger.fatal("No Symbol Segment");
		ERROR();
	}
	
	Symbols symbols(bcd, symbolBase);
	logger.info("STOP");
}
