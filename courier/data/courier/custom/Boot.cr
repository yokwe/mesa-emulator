Boot: PROGRAM 0 VERSION 0 =
BEGIN
  DEPENDS UPON
    Datagram(0) VERSION 0,
    SequencedPacket(0) VERSION 0;

  EtherBootPacketType: TYPE = {simpleRequest(1), simpleData(2), sppRequest(3)};
  
  EthernetBootFileNumber: TYPE = Datagram.Host;
  
  BootFileRequest: TYPE = CHOICE EtherBootPacketType OF {
    simpleRequest => RECORD [bootFileNumber: EthernetBootFileNumber],
    simpleData => RECORD [
      bootFileNumber: EthernetBootFileNumber,
      packetNumber: CARDINAL],  -- starts with 1
      -- data: ARRAY [0..0) OF WORD
      -- one page of data normally, no data means end of file.
    sppRequest => RECORD [
      bootFileNumber: EthernetBootFileNumber,
      connectionID: SequencedPacket.ConnectionID]
  };

END.