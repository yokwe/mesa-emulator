#ifndef STUB_ExpeditedCourier
#define STUB_ExpeditedCourier

#include "../Courier.h"
#include "StubProtocol.h"

namespace Courier {
    namespace ExpeditedCourier {
        struct Header : public CourierData {
            Protocol::ProtocolRange protRange;
            Protocol::Protocol3Body body;
        };
    }
    
    void serialize  (ByteBuffer& buffer, const ExpeditedCourier::Header& value);
    void deserialize(ByteBuffer& buffer, ExpeditedCourier::Header& value);
}
#endif
