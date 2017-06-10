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
// testBase.h
//

#ifndef TESTBASE_H__
#define TESTBASE_H__

#include <cppunit/extensions/HelperMacros.h>

#include "../mesa/Constant.h"
#include "../mesa/Type.h"
#include "../mesa/Memory.h"
#include "../mesa/Function.h"
#include "../mesa/MesaThread.h"

#include "../mesa/Pilot.h"

#include "../simple-opcode/Interpreter.h"


class testBase : public CppUnit::TestFixture {
protected:
	void initRegister();
	void initAV(CARD16 origin, CARD16 limit);
	void initGFT();
	void initSD();
	void initETT();
	void initPDA();

	CARD16 *page_PDA;
	CARD16 *page_GFT;
	CARD16 *page_CB;
	CARD16 *page_MDS;
	CARD16 *page_AV;
	CARD16 *page_SD;
	CARD16 *page_ETT;
	CARD16 *page_LF;
	CARD16 *page_GF;

	CARD16 GFI_GF;
	CARD16 GFI_SD;
	CARD16 GFI_ETT;
	CARD16 GFI_EFC;

	CARD16 pc_SD;
	CARD16 pc_ETT;

public:
	void setUp();
	void tearDown();
};

#endif
