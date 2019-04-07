PacketExchange: PROGRAM 0 VERSION 0 =
BEGIN

ClientType: TYPE = {unspecified(0), time(1), clearinghouse(2), teledebug(8)};

Header: TYPE = RECORD [
    id:         UNSPECIFIED2,
    clientType: ClientType
    -- Data is stored after clientType
];

END.