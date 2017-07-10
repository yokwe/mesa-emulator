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

#include "HTRecord.h"
#include "MDRecord.h"
#include "CTXRecord.h"
#include "SERecord.h"
#include "BodyRecord.h"
#include "Literals.h"
#include "ExtRecord.h"
#include "Tree.h"


BlockDescriptor::BlockDescriptor(BCD* bcd) {
    offset = bcd->file->getCARD16();
    size   = bcd->file->getCARD16();
}
QString BlockDescriptor::toString() {
	return QString("[%1 %2]").arg(offset, 5).arg(size, 5);
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

	version       = new VersionStamp(bcd);
	creator       = new VersionStamp(bcd);
	sourceVersion = new VersionStamp(bcd);
	logger.info("version        %s", version->toString().toLocal8Bit().constData());
	logger.info("creator        %s", creator->toString().toLocal8Bit().constData());
	logger.info("source         %s", sourceVersion->toString().toLocal8Bit().constData());

    CARD16 word = file->getCARD16();
    definitionsFile = bitField(word, 15);
    logger.info("definitionFile %d", definitionsFile);

    directoryCtx    = new CTXIndex(this, bitField(word, 1, 15));
    logger.info("directoryCtx   %s", directoryCtx->toString().toLocal8Bit().constData());
    importCtx       = new CTXIndex(this, file->getCARD16());
    logger.info("importCtx      %s", importCtx->toString().toLocal8Bit().constData());
    outerCtx        = new CTXIndex(this, file->getCARD16());
    logger.info("outerCtx       %s", outerCtx->toString().toLocal8Bit().constData());


    hvBlock         = new BlockDescriptor(bcd);
    logger.info("hvBlock        %s", hvBlock->toString().toLocal8Bit().constData());
    htBlock         = new BlockDescriptor(bcd);
    logger.info("htBlock        %s", htBlock->toString().toLocal8Bit().constData());
    ssBlock         = new BlockDescriptor(bcd);
    logger.info("ssBlock        %s", ssBlock->toString().toLocal8Bit().constData());
    outerPackBlock  = new BlockDescriptor(bcd);
//    logger.info("outerPackBlock %s", outerPackBlock->toString().toLocal8Bit().constData());
    innerPackBlock  = new BlockDescriptor(bcd);
//    logger.info("innerPackBlock %s", innerPackBlock->toString().toLocal8Bit().constData());
    constBlock      = new BlockDescriptor(bcd);
//    logger.info("constBlock     %s", constBlock->toString().toLocal8Bit().constData());
    seBlock         = new BlockDescriptor(bcd);
    logger.info("seBlock        %s", seBlock->toString().toLocal8Bit().constData());
    ctxBlock        = new BlockDescriptor(bcd);
    logger.info("ctxBlock       %s", ctxBlock->toString().toLocal8Bit().constData());
    mdBlock         = new BlockDescriptor(bcd);
    logger.info("mdBlock        %s", mdBlock->toString().toLocal8Bit().constData());
    bodyBlock       = new BlockDescriptor(bcd);
    logger.info("bodyBlock      %s", bodyBlock->toString().toLocal8Bit().constData());
    extBlock        = new BlockDescriptor(bcd);
//    logger.info("extBlock       %s", extBlock->toString().toLocal8Bit().constData());
    treeBlock       = new BlockDescriptor(bcd);
//    logger.info("treeBlock      %s", treeBlock->toString().toLocal8Bit().constData());
    litBlock        = new BlockDescriptor(bcd);
//    logger.info("litBlock       %s", litBlock->toString().toLocal8Bit().constData());
    sLitBlock       = new BlockDescriptor(bcd);
//    logger.info("sLitBlock      %s", sLitBlock->toString().toLocal8Bit().constData());
    epMapBlock      = new BlockDescriptor(bcd);
//    logger.info("epMapBlock     %s", epMapBlock->toString().toLocal8Bit().constData());
    spareBlock      = new BlockDescriptor(bcd);
//    logger.info("spareBlock     %s", spareBlock->toString().toLocal8Bit().constData());

    fgRelPgBase     = file->getCARD16();
//    logger.info("fgRelPgBase    %d", fgRelPgBase);
    fgPgCount       = file->getCARD16();
//    logger.info("fgPgCount      %d", fgPgCount);

    // Read block
    initializeSS(ssBlock);
    initializeHT(htBlock);
    initializeMD(mdBlock);
    initializeCTX(ctxBlock);
    initializeSE(seBlock);
    initializeBody(bodyBlock);
    initializeLT(litBlock);
    initializeExt(extBlock);
    initializeTree(treeBlock);

    // Resolve index
    HTIndex::resolve();
    MDIndex::resolve();
    CTXIndex::resolve();
    SEIndex::resolve();
    BTIndex::resolve();
    LTIndex::resolve();
    ExtIndex::resolve();
    TreeIndex::resolve();

	{
		MDRecord* p = md[MDIndex::OWM_MDI];
		logger.info("MD %s", p->toString().toLocal8Bit().constData());
	}
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

        HTRecord* record = new HTRecord(this, index, lastSSIndex);
        ht[index] = record;

//        logger.info("ht %s", record->toString().toLocal8Bit().constData());
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

    file->position(base);

    for(;;) {
        int pos = file->position();
        if (limit <= pos) break;

        int index = pos - base;
        MDRecord* record = new MDRecord(this, index);
        md[index] = record;

//        logger.info("md %s", record->toString().toLocal8Bit().constData());
    }

    // sanity check
    {
    	int pos = file->position();
        if (pos != limit) {
        	logger.fatal("pos != limit  pos = %d  limit = %d", pos, limit);
            ERROR();
        }
    }

    // Add special
    md[MDIndex::MD_NULL] = new MDRecord(MDIndex::MD_NULL);
}

void Symbols::initializeCTX(BlockDescriptor* block) {
    CARD16 base  = offsetBase + block->offset;
    int    limit = base + block->size;

    file->position(base);

    for(;;) {
        int pos = file->position();
        if (limit <= pos) break;

        int index = pos - base;
        CTXRecord* record = new CTXRecord(this, index);
        ctx[index] = record;

//        logger.info("ctx %s", record->toString().toLocal8Bit().constData());
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

void Symbols::initializeSE(BlockDescriptor* block) {
    CARD16 base  = offsetBase + block->offset;
    int    limit = base + block->size;

    file->position(base);

    for(;;) {
        int pos = file->position();
        if (limit <= pos) break;

        int index = pos - base;
        SERecord* record = new SERecord(this, index);
        se[index] = record;

//        logger.info("se %s", record->toString().toLocal8Bit().constData());
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

void Symbols::initializeBody(BlockDescriptor* block) {
    CARD16 base  = offsetBase + block->offset;
    int    limit = base + block->size;

    file->position(base);

    for(;;) {
        int pos = file->position();
        if (limit <= pos) break;

        int index = pos - base;
        BodyRecord* record = new BodyRecord(this, index);
        body[index] = record;

//        logger.info("body %s", record->toString().toLocal8Bit().constData());
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

void Symbols::initializeLT(BlockDescriptor* block) {
    CARD16 base  = offsetBase + block->offset;
    int    limit = base + block->size;

    file->position(base);

    for(;;) {
        int pos = file->position();
        if (limit <= pos) break;

        int index = pos - base;
        LTRecord* record = new LTRecord(this, index);
        lt[index] = record;

        logger.info("lt %s", record->toString().toLocal8Bit().constData());
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

void Symbols::initializeExt(BlockDescriptor* block) {
    CARD16 base  = offsetBase + block->offset;
    int    limit = base + block->size;

    file->position(base);

    for(;;) {
        int pos = file->position();
        if (limit <= pos) break;

        int index = pos - base;
        ExtRecord* record = new ExtRecord(this, index);
        ext[index] = record;

        logger.info("ext %s", record->toString().toLocal8Bit().constData());
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

void Symbols::initializeTree(BlockDescriptor* block) {
    CARD16 base  = offsetBase + block->offset;
    int    limit = base + block->size;

    file->position(base);

    for(;;) {
        int pos = file->position();
        if (limit <= pos) break;

        int index = pos - base;
        TreeNode* record = new TreeNode(this, index);
        tree[index] = record;

        logger.info("tree %s", record->toString().toLocal8Bit().constData());
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
