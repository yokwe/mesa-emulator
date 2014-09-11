#include "../../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("courierTime2");

#include "Time2.h"

static QMap<Courier::Time2::PacketType, const char*>mapPacketType = {
    {Courier::Time2::PacketType::REQUEST, "REQUEST"},
    {Courier::Time2::PacketType::RESPONSE, "RESPONSE"},
};
const char* Courier::getName(Time2::PacketType value) {
    return mapPacketType.value(value, 0);
}
void serialize  (ByteBuffer& buffer, const Courier::Time2::PacketType& value) {
    buffer.put16((quint16)value);
}
void deserialize(ByteBuffer& buffer, Courier::Time2::PacketType& value) {
    value = (Courier::Time2::PacketType)buffer.get16();
}

static QMap<Courier::Time2::OffsetDirection, const char*>mapOffsetDirection = {
    {Courier::Time2::OffsetDirection::WEST, "WEST"},
    {Courier::Time2::OffsetDirection::EAST, "EAST"},
};
const char* Courier::getName(Time2::OffsetDirection value) {
    return mapOffsetDirection.value(value, 0);
}
void serialize  (ByteBuffer& buffer, const Courier::Time2::OffsetDirection& value) {
    buffer.put16((quint16)value);
}
void deserialize(ByteBuffer& buffer, Courier::Time2::OffsetDirection& value) {
    value = (Courier::Time2::OffsetDirection)buffer.get16();
}

static QMap<Courier::Time2::ToleranceType, const char*>mapToleranceType = {
    {Courier::Time2::ToleranceType::UNKNOWN, "UNKNOWN"},
    {Courier::Time2::ToleranceType::IN_MILLI_SECONDS, "IN_MILLI_SECONDS"},
};
const char* Courier::getName(Time2::ToleranceType value) {
    return mapToleranceType.value(value, 0);
}
void serialize  (ByteBuffer& buffer, const Courier::Time2::ToleranceType& value) {
    buffer.put16((quint16)value);
}
void deserialize(ByteBuffer& buffer, Courier::Time2::ToleranceType& value) {
    value = (Courier::Time2::ToleranceType)buffer.get16();
}

void Courier::serialize  (ByteBuffer& buffer, const Time2::PacketData& value) {
    if (value.base == CourierData::UNITILIAZED_BASE) COURIER_ERROR()
    buffer.setPos(value.base);
    
    Courier::serialize(buffer, value.tag);
    switch(value.tag) {
    case Time2::PacketType::REQUEST:
        break;
    case Time2::PacketType::RESPONSE:
        Courier::serialize(buffer, value.RESPONSE.currentTime);
        Courier::serialize(buffer, value.RESPONSE.offsetDirection);
        Courier::serialize(buffer, value.RESPONSE.offsetHours);
        Courier::serialize(buffer, value.RESPONSE.offsetMinutes);
        Courier::serialize(buffer, value.RESPONSE.startOfDST);
        Courier::serialize(buffer, value.RESPONSE.endOfDST);
        Courier::serialize(buffer, value.RESPONSE.toleranceType);
        Courier::serialize(buffer, value.RESPONSE.tolerance);
        break;
    }
}

void Courier::deserialize(ByteBuffer& buffer, Time2::PacketData& value) {
    Courier::deserialize(buffer, value.tag);
    switch(value.tag) {
    case Time2::PacketType::REQUEST:
        break;
    case Time2::PacketType::RESPONSE:
        Courier::deserialize(buffer, value.RESPONSE.currentTime);
        Courier::deserialize(buffer, value.RESPONSE.offsetDirection);
        Courier::deserialize(buffer, value.RESPONSE.offsetHours);
        Courier::deserialize(buffer, value.RESPONSE.offsetMinutes);
        Courier::deserialize(buffer, value.RESPONSE.startOfDST);
        Courier::deserialize(buffer, value.RESPONSE.endOfDST);
        Courier::deserialize(buffer, value.RESPONSE.toleranceType);
        Courier::deserialize(buffer, value.RESPONSE.tolerance);
        break;
    }
}

void Courier::serialize  (ByteBuffer& buffer, const Time2::Packet& value) {
    if (value.base == CourierData::UNITILIAZED_BASE) COURIER_ERROR()
    buffer.setPos(value.base);
    
    Courier::serialize(buffer, value.version);
    Courier::serialize(buffer, value.data);
}

void Courier::deserialize(ByteBuffer& buffer, Time2::Packet& value) {
    if (value.base != CourierData::UNITILIAZED_BASE) {
        logger.fatal("value.base = %d", value.base);
        COURIER_ERROR()
    }
    value.base = buffer.getPos();
    
    Courier::deserialize(buffer, value.version);
    Courier::deserialize(buffer, value.data);
}
