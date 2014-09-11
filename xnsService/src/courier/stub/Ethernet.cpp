#include "../../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("courierEthernet");

#include "Ethernet.h"

void Courier::serialize  (ByteBuffer& buffer, const Ethernet::Header& value) {
    if (value.base == CourierData::UNITILIAZED_BASE) COURIER_ERROR()
    buffer.setPos(value.base);
    
    Courier::serialize(buffer, value.destination);
    Courier::serialize(buffer, value.source);
    Courier::serialize(buffer, value.type);
}

void Courier::deserialize(ByteBuffer& buffer, Ethernet::Header& value) {
    if (value.base != CourierData::UNITILIAZED_BASE) {
        logger.fatal("value.base = %d", value.base);
        COURIER_ERROR()
    }
    value.base = buffer.getPos();
    
    Courier::deserialize(buffer, value.destination);
    Courier::deserialize(buffer, value.source);
    Courier::deserialize(buffer, value.type);
}
