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
// NetworkPacket.h
//

#ifndef NETWORK_PACKET_H__
#define NETWORK_PACKET_H__

#include "../mesa/MesaBasic.h"
#include "../mesa/Pilot.h"

#ifdef __linux__
#include <linux/if_ether.h>
#else
// We assume this is for WIN32
#define ETH_ALEN        6               /* Octets in one ethernet addr   */
#define ETH_HLEN        14              /* Total octets in header.       */
#define ETH_ZLEN        60              /* Min. octets in frame sans FCS */
#define ETH_DATA_LEN    1500            /* Max. octets in payload        */
#define ETH_FRAME_LEN   1514            /* Max. octets in frame sans FCS */
#define ETH_FCS_LEN     4               /* Octets in the FCS             */
#endif

#include <QtCore>

class NetworkPacket {
public:
	// packet type of Xerox IDP
	static const int ETH_P_IDP = 0x0600;

	class ReceivedPacket {
	public:
		quint64 timeStamp;             // unit is millisecond
		int     dataLen;               // if there is no data, dataLen is 0
		quint8  data[ETH_FRAME_LEN];

		ReceivedPacket();
		ReceivedPacket(const ReceivedPacket& that);
	};

	NetworkPacket() {
		fd = 0;
		for(int i = 0; i < ETH_ALEN; i++) address[i] = 0;
	}

	void getAddress(CARD16& pid1, CARD16& pid2, CARD16& pid3) {
		pid1 = (address[0] << 8) | address[1];
		pid2 = (address[2] << 8) | address[3];
		pid3 = (address[4] << 8) | address[5];
	}

	int getFileDescriptor() {
		return fd;
	}

	void attach(const QString& name_);
	void detach();

	// discard already received packet
	void discardRecievedPacket();
	void discardOnePacket();
	int  select(CARD32 timeout, int& opErrno);

	// transmit is immediate operation
	void transmit(EthernetIOFaceGuam::EthernetIOCBType* iocb);
	// receive is immediate operation
	void receive (EthernetIOFaceGuam::EthernetIOCBType* iocb);

	// returns return code of send and recv. no error checking
	int transmit(CARD8* data, CARD32 dataLen, int& opErrno);
	int receive (CARD8* data, CARD32 dataLen, int& opErrno);

private:
	QString name;
	int    fd;
	CARD8  address[ETH_ALEN];
};

#endif
