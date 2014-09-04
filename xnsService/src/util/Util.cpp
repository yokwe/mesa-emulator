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
// Util.cpp
//

#include "Util.h"

#include <log4cpp/PropertyConfigurator.hh>

#ifdef __linux__
#include <execinfo.h>
#endif

static log4cpp::Category& logger = Logger::getLogger("util");

// Logger
log4cpp::Category& Logger::getLogger(const char *name) {
	char *fileName = getenv("LOG4CPP_CONFIG");
	if (fileName) log4cpp::PropertyConfigurator::configure(fileName);
	if (log4cpp::Category::exists(name)) {
		log4cpp::Category::getRoot().fatal("Duplicate logger name = %s", name);
		exit(1);
	}
	return log4cpp::Category::getInstance(name);
}

static void setLoggerPriority(log4cpp::Priority::Value newValue) {
	for(auto i: *log4cpp::Category::getCurrentCategories()) {
		i->setPriority(newValue);
	}
}

static QStack<log4cpp::Priority::Value> loggerPriorityStack;
void Logger::pushPriority(log4cpp::Priority::Value newValue) {
	loggerPriorityStack.push(log4cpp::Category::getRootPriority());
	setLoggerPriority(newValue);
}
void Logger::popPriority() {
	if (loggerPriorityStack.empty()) return;

	log4cpp::Priority::Value newValue = loggerPriorityStack.pop();
	setLoggerPriority(newValue);
}


void logBackTrace() {
#ifdef __linux__
	const int BUFFER_SIZE = 100;
	void *buffer[BUFFER_SIZE];

	// get void*'s for all entries on the stack
	int size = backtrace(buffer, BUFFER_SIZE);

	char **msg = backtrace_symbols(buffer, size);

	// print out all the frames to fno
	for(int i = 0; i < size; i++) {
		logger.fatal("%3d %s", i, msg[i]);
	}
#endif
}

static void signalHandler(int signum) {
	logger.fatal("Error: signal %d:\n", signum);
	logBackTrace();
	exit(1);
}

void setSignalHandler(int signum) {
	signal(signum, signalHandler);
}
