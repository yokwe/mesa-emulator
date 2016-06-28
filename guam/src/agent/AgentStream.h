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
// AgentStream.h
//

#ifndef AGENTSTREAM_H__
#define AGENTSTREAM_H__

#include "Agent.h"
#include <QtCore>


class AgentStream : public Agent {
public:
	class Stream {
	public:
		const QString name;
		const quint32 serverID;

		Stream(const QString name_, const quint32 serverID_) : name(name_), serverID(serverID_) {}
		virtual ~Stream() {}

		// provide method for each headCommand that returns headResult
		virtual quint16 idle   (CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) = 0;
		virtual quint16 accept (CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) = 0;
		virtual quint16 connect(CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) = 0;
		virtual quint16 destroy(CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) = 0;
		virtual quint16 read   (CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) = 0;
		virtual quint16 write  (CoProcessorIOFaceGuam::CoProcessorFCBType *fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType *iocb) = 0;
	};


	AgentStream() : Agent(GuamInputOutput::stream, "Stream") {
		fcb = 0;
	}

	CARD32 getFCBSize() {
		return SIZE(*fcb);
	}

	void Initialize();
	void Call();

	void addStream(Stream* stream);

private:
	CoProcessorIOFaceGuam::CoProcessorFCBType *fcb;
	QMap<quint32, Stream*> map;
};

#endif
