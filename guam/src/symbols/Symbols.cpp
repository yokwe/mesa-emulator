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

#include "BTIndex.h"
#include "CTXIndex.h"
#include "ExtIndex.h"
#include "HTIndex.h"
#include "LTIndex.h"
#include "MDIndex.h"
#include "SEIndex.h"
#include "Tree.h"


QString Symbols::toString(TypeClass value) {
	TO_STRING_PROLOGUE(TypeClass)

	MAP_ENTRY(MODE)
	MAP_ENTRY(BASIC)
	MAP_ENTRY(ENUMERATED)
	MAP_ENTRY(RECORD)
	MAP_ENTRY(REF)
	//
	MAP_ENTRY(ARRAY)
	MAP_ENTRY(ARRAYDESC)
	MAP_ENTRY(TRANSFER)
	MAP_ENTRY(DEFINITION)
	MAP_ENTRY(UNION)
	//
	MAP_ENTRY(SEQUENCE)
	MAP_ENTRY(RELATIVE)
	MAP_ENTRY(SUBRANGE)
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

bool Symbols::isSymbolsSegment(BCD* bcd, int symbolBase) {
    bcd->file->position((symbolBase - ALTO_BIAS) * WORDS_PER_PAGE);
	CARD16 versionIdent = bcd->file->getCARD16();

    return versionIdent == VersionID;
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
    initializeCTX(ctxBlock);
    initializeSE(seBlock);
    initializeBT(bodyBlock);
    initializeLT(litBlock);
    initializeExt(extBlock);
    initializeTree(treeBlock);

    BTIndex::checkAll();
    CTXIndex::checkAll();
    ExtIndex::checkAll();
    HTIndex::checkAll();
    LTIndex::checkAll();
    MDIndex::checkAll();
    SEIndex::checkAll();
    TreeIndex::checkAll();
	{
		MDRecord* p = md[0];
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

        HTRecord* record = HTRecord::getInstance(this, index, lastSSIndex);
        ht[index] = record;

        logger.info("ht %4d %s", index, record->toString().toLocal8Bit().constData());
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
        MDRecord* record = MDRecord::getInstance(this, index);
        md[index] = record;

        logger.info("md %4d %s", index, record->toString().toLocal8Bit().constData());
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

void Symbols::initializeCTX(BlockDescriptor* block) {
    CARD16 base  = offsetBase + block->offset;
    int    limit = base + block->size;

    file->position(base);

    for(;;) {
        int pos = file->position();
        if (limit <= pos) break;

        int index = pos - base;
        CTXRecord* record = CTXRecord::getInstance(this, index);
        ctx[index] = record;

        logger.info("ctx %4d %s", index, record->toString().toLocal8Bit().constData());
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

void Symbols::initializeSE(BlockDescriptor* block) {
    CARD16 base  = offsetBase + block->offset;
    int    limit = base + block->size;

    file->position(base);

    for(;;) {
        int pos = file->position();
        if (limit <= pos) break;

        int index = pos - base;
        SERecord* record = SERecord::getInstance(this, index);
        se[index] = record;

        logger.info("se %4d %s", index, record->toString().toLocal8Bit().constData());
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

void Symbols::initializeBT(BlockDescriptor* block) {
    CARD16 base  = offsetBase + block->offset;
    int    limit = base + block->size;

    file->position(base);

    for(;;) {
        int pos = file->position();
        if (limit <= pos) break;

        int index = pos - base;
        BTRecord* record = BTRecord::getInstance(this, index);
        bt[index] = record;

        logger.info("bt %4d %s", index, record->toString().toLocal8Bit().constData());
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

void Symbols::initializeExt(BlockDescriptor* block) {
    CARD16 base  = offsetBase + block->offset;
    int    limit = base + block->size;

    file->position(base);

    for(;;) {
        int pos = file->position();
        if (limit <= pos) break;

        int index = pos - base;
        ExtRecord* record = ExtRecord::getInstance(this, index);
        ext[index] = record;

        logger.info("ext %4d %s", index, record->toString().toLocal8Bit().constData());
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

void Symbols::initializeLT(BlockDescriptor* block) {
    CARD16 base  = offsetBase + block->offset;
    int    limit = base + block->size;

    file->position(base);

    for(;;) {
        int pos = file->position();
        if (limit <= pos) break;

        int index = pos - base;
        LTRecord* record = LTRecord::getInstance(this, index);
        lt[index] = record;

        logger.info("lt %4d %s", index, record->toString().toLocal8Bit().constData());
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

void Symbols::initializeTree(BlockDescriptor* block) {
    CARD16 base  = offsetBase + block->offset;
    int    limit = base + block->size;

    file->position(base);

    for(;;) {
        int pos = file->position();
        if (limit <= pos) break;

        int index = pos - base;
        TreeNode* record = TreeNode::getInstance(this, index);
        tree[index] = record;

        logger.info("tree %4d %s", index, record->toString().toLocal8Bit().constData());
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
