ExpeditedCourier: PROGRAM 0 VERSION 0 =
BEGIN
    DEPENDS UPON Protocol(0) VERSION 0;

Header: TYPE = RECORD [
    protRange: Protocol.ProtocolRange,
    body:      Protocol.Protocol3Body
];

END.