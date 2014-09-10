#ifndef COURIER_Error
#define COURIER_Error

#include "../Courier.h"

namespace Courier {
    namespace Error {
        enum class ErrorNumber : quint16 {
            UNSPECIFIED = 0,
            BAD_CHECKSUM = 1,
            NO_SOCKET = 2,
            RESOURCE_LIMIT = 3,
            LISTEN_REJECT = 4,
            INVALID_PACKET_TYPE = 5,
            PROTOCOL_VIOLATION = 6,
            UNSPECIFIED_IN_ROUTE = 512,
            INCONSISTENT = 513,
            CANT_GET_THERE = 514,
            EXCESS_HOPS = 515,
            TOO_BIG = 516,
            CONGESTION_WARNING = 517,
            CONGESTION_DISCARD = 518,
        };
        struct Header {
            ErrorNumber number;
            quint16 parameter;
        };
        const quint16 HEADER_LENGTH = 4U;
        const quint16 MIN_DATA_LENGTH = 42U;
    }
    
    const char* getName(Error::ErrorNumber value);
    void serialize  (ByteBuffer* buffer, const Error::ErrorNumber& value);
    void deserialize(ByteBuffer* buffer, Error::ErrorNumber& value);
    
    void serialize  (ByteBuffer* buffer, const Error::Header& value);
    void deserialize(ByteBuffer* buffer, Error::Header& value);
}
#endif
