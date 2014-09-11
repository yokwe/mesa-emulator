#ifndef STUB_Ethernet
#define STUB_Ethernet

#include "../Courier.h"

namespace Courier {
    namespace Ethernet {
        // IGNORE predefined type.  HostAddress: TYPE = UNSPECIFIED3
        // IGNORE predefined type.  FrameType: TYPE = UNSPECIFIED
        struct Header : public CourierData {
            quint48 destination;
            quint48 source;
            quint16 type;
        };
        // EXPAND Ethernet::HostAddress as UNSPECIFIED3
        const quint48 BROADCAST_ADDRESS = 0xFFFFFFFFFFFFLL;
        // EXPAND Ethernet::FrameType as UNSPECIFIED
        const quint16 FRAME_TYPE_IDP = 0x600;
        const quint16 HEADER_LENGTH = 14U;
        const quint16 MIN_DATA_LENGTH = 46U;
        const quint16 MAX_DATA_LENGTH = 1500U;
    }
    
    void serialize  (ByteBuffer& buffer, const Ethernet::Header& value);
    void deserialize(ByteBuffer& buffer, Ethernet::Header& value);
}
#endif
