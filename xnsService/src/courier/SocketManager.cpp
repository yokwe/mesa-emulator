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
// Datagram.cpp
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
		ERROR();
	}
	logger.info("Add    %4d(%s) %s", no, Courier::getSocketName(no), socket->name);
	map.insert(no, socket);
}
void SocketManager::remove(quint16 no) {
	QMutexLocker mutexLocker(&mutex);
	Socket* that = map.value(no, 0);
	if (!that) {
		logger.fatal("not registered  socket = %s", no, Courier::getSocketName(no));
		ERROR();
	}
	logger.info("Remove %4d(%s) %s", no, Courier::getSocketName(no), that->name);
	map.remove(no);
}

void SocketManager::start() {
	if (thread) ERROR();
	//
	thread = new SocketThread(*this);
	thread->start();
}
void SocketManager::stop() {
	if (!thread) ERROR();
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
		if (dataLength <= 0) ERROR();
		request.setLimit((quint32)dataLength);

		Courier::Ethernet::Header ethernet;
		Courier::Datagram::Header datagram;

		Courier::deserialize(request, ethernet);
		Courier::deserialize(request, datagram);
		request.setPos(datagram.base + datagram.length);
		request.rewind();
		logger.debug("dataLength = %d  newLimit = %u", dataLength, request.getLimit());

		quint16 socketNo = datagram.destination.socket;
		Courier::Datagram::PacketType packetType = (Courier::Datagram::PacketType)(datagram.flags & 0xff);

		logger.debug("ETHER     %012llX  %012llX  %04X", ethernet.destination, ethernet.source, ethernet.type);
		logger.debug("DATAGRAM  %04X %4d %02X %s  %08X-%012llX-%s  %08X-%012llX-%s",
			datagram.checksum, datagram.length, (datagram.flags >> 8), Courier::getName(packetType),
			datagram.destination.network, datagram.destination.host, Courier::getSocketName(datagram.destination.socket),
			datagram.source.network, datagram.source.host, Courier::getSocketName(datagram.source.socket));


		SocketManager::Socket* listener = socketManager.map.value(socketNo, 0);
		if (listener) {
			logger.debug("====");

			// Call listener if exists.
			QMutexLocker mutexLocker(&socketManager.mutex);
			listener->process(request, response);
			if (response.getLimit()) {
				quint8* data    = response.getData();
				quint32 dataLen = response.getLimit();
				int opErrno     = 0;
				int ret = socketManager.network.transmit(data, dataLen, opErrno);
				if (opErrno) {
					logger.fatal("send fail opErrno = %d  ret = %d  dataLen = %d", opErrno, ret, dataLen);
					ERROR();
				}
			}
		} else {
			logger.debug("----");
		}
	}
}
