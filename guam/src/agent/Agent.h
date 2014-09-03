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
// Agent.h
//

#ifndef AGENT_H__
#define AGENT_H__

#include "../mesa/Pilot.h"
#include "../mesa/Constant.h"

class Agent {
public:
	// ioRegionLast: CARDINAL = 255;
	// ioRegionAfterEnd: CARDINAL = ioRegionLast + 1;
	static const CARD16 ioRegionLast = 255;
	static const CARD16 ioRegionAfterEnd = ioRegionLast + 1;

	// rp-vp  0x60-0x00, 0x61-0x01, ... 0xff-0x9f
	//   ioRegionPage = 0xff - 0x9f = 0x60
	//   ioRegionPageCount = 0x100 - 0x60 = 0xa0
	// ioRegionPage = ioRegionLast - PageMap.GetState[ioRegionLast].real;
	// ioRegionPtr = Environment.LongPointerFromPage[ioRegionPage];
	// ioRegionPageCount = ioRegionAfterEnd - Inline.LowHalf[ioRegionPage];
	static const CARD32 ioRegionPage = 0x80; // TODO What is a optimal value for ioRegionPage?  ?GermOpsImpl::pageEndGermVM?
	static const CARD32 ioRegionPtr = ioRegionPage * PageSize;


	static void InitializeAgent();

	static Agent* getAgent(CARD16 index);

	static void CallAgent(CARD16 index) {
		Agent* agent = getAgent(index);
		agent->Call();
	}

	const char *name;
	const GuamInputOutput::AgentDeviceIndex index;
	CARD32 fcbAddress;

	Agent(GuamInputOutput::AgentDeviceIndex index_, char const *name_);
	virtual ~Agent() {}

	void InitializeFCB();

	virtual CARD32 getFCBSize() = 0;
	virtual void Initialize() = 0;
	virtual void Call() = 0;

	static CARD32 getIORegion() {
		return ioRegion;
	}

protected:
	static Agent *allAgent[GuamInputOutput::AgentDeviceIndex_SIZE];

	static CARD32 ioRegion;
	static GuamInputOutput::IORegionType *ioRegionType;

	static inline CARD32 roundUp(CARD32 n) {
		if (n & 1) n++;
		return n;
	}
	static CARD32 Alloc(CARD32 size) {
		CARD32 ret = ioRegion;
		ioRegion += roundUp(size);
		if (size == 0) ret = 0;
		return ret;
	}
};


//class AgentNull : public Agent {
//public:
//	AgentNull() : Agent(GuamInputOutput::null, "null") {}
//
//	CARD32 getFCBSize() {
//		return 0;
//	}
//
//	void Initialize() {}
//	void Call() {
//		ERROR();
//	}
//};

#endif
