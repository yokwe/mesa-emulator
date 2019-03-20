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
// Time.cpp
//

#include "../util/Debug.h"
#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("time");

#include "../itp/IDP.h"
#include "../itp/PEX.h"
#include "../itp/Error.h"

#include "../pex/Time.h"

#include "../service/Time.h"

bool Service::Time::process(ITP::IDP& idp_request, ITP::IDP& idp_response) {
	if (idp_request.dst_socket != ITP::IDP::Socket::TIME) {
		logger.error("Unexpected socket %s", toString(idp_request).toLocal8Bit().constData());
		RUNTIME_ERROR();
	}
	switch(idp_request.packetType) {
	case ITP::IDP::PacketType::PEX:
	{
		ITP::PEX pex_request;
		pex_request.deserialize(idp_request.netData);

		ITP::PEX pex_response;
		pex_response.id         = pex_request.id;
		pex_response.clientType = pex_request.clientType;

		switch(pex_request.clientType) {
		case ITP::PEX::ClientType::TIME:
		{
			PEX::Time time_request;
			time_request.deserialize(pex_request.netData);

			PEX::Time time_response;
			time_response.version   = PEX::Time::VERSION_NUMBER;
			time_response.operation = PEX::Time::Operation::RESPONSE;

			if (time_request.version != PEX::Time::VERSION_NUMBER) {
				logger.error("Unexpected version      %d", time_request.version	);
				logger.error("Unexpected time_request %s", toString(time_request).toLocal8Bit().constData());
				logger.error("Unexpected idp_request  %s", toString(idp_request).toLocal8Bit().constData());
				logger.error("Unexpected data_request %s", toString(pex_request).toLocal8Bit().constData());
				RUNTIME_ERROR();
			}

			switch(time_request.operation) {
			case PEX::Time::Operation::REQUEST:
			{
				const QDateTime dateTime(QDateTime::currentDateTime());

				const QTimeZone             timeZone        = dateTime.timeZone();
				const quint32               currentTime     = dateTime.toTime_t();
				const QTimeZone::OffsetData offsetData      = timeZone.offsetData(dateTime);
				const qint32                offset          = offsetData.offsetFromUtc;
				const qint32                offsetInMinutes = ::abs(offset) / 60;

				// build response
				time_response.value.response.currentTime     = Util::toMesaTime(currentTime);
				time_response.value.response.offsetDirection = (offset < 0) ? PEX::Time::OffsetDirection::WEST : PEX::Time::OffsetDirection::EAST;
				time_response.value.response.offsetHours     = offsetInMinutes / 60;
				time_response.value.response.offsetMinutes   = offsetInMinutes % 60;
				// We don't care about DST
				time_response.value.response.startOfDST      = 0;
				time_response.value.response.endOfDST        = 0;
				// 10 milliseconds precision
				time_response.value.response.toleranceType   = PEX::Time::ToleranceType::IN_MILLI_SECONDS;
				time_response.value.response.tolerance       = 10;

				time_response.serialize(pex_response.netData);
				pex_response.serialize(idp_response.netData);
			}
				return true; // transmit response
			case PEX::Time::Operation::RESPONSE:
				logger.warn("Error %s", toString(time_request).toLocal8Bit().constData());
				break;
			default:
				logger.error("Unexpected operation    %s", toString(time_request.operation).toLocal8Bit().constData());
				logger.error("Unexpected time_request %s", toString(time_request).toLocal8Bit().constData());
				logger.error("Unexpected idp_request  %s", toString(idp_request).toLocal8Bit().constData());
				logger.error("Unexpected data_request %s", toString(pex_request).toLocal8Bit().constData());
				RUNTIME_ERROR();
				break;
			}
		}
			return true; // transmit response
		default:
			logger.error("Unexpected clientType   %s", toString(pex_request.clientType).toLocal8Bit().constData());
			logger.error("Unexpected idp_request  %s", toString(idp_request).toLocal8Bit().constData());
			logger.error("Unexpected data_request %s", toString(pex_request).toLocal8Bit().constData());
			RUNTIME_ERROR();
			break;
		}
	}
		break;
	case ITP::IDP::PacketType::ERROR:
	{
		ITP::Error data_request;
		data_request.deserialize(idp_request.netData);

		logger.warn("Error %s", toString(data_request).toLocal8Bit().constData());
	}
		break;
	default:
		logger.error("Unexpected packetType %s", toString(idp_request).toLocal8Bit().constData());
		RUNTIME_ERROR();
		break;
	}

	return false; // Don't transmit response
}

