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
// Agent.cpp
//


#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("agent");


#include "../mesa/Memory.h"

#include "Agent.h"


Agent* Agent::allAgent[GuamInputOutput::AgentDeviceIndex_SIZE] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

CARD32 Agent::ioRegion = Agent::ioRegionPtr;

GuamInputOutput::IORegionType *Agent::ioRegionType = 0;


Agent* Agent::getAgent(CARD16 index) {
	if (GuamInputOutput::AgentDeviceIndex_SIZE <= index) ERROR();
	Agent* agent = allAgent[index];
	if (agent == 0) ERROR();
	if (agent->index != index) ERROR();
	return agent;
}

Agent::Agent(GuamInputOutput::AgentDeviceIndex index_, char const *name_) : name(name_), index(index_) {
	if (allAgent[index]) ERROR();
	allAgent[index] = this;
	fcbAddress = 0;
}

void Agent::InitializeFCB() {
	CARD32 size = getFCBSize();
	fcbAddress = Alloc(size);
	ioRegionType->fcbptrs[index] = fcbAddress;

	logger.info("Agent %2d %-10s  fcb = %04X  %2d", index, name, fcbAddress, size);
};


void Agent::InitializeAgent() {
	// First allocate IORegionType at ioRegionPtr and fill with zero
	ioRegionType = (GuamInputOutput::IORegionType*)Store(Alloc(SIZE(GuamInputOutput::IORegionType)));
	for(int i = 0; i < GuamInputOutput::AgentDeviceIndex_SIZE; i++) {
		ioRegionType->fcbptrs[i] = 0;
	}

	// Then allocate FCB after IORegionType.
	for(int i = 0; i < GuamInputOutput::AgentDeviceIndex_SIZE; i++) {
		Agent *agent = allAgent[i];
		if (agent == 0) continue;
		if (agent->getFCBSize() == 0) continue;

		agent->InitializeFCB();
	}

	// Call Agent Initialize method to initialize fcb and state
	for(int i = 0; i < GuamInputOutput::AgentDeviceIndex_SIZE; i++) {
		Agent *agent = allAgent[i];
		if (agent == 0) continue;
		if (agent->getFCBSize() == 0) continue;

		agent->Initialize();
	}
}
