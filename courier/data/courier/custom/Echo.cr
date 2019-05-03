Echo: PROGRAM 0 VERSION 0 =
BEGIN

Type: TYPE = {
    request(1),
    response(2)
};

Header: TYPE = RECORD [
    type: Type
    -- Echo data is stored after Header until end of packet
];

END.