Datagram: PROGRAM 0 VERSION 0 =
BEGIN
    DEPENDS UPON Ethernet(0) VERSION 0;

-- Level 1
-- Internet Datagram Protocol

Network: TYPE = UNSPECIFIED2;
Host:    TYPE = Ethernet.HostAddress;
Socket:  TYPE = UNSPECIFIED;

NetworkAddress: TYPE = RECORD [
    network: Network,
    host:    Host,
    socket:  Socket
];

networkUnknown: Network = 0;
networkAll:     Network = 0ffffffffX;
hostAll:        Host    = 0ffffffffffffX;

Header: TYPE = RECORD [
    checksum:    CARDINAL,
    length:      CARDINAL,
    flags:       CARDINAL, -- HopCount(0:4..7) and PacketType(0:8..15)
    destination: NetworkAddress,
    source:      NetworkAddress
];

-- length of header
headerLength: CARDINAL = 30; -- 30 = 6 + 12 + 12

PacketType: TYPE = {routing(1), echo(2), error(3), packetExchange(4), sequencedPacket(5), bootServerPacket(9)};
packetTypeError: CARDINAL = 3;

checksumNone:          CARDINAL = 0FFFFX;

maskHopCount:          CARDINAL = 0FF00X;
maskPacketType:        CARDINAL = 000FFX;

maxHopCount:       CARDINAL = 16;
maxPacketLifetime: CARDINAL = 60; -- 60 seconds

-- Well known socket
WellKnownSocket: TYPE = {
    routing(1), echo(2), error(3), envoy(4), courier(5), clearinghouseOld(7), time(8),
    boot(10), diag(19),
    clearinghouse(20), auth(21), mail(22), netExec(23), wsInfo(24), binding(28),
    germ(35),
    teleDebug(48)
};

maxWellKnownSocket: CARDINAL = 3000; -- NSConstantsExtras.maxWellKnownSocket

socketRoutig:        CARDINAL = 1;
socketEcho:          CARDINAL = 2;
socketError:         CARDINAL = 3;
socketCourier:       CARDINAL = 5;
socketTime:          CARDINAL = 8;
socketBoot:          CARDINAL = 10;
socketClearinghouse: CARDINAL = 20;
socketAuth:          CARDINAL = 21;
socketBinding:       CARDINAL = 28;
socketGerm:          CARDINAL = 35;

END.