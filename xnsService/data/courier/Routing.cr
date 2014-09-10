Routing: PROGRAM 0 VERSION 0 =
BEGIN
    DEPENDS UPON Datagram(0) VERSION 0;

Operation: TYPE = {
    request(1),
    response(2)
};

Hop: TYPE = CARDINAL;

Tuple: TYPE = RECORD [
    network: Datagram.Network,
    hop:     Hop];

Header: TYPE = RECORD [
    operation: Operation
    -- Tupple are stored after operation until end of packet
];

-- infinity: Hop = 16;
-- The word "infinity" make conflict in C source code.
-- So use word "maxHop" instead of "infinity"
maxHop: Hop = 16;

tupleLength: CARDINAL = 6; -- length of Tuple in byte

END.
