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
// StreamOf.cpp
//

#include "../util/Debug.h"
#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("cr/StreamOf");

#include "../courier/StreamOf.h"

QString Courier::toString(const Courier::StreamOfSegmentType value) {
    static QMap<Courier::StreamOfSegmentType, QString> map = {
        {Courier::StreamOfSegmentType::nextSegment, "nextSegment"},
        {Courier::StreamOfSegmentType::lastSegment, "lastSegment"},
    };

    if (map.contains(value)) {
        return map[value];
    } else {
        return QString("%1").arg((quint16)value);
    }

}
void Courier::serialize(BLOCK& block, const Courier::StreamOfSegmentType value) {
    Courier::serialize(block, (quint16)value);
}
void Courier::deserialize(BLOCK& block, Courier::StreamOfSegmentType& value) {
    quint16 t;
    Courier::deserialize(block, t);
    value = (Courier::StreamOfSegmentType)t;
}
