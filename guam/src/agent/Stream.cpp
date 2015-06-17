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
// Stream.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("stream");

#include "Stream.h"

const char* Stream::getCommandString(CARD16 key) {
	static QMap<CARD16, const char*> map = {
			{CoProcessorIOFaceGuam::C_idle,    "idle"},
			{CoProcessorIOFaceGuam::C_accept,  "accept"},
			{CoProcessorIOFaceGuam::C_connect, "connect"},
			{CoProcessorIOFaceGuam::C_delete,  "delete"},
			{CoProcessorIOFaceGuam::C_read,    "read"},
			{CoProcessorIOFaceGuam::C_write,   "write"},
	};
	if (map.contains(key)) return map[key];
	logger.fatal("Unknown key = %d", key);
	ERROR();
	return 0;
}

const char* Stream::getConnectionStateString(CARD16 key) {
	static QMap<CARD16, const char*> map = {
			{CoProcessorIOFaceGuam::S_idle,      "idle"},
			{CoProcessorIOFaceGuam::S_accepting, "accepting"},
			{CoProcessorIOFaceGuam::S_connected, "connected"},
			{CoProcessorIOFaceGuam::S_deleted,   "deleted"},
	};
	if (map.contains(key)) return map[key];
	logger.fatal("Unknown key = %d", key);
	ERROR();
	return 0;
}

const char* Stream::getResultString(CARD16 key) {
	static QMap<CARD16, const char*> map = {
			{CoProcessorIOFaceGuam::R_completed,  "completed"},
			{CoProcessorIOFaceGuam::R_inProgress, "inProgress"},
			{CoProcessorIOFaceGuam::R_error,      "error"},
	};
	if (map.contains(key)) return map[key];
	logger.fatal("Unknown key = %d", key);
	ERROR();
	return 0;
}

const char* Stream::getServerString(CARD32 key) {
	static QMap<CARD32, const char*> map {
		{CoProcessorServerIDs::fileAccess,                             "fileAccess"},
		{CoProcessorServerIDs::dragAndDropToGVService,                 "dragAndDrop"},
		{CoProcessorServerIDs::workspaceWindowControlGVService,        "wwc-gv"},
		{CoProcessorServerIDs::workspaceWindowControlMSWindowsService, "wwc-pc"},
		{CoProcessorServerIDs::tcpService,                             "tcpService"},
	};

	if (map.contains(key)) return map[key];
	static char buffer[20];
	sprintf(buffer, "ID-%d", key);
	return buffer;
}
