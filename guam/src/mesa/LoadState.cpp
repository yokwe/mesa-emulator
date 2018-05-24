/*
Copyright (c) 2018, Yasuhiro Hasegawa
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
// LoadState.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("loadState");

#include "Memory.h"
#include "LoadState.h"

#include "../symbols/BCD.h"
#include "../symbols/BCDFile.h"

void dumpLoadState() {
	CARD32 pesv = ReadDbl(CPSwapDefs::PSEV);

	CPSwapDefs::ExternalStateVector esv;
	esv.version = *Fetch(pesv + OFFSET(CPSwapDefs::ExternalStateVector, version));
	logger.info("esv.version            %5d", esv.version);
	if (esv.version != CPSwapDefs::currentVersion) {
		logger.debug("wrong esv.version");
		return;
	}

	esv.u27 = ReadDbl(pesv + OFFSET(CPSwapDefs::ExternalStateVector, u27));
	// loadStateDirty
	logger.info("esv.loadStateDirty     %5d", esv.loadStateDirty);
	// loadStateChanging
	logger.info("esv.loadStateChanging  %5d", esv.loadStateChanging);
	if (esv.loadStateChanging) {
		logger.debug("loadStateChanging");
		return;
	}

	esv.loadState = ReadDbl(pesv + OFFSET(CPSwapDefs::ExternalStateVector, loadState));
	if (esv.loadState == 0) {
		logger.debug("loadState == 0");
		return;
	}

	LoadStateFormat::Object loadState;
	loadState.versionident = *Fetch(esv.loadState + OFFSET(LoadStateFormat::Object, versionident));
	loadState.nModules     = *Fetch(esv.loadState + OFFSET(LoadStateFormat::Object, nModules));
	loadState.maxModules   = *Fetch(esv.loadState + OFFSET(LoadStateFormat::Object, maxModules));
	loadState.nBcds        = *Fetch(esv.loadState + OFFSET(LoadStateFormat::Object, nBcds));
	loadState.maxBcds      = *Fetch(esv.loadState + OFFSET(LoadStateFormat::Object, maxBcds));
	loadState.nextID       = *Fetch(esv.loadState + OFFSET(LoadStateFormat::Object, nextID));
	loadState.moduleInfo   = *Fetch(esv.loadState + OFFSET(LoadStateFormat::Object, moduleInfo));
	loadState.bcdInfo      = *Fetch(esv.loadState + OFFSET(LoadStateFormat::Object, bcdInfo));

	logger.info("loadState.versionident %5d", loadState.versionident);
	if (loadState.versionident != LoadStateFormat::VersionID) {
		logger.fatal("loadState.versionident != LoadStateFormat::VersionID");
		ERROR();
	}

	logger.info("loadState.nModules     %5d", loadState.nModules);
	logger.info("loadState.maxModules   %5d", loadState.maxModules);
	logger.info("loadState.nBcds        %5d", loadState.nBcds);
	logger.info("loadState.maxBcds      %5d", loadState.maxBcds);
	logger.info("loadState.nextID       %5d", loadState.nextID);
	logger.info("loadState.moduleInfo   %5d", loadState.moduleInfo);
	logger.info("loadState.bcdInfo      %5d", loadState.bcdInfo);

	//BcdInfo: TYPE = MACHINE DEPENDENT RECORD [
	//  exports(0:0..0), typeExported(0:1..1): BOOLEAN,
	//  pages(0:2..15): [1..256],
	//  base(1): LONG BASE POINTER TO BcdDefs.BCD,
	//  id(3): ID];
//	struct BcdInfo {
//		union {
//			CARD16 u0;
//			struct {
//				CARD16 pages        : 14;
//				CARD16 typeExported :  1;
//				CARD16 exports      :  1;
//			};
//		};
//		CARD32 base;
//		CARD16 id;
//	} __attribute__((packed));
	{
		for(CARD16 i = 0; i < loadState.nBcds; i++) {
			CARD32 bcdInfoBase = esv.loadState + loadState.bcdInfo + SIZE(LoadStateFormat::BcdInfo) * i;

			LoadStateFormat::BcdInfo bcdInfo;
			bcdInfo.u0   = *Fetch(bcdInfoBase  + OFFSET(LoadStateFormat::BcdInfo, u0));
			bcdInfo.base = ReadDbl(bcdInfoBase + OFFSET(LoadStateFormat::BcdInfo, base));
			bcdInfo.id   = *Fetch(bcdInfoBase  + OFFSET(LoadStateFormat::BcdInfo, id));
			logger.info("----");
			logger.info("bcdInfo %4d %8X+%4d", i, bcdInfo.base, bcdInfo.pages);

//			BcdDefs::BCD bcd;
//			bcd.versionIdent = *Fetch(bcdInfo.base  + OFFSET(BcdDefs::BCD, versionIdent));
//			logger.info("bcd.versionIdent %d", bcd.versionIdent);
//			if (bcd.versionIdent != BcdDefs::VersionID) {
//				logger.warn("Bogus bcd version  %d", bcd.versionIdent);
//				break;
//			}
			if (Memory::isVacant(bcdInfo.base)) {
				logger.info("VACANT");
			} else {
				BCDFile* bcdFile = BCDFile::getInstance(bcdInfo.base);
				BCD bcd(bcdFile);
			}
		}
	}
}