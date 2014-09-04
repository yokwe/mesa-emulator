#include <stdio.h>

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("test");

#include "../util/Debug.h"

#include "../core/Network.h"
#include "../core/Buffer.h"

int main(int /*argc*/, char** /*argv*/) {
	logger.info("START");

	Network network;

	network.attach("eth1");

	quint16 a;
	quint16 b;
	quint16 c;
	network.getAddress(a, b, c);

	logger.info("%04X-%04X-%04X", a, b, c);

	//networkPacket.discardRecievedPacket();

	for(;;) {
		int opErrno = 0;
		int ret = network.select(1, opErrno);
		if (ret == 0) continue;

		quint8 data[1600];
		ret = network.receive(data, sizeof(data), opErrno);

		IDPBuffer idp(data, ret);

		logger.info("ETHER  %012llX  %012llX  %04X", idp.getDest(), idp.getSource(), ((EthernetBuffer)idp).getType());

		logger.info("%04X  %04X %02X %02X %08X-%012llX-%04X  %08X-%012llX-%04X",  idp.getChecksum(), idp.getLength(), idp.getHop(), idp.getType(),
				idp.getDNetwork(), idp.getDHost(), idp.getDSocket(), idp.getSNetwork(), idp.getSHost(), idp.getSSocket());
	}

	network.detach();

	//
	logger.info("STOP");
	return 0;
}
