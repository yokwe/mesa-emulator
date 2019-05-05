Protocol: PROGRAM 0 VERSION 0 =
BEGIN

ProtocolType: TYPE = {
    PROTOCOL2(2),
    PROTOCOL3(3)
};
    
MessageType:  TYPE = {
    CALL  (0),
    REJECT(1),
    RETURN(2),
    ABORT (3)
};

RejectCode: TYPE = {
    NO_SUCH_PROGRAM_NUMBER  (0),
    NO_SUCH_VERSION_NUMBER  (1),
    NO_SUCH_PROCEDURE_NUMBER(2),
    INALID_ARGUMENTS        (3)
};

Protocol2: TYPE = CHOICE MessageType OF {
    CALL   => RECORD [
                  transaction: CARDINAL,
                  program:     CARDINAL,
                  version:     CARDINAL,
                  procedure:   CARDINAL,
                  data:        BLOCK],
    RETURN => RECORD [
                  transaction: CARDINAL,
                  data:        BLOCK],
    ABORT  => RECORD [
                  transaction: CARDINAL,
                  abortCode:   CARDINAL,
                  data:        BLOCK],
    REJECT => RECORD [
                  transaction: CARDINAL,
                  rejectCode:  RejectCode]
};

ProtocolRange: TYPE = RECORD [low, high: ProtocolType];
VersionRange:  TYPE = RECORD [low, high: CARDINAL];
pvLow:  ProtocolType = PROTOCOL2;
pvHigh: ProtocolType = PROTOCOL3;

Reject: TYPE = CHOICE RejectCode OF {
    NO_SUCH_PROGRAM_NUMBER,
    NO_SUCH_PROCEDURE_NUMBER,
    INALID_ARGUMENTS       => RECORD [],
    NO_SUCH_VERSION_NUMBER => RECORD [range: VersionRange]
};

Protocol3: TYPE = CHOICE MessageType OF {
    CALL   => RECORD [
                  transaction: CARDINAL,
                  program:     LONG CARDINAL,
                  version:     CARDINAL,
                  procedure:   CARDINAL,
                  data:        BLOCK],
    RETURN => RECORD [
                  transaction: CARDINAL,
                  data:        BLOCK],
    ABORT  => RECORD [
                  transaction: CARDINAL,
                  abortCode:   CARDINAL,
                  data:        BLOCK],
    REJECT => RECORD [
                  transaction: CARDINAL,
                  reject:      Reject]
};

Message: TYPE = CHOICE ProtocolType OF {
    PROTOCOL2 => RECORD [protocol2: Protocol2],
    PROTOCOL3 => RECORD [protocol3: Protocol3]
};

END.
