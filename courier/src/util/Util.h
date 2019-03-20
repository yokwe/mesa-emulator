/*
Copyright (c) 2019, Yasuhiro Hasegawa
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
// Util.h
//

#ifndef UTIL_H__
#define UTIL_H__

#include <signal.h>

#include <log4cpp/Category.hh>
#include <QtCore>

class RuntimeError {
public:
	const char *func;
	const char *file;
	const int line;

	RuntimeError(const char *func_, const char *file_, const int line_) : func(func_), file(file_), line(line_) {}
};

#define RUNTIME_ERROR() { logger.fatal("ERROR %s %d %s", __FILE__, __LINE__, __FUNCTION__); logBackTrace(); throw RuntimeError(__FUNCTION__, __FILE__, __LINE__); }

#define DEBUG_TRACE() logger.debug("****  TRACE  %-20s %5d %s", __FUNCTION__, __LINE__, __FILE__)


void logBackTrace();

void setSignalHandler(int signum = SIGSEGV);

class Logger {
public:
	static log4cpp::Category& getLogger(const char *name);

	static void pushPriority(log4cpp::Priority::Value newValue);
	static void popPriority();
};

class Util {
public:
	static void*   mapFile  (const QString& path, quint32& mapSize, bool readOnly = false);
	static void    unmapFile(void* page);

	// Unix Time Epoch  1970-01-01 00:00:00
	// Mesa Time Epoch  1968-01-01 00:00:00
	//   Difference between above 2 date is 731 days.
    static const quint32 EARLIEST_TIME = 2114294400U;
	static const quint32 EPOCH_DIFF = EARLIEST_TIME + (quint32)(731 * 60 * 60 * 24);

	static quint32 toMesaTime(quint32 unixTime) {
		return unixTime + EPOCH_DIFF;
	}

	static quint32 toUnixTime(quint32 mesaTime) {
		return mesaTime - EPOCH_DIFF;
	}
};
#endif
