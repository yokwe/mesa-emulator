#ifndef COURIER_Datagram
#define COURIER_Datagram

#include "../Courier.h"
#include "Ethernet.h"

namespace Courier {
    namespace Datagram {
        // IGNORE predefined type.  Network: TYPE = UNSPECIFIED2
        // IGNORE reference.  Host: TYPE = Ethernet::HostAddress
        // IGNORE predefined type.  Socket: TYPE = UNSPECIFIED
        struct NetworkAddress {
            quint32 network;
            quint48 host;
            quint16 socket;
        };
        struct Header {
            quint16 checksum;
            quint16 length;
            quint16 flags;
            NetworkAddress destination;
            NetworkAddress source;
        };
        enum class PacketType : quint16 {
            ROUTING = 1,
            ECHO = 2,
            ERROR = 3,
            PACKET_EXCHANGE = 4,
            SEQUENCED_PACKET = 5,
            BOOT_SERVER_PACKET = 9,
        };
        enum class WellKnownSocket : quint16 {
            ROUTING = 1,
            ECHO = 2,
            ERROR = 3,
            ENVOY = 4,
            COURIER = 5,
            CLEARINGHOUSE_OLD = 7,
            TIME = 8,
            BOOT = 10,
            DIAG = 19,
            CLEARINGHOUSE = 20,
            AUTH = 21,
            MAIL = 22,
            NET_EXEC = 23,
            WS_INFO = 24,
            BINDING = 28,
            GERM = 35,
            TELE_DEBUG = 48,
        };
        // EXPAND Datagram::Network as UNSPECIFIED2
        const quint32 NETWORK_UNKNOWN = 0x0;
        // EXPAND Datagram::Network as UNSPECIFIED2
        const quint32 NETWORK_ALL = 0xFFFFFFFF;
        // EXPAND Datagram::Host as UNSPECIFIED3
        const quint48 HOST_ALL = 0xFFFFFFFFFFFFLL;
        const quint16 HEADER_LENGTH = 30U;
        const quint16 PACKET_TYPE_ERROR = 3U;
        const quint16 CHECKSUM_NONE = 0xFFFF;
        const quint16 MASK_HOP_COUNT = 0xFF00;
        const quint16 MASK_PACKET_TYPE = 0xFF;
        const quint16 MAX_HOP_COUNT = 16U;
        const quint16 MAX_PACKET_LIFETIME = 60U;
        const quint16 MAX_WELL_KNOWN_SOCKET = 3000U;
        const quint16 SOCKET_ROUTIG = 1U;
        const quint16 SOCKET_ECHO = 2U;
        const quint16 SOCKET_ERROR = 3U;
        const quint16 SOCKET_COURIER = 5U;
        const quint16 SOCKET_TIME = 8U;
        const quint16 SOCKET_BOOT = 10U;
        const quint16 SOCKET_CLEARINGHOUSE = 20U;
        const quint16 SOCKET_AUTH = 21U;
        const quint16 SOCKET_BINDING = 28U;
        const quint16 SOCKET_GERM = 35U;
    }
    
    const char* getName(Datagram::PacketType value);
    void serialize  (ByteBuffer* buffer, const Datagram::PacketType& value);
    void deserialize(ByteBuffer* buffer, Datagram::PacketType& value);
    
    const char* getName(Datagram::WellKnownSocket value);
    void serialize  (ByteBuffer* buffer, const Datagram::WellKnownSocket& value);
    void deserialize(ByteBuffer* buffer, Datagram::WellKnownSocket& value);
    
    void serialize  (ByteBuffer* buffer, const Datagram::NetworkAddress& value);
    void deserialize(ByteBuffer* buffer, Datagram::NetworkAddress& value);
    
    void serialize  (ByteBuffer* buffer, const Datagram::Header& value);
    void deserialize(ByteBuffer* buffer, Datagram::Header& value);
}
#endif
