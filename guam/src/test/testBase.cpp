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
// testBase.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("testBase");

#include "../util/GuiOp.h"

#include "testBase.h"

void testBase::initAV(CARD16 origin, CARD16 limit) {
	if ((sizeof(PrincOps::FrameSizeMap) / sizeof(PrincOps::FrameSizeMap[0])) != (PrincOps::LastAVHeapSlot + 1)) ERROR();
	if ((sizeof(PilotBootmesa::FrameWeightMap) / sizeof(PilotBootmesa::FrameWeightMap[0])) != (PrincOps::LastAVHeapSlot + 1)) ERROR();

	// fill with empty
	for(int i = 0; i < FSIndex_SIZE; i++) {
		page_AV[i] = (CARD16)AT_empty;
	}

	// build AllocationVector
	CARD16 p = origin;
	for(CARD32 fsi = 0; fsi <= PrincOps::LastAVHeapSlot; fsi++) {
		int size = PrincOps::FrameSizeMap[fsi];
		int weight = PilotBootmesa::FrameWeightMap[fsi];
		if (weight == 0) continue;

		for(int j = 0; j < weight; j++) {
			// align (p mod 4) == 0
			p = (CARD16)((p + 3) & ~0x03);

			// round up to next page boundary
			if (((p + 8) % PageSize) < (p % PageSize)) {
				p = (CARD16)((p + (PageSize - 1)) & ~0x00ff);
			}

			page_MDS[p + 0] = fsi; // word
			page_MDS[p + 1] = 0;   // returnlink
			page_MDS[p + 2] = 0;   // globallink
			page_MDS[p + 3] = 0;   // pc
			page_MDS[p + 4] = page_AV[fsi];
			page_AV[fsi] = p + SIZE(LocalOverhead);

			p = (CARD16)(p + size);
		}
	}
	if (limit <= p) {
		logger.debug("origin = %04X\n", origin);
		logger.debug("limit  = %04X\n", limit);
		logger.debug("p      = %04X\n", p);
		ERROR();
	}
}

void testBase::initGFT() {
	// fill with empty
	for(int i = 0; i < GFTIndex_SIZE; i++) {
		page_GFT[i + 0] = 0;
		page_GFT[i + 1] = 0;
		page_GFT[i + 2] = 0;
		page_GFT[i + 3] = 0;
	}


	GFTItem item;
	item.globalFrame = GF;
	item.codebase = CodeCache::CB();
	// GFI_GF
	page_GFT[GFI_GF + 0] = LowHalf(item.globalFrame);
	page_GFT[GFI_GF + 1] = HighHalf(item.globalFrame);
	page_GFT[GFI_GF + 2] = LowHalf(item.codebase);
	page_GFT[GFI_GF + 3] = HighHalf(item.codebase);
	// GFI_SD
	page_GFT[GFI_SD + 0] = LowHalf(item.globalFrame);
	page_GFT[GFI_SD + 1] = HighHalf(item.globalFrame);
	page_GFT[GFI_SD + 2] = LowHalf(item.codebase);
	page_GFT[GFI_SD + 3] = HighHalf(item.codebase);
	// GFI_ETT
	page_GFT[GFI_ETT + 0] = LowHalf(item.globalFrame);
	page_GFT[GFI_ETT + 1] = HighHalf(item.globalFrame);
	page_GFT[GFI_ETT + 2] = LowHalf(item.codebase);
	page_GFT[GFI_ETT + 3] = HighHalf(item.codebase);
	// GFI_EFC
	page_GFT[GFI_EFC + 0] = LowHalf(item.globalFrame);
	page_GFT[GFI_EFC + 1] = HighHalf(item.globalFrame);
	page_GFT[GFI_EFC + 2] = LowHalf(item.codebase);
	page_GFT[GFI_EFC + 3] = HighHalf(item.codebase);
}

void testBase::initSD() {
	// fill with empty
	for(int i = 0; i < 256; i++) {
		ControlLink item = LT_newProcedure;
		page_SD[i * 2 + 0] = LowHalf(item);
		page_SD[i * 2 + 1] = HighHalf(item);
	}

	for(int i = 0; i < 256; i++) {
		NewProcDesc item;
		item.pc = pc_SD | i;
		item.taggedGFI = (CARD16)(GFI_SD | LT_newProcedure);

		page_SD[i * 2 + 0] = LowHalf(item.u);
		page_SD[i * 2 + 1] = HighHalf(item.u);
		page_CB[item.pc / 2] = (CARD16)0;
	}
}

void testBase::initETT() {
	//scTrapTable a;
	// fill with empty
	ControlLink item = LT_newProcedure;
	for(int i = 0; i < 256; i++) {
		page_ETT[i * 2 + 0] = LowHalf(item);
		page_ETT[i * 2 + 1] = HighHalf(item);
	}

	for(int i = 0; i < 256; i++) {
		NewProcDesc item;
		item.pc = pc_ETT | i;
		item.taggedGFI = (CARD16)(GFI_ETT | LT_newProcedure);

		page_ETT[i * 2 + 0] = LowHalf(item.u);
		page_ETT[i * 2 + 1] = HighHalf(item.u);
		page_CB[item.pc / 2] = (CARD16)0;
	}
}

void testBase::initPDA() {
	page_PDA[0] = 0x0000; // ready: Queue
	page_PDA[1] = 1024;   // count: CARDINAL  PsbIndex_SIZE
	page_PDA[2] = 0x0000; // unused: UNSPEC
	page_PDA[3] = 0x0000; // available[0]
	page_PDA[4] = 0x0000; // available[1]
	page_PDA[5] = 0x0000; // available[2]
	page_PDA[6] = 0x0000; // available[3]
	page_PDA[7] = 0x0000; // available[4]

	// initialize state with StateVectorCountMap.
	{
		CARD16 q = SIZE(ProcessStateBlock) * PsbIndex_SIZE;
		const int nElement = sizeof(PilotBootmesa::StateVectorCountMap) / sizeof(PilotBootmesa::StateVectorCountMap[0]);
		for(int i = 0; i < nElement; i++) {
			const int count = PilotBootmesa::StateVectorCountMap[i];
			page_PDA[8 + i] = 0;
			for(int j = 0; j < count; j++) {
				page_PDA[q] = page_PDA[8 + i];
				page_PDA[8 + i] = q;
				q += SIZE(StateVector);
			}
		}
	}

	// initialize interrupt
	{
		CARD16 p = 8 + 8;
		for(int i = 0; i < 16; i++) {
			page_PDA[p++] = 0; // condition
			page_PDA[p++] = 0; // available
		}
	}

	// initialize fault
	{
		CARD16 p = 8 + 8 + 32;
		for(int i = 0; i < 8; i++) {
			page_PDA[p++] = 0; // queue
			page_PDA[p++] = 0; // condition
		}
	}

	// initialize block
	{
		CARD16 p = 8 + 8 + 32 + 16;
		for(int i = 0; i < PsbIndex_SIZE; i++) {
			page_PDA[p++] = 0; // link
			page_PDA[p++] = 0; // flags
			page_PDA[p++] = 0; // context
			page_PDA[p++] = 0; // timeout
			page_PDA[p++] = 0; // mds
			page_PDA[p++] = 0; // available
			page_PDA[p++] = 0; // sticky
			page_PDA[p++] = 0; // sticky
		}
	}
}


void testBase::setUp() {
	Memory::initialize(22, 20, (CARD16)0x00a0);
	Interpreter::initialize();

	GuiOp::setContext(new NullGuiOp);

	// mPDA = 0x0001000
	// mGFT = 0x0002000
	CodeCache::setCB(0x00030080);
	MDSCache::setMDS(0x00040000);
	GF  = 0x00050080 + SIZE(GlobalOverhead);
	PC  = 0x20;

	{
		CARD16 *p = Memory::getAddress(0x00030000);
		for(int i = 0; i < PageSize; i++) p[i] = 0x3000 + i;
	}
	{
		CARD16 *p = Memory::getAddress(0x00030000 + 0x100);
		for(int i = 0; i < PageSize; i++) p[i] = 0x3000 + 0x100 + i;
	}
	{
		CARD16 *p = Memory::getAddress(0x00030000 + 0x200);
		for(int i = 0; i < PageSize; i++) p[i] = 0x3000 + 0x200 + i;
	}
	{
		CARD16 *p = Memory::getAddress(0x00040000);
		for(int i = 0; i < PageSize; i++) p[i] = 0x4000 + i;
	}
	{
		CARD16 *p = Memory::getAddress(0x00040000 + 0x1000);
		for(int i = 0; i < PageSize; i++) p[i] = 0x4100 + i;
	}
	{
		CARD16 *p = Memory::getAddress(0x00050000);
		for(int i = 0; i < PageSize; i++) p[i] = 0x5000 + i;
	}

	page_PDA = Memory::getAddress(mPDA);
	page_GFT = Memory::getAddress(mGFT);
	page_CB  = Memory::getAddress(CodeCache::CB());
	page_MDS = Memory::getAddress(MDSCache::MDS());
	page_AV  = Memory::getAddress(MDSCache::MDS() + mAV);
	page_SD  = Memory::getAddress(MDSCache::MDS() + mSD);
	page_ETT = Memory::getAddress(MDSCache::MDS() + mETT);
	page_GF  = Memory::getAddress(GF);

	GFI = 1;

	GFI_GF  =  4; // 1
	GFI_SD  =  8; // 2
	GFI_ETT = 12; // 3
	GFI_EFC = 16; // 4

	pc_SD  = 0x1000;
	pc_ETT = 0x2000;

	initAV(0x0600, 0x1aff);
	initSD();
	initETT();
	initGFT();
	initPDA();

	int fsi = 10;
	LFCache::setLF(page_AV[fsi]);
	page_AV[fsi] = page_MDS[LFCache::LF()];
	page_LF  = Memory::getAddress(MDSCache::MDS() + LFCache::LF());

	PSB = 1;
}

void testBase::tearDown() {
	Memory::finalize();
	page_PDA = page_GFT = page_CB = page_AV = page_SD = page_ETT = page_LF = page_GF = 0;
}

