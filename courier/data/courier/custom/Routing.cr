Routing: PROGRAM 0 VERSION 0 =
BEGIN
    DEPENDS UPON IDP(0) VERSION 0;

Type: TYPE = {
    REQUEST (1),
    RESPONSE(2)
};

Hop: TYPE = {
    MAX(16)
};

Tuple: TYPE = RECORD [
    network: IDP.Network,
    hop:     Hop
];

Frame: TYPE = RECORD [
    type: Type,
    -- data will follow after header
    data: BLOCK
];

tupleLength: CARDINAL = 6; -- length of Tuple in byte

END.
