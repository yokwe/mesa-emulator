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
// SocketStream.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("socket");

#include "../util/Debug.h"

#include "../mesa/Memory.h"

#include "SocketStream.h"

#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>


SocketStream::SocketStream() {
	fd = ::socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1) {
		lastErrno = errno;
		logger.fatal("%d - %s", lastErrno, strerror(lastErrno));
		ERROR();
	}
}

void SocketStream::connect(SockAddress remoteAddress) {
	struct sockaddr_in remote;

	bzero((char *)&remote, sizeof(remote));
	remote.sin_family      = AF_INET;
	remote.sin_addr.s_addr = qToBigEndian(remoteAddress.addr);
	remote.sin_port        = qToBigEndian(remoteAddress.port);

	int result = ::connect(fd, (const struct sockaddr*)&remote, sizeof(remote));
	if (result == -1) {
		lastErrno = errno;
		logger.fatal("%d - %s", lastErrno, strerror(lastErrno));
		ERROR();
	}
}

void SocketStream::close() {
	int result = ::close(fd);
	if (result == -1) {
		lastErrno = errno;
		logger.fatal("%d - %s", lastErrno, strerror(lastErrno));
		ERROR();
	}
}

int  SocketStream::bind  (SockAddress localAddress) {
	struct sockaddr_in local;

	bzero((char *)&local, sizeof(local));
	local.sin_family      = AF_INET;
	local.sin_addr.s_addr = qToBigEndian(localAddress.addr);
	local.sin_port        = qToBigEndian(localAddress.port);

	int result = ::bind(fd, (const struct sockaddr*)&local, sizeof(local));
	if (result == -1) {
		lastErrno = errno;
		logger.fatal("%d - %s", lastErrno, strerror(lastErrno));
		ERROR();
	}
	return result;
}
int  SocketStream::listen(int backlog) {
	int result = ::listen(fd, backlog);
	if (result == -1) {
		lastErrno = errno;
		logger.fatal("%d - %s", lastErrno, strerror(lastErrno));
		ERROR();
	}
	return result;
}
SocketStream SocketStream::accept(SockAddress& remote) {
	struct sockaddr_in received;
	quint32 receivedSize = sizeof(received);
	bzero((char *)&received, sizeof(received));

	int result = ::accept(fd, (struct sockaddr*)&received, &receivedSize);
	if (result == -1) {
		lastErrno = errno;
		logger.fatal("%d - %s", lastErrno, strerror(lastErrno));
		ERROR();
	}

	SocketStream ret;
	ret.fd        = result;
	ret.lastErrno = 0;

	remote.addr = qFromBigEndian(received.sin_addr.s_addr);
	remote.port = qFromBigEndian(received.sin_port);

	return ret;
}

int  SocketStream::read  (char* buf, int count) {
	int result = ::read(fd, buf, count);
	if (result == -1) {
		lastErrno = errno;
		logger.fatal("%d - %s", lastErrno, strerror(lastErrno));
		ERROR();
	}
	return result;
}
int  SocketStream::write (char* buf, int count) {
	int result = ::write(fd, buf, count);
	if (result == -1) {
		lastErrno = errno;
		logger.fatal("%d - %s", lastErrno, strerror(lastErrno));
		ERROR();
	}
	return result;
}
