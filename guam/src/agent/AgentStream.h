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

#include <log4cpp/Category.hh>

#include "Agent.h"

class AgentStream : public Agent {
public:
	class Handler {
	public:
		static const CARD32 DEFAULT_SERVER_ID = 0;

		const CARD32  serverID;
		const QString name;

		Handler(CARD32 serverID_, QString name_) : serverID(serverID_), name(name_) {}
		virtual ~Handler() {}

		virtual CARD16 idle   (CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) = 0;
		virtual CARD16 accept (CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) = 0;
		virtual CARD16 connect(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) = 0;
		virtual CARD16 destroy(CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) = 0;
		virtual CARD16 read   (CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) = 0;
		virtual CARD16 write  (CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb) = 0;

		static void debugDump(log4cpp::Category& logger, const char* name, CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb);
	};

	static const char* getServerIDString(CARD32 serverID);
	static const char* getCommandString (CARD16 command);
	static const char* getStateString   (CARD16 state);
	static const char* getResultString  (CARD16 result);

	AgentStream() : Agent(GuamInputOutput::stream, "Stream") {
		fcb = 0;
	}

	CARD32 getFCBSize() {
		return SIZE(*fcb);
	}

	void Initialize();
	void Call();

private:
	CoProcessorIOFaceGuam::CoProcessorFCBType* fcb;

	QMap<CARD32, Handler*> handlerMap;

	void addHandler(Handler* handler);
	void processRequest(CoProcessorIOFaceGuam::CoProcessorFCBType* fcb, CoProcessorIOFaceGuam::CoProcessorIOCBType* iocb);
	void initialize();
};

#endif
