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


#define TO_STRING_PROLOGUE(e) \
	typedef e ENUM; \
	static QMap<ENUM, QString> map({
#define MAP_ENTRY(m) {ENUM::m, #m},
#define TO_STRING_EPILOGUE \
	}); \
	if (map.contains(value)) { \
		return map[value]; \
	} else { \
		logger.error("Unknown value = %d", (int)value); \
		ERROR(); \
		return QString("%1").arg((int)value); \
	}


BlockDescriptor::BlockDescriptor(BCD& bcd) {
    offset = bcd.file.getCARD16();
    size   = bcd.file.getCARD16();
}
QString BlockDescriptor::toString() {
	return QString("[%1 %2]").arg(offset, 4).arg(size, 4);
}


QString Symbols::toString(TypeClass value) {
	TO_STRING_PROLOGUE(TypeClass)

	MAP_ENTRY(MODE)
	MAP_ENTRY(BASIC)
	MAP_ENTRY(ENUMERATED)
	MAP_ENTRY(RECORD)
	MAP_ENTRY(REF)
	//
	MAP_ENTRY(ARRAY)
	MAP_ENTRY(ARRAY_DESC)
	MAP_ENTRY(TRANSFER)
	MAP_ENTRY(DEFINITION)
	MAP_ENTRY(UNION)
	//
	MAP_ENTRY(SEQUENCE)
	MAP_ENTRY(RELATIVE)
	MAP_ENTRY(SUB_RANGE)
	MAP_ENTRY(LONG)
	MAP_ENTRY(REAL)
	//
	MAP_ENTRY(OPAQUE)
	MAP_ENTRY(ZONE)
	MAP_ENTRY(ANY)
	MAP_ENTRY(NIL)
	MAP_ENTRY(BITS)
	//
	MAP_ENTRY(FIXED_SEQUENCE)

	TO_STRING_EPILOGUE
}


QString Symbols::toString(TransferMode value) {
	TO_STRING_PROLOGUE(TransferMode)

	MAP_ENTRY(PROC)
	MAP_ENTRY(PORT)
	MAP_ENTRY(SIGNAL)
	MAP_ENTRY(ERROR)
	MAP_ENTRY(PROCESS)
	MAP_ENTRY(PROGRAM)
	MAP_ENTRY(NONE)

	TO_STRING_EPILOGUE
}


QString Symbols::toString(ExtensionType value) {
	TO_STRING_PROLOGUE(ExtensionType)

	MAP_ENTRY(VALUE)
	MAP_ENTRY(FORM)
	MAP_ENTRY(DEFAULT)
	MAP_ENTRY(NONE)

	TO_STRING_EPILOGUE
}


QString Symbols::toString(Linkage value) {
	TO_STRING_PROLOGUE(Linkage)

	MAP_ENTRY(VAL)
	MAP_ENTRY(REF)
	MAP_ENTRY(TYPE)
	MAP_ENTRY(MANIFEST)
	MAP_ENTRY(NONE)

	TO_STRING_EPILOGUE
}


QString Symbols::toString(RefClass value) {
	TO_STRING_PROLOGUE(RefClass)

	MAP_ENTRY(NONE)
	MAP_ENTRY(SIMPLE)
	MAP_ENTRY(COMPOSITE)

	TO_STRING_EPILOGUE
}


QString Symbols::toString(Closure value) {
	TO_STRING_PROLOGUE(Closure)

	MAP_ENTRY(NONE)
	MAP_ENTRY(UNIT)
	MAP_ENTRY(RC)
	MAP_ENTRY(FULL)

	TO_STRING_EPILOGUE
}


QString Symbols::toString(LinkTag value) {
	TO_STRING_PROLOGUE(LinkTag)

	MAP_ENTRY(VARIABLE)
	MAP_ENTRY(PROCEDURE)
	MAP_ENTRY(TYPE)

	TO_STRING_EPILOGUE
}


QString Symbols::toString(VarTag value) {
	TO_STRING_PROLOGUE(VarTag)

	MAP_ENTRY(VAR)
	MAP_ENTRY(PROC0)
	MAP_ENTRY(TYPE)
	MAP_ENTRY(PROC1)

	TO_STRING_EPILOGUE
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
    initializeMD();

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

void Symbols::initializeMD() {
    CARD16 base  = offsetBase + mdBlock.offset;
    int    limit = base + mdBlock.size;

    CARD16 index = 0;
    bcd.file.position(base);

    for(;;) {
        int pos = bcd.file.position();
        if (limit <= pos) break;

        MDRecord data(*this, index);
        md[index] = data;

        logger.info("md %s", data.toString().toLocal8Bit().constData());
        index++;
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
    md[HTIndex::HT_NULL] = MDRecord();
}
