#include "Ethernet.h"

void Courier::serialize  (ByteBuffer* buffer, const Ethernet::Header& value) {
    Courier::serialize(buffer, value.destination);
    Courier::serialize(buffer, value.source);
    Courier::serialize(buffer, value.type);
}

void Courier::deserialize(ByteBuffer* buffer, Ethernet::Header& value) {
    Courier::deserialize(buffer, value.destination);
    Courier::deserialize(buffer, value.source);
    Courier::deserialize(buffer, value.type);
}
