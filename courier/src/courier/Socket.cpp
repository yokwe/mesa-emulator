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
// Socket.cpp
//

#include "../util/Debug.h"
#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("cr/socket");

#include "../courier/Socket.h"

#include "../stub/Programs.h"

//
// Socket::SocketBase
//
void Courier::Socket::SocketBase::callInit() {
    if (initialized) {
        logger.error("Unexpected state initialized");
        COURIER_FATAL_ERROR();
    }
    init();
    initialized = true;
}

void Courier::Socket::SocketBase::callDestroy() {
    if (!initialized) {
        logger.error("Unexpected state initialized");
        COURIER_FATAL_ERROR();
    }
    destroy();
    initialized = false;
}
void Courier::Socket::SocketBase::callService(Frame& request, Frame& response, bool& sendResponse) const {
    if (!initialized) {
        logger.error("Unexpected state initialized");
        COURIER_FATAL_ERROR();
    }
    service(request, response, sendResponse);
}


//
// Socket::Manager
//
void Courier::Socket::Manager::addSocket(SocketBase* socketBase) {
	if (started) {
		logger.error("Unexpected state started");
		COURIER_FATAL_ERROR();
	}

	Socket socket = socketBase->socket;
	logger.info("addSocket  %s  %s", socketBase->name, Courier::toString(socketBase->socket).toLocal8Bit().constData());
	if (socketMap.contains(socket)) {
        logger.error("Unexpected duplicate socket");
        COURIER_FATAL_ERROR();
	}
	socketMap[socket] = socketBase;
}
void Courier::Socket::Manager::startService() {
	if (started) {
		logger.error("Unexpected state started");
		COURIER_FATAL_ERROR();
	}
	// init SocketBase
	logger.info("init    sockeMap");
	for(SocketBase* socketBase: socketMap.values()) {
		logger.info("init %s %s", socketBase->name, Courier::toString(socketBase->socket).toLocal8Bit().constData());
		socketBase->callInit();
	}
	logger.info("try start service thread");
	serviceThread = new ServiceThread(nic, socketMap);
	// reserve one thread
	QThreadPool::globalInstance()->reserveThread();
	QThreadPool::globalInstance()->start(serviceThread);
	logger.info("service thread started");
	started = true;
}
void Courier::Socket::Manager::stopService() {
	if (!started) {
		logger.error("Unexpected state started");
		COURIER_FATAL_ERROR();
	}
	logger.info("try stop  service thread");
	serviceThread->stop();
	for(int i = 0; i < 100; i++) {
		if (QThreadPool::globalInstance()->waitForDone(1000)) break;
		logger.info("try stop  service thread %2d", i + 1);
	}
	logger.info("service thread stopped");
	// release one thread
	QThreadPool::globalInstance()->releaseThread();
	serviceThread = nullptr;
	started       = false;
	logger.info("destroy sockeMap");
	for(SocketBase* socketBase: socketMap.values()) {
		logger.info("destroy %s %s", socketBase->name, Courier::toString(socketBase->socket).toLocal8Bit().constData());
		socketBase->callDestroy();
	}
}


//
// Socket::ServiceThread
//
void Courier::Socket::ServiceThread::run() {
	quint16 timeoutInSec = 1;
	quint64 myAddress    = nic.getAddress();


	Courier::NIC::Data  dataRequest;
	Courier::NIC::Frame etherRequest;
	Courier::IDP::Frame idpRequest;

	Courier::NIC::Data  dataResponse;
	Courier::NIC::Frame etherResponse;
	Courier::IDP::Frame idpResponse;


	for(;;) {
		if (stop) break;
		int ret = nic.select(timeoutInSec);
		if (ret == 0) continue;
		nic.receive(dataRequest.block);

		Courier::deserialize(dataRequest.block, etherRequest);
		Courier::deserialize(etherRequest.data, idpRequest);

		if (!socketMap.contains(idpRequest.dst.socket)) {
			logger.warn("No socket service  %s -> %s",
					Courier::toString(idpRequest.src).toLocal8Bit().constData(),
					Courier::toString(idpRequest.dst).toLocal8Bit().constData());
			continue;
		}
		SocketBase* socketBase = socketMap[idpRequest.dst.socket];

		dataResponse.block.zero();
		dataResponse.block.clear();

		// supply default values
		etherResponse.dst  = etherRequest.src;
		etherResponse.src  = myAddress;
		etherResponse.type = etherRequest.type;
		Courier::serialize(dataResponse.block, etherResponse);
		etherResponse.data = dataResponse.block.remainder();

		// supply default values
		idpResponse.checksum   = IDP::Checksum::NONE;
		idpResponse.length     = 0;
		idpResponse.hopCount   = IDP::HopCount::ZERO;
		idpResponse.packetType = idpRequest.packetType;
		idpResponse.dst        = idpRequest.src;
		idpResponse.src        = myAddress;
		Courier::serialize(etherResponse.data, idpResponse);
		idpResponse.data       = etherResponse.data.remainder();

		// call service method of socketBase
		bool sendResponse = false;
		try {
			socketBase->callService(idpRequest, idpResponse, sendResponse);
		} catch (...) {
			// FIXME catch error for error packet and create error packet
		}
		if (!sendResponse) continue;
		// Don't send error packet for broadcast request
		if (idpResponse.packetType == IDP::PacketType::ERROR) {
			if (idpRequest.dst == IDP::Host::ALL) continue;
		}
		// FIXME if packet size is less than 60, add padding
		// FIXME calculate checksum of idpResponse and set

		dataResponse.block.clear();
		Courier::serialize(etherResponse.data, idpResponse);
		dataResponse.block.clear();
		Courier::serialize(dataResponse.block, etherResponse);
		nic.transmit(dataResponse.block);
	}
}


