#ifndef STUB_SequencedPacket
#define STUB_SequencedPacket

#include "../Courier.h"

namespace Courier {
    namespace SequencedPacket {
        // IGNORE predefined type.  ConnectionID: TYPE = UNSPECIFIED
        // IGNORE predefined type.  SequenceNumber: TYPE = UNSPECIFIED
        struct Header : public CourierData {
            quint16 control;
            quint16 source;
            quint16 destination;
            quint16 sequence;
            quint16 acknowledge;
            quint16 allocation;
        };
        const quint16 MASK_SYSTEM_PACKET = 0x8000;
        const quint16 MASK_SEND_ACKNOWLEDGEMENT = 0x4000;
        const quint16 MASK_ATTENTION = 0x2000;
        const quint16 MASK_END_OF_MESSAGE = 0x1000;
        const quint16 MASK_DATASTREAM_TYPE = 0xFF;
        const quint16 DATA_SST = 0x0;
        const quint16 BULK_SST = 1U;
        const quint16 CLOSE_SST = 254U;
        const quint16 CLOSE_REPLY_SST = 0xFF;
        const quint16 SST_COURIER = 0x0;
        const quint16 SST_BULK = 1U;
        const quint16 SST_CLOSE = 254U;
        const quint16 SST_CLOSE_REPLY = 0xFF;
    }
    
    void serialize  (ByteBuffer& buffer, const SequencedPacket::Header& value);
    void deserialize(ByteBuffer& buffer, SequencedPacket::Header& value);
}
#endif
