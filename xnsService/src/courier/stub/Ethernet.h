#ifndef COURIER_Ethernet
#define COURIER_Ethernet

#include "../Courier.h"

namespace Courier {
    namespace Ethernet {
        // IGNORE predefined type.  HostAddress: TYPE = UNSPECIFIED3
        // IGNORE predefined type.  FrameType: TYPE = UNSPECIFIED
        struct Header {
            quint48 destination;
            quint48 source;
            quint16 type;
        };
        // EXPAND Ethernet::HostAddress as UNSPECIFIED3
        static const quint64 BROADCAST_ADDRESS = 0xFFFFFFFFFFFFLL;
        // EXPAND Ethernet::FrameType as UNSPECIFIED
        static const quint16 FRAME_TYPE_IDP = 0x600;
        static const quint16 HEADER_LENGTH = 14U;
        static const quint16 MIN_DATA_LENGTH = 46U;
        static const quint16 MAX_DATA_LENGTH = 1500U;
    }
    
    void serialize  (ByteBuffer* buffer, const Ethernet::Header& value);
    void deserialize(ByteBuffer* buffer, Ethernet::Header& value);
}
#endif
