//
// SocketPEX.h
//

#ifndef SOCKETPEX_H__
#define SOCKETPEX_H__

#include "../SocketManager.h"
#include "../stub/StubPacketExchange.h"

class PEX {
public:
	class Context {
	public:
		const SocketManager::Context& socketContext;
		Courier::PacketExchange::Header reqPEX;
		Courier::PacketExchange::Header resPEX;

		Context(const SocketManager::Context& socketContext_) : socketContext(socketContext_) {}
	};

	const char* name;
	//
	PEX(const char* name_) : name(name_) {}
	virtual ~PEX() {}

	void process(const Context& context, ByteBuffer& request, ByteBuffer& response);
};

class SocketPEX : public SocketManager::Socket {
public:
	SocketPEX(Courier::PacketExchange::ClientType clientType_, PEX* pex) : SocketManager::Socket("PEX"), clientType(clientType_) {}
	void process(const SocketManager::Context& context, ByteBuffer& request, ByteBuffer& response);

protected:
	const Courier::PacketExchange::ClientType clientType;
};

#endif
