#include "../../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("StubRouting");

#include "StubRouting.h"

static QMap<Courier::Routing::Operation, const char*>mapOperation = {
    {Courier::Routing::Operation::REQUEST, "REQUEST"},
    {Courier::Routing::Operation::RESPONSE, "RESPONSE"},
};
const char* Courier::getName(Routing::Operation value) {
    return mapOperation.value(value, 0);
}
void Courier::serialize  (ByteBuffer& buffer, const Courier::Routing::Operation& value) {
    buffer.put16((quint16)value);
}
void Courier::deserialize(ByteBuffer& buffer, Courier::Routing::Operation& value) {
    value = (Courier::Routing::Operation)buffer.get16();
}

void Courier::serialize  (ByteBuffer& buffer, const Routing::Tuple& value) {
    if (value.base == CourierData::UNITILIAZED_BASE) COURIER_ERROR()
    buffer.setPos(value.base);
    
    Courier::serialize(buffer, value.network);
    Courier::serialize(buffer, value.hop);
}

void Courier::deserialize(ByteBuffer& buffer, Routing::Tuple& value) {
    if (value.base != CourierData::UNITILIAZED_BASE) {
        logger.fatal("value.base = %d", value.base);
        COURIER_ERROR()
    }
    value.base = buffer.getPos();
    
    Courier::deserialize(buffer, value.network);
    Courier::deserialize(buffer, value.hop);
}

void Courier::serialize  (ByteBuffer& buffer, const Routing::Header& value) {
    if (value.base == CourierData::UNITILIAZED_BASE) COURIER_ERROR()
    buffer.setPos(value.base);
    
    Courier::serialize(buffer, value.operation);
}

void Courier::deserialize(ByteBuffer& buffer, Routing::Header& value) {
    if (value.base != CourierData::UNITILIAZED_BASE) {
        logger.fatal("value.base = %d", value.base);
        COURIER_ERROR()
    }
    value.base = buffer.getPos();
    
    Courier::deserialize(buffer, value.operation);
}
