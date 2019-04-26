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
// Protocol.h
//

#ifndef COURIER_PROTOCOL_H__
#define COURIER_PROTOCOL_H__

#include "../courier/Block.h"

namespace Courier {
namespace Protocol {

enum class ProtocolType : quint16 {
    protocol2 = 2,
    protocol3 = 3,
};

enum class MessageType : quint16 {
    call     = 0,
    reject   = 1,
    return__ = 2,
    abort__  = 3,
};

enum class RejectCode : quint16 {
    noSuchProgramNumber  = 0,
    noSuchVersionNumber  = 1,
    noSuchProcedureValue = 2,
    invalidArguments     = 3,
};

struct ProtocolRange {
	ProtocolType low;
	ProtocolType high;

	ProtocolRange() : low(ProtocolType::protocol2), high(ProtocolType::protocol3) {}
};

struct VersionRange {
	quint16 low;
	quint16 high;

	VersionRange() : low(0), high(0) {}
	VersionRange(quint16 low_, quint16 high_) : low(low_), high(high_) {}
};

struct Reject {
	RejectCode rejectCode;

	VersionRange& noSuchVersionNumber() const;

	Reject() : rejectCode(RejectCode::noSuchProgramNumber) {}
	Reject(RejectCode rejectCode_) : rejectCode(rejectCode_) {}
	Reject(quint16 low, quint16 high) : rejectCode(RejectCode::noSuchProcedureValue), CHOICE_noSuchVersionNumber(VersionRange(low, high)) {}
private:
	mutable VersionRange CHOICE_noSuchVersionNumber;
};

// Parent class of all Error
struct Abort {
    const char*   programName;
    const quint32 programCode;
    const quint16 versionCode;

    const char*   abortName;
    const quint16 abortCode;

    Abort(const char* programName_, quint32 programCode_, quint16 versionCode_, const char* name_, quint16 code_) :
    	programName(programName_), programCode(programCode_), versionCode(versionCode_), abortName(name_), abortCode(code_) {}
};

struct Protocol2 {
	struct CallMessage {
        quint16 transaction;
        quint16 program;
        quint16 version;
        quint16 procedure;
	};
	struct RejectMessage {
        quint16    transaction;
        RejectCode rejectCode;
	};
	struct ReturnMessage {
        quint16 transaction;
	};
	struct AbortMessage {
		quint16 transaction;
		quint16 abortCode;
	};
	MessageType messageType;

	CallMessage&   call()     const;
	RejectMessage& reject()   const;
	ReturnMessage& return__() const;
	AbortMessage&  abort__()  const;

	static Protocol2 reject(const CallMessage& callMessage, RejectCode rejectCode) {
		Protocol2 ret;
		ret.messageType = Protocol::MessageType::reject;
		ret.reject().transaction = callMessage.transaction;
		ret.reject().rejectCode = rejectCode;
		return ret;
	}
private:
	mutable CallMessage   CHOICE_call;
	mutable RejectMessage CHOICE_reject;
	mutable ReturnMessage CHOICE_return;
	mutable AbortMessage  CHOICE_abort;
};

struct Protocol3 {
	struct CallMessage {
        quint16 transaction;
        quint32 program;
        quint16 version;
        quint16 procedure;
	};
	struct RejectMessage {
        quint16 transaction;
        Reject  reject;
	};
	struct ReturnMessage {
        quint16 transaction;
	};
	struct AbortMessage {
		quint16 transaction;
		quint16 abortCode;
	};
	MessageType messageType;

	CallMessage&   call()     const;
	RejectMessage& reject()   const;
	ReturnMessage& return__() const;
	AbortMessage&  abort__()  const;

	static Protocol3 reject(const CallMessage& callMessage, RejectCode rejectCode) {
		Protocol3 ret;
		ret.messageType = Protocol::MessageType::reject;
		ret.reject().transaction       = callMessage.transaction;
		ret.reject().reject.rejectCode = rejectCode;
		return ret;
	}
	static Protocol3 return__(const CallMessage& callMessage) {
		Protocol3 ret;
		ret.messageType = Protocol::MessageType::return__;
		ret.return__().transaction = callMessage.transaction;
		return ret;
	}
	static Protocol3 abort__(const CallMessage& callMessage, quint16 abortCode) {
		Protocol3 ret;
		ret.messageType = Protocol::MessageType::abort__;
		ret.abort__().transaction = callMessage.transaction;
		ret.abort__().abortCode   = abortCode;
		return ret;
	}

private:
	mutable CallMessage   CHOICE_call;
	mutable RejectMessage CHOICE_reject;
	mutable ReturnMessage CHOICE_return;
	mutable AbortMessage  CHOICE_abort;
};

struct Message {
	ProtocolType protocolType;

	Protocol2& protocol2() const;
	Protocol3& protocol3() const;
private:
	mutable Protocol2 CHOICE_protocol2;
	mutable Protocol3 CHOICE_protocol3;
};

}

// ProtocolType
QString toString   (              const Protocol::ProtocolType  value);
void    serialize  (Block& block, const Protocol::ProtocolType  value);
void    deserialize(Block& block,       Protocol::ProtocolType& value);
// MessageType
QString toString   (              const Protocol::MessageType  value);
void    serialize  (Block& block, const Protocol::MessageType  value);
void    deserialize(Block& block,       Protocol::MessageType& value);
// RejectCode
QString toString   (              const Protocol::RejectCode value);
void    serialize  (Block& block, const Protocol::RejectCode value);
void    deserialize(Block& block,       Protocol::RejectCode& value);
// ProtocolRange
QString toString   (              const Protocol::ProtocolRange& value);
void    serialize  (Block& block, const Protocol::ProtocolRange& value);
void    deserialize(Block& block,       Protocol::ProtocolRange& value);
// VersionRange
QString toString   (              const Protocol::VersionRange& value);
void    serialize  (Block& block, const Protocol::VersionRange& value);
void    deserialize(Block& block,       Protocol::VersionRange& value);
// Reject
QString toString   (              const Protocol::Reject& value);
void    serialize  (Block& block, const Protocol::Reject& value);
void    deserialize(Block& block,       Protocol::Reject& value);
// Protocol2
QString toString   (              const Protocol::Protocol2& value);
void    serialize  (Block& block, const Protocol::Protocol2& value);
void    deserialize(Block& block,       Protocol::Protocol2& value);
// Protocol2::Call
QString toString   (              const Protocol::Protocol2::CallMessage& value);
void    serialize  (Block& block, const Protocol::Protocol2::CallMessage& value);
void    deserialize(Block& block,       Protocol::Protocol2::CallMessage& value);
// Protocol2::Reject
QString toString   (              const Protocol::Protocol2::RejectMessage& value);
void    serialize  (Block& block, const Protocol::Protocol2::RejectMessage& value);
void    deserialize(Block& block,       Protocol::Protocol2::RejectMessage& value);
// Protocol2::Return
QString toString   (              const Protocol::Protocol2::ReturnMessage& value);
void    serialize  (Block& block, const Protocol::Protocol2::ReturnMessage& value);
void    deserialize(Block& block,       Protocol::Protocol2::ReturnMessage& value);
// Protocol2::Abort
QString toString   (              const Protocol::Protocol2::AbortMessage& value);
void    serialize  (Block& block, const Protocol::Protocol2::AbortMessage& value);
void    deserialize(Block& block,       Protocol::Protocol2::AbortMessage& value);
// Protocol3
QString toString   (              const Protocol::Protocol3& value);
void    serialize  (Block& block, const Protocol::Protocol3& value);
void    deserialize(Block& block,       Protocol::Protocol3& value);
// Protocol3::Call
QString toString   (              const Protocol::Protocol3::CallMessage& value);
void    serialize  (Block& block, const Protocol::Protocol3::CallMessage& value);
void    deserialize(Block& block,       Protocol::Protocol3::CallMessage& value);
// Protocol3::Reject
QString toString   (              const Protocol::Protocol3::RejectMessage& value);
void    serialize  (Block& block, const Protocol::Protocol3::RejectMessage& value);
void    deserialize(Block& block,       Protocol::Protocol3::RejectMessage& value);
// Protocol3::Return
QString toString   (              const Protocol::Protocol3::ReturnMessage& value);
void    serialize  (Block& block, const Protocol::Protocol3::ReturnMessage& value);
void    deserialize(Block& block,       Protocol::Protocol3::ReturnMessage& value);
// Protocol3::Abort
QString toString   (              const Protocol::Protocol3::AbortMessage& value);
void    serialize  (Block& block, const Protocol::Protocol3::AbortMessage& value);
void    deserialize(Block& block,       Protocol::Protocol3::AbortMessage& value);
// Message
QString toString   (              const Protocol::Message& value);
void    serialize  (Block& block, const Protocol::Message& value);
void    deserialize(Block& block,       Protocol::Message& value);

}

#endif
