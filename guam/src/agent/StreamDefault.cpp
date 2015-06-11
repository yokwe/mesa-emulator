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
// StreamDefault.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("default");


#include "../util/Debug.h"

#include "StreamDefault.h"

#define DEBUG_SHOW_STREAM_DEFAULT 0

StreamDefault::StreamDefault() : AgentStream::Handler(AgentStream::DEFAULT_HANDLER_ID, "DEFAULT") {}

CARD16 StreamDefault::idle(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) {
	if (DEBUG_SHOW_STREAM_DEFAULT) debugDump(logger, __FUNCTION__, iocb);
	return CoProcessorIOFaceGuam::R_error;
}
CARD16 StreamDefault::accept(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) {
	if (DEBUG_SHOW_STREAM_DEFAULT) debugDump(logger, __FUNCTION__, iocb);
	return CoProcessorIOFaceGuam::R_error;
}
CARD16 StreamDefault::connect(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) {
	if (DEBUG_SHOW_STREAM_DEFAULT) debugDump(logger, __FUNCTION__, iocb);
	return CoProcessorIOFaceGuam::R_error;
}
CARD16 StreamDefault::destroy(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) {
	if (DEBUG_SHOW_STREAM_DEFAULT) debugDump(logger, __FUNCTION__, iocb);
	return CoProcessorIOFaceGuam::R_error;
}
CARD16 StreamDefault::read(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) {
	if (DEBUG_SHOW_STREAM_DEFAULT) debugDump(logger, __FUNCTION__, iocb);
	return CoProcessorIOFaceGuam::R_error;
}
CARD16 StreamDefault::write(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) {
	if (DEBUG_SHOW_STREAM_DEFAULT) debugDump(logger, __FUNCTION__, iocb);
	return CoProcessorIOFaceGuam::R_error;
}
