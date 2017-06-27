/*
Copyright (c) 2017, Yasuhiro Hasegawa
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
// BcdOps.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("bcdops");

#include "../mesa/Memory.h"

#include "BCDOps.h"

VersionStamp::VersionStamp(const TimeStamp::Stamp& stamp) {
	this->net  = stamp.net;
	this->host = stamp.host;
	this->time = QDateTime::fromTime_t(Util::toUnixTime(stamp.time));
}
QString VersionStamp::toString() {
	return QString("%1#%2#%3").arg(net, 0, 16).arg(host, 0, 16).arg(time.toString("yyyy-MM-dd HH:mm:ss"));
}

BCDOps::BCDOps(CARD32 ptr) {
	if (Memory::isVacant(ptr)) {
		logger.fatal("ptr is not mapped");
		ERROR();
	}
	readObject(ptr, &header);

	version = VersionStamp(header.version);
	creator = VersionStamp(header.version);

	logger.info("version %s", version.toString().toLocal8Bit().constData());
	logger.info("creator %s", creator.toString().toLocal8Bit().constData());

}
