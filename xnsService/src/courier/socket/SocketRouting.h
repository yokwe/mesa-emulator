//
// SocketRouting.h
//

#ifndef SOCKETROUTING_H__
#define SOCKETROUTING_H__

#include "../SocketManager.h"

class SocketRouting : public SocketManager::Socket {
public:
	SocketRouting() : SocketManager::Socket("Routing") {}

	void process(ByteBuffer& request, ByteBuffer& response);
};

#endif
