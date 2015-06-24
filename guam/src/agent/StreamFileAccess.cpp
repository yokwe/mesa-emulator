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
// StreamFileAccess.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("fileaccess");

#include "StreamFileAccess.h"

#define DEBUG_SHOW_STREAM_FILE_ACCESS 1

const char* StreamFileAccess::toString(Command value) {
	static QMap<Command, const char*> map {
		{Command::setPath,          "setPath"},
		{Command::push,             "push"},
		{Command::pop,              "pop"},
		{Command::listFiles,        "listFiles"},
		{Command::deleteFile,       "deleteFile"},
		{Command::openFileForRead,  "openFileForRead"},
		{Command::readFileData,     "readFileData"},
		{Command::readFileAttr,     "readFileAttr"},
		{Command::openFileForWrite, "openFileForWrite"},
		{Command::writeFileData,    "writeFileData"},
		{Command::writeFileAttr,    "writeFileAttr"},
		{Command::closeFile,        "closeFile"},
		{Command::endStream,        "endStream"},
	};

	if (map.contains(value)) {
		return map[value];
	}
	logger.fatal("value = %d", value);
	ERROR();
	return "UNKNONW";
}
const char* StreamFileAccess::toString(Response value) {
	static QMap<Response, const char*> map {
		{Response::commandCompleted,         "commandCompleted"},
		{Response::directoryIsRoot,          "directoryIsRoot"},
		{Response::openedFileAlreadyExists,  "openedFileAlreadyExists"},
		{Response::openedFileRenamed,        "openedFileRenamed"},
		{Response::infoFollows,              "infoFollows"},
		{Response::dataFollows,              "dataFollows"},
		{Response::attrFollows,              "attrFollows"},
		{Response::errorMsg,                 "errorMsg"},
	};

	if (map.contains(value)) {
		return map[value];
	}
	logger.fatal("value = %d", value);
	ERROR();
	return "UNKNONW";
}


StreamFileAccess::StreamFileAccess() : AgentStream::Handler(CoProcessorServerIDs::fileAccess, "fileaccess") {}

AgentStream::Task* StreamFileAccess::createTask() {
	return new FileAccessTask();
}

void StreamFileAccess::idle   (CoProcessorIOFaceGuam::CoProcessorFCBType* /*fcb*/) {
}

AgentStream::Result StreamFileAccess::accept (CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/, AgentStream::Task* /*task*/) {
	return AgentStream::Result::completed;
}
AgentStream::Result StreamFileAccess::connect(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/, AgentStream::Task* /*task*/) {
	return AgentStream::Result::completed;
}
AgentStream::Result StreamFileAccess::destroy(CoProcessorIOFaceGuam::CoProcessorIOCBType* /*iocb*/, AgentStream::Task* /*task*/) {
	return AgentStream::Result::error;
}

void StreamFileAccess::run() {
	logger.info("StreamFileAccess::run START");

	stopThread = 0;
	QThread::currentThread()->setPriority(PRIORITY);
	int processCount = 0;

	QByteArray commandData;
	try {
		for(;;) {
			if (stopThread) break;

			{
				AgentStream::StreamData streamData = getData();
				QByteArray data = streamData.data;
				commandData.append(data);

				// Append data to commandData until endRecord
				if (!streamData.endRecord) continue;
			}

			Command cmd = static_cast<Command>(commandData.at(0));
			logger.debug("Command  %s  %s", toString(cmd), Util::toString(commandData));
			switch(cmd) {
			case Command::writeFileAttr:
				writeFileAttrProcess(commandData);
				break;
			default:
				ERROR();
				break;
			}
		}
	} catch(AgentStream::Handler::StopThreadException&) {
		//
	}

	logger.info("processCount           = %8u", processCount);
	logger.info("StreamFileAccess::run STOP");
}

void StreamFileAccess::writeFileAttrProcess(const QByteArray& /*data_*/) {
	{
		QByteArray data(1, static_cast<char>(Response::directoryIsRoot));
		AgentStream::StreamData streamData(data);
		putData(streamData);
	}
	{
		QByteArray data(1, static_cast<char>(Response::commandCompleted));
		AgentStream::StreamData streamData(data);
		putData(streamData);
	}
}
