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

#ifndef RPC_COURIER_H_
#define RPC_COURIER_H_

#include "../itp/IDP.h"

namespace RPC {
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

    // Protocol2
    struct Protocol2Body {
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

    // Protocol3
    struct ProtocolRange {
        ProtocolType low;
        ProtocolType high;
    };
    struct VersionRange {
        quint16 low;
        quint16 high;
    };
    struct RejectBody {
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
    struct Protocol3Body {
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

    // Main
    struct MessageObject {
        ProtocolType tag;
        union {
            struct {
                Protocol2Body protocol2Body;
            } PROTOCOL2;
            struct {
                Protocol3Body protocol3Body;
            } PROTOCOL3;
        };
    };

    //See ExpeditedCourier.mesa
    //   Header: TYPE = MACHINE DEPENDENT RECORD [
    //    protRange: CourierProtocol.ProtocolRange � [protocol3, protocol3],
    //    body: CourierProtocol.Protocol3Body];
    //0003 0003 0000 0000 0000 0002 0003 0000
    //PROTOCOL3
    //     PROTOCOL3
    //          CALL
    //               TRANSACTION
    //                    PROGRAM
    //                              VERSION
    //                                   PROCEDUER
    class ExpeditedCourier {
    public:
    	Courier::ProtocolRange protocolRange;
    	Courier::Protocol3Body body;
    };
};
}



#endif /* RPC_COURIER_H_ */
