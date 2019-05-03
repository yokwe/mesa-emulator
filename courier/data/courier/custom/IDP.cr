IDP: PROGRAM 0 VERSION 0 =
BEGIN
    DEPENDS UPON Ethernet(0) VERSION 0;

-- Level 1
-- Internet Datagram Protocol
Network: TYPE = {
    UNKNOWN(0),
    ALL(0FFFFFFFFX)
} OF UNSPECIFIED2;

Host: TYPE = Ethernet.Host;

Socket: TYPE = {
    RIP        (1),
    ECHO       (2),
    ERR        (3), -- ERROR is keyword. Use ERR instead
    ENVOY      (4),
    COURIER    (5),
    CHS_OLD    (7),
    TIME       (8),
    BOOT      (10),
    DIAG      (19),
    CHS       (20),
    AUTH      (21),
    MAIL      (22),
    NET_EXEC  (23),
    WS_INFO   (24),
    BINDING   (28),
    GERM      (35),
    TELEDEBUG (48)
} OF UNSPECIFIED;

NetworkAddress: TYPE = RECORD [
    network: Network,
    host:    Host,
    socket:  Socket
];

Checksum: TYPE = {
    NONE(0FFFFX)
} OF UNSPECIFIED;

Control: TYPE = MACHINE DEPENDENT [
    trace (0..0): BOOLEAN,
    filler(1..3): CARDINAL,
    hop   (4..7): CARDINAL
] OF BYTE;

Type: TYPE = {
    RI   (1), -- Routing Information
    ECHO (2), -- Echo
    ERR  (3), -- ERROR is keyword. Use ERR instead
    PE   (4), -- Packet Exchange
    SP   (5), -- Sequenced Packet
    BOOT (9)  -- BOOT Server
} OF BYTE;

Header: TYPE = RECORD [
    checksum: Checksum,
    length:   CARDINAL,
    control:  Control,
    type:     Type,
    dst:      NetworkAddress,
    src:      NetworkAddress
];

-- length of header
HEADER_LENGTH: CARDINAL = 30; -- 30 = 6 + 12 + 12

DATA_LENGTH_MIN: CARDINAL =   0;
DATA_LENGTH_MAX: CARDINAL = 546;

MAX_PACKET_LIFETIME: CARDINAL = 60; -- 60 seconds

END.