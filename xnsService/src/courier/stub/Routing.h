#ifndef COURIER_Routing
#define COURIER_Routing

#include "../Courier.h"
#include "Datagram.h"

namespace Courier {
    namespace Routing {
        enum class Operation : quint16 {
            REQUEST = 1,
            RESPONSE = 2,
        };
        // IGNORE predefined type.  Hop: TYPE = CARDINAL
        struct Tuple : public CourierData {
            quint32 network;
            quint16 hop;
        };
        struct Header : public CourierData {
            Operation operation;
        };
        // EXPAND Routing::Hop as CARDINAL
        const quint16 MAX_HOP = 16U;
        const quint16 TUPLE_LENGTH = 6U;
    }
    
    const char* getName(Routing::Operation value);
    void serialize  (ByteBuffer& buffer, const Routing::Operation& value);
    void deserialize(ByteBuffer& buffer, Routing::Operation& value);
    
    void serialize  (ByteBuffer& buffer, const Routing::Tuple& value);
    void deserialize(ByteBuffer& buffer, Routing::Tuple& value);
    
    void serialize  (ByteBuffer& buffer, const Routing::Header& value);
    void deserialize(ByteBuffer& buffer, Routing::Header& value);
}
#endif
