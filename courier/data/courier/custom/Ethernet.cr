Ethernet: PROGRAM 0 VERSION 0 =
BEGIN

Host: TYPE = {
    ALL(0FFFFFFFFFFFFX)
} OF UNSPECIFIED3;

Type: TYPE = {
    PUP(0200X),
    IDP(0600X),
    IP4(0800X)
} OF UNSPECIFIED;

Frame: TYPE = RECORD [
    dst:  Host,
    src:  Host,
    type: Type,
    -- data will follow after header
    data: BLOCK
];

-- length of header
HEADER_LENGTH: CARDINAL = 14; -- 14 = 6 + 6 + 2

-- minimum length of data (exclude Header)
DATA_LENGTH_MIN: CARDINAL = 46;
DATA_LENGTH_MAX: CARDINAL = 1500;

FRAME_LENGTH_MIN: CARDINAL =   60; -- HEADER_LENGTH + DATA_LENGTH_MIN;
FRAME_LENGTH_MAX: CARDINAL = 1514; -- HEADER_LENGTH + DATA_LENGTH_MAX;

END.