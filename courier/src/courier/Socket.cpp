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

void Courier::Socket::SocketBase::callInit() {
    if (initialized) {
        logger.error("Unexpected state initialized");
        COURIER_FATAL_ERROR();
    } else {
        init();
        initialized = true;
    }
}

void Courier::Socket::SocketBase::callDestroy() {
    if (!initialized) {
        logger.error("Unexpected state initialized");
        COURIER_FATAL_ERROR();
    } else {
        destroy();
        initialized = false;
    }
}
void Courier::Socket::SocketBase::callService(Frame& request, Frame& reply, bool& sendReply) const {
    if (!initialized) {
        logger.error("Unexpected state initialized");
        COURIER_FATAL_ERROR();
    } else {
        service(request, reply, sendReply);
    }
}

void Courier::Socket::Manager::addSocket(SocketBase* socketBase) {
	Socket socket = socketBase->socket;

	if (socketMap.contains(socket)) {
		SocketBase* old = socketMap[socket];
		logger.error("Unexpected duplicate %s %d", old->name, socket);
		COURIER_FATAL_ERROR();
	} else {
		socketMap[socket] = socketBase;
	}
}
void Courier::Socket::Manager::init() {
	if (initialized) {
		logger.error("Unexpected state initialized");
		COURIER_FATAL_ERROR();
	} else {
		for(SocketBase* socketBase: socketMap.values()) {
			logger.info("init %s %s", socketBase->name, Courier::toString(socketBase->socket).toLocal8Bit().constData());
			socketBase->callInit();
		}
		initialized = true;
	}
}
void Courier::Socket::Manager::destroy() {
	if (!initialized) {
		logger.error("Unexpected state initialized");
		COURIER_FATAL_ERROR();
	} else {
		for(SocketBase* socketBase: socketMap.values()) {
			logger.info("destroy %s %s", socketBase->name, Courier::toString(socketBase->socket).toLocal8Bit().constData());
			socketBase->callDestroy();
		}
		initialized = false;
	}
}

