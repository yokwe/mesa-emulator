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
// SPP.h
//


#ifndef IDP_SPP_H__
#define IDP_SPP_H__

#include <QtCore>

#include "../idp/IDP.h"
#include "../idp/NetData.h"

class SPP {
public:
	static const quint32 DATA_SIZE = IDP::DATA_SIZE - 12;

    union Control {
    	quint8 u;
    	struct {
    		quint8 reserved        : 4;
    		quint8 endOfMessage    : 1;
    		quint8 attention       : 1;
    		quint8 sendAcknowledge : 1;
    		quint8 systemPacket    : 1;
    	};

    	Control() : u(0) {};
    	Control(quint8 value) {
    		u = value;
    	}
    };

    enum class SST : quint8 {
        DATA        = 0,
        BULK        = 1,
        CLOSE       = 254,
        CLOSE_REPLY = 255,
    };


    void serialize  (NetData& netData);
    void deserialize(NetData& netData);

    SPP() : control(0), sst((SST)0), src_conn_id(0), dst_conn_id(0),
    		sequence(0), acknowledge(0), allocation(0), netData(data, sizeof(data)) {}

    Control control;
    SST     sst;
	quint16 src_conn_id;
	quint16 dst_conn_id;
	quint16 sequence;
	quint16 acknowledge;
	quint16 allocation;

    quint8  data[DATA_SIZE];
    NetData netData; // access data through netData
};

QString toString(const SPP::Control value);
QString toString(const SPP::SST value);
QString toString(const SPP& value);

#endif
