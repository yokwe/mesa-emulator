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

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("copyPaste");


#include "../util/Debug.h"

#include "../mesa/Pilot.h"
#include "../mesa/Memory.h"
#include "../mesa/MesaThread.h"

#include "Agent.h"
#include "AgentStream.h"
#include "StreamCopyPaste.h"


//stream � OpenStream[];
//PutTextData[stream, buffer];
//[problem, ] � GetAck[stream, putDataAck];
//PutCommand[stream, endStream, cpMsgLen[endStream]];
//Stream.SendNow[stream, TRUE];
//[] � GetAck[stream, endStreamAck ! ANY => CONTINUE];
//CloseStream[stream];

//PutTextData
//put LONG[M_putData]  -- command msgID
//put LONG[msgLen]     -- command msgLen
//put LONG[S_success]  -- command status
//put LONG[F_text]     -- format
//put LONG[strLen]     -- number of byte of data, end of record
//put BLOCK[data]      -- block string, end of record

//GetAck
//get LONG[M_putDataAck]
//get LONG[msgLen]
//get LONG[success]

//PutCommand
//put LONG[M_endStream] -- command msgID
//put LONG[msgLen]      -- command msgLen
//put LONG[S_success]   -- command status

//GetAck
//get LONG[M_endStreamAck]
//get LONG[msgLen]
//get LONG[success]



//PutTextData: PROC [stream: Stream.Handle, str: LONG STRING] =
//    BEGIN
//
//    PutCommand[stream, putData, cpMsgLen[putData] + str.length];
//    PutLongNumber[stream, CP.Format.text.ORD]; -- Text format
//    PutLongNumber[stream, str.length]; -- number of bytes in data buffer
//    Stream.PutBlock[stream, [LOOPHOLE[@str.text], 0, str.length + 1], FALSE�TRUE�];
//    Stream.SendNow[stream, TRUE];
//    END; -- PutTextData

//PutCommand: PROC [stream: Stream.Handle, msgID: CP.MsgId, msgLen: LONG CARDINAL, status: CP.Status � success] =
//BEGIN
//    PutLongNumber[stream, msgID.ORD];
//    PutLongNumber[stream, msgLen]; --  total byte len in the message.
//    PutLongNumber[stream, status.ORD];
//END; -- PutCommand

StreamCopyPaste::GetBuffer::GetBuffer(CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) : transferRec(iocb->mesaPut) {
	data = (CARD8*)Memory::getAddress(transferRec.buffer);
}
CARD8 StreamCopyPaste::GetBuffer::get8() {
	if (transferRec.bytesWritten == transferRec.bytesRead) {
		logger.error("bytesWritten == bytesRead");
		ERROR();
	}
	CARD8* p = (CARD8*)data;
	return p[transferRec.bytesRead++ ^ 1];
}

CARD16 StreamCopyPaste::GetBuffer::get16() {
	if (transferRec.bytesWritten == transferRec.bytesRead) {
		logger.error("bytesWritten == bytesRead");
		ERROR();
	}
	CARD16 b1 = get8();
	CARD16 b2 = get8();
	return (b2 << 8) | (b1 << 0);
}
CARD32 StreamCopyPaste::GetBuffer::get32() {
	if (transferRec.bytesWritten == transferRec.bytesRead) {
		logger.error("bytesWritten == bytesRead");
		ERROR();
	}
	CARD32 b1 = get16();
	CARD32 b2 = get16();
	return (b2 << 16) | (b1 << 0);
}


StreamCopyPaste::PutBuffer::PutBuffer(CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) : transferRec(iocb->mesaGet) {
	data = (CARD8*)Memory::getAddress(transferRec.buffer);
}
void StreamCopyPaste::PutBuffer::PutBuffer::put8(CARD8 value) {
	if (transferRec.bytesWritten == transferRec.bufferSize) {
		logger.error("bytesWritten == bufferSize");
		ERROR();
	}
	data[transferRec.bytesWritten++ ^ 1] = value;
}
void StreamCopyPaste::PutBuffer::put16(CARD16 value) {
	if (transferRec.bytesWritten == transferRec.bufferSize) {
		logger.error("bytesWritten == bufferSize");
		ERROR();
	}
	CARD8 b1 = (CARD8)value;
	CARD8 b2 = (CARD8)(value >> 8);
	put8(b1);
	put8(b2);
}
void StreamCopyPaste::PutBuffer::put32(CARD32 value) {
	if (transferRec.bytesWritten == transferRec.bufferSize) {
		logger.error("bytesWritten == bufferSize");
		ERROR();
	}
	CARD16 b1 = (CARD16)value;
	CARD16 b2 = (CARD16)(value >> 16);
	put16(b1);
	put16(b2);
}

StreamCopyPaste::StreamCopyPaste() : Stream("CopyPaste", CoProcessorServerIDs::copyPaste) {
	logger.info("%3d %-8s", serverID, name.toLatin1().constData());
}

quint16 StreamCopyPaste::idle   (CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) {
	logger.error("%-8s idle %d %d", name.toLatin1().constData(), fcb->headCommand, iocb->serverID);
	return CoProcessorIOFaceGuam::R_completed;
}
quint16 StreamCopyPaste::accept (CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) {
	logger.error("%-8s accept %d %d", name.toLatin1().constData(), fcb->headCommand, iocb->serverID);
	ERROR();
	return CoProcessorIOFaceGuam::R_error;
}
quint16 StreamCopyPaste::connect(CoProcessorIOFaceGuam::CoProcessorFCBType * /*fcb*/, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) {
	logger.info("%-8s connect  mesaIsServer = %d  state mesa = %d  pc = %d", name.toLatin1().constData(), iocb->mesaIsServer, iocb->mesaConnectionState, iocb->pcConnectionState);

	iocb->pcConnectionState = CoProcessorIOFaceGuam::S_connected;
	// Need to assign non-zero to mesaGet.hTaskactually. See CoProcessorFace.Get
	iocb->mesaGet.hTask = 1;

//	return CoProcessorIOFaceGuam::R_completed;
	return CoProcessorIOFaceGuam::R_error;
}
quint16 StreamCopyPaste::destroy(CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) {
	logger.info("%-8s destroy %d %d", name.toLatin1().constData(), fcb->headCommand, iocb->serverID);
	return CoProcessorIOFaceGuam::R_error;
}
quint16 StreamCopyPaste::read   (CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) {
	logger.error("%-8s read %d %d", name.toLatin1().constData(), fcb->headCommand, iocb->serverID);
	{
		CoProcessorIOFaceGuam::TransferRec& tr = iocb->mesaPut;
		logger.info("mesaPut  sst: %d  end [Stream: %d  Record: %d  SST: %d]  written: %3d  read: %3d  hTask: %d  int: %d  buffer: %4X  bufferSize: %3d  lock: %d",
			tr.subSequence, tr.endStream, tr.endRecord, tr.endSST,
			tr.bytesWritten, tr.bytesRead, tr.hTask, tr.interruptMesa, tr.buffer, tr.bufferSize, tr.writeLockedByMesa);
	}
	{
		CoProcessorIOFaceGuam::TransferRec& tr = iocb->mesaGet;
		logger.info("mesaGet  sst: %d  end [Stream: %d  Record: %d  SST: %d]  written: %3d  read: %3d  hTask: %d  int: %d  buffer: %4X  bufferSize: %3d  lock: %d",
			tr.subSequence, tr.endStream, tr.endRecord, tr.endSST,
			tr.bytesWritten, tr.bytesRead, tr.hTask, tr.interruptMesa, tr.buffer, tr.bufferSize, tr.writeLockedByMesa);
	}
	return CoProcessorIOFaceGuam::R_error;
}
quint16 StreamCopyPaste::write  (CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) {
	logger.error("%-8s write %d %d", name.toLatin1().constData(), fcb->headCommand, iocb->serverID);

	GetBuffer buffer(iocb);
	{
		CoProcessorIOFaceGuam::TransferRec& tr = iocb->mesaPut;
		logger.info("mesaPut  sst: %d  end [Stream: %d  Record: %d  SST: %d]  written: %3d  read: %3d  hTask: %d  int: %d  buffer: %4X  bufferSize: %3d  lock: %d",
			tr.subSequence, tr.endStream, tr.endRecord, tr.endSST,
			tr.bytesWritten, tr.bytesRead, tr.hTask, tr.interruptMesa, tr.buffer, tr.bufferSize, tr.writeLockedByMesa);

		CARD32 msgID  = buffer.get32();
		CARD32 len    = buffer.get32();
		CARD32 status = buffer.get32();

		logger.info("get  %d  %d  %d", msgID, len, status);

		switch(msgID) {
		case M_putData: {
			//
			logger.info("putData %d -- %d", len, status);
			return CoProcessorIOFaceGuam::R_error;
		}
			break;
		default: {
			//
			logger.info("Unknown msgID %d", msgID);
			return CoProcessorIOFaceGuam::R_error;
		}
		}
	}
}
