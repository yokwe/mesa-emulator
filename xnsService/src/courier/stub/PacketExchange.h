#ifndef COURIER_PacketExchange
#define COURIER_PacketExchange

#include "../Courier.h"

namespace Courier {
    namespace PacketExchange {
        enum class ClientType : quint16 {
            UNSPECIFIED = 0,
            TIME = 1,
            CLEARINGHOUSE = 2,
            TELEDEBUG = 8,
        };
        struct Header {
            quint32 id;
            ClientType clientType;
        };
    }
    
    const char* getName(PacketExchange::ClientType value);
    void serialize  (ByteBuffer* buffer, const PacketExchange::ClientType& value);
    void deserialize(ByteBuffer* buffer, PacketExchange::ClientType& value);
    
    void serialize  (ByteBuffer* buffer, const PacketExchange::Header& value);
    void deserialize(ByteBuffer* buffer, PacketExchange::Header& value);
}
#endif
