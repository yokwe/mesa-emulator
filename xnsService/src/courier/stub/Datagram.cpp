#include "Datagram.h"

static QMap<Courier::Datagram::PacketType, const char*>mapPacketType = {
    {Courier::Datagram::PacketType::ROUTING, "ROUTING"},
    {Courier::Datagram::PacketType::ECHO, "ECHO"},
    {Courier::Datagram::PacketType::ERROR, "ERROR"},
    {Courier::Datagram::PacketType::PACKET_EXCHANGE, "PACKET_EXCHANGE"},
    {Courier::Datagram::PacketType::SEQUENCED_PACKET, "SEQUENCED_PACKET"},
    {Courier::Datagram::PacketType::BOOT_SERVER_PACKET, "BOOT_SERVER_PACKET"},
};
const char* Courier::getName(Datagram::PacketType value) {
    return mapPacketType.value(value, 0);
}
void serialize  (ByteBuffer* buffer, const Courier::Datagram::PacketType& value) {
    buffer->put16((quint16)value);
}
void deserialize(ByteBuffer* buffer, Courier::Datagram::PacketType& value) {
    value = (Courier::Datagram::PacketType)buffer->get16();
}

static QMap<Courier::Datagram::WellKnownSocket, const char*>mapWellKnownSocket = {
    {Courier::Datagram::WellKnownSocket::ROUTING, "ROUTING"},
    {Courier::Datagram::WellKnownSocket::ECHO, "ECHO"},
    {Courier::Datagram::WellKnownSocket::ERROR, "ERROR"},
    {Courier::Datagram::WellKnownSocket::ENVOY, "ENVOY"},
    {Courier::Datagram::WellKnownSocket::COURIER, "COURIER"},
    {Courier::Datagram::WellKnownSocket::CLEARINGHOUSE_OLD, "CLEARINGHOUSE_OLD"},
    {Courier::Datagram::WellKnownSocket::TIME, "TIME"},
    {Courier::Datagram::WellKnownSocket::BOOT, "BOOT"},
    {Courier::Datagram::WellKnownSocket::DIAG, "DIAG"},
    {Courier::Datagram::WellKnownSocket::CLEARINGHOUSE, "CLEARINGHOUSE"},
    {Courier::Datagram::WellKnownSocket::AUTH, "AUTH"},
    {Courier::Datagram::WellKnownSocket::MAIL, "MAIL"},
    {Courier::Datagram::WellKnownSocket::NET_EXEC, "NET_EXEC"},
    {Courier::Datagram::WellKnownSocket::WS_INFO, "WS_INFO"},
    {Courier::Datagram::WellKnownSocket::BINDING, "BINDING"},
    {Courier::Datagram::WellKnownSocket::GERM, "GERM"},
    {Courier::Datagram::WellKnownSocket::TELE_DEBUG, "TELE_DEBUG"},
};
const char* Courier::getName(Datagram::WellKnownSocket value) {
    return mapWellKnownSocket.value(value, 0);
}
void serialize  (ByteBuffer* buffer, const Courier::Datagram::WellKnownSocket& value) {
    buffer->put16((quint16)value);
}
void deserialize(ByteBuffer* buffer, Courier::Datagram::WellKnownSocket& value) {
    value = (Courier::Datagram::WellKnownSocket)buffer->get16();
}

void Courier::serialize  (ByteBuffer* buffer, const Datagram::NetworkAddress& value) {
    Courier::serialize(buffer, value.network);
    Courier::serialize(buffer, value.host);
    Courier::serialize(buffer, value.socket);
}

void Courier::deserialize(ByteBuffer* buffer, Datagram::NetworkAddress& value) {
    Courier::deserialize(buffer, value.network);
    Courier::deserialize(buffer, value.host);
    Courier::deserialize(buffer, value.socket);
}

void Courier::serialize  (ByteBuffer* buffer, const Datagram::Header& value) {
    Courier::serialize(buffer, value.checksum);
    Courier::serialize(buffer, value.length);
    Courier::serialize(buffer, value.flags);
    Courier::serialize(buffer, value.destination);
    Courier::serialize(buffer, value.source);
}

void Courier::deserialize(ByteBuffer* buffer, Datagram::Header& value) {
    Courier::deserialize(buffer, value.checksum);
    Courier::deserialize(buffer, value.length);
    Courier::deserialize(buffer, value.flags);
    Courier::deserialize(buffer, value.destination);
    Courier::deserialize(buffer, value.source);
}
