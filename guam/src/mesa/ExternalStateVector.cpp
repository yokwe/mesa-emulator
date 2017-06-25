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
// ExternalStateVector.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("esv");

#include "Memory.h"
#include "ExternalStateVector.h"

#define PDA_OFFSET(m) (PDA + OFFSET(ProcessDataArea, m))


void read(CARD32 ptr, CPSwapDefs::ExternalStateVector& externalStateVector) {
	{
		CARD16* p = (CARD16*)&externalStateVector;
		for(CARD32 i = 0; i < SIZE(externalStateVector); i++) *p++ = 0;
	}

	if (ptr == 0) {
		logger.warn("ptr is zero");
		return;
	}
	externalStateVector.version = Memory::read16(ptr + OFFSET(CPSwapDefs::ExternalStateVector, version));
	if (externalStateVector.version != CPSwapDefs::currentVersion) {
		logger.error("version mismatch.  %d", externalStateVector.version);
		ERROR();
		return;
	}
	externalStateVector.bootSession        = Memory::read32(ptr + OFFSET(CPSwapDefs::ExternalStateVector, bootSession));
	externalStateVector.loadState          = Memory::read32(ptr + OFFSET(CPSwapDefs::ExternalStateVector, loadState));
	externalStateVector.mds                = Memory::read16(ptr + OFFSET(CPSwapDefs::ExternalStateVector, mds));
	externalStateVector.virtualMemoryCount = Memory::read32(ptr + OFFSET(CPSwapDefs::ExternalStateVector, virtualMemoryCount));
	externalStateVector.u27                = Memory::read16(ptr + OFFSET(CPSwapDefs::ExternalStateVector, u27));
}

void read(CARD32 ptr, LoadStateFormat::Object& object) {
	{
		CARD16* p = (CARD16*)&object;
		for(CARD32 i = 0; i < SIZE(object); i++) *p++ = 0;
	}

	if (ptr == 0) {
		logger.warn("ptr is zero");
		return;
	}
	object.versionident = Memory::read16(ptr + OFFSET(LoadStateFormat::Object, versionident));
	if (object.versionident != LoadStateFormat::VersionID) {
		logger.warn("version mismatch.  %d", object.versionident);
		return;
	}

	object.nModules   = Memory::read16(ptr + OFFSET(LoadStateFormat::Object, nModules));
	object.maxModules = Memory::read16(ptr + OFFSET(LoadStateFormat::Object, maxModules));
	object.nBcds      = Memory::read16(ptr + OFFSET(LoadStateFormat::Object, nBcds));
	object.maxBcds    = Memory::read16(ptr + OFFSET(LoadStateFormat::Object, maxBcds));
	object.nextID     = Memory::read16(ptr + OFFSET(LoadStateFormat::Object, nextID));
	object.moduleInfo = Memory::read16(ptr + OFFSET(LoadStateFormat::Object, moduleInfo));
	object.bcdInfo    = Memory::read16(ptr + OFFSET(LoadStateFormat::Object, bcdInfo));
}
