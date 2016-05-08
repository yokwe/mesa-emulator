#ifndef STUB_Boot
#define STUB_Boot

#include "../Courier.h"
#include "StubDatagram.h"
#include "StubSequencedPacket.h"

namespace Courier {
    namespace Boot {
        enum class EtherBootPacketType : quint16 {
            SIMPLE_REQUEST = 1,
            SIMPLE_DATA = 2,
            SPP_REQUEST = 3,
        };
        // IGNORE reference.  EthernetBootFileNumber: TYPE = Datagram::Host
        struct BootFileRequest : public CourierData {
            EtherBootPacketType tag;
            union {
                struct {
                    quint48 bootFileNumber;
                } SIMPLE_REQUEST;
                struct {
                    quint48 bootFileNumber;
                    quint16 packetNumber;
                } SIMPLE_DATA;
                struct {
                    quint48 bootFileNumber;
                    quint16 connectionID;
                } SPP_REQUEST;
            };
        };
    }
    
    const char* getName(Boot::EtherBootPacketType value);
    void serialize  (ByteBuffer& buffer, const Boot::EtherBootPacketType& value);
    void deserialize(ByteBuffer& buffer, Boot::EtherBootPacketType& value);
    
    void serialize  (ByteBuffer& buffer, const Boot::BootFileRequest& value);
    void deserialize(ByteBuffer& buffer, Boot::BootFileRequest& value);
}
#endif
