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
// AgentDisplay.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("agentdisplay");


#include "../util/Debug.h"

#include "../mesa/Pilot.h"
#include "../mesa/Memory.h"

#include "../util/GuiOp.h"

#include "Agent.h"
#include "AgentDisplay.h"

void AgentDisplay::Initialize() {
	if (fcbAddress == 0) ERROR();

	fcb = (DisplayIOFaceGuam::DisplayFCBType *)Memory::getAddress(fcbAddress);
	fcb->command                = DisplayIOFaceGuam::C_nop;
	fcb->status                 = DisplayIOFaceGuam::S_success;
	fcb->displayMemoryAddress   = Memory::getDisplayRealPage();
	fcb->color.red              = 0;
	fcb->color.green            = 0;
	fcb->color.blue             = 0;
	fcb->color.reserved         = 0;
	fcb->colorMapping[0]        = 0;
	fcb->colorMapping[1]        = 1;
	fcb->destRectangle.origin.x = 0;
	fcb->destRectangle.origin.y = 0;
	fcb->destRectangle.width    = 0;
	fcb->destRectangle.height   = 0;
	fcb->sourceOrigin.x         = 0;
	fcb->sourceOrigin.y         = 0;
	fcb->cursorPattern.word[ 0] = 0;
	fcb->cursorPattern.word[ 1] = 0;
	fcb->cursorPattern.word[ 2] = 0;
	fcb->cursorPattern.word[ 3] = 0;
	fcb->cursorPattern.word[ 4] = 0;
	fcb->cursorPattern.word[ 5] = 0;
	fcb->cursorPattern.word[ 6] = 0;
	fcb->cursorPattern.word[ 7] = 0;
	fcb->cursorPattern.word[ 8] = 0;
	fcb->cursorPattern.word[ 9] = 0;
	fcb->cursorPattern.word[10] = 0;
	fcb->cursorPattern.word[11] = 0;
	fcb->cursorPattern.word[12] = 0;
	fcb->cursorPattern.word[13] = 0;
	fcb->cursorPattern.word[14] = 0;
	fcb->cursorPattern.word[15] = 0;
	fcb->pattern[0]             = 0;
	fcb->pattern[1]             = 0;
	fcb->pattern[2]             = 0;
	fcb->pattern[3]             = 0;
	fcb->patternFillMode        = DisplayIOFaceGuam::PFM_copy;
	fcb->complemented           = 0;
	fcb->colorIndex             = 0;
	fcb->displayType            = DisplayIOFaceGuam::T_monochrome;
	fcb->displayWidth           = this->displayWidth;
	fcb->displayHeight          = this->displayHeight;
}

void AgentDisplay::Call() {
	switch (fcb->command) {
	case DisplayIOFaceGuam::C_nop:
		if (DEBUG_SHOW_AGENT_DISPLAY) logger.debug("AGENT %s nop", name);
		break;
	case DisplayIOFaceGuam::C_setCLTEntry:
		if (DEBUG_SHOW_AGENT_DISPLAY) logger.debug("AGENT %s setCLTEntry", name);
		// TODO AgentDisplay::Call DisplayIOFaceGuam::C_setCLTEntry
		break;
	case DisplayIOFaceGuam::C_getCLTEntry:
		if (DEBUG_SHOW_AGENT_DISPLAY) logger.debug("AGENT %s getCLTEntry  colorIndex = %d", name, fcb->colorIndex);
		// TODO AgentDisplay::Call DisplayIOFaceGuam::C_getCLTEntry
	    if (1 < fcb->colorIndex) fcb->status = DisplayIOFaceGuam::S_readOnlyCLT;
	    else {
	        fcb->color.red      = 255;
	        fcb->color.green    = 255;
	        fcb->color.blue     = 255;
	        fcb->color.reserved =   0;
	        fcb->status = DisplayIOFaceGuam::S_success;
	    }

		switch (fcb->colorIndex) {
		case ColorDisplayFace::black:
			break;
		case ColorDisplayFace::white:
			break;
		default:
			break;
		}
		break;
	case DisplayIOFaceGuam::C_setBackground:
		logger.warn("AGENT %s setBackground", name);
		// Called when make Sword dormant
		// TODO AgentDisplay::Call DisplayIOFaceGuam::C_setBackground
		break;
	case DisplayIOFaceGuam::C_setCursorPattern:
		if (DEBUG_SHOW_AGENT_DISPLAY) logger.debug("AGENT %s setCursorPattern", name);
		{
			GuiOp::CursorPattern data;
			for(CARD32 i = 0; i < ELEMENTSOF(data.data); i++) {
				data.data[i] = qFromBigEndian(fcb->cursorPattern.word[i]);
			}
			GuiOp::setCursorPattern(&data);
		}
		break;
	case DisplayIOFaceGuam::C_updateRectangle:
		logger.fatal("AGENT %s updateRectangle", name);
		ERROR();
		// TODO AgentDisplay::Call DisplayIOFaceGuam::C_updateRectangle
		break;
	case DisplayIOFaceGuam::C_copyRectangle:
		logger.fatal("AGENT %s copyRectangle", name);
		ERROR();
		// TODO AgentDisplay::Call DisplayIOFaceGuam::C_copyRectangle
		break;
	case DisplayIOFaceGuam::C_patternFillRectangle:
		logger.fatal("AGENT %s patternFillRectangle", name);
		ERROR();
		// TODO AgentDisplay::Call DisplayIOFaceGuam::C_patternFillRectangle
		break;
	default:
		logger.fatal("AGENT %s %5d", name, fcb->command);
		ERROR();
		break;
	}
}
