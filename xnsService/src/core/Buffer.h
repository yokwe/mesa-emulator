//
// Buffer.h
//

#ifndef BUFFER_H__
#define BUFFER_H__

#include "../util/Util.h"

class Buffer {
protected:
	static const quint32 MAX_SIZE = 1514;
	static const quint32 MIN_SIZE =   46;
	quint32 size;
	quint8  data[MAX_SIZE];

	quint64 get48(quint32 offset);
	quint32 get32(quint32 offset);
	quint16 get16(quint32 offset);
	quint8  get8 (quint32 offset);

	void set48(quint32 offset, quint64 value);
	void set32(quint32 offset, quint32 value);
	void set16(quint32 offset, quint16 value);
	void set8 (quint32 offset, quint8  value);

public:
	Buffer(quint8* data_, quint32 size_) {
		//if (MAX_DATA_SIZE < size) ERROR();
		size = size_;
		for(quint32 i = 0; i < size; i++) data[i] = data_[i];
	}
};

class EthernetBuffer : public Buffer {
public:
	static const quint32 OFFSET_DEST   =  0;
	static const quint32 OFFSET_SOURCE =  6;
	static const quint32 OFFSET_TYPE   = 12;
	static const quint32 SIZE          = 14;

	EthernetBuffer(quint8* data_, quint32 size_) : Buffer(data_, size_) {}
	quint64 getDest() {
		return get48(OFFSET_DEST);
	}
	quint64 getSource() {
		return get48(OFFSET_SOURCE);
	}
	quint16 getType() {
		return get16(OFFSET_TYPE);
	}
};

class NetworkAddress {
public:
	static const quint32 OFFSET_NETWORK =  0;
	static const quint32 OFFSET_HOST    =  4;
	static const quint32 OFFSET_SOCKET  = 10;
	static const quint32 SIZE           = 12;

	quint32 network;
	quint64 host;
	quint16 socket;
};

class IDPBuffer : public EthernetBuffer {
public:
	static const quint32 OFFSET_CHECKSUM   =  0;
	static const quint32 OFFSET_LENGTH     =  2;
	static const quint32 OFFSET_HOP        =  4;
	static const quint32 OFFSET_TYPE       =  5;
	static const quint32 OFFSET_NET_DEST   =  6;
	static const quint32 OFFSET_NET_SOURCE = 18;
	static const quint32 SIZE              = 30;

	IDPBuffer(quint8* data_, quint32 size_) : EthernetBuffer(data_, size_) {}


	quint16 getChecksum() {
		return get16(EthernetBuffer::SIZE + OFFSET_CHECKSUM);
	}
	quint16 getLength() {
		return get16(EthernetBuffer::SIZE + OFFSET_LENGTH);
	}
	quint8  getHop() {
		return get8(EthernetBuffer::SIZE + OFFSET_HOP);
	}
	quint8  getType() {
		return get8(EthernetBuffer::SIZE + OFFSET_TYPE);
	}
	quint32 getDNetwork() {
		return get32(EthernetBuffer::SIZE + OFFSET_NET_DEST + NetworkAddress::OFFSET_NETWORK);
	}
	quint64 getDHost() {
		return get48(EthernetBuffer::SIZE + OFFSET_NET_DEST + NetworkAddress::OFFSET_HOST);
	}
	quint16 getDSocket() {
		return get16(EthernetBuffer::SIZE + OFFSET_NET_DEST + NetworkAddress::OFFSET_SOCKET);
	}
	quint32 getSNetwork() {
		return get32(EthernetBuffer::SIZE + OFFSET_NET_SOURCE + NetworkAddress::OFFSET_NETWORK);
	}
	quint64 getSHost() {
		return get48(EthernetBuffer::SIZE + OFFSET_NET_SOURCE + NetworkAddress::OFFSET_HOST);
	}
	quint16 getSSocket() {
		return get16(EthernetBuffer::SIZE + OFFSET_NET_SOURCE + NetworkAddress::OFFSET_SOCKET);
	}
};
#endif
