/*
Copyright (c) 2017, Yasuhiro Hasegawa
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
  3. The name of the author may not be used to endorse or promote products derived
     from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/


//
// StreamCopyPaste.cpp
//

#ifndef STREAMCOPYPASTE_H__
#define STREAMCOPYPASTE_H__

#include "../mesa/Pilot.h"

#include "AgentStream.h"


class StreamCopyPaste : public AgentStream::Stream {
public:
	StreamCopyPaste();
	quint16 idle   (CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb);
	quint16 accept (CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb);
	quint16 connect(CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb);
	quint16 destroy(CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb);
	quint16 read   (CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb);
	quint16 write  (CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb);

private:
	// MsgId: TYPE = MACHINE DEPENDENT {
	//  nop(0), invalidCmdAck(1), getProtocolVer(2), getProtocolVerAck(3), getData(4),
	//  getDataAck(5), putData(6), putDataAck(7), endStream(8), endStreamAck(9), lastCpMsgid(10), maxCpMsgid(11)};
	static const CARD32 M_nop               = 0;
	static const CARD32 M_invalidCmdAck     = 1;
	static const CARD32 M_getProtocolVer    = 2;
	static const CARD32 M_getProtocolVerAck = 3;
	static const CARD32 M_getData           = 4;
	static const CARD32 M_getDataAck        = 5;
	static const CARD32 M_putData           = 6;
	static const CARD32 M_putDataAck        = 7;
	static const CARD32 M_endStream         = 8;
	static const CARD32 M_endStreamAck      = 9;
	static const CARD32 M_lastCpMsgid       = 10;
	static const CARD32 M_maxCpMsgid        = 11;

	// Status: TYPE = MACHINE DEPENDENT{
	//   success(0), generalFailure(1), invalidCmd(2), outOfMemory(3), dataExceedRequest(4),
	//   dataUnknownFormat(5), dataNone(6), cbInUse(7), lastMsg(8)};
	static const CARD32 S_success           = 0;
	static const CARD32 S_generalFailure    = 1;
	static const CARD32 S_invalidCmd        = 2;
	static const CARD32 S_outOfMemory       = 3;
	static const CARD32 S_dataExceedRequest = 4;
	static const CARD32 S_dataUnknownFormat = 5;
	static const CARD32 S_dataNone          = 6;
	static const CARD32 S_cbInUse           = 7;
	static const CARD32 S_lastMsg           = 8;

	// Format: TYPE = MACHINE DEPENDENT{none(0), text(1), bitmap(2)};
	static const CARD32 F_none   = 0;
	static const CARD32 F_text   = 1;
	static const CARD32 F_bitmap = 2;

	class GetBuffer {
	public:
		GetBuffer(CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb);

		CARD8  get8();
		CARD16 get16();
		CARD32 get32();

		CARD32 remain() {
			return transferRec.bytesWritten - transferRec.bytesRead;
		}

	private:
		CoProcessorIOFaceGuam::TransferRec& transferRec;
		CARD8* data;
	};

	class PutBuffer {
	public:
		PutBuffer(CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb);

		void put8 (CARD8  value);
		void put16(CARD16 value);
		void put32(CARD32 value);

		CARD32 remain();

	private:
		CoProcessorIOFaceGuam::TransferRec& transferRec;
		CARD8* data;
	};
};

#endif
