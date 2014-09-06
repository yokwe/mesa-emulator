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
// Socket.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("socket");

#include "../util/Debug.h"

#include "Buffer.h"
#include "Datagram.h"
#include "Socket.h"

class ListenerThread : public QThread {
public:
	ListenerThread() {
		stop.storeRelease(0);
	}
	void run();
	void stopThread() {
		stop.storeRelease(1);
	}
protected:
	QAtomicInt stop;
};


static QMap<quint16, Socket::Listener*> map;
static Network*                         network = 0;
static ListenerThread*                  thread  = 0;
static QMutex                           mutex;

void Socket::registerListener(quint16 socket, Socket::Listener* listener) {
	QMutexLocker mutexLocker(&mutex);
	Listener* p = map.value(socket, 0);
	if (p) {
		logger.fatal("%s already registered  socket = %s  listener = %s", __FUNCTION__, socket, p->name);
		ERROR();
	}
	map.insert(socket, listener);
	logger.info("%s Register %4d(%s) %s", __FUNCTION__, socket, Datagram::getSocketName(socket), listener->name);
}
void Socket::unregisterListener(quint16 socket) {
	QMutexLocker mutexLocker(&mutex);
	Listener* p = map.value(socket, 0);
	if (!p) {
		logger.fatal("%s not registered  socket = %s", __FUNCTION__, socket, Datagram::getSocketName(socket));
		ERROR();
	}
	map.remove(socket);
}

void Socket::startListen(Network* network_) {
	if (thread) ERROR();
	//
	network = network_;
	thread = new ListenerThread;
	thread->start();
}
void Socket::stopListen() {
	if (!thread) ERROR();
	//
	thread->stopThread();
	for(;;) {
		if (thread->isFinished()) break;
		QThread::msleep(200);
	}
	network = 0;
	thread = 0;
}
int  Socket::isListening() {
	return thread != 0;
}


void ListenerThread::run() {
	for(;;) {
		if (stop.loadAcquire()) break;
		int opErrno = 0;
		int ret = network->select(1, opErrno);
		if (ret == 0) continue;

		quint8 data[Buffer::MAX_SIZE];
		ret = network->receive(data, sizeof(data), opErrno);

		DatagramBuffer request(data, ret);
		DatagramBuffer response;

		// Build response - Ethernet
		response.setDest  (request.getSource());
		response.setSource(network->getAddress());
		response.setType  (request.getType());

		// Build response - Datagram
		response.setChecksum(0);
		response.setLength(0);
		response.setHop(0);
		response.setType(request.getType());
		response.dest.set(
			request.source.getNetwork(),
			request.source.getHost(),
			request.source.getSocket());
		response.source.set(
			network->getNetwork(),
			network->getAddress(),
			request.dest.getSocket());

		quint16 socket = request.dest.getSocket();
		Socket::Listener* listener = map.value(socket, 0);
		if (listener) {
			logger.debug("ETHER     %012llX  %012llX  %04X", request.getDest(), request.getSource(), ((EthernetBuffer)request).getType());
			logger.debug("DATAGRAM  %04X  %04X %02X %s  %08X-%012llX-%s  %08X-%012llX-%s",
				request.getChecksum(), request.getLength(), request.getHop(), Datagram::getTypeName(request.getType()),
				request.dest.getNetwork(), request.dest.getHost(), Datagram::getSocketName(request.dest.getSocket()),
				request.source.getNetwork(), request.source.getHost(), Datagram::getSocketName(request.source.getSocket()));
			logger.debug("====");

			// Call listener if exists.
			QMutexLocker mutexLocker(&mutex);
			listener->process(&request, &response);
			if (response.getLimit()) {
				quint8* data    = response.getData();
				quint32 dataLen = response.getLimit();
				int opErrno     = 0;
				int ret = network->transmit(data, dataLen, opErrno);
				if (opErrno) {
					logger.fatal("send fail opErrno = %d  ret = %d  dataLen = %d", opErrno, ret, dataLen);
					ERROR();
				}
			}
		} else {
			logger.debug("ETHER     %012llX  %012llX  %04X", request.getDest(), request.getSource(), ((EthernetBuffer)request).getType());
			logger.debug("DATAGRAM  %04X  %04X %02X %s  %08X-%012llX-%s  %08X-%012llX-%s",
				request.getChecksum(), request.getLength(), request.getHop(), Datagram::getTypeName(request.getType()),
				request.dest.getNetwork(), request.dest.getHost(), Datagram::getSocketName(request.dest.getSocket()),
				request.source.getNetwork(), request.source.getHost(), Datagram::getSocketName(request.source.getSocket()));
			logger.debug("----");
		}
	}
}
