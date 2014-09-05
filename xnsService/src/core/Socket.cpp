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

QMap<quint16, Socket::Listener*> Socket::map;
Network*                         Socket::network = 0;
Socket::ListenerThread*          Socket::thread  = 0;
QMutex                           Socket::mutex;

void Socket::regist(quint16 socket, Socket::Listener* listener) {
	QMutexLocker mutexLocker(&mutex);
	Listener* p = map.value(socket, 0);
	if (p) {
		logger.fatal("%s already registered  socket = %s  listener = %s", __FUNCTION__, socket, p->name);
		ERROR();
	}
	map.insert(socket, listener);
	logger.info("%s Regist %4d(%s) %s", __FUNCTION__, socket, Datagram::getSocketName(socket), listener->name);
}
void Socket::unregist(quint16 socket) {
	QMutexLocker mutexLocker(&mutex);
	Listener* p = map.value(socket, 0);
	if (!p) {
		logger.fatal("%s not registered  socket = %s", __FUNCTION__, socket, Datagram::getSocketName(socket));
		ERROR();
	}
	map.remove(socket);
}

void Socket::start(Network* network_) {
	if (thread) ERROR();
	//
	network = network_;
	thread = new ListenerThread;
	thread->start();
}
void Socket::stop() {
	if (!thread) ERROR();
	//
	logger.info("Socket::stop START");
	thread->stopThread();
	for(;;) {
		if (thread->isFinished()) break;
		QThread::msleep(200);
	}
	network = 0;
	thread = 0;
	logger.info("Socket::stop STOP");
}

void Socket::ListenerThread::run() {
	logger.info("ListenerThread::run START");
	for(;;) {
		if (stop.loadAcquire()) break;
		int opErrno = 0;
		int ret = network->select(1, opErrno);
		if (ret == 0) continue;

		quint8 data[Buffer::MAX_SIZE];
		ret = network->receive(data, sizeof(data), opErrno);

		DatagramBuffer datagram(data, ret);

		quint16 socket = datagram.getDSocket();
		Listener* listener = map.value(socket, 0);
		if (listener) {
			// Call listener if exists.
			listener->process(&datagram);
		} else {
			logger.debug("ETHER     %012llX  %012llX  %04X", datagram.getDest(), datagram.getSource(), ((EthernetBuffer)datagram).getType());
			logger.debug("DATAGRAM  %04X  %04X %02X %s  %08X-%012llX-%s  %08X-%012llX-%s",
				datagram.getChecksum(), datagram.getLength(), datagram.getHop(), Datagram::getTypeName(datagram.getType()),
				datagram.getDNetwork(), datagram.getDHost(), Datagram::getSocketName(datagram.getDSocket()),
				datagram.getSNetwork(), datagram.getSHost(), Datagram::getSocketName(datagram.getSSocket()));
			logger.debug("----");
		}
	}
	logger.info("ListenerThread::run STOP");
}
