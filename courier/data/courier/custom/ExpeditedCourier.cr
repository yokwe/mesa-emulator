ExpeditedCourier: PROGRAM 0 VERSION 0 =
BEGIN
    DEPENDS UPON Protocol(0) VERSION 0;

Header: TYPE = RECORD [
    protocolRange: Protocol.ProtocolRange,
    message:       Protocol.Protocol3
];

END.