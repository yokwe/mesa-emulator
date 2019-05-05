ExpeditedCourier: PROGRAM 0 VERSION 0 =
BEGIN
    DEPENDS UPON Protocol(0) VERSION 0;

Frame: TYPE = RECORD [
    protocolRange: Protocol.ProtocolRange,
    message:       Protocol.Protocol3,
    -- data will follow after header
    data:          BLOCK
];

END.