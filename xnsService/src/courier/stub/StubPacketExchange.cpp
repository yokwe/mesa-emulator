#include "../../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("StubPacketExchange");

#include "StubPacketExchange.h"

static QMap<Courier::PacketExchange::ClientType, const char*>mapClientType = {
    {Courier::PacketExchange::ClientType::UNSPECIFIED, "UNSPECIFIED"},
    {Courier::PacketExchange::ClientType::TIME, "TIME"},
    {Courier::PacketExchange::ClientType::CLEARINGHOUSE, "CLEARINGHOUSE"},
    {Courier::PacketExchange::ClientType::TELEDEBUG, "TELEDEBUG"},
};
const char* Courier::getName(PacketExchange::ClientType value) {
    return mapClientType.value(value, 0);
}
void Courier::serialize  (ByteBuffer& buffer, const Courier::PacketExchange::ClientType& value) {
    buffer.put16((quint16)value);
}
void Courier::deserialize(ByteBuffer& buffer, Courier::PacketExchange::ClientType& value) {
    value = (Courier::PacketExchange::ClientType)buffer.get16();
}

void Courier::serialize  (ByteBuffer& buffer, const PacketExchange::Header& value) {
    if (value.base == CourierData::UNITILIAZED_BASE) COURIER_ERROR()
    buffer.setPos(value.base);
    
    Courier::serialize(buffer, value.id);
    Courier::serialize(buffer, value.clientType);
}

void Courier::deserialize(ByteBuffer& buffer, PacketExchange::Header& value) {
    if (value.base != CourierData::UNITILIAZED_BASE) {
        logger.fatal("value.base = %d", value.base);
        COURIER_ERROR()
    }
    value.base = buffer.getPos();
    
    Courier::deserialize(buffer, value.id);
    Courier::deserialize(buffer, value.clientType);
}
