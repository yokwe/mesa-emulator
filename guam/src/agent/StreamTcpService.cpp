/*
Copyright (c) 2014, Yasuhiro Hasegawa
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
// StreamTcpService.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("tcpservice");


#include "../util/Debug.h"

#include "../mesa/Pilot.h"

#include "StreamTcpService.h"

#define DEBUG_SHOW_STREAM_TCP_SERVICE 1

StreamTcpService::StreamTcpService() : AgentStream::Stream(CoProcessorServerIDs::tcpService, "TcpService") {}

CARD16 StreamTcpService::process(CoProcessorIOFaceGuam::CoProcessorFCBType* fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) {
	if (DEBUG_SHOW_STREAM_TCP_SERVICE) {
		logger.debug("    serverID = %-11s  mesaIsServer = %d  mesaState = %10s  pcState = %10s  next = %8X",
				getServerIDString(iocb->serverID), iocb->mesaIsServer, getStateString(iocb->mesaConnectionState), getStateString(iocb->pcConnectionState), iocb->nextIOCB);
		logger.debug("    %-8s  %-8s  %6X %6X", getCommandString(fcb->headCommand), getResultString(fcb->headResult), fcb->iocbHead, fcb->iocbNext);

		logger.debug("        put  sst = %3d  u2 = %02X  bytesWritten = %4d  bytesRead = %4d  hTask = %d  interruptMesa = %d  writeLockedByMesa = %d",
				iocb->mesaPut.subSequence, iocb->mesaPut.u2, iocb->mesaPut.bytesWritten, iocb->mesaPut.bytesRead, iocb->mesaPut.hTask, iocb->mesaPut.interruptMesa, iocb->mesaPut.writeLockedByMesa);
		logger.debug("        get  sst = %3d  u2 = %02X  bytesWritten = %4d  bytesRead = %4d  hTask = %d  interruptMesa = %d  writeLockedByMesa = %d",
				iocb->mesaGet.subSequence, iocb->mesaGet.u2, iocb->mesaGet.bytesWritten, iocb->mesaGet.bytesRead, iocb->mesaGet.hTask, iocb->mesaGet.interruptMesa, iocb->mesaGet.writeLockedByMesa);
	//	logger.debug("        -----");
	//	logger.debug("        put  subSequence       %6X", iocb->mesaPut.subSequence);
	//	logger.debug("        put  u2                %6X", iocb->mesaPut.u2);
	//	logger.debug("        put  bytesWritten      %6X", iocb->mesaPut.bytesWritten);
	//	logger.debug("        put  bytesRead         %6X", iocb->mesaPut.bytesRead);
	//	logger.debug("        put  hTask             %6X", iocb->mesaPut.hTask);
	//	logger.debug("        put  interruptMesa     %6X", iocb->mesaPut.interruptMesa);
	//	logger.debug("        put  buffer            %6X", iocb->mesaPut.buffer);
	//	logger.debug("        put  bufferSize        %6X", iocb->mesaPut.bufferSize);
	//	logger.debug("        put  writeLockedByMesa %6X", iocb->mesaPut.writeLockedByMesa);
	//	logger.debug("        -----");
	//	logger.debug("        get  subSequence       %6X", iocb->mesaGet.subSequence);
	//	logger.debug("        get  u2                %6X", iocb->mesaGet.u2);
	//	logger.debug("        get  bytesWritten      %6X", iocb->mesaGet.bytesWritten);
	//	logger.debug("        get  bytesRead         %6X", iocb->mesaGet.bytesRead);
	//	logger.debug("        get  hTask             %6X", iocb->mesaGet.hTask);
	//	logger.debug("        get  interruptMesa     %6X", iocb->mesaGet.interruptMesa);
	//	logger.debug("        get  buffer            %6X", iocb->mesaGet.buffer);
	//	logger.debug("        get  bufferSize        %6X", iocb->mesaGet.bufferSize);
	//	logger.debug("        get  writeLockedByMesa %6X", iocb->mesaGet.writeLockedByMesa);
	}
	return CoProcessorIOFaceGuam::R_error;
}
