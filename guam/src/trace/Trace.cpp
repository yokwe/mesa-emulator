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
// trace.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("trace");

#include "../mesa/Pilot.h"
#include "../mesa/Memory.h"

#include "Trace.h"

#define READ_OBJECT_PROLOGUE() \
{ \
	if (ptr == 0) { \
		logger.fatal("ptr is zero"); \
		ERROR(); \
		return; \
	} \
	if (target == 0) { \
		logger.fatal("target is zero"); \
		ERROR(); \
		return; \
	} \
	::memset(target, 0, sizeof(*target)); \
}

#define READ_TARGET(t, f) readObject(ptr + OFFSET(t, f), &(target->f));



void readObject(CARD32 ptr, CARD16* target) {
	READ_OBJECT_PROLOGUE();

	CARD16* p = Memory::getAddress(ptr);
	CARD16  t = *p;
	*target = t;
}
void readObject(CARD32 ptr, CARD32* target) {
	READ_OBJECT_PROLOGUE();

	CARD16* p0 = Memory::getAddress(ptr + 0);
	CARD16* p1 = Memory::getAddress(ptr + 1);

	CARD32 t0 = *p0;
	CARD32 t1 = *p1;
	CARD32 t = (t1 << WordSize) | t0;

	*target = t;
}

void readObject(CARD32 ptr, LoadStateFormat::ModuleInfo* target) {
	READ_OBJECT_PROLOGUE();

	READ_TARGET(LoadStateFormat::ModuleInfo, u0);
	READ_TARGET(LoadStateFormat::ModuleInfo, index);
	READ_TARGET(LoadStateFormat::ModuleInfo, globalFrame);
}
void readObject(CARD32 ptr, CPSwapDefs::ExternalStateVector* target) {
	READ_OBJECT_PROLOGUE();

	READ_TARGET(CPSwapDefs::ExternalStateVector, version);
	if (target->version != CPSwapDefs::currentVersion) {
		logger.warn("Unknown version  %d", target->version);
		return;
	}

	READ_TARGET(CPSwapDefs::ExternalStateVector, bootSession);
	READ_TARGET(CPSwapDefs::ExternalStateVector, loadState);
	READ_TARGET(CPSwapDefs::ExternalStateVector, mds);
	READ_TARGET(CPSwapDefs::ExternalStateVector, virtualMemoryCount);
	READ_TARGET(CPSwapDefs::ExternalStateVector, u27);
}
void readObject(CPSwapDefs::ExternalStateVector* target) {
	CARD32 ptr = 0;
	readObject(PDA + OFFSET(ProcessDataArea, available), &ptr);
	readObject(ptr, target);
}
void readObject(CARD32 ptr, LoadStateFormat::BcdInfo* target) {
	READ_OBJECT_PROLOGUE();

	READ_TARGET(LoadStateFormat::BcdInfo, u0);
	READ_TARGET(LoadStateFormat::BcdInfo, base);
	READ_TARGET(LoadStateFormat::BcdInfo, id);
}

void readObject(CARD32 ptr, LoadStateFormat::Object* target) {
	READ_OBJECT_PROLOGUE();

	READ_TARGET(LoadStateFormat::Object, versionident);
	if (target->versionident != LoadStateFormat::VersionID) {
		logger.error("version mismatch.  %d", target->versionident);
		ERROR();
		return;
	}

	READ_TARGET(LoadStateFormat::Object, nModules);
	READ_TARGET(LoadStateFormat::Object, maxModules);
	READ_TARGET(LoadStateFormat::Object, nBcds);
	READ_TARGET(LoadStateFormat::Object, maxBcds);
	READ_TARGET(LoadStateFormat::Object, nextID);
	READ_TARGET(LoadStateFormat::Object, moduleInfo);
	READ_TARGET(LoadStateFormat::Object, bcdInfo);
}

void readObject(CARD32 ptr, TimeStamp::Stamp* target) {
	READ_OBJECT_PROLOGUE();

	READ_TARGET(TimeStamp::Stamp, u0);
	READ_TARGET(TimeStamp::Stamp, time);
}
void readObject(CARD32 ptr, BcdDefs::BCD* target) {
	READ_OBJECT_PROLOGUE();

	if (Memory::isVacant(ptr)) {
		logger.warn("ptr is not mapped  %8X", ptr);
		return;
	}

	READ_TARGET(BcdDefs::BCD, versionIdent);
	if (target->versionIdent != BcdDefs::VersionID) {
		logger.warn("version mismatch.  %d", target->versionIdent);
		return;
	}

	READ_TARGET(BcdDefs::BCD, version);
	READ_TARGET(BcdDefs::BCD, creator);
	READ_TARGET(BcdDefs::BCD, sourceFile);
	READ_TARGET(BcdDefs::BCD, unpackagedFile);
	READ_TARGET(BcdDefs::BCD, nConfigs);
	READ_TARGET(BcdDefs::BCD, nModules);
	READ_TARGET(BcdDefs::BCD, nImports);
	READ_TARGET(BcdDefs::BCD, nExports);
	READ_TARGET(BcdDefs::BCD, flags);

	READ_TARGET(BcdDefs::BCD, firstdummy);
	READ_TARGET(BcdDefs::BCD, nDummies);
	READ_TARGET(BcdDefs::BCD, ssOffset);
	READ_TARGET(BcdDefs::BCD, ssLimit);
	READ_TARGET(BcdDefs::BCD, ctOffset);
	READ_TARGET(BcdDefs::BCD, ctLimit);
	READ_TARGET(BcdDefs::BCD, mtOffset);
	READ_TARGET(BcdDefs::BCD, mtLimit);
	READ_TARGET(BcdDefs::BCD, impOffset);
	READ_TARGET(BcdDefs::BCD, impLimit);
	READ_TARGET(BcdDefs::BCD, expOffset);
	READ_TARGET(BcdDefs::BCD, expLimit);
	READ_TARGET(BcdDefs::BCD, enOffset);
	READ_TARGET(BcdDefs::BCD, enLimit);
	READ_TARGET(BcdDefs::BCD, sgOffset);
	READ_TARGET(BcdDefs::BCD, sgLimit);
	READ_TARGET(BcdDefs::BCD, ftOffset);
	READ_TARGET(BcdDefs::BCD, ftLimit);
	READ_TARGET(BcdDefs::BCD, spOffset);
	READ_TARGET(BcdDefs::BCD, spLimit);
	READ_TARGET(BcdDefs::BCD, ntOffset);
	READ_TARGET(BcdDefs::BCD, ntLimit);
	READ_TARGET(BcdDefs::BCD, typOffset);
	READ_TARGET(BcdDefs::BCD, typLimit);
	READ_TARGET(BcdDefs::BCD, tmOffset);
	READ_TARGET(BcdDefs::BCD, tmLimit);
	READ_TARGET(BcdDefs::BCD, fpOffset);
	READ_TARGET(BcdDefs::BCD, fpLimit);
	READ_TARGET(BcdDefs::BCD, lfOffset);
	READ_TARGET(BcdDefs::BCD, lfLimit);
	READ_TARGET(BcdDefs::BCD, atOffset);
	READ_TARGET(BcdDefs::BCD, atLimit);
	READ_TARGET(BcdDefs::BCD, apOffset);
	READ_TARGET(BcdDefs::BCD, apLimit);
}


void readObject(CARD32 ptr, PrincOpsExtras2::GFTItem* target) {
	READ_OBJECT_PROLOGUE();

	READ_TARGET(PrincOpsExtras2::GFTItem, globalFrame);
	READ_TARGET(PrincOpsExtras2::GFTItem, codebase);
}

//void dumpESV() {
//	{
//		for(int i = PrincOpsExtras2::GermUseOnly_FIRST; i <= PrincOpsExtras2::GermUseOnly_LAST; i++) {
//			CARD32 o = i * SIZE(GFTItem);
//			CARD32 p = mGFT + o;
//			CARD32 globalframe = ReadDbl(p + 0);
//			CARD32 codebase    = ReadDbl(p + 2);
//
//			if (codebase) logger.info("GFT GERM  %4d  %4X  gf %8X   cb %8X", i, o, globalframe, codebase);
//		}
//		for(int i = PrincOpsExtras2::Reserved_FIRST; i <= PrincOpsExtras2::Reserved_LAST; i++) {
//			CARD32 o = i * SIZE(GFTItem);
//			CARD32 p = mGFT + o;
//			CARD32 globalframe = ReadDbl(p + 0);
//			CARD32 codebase    = ReadDbl(p + 2);
//
//			if (codebase) logger.info("GFT PILOT %4d  %4X  gf %8X   cb %8X", i, o, globalframe, codebase);
//		}
//		for(int i = PrincOpsExtras2::Reserved_LAST + 1; i < PrincOpsExtras2::GFTIndex_LAST; i++) {
//			CARD32 o = i * SIZE(GFTItem);
//			CARD32 p = mGFT + o;
//			CARD32 globalframe = ReadDbl(p + 0);
//			CARD32 codebase    = ReadDbl(p + 2);
//
//			if (codebase == 0) break;
//			logger.info("GFT APPL  %4d  %4X  gf %8X   cb %8X", i, o, globalframe, codebase);
//		}
//	}
//	CARD32 esv = ReadDbl(PDA_OFFSET(available)); // first CARD32 of available is LONG POINTER TO ESV
//	logger.info("dumpESV esv  %8X", esv);
//	if (esv == 0) {
//		logger.warn("dumpESV esv is zero");
//		return;
//	}
//
//	// Sanity Check
//	{
//		CARD16 version = *Fetch(esv + OFFSET(CPSwapDefs::ExternalStateVector, version));
//		if (version != CPSwapDefs::currentVersion) {
//			logger.warn("dumpESV ESV version mismatch  %d", version);
//			return;
//		}
//	}
//
//	CARD32 loadState = ReadDbl(esv + OFFSET(CPSwapDefs::ExternalStateVector, loadState));
//	logger.info("dumpESV loadState %8X", loadState);
//
//	if (loadState == 0) {
//		logger.warn("dumpESV loadState is zero");
//		return;
//	}
//
//	// Sanity Check
//	{
//		CARD16 version = *Fetch(loadState + OFFSET(LoadStateFormat::Object, versionident));
//		if (version != LoadStateFormat::VersionID) {
//			logger.warn("dumpESV LoadState version mismatch  %d", version);
//			return;
//		}
//	}
//	{
////		CARD16 versionident = *Memory::getAddress(loadState + OFFSET(LoadStateFormat::Object, versionident));
//		CARD16 nModules     = *Memory::getAddress(loadState + OFFSET(LoadStateFormat::Object, nModules));
//		CARD16 maxModules   = *Memory::getAddress(loadState + OFFSET(LoadStateFormat::Object, maxModules));
//		CARD16 nBcds        = *Memory::getAddress(loadState + OFFSET(LoadStateFormat::Object, nBcds));
//		CARD16 maxBcds      = *Memory::getAddress(loadState + OFFSET(LoadStateFormat::Object, maxBcds));
//		CARD16 nextID       = *Memory::getAddress(loadState + OFFSET(LoadStateFormat::Object, nextID));
//		CARD16 moduleInfo   = *Memory::getAddress(loadState + OFFSET(LoadStateFormat::Object, moduleInfo));
//		CARD16 bcdInfo      = *Memory::getAddress(loadState + OFFSET(LoadStateFormat::Object, bcdInfo));
//		logger.info("dumpESV LoadState nModules    %4d / %4d", nModules, maxModules);
//		logger.info("dumpESV LoadState nBcds       %4d / %4d", nBcds, maxBcds);
//		logger.info("dumpESV LoadState nextID     %5d", nextID);
//		logger.info("dumpESV LoadState moduleInfo %5d", moduleInfo);
//		logger.info("dumpESV LoadState bcdInfo    %5d", bcdInfo);
//
//		{
//			CARD32 p = loadState + bcdInfo;
//			for(CARD32 i = 0; i < nBcds; i++) {
//				CARD16 u0   = *Memory::getAddress(p + 0);
//				CARD32 u1   = *Memory::getAddress(p + 1);
//				CARD32 u2   = *Memory::getAddress(p + 2);
//				CARD16 u3   = *Memory::getAddress(p + 3);
//
//				CARD32 base = u2 << 16 | u1;
//				CARD16 id   = u3;
//
//				CARD32 v = 0;
//				if (!Memory::isVacant(base)) v = *Memory::getAddress(base);
//				logger.info("bcd %4d pages = %3d  id = %5d  base = %8X  %5d", i, u0 & 0x3FFFU, id, base, v);
//
////				CARD16 version = *Memory::getAddress(loadState + base);
////				logger.info("bcd version  %5d", version);
//
//				p += SIZE(LoadStateFormat::BcdInfo);
//			}
//		}
//
//		{
//			CARD32 p = loadState + moduleInfo;
//			for(CARD32 i = 0; i < nModules; i++) {
////				if (i == 10) break;
//				CARD16 u0          = *Fetch(p + OFFSET(LoadStateFormat::ModuleInfo, u0));
//				CARD16 index       = *Fetch(p + OFFSET(LoadStateFormat::ModuleInfo, index));
//				CARD16 globalFrame = *Fetch(p + OFFSET(LoadStateFormat::ModuleInfo, globalFrame));
//
//				logger.info("mod %4d %3d  %04X  %04X", i, index, u0 & 0x7FFF, globalFrame);
//
//				p += SIZE(LoadStateFormat::ModuleInfo);
//			}
//		}
//
//	}
//	{
//		CARD16* p27 = Store(esv + OFFSET(CPSwapDefs::ExternalStateVector, u27));
//		*p27 = 0;
//	}
//}
//
//void a() {
//	CPSwapDefs::ExternalStateVector esv;
//	{
//		CARD32 ptr = 0;
//		// read first long word from PDA available
//		readObject(PDA + OFFSET(ProcessDataArea, available), &ptr);
//		readObject(ptr, &esv);
//	}
//	logger.info("esv       version      %8d", esv.version);
//	logger.info("esv       vmCount      %8d", esv.virtualMemoryCount);
//	logger.info("esv       mds          %8d", esv.mds);
//	logger.info("esv       loadState    %8X", esv.loadState);
//
//	LoadStateFormat::Object loadState;
//	readObject(esv.loadState, &loadState);
//	logger.info("loadState version      %8X", loadState.versionident);
//	logger.info("loadState mod       %4d / %4d", loadState.nModules, loadState.maxModules);
//	logger.info("loadState bcd       %4d / %4d", loadState.nBcds, loadState.maxBcds);
//	logger.info("loadState nextID       %8X", loadState.nextID);
//	logger.info("loadState moduleInfo   %8d", loadState.moduleInfo);
//	logger.info("loadState bcdInfo      %8d", loadState.bcdInfo);
//
//	for(CARD32 i = 0; i < loadState.nModules; i++) {
//		CARD32 p = esv.loadState + loadState.moduleInfo + (SIZE(LoadStateFormat::ModuleInfo) * i);
//		LoadStateFormat::ModuleInfo moduleInfo;
//		readObject(p, &moduleInfo);
//		logger.info("mod %4d %4d     %04X", moduleInfo.index, moduleInfo.cgfi, moduleInfo.globalFrame);
//	}
//
//	for(CARD32 i = 0; i < loadState.nBcds; i++) {
//		CARD32 p = esv.loadState + loadState.bcdInfo + (SIZE(LoadStateFormat::BcdInfo) * i);
//		LoadStateFormat::BcdInfo bcdInfo;
//		readObject(p, &bcdInfo);
//		logger.info("bcd %4d %4d %8X", bcdInfo.id, bcdInfo.pages, bcdInfo.base);
//
//		BcdDefs::BCD bcd;
//		readObject(bcdInfo.base, &bcd);
//		{
//			QDateTime time = QDateTime::fromTime_t(Util::toUnixTime(bcd.version.time));
//			logger.info("    %d#%d#%s", bcd.version.net, bcd.version.host, time.toString("yyyy-MM-dd HH:mm:ss").toLocal8Bit().constData());
//		}
//
//
//	}
//}
