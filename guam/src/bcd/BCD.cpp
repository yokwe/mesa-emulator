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
// BCD.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("bcd");

#include "../mesa/Memory.h"

#include "BCD.h"

static const int MAX_BIT = 15;

CARD16 bitField(CARD16 word, int startBit, int stopBit) {
	if (startBit < 0)        ERROR();
	if (stopBit  < 0)        ERROR();
	if (stopBit  < startBit) ERROR();
	if (MAX_BIT  < startBit) ERROR();
	if (MAX_BIT  < stopBit)  ERROR();

	int shift  = MAX_BIT - stopBit;
	int mask   = ((int)(1L << (stopBit - startBit + 1)) - 1) << shift;

	return (CARD16)((word & mask) >> shift);
}
CARD16 bitField(CARD16 word, int startBit) {
	return bitField(word, startBit, startBit);
}


VersionStamp::VersionStamp(BCD& bcd) {
	net  = bcd.file.getCARD8();
	host = bcd.file.getCARD8();
	time = bcd.file.getCARD32();
	//	logger.info("VersionStamp  %02X %02X %08X", net, host, time);

	dateTime = QDateTime::fromTime_t(Util::toUnixTime(time));
}

QString VersionStamp::toString() {
	if (net == 0 && host == 0 && time == 0) return "#NULL#";
	return QString("%1#%2#%3%4").arg(dateTime.toString("yyyyMMdd")).arg(dateTime.toString("HHmmss")).arg(net, 2, 16, QChar('0')).arg(host, 2, 16, QChar('0'));
}

QString NameRecord::toString() {
	if (index == NullName) return "#NULL#";
	return name;
}

FTRecord::FTRecord(BCD& bcd, CARD16 index_) {
	CARD16 nameRecord = bcd.file.getCARD16();
	index   = index_;
	name    = bcd.ss[nameRecord].toString();
	version = VersionStamp(bcd);
	//	logger.info("%s  %3d  %s!", "FTReord", nameRecord, name.toLocal8Bit().constData());
}
QString FTRecord::toString() {
	if (index == FT_NULL) return "#NULL#";
	if (index == FT_SELF) return "#SELF#";
	return QString("%1 %2").arg(version.toString()).arg(name);
}


QMap<SGRecord::SegClass, QString> segClassMapInit() {
	QMap<SGRecord::SegClass, QString> ret;

	ret[SGRecord::SegClass::CODE]    = "CODE";
	ret[SGRecord::SegClass::SYMBOLS] = "SYMBOLS";
	ret[SGRecord::SegClass::AC_MAP]  = "AC_MAP";
	ret[SGRecord::SegClass::OTHER]   = "OTHER";

	return ret;
}
static QMap<SGRecord::SegClass, QString> segClassMap = segClassMapInit();
QString SGRecord::toString(SegClass value) {
	if (segClassMap.contains(value)) {
		return segClassMap[value];
	} else {
		logger.error("Unknown value = %d", (int)value);
		ERROR();
		return QString("%1").arg((int)value);
	}
};
SGRecord::SGRecord(BCD& bcd, CARD16 index_) {
	index = index_;

	fileIndex = bcd.file.getCARD16();
	file      = bcd.ft[fileIndex];

	base = bcd.file.getCARD16();

	CARD16 word = bcd.file.getCARD16();
	pages      = bitField(word, 0, 7);
	extraPages = bitField(word, 8, 13);
	segClass   = (SegClass)bitField(word, 14, 15);
}
QString SGRecord::toString() {
	if (index == SG_NULL) return "#NULL#";
	return QString("%1+%2+%3 %4 %5").arg(base, 4).arg(pages, 4).arg(extraPages, 4).arg(toString(segClass), -7).arg(file.toString());
}


BCD::BCD(BCDFile& bcdFile) : file(bcdFile) {
	versionIdent = file.getCARD16();
	if (versionIdent != VersionID) {
		logger.fatal("versionIdent %d", versionIdent);
		ERROR();
	}

	version = VersionStamp(*this);
	creator = VersionStamp(*this);

	CARD16 sourceFileIndex     = file.getCARD16();
	CARD16 unpackagedFileIndex = file.getCARD16();

	nConfigs = file.getCARD16();
	nModules = file.getCARD16();
	nImports = file.getCARD16();
	nExports = file.getCARD16();

	CARD16 flags  = file.getCARD16();
	nPages        = bitField(flags, 0, 7);
	definitions   = bitField(flags, 8);
	repackaged    = bitField(flags, 9);
	typeExported  = bitField(flags, 10);
	tableCompiled = bitField(flags, 11);
	spare4        = bitField(flags, 12, 15);

	firstDummy = file.getCARD16();
	nDummies   = file.getCARD16();
	ssOffset   = file.getCARD16();
	ssLimit    = file.getCARD16();
	ctOffset   = file.getCARD16();
	ctLimit    = file.getCARD16();
	mtOffset   = file.getCARD16();
	mtLimit    = file.getCARD16();
	impOffset  = file.getCARD16();
	impLimit   = file.getCARD16();
	expOffset  = file.getCARD16();
	expLimit   = file.getCARD16();
	enOffset   = file.getCARD16();
	enLimit    = file.getCARD16();
	sgOffset   = file.getCARD16();
	sgLimit    = file.getCARD16();
	ftOffset   = file.getCARD16();
	ftLimit    = file.getCARD16();
	spOffset   = file.getCARD16();
	spLimit    = file.getCARD16();
	ntOffset   = file.getCARD16();
	ntLimit    = file.getCARD16();
	typOffset  = file.getCARD16();
	typLimit   = file.getCARD16();
	tmOffset   = file.getCARD16();
	tmLimit    = file.getCARD16();
	fpOffset   = file.getCARD16();
	fpLimit    = file.getCARD16();
	lfOffset   = file.getCARD16();
	lfLimit    = file.getCARD16();
	atOffset   = file.getCARD16();
	atLimit    = file.getCARD16();
	apOffset   = file.getCARD16();
	apLimit    = file.getCARD16();

	//
	initializeNameRecord();
	initializeFTRecord();
	initializeSGRecord();

	sourceFile     = ft[sourceFileIndex];
	unpackagedFile = ft[unpackagedFileIndex];

	//
	logger.info("version        %s", version.toString().toLocal8Bit().constData());
	logger.info("creator        %s", creator.toString().toLocal8Bit().constData());
	logger.info("sourceFile     %s", sourceFile.toString().toLocal8Bit().constData());
	logger.info("unpackagedFile %s", unpackagedFile.toString().toLocal8Bit().constData());

	logger.info("nConfigs       %d", nConfigs);
	logger.info("nModules       %d", nModules);
	logger.info("nImports       %d", nImports);
	logger.info("nExports       %d", nExports);
	logger.info("nPages         %d", nPages);
	logger.info("flags          %s%s%s%s", definitions ? "definitions " : "", repackaged ? "repackaged " : "", typeExported ? "typeExported" : "", tableCompiled ? "tableCompiled " : "");
}

void BCD::initializeNameRecord() {
	int offset = ssOffset;
	int limit  = ssLimit;

	if (limit == 0) return;

	file.position(offset + 2);
	file.getCARD8();
	for(;;) {
		int bytePos = file.bytePosition();
		int pos = file.position();

		if ((offset + limit) <= pos) break; // position exceed limit

		int length = file.getCARD8();
		QString value;
		for(int i = 0; i < length; i++) {
	     	char data = file.getCARD8();
	    	QChar c(data);
	    	value.append(c);
		}

		int index = bytePos - (offset * 2) - 3;
		NameRecord record(index, value);
		ss[index] = record;

		logger.info("ss %4d %s!", index, record.toString().toLocal8Bit().constData());
	}

	// Add special
	ss[NameRecord::NullName] = NameRecord();
}

void BCD::initializeFTRecord() {
	int offset = ftOffset;
	int limit  = ftLimit;

	file.position(offset);
	for(;;) {
		int pos = file.position();
		if ((offset + limit) <= pos) break; // position exceed limit

		int index = pos - offset;
		FTRecord record(*this, index);
		ft[index] = record;

		logger.info("ft %3d %s", index, record.toString().toLocal8Bit().constData());
	}

	// Add special
	ft[FTRecord::FT_NULL] = FTRecord();
	ft[FTRecord::FT_SELF] = FTRecord(FTRecord::FT_SELF, "#SELF#", version);
}


void BCD::initializeSGRecord() {
	int offset = sgOffset;
	int limit  = sgLimit;

	file.position(offset);
	for(;;) {
		int pos = file.position();
		if ((offset + limit) <= pos) break; // position exceed limit

		int index = pos - offset;
		SGRecord record(*this, index);
		sg[index] = record;

		logger.info("sg %3d %s", index, record.toString().toLocal8Bit().constData());
	}

	// Add special
	sg[SGRecord::SG_NULL] = SGRecord();
}
