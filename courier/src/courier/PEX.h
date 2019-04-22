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
// PEX.h
//

#ifndef COURIER_PEX_H__
#define COURIER_PEX_H__

#include "../courier/Block.h"
#include "../courier/IDP.h"

namespace Courier {

class PEX {
public:
    enum class ClientType : quint16 {
        UNSPEC   = 0,
        TIME     = 1,
        CHS      = 2,
        DEBUGGER = 8,
    };

    struct Header {
        quint32    id;
        ClientType clientType;
    };

	struct Frame {
		Header header;
		Block  data{nullptr, 0};
	};
};

QString toString(const Courier::PEX::ClientType value);

// Frame
QString toString(const Courier::PEX::Frame&  frame);
void serialize(Block& block, const Courier::PEX::Frame& frame);
void deserialize(Block& block, Courier::PEX::Frame& frame);

}

#endif

