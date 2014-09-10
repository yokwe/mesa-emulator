#include "PacketExchange.h"

static QMap<Courier::PacketExchange::ClientType, const char*>mapClientType = {
    {Courier::PacketExchange::ClientType::UNSPECIFIED, "UNSPECIFIED"},
    {Courier::PacketExchange::ClientType::TIME, "TIME"},
    {Courier::PacketExchange::ClientType::CLEARINGHOUSE, "CLEARINGHOUSE"},
    {Courier::PacketExchange::ClientType::TELEDEBUG, "TELEDEBUG"},
};
const char* Courier::getName(PacketExchange::ClientType value) {
    return mapClientType.value(value, 0);
}
void serialize  (ByteBuffer* buffer, const Courier::PacketExchange::ClientType& value) {
    buffer->put16((quint16)value);
}
void deserialize(ByteBuffer* buffer, Courier::PacketExchange::ClientType& value) {
    value = (Courier::PacketExchange::ClientType)buffer->get16();
}

void Courier::serialize  (ByteBuffer* buffer, const PacketExchange::Header& value) {
    Courier::serialize(buffer, value.id);
    Courier::serialize(buffer, value.clientType);
}

void Courier::deserialize(ByteBuffer* buffer, PacketExchange::Header& value) {
    Courier::deserialize(buffer, value.id);
    Courier::deserialize(buffer, value.clientType);
}
