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
// Time.h
//

#ifndef PEX_TIME_H_
#define PEX_TIME_H_

#include "../itp/IDP.h"

namespace PEX {
class Time {
public:
	enum class Operation : quint16 {
        REQUEST  = 1,
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

    static const quint32 PROGRAM_NUMBER = 15;
    static const quint32 VERSION_NUMBER = 2;

    class Request {
    public:
        void serialize  (NetData& netData);
        void deserialize(NetData& netData);
    };

    class Response {
    public:
        void serialize  (NetData& netData);
        void deserialize(NetData& netData);

        quint32         currentTime;
        OffsetDirection offsetDirection;
        quint16         offsetHours;
        quint16         offsetMinutes;
        quint16         startOfDST;
        quint16         endOfDST;
        ToleranceType   toleranceType;
        quint32         tolerance;
    };

    void serialize  (NetData& netData);
    void deserialize(NetData& netData);

	quint16   version;
	Operation operation;

	union union_value {
		Request  request;
		Response response;
	} value;
};
}

QString toString(const PEX::Time::Operation value);
QString toString(const PEX::Time::OffsetDirection value);
QString toString(const PEX::Time::ToleranceType value);
QString toString(const PEX::Time& value);

#endif /* PEX_TIME_H_ */
