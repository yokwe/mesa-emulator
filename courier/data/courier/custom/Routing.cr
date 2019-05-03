Routing: PROGRAM 0 VERSION 0 =
BEGIN
    DEPENDS UPON IDP(0) VERSION 0;

Type: TYPE = {
    request(1),
    response(2)
};

Hop: TYPE = {
    MAX(16)
};

Tuple: TYPE = RECORD [
    network: IDP.Network,
    hop:     Hop];

Header: TYPE = RECORD [
    type: Type
    -- Tuple are stored after Header until end of packet
];

tupleLength: CARDINAL = 6; -- length of Tuple in byte

END.
