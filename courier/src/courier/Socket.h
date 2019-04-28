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
// Socket.h
//

#ifndef COURIER_SOCKET_H__
#define COURIER_SOCKET_H__

#include "../courier/NIC.h"
#include "../courier/IDP.h"
#include "../courier/Block.h"

namespace Courier {
namespace Socket {

using Socket = IDP::Socket;
using Frame  = IDP::Frame;

class SocketBase {
public:
	const char*  name;
	const Socket socket;

	SocketBase(const char* name_, Socket socket_) : name(name_), socket(socket_) {}
	virtual ~SocketBase() {}

    void callInit();
    void callDestroy();
    void callService(Frame& request, Frame& response, bool& sendResponse) const;

private:
    bool initialized = false;

	// init, destroy and service is life cycle event
	virtual void init()    = 0;
	virtual void destroy() = 0;
	virtual void service(Frame& request, Frame& response, bool& sendResponse) const = 0;
};

class ServiceThread : public QRunnable {
public:
	ServiceThread(const NIC& nic_, const QMap<Socket, SocketBase*>& socketMap_) : nic(nic_), socketMap(socketMap_) {}

	void run();
	void stop() {
		needStop = true;
	}
private:
	const NIC&                       nic;
	const QMap<Socket, SocketBase*>& socketMap;

	bool needStop = false;
};

class Manager {
public:
	Manager(NIC& nic_) : nic(nic_) {}

	void addSocket(SocketBase* socketBase);

	// start service thread
	void startService();
	// stop service thread
	void stopService();

private:
	NIC& nic;

	QMap<Socket, SocketBase*> socketMap;

    bool           started       = false;
	ServiceThread* serviceThread = nullptr;
};


}
}

#endif
