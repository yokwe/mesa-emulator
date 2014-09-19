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
// SocketManager.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("socketMan");

#include "../util/Debug.h"

#include "SocketManager.h"

#include "Courier.h"
#include "stub/StubEthernet.h"
#include "stub/StubDatagram.h"

void SocketManager::add(quint16 no, SocketManager::Socket* socket) {
	QMutexLocker mutexLocker(&mutex);
	Socket* that = map.value(no, 0);
	if (that) {
		logger.fatal("already registered  no = %d  listener = %s", no, that->name);
		RUNTIME_ERROR();
	}
	logger.info("Add    %4d(%s) %s", no, Courier::getSocketName(no), socket->name);
	map.insert(no, socket);
}
void SocketManager::remove(quint16 no) {
	QMutexLocker mutexLocker(&mutex);
	Socket* that = map.value(no, 0);
	if (!that) {
		logger.fatal("not registered  socket = %s", no, Courier::getSocketName(no));
		RUNTIME_ERROR();
	}
	logger.info("Remove %4d(%s) %s", no, Courier::getSocketName(no), that->name);
	map.remove(no);
}

void SocketManager::start() {
	if (thread) RUNTIME_ERROR();
	//
	thread = new SocketThread(*this);
	thread->start();
}
void SocketManager::stop() {
	if (!thread) RUNTIME_ERROR();
	//
	thread->stopThread();
	for(;;) {
		if (thread->isFinished()) break;
		QThread::msleep(200);
	}
	thread = 0;
}
int  SocketManager::isRunning() {
	return thread != 0;
}


void SocketManager::SocketThread::run() {
	for(;;) {
		if (stop.loadAcquire()) break;
		int opErrno = 0;
		int ret = socketManager.network.select(1, opErrno);
		if (ret == 0) continue;

		ByteBuffer request;
		ByteBuffer response;

		int dataLength = socketManager.network.receive(request.getData(), request.getCapacity(), opErrno);
		if (dataLength <= 0) RUNTIME_ERROR();
		request.setLimit((quint32)dataLength);

		Socket::Context context(socketManager.localNetworkNumber, socketManager.network.getAddress());

		// make alias of fields in context
		Courier::Ethernet::Header& reqEthernet = context.reqEthernet;
		Courier::Datagram::Header& reqDatagram = context.reqDatagram;
		Courier::Ethernet::Header& resEthernet = context.resEthernet;
		Courier::Datagram::Header& resDatagram = context.resDatagram;

		Courier::deserialize(request, reqEthernet);
		Courier::deserialize(request, reqDatagram);
		request.setLimit(reqDatagram.base + reqDatagram.length);
		//logger.debug("dataLength = %d  newLimit = %u", dataLength, request.getLimit());

		try {
			// check integrity of packet
			if (reqDatagram.checksum == Courier::Datagram::CHECKSUM_NONE) {
				// DON'T VERIFY CHECKSUM
			} else {
				if (reqDatagram.checksum != Courier::checksum(request.getData(), reqDatagram.base + 2, reqDatagram.length - 2)) {
					// packet data is corrupted
					throw PacketError(Courier::Error::ErrorNumber::BAD_CHECKSUM);
				}
			}

			// check destination network
			switch(reqDatagram.destination.network) {
			case Courier::Datagram::NETWORK_UNKNOWN:
			case Courier::Datagram::NETWORK_ALL:
				break;
			default:
				// If packet is for local network
				if (reqDatagram.destination.network == context.network) break;
				// packet is for other network
				// TODO forward this packet
				logger.warn("FORWARD %08X-%012llX-%s", reqDatagram.destination.network, reqDatagram.destination.host, Courier::getSocketName(reqDatagram.destination.socket));
				continue;
			}

			// Build response
			response.clear();

			// Build response ethernet
			resEthernet.base        = reqEthernet.base;
			resEthernet.destination = reqEthernet.source;
			resEthernet.source      = context.host;
			resEthernet.type        = reqEthernet.type;
			serialize(response, resEthernet);

			// Build response datagram (partial)
			resDatagram.base                = reqDatagram.base;
			resDatagram.checksum            = 0;
			resDatagram.length              = 0;
			resDatagram.flags               = reqDatagram.flags & 0x00ffU;
			resDatagram.destination.base    = reqDatagram.destination.base;
			resDatagram.destination.network = reqDatagram.source.network;
			resDatagram.destination.host    = reqDatagram.source.host;
			resDatagram.destination.socket  = reqDatagram.source.socket;
			resDatagram.source.base         = reqDatagram.source.base;
			resDatagram.source.network      = context.network;
			resDatagram.source.host         = context.host;
			resDatagram.source.socket       = reqDatagram.destination.socket;
			serialize(response, resDatagram);

			SocketManager::Socket* listener = socketManager.map.value(reqDatagram.destination.socket, 0);
			if (listener) {
				Courier::Datagram::PacketType reqPacketType = (Courier::Datagram::PacketType)(reqDatagram.flags & 0xff);
				logger.debug("ETHER     %012llX  %012llX  %04X", reqEthernet.destination, reqEthernet.source, reqEthernet.type);
				logger.debug("DATAGRAM  %04X %4d %02X %s  %08X-%012llX-%s  %08X-%012llX-%s",
					reqDatagram.checksum, reqDatagram.length, (reqDatagram.flags >> 8), Courier::getName(reqPacketType),
					reqDatagram.destination.network, reqDatagram.destination.host, Courier::getSocketName(reqDatagram.destination.socket),
					reqDatagram.source.network, reqDatagram.source.host, Courier::getSocketName(reqDatagram.source.socket));

				logger.debug("====");

				// Call listener if exists.
				QMutexLocker mutexLocker(&socketManager.mutex);
				listener->process(context, request, response);
			} else {
				// no one listen the socket
				logger.warn("NO SOCKET LISTENER  %s (%d)", Courier::getSocketName(reqDatagram.destination.socket), reqDatagram.destination.socket);
				throw PacketError(Courier::Error::ErrorNumber::NO_SOCKET);
			}
		} catch (PacketError& packetError) {
			// Don't send error packet of broadcast request
			if (reqDatagram.destination.network == Courier::Datagram::NETWORK_ALL) continue;
			if (reqDatagram.destination.host    == Courier::Datagram::HOST_ALL)    continue;

			// Clear buffer in case data is already written to response
			response.clear();

			// Build response
			serialize(response, resEthernet);

			// Build response
			resDatagram.flags               = (quint16)Courier::Datagram::PacketType::ERROR;
			serialize(response, resDatagram);

			// Build response
			Courier::Error::Header resError;
			resError.base      = response.getPos();
			resError.number    = packetError.errorNumber;
			resError.parameter = packetError.errorParameter;
			serialize(response, resError);

			// output request packet data from datagram
			quint8* p = request.getData() + reqDatagram.base;
			for(quint32 i = response.getPos(); i < reqDatagram.length; i++) {
				response.put8(*p++);
			}
		}

		// if something is written to response, sent it
		if (response.getLimit()) {
			// set length of response datagram
			resDatagram.length = response.getLimit() - reqDatagram.base;

			// Move position to last written data == limit
			response.setPos(response.getLimit());

			// if number of datagram size is odd, add one zero byte to make even length
			if (reqDatagram.length & 1) {
				response.put8(0);
			}

			// expand packet size to meet minimum packet length requirement if needed
			if (response.getLimit() < (reqDatagram.base + Courier::Ethernet::MIN_DATA_LENGTH)) {
				quint32 padSize = reqDatagram.base + Courier::Ethernet::MIN_DATA_LENGTH - response.getLimit();
				for(quint32 i = 0; i < padSize; i++) {
					response.put8(0);
				}
			}

			// write resDatagram to response (length and padding)
			serialize(response, resDatagram);

			// calculate checksum from response
			resDatagram.checksum = Courier::checksum(response.getData(), resDatagram.base + 2, resDatagram.length - 2);

			// write resDatagram to response
			serialize(response, resDatagram);

			// finally output response packet using response buffer.
			int opErrno = 0;
			int ret = socketManager.network.transmit(response.getData(), response.getLimit(), opErrno);
			if (opErrno) {
				logger.fatal("send fail opErrno = %d  ret = %d  resLimit = %d", opErrno, ret, response.getLimit());
				RUNTIME_ERROR();
			}
		}
	}
}
