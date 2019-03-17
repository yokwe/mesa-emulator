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
// Error.h
//


#ifndef ITP_ERROR_H__
#define ITP_ERROR_H__

#include "../itp/IDP.h"

namespace ITP {
class Error {
public:
    enum class ErrorNumber : quint16 {
        UNSPECIFIED          = 0,
        BAD_CHECKSUM         = 1,
        NO_SOCKET            = 2,
        RESOURCE_LIMIT       = 3,
        LISTEN_REJECT        = 4,
        INVALID_PACKET_TYPE  = 5,
        PROTOCOL_VIOLATION   = 6,
        UNSPECIFIED_IN_ROUTE = 512,
        INCONSISTENT         = 513,
        CANT_GET_THERE       = 514,
        EXCESS_HOPS          = 515,
        TOO_BIG              = 516,
        CONGESTION_WARNING   = 517,
        CONGESTION_DISCARD   = 518,
    };

	static const quint32 DATA_SIZE = ITP::IDP::DATA_SIZE - 2;

    void serialize  (NetData& netData);
    void deserialize(NetData& netData);

    Error() : errorNumber((ErrorNumber)0), errorParameter(0), netData(data, sizeof(data)) {}
    Error(ErrorNumber errorNumber_) : errorNumber(errorNumber_), errorParameter(0), netData(data, sizeof(data)) {}

    ErrorNumber errorNumber;
    quint16     errorParameter;

    quint8  data[DATA_SIZE];
    NetData netData; // access data through netData
};
}

QString toString(const ITP::Error::ErrorNumber value);
QString toString(const ITP::Error& value);

#endif
