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
// AgentKeyboard.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("agentkyeboard");


#include "../util/Debug.h"

#include "../mesa/Pilot.h"
#include "../mesa/Memory.h"

#include "Agent.h"
#include "AgentKeyboard.h"

void AgentKeyboard::Initialize() {
	if (fcbAddress == 0) ERROR();

	fcb = (KeyboardIOFaceGuam::KeyboardFCBType *)Store(fcbAddress);
	fcb->keyStates[0] = 0xffff;
	fcb->keyStates[1] = 0xffff;
	fcb->keyStates[2] = 0xffff;
	fcb->keyStates[3] = 0xffff;
	fcb->keyStates[4] = 0xffff;
	fcb->keyStates[5] = 0xffff;
	fcb->keyStates[6] = 0xffff;
}

void AgentKeyboard::Call() {
	logger.fatal("AGENT %s", name);
	ERROR();
}

void AgentKeyboard::keyPress(LevelVKeys::KeyName keyName) {
	const int keyStates_SIZE = sizeof(fcb->keyStates) / sizeof(fcb->keyStates[0]);
	const int a = keyName / WordSize;
	const int b = keyName % WordSize;

	if(keyStates_SIZE <= a) {
		logger.fatal("keyName = %3d  %02X", keyName, keyName);
		ERROR();
	}
	CARD16 mask = 1 << (WordSize - 1 - b);

	fcb->keyStates[a] &= ~mask;
}
void AgentKeyboard::keyRelease(LevelVKeys::KeyName keyName) {
	const int keyStates_SIZE = sizeof(fcb->keyStates) / sizeof(fcb->keyStates[0]);
	const int a = keyName / WordSize;
	const int b = keyName % WordSize;

	if(keyStates_SIZE <= a) {
		logger.fatal("keyName = %3d  %02X", keyName, keyName);
		ERROR();
	}
	CARD16 mask = 1 << (WordSize - 1 - b);

	fcb->keyStates[a] |= mask;
}
