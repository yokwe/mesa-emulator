Protocol: PROGRAM 0 VERSION 0 =
BEGIN

ProtocolType:    TYPE = {protocol0(0), protocol1(1), protocol2(2), protocol3(3)};
MessageType: TYPE = {call(0), reject(1), return(2), abort(3)};

MessageObject: TYPE = CHOICE ProtocolType OF {
    protocol0 => RECORD[],
    protocol1 => RECORD[],
    protocol2 => RECORD[ protocol2Body: Protocol2Body ],
    protocol3 => RECORD[ protocol3Body: Protocol3Body ]
};

Protocol2Body: TYPE = CHOICE MessageType OF {
    call   => RECORD [transaction, program, version, procedure: CARDINAL ],
    reject => RECORD [transaction: CARDINAL, reject: RejectCode ],
    return => RECORD [transaction: CARDINAL ],
    abort  => RECORD [transaction: CARDINAL, abort: CARDINAL ]};

RejectCode: TYPE = {
    noSuchProgramNumber(0),
    noSuchVersionNumber(1),
    noSuchProcedureValue(2),
    invalidArguments(3)};

RejectBody: TYPE = CHOICE RejectCode OF {
    noSuchProgramNumber, noSuchProcedureValue, invalidArguments => RECORD [],
    noSuchVersionNumber => RECORD [ range: VersionRange]
};

Protocol3Body: TYPE = CHOICE MessageType OF {
    call   => RECORD [transaction: CARDINAL,
                      program:     LONG CARDINAL,
                      version:     CARDINAL,
                      procedure:   CARDINAL],
    reject => RECORD [transaction: CARDINAL, rejectBody:  RejectBody],
    return => RECORD [transaction: CARDINAL],
    abort  => RECORD [transaction: CARDINAL, abort: CARDINAL ]};

ProtocolRange: TYPE = RECORD [low, high: ProtocolType];
VersionRange:  TYPE = RECORD [low, high: CARDINAL];
pvLow:  ProtocolType = protocol2;
pvHigh: ProtocolType = protocol3;

END.