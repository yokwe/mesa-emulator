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

#include "../mesa/Memory.h"

#include "Symbols.h"

#include "BCDFile.h"


//  altoBias: CARDINAL = 1;  -- AMesa/14.0/Compiler/Friends/FilePack.mesa
static const CARD16 ALTO_BIAS = 1;

static const CARD16 WORDS_PER_PAGE = 256;


//
// HTIndex
//
QList<HTIndex*> HTIndex::all;
HTIndex* HTIndex::getNull() {
	static HTIndex ret(0, HTIndex::HT_NULL);
	return &ret;
}
HTIndex* HTIndex::getInstance(Symbols* symbols_, CARD16 index_) {
	return new HTIndex(symbols_, index_);
}
void HTIndex::resolve() {
	for(HTIndex* p: all) {
		if (p->isNull()) continue;
		if (p->value)    continue;

		QMap<CARD16, HTRecord*>& map(p->symbols->ht);
		CARD16                   index   = p->index;

		if (map.contains(index)) {
			p->value = map[index];
		} else {
			logger.fatal("Unknown index = %d", index);
			ERROR();
		}
	}
	logger.info("HTIndex::resove %d", all.size());
}
QString HTIndex::toString() const {
	if (isNull()) return "#NULL";
	if (value == 0) return QString("ht-%1").arg(index);
	return QString("ht-%1-[%2]").arg(index).arg(value->value);
}


//
// CTXIndex
//
QList<CTXIndex*> CTXIndex::all;
CTXIndex* CTXIndex::getNull() {
	static CTXIndex ret(0, CTXIndex::CTX_NULL);
	return &ret;
}
CTXIndex* CTXIndex::getInstance(Symbols* symbols_, CARD16 index_) {
	return new CTXIndex(symbols_, index_);
}
void CTXIndex::resolve() {
	for(CTXIndex* p: all) {
		if (p->isNull()) continue;
		if (p->value)    continue;

		QMap<CARD16, CTXRecord*>& map(p->symbols->ctx);
		CARD16 index = p->index;

		if (map.contains(index)) {
			p->value = map[index];
		} else {
			logger.fatal("Unknown index = %d", index);
			ERROR();
		}
	}
	logger.info("CTXIndex::resove %d", all.size());
}
QString CTXIndex::toString() const {
	if (isNull()) return "#NULL";
	if (value == 0) return QString("ctx-%1").arg(index);
	return QString("ctx-%1-[%2]").arg(index); // TODO
//	return QString("ctx-%1-[%2]").arg(index).arg(value->toString(value));
}


//
// MDIndex
//
QList<MDIndex*> MDIndex::all;
MDIndex* MDIndex::getNull() {
	static MDIndex ret(0, MDIndex::MD_NULL);
	return &ret;
}
MDIndex* MDIndex::getInstance(Symbols* symbols_, CARD16 index_) {
	return new MDIndex(symbols_, index_);
}
void MDIndex::resolve() {
	for(MDIndex* p: all) {
		if (p->isNull()) continue;
		if (p->value)    continue;

		QMap<CARD16, MDRecord*>& map(p->symbols->md);
		CARD16 index = p->index;

		if (map.contains(index)) {
			p->value = map[index];
		} else {
			logger.fatal("Unknown index = %d", index);
			ERROR();
		}
	}
	logger.info("MDIndex::resove %d", all.size());
}
QString MDIndex::toString() const {
	if (isNull()) return "#NULL";
	if (value == 0) return QString("md-%1").arg(index);
	return QString("md-%1-[%2]").arg(index); // TODO
//	return QString("ctx-%1-[%2]").arg(index).arg(value->toString(value));
}


//
// MDRecord
//
MDRecord* MDRecord::getInstance(Symbols* symbols, CARD16 index) {
	Stamp*   stamp    = Stamp::getInstance(symbols->bcd);
	HTIndex* moduleId = HTIndex::getInstance(symbols, symbols->file->getCARD16());

	CARD16   word     = symbols->file->getCARD16();
	HTIndex* fileId   = HTIndex::getInstance(symbols, bitField(word, 0, 12));
    bool     shared   = bitField(word, 13);
    bool     exported = bitField(word, 14, 15);

    CTXIndex* ctx           = CTXIndex::getInstance(symbols, symbols->file->getCARD16());
    CTXIndex* defaultImport = CTXIndex::getInstance(symbols, symbols->file->getCARD16());

    CARD16 file = symbols->file->getCARD16();

    return new MDRecord(symbols, index, stamp, moduleId, fileId, shared, exported, ctx, defaultImport, file);
}
QString MDRecord::toString() const {
//	return QString("%1 %2 %3 %4 %5 %6 %7 %8").
//			arg(index, 4).arg(stamp->toString()).arg(moduleId->getValue()).arg(fileId->getValue()).
//			arg(shared ? "S" : " ").arg(exported ? "E" : " ").arg(ctx->toString()).arg(defaultImport->toString());
	return QString("%1 %2 %3 %4 %5 %6 %7 %8").
			arg(index, 4).arg(stamp->toString()).arg(moduleId->toString()).arg(fileId->toString()).
			arg(shared ? "S" : " ").arg(exported ? "E" : " ").arg(ctx->toString()).arg(defaultImport->toString());
}


//
// HTRecord
//
HTRecord* HTRecord::getInstance(Symbols* symbols, CARD16 index, CARD16 lastSSIndex) {
    // 0
	CARD16 word = symbols->file->getCARD16();
    bool   anyInternal = bitField(word, 0);
    bool   anyPublic   = bitField(word, 1);
    CARD16 link        = bitField(word, 2, 15);
    // 1
    CARD16 ssIndex     = symbols->file->getCARD16();
    // ss.substring(lastSSIndex, data.ssIndex);
    QString value       = symbols->ss.mid(lastSSIndex, ssIndex - lastSSIndex);

    return new HTRecord(symbols, index, anyInternal, anyPublic, link, ssIndex, value);
}
QString HTRecord::toString() const {
	return QString("%1 %2%3 %4").arg(index, 4).arg(anyInternal ? "I" : " ").arg(anyPublic ? "P" : " ").arg(value);
}


//
// Symbols
//

Symbols::BlockDescriptor* Symbols::BlockDescriptor::getInstance(BCD* bcd) {
	CARD16 offset = bcd->file->getCARD16();
	CARD16 size   = bcd->file->getCARD16();

	return new BlockDescriptor(offset, size);
}

QString Symbols::BlockDescriptor::toString() const {
	return QString("[%1 %2]").arg(offset, 5).arg(size, 5);
}

bool Symbols::isSymbolsFile(BCD* bcd) {
    bcd->file->position(0);
	CARD16 word0 = bcd->file->getCARD16();
    bcd->file->position(WORDS_PER_PAGE);
    CARD16 word256 = bcd->file->getCARD16();

    return word0 == BCD::VersionID && word256 == Symbols::VersionID;
}

Symbols::Symbols(BCD* bcd_, int symbolBase_) : bcd(bcd_) {
	bcd             = bcd_;
	file            = bcd->file;

    symbolBase      = symbolBase_;
    offsetBase      = (symbolBase - ALTO_BIAS) * WORDS_PER_PAGE;

    file->position(offsetBase);

	versionIdent = file->getCARD16();
	if (versionIdent != VersionID) {
		logger.fatal("versionIdent %d", versionIdent);
		logger.fatal("symbolBase   %d", symbolBase);
		ERROR();
	}

	version       = Stamp::getInstance(bcd) ;
	creator       = Stamp::getInstance(bcd) ;
	sourceVersion = Stamp::getInstance(bcd) ;
	logger.info("version        %s", version->toString().toLocal8Bit().constData());
	logger.info("creator        %s", creator->toString().toLocal8Bit().constData());
	logger.info("source         %s", sourceVersion->toString().toLocal8Bit().constData());

    CARD16 word = file->getCARD16();
    definitionsFile = bitField(word, 15);
    logger.info("definitionFile %d", definitionsFile);

    directoryCtx    = CTXIndex::getInstance(this, bitField(word, 1, 15));
    logger.info("directoryCtx   %s", directoryCtx->toString().toLocal8Bit().constData());
    importCtx       = CTXIndex::getInstance(this, file->getCARD16());
    logger.info("importCtx      %s", importCtx->toString().toLocal8Bit().constData());
    outerCtx        = CTXIndex::getInstance(this, file->getCARD16());
    logger.info("outerCtx       %s", outerCtx->toString().toLocal8Bit().constData());


    hvBlock         = BlockDescriptor::getInstance(bcd);
    logger.info("hvBlock        %s", hvBlock->toString().toLocal8Bit().constData());
    htBlock         = BlockDescriptor::getInstance(bcd);
    logger.info("htBlock        %s", htBlock->toString().toLocal8Bit().constData());
    ssBlock         = BlockDescriptor::getInstance(bcd);
    logger.info("ssBlock        %s", ssBlock->toString().toLocal8Bit().constData());
    outerPackBlock  = BlockDescriptor::getInstance(bcd);
//    logger.info("outerPackBlock %s", outerPackBlock->toString().toLocal8Bit().constData());
    innerPackBlock  = BlockDescriptor::getInstance(bcd);
//    logger.info("innerPackBlock %s", innerPackBlock->toString().toLocal8Bit().constData());
    constBlock      = BlockDescriptor::getInstance(bcd);
//    logger.info("constBlock     %s", constBlock->toString().toLocal8Bit().constData());
    seBlock         = BlockDescriptor::getInstance(bcd);
    logger.info("seBlock        %s", seBlock->toString().toLocal8Bit().constData());
    ctxBlock        = BlockDescriptor::getInstance(bcd);
    logger.info("ctxBlock       %s", ctxBlock->toString().toLocal8Bit().constData());
    mdBlock         = BlockDescriptor::getInstance(bcd);
    logger.info("mdBlock        %s", mdBlock->toString().toLocal8Bit().constData());
    bodyBlock       = BlockDescriptor::getInstance(bcd);
    logger.info("bodyBlock      %s", bodyBlock->toString().toLocal8Bit().constData());
    extBlock        = BlockDescriptor::getInstance(bcd);
//    logger.info("extBlock       %s", extBlock->toString().toLocal8Bit().constData());
    treeBlock       = BlockDescriptor::getInstance(bcd);
//    logger.info("treeBlock      %s", treeBlock->toString().toLocal8Bit().constData());
    litBlock        = BlockDescriptor::getInstance(bcd);
//    logger.info("litBlock       %s", litBlock->toString().toLocal8Bit().constData());
    sLitBlock       = BlockDescriptor::getInstance(bcd);
//    logger.info("sLitBlock      %s", sLitBlock->toString().toLocal8Bit().constData());
    epMapBlock      = BlockDescriptor::getInstance(bcd);
//    logger.info("epMapBlock     %s", epMapBlock->toString().toLocal8Bit().constData());
    spareBlock      = BlockDescriptor::getInstance(bcd);
//    logger.info("spareBlock     %s", spareBlock->toString().toLocal8Bit().constData());

    fgRelPgBase     = file->getCARD16();
//    logger.info("fgRelPgBase    %d", fgRelPgBase);
    fgPgCount       = file->getCARD16();
//    logger.info("fgPgCount      %d", fgPgCount);

    // Read block
    initializeSS(ssBlock);
    initializeHT(htBlock);
    initializeMD(mdBlock);
//    initializeCTX(ctxBlock);
//    initializeSE(seBlock);
//    initializeBody(bodyBlock);
//    initializeLT(litBlock);
//    initializeExt(extBlock);
//    initializeTree(treeBlock);

    // Resolve index
    HTIndex::resolve();
    MDIndex::resolve();
//    CTXIndex::resolve();
//    SEIndex::resolve();
//    BTIndex::resolve();
//    LTIndex::resolve();
//    ExtIndex::resolve();
//    TreeIndex::resolve();

//	{
//		MDRecord* p = md[MDIndex::OWM_MDI];
//		logger.info("MD %s", p->toString().toLocal8Bit().constData());
//	}
};

void Symbols::initializeSS(BlockDescriptor* block) {
    CARD16 base  = offsetBase + block->offset;
    CARD16 limit = base + block->size;

    file->position(base);
    CARD16 length    = file->getCARD16();
    CARD16 maxLength = file->getCARD16();

    for(int i = 0; i < length; i++) {
     	char data = file->getCARD8();
    	QChar c(data);
    	ss.append(c);
    }

    // Advance position
    for(int i = length; i <= maxLength; i++) {
    	file->getCARD8();
    }

    // sanity check
    {
    	int pos = file->position();
        if (pos != limit) {
        	logger.fatal("pos != limit  pos = %d  limit = %d", pos, limit);
            ERROR();
        }
    }
//    logger.info("ss = (%d)%s ... %s!", ss.length(), ss.left(10).toLatin1().constData(), ss.right(10).toLatin1().constData());
}

void Symbols::initializeHT(BlockDescriptor* block) {
    CARD16 base  = offsetBase + block->offset;
    int    limit = base + block->size;

    CARD16 lastSSIndex = 0;
    CARD16 index = 0;
    file->position(base);

    for(;;) {
        int pos = file->position();
        if (limit <= pos) break;

        HTRecord* record = HTRecord::getInstance(this, index, lastSSIndex);
        ht[index] = record;

        logger.info("ht %s", record->toString().toLocal8Bit().constData());
        index++;
        lastSSIndex = record->ssIndex;
    }

    // sanity check
    {
    	int pos = file->position();
        if (pos != limit) {
        	logger.fatal("pos != limit  pos = %d  limit = %d", pos, limit);
            ERROR();
        }
    }
}

void Symbols::initializeMD(BlockDescriptor* block) {
    CARD16 base  = offsetBase + block->offset;
    int    limit = base + block->size;

    CARD16 index = 0;
    file->position(base);

    for(;;) {
        int pos = file->position();
        if (limit <= pos) break;

        MDRecord* record = MDRecord::getInstance(this, index);
        md[index] = record;

        logger.info("md %s", record->toString().toLocal8Bit().constData());
        index++;
    }

    // sanity check
    {
    	int pos = file->position();
        if (pos != limit) {
        	logger.fatal("pos != limit  pos = %d  limit = %d", pos, limit);
            ERROR();
        }
    }
}

