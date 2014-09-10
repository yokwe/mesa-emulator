#include "Protocol.h"

static QMap<Courier::Protocol::ProtocolType, const char*>mapProtocolType = {
    {Courier::Protocol::ProtocolType::PROTOCOL0, "PROTOCOL0"},
    {Courier::Protocol::ProtocolType::PROTOCOL1, "PROTOCOL1"},
    {Courier::Protocol::ProtocolType::PROTOCOL2, "PROTOCOL2"},
    {Courier::Protocol::ProtocolType::PROTOCOL3, "PROTOCOL3"},
};
const char* Courier::getName(Protocol::ProtocolType value) {
    return mapProtocolType.value(value, 0);
}
void serialize  (ByteBuffer* buffer, const Courier::Protocol::ProtocolType& value) {
    buffer->put16((quint16)value);
}
void deserialize(ByteBuffer* buffer, Courier::Protocol::ProtocolType& value) {
    value = (Courier::Protocol::ProtocolType)buffer->get16();
}

static QMap<Courier::Protocol::MessageType, const char*>mapMessageType = {
    {Courier::Protocol::MessageType::CALL, "CALL"},
    {Courier::Protocol::MessageType::REJECT, "REJECT"},
    {Courier::Protocol::MessageType::RETURN, "RETURN"},
    {Courier::Protocol::MessageType::ABORT, "ABORT"},
};
const char* Courier::getName(Protocol::MessageType value) {
    return mapMessageType.value(value, 0);
}
void serialize  (ByteBuffer* buffer, const Courier::Protocol::MessageType& value) {
    buffer->put16((quint16)value);
}
void deserialize(ByteBuffer* buffer, Courier::Protocol::MessageType& value) {
    value = (Courier::Protocol::MessageType)buffer->get16();
}

static QMap<Courier::Protocol::RejectCode, const char*>mapRejectCode = {
    {Courier::Protocol::RejectCode::NO_SUCH_PROGRAM_NUMBER, "NO_SUCH_PROGRAM_NUMBER"},
    {Courier::Protocol::RejectCode::NO_SUCH_VERSION_NUMBER, "NO_SUCH_VERSION_NUMBER"},
    {Courier::Protocol::RejectCode::NO_SUCH_PROCEDURE_VALUE, "NO_SUCH_PROCEDURE_VALUE"},
    {Courier::Protocol::RejectCode::INVALID_ARGUMENTS, "INVALID_ARGUMENTS"},
};
const char* Courier::getName(Protocol::RejectCode value) {
    return mapRejectCode.value(value, 0);
}
void serialize  (ByteBuffer* buffer, const Courier::Protocol::RejectCode& value) {
    buffer->put16((quint16)value);
}
void deserialize(ByteBuffer* buffer, Courier::Protocol::RejectCode& value) {
    value = (Courier::Protocol::RejectCode)buffer->get16();
}

void Courier::serialize  (ByteBuffer* buffer, const Protocol::Protocol2Body& value) {
    Courier::serialize(buffer, value.tag);
    switch(value.tag) {
    case Protocol::MessageType::CALL:
        Courier::serialize(buffer, value.CALL.transaction);
        Courier::serialize(buffer, value.CALL.program);
        Courier::serialize(buffer, value.CALL.version);
        Courier::serialize(buffer, value.CALL.procedure);
        break;
    case Protocol::MessageType::REJECT:
        Courier::serialize(buffer, value.REJECT.transaction);
        Courier::serialize(buffer, value.REJECT.reject);
        break;
    case Protocol::MessageType::RETURN:
        Courier::serialize(buffer, value.RETURN.transaction);
        break;
    case Protocol::MessageType::ABORT:
        Courier::serialize(buffer, value.ABORT.transaction);
        Courier::serialize(buffer, value.ABORT.abort);
        break;
    }
}

void Courier::deserialize(ByteBuffer* buffer, Protocol::Protocol2Body& value) {
    Courier::deserialize(buffer, value.tag);
    switch(value.tag) {
    case Protocol::MessageType::CALL:
        Courier::deserialize(buffer, value.CALL.transaction);
        Courier::deserialize(buffer, value.CALL.program);
        Courier::deserialize(buffer, value.CALL.version);
        Courier::deserialize(buffer, value.CALL.procedure);
        break;
    case Protocol::MessageType::REJECT:
        Courier::deserialize(buffer, value.REJECT.transaction);
        Courier::deserialize(buffer, value.REJECT.reject);
        break;
    case Protocol::MessageType::RETURN:
        Courier::deserialize(buffer, value.RETURN.transaction);
        break;
    case Protocol::MessageType::ABORT:
        Courier::deserialize(buffer, value.ABORT.transaction);
        Courier::deserialize(buffer, value.ABORT.abort);
        break;
    }
}

void Courier::serialize  (ByteBuffer* buffer, const Protocol::ProtocolRange& value) {
    Courier::serialize(buffer, value.low);
    Courier::serialize(buffer, value.high);
}

void Courier::deserialize(ByteBuffer* buffer, Protocol::ProtocolRange& value) {
    Courier::deserialize(buffer, value.low);
    Courier::deserialize(buffer, value.high);
}

void Courier::serialize  (ByteBuffer* buffer, const Protocol::VersionRange& value) {
    Courier::serialize(buffer, value.low);
    Courier::serialize(buffer, value.high);
}

void Courier::deserialize(ByteBuffer* buffer, Protocol::VersionRange& value) {
    Courier::deserialize(buffer, value.low);
    Courier::deserialize(buffer, value.high);
}

void Courier::serialize  (ByteBuffer* buffer, const Protocol::RejectBody& value) {
    Courier::serialize(buffer, value.tag);
    switch(value.tag) {
    case Protocol::RejectCode::NO_SUCH_PROGRAM_NUMBER:
        break;
    case Protocol::RejectCode::NO_SUCH_PROCEDURE_VALUE:
        break;
    case Protocol::RejectCode::INVALID_ARGUMENTS:
        break;
    case Protocol::RejectCode::NO_SUCH_VERSION_NUMBER:
        Courier::serialize(buffer, value.NO_SUCH_VERSION_NUMBER.range);
        break;
    }
}

void Courier::deserialize(ByteBuffer* buffer, Protocol::RejectBody& value) {
    Courier::deserialize(buffer, value.tag);
    switch(value.tag) {
    case Protocol::RejectCode::NO_SUCH_PROGRAM_NUMBER:
        break;
    case Protocol::RejectCode::NO_SUCH_PROCEDURE_VALUE:
        break;
    case Protocol::RejectCode::INVALID_ARGUMENTS:
        break;
    case Protocol::RejectCode::NO_SUCH_VERSION_NUMBER:
        Courier::deserialize(buffer, value.NO_SUCH_VERSION_NUMBER.range);
        break;
    }
}

void Courier::serialize  (ByteBuffer* buffer, const Protocol::Protocol3Body& value) {
    Courier::serialize(buffer, value.tag);
    switch(value.tag) {
    case Protocol::MessageType::CALL:
        Courier::serialize(buffer, value.CALL.transaction);
        Courier::serialize(buffer, value.CALL.program);
        Courier::serialize(buffer, value.CALL.version);
        Courier::serialize(buffer, value.CALL.procedure);
        break;
    case Protocol::MessageType::REJECT:
        Courier::serialize(buffer, value.REJECT.transaction);
        Courier::serialize(buffer, value.REJECT.rejectBody);
        break;
    case Protocol::MessageType::RETURN:
        Courier::serialize(buffer, value.RETURN.transaction);
        break;
    case Protocol::MessageType::ABORT:
        Courier::serialize(buffer, value.ABORT.transaction);
        Courier::serialize(buffer, value.ABORT.abort);
        break;
    }
}

void Courier::deserialize(ByteBuffer* buffer, Protocol::Protocol3Body& value) {
    Courier::deserialize(buffer, value.tag);
    switch(value.tag) {
    case Protocol::MessageType::CALL:
        Courier::deserialize(buffer, value.CALL.transaction);
        Courier::deserialize(buffer, value.CALL.program);
        Courier::deserialize(buffer, value.CALL.version);
        Courier::deserialize(buffer, value.CALL.procedure);
        break;
    case Protocol::MessageType::REJECT:
        Courier::deserialize(buffer, value.REJECT.transaction);
        Courier::deserialize(buffer, value.REJECT.rejectBody);
        break;
    case Protocol::MessageType::RETURN:
        Courier::deserialize(buffer, value.RETURN.transaction);
        break;
    case Protocol::MessageType::ABORT:
        Courier::deserialize(buffer, value.ABORT.transaction);
        Courier::deserialize(buffer, value.ABORT.abort);
        break;
    }
}

void Courier::serialize  (ByteBuffer* buffer, const Protocol::MessageObject& value) {
    Courier::serialize(buffer, value.tag);
    switch(value.tag) {
    case Protocol::ProtocolType::PROTOCOL0:
        break;
    case Protocol::ProtocolType::PROTOCOL1:
        break;
    case Protocol::ProtocolType::PROTOCOL2:
        Courier::serialize(buffer, value.PROTOCOL2.protocol2Body);
        break;
    case Protocol::ProtocolType::PROTOCOL3:
        Courier::serialize(buffer, value.PROTOCOL3.protocol3Body);
        break;
    }
}

void Courier::deserialize(ByteBuffer* buffer, Protocol::MessageObject& value) {
    Courier::deserialize(buffer, value.tag);
    switch(value.tag) {
    case Protocol::ProtocolType::PROTOCOL0:
        break;
    case Protocol::ProtocolType::PROTOCOL1:
        break;
    case Protocol::ProtocolType::PROTOCOL2:
        Courier::deserialize(buffer, value.PROTOCOL2.protocol2Body);
        break;
    case Protocol::ProtocolType::PROTOCOL3:
        Courier::deserialize(buffer, value.PROTOCOL3.protocol3Body);
        break;
    }
}
