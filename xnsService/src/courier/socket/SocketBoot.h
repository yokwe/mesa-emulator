//
// SocketBoot.h
//

#ifndef SOCKETBOOT_H__
#define SOCKETBOOT_H__

#include "../SocketManager.h"

class SocketBoot : public SocketManager::Socket {
public:
	SocketBoot() : SocketManager::Socket("Boot") {}
	void process(const Socket::Context& context, ByteBuffer& request, ByteBuffer& response);
};

#endif
