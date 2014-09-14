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
// SocketManager.h
//

#ifndef SOCKETMANAGER_H__
#define SOCKETMANAGER_H__

#include "Network.h"
#include "Courier.h"

class SocketManager {
public:
	class Context {
	public:
		const quint32 network;
		// network number of local network
		const quint64 host;
		// address of network interface
		Context(quint32 network_, quint64 host_) : network(network_), host(host_) {}
	};

	class Socket {
	public:
		const char* name;
		//
		Socket(const char* name_) : name(name_) {}
		virtual ~Socket() {}

		virtual void process(const Context& context, ByteBuffer& request, ByteBuffer& response) = 0;
	};

	SocketManager(Network& network_, quint48 localNetworkNumber_) : network(network_), thread(0), localNetworkNumber(localNetworkNumber_) {}

	void add   (quint16 no, Socket* socket);
	void remove(quint16 no);

	void start();
	void stop();
	int  isRunning();
private:
	class SocketThread : public QThread {
	public:
		SocketThread(SocketManager& socketManager_) : socketManager(socketManager_) {
			stop.storeRelease(0);
		}
		void run();
		void stopThread() {
			stop.storeRelease(1);
		}
	protected:
		QAtomicInt      stop;
		SocketManager&  socketManager;
	};

	QMap<quint16, Socket*> map;
	QMutex                 mutex;
	Network&               network;
	SocketThread*          thread;
	quint48                localNetworkNumber;
};
#endif
