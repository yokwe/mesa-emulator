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
    DATA         (0),
    BULK         (1),
    CLOSE      (254),
    CLOSE_REPLY(255)
} OF BYTE;

Header: TYPE = RECORD [
    Control:     control,
    Type:        type,
    control:     UNSPECIFIED,
    source:      ConnectionID,
    destination: ConnectionID,
    sequence:    SequenceNumber,
    acknowledge: SequenceNumber,
    allocation:  SequenceNumber
    -- Data is stored after allocation
];

-- mask for control
maskSystemPacket:        UNSPECIFIED = 08000X;
maskSendAcknowledgement: UNSPECIFIED = 04000X;
maskAttention:           UNSPECIFIED = 02000X;
maskEndOfMessage:        UNSPECIFIED = 01000X;
maskDatastreamType:      UNSPECIFIED = 000FFX;

-- datastreamType AKA SubSystemType - SST
-- from Courier/Friends/CourierProtocol.mesa
dataSST:       UNSPECIFIED =   0; --SST used for all Courier messages
-- from Courier/Private/BulkData.mesa
bulkSST:       UNSPECIFIED =   1; -- SST used for bulk data
-- from NS/Public/NetworkStream.mesa
closeSST:      UNSPECIFIED = 254; -- SST used for closing connection
closeReplySST: UNSPECIFIED = 255; -- SST used for rely of closing connection (handshake)

sstCourier:    UNSPECIFIED =   0; -- alias of dataSST
sstBulk:       UNSPECIFIED =   1; -- alias of bulkSST
sstClose:      UNSPECIFIED = 254; -- alias of closeSST
sstCloseReply: UNSPECIFIED = 255; -- alias of closeReplySST

END.