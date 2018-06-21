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

	QDate sdate = dateTime.date();
	int y = sdate.year();
	int m = sdate.month();
	int d = sdate.day();
	QTime stime = dateTime.time();
	int H = stime.hour();
	int M = stime.minute();
	int S = stime.second();

	quint64 value = 0;
	// To prevent overflow of value, use last 2 digts of year
	value = (value *  100) + (y % 100);
	value = (value *  100) + m;
	value = (value *  100) + d;
	value = (value *  100) + H;
	value = (value *  100) + M;
	value = (value *  100) + S;
	value = (value * 1000) + net;
	value = (value * 1000) + host;

	return new Stamp(net, host, time, dateTime, value);
}
Stamp* Stamp::getNull() {
	return new Stamp(0, 0, 0, QDateTime(), 0);
}
QString Stamp::toString() const {
	if (isNull()) return "#NULL#";
	return QString("%1").arg(value);
//	return QString("%1#%2#%3#%4").arg(dateTime.toString("yyyyMMdd")).arg(dateTime.toString("HHmmss")).arg(net, 3, 10, QChar('0')).arg(host, 3, 10, QChar('0'));
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
	QString name    = bcd->getName(nameRecord);
	return new FTRecord(index, name, version);
}
FTRecord* FTRecord::getNull() {
	static FTRecord ret(FT_NULL, "#NULL", 0);
	return &ret;
}
QString FTRecord::toString() const {
	if (index == FT_NULL) return "#NULL#";
	if (index == FT_SELF) return "#SELF#";
	return QString("[%1 %2]").arg(this->version->toString()).arg(name);
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
		logger.fatal("Unknown value = %d", (int)value);
		ERROR();
	}
};
SGRecord* SGRecord::getInstance(BCD* bcd, CARD16 index_) {
	CARD16 index = index_;

	CARD16    fileIndex  = bcd->file->getCARD16();
	FTRecord* file       = bcd->getFTRecord(fileIndex);

	CARD16    base       = bcd->file->getCARD16();

	CARD16    word       = bcd->file->getCARD16();
	CARD16    pages      = bitField(word, 0, 7);
	CARD16    extraPages = bitField(word, 8, 13);

	SegClass segClass   = (SegClass)bitField(word, 14, 15);

	return new SGRecord(index, fileIndex, file, base, pages, extraPages, segClass);
}
SGRecord* SGRecord::getNull() {
	return new SGRecord(SG_NULL, 0, 0, 0, 0, 0, (SegClass)0);
}
QString SGRecord::toString() const {
	if (index == SG_NULL) return "#NULL#";
	return QString("[%1+%2+%3 %4 %5]").arg(base, 4).arg(pages, 4).arg(extraPages, 4).arg(toString(segClass), -7).arg(file->toString());
}


ENRecord* ENRecord::getInstance(BCD* bcd, CARD16 index_) {
	CARD16 n = bcd->file->getCARD16();
	QVector<CARD16> initialPC_(n);

	for(CARD16 i = 0; i < n; i++) {
		initialPC_[i] = bcd->file->getCARD16();
	}

	return new ENRecord(index_, initialPC_);
}
ENRecord* ENRecord::getNull() {
	QVector<CARD16> initialPC_(0);
	return new ENRecord(EN_NULL, initialPC_);
}
QString ENRecord::toString() const {
	if (index == EN_NULL) return "#NULL#";

	QString ret;
	ret.append(QString("(%1)[").arg(initialPC.size()));
	for(CARD16 i = 0; i < initialPC.size(); i++) {
		if (0 < i) ret.append(", ");
		ret.append(QString("%1").arg(initialPC[i]));
	}
	ret.append(']');

	return ret;
}


CodeDesc* CodeDesc::getInstance(BCD* bcd) {
	CARD16    sgiIndex = bcd->file->getCARD16();
	SGRecord* sgi      = bcd->getSGRecord(sgiIndex);
	CARD16    offset   = bcd->file->getCARD16();
	CARD16    length   = bcd->file->getCARD16();

	return new CodeDesc(sgi, offset, length);
}
QString CodeDesc::toString() const {
	return QString("[%1 %2+ %3]").arg(sgi->toString()).arg(offset, 5).arg(length, 5);
}


MTRecord* MTRecord::getInstance(BCD* bcd, CARD16 index_) {
	CARD16    index        = index_;
	CARD16    nameIndex    = bcd->file->getCARD16();
	QString   name         = bcd->getName(nameIndex);
	CARD16    fileIndex    = bcd->file->getCARD16();
	FTRecord* file         = bcd->getFTRecord(fileIndex);
	CARD16    config       = bcd->file->getCARD16();
	CodeDesc* code         = CodeDesc::getInstance(bcd);
	CARD16    ssegIndex    = bcd->file->getCARD16();
	SGRecord* sseg         = bcd->getSGRecord(ssegIndex);
	CARD16    links        = bcd->file->getCARD16();
	CARD16    u8           = bcd->file->getCARD16();
	//    packed (8:10..10): BOOLEAN,
	bool      packed       = (bool)bitField(u8, 10, 10);
	CARD16    framesize    = bcd->file->getCARD16();
	CARD16    entriesIndex = bcd->file->getCARD16();
	ENRecord* entries      = bcd->getENRecord(entriesIndex);
	CARD16    atoms        = bcd->file->getCARD16();

	return new MTRecord(index, name, file, config, code, sseg, links, packed, framesize, entries, atoms);
}
MTRecord* MTRecord::getNull() {
	QVector<CARD16> initialPC_(0);
	return new MTRecord(MT_NULL, "", 0, 0, 0, 0, 0, 0, 0, 0, 0);
}
QString MTRecord::toString() const {
	if (index == MT_NULL) return "#NULL#";
	return QString("[%1 %2 %3 %4 %5 %6 %7]").arg(index, 4).arg(name).arg(file->toString()).arg(code->toString()).arg(sseg->toString()).arg(framesize).arg(entries->toString());
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
	//initializeNameRecord();
	initializeFTRecord();
	initializeSGRecord();
	initializeENRecord();
	initializeMTRecord();

	sourceFile     = getFTRecord(sourceFileIndex);
	unpackagedFile = getFTRecord(unpackagedFileIndex);

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

BCD::~BCD() {
	delete file;
}

//NameRecord* BCD::getNameRecord(CARD16 index) {
//	if (ss.contains(index)) return ss[index];
//	logger.fatal("Unknown index = %d", index);
//	ERROR();
//}
FTRecord*   BCD::getFTRecord(CARD16 index) {
	if (ft.contains(index)) return ft[index];
	logger.fatal("Unknown index = %d", index);
	ERROR();
}
SGRecord*   BCD::getSGRecord(CARD16 index) {
	if (sg.contains(index)) return sg[index];
	logger.fatal("Unknown index = %d", index);
	ERROR();
}
ENRecord*   BCD::getENRecord(CARD16 index) {
	if (en.contains(index)) return en[index];
	logger.fatal("Unknown index = %d", index);
	ERROR();
}
MTRecord*   BCD::getMTRecord(CARD16 index) {
	if (mt.contains(index)) return mt[index];
	logger.fatal("Unknown index = %d", index);
	ERROR();
}

//void BCD::initializeNameRecord() {
//	int offset = ssOffset;
//	int limit  = ssLimit;
//
//	if (limit == 0) return;
//
//	file->position(offset + 2);
//	file->getCARD8();
//	for(;;) {
//		int bytePos = file->bytePosition();
//		int pos = file->position();
//
//		if ((offset + limit) <= pos) break; // position exceed limit
//
//		int length = file->getCARD8();
//		QString value;
//		for(int i = 0; i < length; i++) {
//	     	char data = file->getCARD8();
//	    	QChar c(data);
//	    	value.append(c);
//		}
//
//		int index = bytePos - (offset * 2) - 3;
//		NameRecord* record = new NameRecord(index, value);
//		ss[index] = record;
//
//		logger.info("ss %4d %s!", index, record->toString().toLocal8Bit().constData());
//	}
//
//	// Add special
//	ss[NameRecord::NullName] = new NameRecord();
//}
QString BCD::getName(CARD16 index) {
	int oldBytePosition = file->bytePosition();

	int offset = ssOffset;
	int limit  = ssLimit;

	int bytePos = index + (offset * 2) + 3;
	file->bytePosition(bytePos);

	int pos = file->position();
	if ((offset + limit) <= pos) {
		logger.fatal("position exceed limit"); // position exceed limit
		logger.fatal("index = %d  offset = %d  limit = %d  pos = %d", index, offset, limit, pos);
		ERROR();
	}

	QString value;
	try {
		int length = file->getCARD8();
		for(int i = 0; i < length; i++) {
	     	char data = file->getCARD8();
	    	QChar c(data);
	    	value.append(c);
		}
	} catch(Abort &e) {
		logger.warn("BCD::getName failed %5d", index);
		value.append(QString("#NAME-%1#").arg(index));
	}

	file->bytePosition(oldBytePosition);
	return value;
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

//		logger.info("ft %3d %s", index, record->toString().toLocal8Bit().constData());
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

//		logger.info("sg %3d %s", index, record->toString().toLocal8Bit().constData());
	}

	// Add special
	sg[SGRecord::SG_NULL] = SGRecord::getNull();
}


void BCD::initializeENRecord() {
	int offset = enOffset;
	int limit  = enLimit;

	file->position(offset);
	for(;;) {
		int pos = file->position();
		if ((offset + limit) <= pos) break; // position exceed limit

		int index = pos - offset;
		ENRecord* record = ENRecord::getInstance(this, index);
		en[index] = record;

//		logger.info("en %3d %s", index, record->toString().toLocal8Bit().constData());
	}

	// Add special
	en[ENRecord::EN_NULL] = ENRecord::getNull();
}

void BCD::initializeMTRecord() {
	int offset = mtOffset;
	int limit  = mtLimit;

	file->position(offset);
	for(;;) {
		int pos = file->position();
		if ((offset + limit) <= pos) break; // position exceed limit

		int index = pos - offset;
		MTRecord* record = MTRecord::getInstance(this, index);
		mt[index] = record;

//		logger.info("mt %3d %s", index, record->toString().toLocal8Bit().constData());
	}

	// Add special
	mt[MTRecord::MT_NULL] = MTRecord::getNull();
}
