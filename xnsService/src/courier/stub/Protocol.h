#ifndef COURIER_Protocol
#define COURIER_Protocol

#include "../Courier.h"

namespace Courier {
    namespace Protocol {
        enum class ProtocolType : quint16 {
            PROTOCOL0 = 0,
            PROTOCOL1 = 1,
            PROTOCOL2 = 2,
            PROTOCOL3 = 3,
        };
        enum class MessageType : quint16 {
            CALL = 0,
            REJECT = 1,
            RETURN = 2,
            ABORT = 3,
        };
        enum class RejectCode : quint16 {
            NO_SUCH_PROGRAM_NUMBER = 0,
            NO_SUCH_VERSION_NUMBER = 1,
            NO_SUCH_PROCEDURE_VALUE = 2,
            INVALID_ARGUMENTS = 3,
        };
        struct Protocol2Body : public CourierData {
            MessageType tag;
            union {
                struct {
                    quint16 transaction;
                    quint16 program;
                    quint16 version;
                    quint16 procedure;
                } CALL;
                struct {
                    quint16 transaction;
                    RejectCode reject;
                } REJECT;
                struct {
                    quint16 transaction;
                } RETURN;
                struct {
                    quint16 transaction;
                    quint16 abort;
                } ABORT;
            };
        };
        struct ProtocolRange : public CourierData {
            ProtocolType low;
            ProtocolType high;
        };
        struct VersionRange : public CourierData {
            quint16 low;
            quint16 high;
        };
        struct RejectBody : public CourierData {
            RejectCode tag;
            union {
                struct {
                } NO_SUCH_PROGRAM_NUMBER;
                struct {
                } NO_SUCH_PROCEDURE_VALUE;
                struct {
                } INVALID_ARGUMENTS;
                struct {
                    VersionRange range;
                } NO_SUCH_VERSION_NUMBER;
            };
        };
        struct Protocol3Body : public CourierData {
            MessageType tag;
            union {
                struct {
                    quint16 transaction;
                    quint32 program;
                    quint16 version;
                    quint16 procedure;
                } CALL;
                struct {
                    quint16 transaction;
                    RejectBody rejectBody;
                } REJECT;
                struct {
                    quint16 transaction;
                } RETURN;
                struct {
                    quint16 transaction;
                    quint16 abort;
                } ABORT;
            };
        };
        struct MessageObject : public CourierData {
            ProtocolType tag;
            union {
                struct {
                } PROTOCOL0;
                struct {
                } PROTOCOL1;
                struct {
                    Protocol2Body protocol2Body;
                } PROTOCOL2;
                struct {
                    Protocol3Body protocol3Body;
                } PROTOCOL3;
            };
        };
        const ProtocolType PV_LOW = ProtocolType::PROTOCOL2;
        const ProtocolType PV_HIGH = ProtocolType::PROTOCOL3;
    }
    
    const char* getName(Protocol::ProtocolType value);
    void serialize  (ByteBuffer& buffer, const Protocol::ProtocolType& value);
    void deserialize(ByteBuffer& buffer, Protocol::ProtocolType& value);
    
    const char* getName(Protocol::MessageType value);
    void serialize  (ByteBuffer& buffer, const Protocol::MessageType& value);
    void deserialize(ByteBuffer& buffer, Protocol::MessageType& value);
    
    const char* getName(Protocol::RejectCode value);
    void serialize  (ByteBuffer& buffer, const Protocol::RejectCode& value);
    void deserialize(ByteBuffer& buffer, Protocol::RejectCode& value);
    
    void serialize  (ByteBuffer& buffer, const Protocol::Protocol2Body& value);
    void deserialize(ByteBuffer& buffer, Protocol::Protocol2Body& value);
    
    void serialize  (ByteBuffer& buffer, const Protocol::ProtocolRange& value);
    void deserialize(ByteBuffer& buffer, Protocol::ProtocolRange& value);
    
    void serialize  (ByteBuffer& buffer, const Protocol::VersionRange& value);
    void deserialize(ByteBuffer& buffer, Protocol::VersionRange& value);
    
    void serialize  (ByteBuffer& buffer, const Protocol::RejectBody& value);
    void deserialize(ByteBuffer& buffer, Protocol::RejectBody& value);
    
    void serialize  (ByteBuffer& buffer, const Protocol::Protocol3Body& value);
    void deserialize(ByteBuffer& buffer, Protocol::Protocol3Body& value);
    
    void serialize  (ByteBuffer& buffer, const Protocol::MessageObject& value);
    void deserialize(ByteBuffer& buffer, Protocol::MessageObject& value);
}
#endif
