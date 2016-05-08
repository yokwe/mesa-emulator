#include "../../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("StubBoot");

#include "StubBoot.h"

static QMap<Courier::Boot::EtherBootPacketType, const char*>mapEtherBootPacketType = {
    {Courier::Boot::EtherBootPacketType::SIMPLE_REQUEST, "SIMPLE_REQUEST"},
    {Courier::Boot::EtherBootPacketType::SIMPLE_DATA, "SIMPLE_DATA"},
    {Courier::Boot::EtherBootPacketType::SPP_REQUEST, "SPP_REQUEST"},
};
const char* Courier::getName(Boot::EtherBootPacketType value) {
    return mapEtherBootPacketType.value(value, 0);
}
void Courier::serialize  (ByteBuffer& buffer, const Courier::Boot::EtherBootPacketType& value) {
    buffer.put16((quint16)value);
}
void Courier::deserialize(ByteBuffer& buffer, Courier::Boot::EtherBootPacketType& value) {
    value = (Courier::Boot::EtherBootPacketType)buffer.get16();
}

void Courier::serialize  (ByteBuffer& buffer, const Boot::BootFileRequest& value) {
    if (value.base == CourierData::UNITILIAZED_BASE) COURIER_ERROR()
    buffer.setPos(value.base);
    
    Courier::serialize(buffer, value.tag);
    switch(value.tag) {
    case Boot::EtherBootPacketType::SIMPLE_REQUEST:
        Courier::serialize(buffer, value.SIMPLE_REQUEST.bootFileNumber);
        break;
    case Boot::EtherBootPacketType::SIMPLE_DATA:
        Courier::serialize(buffer, value.SIMPLE_DATA.bootFileNumber);
        Courier::serialize(buffer, value.SIMPLE_DATA.packetNumber);
        break;
    case Boot::EtherBootPacketType::SPP_REQUEST:
        Courier::serialize(buffer, value.SPP_REQUEST.bootFileNumber);
        Courier::serialize(buffer, value.SPP_REQUEST.connectionID);
        break;
    }
}

void Courier::deserialize(ByteBuffer& buffer, Boot::BootFileRequest& value) {
    if (value.base != CourierData::UNITILIAZED_BASE) {
        logger.fatal("value.base = %d", value.base);
        COURIER_ERROR()
    }
    value.base = buffer.getPos();
    
    Courier::deserialize(buffer, value.tag);
    switch(value.tag) {
    case Boot::EtherBootPacketType::SIMPLE_REQUEST:
        Courier::deserialize(buffer, value.SIMPLE_REQUEST.bootFileNumber);
        break;
    case Boot::EtherBootPacketType::SIMPLE_DATA:
        Courier::deserialize(buffer, value.SIMPLE_DATA.bootFileNumber);
        Courier::deserialize(buffer, value.SIMPLE_DATA.packetNumber);
        break;
    case Boot::EtherBootPacketType::SPP_REQUEST:
        Courier::deserialize(buffer, value.SPP_REQUEST.bootFileNumber);
        Courier::deserialize(buffer, value.SPP_REQUEST.connectionID);
        break;
    }
}
