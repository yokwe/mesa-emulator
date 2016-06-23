//
// SocketBoot.h
//

#ifndef SOCKETBOOT_H__
#define SOCKETBOOT_H__

#include "../SocketManager.h"
#include "../stub/StubDatagram.h"
#include "../stub/StubSequencedPacket.h"


class SocketBoot : public SocketManager::Socket {
public:
	SocketBoot(QString path);
	void process(Socket::Context& context, ByteBuffer& request, ByteBuffer& response);

private:
	void   *map;
	quint32 mapSize;
	quint16 nextLocalID;

	struct Connection {
		Courier::Datagram::NetworkAddress remote;
		Courier::SequencedPacket::Header  header;

		quint32 pos;
	};

	QMap<quint48, Connection*> connectionMap;
};

#endif
