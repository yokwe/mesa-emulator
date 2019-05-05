Time: PROGRAM 0 VERSION 0 =
BEGIN
        -- number of seconds since 12:00:00 AM, 1 Jan. 1901, GMT --
        Time: TYPE = LONG CARDINAL;

        -- Thurs., 12:00:00AM, 1 Jan. 1968 --
        earliestTime: Time = 2114294400;

PacketType:      TYPE = {request(1), response(2)};
OffsetDirection: TYPE = {west(0), east(1)};
ToleranceType:   TYPE = {unknown(0), inMilliSeconds(1)};

Version: TYPE = CARDINAL;
version: Version = 2;

PacketData: TYPE = CHOICE PacketType OF {
    REQUEST  => RECORD [],
    RESPONSE => RECORD [
                    currentTime:     LONG CARDINAL,
                    offsetDirection: OffsetDirection,
                    offsetHours:     CARDINAL,
                    offsetMinutes:   CARDINAL,
                    startOfDST:      CARDINAL,
                    endOfDST:        CARDINAL,
                    toleranceType:   ToleranceType,
                    tolerance:       LONG CARDINAL]};

Packet: TYPE = RECORD [
    version: Version, -- must be 2
    data:    PacketData];

END.
