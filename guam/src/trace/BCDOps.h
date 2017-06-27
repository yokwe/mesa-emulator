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
// BCDOps.h
//

#ifndef BCDOPS_H_
#define BCDOPS_H_ 1

#include "../mesa/Pilot.h"

#include "Trace.h"

#include <QtCore>

class VersionStamp {
public:
	CARD8     net;
	CARD8     host;
	QDateTime time;

	VersionStamp(const TimeStamp::Stamp& stamp);
	VersionStamp(const VersionStamp& that) : net(that.net), host(that.host), time(that.time) {}
	VersionStamp() : net(0), host(0), time(QDateTime::fromTime_t(0)) {}
	QString toString();
};

class FTRecord {
public:
	QString          name;
	TimeStamp::Stamp stamp;
	FTRecord(CARD32& ptr);
	FTRecord(const FTRecord& that) : name(that.name), stamp(that.stamp) {}
	FTRecord() {}
};

class BCDOps {
public:
	BCDOps(CARD32 ptr);

	VersionStamp version;
	VersionStamp creator;

	QMap<CARD16, QString>  ss;
	QMap<CARD16, FTRecord> ft;

private:
	BcdDefs::BCD header;
};

#endif


