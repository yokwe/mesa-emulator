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
// Pilot.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("pilot");

#include "Pilot.h"

const int PrincOps::FrameSizeMap[PrincOps::AVHeapSize - 1] = {
	   8,   12,   16,   20,   24,
	  28,   32,   40,   48,   56,
	  68,   80,   96,  112,  128,
	 148,  168,  192,  224,  252,
	 508,  764, 1020, 1276, 1532,
	1788, 2044, 2556, 3068, 3580, 4092
};

const int PilotBootmesa::StateVectorCountMap[8] = {
 // 0  1  2  3  4  5  6  7 - priority level
	1, 3, 2, 2, 4, 1, 3, 1
};

const int PilotBootmesa::FrameWeightMap[PrincOps::AVHeapSize - 1] = {
	20, 26, 15, 16, 16,
	12,  8,  8,  5,  5,
	 7,  2,  2,  1,  1,
	 1,  1,  1,  1,  0,
	 0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0, 0
};
