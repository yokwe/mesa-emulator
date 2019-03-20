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
// Courier.h
//

#ifndef COURIER_COURIER_H_
#define COURIER_COURIER_H_

#include "../itp/IDP.h"

//
// See APilot/15.0.1/Corier/Friends/CourierProtocol.mesa
//
namespace Courier {
class Courier {
public:
    enum class ProtocolType : quint16 {
        PROTOCOL2 = 2,
        PROTOCOL3 = 3,
    };
    enum class MessageType : quint16 {
        CALL   = 0,
        REJECT = 1,
        RETURN = 2,
        ABORT  = 3,
    };
    enum class RejectCode : quint16 {
        NO_SUCH_PROGRAM_NUMBER  = 0,
        NO_SUCH_VERSION_NUMBER  = 1,
        NO_SUCH_PROCEDURE_VALUE = 2,
        INVALID_ARGUMENTS       = 3,
    };

    class ProtocolRange {
    public:
        ProtocolType low;
        ProtocolType high;
    };
    class VersionRange {
    public:
        quint16 low;
        quint16 high;
    };

	class Return {
        quint16 transaction;

        quint8  data[ITP::IDP::DATA_SIZE];
        NetData netData; // access data through netData
	};
	class Abort {
        quint16 transaction;
        quint16 abort;

        quint8  data[ITP::IDP::DATA_SIZE];
        NetData netData; // access data through netData
	};

	class Call2 {
	public:
        quint16 transaction;
        quint16 program;
        quint16 version;
        quint16 procedure;

        quint8  data[ITP::IDP::DATA_SIZE];
        NetData netData; // access data through netData
	};

	class Call3 {
	public:
        quint16 transaction;
        quint32 program;
        quint16 version;
        quint16 procedure;

        quint8  data[ITP::IDP::DATA_SIZE];
        NetData netData; // access data through netData
	};

	class Reject2 {
	public:
        quint16    transaction;
        RejectCode rejectCode;
    };

	class Reject3 {
	public:
        quint16    transaction;
        RejectCode tag;
        union union_choice {
            struct {
            } NO_SUCH_PROGRAM_NUMBER;
            struct {
            } NO_SUCH_PROCEDURE_VALUE;
            struct {
            } INVALID_ARGUMENTS;
            struct {
                VersionRange range;
            } NO_SUCH_VERSION_NUMBER;
        } choice;
	};

    // Protocol2
    class Protocol2 {
    public:
        MessageType tag;
        union union_choice {
        	Call2   call;
        	Reject2 reject;
        	Return  return_;
        	Abort   abort;
        } choice;
    };

    // Protocol3
    class Protocol3 {
    public:
        MessageType tag;
        union union_choice {
        	Call3   call;
        	Reject3 reject;
        	Return  return_;
        	Abort   abort;
        } choice;
    };

    // Main
    class Message {
    public:
        ProtocolType tag;
        union union_choice {
            Protocol2 protocol2;
            Protocol3 protocol3;
        } choice;
    };
};
}

#endif /* PEX_COURIER_H_ */
