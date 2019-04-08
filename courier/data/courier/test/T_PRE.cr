T_PRE: PROGRAM 0 VERSION 0 =
BEGIN

-- predefined
--   BOOLEAN, BYTE, CARDINAL, LONG_CARDINAL, STRING, UNSPECIFIED, UNSPECIFIED2, UNSPECIFIED3,
-- constructed
--   ENUM, ARRAY, SEQUENCE, RECORD, CHOICE, PROCEDURE, ERROR
-- reference
--    REFERENCE

    REC_PRE01: TYPE = RECORD [v: BOOLEAN];
    REC_PRE02: TYPE = RECORD [v: BYTE];
    REC_PRE03: TYPE = RECORD [v: CARDINAL];
    REC_PRE04: TYPE = RECORD [v: LONG CARDINAL];
    REC_PRE05: TYPE = RECORD [v: STRING];
    REC_PRE06: TYPE = RECORD [v: UNSPECIFIED];
    REC_PRE07: TYPE = RECORD [v: UNSPECIFIED];
    REC_PRE08: TYPE = RECORD [v: UNSPECIFIED];

ALI01: TYPE = BOOLEAN;
ALI02: TYPE = BYTE;
ALI03: TYPE = CARDINAL;
ALI04: TYPE = LONG CARDINAL;
ALI05: TYPE = STRING
ALI06: TYPE = UNSPECIFIED;
ALI07: TYPE = UNSPECIFIED2;
ALI08: TYPE = UNSPECIFIED3;

    REC_ALI01: TYPE = RECORD [v: ALI01];
    REC_ALI02: TYPE = RECORD [v: ALI02];
    REC_ALI03: TYPE = RECORD [v: ALI03];
    REC_ALI04: TYPE = RECORD [v: ALI04];
    REC_ALI05: TYPE = RECORD [v: ALI05];
    REC_ALI06: TYPE = RECORD [v: ALI06];
    REC_ALI07: TYPE = RECORD [v: ALI07];
    REC_ALI08: TYPE = RECORD [v: ALI08];

SEQ01: TYPE = SEQUENCE 4 OF BOOLEAN;
SEQ02: TYPE = SEQUENCE 4 OF BYTE;
SEQ03: TYPE = SEQUENCE 4 OF CARDINAL;
SEQ04: TYPE = SEQUENCE 4 OF LONG CARDINAL;
SEQ05: TYPE = SEQUENCE 4 OF STRING
SEQ06: TYPE = SEQUENCE 4 OF UNSPECIFIED;
SEQ07: TYPE = SEQUENCE 4 OF UNSPECIFIED2;
SEQ08: TYPE = SEQUENCE 4 OF UNSPECIFIED3;

    REC_SEQ01: TYPE = RECORD [v: SEQ01];
    REC_SEQ02: TYPE = RECORD [v: SEQ02];
    REC_SEQ03: TYPE = RECORD [v: SEQ03];
    REC_SEQ04: TYPE = RECORD [v: SEQ04];
    REC_SEQ05: TYPE = RECORD [v: SEQ05];
    REC_SEQ06: TYPE = RECORD [v: SEQ06];
    REC_SEQ07: TYPE = RECORD [v: SEQ07];
    REC_SEQ08: TYPE = RECORD [v: SEQ08];

ARR01: TYPE = ARRAY 4 OF BOOLEAN;
ARR02: TYPE = ARRAY 4 OF BYTE;
ARR03: TYPE = ARRAY 4 OF CARDINAL;
ARR04: TYPE = ARRAY 4 OF LONG CARDINAL;
ARR05: TYPE = ARRAY 4 OF STRING
ARR06: TYPE = ARRAY 4 OF UNSPECIFIED;
ARR07: TYPE = ARRAY 4 OF UNSPECIFIED2;
ARR08: TYPE = ARRAY 4 OF UNSPECIFIED3;

    REC_ARR01: TYPE = RECORD [v: ARR01];
    REC_ARR02: TYPE = RECORD [v: ARR02];
    REC_ARR03: TYPE = RECORD [v: ARR03];
    REC_ARR04: TYPE = RECORD [v: ARR04];
    REC_ARR05: TYPE = RECORD [v: ARR05];
    REC_ARR06: TYPE = RECORD [v: ARR06];
    REC_ARR07: TYPE = RECORD [v: ARR07];
    REC_ARR08: TYPE = RECORD [v: ARR08];

END.