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
// PEXTime.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("pexTime");

#include "../util/Debug.h"

#include "PEXTime.h"

#include "../Courier.h"

#include <QDateTime>


// Unix Time Epoch  1970-01-01 00:00:00
// Mesa Time Epoch  1968-01-01 00:00:00
//   Difference between above 2 date is 731 days.
static const quint32 EPOCH_DIFF = Courier::Time2::EARLIEST_TIME + (quint32)(731 * 60 * 60 * 24);

static quint32 toMesaTime(quint32 unixTime) {
	return unixTime + EPOCH_DIFF;
}

static quint32 toUnixTime(quint32 mesaTime) {
	return mesaTime - EPOCH_DIFF;
}

void PEXTime::process(const Context& /*context*/, ByteBuffer& request, ByteBuffer& response) {
	using namespace Courier;

	Time2::Packet reqPacket;
	deserialize(request, reqPacket);

	Time2::Packet resPacket;
	resPacket.base      = reqPacket.base;
	resPacket.data.base = reqPacket.data.base;

	if (reqPacket.version != Time2::VERSION) {
		logger.error("Unexpected version = %d", reqPacket.version);
		throw SocketManager::PacketError(Error::ErrorNumber::UNSPECIFIED);
	}

	switch(reqPacket.data.tag) {
	case Time2::PacketType::REQUEST: {
		const QDateTime dateTime(QDateTime::currentDateTime());
		const QTimeZone timeZone               = dateTime.timeZone();
		//const quint32 currentTime              = dateTime.toTime_t() + ((24 * 60 * 60) * 2);
		const quint32 currentTime              = dateTime.toTime_t();
		const QTimeZone::OffsetData offsetData = timeZone.offsetData(dateTime);
		const qint32 offset                    = offsetData.offsetFromUtc;
		const qint32 offsetInMinutes           = ::abs(offset) / 60;

		// start of build response
		resPacket.version   = Time2::VERSION;

		resPacket.data.tag  = Time2::PacketType::RESPONSE;

		resPacket.data.RESPONSE.currentTime     = toMesaTime(currentTime);
		resPacket.data.RESPONSE.offsetDirection = (offset < 0) ? Time2::OffsetDirection::WEST : Time2::OffsetDirection::EAST;
		resPacket.data.RESPONSE.offsetHours     = offsetInMinutes / 60;
		resPacket.data.RESPONSE.offsetMinutes   = offsetInMinutes % 60;
		// We don't care about DST
		resPacket.data.RESPONSE.startOfDST      = 0;
		resPacket.data.RESPONSE.endOfDST        = 0;
		// 10 milliseconds precision
		resPacket.data.RESPONSE.toleranceType   = Time2::ToleranceType::IN_MILLI_SECONDS;
		resPacket.data.RESPONSE.tolerance       = 10;

		serialize(response, resPacket);

		// end of building response
		response.rewind();

		{
			QDateTime t;
			t.setTime_t(toUnixTime(resPacket.data.RESPONSE.currentTime));
			logger.debug("REQUEST  %s  %08X", qPrintable(t.toString("yyyy-MM-dd HH:mm:ss")), resPacket.data.RESPONSE.currentTime);
		}
	}
		break;
	case Time2::PacketType::RESPONSE:
		// DO NOTHING
		logger.fatal("RESPONSE");
		RUNTIME_ERROR();
		break;
	default:
		logger.fatal("tag = %d", (quint16)reqPacket.data.tag);
		RUNTIME_ERROR();
		break;
	}
}
