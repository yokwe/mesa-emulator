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
// Service.cpp
//

#include "../util/Debug.h"
#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("cr/service");

#include "../courier/Service.h"

#include "../stub/Programs.h"

void Courier::Service::ServiceManager::addService(ServiceBase* service) {
	quint32 programCode = service->PROGRAM_CODE;
	quint32 versionCode = service->VERSION_CODE;

	if (serviceMap.contains(programCode)) {
		QMap<quint16, ServiceBase*>& versionMap = serviceMap[programCode];
		if (versionMap.contains(versionCode)) {
			logger.error("Unexpected duplicate %s %d", service->PROGRAM_NAME, versionCode);
			COURIER_FATAL_ERROR();
		} else {
			versionMap[versionCode] = service;
		}
	} else {
		QMap<quint16, ServiceBase*> versionMap;
		versionMap[versionCode] = service;
		serviceMap[programCode] = versionMap;
	}
}

void Courier::Service::ServiceManager::call(Protocol::Protocol2::CallMessage& callMessage, Block& request, Block& response) const {
	const quint16 transaction   = callMessage.transaction;
	const quint32 programCode   = callMessage.program;
	const quint16 versionCode   = callMessage.version;
	const quint16 procedureCode = callMessage.procedure;

	if (!serviceMap.contains(programCode)) {
		Protocol::Protocol2 protocol2 = Protocol::Protocol2::reject(callMessage, Protocol::RejectCode::noSuchProgramNumber);
		Courier::serialize(response, protocol2);
		return;
	}

	const QMap<quint16, ServiceBase*>& versionMap = serviceMap[programCode];
	if (!versionMap.contains(versionCode)) {
		Protocol::Protocol2 protocol2 = Protocol::Protocol2::reject(callMessage, Protocol::RejectCode::noSuchVersionNumber);
		Courier::serialize(response, protocol2);
		return;
	}

	ServiceBase* service = versionMap[versionCode];
	if (!service->isProcedureValid(procedureCode)) {
		Protocol::Protocol2 protocol2 = Protocol::Protocol2::reject(callMessage, Protocol::RejectCode::noSuchProcedureValue);
		Courier::serialize(response, protocol2);
		return;
	}

	Protocol::Protocol3::CallMessage callMessage3;
	callMessage3.transaction = transaction;
	callMessage3.program     = programCode;
	callMessage3.version     = versionCode;
	callMessage3.procedure   = procedureCode;

	call(callMessage3, request, response);
}
void Courier::Service::ServiceManager::call(Protocol::Protocol3::CallMessage& callMessage, Block& request, Block& response) const {
//	const quint16 transaction   = callMessage.transaction;
	const quint32 programCode   = callMessage.program;
	const quint16 versionCode   = callMessage.version;
	const quint16 procedureCode = callMessage.procedure;

	if (!serviceMap.contains(programCode)) {
		Protocol::Protocol3 protocol3 = Protocol::Protocol3::reject(callMessage, Protocol::RejectCode::noSuchProgramNumber);
		Courier::serialize(response, protocol3);
		return;
	}

	const QMap<quint16, ServiceBase*>& versionMap = serviceMap[programCode];
	if (!versionMap.contains(versionCode)) {
		QList<quint16> versionList = versionMap.keys();
		quint16 low  = *std::min_element(versionList.begin(), versionList.end());
		quint16 high = *std::max_element(versionList.begin(), versionList.end());

		Protocol::Protocol3 protocol3 = Protocol::Protocol3::reject(callMessage, Protocol::RejectCode::noSuchVersionNumber);
		protocol3.reject().reject.noSuchVersionNumber().low  = low;
		protocol3.reject().reject.noSuchVersionNumber().high = high;

		Courier::serialize(response, protocol3);
		return;
	}

	ServiceBase* service = versionMap[versionCode];
	if (!service->isProcedureValid(procedureCode)) {
		Protocol::Protocol3 protocol3 = Protocol::Protocol3::reject(callMessage, Protocol::RejectCode::noSuchProcedureValue);
		Courier::serialize(response, protocol3);
		return;
	}

	service->call(callMessage, request, response);
}
