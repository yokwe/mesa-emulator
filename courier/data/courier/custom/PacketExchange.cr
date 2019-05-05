PacketExchange: PROGRAM 0 VERSION 0 =
BEGIN

Type: TYPE = {
    UNSPEC     (0), -- UNSPECIFIED is key word. Use UNSPEC instead.
    TIME       (1),
    CHS        (2),
    TELE_DEBUG (3)
};

Frame: TYPE = RECORD [
    id:   UNSPECIFIED2,
    type: Type,
    -- data will follow after header
    data: BLOCK
];

END.