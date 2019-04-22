Protocol: PROGRAM 0 VERSION 0 =
BEGIN

ProtocolType: TYPE = {protocol2(2), protocol3(3)};
MessageType:  TYPE = {call(0), reject(1), return(2), abort(3)};

RejectCode: TYPE = {
    noSuchProgramNumber(0),
    noSuchVersionNumber(1),
    noSuchProcedureValue(2),
    invalidArguments(3)};

Protocol2: TYPE = CHOICE MessageType OF {
    call   => RECORD [transaction, program, version, procedure: CARDINAL],
    reject => RECORD [transaction: CARDINAL, rejectCode: RejectCode],
    return => RECORD [transaction: CARDINAL],
    abort  => RECORD [transaction: CARDINAL, abort: CARDINAL]};

ProtocolRange: TYPE = RECORD [low, high: ProtocolType];
VersionRange:  TYPE = RECORD [low, high: CARDINAL];
pvLow:  ProtocolType = protocol2;
pvHigh: ProtocolType = protocol3;

Reject: TYPE = CHOICE RejectCode OF {
    noSuchProgramNumber,
    noSuchProcedureValue,
    invalidArguments    => RECORD [],
    noSuchVersionNumber => RECORD [range: VersionRange]
};

Protocol3: TYPE = CHOICE MessageType OF {
    call   => RECORD [transaction: CARDINAL,
                      program:     LONG CARDINAL,
                      version:     CARDINAL,
                      procedure:   CARDINAL],
    reject => RECORD [transaction: CARDINAL, reject:  Reject],
    return => RECORD [transaction: CARDINAL],
    abort  => RECORD [transaction: CARDINAL, abort: CARDINAL]};

Message: TYPE = CHOICE ProtocolType OF {
    protocol2 => RECORD[ protocol2: Protocol2],
    protocol3 => RECORD[ protocol3: Protocol3]
};

END.