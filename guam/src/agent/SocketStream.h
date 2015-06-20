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
// SocketStream.h
//

#ifndef SOCKET_STREAM_H__
#define SOCKET_STREAM_H__

#include "../mesa/MesaBasic.h"
#include "../mesa/Pilot.h"


#include <QtCore>

class SocketStream {
public:
	static const quint32 INADDR_ANY = 0;

	// Assume, we are using IPv4
	class SockAddress {
	public:
		quint32 addr;
		quint16 port;
	};

	SocketStream();
	SocketStream(const SocketStream& that) {
		fd        = that.fd;
		lastErrno = that.lastErrno;
	}
	bool isValid() {
		return fd != 0;
	}
	int getLastErrno() {
		return lastErrno;
	}

	void connect(SockAddress remoteAddress);
	void close();

	int  bind  (SockAddress localAddress);
	int  listen(int backlog);
	SocketStream accept(SockAddress& remote);

	int  read  (char* buf, int count);
	int  write (char* buf, int count);

private:
	int        fd;
	int        lastErrno;
};

#endif
