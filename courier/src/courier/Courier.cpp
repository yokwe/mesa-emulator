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
// Courier.cpp
//

#include "../util/Debug.h"
#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("cr/courier");

#include "../NetData.h"
#include "../courier/Courier.h"

void Courier::Courier::ProtocolRange::serialize(NetData& netData_) {
	netData_.put16((quint16)low);
	netData_.put16((quint16)high);
}
void Courier::Courier::ProtocolRange::deserialize(NetData& netData_) {
	low  = netData_.get16();
	high = netData_.get16();
}

void Courier::Courier::VersionRange::serialize(NetData& netData_) {
	netData_.put16((quint16)low);
	netData_.put16((quint16)high);
}
void Courier::Courier::VersionRange::deserialize(NetData& netData_) {
	low  = netData_.get16();
	high = netData_.get16();
}

void Courier::Courier::Return::serialize(NetData& netData_) {
	netData_.put16(transaction);
	netData_.put(netData, 0);
}
void Courier::Courier::Return::deserialize(NetData& netData_) {
	transaction = netData_.get16();

	netData.clear();
	netData.put(netData_, netData_.getPos());
	netData.rewind();
}

void Courier::Courier::Abort::serialize(NetData& netData_) {
	netData_.put16(transaction);
	netData_.put16(abort);
	netData_.put(netData, 0);
}
void Courier::Courier::Abort::deserialize(NetData& netData_) {
	transaction = netData_.get16();
	abort = netData_.get16();

	netData.clear();
	netData.put(netData_, netData_.getPos());
	netData.rewind();
}

void Courier::Courier::Call2::serialize(NetData& netData_) {
	netData_.put16(transaction);
	netData_.put16(program);
	netData_.put16(version);
	netData_.put16(procedure);
	netData_.put(netData, 0);
}
void Courier::Courier::Call2::deserialize(NetData& netData_) {
	transaction = netData_.get16();
	program = netData_.get16();
	version = netData_.get16();
	procedure = netData_.get16();

	netData.clear();
	netData.put(netData_, netData_.getPos());
	netData.rewind();
}

void Courier::Courier::Reject2::serialize(NetData& netData_) {
	netData_.put16(transaction);
	netData_.put16((quint16)rejectCode);
}
void Courier::Courier::Reject2::deserialize(NetData& netData_) {
	transaction = netData_.get16();
	rejectCode = (RejectCode)netData_.get16();
}

void Courier::Courier::Call3::serialize(NetData& netData_) {
	netData_.put16(transaction);
	netData_.put32(program);
	netData_.put16(version);
	netData_.put16(procedure);
	netData_.put(netData, 0);
}
void Courier::Courier::Call3::deserialize(NetData& netData_) {
	transaction = netData_.get16();
	program = netData_.get32();
	version = netData_.get16();
	procedure = netData_.get16();

	netData.clear();
	netData.put(netData_, netData_.getPos());
	netData.rewind();
}

void Courier::Courier::Reject3::serialize(NetData& netData_) {
	netData_.put16(transaction);
	netData_.put16((quint16)tag);
	switch(tag) {
	case Courier::Courier::RejectCode::NO_SUCH_PROGRAM_NUMBER:
	case Courier::Courier::RejectCode::NO_SUCH_PROCEDURE_VALUE:
	case Courier::Courier::RejectCode::INVALID_ARGUMENTS:
		break;
	case Courier::Courier::RejectCode::NO_SUCH_VERSION_NUMBER:
		choice.noSuchVersionNumber.serialize(netData_);
		break;
	default:
		RUNTIME_ERROR();
		break;
	}
}
void Courier::Courier::Reject3::(NetData& netData_) {
	transaction = netData_.get16();
	tag = (RejectCode)netData_.get16();
	switch(tag) {
	case Courier::Courier::RejectCode::NO_SUCH_PROGRAM_NUMBER:
	case Courier::Courier::RejectCode::NO_SUCH_PROCEDURE_VALUE:
	case Courier::Courier::RejectCode::INVALID_ARGUMENTS:
		break;
	case Courier::Courier::RejectCode::NO_SUCH_VERSION_NUMBER:
		choice.noSuchVersionNumber.deserialize(netData_);
		break;
	default:
		RUNTIME_ERROR();
		break;
	}
}

void Courier::Courier::Protocol2::serialize(NetData& netData_) {
	netData_.put16((quint16)tag);
	switch(tag) {
	case Courier::Courier::MessageType::CALL:
		choice.call.serialize(netData_);
		break;
	case Courier::Courier::MessageType::REJECT:
		choice.reject.serialize(netData_);
		break;
	case Courier::Courier::MessageType::RETURN:
		choice.return.serialize(netData_);
		break;
	case Courier::Courier::MessageType::ABORT:
		choice.abort.serialize(netData_);
		break;
	default:
		RUNTIME_ERROR();
		break;
	}
}
void Courier::Courier::Protocol2::deserialize(NetData& netData_) {
	tag = (MessageTag)netData_.get16();
	switch(tag) {
	case Courier::Courier::MessageType::CALL:
		choice.call.deserialize(netData_);
		break;
	case Courier::Courier::MessageType::REJECT:
		choice.reject.deserialize(netData_);
		break;
	case Courier::Courier::MessageType::RETURN:
		choice.return.deserialize(netData_);
		break;
	case Courier::Courier::MessageType::ABORT:
		choice.abort.desserialize(netData_);
		break;
	default:
		RUNTIME_ERROR();
		break;
	}
}

void Courier::Courier::Protocol3::serialize(NetData& netData_) {
	netData_.put16((quint16)tag);
	switch(tag) {
	case Courier::Courier::MessageType::CALL:
		choice.call.serialize(netData_);
		break;
	case Courier::Courier::MessageType::REJECT:
		choice.reject.serialize(netData_);
		break;
	case Courier::Courier::MessageType::RETURN:
		choice.return.serialize(netData_);
		break;
	case Courier::Courier::MessageType::ABORT:
		choice.abort.serialize(netData_);
		break;
	default:
		RUNTIME_ERROR();
		break;
	}
}
void Courier::Courier::Protocol3::deserialize(NetData& netData_) {
	tag = (MessageTag)netData_.get16();
	switch(tag) {
	case Courier::Courier::MessageType::CALL:
		choice.call.deserialize(netData_);
		break;
	case Courier::Courier::MessageType::REJECT:
		choice.reject.deserialize(netData_);
		break;
	case Courier::Courier::MessageType::RETURN:
		choice.return.deserialize(netData_);
		break;
	case Courier::Courier::MessageType::ABORT:
		choice.abort.desserialize(netData_);
		break;
	default:
		RUNTIME_ERROR();
		break;
	}
}

void Courier::Courier::Message::deserialize(NetData& netData_) {
	tag = (ProtocolType)netData_.get16();
	switch(tag) {
	case Courier::Courier::ProtocolType::PROTOCOL2:
		choice.protocol2.deserialize(netData_);
		break;
	case Courier::Courier::ProtocolType::PROTOCOL3:
		choice.protocol3.deserialize(netData_);
		break;
	default:
		RUNTIME_ERROR();
		break;
	}
}

//QString toString(const Courier::Courier::ProtocolType& value) {}
//QString toString(const Courier::Courier::MessageType& value) {}
//QString toString(const Courier::Courier::RejectCode& value) {}
//QString toString(const Courier::Courier::ProtocolRange& value) {}
//QString toString(const Courier::Courier::VersionRange& value) {}
//QString toString(const Courier::Courier::Return& value) {}
//QString toString(const Courier::Courier::Abort& value) {}
//QString toString(const Courier::Courier::Call2& value) {}
//QString toString(const Courier::Courier::Reject2& value) {}
//QString toString(const Courier::Courier::Call3& value) {}

QString toString(const Courier::Courier::Reject3& value) {
	QString ret = QString("[%1 %2 ").arg(transaction).arg(toString(rejectCodes);
	switch(tag) {
	case Courier::Courier::RejectCode::NO_SUCH_PROGRAM_NUMBER:
	case Courier::Courier::RejectCode::NO_SUCH_PROCEDURE_VALUE:
	case Courier::Courier::RejectCode::INVALID_ARGUMENTS:
		break;
	case Courier::Courier::RejectCode::NO_SUCH_VERSION_NUMBER:
		ret.append(toString(choice.noSuchVersionNumber));
		break;
	default:
		RUNTIME_ERROR();
		break;
	}
	ret.append("]");
	return ret;
}
QString toString(const Courier::Courier::Protocol2& value) {
	QString ret = QString("[%1 ").arg(toString(tag);
	switch(tag) {
	case Courier::Courier::MessageType::CALL:
		ret.append(toString(choice.call));
		break;
	case Courier::Courier::MessageType::REJECT:
		ret.append(toString(choice.reject));
		break;
	case Courier::Courier::MessageType::RETURN:
		ret.append(toString(choice.return_));
		break;
	case Courier::Courier::MessageType::ABORT:
		ret.append(toString(choice.abort));
		break;
	default:
		RUNTIME_ERROR();
		break;
	}
	ret.append("]");
	return ret;
}
QString toString(const Courier::Courier::Protocol3& value) {
	QString ret = QString("[%1 ").arg(toString(tag);
	switch(tag) {
	case Courier::Courier::MessageType::CALL:
		ret.append(toString(choice.call));
		break;
	case Courier::Courier::MessageType::REJECT:
		ret.append(toString(choice.reject));
		break;
	case Courier::Courier::MessageType::RETURN:
		ret.append(toString(choice.return_));
		break;
	case Courier::Courier::MessageType::ABORT:
		ret.append(toString(choice.abort));
		break;
	default:
		RUNTIME_ERROR();
		break;
	}
	ret.append("]");
	return ret;
}
QString toString(const Courier::Courier::Message& value) {
	QString ret = QString("[%1 ").arg(toString(tag);
	switch(tag) {
	case Courier::Courier::ProtocolType::PROTOCOL2:
		ret.append(toString(choice.protocol2));
		break;
	case Courier::Courier::ProtocolType::PROTOCOL3:
		ret.append(toString(choice.protocol3));
		break;
	default:
		RUNTIME_ERROR();
		break;
	}
	ret.append("]");
	return ret;
}

