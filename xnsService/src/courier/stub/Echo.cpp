#include "../../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("courierEcho");

#include "Echo.h"

static QMap<Courier::Echo::Operation, const char*>mapOperation = {
    {Courier::Echo::Operation::REQUEST, "REQUEST"},
    {Courier::Echo::Operation::RESPONSE, "RESPONSE"},
};
const char* Courier::getName(Echo::Operation value) {
    return mapOperation.value(value, 0);
}
void serialize  (ByteBuffer& buffer, const Courier::Echo::Operation& value) {
    buffer.put16((quint16)value);
}
void deserialize(ByteBuffer& buffer, Courier::Echo::Operation& value) {
    value = (Courier::Echo::Operation)buffer.get16();
}

void Courier::serialize  (ByteBuffer& buffer, const Echo::Header& value) {
    if (value.base == CourierData::UNITILIAZED_BASE) COURIER_ERROR()
    buffer.setPos(value.base);
    
    Courier::serialize(buffer, value.operation);
}

void Courier::deserialize(ByteBuffer& buffer, Echo::Header& value) {
    if (value.base != CourierData::UNITILIAZED_BASE) {
        logger.fatal("value.base = %d", value.base);
        COURIER_ERROR()
    }
    value.base = buffer.getPos();
    
    Courier::deserialize(buffer, value.operation);
}
