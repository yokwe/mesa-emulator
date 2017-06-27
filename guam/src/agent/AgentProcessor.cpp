/*
Copyright (c) 2014, Yasuhiro Hasegawa
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
// AgentProcessor.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("agentproc");


#include "../util/Debug.h"

#include "../mesa/Pilot.h"
#include "../mesa/Memory.h"
#include "../mesa/Constant.h"

#include "Agent.h"
#include "AgentProcessor.h"

static quint32 getMesaTime() {
	return Util::toMesaTime(Util::getUnixTime());
}

void AgentProcessor::Initialize() {
	if (fcbAddress == 0) ERROR();

	fcb = (ProcessorIOFaceGuam::ProcessorFCBType *)Store(fcbAddress);
	fcb->processorID[0]               = processorID0;
	fcb->processorID[1]               = processorID1;
	fcb->processorID[2]               = processorID2;
	fcb->microsecondsPerHundredPulses = 100; // one pulse = 1 microsecond
	fcb->millisecondsPerTick          = cTick;
	fcb->alignmentFiller              = 0;
	fcb->realMemoryPageCount          = Memory::getRPSize();
	fcb->virtualMemoryPageCount       = Memory::getVPSize();
	fcb->gmt                          = getMesaTime();
	fcb->command                      = ProcessorIOFaceGuam::C_noop;
	fcb->status                       = ProcessorIOFaceGuam::S_success;
}

static CARD32 gmtDifference = 0;
void AgentProcessor::Call() {
	switch (fcb->command) {
	case ProcessorIOFaceGuam::C_noop:
		if (DEBUG_SHOW_AGENT_PROCESSOR) logger.debug("AGENT %s noop", name);
		fcb->status = ProcessorIOFaceGuam::S_success;
		break;
	case ProcessorIOFaceGuam::C_readGMT:
		if (DEBUG_SHOW_AGENT_PROCESSOR) logger.debug("AGENT %s readGMT", name);
		fcb->gmt = getMesaTime() + gmtDifference;
		fcb->status = ProcessorIOFaceGuam::S_success;
		break;
	case ProcessorIOFaceGuam::C_writeGMT:
		gmtDifference = fcb->gmt - getMesaTime();
		if (DEBUG_SHOW_AGENT_PROCESSOR) logger.debug("AGENT %s writeGMT  %5d", name, gmtDifference);
		break;
	default:
		if (DEBUG_SHOW_AGENT_PROCESSOR) logger.debug("AGENT %s %d", name, fcb->command);
		ERROR();
		break;
	}
}
