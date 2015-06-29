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
// StreamTCP.h
//

#ifndef STREAMTCP_H__
#define STREAMTCP_H__

#include "AgentStream.h"

#include "SocketStream.h"

class StreamTCP : public AgentStream::Handler {
public:
	// MsgId: TYPE = MACHINE DEPENDENT {connect(0), listen(1), put(2), get(3), close(4), setWaitTime(5), endStream(6), shutDown(7), reset(8)};
	static const CARD32 MSG_ID_OFFSET = 1000;
	enum class MsgID : CARD32 {
		connect     = MSG_ID_OFFSET + 0,
		listen      = MSG_ID_OFFSET + 1,
		put         = MSG_ID_OFFSET + 2,
		get         = MSG_ID_OFFSET + 3,
		close       = MSG_ID_OFFSET + 4,
		setWaitTime = MSG_ID_OFFSET + 5,
		endStream   = MSG_ID_OFFSET + 6,
		shutDown    = MSG_ID_OFFSET + 7,
		reset       = MSG_ID_OFFSET + 8,
	};
	static const char* getMsgIDString(MsgID msgId);

	// Status: TYPE = MACHINE DEPENDENT {success(0), failure(1)};
	static const CARD16 STATUS_OFFSET = 0;
	enum Status : CARD32 {
		success = STATUS_OFFSET + 0,
		failure = STATUS_OFFSET + 1,
	};
	const char* getStatusString(Status status);

	// WSEReason: TYPE = MACHINE DEPENDENT {WSAEINTR(4), WSAEBADF(9), WSAEACCES(13), WSAEFAULT(14), WSAEINVAL(22), WSAEMFILE(24), WSAEWOULDBLOCK(35), WSAEINPROGRESS(36), WSAEALREADY(37), WSAENOTSOCK(38), WSAEDESTADDRREQ(39), WSAEMSGSIZE(40), WSAEPROTOTYPE(41), WSAENOPROTOOPT(42), WSAEPROTONOSUPPORT(43), WSAESOCKTNOSUPPORT(44), WSAEOPNOTSUPP(45), WSAEPFNOSUPPORT(46), WSAEAFNOSUPPORT(47), WSAEADDRINUSE(48), WSAEADDRNOTAVAIL(49), WSAENETDOWN(50), WSAENETUNREACH(51), WSAENETRESET(52), WSAECONNABORTED(53), WSAECONNRESET(54), WSAENOBUFS(55), WSAEISCONN(56), WSAENOTCONN(57), WSAESHUTDOWN(58), WSAETOOMANYREFS(59), WSAETIMEDOUT(60), WSAECONNREFUSED(61), WSAELOOP(62), WSAENAMETOOLONG(63), WSAEHOSTDOWN(64), WSAEHOSTUNREACH(65), WSAENOTEMPTY(66), WSAEPROCLIM(67), WSAEUSERS(68), WSAEDQUOT(69), WSAESTALE(70), WSAEREMOTE(71), WSASYSNOTREADY(91), WSAVERNOTSUPPORTED(92), WSANOTINITIALISED(93)};
	static const CARD16 WSEREASON_OFFSET = 10000;
	enum WSEReason {
		WSAEINTR           = WSEREASON_OFFSET + 4,
		WSAEBADF           = WSEREASON_OFFSET + 9,
		WSAEACCES          = WSEREASON_OFFSET + 13,
		WSAEFAULT          = WSEREASON_OFFSET + 14,
		WSAEINVAL          = WSEREASON_OFFSET + 22,
		WSAEMFILE          = WSEREASON_OFFSET + 24,
		WSAEWOULDBLOCK     = WSEREASON_OFFSET + 35,
		WSAEINPROGRESS     = WSEREASON_OFFSET + 36,
		WSAEALREADY        = WSEREASON_OFFSET + 37,
		WSAENOTSOCK        = WSEREASON_OFFSET + 38,
		WSAEDESTADDRREQ    = WSEREASON_OFFSET + 39,
		WSAEMSGSIZE        = WSEREASON_OFFSET + 40,
		WSAEPROTOTYPE      = WSEREASON_OFFSET + 41,
		WSAENOPROTOOPT     = WSEREASON_OFFSET + 42,
		WSAEPROTONOSUPPORT = WSEREASON_OFFSET + 43,
		WSAESOCKTNOSUPPORT = WSEREASON_OFFSET + 44,
		WSAEOPNOTSUPP      = WSEREASON_OFFSET + 45,
		WSAEPFNOSUPPORT    = WSEREASON_OFFSET + 46,
		WSAEAFNOSUPPORT    = WSEREASON_OFFSET + 47,
		WSAEADDRINUSE      = WSEREASON_OFFSET + 48,
		WSAEADDRNOTAVAIL   = WSEREASON_OFFSET + 49,
		WSAENETDOWN        = WSEREASON_OFFSET + 50,
		WSAENETUNREACH     = WSEREASON_OFFSET + 51,
		WSAENETRESET       = WSEREASON_OFFSET + 52,
		WSAECONNABORTED    = WSEREASON_OFFSET + 53,
		WSAECONNRESET      = WSEREASON_OFFSET + 54,
		WSAENOBUFS         = WSEREASON_OFFSET + 55,
		WSAEISCONN         = WSEREASON_OFFSET + 56,
		WSAENOTCONN        = WSEREASON_OFFSET + 57,
		WSAESHUTDOWN       = WSEREASON_OFFSET + 58,
		WSAETOOMANYREFS    = WSEREASON_OFFSET + 59,
		WSAETIMEDOUT       = WSEREASON_OFFSET + 60,
		WSAECONNREFUSED    = WSEREASON_OFFSET + 61,
		WSAELOOP           = WSEREASON_OFFSET + 62,
		WSAENAMETOOLONG    = WSEREASON_OFFSET + 63,
		WSAEHOSTDOWN       = WSEREASON_OFFSET + 64,
		WSAEHOSTUNREACH    = WSEREASON_OFFSET + 65,
		WSAENOTEMPTY       = WSEREASON_OFFSET + 66,
		WSAEPROCLIM        = WSEREASON_OFFSET + 67,
		WSAEUSERS          = WSEREASON_OFFSET + 68,
		WSAEDQUOT          = WSEREASON_OFFSET + 69,
		WSAESTALE          = WSEREASON_OFFSET + 70,
		WSAEREMOTE         = WSEREASON_OFFSET + 71,
		WSASYSNOTREADY     = WSEREASON_OFFSET + 91,
		WSAVERNOTSUPPORTED = WSEREASON_OFFSET + 92,
		WSANOTINITIALISED  = WSEREASON_OFFSET + 93,
	};

	// State: TYPE = {closed, established, finWait, closing};
	enum State : CARD32 {
		closed = 0, established = 1, finWait = 2, closing = 3,
	};
	const char* getStateString(State state);


	//
	// SocketInfo
	//
	class SocketInfo {
	private:
		static CARD32 socketIDNext;

	public:
		const CARD32 socketID;

		CARD32 remoteAddress;
		CARD16 remotePort;
		CARD32 localAddress;
		CARD16 localPort;
		CARD32 timeout;

		SocketStream socket;;

		SocketInfo() : socketID(++socketIDNext) {
			localAddress = remoteAddress = localPort = remotePort = 0;
			timeout = 0;

			addSocket(this);
		}
	};

	static void addSocket(SocketInfo* socketInfo) {
		socketMap.insert(socketInfo->socketID, socketInfo);
	}
	static void removeSocket(SocketInfo* socketInfo);
	static SocketInfo* getSocket(CARD32 socketID);

	//
	// Task
	//
	class StreamTask: public AgentStream::Task {
	public:
		StreamTask() : AgentStream::Task() {}
	};

	AgentStream::Task* createTask() {
		return new StreamTask();
	}

	//
	// AgentStream::Handler
	//

	StreamTCP();

	void idle   (AgentStream::FCB* fcb);

	// Return value of methods below is CoProcessorIOFaceGuam::R_*
	//   R_completed, R_inProgress, R_error

	AgentStream::Result accept (AgentStream::IOCB* iocb, AgentStream::Task* task);
	AgentStream::Result connect(AgentStream::IOCB* iocb, AgentStream::Task* task);
	// To avoid conflict with keyword "delete", use "destroy" instead.
	AgentStream::Result destroy(AgentStream::IOCB* iocb, AgentStream::Task* task);

	void run();

	// Command described in MsgID

	void connect    (const CARD32 arg1, const CARD32 arg2, const CARD32 arg3);
	void get        (const CARD32 arg1, const CARD32 arg2, const CARD32 arg3);


	void listen     (const CARD32 arg1, const CARD32 arg2, const CARD32 arg3);
	void put        (const CARD32 arg1, const CARD32 arg2, const CARD32 arg3);
	void close      (const CARD32 arg1, const CARD32 arg2, const CARD32 arg3);
	void setWaitTime(const CARD32 arg1, const CARD32 arg2, const CARD32 arg3);
	void endStream  (const CARD32 arg1, const CARD32 arg2, const CARD32 arg3);
	void shutDown   (const CARD32 arg1, const CARD32 arg2, const CARD32 arg3);
	void reset      (const CARD32 arg1, const CARD32 arg2, const CARD32 arg3);

private:
	static QMap<CARD32, SocketInfo*> socketMap;
};

#endif
