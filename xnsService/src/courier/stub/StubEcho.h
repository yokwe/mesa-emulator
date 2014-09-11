#ifndef STUB_Echo
#define STUB_Echo

#include "../Courier.h"

namespace Courier {
    namespace Echo {
        enum class Operation : quint16 {
            REQUEST = 1,
            RESPONSE = 2,
        };
        struct Header : public CourierData {
            Operation operation;
        };
    }
    
    const char* getName(Echo::Operation value);
    void serialize  (ByteBuffer& buffer, const Echo::Operation& value);
    void deserialize(ByteBuffer& buffer, Echo::Operation& value);
    
    void serialize  (ByteBuffer& buffer, const Echo::Header& value);
    void deserialize(ByteBuffer& buffer, Echo::Header& value);
}
#endif
