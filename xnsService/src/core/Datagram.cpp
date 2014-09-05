/*
Copyright (c) 2014, Yasuhiro Hasegawa
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
// Datagram.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("datagram");

#include "../util/Debug.h"

#include "Datagram.h"


static QMap<quint8, const char*>typeNameMap {
	{Datagram::TYPE_ROUTING, "ROUTING"},
	{Datagram::TYPE_ECHO,    "ECHO"},
	{Datagram::TYPE_ERROR,   "ERROR"},
	{Datagram::TYPE_PEX,     "PEX"},
	{Datagram::TYPE_SPP,     "SPP"},
	{Datagram::TYPE_BOOT,    "BOOT"},
};
const char* Datagram::getTypeName(quint8 value) {
	const char* ret = typeNameMap.value(value, 0);
	if (ret == 0) {
		logger.fatal("%s  value = %d", __FUNCTION__, value);
		ERROR();
	}
	return ret;
}

static QMap<quint8, const char*>socketNameMap {
	{Datagram::SOCKET_ROUTING,    "ROUTING"},
	{Datagram::SOCKET_ECHO,       "ECHO"},
	{Datagram::SOCKET_ERROR,      "ERROR"},
	{Datagram::SOCKET_ENVOY,      "ENVOY"},
	{Datagram::SOCKET_COURIER,    "COURIER"},
	{Datagram::SOCKET_CH_OLD,     "CH_OLD"},
	{Datagram::SOCKET_TIME,       "TIME"},
	{Datagram::SOCKET_BOOT,       "BOOT"},
	{Datagram::SOCKET_DIAG,       "DIAG"},
	{Datagram::SOCKET_CH,         "CH"},
	{Datagram::SOCKET_AUTH,       "AUTH"},
	{Datagram::SOCKET_MAIL,       "MAIL"},
	{Datagram::SOCKET_NET_EXEC,   "NET_EXEC"},
	{Datagram::SOCKET_WS_INFO,    "WS_INFO"},
	{Datagram::SOCKET_BINDING,    "BINDING"},
	{Datagram::SOCKET_GERM,       "GERM"},
	{Datagram::SOCKET_TELE_DEBUG, "TELE_DEBUG"},
};

static char getSocketNameBuffer[8];
const char* Datagram::getSocketName(quint16 value) {
	if (value <= MAX_WELLKNOWN_SOCKET) {
		const char* ret = socketNameMap.value(value, 0);
		if (ret == 0) {
			logger.fatal("%s  value = %d", __FUNCTION__, value);
			ERROR();
		}
		return ret;
	}
	sprintf(getSocketNameBuffer, "%04X", value);
	return getSocketNameBuffer;
}

