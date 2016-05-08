#include "../../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("StubSequencedPacket");

#include "StubSequencedPacket.h"

void Courier::serialize  (ByteBuffer& buffer, const SequencedPacket::Header& value) {
    if (value.base == CourierData::UNITILIAZED_BASE) COURIER_ERROR()
    buffer.setPos(value.base);
    
    Courier::serialize(buffer, value.control);
    Courier::serialize(buffer, value.source);
    Courier::serialize(buffer, value.destination);
    Courier::serialize(buffer, value.sequence);
    Courier::serialize(buffer, value.acknowledge);
    Courier::serialize(buffer, value.allocation);
}

void Courier::deserialize(ByteBuffer& buffer, SequencedPacket::Header& value) {
    if (value.base != CourierData::UNITILIAZED_BASE) {
        logger.fatal("value.base = %d", value.base);
        COURIER_ERROR()
    }
    value.base = buffer.getPos();
    
    Courier::deserialize(buffer, value.control);
    Courier::deserialize(buffer, value.source);
    Courier::deserialize(buffer, value.destination);
    Courier::deserialize(buffer, value.sequence);
    Courier::deserialize(buffer, value.acknowledge);
    Courier::deserialize(buffer, value.allocation);
}
