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
// Socket_Echo.cpp
//

#include "../util/Debug.h"
#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("so/echo");

#include "../courier/Socket_Echo.h"

#include "../stub/IDP.h"
#include "../stub/Error.h"
#include "../stub/Echo.h"

namespace Echo = Courier::Stub::Echo;

void Courier::Socket::Listener_Echo::init() {
	logger.info(__FUNCTION__);
}

void Courier::Socket::Listener_Echo::destroy() {
	logger.info(__FUNCTION__);
}
void Courier::Socket::Listener_Echo::service(Frame& request, Frame& response, bool& sendResponse) const {
	logger.info(__FUNCTION__);

	logger.info("request   %s", Courier::toString(request) .toLocal8Bit().constData());
	logger.info("response  %s", Courier::toString(response).toLocal8Bit().constData());
	logger.info("sendResponse %s", Courier::toString(sendResponse) .toLocal8Bit().constData());
}
