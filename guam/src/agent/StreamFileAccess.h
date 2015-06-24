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
// StreamFileAccess.h
//

#ifndef STREAMFILEACCESS_H__
#define STREAMFILEACCESS_H__

#include "AgentStream.h"

class StreamFileAccess : public AgentStream::Handler {
public:
	//FileAccessProtocol.Command: TYPE = MACHINE DEPENDENT {setPath(1), push(2), pop(3), listFiles(4), deleteFile(5), openFileForRead(6), readFileData(7), readFileAttr(8), openFileForWrite(9), writeFileData(10), writeFileAttr(11), closeFile(12), endStream(255)};
	enum class Command : CARD8 {
		setPath          = 1,
		push             = 2,
		pop              = 3,
		listFiles        = 4,
		deleteFile       = 5,
		openFileForRead  = 6,
		readFileData     = 7,
		readFileAttr     = 8,
		openFileForWrite = 9,
		writeFileData    = 10,
		writeFileAttr    = 11,
		closeFile        = 12,
		endStream        = 255,
	};

	//FileAccessProtocol.Response: TYPE = MACHINE DEPENDENT {commandCompleted(0), directoryIsRoot(1), openedFileAlreadyExists(2), openedFileRenamed(3), infoFollows(4), dataFollows(5), attrFollows(6), errorMsg(255)};
	enum class Response : CARD8 {
		commandCompleted        = 0,
		directoryIsRoot         = 1,
		openedFileAlreadyExists = 2,
		openedFileRenamed       = 3,
		infoFollows             = 4,
		dataFollows             = 5,
		attrFollows             = 6,
		errorMsg                = 255,
	};

	class FileAccessTask : public AgentStream::Task {
	public:
		//
	};

	static const char* toString(Command value);
	static const char* toString(Response value);

	StreamFileAccess();

	AgentStream::Task* createTask();

	void idle   (CoProcessorIOFaceGuam::CoProcessorFCBType* fcb);

	AgentStream::Result accept (CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb, AgentStream::Task* task);
	AgentStream::Result connect(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb, AgentStream::Task* task);
	// To avoid conflict with keyword "delete", use "destroy" instead.
	AgentStream::Result destroy(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb, AgentStream::Task* task);

	void run();


	void writeFileAttrProcess(const QByteArray& data);
};

#endif
