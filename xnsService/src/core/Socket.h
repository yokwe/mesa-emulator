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
// Socket.h
//

#ifndef SOCKET_H__
#define SOCKET_H__

#include "Network.h"
#include "Buffer.h"
#include "Datagram.h"

class Socket {
public:
	class Listener {
	public:
		const char* name;
		//
		Listener(const char* name_) : name(name_) {}
		virtual ~Listener() {}

		virtual void process(DatagramBuffer* request, DatagramBuffer* response) = 0;
	};

	static void regist  (quint16 socket, Listener* listenr);
	static void unregist(quint16 socket);

	static void start(Network* network_);
	static void stop();
	static int  isRunning() {
		return thread != 0;
	}

	void transmit(DatagramBuffer* datagram);
private:
	class ListenerThread : public QThread {
	public:
		ListenerThread() {
			stop.storeRelease(0);
		}
		void run();
		void stopThread() {
			stop.storeRelease(1);
		}
	private:
		QAtomicInt stop;
	};

	static QMap<quint16, Listener*> map;
	static Network*                 network;
	static ListenerThread*          thread;
	static QMutex                   mutex;
};
#endif
