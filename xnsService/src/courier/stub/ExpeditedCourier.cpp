#include "ExpeditedCourier.h"

void Courier::serialize  (ByteBuffer* buffer, const ExpeditedCourier::Header& value) {
    Courier::serialize(buffer, value.protRange);
    Courier::serialize(buffer, value.body);
}

void Courier::deserialize(ByteBuffer* buffer, ExpeditedCourier::Header& value) {
    Courier::deserialize(buffer, value.protRange);
    Courier::deserialize(buffer, value.body);
}
