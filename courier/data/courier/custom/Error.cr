Error: PROGRAM 0 VERSION 0 =
BEGIN

ErrorNumber: TYPE = {
    -- An unspecified error is detected at destination
    unspecified(0),
    -- The checksum is incorrect or the packet has some other serious inconsistency detected at destination
    badChecksum(1),
    -- The specified socket does not exist at the specified destination host.
    noSocket(2),
    -- The destination cannot accept the packet due to resource limitations.
    resourceLimit(3),
    listenReject(4),
    invalidPacketType(5),
    protocolViolation(6),
    -- An unspecified error occurred before reaching destination.
    unspecifiedInRoute(01000B),
    
    -- The checksum is incorrect, or the packet has some other serious inconsistency before reaching destination.
    inconsistent(01001B),
    -- The destination host cannot be reached from here.
    cantGetThere(01002B),
    -- The packet has passed through 15 internet routes without reaching its destination.
    excessHops(01003B),
    -- The packet is too large to be forwarded through some intermediate network.
    -- The Error Parameter field contains the length of the largest packet that can be accommodated.
    tooBig(01004B),
    congestionWarning(01005B),
    congestionDiscard(01006B)
};


Header: TYPE = RECORD [
    number:    ErrorNumber,
    parameter: UNSPECIFIED
    -- Copy of datagram header and data of offending packet (minimum 42 bytes to include SPP header)
];

headerLength:  CARDINAL =  4; -- in byte
minDataLength: CARDINAL = 42;

END.