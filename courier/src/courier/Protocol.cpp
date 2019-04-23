/*
Copyright (c) 2019, Yasuhiro Hasegawa
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
  3. The name of the author may not be used to endorse or promote products derived
     from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/


//
// Protocol.cpp
//

#include "../util/Debug.h"
#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("cr/protocol");

#include "../courier/Protocol.h"
#include "../stub/Protocol.h"


static void copy(Courier::Protocol::ProtocolRange& left, const Courier::Stub::Protocol::ProtocolRange& right) {
	left.low  = (decltype(left.low))right.low;
	left.high = (decltype(left.high))right.high;
}
static void copy(Courier::Stub::Protocol::ProtocolRange& left, const Courier::Protocol::ProtocolRange& right) {
	left.low  = (decltype(left.low))right.low;
	left.high = (decltype(left.high))right.high;
}
static void copy(Courier::Protocol::VersionRange& left, const Courier::Stub::Protocol::VersionRange& right) {
	left.low  = right.low;
	left.high = right.high;
}
static void copy(Courier::Stub::Protocol::VersionRange& left, const Courier::Protocol::VersionRange& right) {
	left.low  = right.low;
	left.high = right.high;
}

// Reject
Courier::Protocol::VersionRange& Courier::Protocol::Reject::noSuchVersionNumber() const {
	if (rejectCode == Courier::Protocol::RejectCode::noSuchVersionNumber) {
		return CHOICE_noSuchVersionNumber;
	} else {
        logger.error("Unexpected rejectCode = %d", (quint16)rejectCode);
        COURIER_FATAL_ERROR();
	}
}
// Protocol2
Courier::Protocol::Protocol2::CallMessage& Courier::Protocol::Protocol2::call() const {
	if (messageType == Courier::Protocol::MessageType::call) {
		return CHOICE_call;
	} else {
        logger.error("Unexpected messageType = %d", (quint16)messageType);
        COURIER_FATAL_ERROR();
	}
}
Courier::Protocol::Protocol2::RejectMessage& Courier::Protocol::Protocol2::reject() const {
	if (messageType == Courier::Protocol::MessageType::reject) {
		return CHOICE_reject;
	} else {
        logger.error("Unexpected messageType = %d", (quint16)messageType);
        COURIER_FATAL_ERROR();
	}
}
Courier::Protocol::Protocol2::ReturnMessage& Courier::Protocol::Protocol2::return__() const {
	if (messageType == Courier::Protocol::MessageType::return__) {
		return CHOICE_return;
	} else {
        logger.error("Unexpected messageType = %d", (quint16)messageType);
        COURIER_FATAL_ERROR();
	}
}
Courier::Protocol::Protocol2::AbortMessage& Courier::Protocol::Protocol2::abort__() const {
	if (messageType == Courier::Protocol::MessageType::abort__) {
		return CHOICE_abort;
	} else {
        logger.error("Unexpected messageType = %d", (quint16)messageType);
        COURIER_FATAL_ERROR();
	}
}
// Protocol3
Courier::Protocol::Protocol3::CallMessage& Courier::Protocol::Protocol3::call() const {
	if (messageType == Courier::Protocol::MessageType::call) {
		return CHOICE_call;
	} else {
        logger.error("Unexpected messageType = %d", (quint16)messageType);
        COURIER_FATAL_ERROR();
	}
}
Courier::Protocol::Protocol3::RejectMessage& Courier::Protocol::Protocol3::reject() const {
	if (messageType == Courier::Protocol::MessageType::reject) {
		return CHOICE_reject;
	} else {
        logger.error("Unexpected messageType = %d", (quint16)messageType);
        COURIER_FATAL_ERROR();
	}
}
Courier::Protocol::Protocol3::ReturnMessage& Courier::Protocol::Protocol3::return__() const {
	if (messageType == Courier::Protocol::MessageType::return__) {
		return CHOICE_return;
	} else {
        logger.error("Unexpected messageType = %d", (quint16)messageType);
        COURIER_FATAL_ERROR();
	}
}
Courier::Protocol::Protocol3::AbortMessage& Courier::Protocol::Protocol3::abort__() const {
	if (messageType == Courier::Protocol::MessageType::abort__) {
		return CHOICE_abort;
	} else {
        logger.error("Unexpected messageType = %d", (quint16)messageType);
        COURIER_FATAL_ERROR();
	}
}
// Message
Courier::Protocol::Protocol2& Courier::Protocol::Message::protocol2() const {
	if (protocolType == Courier::Protocol::ProtocolType::protocol2) {
		return CHOICE_protocol2;
	} else {
        logger.error("Unexpected protocolType = %d", (quint16)protocolType);
        COURIER_FATAL_ERROR();
	}
}
Courier::Protocol::Protocol3& Courier::Protocol::Message::protocol3() const {
	if (protocolType == Courier::Protocol::ProtocolType::protocol3) {
		return CHOICE_protocol3;
	} else {
        logger.error("Unexpected protocolType = %d", (quint16)protocolType);
        COURIER_FATAL_ERROR();
	}
}


// ProtocolType
QString Courier::toString   (              const Protocol::ProtocolType value) {
	return Courier::toString((Courier::Stub::Protocol::ProtocolType)value);
}
void    Courier::serialize  (Block& block, const Protocol::ProtocolType value) {
    Courier::serialize(block, (quint16)value);
}
void    Courier::deserialize(Block& block,       Protocol::ProtocolType& value) {
    quint16 t;
    Courier::deserialize(block, t);
    value = (Protocol::ProtocolType)t;
}
// MessageType
QString Courier::toString   (              const Protocol::MessageType value) {
	return Courier::toString((Courier::Stub::Protocol::MessageType)value);
}
void    Courier::serialize  (Block& block, const Protocol::MessageType value) {
    Courier::serialize(block, (quint16)value);
}
void    Courier::deserialize(Block& block,       Protocol::MessageType& value) {
    quint16 t;
    Courier::deserialize(block, t);
    value = (Protocol::MessageType)t;
}
// RejectCode
QString Courier::toString   (              const Protocol::RejectCode value) {
	return Courier::toString((Courier::Stub::Protocol::RejectCode)value);
}
void    Courier::serialize  (Block& block, const Protocol::RejectCode value) {
    Courier::serialize(block, (quint16)value);
}
void    Courier::deserialize(Block& block,       Protocol::RejectCode& value) {
    quint16 t;
    Courier::deserialize(block, t);
    value = (Protocol::RejectCode)t;
}
// ProtocolRange
QString Courier::toString   (              const Protocol::ProtocolRange& value) {
	return QString("[%1 %2]").arg(Courier::toString(value.low)).arg(Courier::toString(value.high));
}
void    Courier::serialize  (Block& block, const Protocol::ProtocolRange& value) {
	Courier::Stub::Protocol::ProtocolRange protocolRange;

	copy(protocolRange, value);
	Courier::serialize(block, protocolRange);
}
void    Courier::deserialize(Block& block,       Protocol::ProtocolRange& value) {
	Courier::Stub::Protocol::ProtocolRange protocolRange;
    Courier::deserialize(block, protocolRange);

    copy(value, protocolRange);
}
// VersionRange
QString Courier::toString   (              const Protocol::VersionRange& value) {
	return QString("[%1 %2]").arg(Courier::toString(value.low)).arg(Courier::toString(value.high));
}
void    Courier::serialize  (Block& block, const Protocol::VersionRange& value) {
	Courier::Stub::Protocol::VersionRange versionRange;

	copy(versionRange, value);
	Courier::serialize(block, versionRange);
}
void    Courier::deserialize(Block& block,       Protocol::VersionRange& value) {
	Courier::Stub::Protocol::VersionRange versionRange;
    Courier::deserialize(block, versionRange);

    copy(value, versionRange);
}
// Reject
QString Courier::toString   (              const Protocol::Reject& value) {
    switch(value.rejectCode) {
    case Courier::Protocol::RejectCode::noSuchProgramNumber:
    	return "noSuchProgramNumber";
    case Courier::Protocol::RejectCode::noSuchProcedureValue:
    	return "noSuchProcedureValue";
    case Courier::Protocol::RejectCode::invalidArguments:
    	return "invalidArguments";
    case Courier::Protocol::RejectCode::noSuchVersionNumber:
        return QString("[%1 %2]").arg("noSuchVersionNumber").arg(Courier::toString(value.noSuchVersionNumber()));
    default:
        logger.error("Unexpected rejectCode = %d", (quint16)value.rejectCode);
        COURIER_FATAL_ERROR();
        break;
    }
}
void    Courier::serialize  (Block& block, const Protocol::Reject& value) {
	Courier::Stub::Protocol::Reject reject;
	// Build reject form value
	reject.choiceTag = (decltype(reject.choiceTag))value.rejectCode;
	switch(value.rejectCode) {
    case Courier::Protocol::RejectCode::noSuchProgramNumber:
    case Courier::Protocol::RejectCode::noSuchProcedureValue:
    case Courier::Protocol::RejectCode::invalidArguments:
    	break;
    case Courier::Protocol::RejectCode::noSuchVersionNumber:
    	copy(reject.noSuchVersionNumber().range, value.noSuchVersionNumber());
    	break;
    default:
        logger.error("Unexpected value.rejectCode = %d", (quint16)value.rejectCode);
        COURIER_FATAL_ERROR();
        break;
    }

    Courier::serialize(block, reject);
}
void    Courier::deserialize(Block& block,       Protocol::Reject& value) {
	Courier::Stub::Protocol::Reject reject;
    Courier::deserialize(block, reject);

    // Build value from reject
    value.rejectCode = (decltype(value.rejectCode))reject.choiceTag;
	switch(value.rejectCode) {
    case Courier::Protocol::RejectCode::noSuchProgramNumber:
    case Courier::Protocol::RejectCode::noSuchProcedureValue:
    case Courier::Protocol::RejectCode::invalidArguments:
    	break;
    case Courier::Protocol::RejectCode::noSuchVersionNumber:
    	copy(value.noSuchVersionNumber(), reject.noSuchVersionNumber().range);
    	break;
    default:
        logger.error("Unexpected value.rejectCode = %d", (quint16)value.rejectCode);
        COURIER_FATAL_ERROR();
        break;
    }
}
// Protocol2
QString Courier::toString   (              const Protocol::Protocol2& value) {
    switch(value.messageType) {
    case Courier::Protocol::MessageType::call:
        return QString("[%1 %2]").arg("call").arg(Courier::toString(value.call()));
    case Courier::Protocol::MessageType::reject:
        return QString("[%1 %2]").arg("reject").arg(Courier::toString(value.reject()));
    case Courier::Protocol::MessageType::return__:
        return QString("[%1 %2]").arg("return").arg(Courier::toString(value.return__()));
    case Courier::Protocol::MessageType::abort__:
        return QString("[%1 %2]").arg("abort").arg(Courier::toString(value.abort__()));
    default:
        logger.error("Unexpected messageType = %d", (quint16)value.messageType);
        COURIER_FATAL_ERROR();
        break;
    }
}
void    Courier::serialize  (Block& block, const Protocol::Protocol2& value) {
	Courier::Stub::Protocol::Protocol2 protocol2;
	// Build protocol2 from value
	protocol2.choiceTag = (decltype(protocol2.choiceTag))value.messageType;
	switch(value.messageType) {
    case Courier::Protocol::MessageType::call:
    {
    	Courier::Stub::Protocol::Protocol2::Protocol2_CHOICE_01& left  = protocol2.call();
    	Courier::Protocol::Protocol2::CallMessage&               right = value.call();

    	left.transaction = right.transaction;
    	left.program     = right.program;
    	left.version     = right.version;
    	left.procedure   = right.procedure;
    }
    	break;
    case Courier::Protocol::MessageType::reject:
    {
    	Courier::Stub::Protocol::Protocol2::Protocol2_CHOICE_02& left  = protocol2.reject();
    	Courier::Protocol::Protocol2::RejectMessage&             right = value.reject();

    	left.transaction = right.transaction;
    	left.rejectCode  = (decltype(left.rejectCode))right.rejectCode;
    }
    	break;
    case Courier::Protocol::MessageType::return__:
    {
    	Courier::Stub::Protocol::Protocol2::Protocol2_CHOICE_03& left  = protocol2.return__();
    	Courier::Protocol::Protocol2::ReturnMessage&             right = value.return__();

    	left.transaction = right.transaction;
    }
    	break;
    case Courier::Protocol::MessageType::abort__:
    {
    	Courier::Stub::Protocol::Protocol2::Protocol2_CHOICE_04& left  = protocol2.abort__();
    	Courier::Protocol::Protocol2::AbortMessage&              right = value.abort__();

    	left.transaction = right.transaction;
    	left.abortCode   = right.abortCode;
    }
    	break;
    default:
        logger.error("Unexpected messageType = %d", (quint16)value.messageType);
        COURIER_FATAL_ERROR();
        break;
    }

    Courier::serialize(block, protocol2);
}
void    Courier::deserialize(Block& block,       Protocol::Protocol2& value) {
	Courier::Stub::Protocol::Protocol2 protocol2;
    Courier::deserialize(block, protocol2);

    // Build value from protocol2
    value.messageType = (decltype(value.messageType))protocol2.choiceTag;
	switch(value.messageType) {
    case Courier::Protocol::MessageType::call:
    {
    	Courier::Stub::Protocol::Protocol2::Protocol2_CHOICE_01& right = protocol2.call();
    	Courier::Protocol::Protocol2::CallMessage&               left  = value.call();

    	left.transaction = right.transaction;
    	left.program     = right.program;
    	left.version     = right.version;
    	left.procedure   = right.procedure;
    }
    	break;
    case Courier::Protocol::MessageType::reject:
    {
    	Courier::Stub::Protocol::Protocol2::Protocol2_CHOICE_02& right = protocol2.reject();
    	Courier::Protocol::Protocol2::RejectMessage&             left  = value.reject();

    	left.transaction = right.transaction;
    	left.rejectCode  = (decltype(left.rejectCode))right.rejectCode;
    }
    	break;
    case Courier::Protocol::MessageType::return__:
    {
    	Courier::Stub::Protocol::Protocol2::Protocol2_CHOICE_03& right = protocol2.return__();
    	Courier::Protocol::Protocol2::ReturnMessage&             left  = value.return__();

    	left.transaction = right.transaction;
    }
    	break;
    case Courier::Protocol::MessageType::abort__:
    {
    	Courier::Stub::Protocol::Protocol2::Protocol2_CHOICE_04& right = protocol2.abort__();
    	Courier::Protocol::Protocol2::AbortMessage&              left  = value.abort__();
    	left.transaction = right.transaction;
    	left.abortCode   = right.abortCode;
    }
    	break;
    default:
        logger.error("Unexpected messageType = %d", (quint16)value.messageType);
        COURIER_FATAL_ERROR();
        break;
	}
}
// Protocol2::Call
QString Courier::toString   (              const Protocol::Protocol2::CallMessage& value) {
	return QString("[%1 %2 %3 %4]").arg(value.transaction, 4, 16).arg(value.program, 2).arg(value.version).arg(value.procedure, 2);
}
// Protocol2::Reject
QString Courier::toString   (              const Protocol::Protocol2::RejectMessage& value) {
	return QString("[%1 %2]").arg(value.transaction, 4, 16).arg(Courier::toString(value.rejectCode));
}
// Protocol2::Return
QString Courier::toString   (              const Protocol::Protocol2::ReturnMessage& value) {
	return QString("[%1]").arg(value.transaction, 4, 16);
}
// Protocol2::Abort
QString Courier::toString   (              const Protocol::Protocol2::AbortMessage& value) {
	return QString("[%1 %2]").arg(value.transaction, 4, 16).arg(value.abortCode, 2);
}
// Protocol3
QString Courier::toString   (              const Protocol::Protocol3& value) {
    switch(value.messageType) {
    case Courier::Protocol::MessageType::call:
        return QString("[%1 %2]").arg("call").arg(Courier::toString(value.call()));
    case Courier::Protocol::MessageType::reject:
        return QString("[%1 %2]").arg("reject").arg(Courier::toString(value.reject()));
    case Courier::Protocol::MessageType::return__:
        return QString("[%1 %2]").arg("return").arg(Courier::toString(value.return__()));
    case Courier::Protocol::MessageType::abort__:
        return QString("[%1 %2]").arg("abort").arg(Courier::toString(value.abort__()));
    default:
        logger.error("Unexpected messageType = %d", (quint16)value.messageType);
        COURIER_FATAL_ERROR();
        break;
    }
}
void    Courier::serialize  (Block& block, const Protocol::Protocol3& value) {
	Courier::Stub::Protocol::Protocol3 protocol3;
	// Build protocol3 from value
	protocol3.choiceTag = (decltype(protocol3.choiceTag))value.messageType;
	switch(value.messageType) {
    case Courier::Protocol::MessageType::call:
    {
    	Courier::Stub::Protocol::Protocol3::Protocol3_CHOICE_01& left  = protocol3.call();
    	Courier::Protocol::Protocol3::CallMessage&               right = value.call();

    	left.transaction = right.transaction;
    	left.program     = right.program;
    	left.version     = right.version;
    	left.procedure   = right.procedure;
    }
    	break;
    case Courier::Protocol::MessageType::reject:
    {
    	Courier::Stub::Protocol::Protocol3::Protocol3_CHOICE_02& left  = protocol3.reject();
    	Courier::Protocol::Protocol3::RejectMessage&             right = value.reject();

    	left.transaction = right.transaction;
    	left.reject.choiceTag = (decltype(left.reject.choiceTag))right.reject.rejectCode;
    	if (right.reject.rejectCode == Courier::Protocol::RejectCode::noSuchVersionNumber) {
    		copy(left.reject.noSuchVersionNumber().range, right.reject.noSuchVersionNumber());
    	}
    }
    	break;
    case Courier::Protocol::MessageType::return__:
    {
    	Courier::Stub::Protocol::Protocol3::Protocol3_CHOICE_03& left  = protocol3.return__();
    	Courier::Protocol::Protocol3::ReturnMessage&             right = value.return__();

    	left.transaction = right.transaction;
    }
    	break;
    case Courier::Protocol::MessageType::abort__:
    {
    	Courier::Stub::Protocol::Protocol3::Protocol3_CHOICE_04& left  = protocol3.abort__();
    	Courier::Protocol::Protocol3::AbortMessage&              right = value.abort__();
    	left.transaction = right.transaction;
    	left.abortCode   = right.abortCode;
    }
    	break;
    default:
        logger.error("Unexpected messageType = %d", (quint16)value.messageType);
        COURIER_FATAL_ERROR();
        break;
    }

    Courier::serialize(block, protocol3);
}
void    Courier::deserialize(Block& block,       Protocol::Protocol3& value) {
	Courier::Stub::Protocol::Protocol3 protocol3;
    Courier::deserialize(block, protocol3);

    // Build value from protocol3
    value.messageType = (decltype(value.messageType))protocol3.choiceTag;
	switch(value.messageType) {
    case Courier::Protocol::MessageType::call:
    {
    	Courier::Stub::Protocol::Protocol3::Protocol3_CHOICE_01& right = protocol3.call();
    	Courier::Protocol::Protocol3::CallMessage&               left  = value.call();

    	left.transaction = right.transaction;
    	left.program     = right.program;
    	left.version     = right.version;
    	left.procedure   = right.procedure;
    }
    	break;
    case Courier::Protocol::MessageType::reject:
    {
    	Courier::Stub::Protocol::Protocol3::Protocol3_CHOICE_02& right = protocol3.reject();
    	Courier::Protocol::Protocol3::RejectMessage&             left  = value.reject();

    	left.transaction = right.transaction;
    	left.reject.rejectCode = (decltype(left.reject.rejectCode))right.reject.choiceTag;
    	if (left.reject.rejectCode == Courier::Protocol::RejectCode::noSuchVersionNumber) {
    		copy(left.reject.noSuchVersionNumber(), right.reject.noSuchVersionNumber().range);
    	}
    }
    	break;
    case Courier::Protocol::MessageType::return__:
    {
    	Courier::Stub::Protocol::Protocol3::Protocol3_CHOICE_03& right = protocol3.return__();
    	Courier::Protocol::Protocol3::ReturnMessage&             left  = value.return__();

    	left.transaction = right.transaction;
    }
    	break;
    case Courier::Protocol::MessageType::abort__:
    {
    	Courier::Stub::Protocol::Protocol3::Protocol3_CHOICE_04& right = protocol3.abort__();
    	Courier::Protocol::Protocol3::AbortMessage&              left  = value.abort__();
    	left.transaction = right.transaction;
    	left.abortCode   = right.abortCode;
    }
    	break;
    default:
        logger.error("Unexpected messageType = %d", (quint16)value.messageType);
        COURIER_FATAL_ERROR();
        break;
	}
}
// Protocol3::Call
QString Courier::toString   (              const Protocol::Protocol3::CallMessage& value) {
	return QString("[%1 %2 %3 %4]").arg(value.transaction, 4, 16).arg(value.program, 2).arg(value.version).arg(value.procedure, 2);
}
// Protocol3::Reject
QString Courier::toString   (              const Protocol::Protocol3::RejectMessage& value) {
	return QString("[%1 %2]").arg(value.transaction, 4, 16).arg(Courier::toString(value.reject));
}
// Protocol3::Return
QString Courier::toString   (              const Protocol::Protocol3::ReturnMessage& value) {
	return QString("[%1]").arg(value.transaction, 4, 16);
}
// Protocol3::Abort
QString Courier::toString   (              const Protocol::Protocol3::AbortMessage& value) {
	return QString("[%1 %2]").arg(value.transaction, 4, 16).arg(value.abortCode, 2);
}
// Message
QString Courier::toString   (              const Protocol::Message& value) {
    switch(value.protocolType) {
    case Courier::Protocol::ProtocolType::protocol2:
        return QString("[%1 %2]").arg(Courier::toString(value.protocolType)).arg(Courier::toString(value.protocol2()));
    case Courier::Protocol::ProtocolType::protocol3:
        return QString("[%1 %2]").arg(Courier::toString(value.protocolType)).arg(Courier::toString(value.protocol3()));
    default:
        logger.error("Unexpected protocolType = %d", (quint16)value.protocolType);
        COURIER_FATAL_ERROR();
        break;
    }
}
void    Courier::serialize  (Block& block, const Protocol::Message& value) {
	Courier::serialize(block, value.protocolType);
    switch(value.protocolType) {
    case Courier::Protocol::ProtocolType::protocol2:
    	Courier::serialize(block, value.protocol2());
        break;
    case Courier::Protocol::ProtocolType::protocol3:
    	Courier::serialize(block, value.protocol3());
    	break;
    default:
        logger.error("Unexpected protocolType = %d", (quint16)value.protocolType);
        COURIER_FATAL_ERROR();
        break;
    }
}
void    Courier::deserialize(Block& block,       Protocol::Message& value) {
	Courier::deserialize(block, value.protocolType);
    switch(value.protocolType) {
    case Courier::Protocol::ProtocolType::protocol2:
    	Courier::deserialize(block, value.protocol2());
        break;
    case Courier::Protocol::ProtocolType::protocol3:
    	Courier::deserialize(block, value.protocol3());
    	break;
    default:
        logger.error("Unexpected protocolType = %d", (quint16)value.protocolType);
        COURIER_FATAL_ERROR();
        break;
    }
}
