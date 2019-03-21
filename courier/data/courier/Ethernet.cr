Ethernet: PROGRAM 0 VERSION 0 =
BEGIN

HostAddress: TYPE = UNSPECIFIED3;
FrameType:   TYPE = UNSPECIFIED;

Header: TYPE = RECORD [
    destination: HostAddress,
    source:      HostAddress,
    type:        FrameType
    -- data will follow after header
];

broadcastAddress: HostAddress = 0FFFFFFFFFFFFX;
frameTypeIDP:     FrameType   = 0600X;

-- length of header
headerLength: CARDINAL = 14; -- 14 = 6 + 6 + 2

-- minimum length of data (exclude Header)
minDataLength: CARDINAL = 46;
maxDataLength: CARDINAL = 1500;

END.