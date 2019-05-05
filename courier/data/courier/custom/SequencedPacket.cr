SequencedPacket: PROGRAM 0 VERSION 0 =
BEGIN

ConnectionID:   TYPE = UNSPECIFIED;
SequenceNumber: TYPE = UNSPECIFIED;

Control: TYPE = MACHINE DEPENDENT [
    system     (0..0): BOOLEAN,
    sendAck    (1..1): BOOLEAN,
    attention  (2..2): BOOLEAN,
    endMessage (3..3): BOOLEAN,
    reserve    (4..7): UNSPECIFIED
] OF BYTE;

Type: TYPE = {
    DATA         (0), -- SST used for all Courier messages
    BULK         (1), -- SST used for bulk data
    CLOSE      (254), -- SST used for closing connection
    CLOSE_REPLY(255)  -- SST used for reply of closing connection (handshake)
} OF BYTE;

Frame: TYPE = RECORD [
    control: Control,
    type:    Type,
    src:     ConnectionID,
    dst:     ConnectionID,
    seq:     SequenceNumber,
    ack:     SequenceNumber,
    alloc:   SequenceNumber,
    -- data will follow after header
    data:    BLOCK
];

END.