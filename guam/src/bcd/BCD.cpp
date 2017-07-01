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
// BCD.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("bcd");

#include "../mesa/Memory.h"

#include "BCD.h"

VersionStamp::VersionStamp(BCDData *data) {
	net  = data->getCARD8();
	host = data->getCARD8();
	time = data->getCARD32();
	//	logger.info("versionStamp  %02X %02X %08X", net, host, time);

	dateTime = QDateTime::fromTime_t(Util::toUnixTime(time));
}

QString VersionStamp::toString() {
	return QString("%1#%2#%3%4").arg(dateTime.toString("yyyyMMdd")).arg(dateTime.toString("HHmmss")).arg(net, 2, 16, QChar('0')).arg(host, 2, 16, QChar('0'));
}

BCD::BCD(BCDData *data_) {
	data = data_;

	versionIdent = data->getCARD16();
	if (versionIdent != VersionID) {
		logger.fatal("versionIdent %d", versionIdent);
		ERROR();
	}

	version = VersionStamp(data);
	creator = VersionStamp(data);

	logger.info("version %s", version.toString().toLocal8Bit().constData());
	logger.info("creator %s", creator.toString().toLocal8Bit().constData());

	sourceFile     = data->getCARD16();
	unpackagedFile = data->getCARD16();

	nConfigs = data->getCARD16();
	nModules = data->getCARD16();
	nImports = data->getCARD16();
	nExports = data->getCARD16();
	logger.info("nConfigs %d", nConfigs);
	logger.info("nModules %d", nModules);
	logger.info("nImports %d", nImports);
	logger.info("nExports %d", nExports);

	CARD16 flags = data->getCARD16();
	nPages        = flags >> 8;
	definitions   = (flags & 0x80) != 0;
	repackaged    = (flags & 0x40) != 0;
	typeExported  = (flags & 0x20) != 0;
	tableCompiled = (flags & 0x10) != 0;
	spare4        = flags & 0x0f;

	logger.info("nPages   %d", nPages);
	logger.info("defs     %s", definitions ? "T" : "F");


	firstDummy = data->getCARD16();
	nDummies   = data->getCARD16();
	ssOffset   = data->getCARD16();
	ssLimit    = data->getCARD16();
	ctOffset   = data->getCARD16();
	ctLimit    = data->getCARD16();
	mtOffset   = data->getCARD16();
	mtLimit    = data->getCARD16();
	impOffset  = data->getCARD16();
	impLimit   = data->getCARD16();
	expOffset  = data->getCARD16();
	expLimit   = data->getCARD16();
	enOffset   = data->getCARD16();
	enLimit    = data->getCARD16();
	sgOffset   = data->getCARD16();
	sgLimit    = data->getCARD16();
	ftOffset   = data->getCARD16();
	ftLimit    = data->getCARD16();
	spOffset   = data->getCARD16();
	spLimit    = data->getCARD16();
	ntOffset   = data->getCARD16();
	ntLimit    = data->getCARD16();
	typOffset  = data->getCARD16();
	typLimit   = data->getCARD16();
	tmOffset   = data->getCARD16();
	tmLimit    = data->getCARD16();
	fpOffset   = data->getCARD16();
	fpLimit    = data->getCARD16();
	lfOffset   = data->getCARD16();
	lfLimit    = data->getCARD16();
	atOffset   = data->getCARD16();
	atLimit    = data->getCARD16();
	apOffset   = data->getCARD16();
	apLimit    = data->getCARD16();

	//
	initializeNameRecord();
}

void BCD::initializeNameRecord() {
		int offset = ssOffset;
		int limit  = ssLimit;

		if (limit == 0) return;

		data->position(offset + 2);
		data->getCARD8();
		for(;;) {
			int bytePos = data->bytePosition();
			int pos = data->position();

			if ((offset + limit) <= pos) break; // position exceed limit

			int length = data->getCARD8();
			QString value;
			for(int i = 0; i < length; i++) {
				CARD8 byte = data->getCARD8();
				value.append(byte);
			}

			int index = bytePos - (offset * 2) - 3;
//			logger.info("ss %4d %s!", index, value.toLocal8Bit().constData());
			ss[index] = value;
		}
		// Add special
		ss[1] = "#NULL#";
	}

