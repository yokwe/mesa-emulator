Error: PROGRAM 0 VERSION 0 =
BEGIN

Type: TYPE = {
    -- An unspecified error is detected at destination
    UNSPECIFIED_ERROR  (0),
    -- The checksum is incorrect or the packet has some other serious inconsistency detected at destination
    BAD_CHECKSUM       (1),
    -- The specified socket does not exist at the specified destination host.
    NO_SOCKET          (2),
    -- The destination cannot accept the packet due to resource limitations.
    RESOURCE_LIMIT     (3),
    LISTENER_REJCR     (4),
    INVALID_PACKET_TYPE(5),
    PROTOCOL_VIOLATION (6)
};


Frame: TYPE = RECORD [
    type:      Type,
    parameter: UNSPECIFIED,
    -- data will follow after header
    data:       BLOCK
];

DATA_LENGTH_MIN: CARDINAL = 42;

END.