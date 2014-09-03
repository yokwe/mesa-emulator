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
// AgentMouse.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("agentmouse");


#include "../util/Debug.h"

#include "../mesa/Pilot.h"
#include "../mesa/Memory.h"

#include "Agent.h"
#include "AgentMouse.h"

void AgentMouse::Initialize() {
	if (fcbAddress == 0) ERROR();

	fcb = (MouseIOFaceGuam::MouseFCBType *)Memory::getAddress(fcbAddress);
	fcb->currentMousePosition.mouseXCoord = 0;
	fcb->currentMousePosition.mouseYCoord = 0;
	fcb->cursorOffset.mouseXCoord         = 0;
	fcb->cursorOffset.mouseYCoord         = 0;
	fcb->newValue.mouseXCoord             = 0;
	fcb->newValue.mouseYCoord             = 0;
	fcb->command = MouseIOFaceGuam::C_nop;
}

void AgentMouse::Call() {
	switch(fcb->command) {
	case MouseIOFaceGuam::C_nop:
		if (DEBUG_SHOW_AGENT_MOUSE) logger.debug("AGENT %s nop", name);
		break;
	case MouseIOFaceGuam::C_setPosition:
		if (DEBUG_SHOW_AGENT_MOUSE) logger.debug("AGENT %s setPosition  %4d %4d", name, fcb->newValue.mouseXCoord, fcb->newValue.mouseYCoord);
		fcb->currentMousePosition.mouseXCoord = fcb->newValue.mouseXCoord;
		fcb->currentMousePosition.mouseYCoord = fcb->newValue.mouseYCoord;
		// TODO Implement MouseIOFaceGuam::C_setPosition
		break;
	case MouseIOFaceGuam::C_setCursorPosition:
		if (DEBUG_SHOW_AGENT_MOUSE) logger.debug("AGENT %s setCursorPosition  %4d %4d", name, fcb->newValue.mouseXCoord, fcb->newValue.mouseYCoord);
		// TODO Implement MouseIOFaceGuam::C_setCursorPosition
		break;
	default:
		logger.fatal("command = %d", fcb->command);
		ERROR();
		break;
	}
}
