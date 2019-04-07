Echo: PROGRAM 0 VERSION 0 =
BEGIN

Operation: TYPE = {
    request(1),
    response(2)
};

Header: TYPE = RECORD [
    operation: Operation
    -- Echo data is stored after operation
];

END.