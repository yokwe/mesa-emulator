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
#include "BCDFile.h"

Stamp* Stamp::getInstance(BCD* bcd) {
	CARD16 net  = bcd->file->getCARD8();
	CARD16 host = bcd->file->getCARD8();
	CARD32 time = bcd->file->getCARD32();
	//	logger.info("VersionStamp  %02X %02X %08X", net, host, time);

	QDateTime dateTime = QDateTime::fromTime_t(Util::toUnixTime(time));
	return new Stamp(net, host, time, dateTime);
}
Stamp* Stamp::getNull() {
	return new Stamp(0, 0, 0, QDateTime());
}
QString Stamp::toString() const {
	if (isNull()) return "#NULL#";
	return QString("%1#%2#%3%4").arg(dateTime.toString("yyyyMMdd")).arg(dateTime.toString("HHmmss")).arg(net, 2, 16, QChar('0')).arg(host, 2, 16, QChar('0'));
}

QString NameRecord::toString() {
	static QString null("#NULL#");
	if (index == NullName) return null;
	return name;
}

FTRecord* FTRecord::getInstance(BCD* bcd, CARD16 index_) {
	CARD16  nameRecord = bcd->file->getCARD16();
	Stamp*  version    = Stamp::getInstance(bcd) ;

	CARD16  index   = index_;
	QString name    = bcd->ss[nameRecord]->toString();
	//	logger.info("%s  %3d  %s!", "FTReord", nameRecord, name.toLocal8Bit().constData());
	return new FTRecord(index, name, version);
}
FTRecord* FTRecord::getNull() {
	static FTRecord ret(FT_NULL, "#NULL", 0);
	return &ret;
}
QString FTRecord::toString() const {
	if (index == FT_NULL) return "#NULL#";
	if (index == FT_SELF) return "#SELF#";
	return QString("%1 %2").arg(this->version->toString()).arg(name);
}


QString SGRecord::toString(SegClass value) {
	static QMap<SGRecord::SegClass, QString> map({
		{SegClass::CODE,    "CODE"},
		{SegClass::SYMBOLS, "SYMBOLS"},
		{SegClass::AC_MAP,  "AC_MAP"},
		{SegClass::OTHER,   "OTHER"},
	});
	if (map.contains(value)) {
		return map[value];
	} else {
		logger.error("Unknown value = %d", (int)value);
		ERROR();
		return QString("%1").arg((int)value);
	}
};
SGRecord* SGRecord::getInstance(BCD* bcd, CARD16 index_) {
	CARD16 index = index_;

	CARD16 fileIndex = bcd->file->getCARD16();
	FTRecord* file      = bcd->ft[fileIndex];

	CARD16 base = bcd->file->getCARD16();

	CARD16 word = bcd->file->getCARD16();
	CARD16 pages      = bitField(word, 0, 7);
	CARD16 extraPages = bitField(word, 8, 13);

	SegClass segClass   = (SegClass)bitField(word, 14, 15);

	return new SGRecord(index, fileIndex, file, base, pages, extraPages, segClass);
}
SGRecord* SGRecord::getNull() {
	return new SGRecord(SG_NULL, 0, 0, 0, 0, 0, (SegClass)0);
}
QString SGRecord::toString() const {
	if (index == SG_NULL) return "#NULL#";
	return QString("%1+%2+%3 %4 %5").arg(base, 4).arg(pages, 4).arg(extraPages, 4).arg(toString(segClass), -7).arg(file->toString());
}


BCD::BCD(BCDFile* bcdFile) : file(bcdFile) {
	versionIdent = file->getCARD16();
	if (versionIdent != VersionID) {
		logger.fatal("versionIdent %d", versionIdent);
		ERROR();
	}

	version = Stamp::getInstance(this);
	creator = Stamp::getInstance(this);

	CARD16 sourceFileIndex     = file->getCARD16();
	CARD16 unpackagedFileIndex = file->getCARD16();

	nConfigs = file->getCARD16();
	nModules = file->getCARD16();
	nImports = file->getCARD16();
	nExports = file->getCARD16();

	CARD16 flags  = file->getCARD16();
	nPages        = bitField(flags, 0, 7);
	definitions   = bitField(flags, 8);
	repackaged    = bitField(flags, 9);
	typeExported  = bitField(flags, 10);
	tableCompiled = bitField(flags, 11);
	spare4        = bitField(flags, 12, 15);

	firstDummy = file->getCARD16();
	nDummies   = file->getCARD16();
	ssOffset   = file->getCARD16();
	ssLimit    = file->getCARD16();
	ctOffset   = file->getCARD16();
	ctLimit    = file->getCARD16();
	mtOffset   = file->getCARD16();
	mtLimit    = file->getCARD16();
	impOffset  = file->getCARD16();
	impLimit   = file->getCARD16();
	expOffset  = file->getCARD16();
	expLimit   = file->getCARD16();
	enOffset   = file->getCARD16();
	enLimit    = file->getCARD16();
	sgOffset   = file->getCARD16();
	sgLimit    = file->getCARD16();
	ftOffset   = file->getCARD16();
	ftLimit    = file->getCARD16();
	spOffset   = file->getCARD16();
	spLimit    = file->getCARD16();
	ntOffset   = file->getCARD16();
	ntLimit    = file->getCARD16();
	typOffset  = file->getCARD16();
	typLimit   = file->getCARD16();
	tmOffset   = file->getCARD16();
	tmLimit    = file->getCARD16();
	fpOffset   = file->getCARD16();
	fpLimit    = file->getCARD16();
	lfOffset   = file->getCARD16();
	lfLimit    = file->getCARD16();
	atOffset   = file->getCARD16();
	atLimit    = file->getCARD16();
	apOffset   = file->getCARD16();
	apLimit    = file->getCARD16();

	//
	initializeNameRecord();
	initializeFTRecord();
	initializeSGRecord();

	sourceFile     = ft[sourceFileIndex];
	unpackagedFile = ft[unpackagedFileIndex];

	//
	logger.info("version        %s", version->toString().toLocal8Bit().constData());
	logger.info("creator        %s", creator->toString().toLocal8Bit().constData());
	logger.info("sourceFile     %s", sourceFile->toString().toLocal8Bit().constData());
	logger.info("unpackagedFile %s", unpackagedFile->toString().toLocal8Bit().constData());

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

	file->position(offset + 2);
	file->getCARD8();
	for(;;) {
		int bytePos = file->bytePosition();
		int pos = file->position();

		if ((offset + limit) <= pos) break; // position exceed limit

		int length = file->getCARD8();
		QString value;
		for(int i = 0; i < length; i++) {
	     	char data = file->getCARD8();
	    	QChar c(data);
	    	value.append(c);
		}

		int index = bytePos - (offset * 2) - 3;
		NameRecord* record = new NameRecord(index, value);
		ss[index] = record;

		logger.info("ss %4d %s!", index, record->toString().toLocal8Bit().constData());
	}

	// Add special
	ss[NameRecord::NullName] = new NameRecord();
}

void BCD::initializeFTRecord() {
	int offset = ftOffset;
	int limit  = ftLimit;

	file->position(offset);
	for(;;) {
		int pos = file->position();
		if ((offset + limit) <= pos) break; // position exceed limit

		int index = pos - offset;
		FTRecord* record = FTRecord::getInstance(this, index);
		ft[index] = record;

		logger.info("ft %3d %s", index, record->toString().toLocal8Bit().constData());
	}

	// Add special
	ft[FTRecord::FT_NULL] = FTRecord::getNull();
	ft[FTRecord::FT_SELF] = new FTRecord(FTRecord::FT_SELF, "#SELF#", version);
}


void BCD::initializeSGRecord() {
	int offset = sgOffset;
	int limit  = sgLimit;

	file->position(offset);
	for(;;) {
		int pos = file->position();
		if ((offset + limit) <= pos) break; // position exceed limit

		int index = pos - offset;
		SGRecord* record = SGRecord::getInstance(this, index);
		sg[index] = record;

		logger.info("sg %3d %s", index, record->toString().toLocal8Bit().constData());
	}

	// Add special
	sg[SGRecord::SG_NULL] = SGRecord::getNull();
}