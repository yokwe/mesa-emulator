#ifndef COURIER_Time2
#define COURIER_Time2

#include "../Courier.h"

namespace Courier {
    namespace Time2 {
        const quint32 PROGRAM_NUMBER = 15;
        const quint32 VERSION_NUMBER = 2;
        
        // IGNORE predefined type.  Time: TYPE = LONG_CARDINAL
        enum class PacketType : quint16 {
            REQUEST = 1,
            RESPONSE = 2,
        };
        enum class OffsetDirection : quint16 {
            WEST = 0,
            EAST = 1,
        };
        enum class ToleranceType : quint16 {
            UNKNOWN = 0,
            IN_MILLI_SECONDS = 1,
        };
        // IGNORE predefined type.  Version: TYPE = CARDINAL
        struct PacketData : public CourierData {
            PacketType tag;
            union {
                struct {
                } REQUEST;
                struct {
                    quint32 currentTime;
                    OffsetDirection offsetDirection;
                    quint16 offsetHours;
                    quint16 offsetMinutes;
                    quint16 startOfDST;
                    quint16 endOfDST;
                    ToleranceType toleranceType;
                    quint32 tolerance;
                } RESPONSE;
            };
        };
        struct Packet : public CourierData {
            quint16 version;
            PacketData data;
        };
        // EXPAND Time2::Time as LONG_CARDINAL
        const quint32 EARLIEST_TIME = 2114294400U;
        // EXPAND Time2::Version as CARDINAL
        const quint16 VERSION = 2U;
    }
    
    const char* getName(Time2::PacketType value);
    void serialize  (ByteBuffer& buffer, const Time2::PacketType& value);
    void deserialize(ByteBuffer& buffer, Time2::PacketType& value);
    
    const char* getName(Time2::OffsetDirection value);
    void serialize  (ByteBuffer& buffer, const Time2::OffsetDirection& value);
    void deserialize(ByteBuffer& buffer, Time2::OffsetDirection& value);
    
    const char* getName(Time2::ToleranceType value);
    void serialize  (ByteBuffer& buffer, const Time2::ToleranceType& value);
    void deserialize(ByteBuffer& buffer, Time2::ToleranceType& value);
    
    void serialize  (ByteBuffer& buffer, const Time2::PacketData& value);
    void deserialize(ByteBuffer& buffer, Time2::PacketData& value);
    
    void serialize  (ByteBuffer& buffer, const Time2::Packet& value);
    void deserialize(ByteBuffer& buffer, Time2::Packet& value);
}
#endif
