//
// SocketRouting.h
//

#ifndef SOCKETROUTING_H__
#define SOCKETROUTING_H__

#include "../SocketManager.h"

class SocketRouting : public SocketManager::Socket {
public:
	SocketRouting(quint32 localNetworkNumber_) : SocketManager::Socket("Routing"), localNetworkNumber(localNetworkNumber_) {
		addNetwork(localNetworkNumber, 0);
	}
	void addNetwork(quint32 networkNumber, quint16 hop);
	void process(const SocketManager::Context& context, ByteBuffer& request, ByteBuffer& response);

protected:
	const quint32 localNetworkNumber;
	QMap<quint32, quint16>networkMap;
};

#endif
