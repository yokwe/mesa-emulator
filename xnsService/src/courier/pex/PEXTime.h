//
// PEXTime.h
//

#ifndef PEXTIME_H__
#define PEXTIME_H__

#include "../socket/SocketPEX.h"
#include "../stub/StubTime2.h"

class PEXTime : public SocketPEX::PEX {
public:
	PEXTime() : SocketPEX::PEX("Time") {}

	void process(Context& context, ByteBuffer& request, ByteBuffer& response);
};

#endif
