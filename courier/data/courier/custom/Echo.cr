Echo: PROGRAM 0 VERSION 0 =
BEGIN

Type: TYPE = {
    REQUEST (1),
    RESPONSE(2)
};

Frame: TYPE = RECORD [
    type: Type,
    -- data will follow after header
    data: BLOCK
];

END.