#include "../../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("courierExpeditedCourier");

#include "ExpeditedCourier.h"

void Courier::serialize  (ByteBuffer& buffer, const ExpeditedCourier::Header& value) {
    if (value.base == CourierData::UNITILIAZED_BASE) COURIER_ERROR()
    buffer.setPos(value.base);
    
    Courier::serialize(buffer, value.protRange);
    Courier::serialize(buffer, value.body);
}

void Courier::deserialize(ByteBuffer& buffer, ExpeditedCourier::Header& value) {
    if (value.base != CourierData::UNITILIAZED_BASE) {
        logger.fatal("value.base = %d", value.base);
        COURIER_ERROR()
    }
    value.base = buffer.getPos();
    
    Courier::deserialize(buffer, value.protRange);
    Courier::deserialize(buffer, value.body);
}
