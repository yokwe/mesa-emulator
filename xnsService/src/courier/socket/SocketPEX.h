//
// SocketPEX.h
//

#ifndef SOCKETPEX_H__
#define SOCKETPEX_H__

#include "../SocketManager.h"
#include "../stub/StubPacketExchange.h"

class SocketPEX : public SocketManager::Socket {
public:
	class PEX {
	public:
		class Context : public SocketManager::Socket::Context {
		public:
			Courier::PacketExchange::Header reqPEX;
			Courier::PacketExchange::Header resPEX;

			Context(const SocketManager::Socket::Context& socketContext_) : SocketManager::Socket::Context(socketContext_) {}
		};

		const char* name;
		//
		PEX(const char* name_) : name(name_) {}
		virtual ~PEX() {}

		virtual void process(Context& context, ByteBuffer& request, ByteBuffer& response) = 0;
	};

	SocketPEX(Courier::PacketExchange::ClientType clientType_, PEX* pex_) :
		SocketManager::Socket("PEX"), clientType(clientType_), pex(pex_) {}
	void process(Socket::Context& context, ByteBuffer& request, ByteBuffer& response);

protected:
	const Courier::PacketExchange::ClientType clientType;
	PEX*                                      pex;
};

#endif
