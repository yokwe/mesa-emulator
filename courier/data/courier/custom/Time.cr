Time: PROGRAM 0 VERSION 0 =
BEGIN
        -- number of seconds since 12:00:00 AM, 1 Jan. 1901, GMT --
        Time: TYPE = LONG CARDINAL;

        -- Thurs., 12:00:00AM, 1 Jan. 1968 --
        earliestTime: Time = 2114294400;

Type:            TYPE = {REQUEST(1), RESPONSE(2)};
OffsetDirection: TYPE = {WEST(0), EAST(1)};
ToleranceType:   TYPE = {UNKNOWN (0), IN_MILLI_SECONDS(1)};


Version: TYPE = CARDINAL;
version: Version = 2;

PacketData: TYPE = CHOICE Type OF {
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
