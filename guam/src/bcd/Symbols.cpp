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
// Symbols.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("symbols");

#include "Symbols.h"

BlockDescriptor::BlockDescriptor(BCD& bcd) {
    offset = bcd.file.getCARD16();
    size   = bcd.file.getCARD16();
}
QString BlockDescriptor::toString() {
	return QString("[%1 %2]").arg(offset, 4).arg(size, 4);
}


static QMap<Symbols::TypeClass, QString> typeClassMapInit() {
	QMap<Symbols::TypeClass, QString> ret;

	ret[Symbols::TypeClass::MODE]       = "MODE";
	ret[Symbols::TypeClass::BASIC]      = "BASIC";
	ret[Symbols::TypeClass::ENUMERATED] = "ENUMERATED";
	ret[Symbols::TypeClass::RECORD]     = "RECORD";
	ret[Symbols::TypeClass::REF]        = "REF";
	//
	ret[Symbols::TypeClass::ARRAY]      = "ARRAY";
	ret[Symbols::TypeClass::ARRAY_DESC] = "ARRAY_DESC";
	ret[Symbols::TypeClass::TRANSFER]   = "TRANSFER";
	ret[Symbols::TypeClass::DEFINITION] = "DEFINITION";
	ret[Symbols::TypeClass::UNION]      = "UNION";
	//
	ret[Symbols::TypeClass::SEQUENCE]   = "SEQUENCE";
	ret[Symbols::TypeClass::RELATIVE]   = "RELATIVE";
	ret[Symbols::TypeClass::SUB_RANGE]  = "SUB_RANGE";
	ret[Symbols::TypeClass::LONG]       = "LONG";
	ret[Symbols::TypeClass::REAL]       = "REAL";
	//
	ret[Symbols::TypeClass::OPAQUE]     = "OPAQUE";
	ret[Symbols::TypeClass::ZONE]       = "ZONE";
	ret[Symbols::TypeClass::ANY]        = "ANY";
	ret[Symbols::TypeClass::NIL]        = "NIL";
	ret[Symbols::TypeClass::BITS]       = "BITS";
	//
	ret[Symbols::TypeClass::FIXED_SEQUENCE] = "FIXED_SEQUENCE";

	return ret;
}
static QMap<Symbols::TypeClass, QString> typeClassMap = typeClassMapInit();

QString Symbols::toString(TypeClass value) {
	if (typeClassMap.contains(value)) {
		return typeClassMap[value];
	} else {
		logger.error("Unknown value = %d", (int)value);
		ERROR();
		return QString("%1").arg((int)value);
	}
}

static QMap<Symbols::TransferMode, QString> transferModeMapInit() {
	QMap<Symbols::TransferMode, QString> ret;

	ret[Symbols::TransferMode::PROC]    = "PROC";
	ret[Symbols::TransferMode::PORT]    = "PORT";
	ret[Symbols::TransferMode::SIGNAL]  = "SIGNAL";
	ret[Symbols::TransferMode::ERROR]   = "ERROR";
	ret[Symbols::TransferMode::PROCESS] = "PROCESS";
	ret[Symbols::TransferMode::PROGRAM] = "PROGRAM";
	ret[Symbols::TransferMode::NONE]    = "NONE";

	return ret;
}
static QMap<Symbols::TransferMode, QString> transferModeMap = transferModeMapInit();
QString Symbols::toString(TransferMode value) {
	if (transferModeMap.contains(value)) {
		return transferModeMap[value];
	} else {
		logger.error("Unknown value = %d", (int)value);
		ERROR();
		return QString("%1").arg((int)value);
	}
}

static QMap<Symbols::ExtensionType, QString> extensionTypeMapInit() {
	QMap<Symbols::ExtensionType, QString> ret;

	ret[Symbols::ExtensionType::VALUE]   = "VALUE";
	ret[Symbols::ExtensionType::FORM]    = "FORM";
	ret[Symbols::ExtensionType::DEFAULT] = "DEFAULT";
	ret[Symbols::ExtensionType::NONE]    = "NONE";

	return ret;
}
static QMap<Symbols::ExtensionType, QString> extensionTypeMap = extensionTypeMapInit();
QString Symbols::toString(ExtensionType value) {
	if (extensionTypeMap.contains(value)) {
		return extensionTypeMap[value];
	} else {
		logger.error("Unknown value = %d", (int)value);
		ERROR();
		return QString("%1").arg((int)value);
	}
}

static QMap<Symbols::Linkage, QString> linkageMapInit() {
	QMap<Symbols::Linkage, QString> ret;

	ret[Symbols::Linkage::VAL]      = "VAL";
	ret[Symbols::Linkage::REF]      = "REF";
	ret[Symbols::Linkage::TYPE]     = "TYPE";
	ret[Symbols::Linkage::MANIFEST] = "MANIFEST";
	ret[Symbols::Linkage::NONE]     = "NONE";

	return ret;
}
static QMap<Symbols::Linkage, QString> linkageMap = linkageMapInit();
QString Symbols::toString(Linkage value) {
	if (linkageMap.contains(value)) {
		return linkageMap[value];
	} else {
		logger.error("Unknown value = %d", (int)value);
		ERROR();
		return QString("%1").arg((int)value);
	}
}

static QMap<Symbols::RefClass, QString> refClassMapInit() {
	QMap<Symbols::RefClass, QString> ret;

	ret[Symbols::RefClass::NONE]      = "NONE";
	ret[Symbols::RefClass::SIMPLE]    = "SIMPLE";
	ret[Symbols::RefClass::COMPOSITE] = "COMPOSITE";

	return ret;
}
static QMap<Symbols::RefClass, QString> refClassMap = refClassMapInit();
QString Symbols::toString(RefClass value) {
	if (refClassMap.contains(value)) {
		return refClassMap[value];
	} else {
		logger.error("Unknown value = %d", (int)value);
		ERROR();
		return QString("%1").arg((int)value);
	}
}

static QMap<Symbols::Closure, QString> closureMapInit() {
	QMap<Symbols::Closure, QString> ret;

	ret[Symbols::Closure::NONE] = "NONE";
	ret[Symbols::Closure::UNIT] = "UNIT";
	ret[Symbols::Closure::RC]   = "RC";
	ret[Symbols::Closure::FULL] = "FULL";

	return ret;
}
static QMap<Symbols::Closure, QString> closureMap = closureMapInit();
QString Symbols::toString(Closure value) {
	if (closureMap.contains(value)) {
		return closureMap[value];
	} else {
		logger.error("Unknown value = %d", (int)value);
		ERROR();
		return QString("%1").arg((int)value);
	}
}

static QMap<Symbols::LinkTag, QString> linkTagMapInit() {
	QMap<Symbols::LinkTag, QString> ret;

	ret[Symbols::LinkTag::VARIABLE]  = "VARIABLE";
	ret[Symbols::LinkTag::PROCEDURE] = "PROCEDURE";
	ret[Symbols::LinkTag::TYPE]      = "TYPE";

	return ret;
}
static QMap<Symbols::LinkTag, QString> linkTagMap = linkTagMapInit();
QString Symbols::toString(LinkTag value) {
	if (linkTagMap.contains(value)) {
		return linkTagMap[value];
	} else {
		logger.error("Unknown value = %d", (int)value);
		ERROR();
		return QString("%1").arg((int)value);
	}
}

static QMap<Symbols::VarTag, QString> varTagMapInit() {
	QMap<Symbols::VarTag, QString> ret;

	ret[Symbols::VarTag::VAR]   = "VAR";
	ret[Symbols::VarTag::PROC0] = "PROC0";
	ret[Symbols::VarTag::TYPE]  = "TYPE";
	ret[Symbols::VarTag::PROC1] = "PROC1";

	return ret;
}
static QMap<Symbols::VarTag, QString> varTagMap = varTagMapInit();
QString Symbols::toString(VarTag value) {
	if (varTagMap.contains(value)) {
		return varTagMap[value];
	} else {
		logger.error("Unknown value = %d", (int)value);
		ERROR();
		return QString("%1").arg((int)value);
	}
}

bool Symbols::isSymbolsFile(const BCD& bcd) {
    bcd.file.position(0);
	CARD16 word0 = bcd.file.getCARD16();
    bcd.file.position(WORDS_PER_PAGE);
    CARD16 word256 = bcd.file.getCARD16();

    return word0 == BCD::VersionID && word256 == Symbols::VersionID;
}

Symbols::Symbols(BCD& bcd_, int symbolBase_) : bcd(bcd_) {
    symbolBase      = symbolBase_;
    offsetBase      = (symbolBase - ALTO_BIAS) * WORDS_PER_PAGE;

    bcd.file.position(offsetBase);

	versionIdent = bcd.file.getCARD16();
	if (versionIdent != VersionID) {
		logger.fatal("versionIdent %d", versionIdent);
		logger.fatal("symbolBase   %d", symbolBase);
		ERROR();
	}

	version = VersionStamp(bcd);
	creator = VersionStamp(bcd);
	sourceVersion = VersionStamp(bcd);
	logger.info("version        %s", version.toString().toLocal8Bit().constData());
	logger.info("creator        %s", creator.toString().toLocal8Bit().constData());
	logger.info("source         %s", sourceVersion.toString().toLocal8Bit().constData());

    CARD16 word = bcd.file.getCARD16();
    definitionsFile = bitField(word, 15);
    logger.info("definitionFile %d", definitionsFile);

    directoryCtx    = bitField(word, 1, 15);
    logger.info("directoryCtx   %d", directoryCtx);
    importCtx       = bcd.file.getCARD16();
    logger.info("importCtx      %d", importCtx);
    outerCtx        = bcd.file.getCARD16();
    logger.info("outerCtx       %d", outerCtx);


    hvBlock         = BlockDescriptor(bcd);
    logger.info("hvBlock        %s", hvBlock.toString().toLocal8Bit().constData());
    htBlock         = BlockDescriptor(bcd);
    logger.info("htBlock        %s", htBlock.toString().toLocal8Bit().constData());
    ssBlock         = BlockDescriptor(bcd);
    logger.info("ssBlock        %s", ssBlock.toString().toLocal8Bit().constData());
    outerPackBlock  = BlockDescriptor(bcd);
//    logger.info("outerPackBlock %s", outerPackBlock.toString().toLocal8Bit().constData());
    innerPackBlock  = BlockDescriptor(bcd);
//    logger.info("innerPackBlock %s", innerPackBlock.toString().toLocal8Bit().constData());
    constBlock      = BlockDescriptor(bcd);
//    logger.info("constBlock     %s", constBlock.toString().toLocal8Bit().constData());
    seBlock         = BlockDescriptor(bcd);
//    logger.info("seBlock        %s", seBlock.toString().toLocal8Bit().constData());
    ctxBlock        = BlockDescriptor(bcd);
//    logger.info("ctxBlock       %s", ctxBlock.toString().toLocal8Bit().constData());
    mdBlock         = BlockDescriptor(bcd);
//    logger.info("mdBlock        %s", mdBlock.toString().toLocal8Bit().constData());
    bodyBlock       = BlockDescriptor(bcd);
//    logger.info("bodyBlock      %s", bodyBlock.toString().toLocal8Bit().constData());
    extBlock        = BlockDescriptor(bcd);
//    logger.info("extBlock       %s", extBlock.toString().toLocal8Bit().constData());
    treeBlock       = BlockDescriptor(bcd);
//    logger.info("treeBlock      %s", treeBlock.toString().toLocal8Bit().constData());
    litBlock        = BlockDescriptor(bcd);
//    logger.info("litBlock       %s", litBlock.toString().toLocal8Bit().constData());
    sLitBlock       = BlockDescriptor(bcd);
//    logger.info("sLitBlock      %s", sLitBlock.toString().toLocal8Bit().constData());
    epMapBlock      = BlockDescriptor(bcd);
//    logger.info("epMapBlock     %s", epMapBlock.toString().toLocal8Bit().constData());
    spareBlock      = BlockDescriptor(bcd);
//    logger.info("spareBlock     %s", spareBlock.toString().toLocal8Bit().constData());

    fgRelPgBase     = bcd.file.getCARD16();
//    logger.info("fgRelPgBase    %d", fgRelPgBase);
    fgPgCount       = bcd.file.getCARD16();
//    logger.info("fgPgCount      %d", fgPgCount);

    // Read block
    initializeSS();
    initializeHT();

    // Resolve index
    HTIndex::resolve();
};

void Symbols::initializeSS() {
    CARD16 base  = offsetBase + ssBlock.offset;
    CARD16 limit = base + ssBlock.size;

    bcd.file.position(base);
    CARD16 length    = bcd.file.getCARD16();
    CARD16 maxLength = bcd.file.getCARD16();

    for(int i = 0; i < length; i++) {
     	char data = bcd.file.getCARD8();
    	QChar c(data);
    	ss.append(c);
    }

    // Advance position
    for(int i = length; i <= maxLength; i++) {
    	bcd.file.getCARD8();
    }

    // sanity check
    {
    	int pos = bcd.file.position();
        if (pos != limit) {
        	logger.fatal("pos != limit  pos = %d  limit = %d", pos, limit);
            ERROR();
        }
    }
    logger.info("ss = (%d)%s ... %s!", ss.length(), ss.left(10).toLatin1().constData(), ss.right(10).toLatin1().constData());
}

void Symbols::initializeHT() {
    CARD16 base  = offsetBase + htBlock.offset;
    int    limit = base + htBlock.size;

    CARD16 lastSSIndex = 0;
    CARD16 index = 0;
    bcd.file.position(base);

    for(;;) {
        int pos = bcd.file.position();
        if (limit <= pos) break;

        HTRecord data(*this, index, lastSSIndex);
        ht[index] = data;

        logger.info("ht %s", data.toString().toLocal8Bit().constData());
        index++;
        lastSSIndex = data.ssIndex;
    }

    // sanity check
    {
    	int pos = bcd.file.position();
        if (pos != limit) {
        	logger.fatal("pos != limit  pos = %d  limit = %d", pos, limit);
            ERROR();
        }
    }

    // Add special
    ht[HTIndex::HT_NULL] = HTRecord();
}
