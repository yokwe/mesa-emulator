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
// DumpSymbol.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("dumpSymbol");

#include "DumpSymbol.h"

#include "ShowType.h"

#include "BCDFile.h"
#include "CTXIndex.h"
#include "HTIndex.h"
#include "MDIndex.h"
#include "SEIndex.h"

static int indentWidth = 4;
static int indentLevel = 0;
void DumpSymbol::nest() {
	indentLevel++;
}
void DumpSymbol::unnest() {
	indentLevel--;
	if (indentLevel < 0) ERROR();
}
QString DumpSymbol::indent() {
	return QString("%1").arg("", indentWidth * indentLevel);
}

void DumpSymbol::dumpSymbol(QString filePath, QString outDirPath) {
	// Prepare output file
	QString outFilePath;
	{
		QDir outDir(outDirPath);
		if (!outDir.exists()) {
			logger.fatal("outDirPath does not exist. outDirPath = %s", outDir.absolutePath().toLocal8Bit().constData());
			ERROR();
		}

		QString fileName(QFileInfo(filePath).baseName() + ".log");
		outFilePath = outDir.absoluteFilePath(fileName);
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


	// Check existence of file
	if (!QFile::exists(filePath)) {
		logger.fatal("File does not exist. pathFile = %s", filePath.toLocal8Bit().constData());
		ERROR();
	}
	BCDFile* file = BCDFile::getInstance(filePath);

	// Read bcd file
	BCD* bcd = new BCD(file);

	// Locate target segment
	int symbolBase = -1;
	if (Symbols::isSymbolsFile(bcd)) {
		logger.info("file is symbol file");
		symbolBase = 2;
	} else {
	   for (SGRecord* p : bcd->sg.values()) {
			if (p->segClass != SGRecord::SegClass::SYMBOLS)
				continue;
			if (p->file->isSelf()) {
				logger.info("found symbol segment  %s", p->toString().toLocal8Bit().constData());
				symbolBase = p->base;
				break;
			}
		}
	}
	if (symbolBase == -1) {
		logger.fatal("No Symbol Segment");
		ERROR();
	}

//	int indentWidth = 4;
//	int indentLevel = 0;

	// Read symbols
	Symbols symbols(bcd, symbolBase);

	// Print Header
	{
		MDRecord* self = symbols.md[MDIndex::MD_OWN];
		//FTRecord* self = bcd->ft[FTRecord::FT_SELF];

		QFileInfo info(filePath);

		out << "--" << endl;
		out << QString("-- File   %1 %2").arg(self->stamp->toString()).arg(self->fileId->getValue().value) << endl;
		if (bcd->sourceFile) out << QString("-- Source %1 %2").arg(bcd->sourceFile->version->toString()).arg(bcd->sourceFile->name) << endl;
		out << "--" << endl;
		out << endl;
	}

	// Print Directory
	{
		int mdSize = symbols.md.size();
		if (1 < mdSize) {
			out << "DIRECTORY" << endl;
			nest();
			QListIterator<CARD16> i(symbols.md.keys());
			for(;;) {
				if (!i.hasNext()) {
					break;
				} else {
					CARD16 index = i.next();
					if (index == MDIndex::MD_OWN) continue;
	                MDRecord* e = symbols.md[index];

	                out << indent() << QString("%1 %2%3").arg(e->moduleId->getValue().value, -30).arg(e->stamp->toString()).arg(i.hasNext() ? "," : ";") << endl;
				}
			}
		}
		unnest();
	}

	// Module body
    {
		MDRecord* self = symbols.md[MDIndex::MD_OWN];

		out << indent() << QString("%1 %2 = BEGIN").arg(self->moduleId->getValue().value).arg(self->stamp->toString()) << endl;
    }
    nest();

    const CTXRecord& outerCtx = symbols.outerCtx->getValue();
    logger.info(QString::asprintf("ctx %3d - %d", outerCtx.getIndex(), outerCtx.level).toLocal8Bit().constData());

    for (const SEIndex* sei = outerCtx.seList; !sei->isNull(); sei = sei->nextSe()) {
    	out << indent();
    	ShowType::printSym(out, sei, ": ");
    	out << ";" << endl;
    }

    unnest();
    out << "END." << endl;
}
