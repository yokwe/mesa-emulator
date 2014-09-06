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
// Datagram.h
//

#ifndef DATAGRAM_H__
#define DATAGRAM_H__

class Datagram {
public:
	static const quint16 CHECKSUM_DONTCARE = 0xffff;
	static const quint8  TYPE_ROUTING      = 1;
	static const quint8  TYPE_ECHO         = 2;
	static const quint8  TYPE_ERROR        = 3;
	static const quint8  TYPE_PEX          = 4;
	static const quint8  TYPE_SPP          = 5;
	static const quint8  TYPE_BOOT         = 9;
	static const char* getTypeName(quint8 value);

    static const quint16 SOCKET_ROUTING    =  1;
    static const quint16 SOCKET_ECHO       =  2;
    static const quint16 SOCKET_ERROR      =  3;
    static const quint16 SOCKET_ENVOY      =  4;
    static const quint16 SOCKET_COURIER    =  5;
    static const quint16 SOCKET_CH_OLD     =  7;
    static const quint16 SOCKET_TIME       =  8;
    static const quint16 SOCKET_BOOT       = 10;
    static const quint16 SOCKET_DIAG       = 19;
    static const quint16 SOCKET_CH         = 20;
    static const quint16 SOCKET_AUTH       = 21;
    static const quint16 SOCKET_MAIL       = 22;
    static const quint16 SOCKET_NET_EXEC   = 23;
    static const quint16 SOCKET_WS_INFO    = 24;
    static const quint16 SOCKET_BINDING    = 28;
    static const quint16 SOCKET_GERM       = 35;
    static const quint16 SOCKET_TELE_DEBUG = 48;
	static const char* getSocketName(quint16 value);

    static const quint16 WELLKNOWN_SOCKET_MAX = 3000;

	static const quint32 NETWORK_UNKNONW = 0;
	static const quint32 NETWORK_ALL     = 0xffffffff;
	static const quint64 HOST_UNKNONW    = 0LL;
	static const quint64 HOST_ALL        = 0xffffffffffffLL;
};

#endif
