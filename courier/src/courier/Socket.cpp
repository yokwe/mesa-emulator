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

#include "../stub/Ethernet.h"
#include "../stub/IDP.h"
#include "../stub/Error.h"

//
// Socket::SocketBase
//
void Courier::Socket::Listener::callInit() {
    if (initialized) {
        logger.error("Unexpected state initialized");
        COURIER_FATAL_ERROR();
    }
    init();
    initialized = true;
}

void Courier::Socket::Listener::callDestroy() {
    if (!initialized) {
        logger.error("Unexpected state initialized");
        COURIER_FATAL_ERROR();
    }
    destroy();
    initialized = false;
}
void Courier::Socket::Listener::callService(Frame& request, Frame& response, bool& sendResponse) const {
    if (!initialized) {
        logger.error("Unexpected state initialized");
        COURIER_FATAL_ERROR();
    }
    service(request, response, sendResponse);
}


//
// Socket::Manager
//
void Courier::Socket::Manager::addNetwork(Network network, quint16 hop) {
	if (started) {
		logger.error("Unexpected state started");
		COURIER_FATAL_ERROR();
	}

	if (networkMap.contains(network)) {
        logger.error("Unexpected duplicate network = %d", network);
        COURIER_FATAL_ERROR();
	}
	networkMap[network] = hop;
}

void Courier::Socket::Manager::addListener(Listener* socketBase) {
	if (started) {
		logger.error("Unexpected state started");
		COURIER_FATAL_ERROR();
	}

	Socket socket = socketBase->socket;
	logger.info("addSocket  %s  %s", socketBase->name, Courier::toString(socketBase->socket).toLocal8Bit().constData());
	if (listenerMap.contains(socket)) {
        logger.error("Unexpected duplicate socket = %s", Courier::toString(socket));
        COURIER_FATAL_ERROR();
	}
	listenerMap[socket] = socketBase;
}
void Courier::Socket::Manager::startService() {
	if (started) {
		logger.error("Unexpected state started");
		COURIER_FATAL_ERROR();
	}
	// init SocketBase
	logger.info("init    sockeMap");
	for(Listener* socketBase: listenerMap.values()) {
		logger.info("init %s %s", socketBase->name, Courier::toString(socketBase->socket).toLocal8Bit().constData());
		socketBase->callInit();
	}
	logger.info("try start service thread");
	thread  = new Thread(context);
	started = true;
	// reserve one thread
	QThreadPool::globalInstance()->reserveThread();
	QThreadPool::globalInstance()->start(thread);
	logger.info("service thread started");
}
void Courier::Socket::Manager::stopService() {
	if (!started) {
		logger.error("Unexpected state started");
		COURIER_FATAL_ERROR();
	}
	logger.info("try stop  service thread");
	thread->stop();
	for(int i = 0; i < 100; i++) {
		if (QThreadPool::globalInstance()->waitForDone(1000)) break;
		logger.info("try stop  service thread %2d", i + 1);
	}
	logger.info("service thread stopped");
	// release one thread
	QThreadPool::globalInstance()->releaseThread();
	thread  = nullptr;
	started = false;
	logger.info("destroy sockeMap");
	for(Listener* socketBase: listenerMap.values()) {
		logger.info("destroy %s %s", socketBase->name, Courier::toString(socketBase->socket).toLocal8Bit().constData());
		socketBase->callDestroy();
	}
}


//
// Socket::ServiceThread
//
void Courier::Socket::Thread::run() {
	namespace Error = Stub::Error;
	namespace Ether = Stub::Ethernet;
	namespace IDP   = Stub::IDP;

	quint16 timeoutInSec = 1;
	Host    myAddress    = (Host)context.nic.getAddress();

	NIC&    nic       = context.nic;
	Network myNetwork = context.myNetwork;

//	FIXME QMap<Network, quint16>&  networkMap  = context.networkMap;
	QMap<Socket, Listener*>& listenerMap = context.listenerMap;

	BlockData<Ether::FRAME_LENGTH_MAX> reqData;
	BlockData<Ether::FRAME_LENGTH_MAX> resData;

	Ether::Frame reqEther;
	IDP::Frame   reqIDP;

	Ether::Frame resEther;
	IDP::Frame   resIDP;

	for(;;) {
		if (needStop) break;

		{
			int ret = nic.select(timeoutInSec);
			if (ret == 0) continue;
		}

		reqData.block.zero();
		reqData.block.clear();

		{
			int ret = nic.receive(reqData.block.getData(), reqData.block.getCapacity());
			reqData.block.setLimit(ret);
			reqData.block.rewind();
		}
		logger.info("reqEther  %s", Courier::toString(reqEther).toLocal8Bit().constData());
		logger.info("reqData.block  %s", Courier::toString(reqData.block).toLocal8Bit().constData());
		Courier::deserialize(reqData.block, reqEther);
		Courier::deserialize(reqEther.data, reqIDP);

		if (!listenerMap.contains(reqIDP.dst.socket)) {
			logger.warn("No listener %s --  %s %s %s -> %s %s %s",
					Courier::toString(reqIDP.type).toLocal8Bit().constData(),
					Courier::toString(reqIDP.src.network).toLocal8Bit().constData(),
					Courier::toString(reqIDP.src.host).toLocal8Bit().constData(),
					Courier::toString(reqIDP.src.socket).toLocal8Bit().constData(),
					Courier::toString(reqIDP.dst.network).toLocal8Bit().constData(),
					Courier::toString(reqIDP.dst.host).toLocal8Bit().constData(),
					Courier::toString(reqIDP.dst.socket).toLocal8Bit().constData());
			continue;
		}
		Listener* socketBase = listenerMap[reqIDP.dst.socket];

		resData.block.zero();
		resData.block.clear();

		// supply default values
		resEther.dst  = reqEther.src;
		resEther.src  = myAddress;
		resEther.type = reqEther.type;

		Courier::serialize(resData.block, resEther);
		resEther.data = resData.block.remainder();

		// supply default values
		resIDP.checksum       = reqIDP.checksum;
		resIDP.length         = 0;
		resIDP.control.trace  = false;
		resIDP.control.filler = 0;
		resIDP.control.hop    = 0;
		resIDP.type           = reqIDP.type;

		resIDP.dst.network = reqIDP.src.network;
		resIDP.dst.host    = reqIDP.src.host;
		resIDP.dst.socket  = reqIDP.src.socket;

		resIDP.src.network = myNetwork;
		resIDP.src.host    = myAddress;
		resIDP.src.socket  = reqIDP.dst.socket;

		Courier::serialize(resEther.data, resIDP);
		resIDP.data = resEther.data.remainder();

		// call service method of socketBase
		bool sendResponse = false;
		try {
			socketBase->callService(reqIDP, resIDP, sendResponse);
		} catch (Error::Type& e) {
			// Change packet type to ERROR
			resIDP.type = IDP::Type::ERR;
			// Clear block in case of middle of writing
			resIDP.data.clear();
			// Need to send response
			sendResponse = true;

			Error::Frame frame;
			frame.type      = e;
			frame.parameter = 0;
			Courier::serialize(resIDP.data, frame);
		}
		if (!sendResponse) continue;
		// Don't send error packet for broadcast request
		if (resIDP.type == IDP::Type::ERR && reqIDP.dst.host == IDP::Host::ALL)
			continue;
		// If packet size is less than Ether::DATA_LENGTH_MIN, add padding
		{
			quint16 minimum = Ether::DATA_LENGTH_MIN - IDP::HEADER_LENGTH;
			quint16 size = resIDP.data.getLimit();
			if (size < minimum) {
				quint16 paddingSize = minimum - size;
				logger.info("addPadding %d", paddingSize);
				resIDP.data.addPadding(paddingSize);
			}
		}
		// Calculate checksum of resIDP and set
		if (resIDP.checksum != IDP::Checksum::NONE) {
			resIDP.checksum = (IDP::Checksum)resEther.data.computeChecksum();
			// update resEther.data
			Courier::serialize(resEther.data, resIDP);
		}

		// FIXME

		resData.block.clear();
		Courier::serialize(resEther.data, resIDP);
		resData.block.clear();
		Courier::serialize(resData.block, resEther);
		nic.transmit(resData.block.getData(), resData.block.getLimit());
	}
}

QString Courier::Socket::toStarStyleAddress(quint64 value) {
	const QChar ZERO = QLatin1Char('0');

	// special case for less than 1000
	if (value < 1000) {
		return QString("0-%1").arg(QString("%1").arg(value, 3, 10, ZERO));
	}

	QString ret(QString("%1").arg(value % 1000, 3, 10, ZERO));
	value = value / 1000;

	for(;;) {
		quint64 nnn = value % 1000;

		if (value < 1000) {
			ret.prepend(QString("%1-").arg(nnn, 0, 10, ZERO));
			break;
		} else {
			ret.prepend(QString("%1-").arg(nnn, 3, 10, ZERO));
		}

		value = value / 1000;
	}

	return ret;
}
