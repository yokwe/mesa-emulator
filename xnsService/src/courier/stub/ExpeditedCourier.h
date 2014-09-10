#ifndef COURIER_ExpeditedCourier
#define COURIER_ExpeditedCourier

#include "../Courier.h"
#include "Protocol.h"

namespace Courier {
    namespace ExpeditedCourier {
        struct Header {
            Protocol::ProtocolRange protRange;
            Protocol::Protocol3Body body;
        };
    }
    
    void serialize  (ByteBuffer* buffer, const ExpeditedCourier::Header& value);
    void deserialize(ByteBuffer* buffer, ExpeditedCourier::Header& value);
}
#endif
