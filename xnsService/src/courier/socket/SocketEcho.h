//
// SocketEcho.h
//

#ifndef SOCKETECHO_H__
#define SOCKETECHO_H__

#include "../SocketManager.h"

class SocketEcho : public SocketManager::Socket {
public:
	SocketEcho() : SocketManager::Socket("Echo") {}
	void process(const Socket::Context& context, ByteBuffer& request, ByteBuffer& response);
};

#endif
