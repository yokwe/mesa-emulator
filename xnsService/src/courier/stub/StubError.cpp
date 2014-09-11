#include "../../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("StubError");

#include "StubError.h"

static QMap<Courier::Error::ErrorNumber, const char*>mapErrorNumber = {
    {Courier::Error::ErrorNumber::UNSPECIFIED, "UNSPECIFIED"},
    {Courier::Error::ErrorNumber::BAD_CHECKSUM, "BAD_CHECKSUM"},
    {Courier::Error::ErrorNumber::NO_SOCKET, "NO_SOCKET"},
    {Courier::Error::ErrorNumber::RESOURCE_LIMIT, "RESOURCE_LIMIT"},
    {Courier::Error::ErrorNumber::LISTEN_REJECT, "LISTEN_REJECT"},
    {Courier::Error::ErrorNumber::INVALID_PACKET_TYPE, "INVALID_PACKET_TYPE"},
    {Courier::Error::ErrorNumber::PROTOCOL_VIOLATION, "PROTOCOL_VIOLATION"},
    {Courier::Error::ErrorNumber::UNSPECIFIED_IN_ROUTE, "UNSPECIFIED_IN_ROUTE"},
    {Courier::Error::ErrorNumber::INCONSISTENT, "INCONSISTENT"},
    {Courier::Error::ErrorNumber::CANT_GET_THERE, "CANT_GET_THERE"},
    {Courier::Error::ErrorNumber::EXCESS_HOPS, "EXCESS_HOPS"},
    {Courier::Error::ErrorNumber::TOO_BIG, "TOO_BIG"},
    {Courier::Error::ErrorNumber::CONGESTION_WARNING, "CONGESTION_WARNING"},
    {Courier::Error::ErrorNumber::CONGESTION_DISCARD, "CONGESTION_DISCARD"},
};
const char* Courier::getName(Error::ErrorNumber value) {
    return mapErrorNumber.value(value, 0);
}
void Courier::serialize  (ByteBuffer& buffer, const Courier::Error::ErrorNumber& value) {
    buffer.put16((quint16)value);
}
void Courier::deserialize(ByteBuffer& buffer, Courier::Error::ErrorNumber& value) {
    value = (Courier::Error::ErrorNumber)buffer.get16();
}

void Courier::serialize  (ByteBuffer& buffer, const Error::Header& value) {
    if (value.base == CourierData::UNITILIAZED_BASE) COURIER_ERROR()
    buffer.setPos(value.base);
    
    Courier::serialize(buffer, value.number);
    Courier::serialize(buffer, value.parameter);
}

void Courier::deserialize(ByteBuffer& buffer, Error::Header& value) {
    if (value.base != CourierData::UNITILIAZED_BASE) {
        logger.fatal("value.base = %d", value.base);
        COURIER_ERROR()
    }
    value.base = buffer.getPos();
    
    Courier::deserialize(buffer, value.number);
    Courier::deserialize(buffer, value.parameter);
}
