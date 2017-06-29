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
// BcdOps.cpp
//


#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("bcdops");

#include "../mesa/Memory.h"

#include "BCDOps.h"

QString VersionStamp::toString() {
	return QString("%1#%2# %3").arg(net, 0, 8).arg(host, 0, 8).arg(time.toString("yyyy-MM-dd HH:mm:ss"));
}

QString FTRecord::toString() {
	return QString("%1 %2").arg(name, -20).arg(stamp.toString(), 30);
}

QString ENRecord::toString() {
	QString ret;

	CARD16 size = initialPC.size();
	for(CARD16 i = 0; i < size; i++) {
		ret.append(QString(i ? ", %1" : "%1").arg(initialPC[i]));
	}
	return ret;
}

QString SGRecord::toString() {
	//SegClass: TYPE = {code, symbols, acMap, other};
	const char* sc;
	switch(segClass) {
	case BcdDefs::SC_code:
		sc = "code";
		break;
	case BcdDefs::SC_symbols:
		sc = "symbols";
		break;
	case BcdDefs::SC_acMap:
		sc = "acMap";
		break;
	case BcdDefs::SC_other:
		sc = "other";
		break;
	default:
		ERROR();
	}
	return QString("%1 %2+%3+%4 %5").arg(file.toString()).arg(base).arg(pages).arg(extraPages).arg(sc);
}


BCDOps::BCDOps(CARD32 ptr) {
	if (Memory::isVacant(ptr)) {
		logger.fatal("ptr is not mapped");
		ERROR();
	}

	readObject(ptr, &header);
	// Build ss
	{
		const CARD32 limit  = header.ssLimit;
		const CARD32 offset = header.ssOffset;
		logger.info("ss  %5d %5d", offset, limit);

		CARD32 p = ptr + offset;
		CARD32 bytePos    = 2 * sizeof(CARD16) + 1;
		CARD32 byteLimit  = limit * sizeof(CARD16);

//		logger.info("bytePos   %5d", bytePos);
//		logger.info("byteLimit %5d", byteLimit);

		for(;;) {
			if (byteLimit <= bytePos) break;

			CARD16 index = bytePos - 3;
			CARD32 len = FetchByte(p, bytePos++);
			QString value;
			for(CARD32 i = 0; i < len; i++) {
				CARD8 byte = FetchByte(p, bytePos++);
				value.append(byte);
			}
			ss[index] = value;

//			logger.info("ss %5d %3d %s!", index, len, value.toLocal8Bit().constData());
		}
	}
	// build ft
	{
		const CARD32 limit  = header.ftLimit;
		const CARD32 offset = header.ftOffset;
//		logger.info("ft  %5d %5d", offset, limit);

		CARD32 posOffset = ptr + offset;
		CARD32 posLimit  = ptr + offset + limit;

		CARD32 pos       = posOffset;

		for(;;) {
			if (posLimit <= pos) {
				if (posLimit != pos) {
					ERROR();
				}
				break;
			}
			CARD16 nameRecord;
			TimeStamp::Stamp stamp;

			CARD16 index = pos - offset;

			READ_OBJECT(pos, nameRecord);
			READ_OBJECT(pos, stamp);

			VersionStamp versionStamp(stamp);
			FTRecord ftRecord(ss[nameRecord], versionStamp);
			ft[index] = ftRecord;

			logger.info("ft %5d %s", index, ftRecord.toString().toLocal8Bit().constData());
		}

		// Add FTSelf
		{
//			QString sourceName = ft[header.sourceFile].name;
//			QString name = sourceName.replace(".mesa", ".bcd").replace(".config", ".bcd").prepend("#SELF#");
			QString name = "#SELF#";
			FTRecord record(name, header.version);
			ft[BcdDefs::FTSelf] = record;
		}
	}
	// build en
	{
		const CARD32 limit  = header.enLimit;
		const CARD32 offset = header.enOffset;
//		logger.info("en  %5d %5d", offset, limit);

		CARD32 posOffset = ptr + offset;
		CARD32 posLimit  = ptr + offset + limit;

		CARD32 pos       = posOffset;

		for(;;) {
			if (posLimit <= pos) {
				if (posLimit != pos) {
					ERROR();
				}
				break;
			}
			CARD16 nEntries;
			ENRecord enRecord;

			CARD16 index = pos - offset;

			READ_OBJECT(pos, nEntries);

			for(CARD16 i = 0; i < nEntries; i++) {
				CARD16 pc;
				READ_OBJECT(pos, pc);
				enRecord.initialPC.append(pc);
			}
			en[index] = enRecord;

			logger.info("en %5d  (%d)%s", index, enRecord.initialPC.size(), enRecord.toString().toLocal8Bit().constData());
		}
	}
	// build sg
	{
		const CARD32 limit  = header.sgLimit;
		const CARD32 offset = header.sgOffset;
		logger.info("sg  %5d %5d", offset, limit);

		CARD32 posOffset = ptr + offset;
		CARD32 posLimit  = ptr + offset + limit;

		CARD32 pos       = posOffset;

		for(;;) {
			if (posLimit <= pos) {
				if (posLimit != pos) {
					ERROR();
				}
				break;
			}

			CARD16 index = pos - offset;

			BcdDefs::SGRecord sgRecord;
			READ_OBJECT(pos, sgRecord.file);
			READ_OBJECT(pos, sgRecord.base);
			READ_OBJECT(pos, sgRecord.u2);

			SGRecord record;
			record.file       = ft[sgRecord.file];
			record.base       = sgRecord.base;
			record.pages      = sgRecord.pages;
			record.extraPages = sgRecord.extraPages;
			record.segClass   = sgRecord.segClass;
			sg[index] = record;

			logger.info("sg %5d %s", index, record.toString().toLocal8Bit().constData());
		}
	}

	version     = VersionStamp(header.version);
	creator     = VersionStamp(header.creator);

	sourceFile  = ft[header.sourceFile];
	nPages      = header.nPages;
	firstDummy  = header.firstdummy;
	definitions = header.definitions;
	nConfigs    = header.nConfigs;
	nModules    = header.nModules;
	nImports    = header.nImports;
	nExports    = header.nExports;

	logger.info("version    %s", version.toString().toLocal8Bit().constData());
	logger.info("creator    %s", creator.toString().toLocal8Bit().constData());
	logger.info("sourceFile %s", sourceFile.toString().toLocal8Bit().constData());
	logger.info("nPages     %4d", nPages);
	logger.info("firstDummy %4d", firstDummy);
	logger.info("definitions%4d", definitions);
	logger.info("nConfigs   %4d", nConfigs);
	logger.info("nModules   %4d", nModules);
	logger.info("nImports   %4d", nImports);
	logger.info("nExports   %4d", nExports);


}
