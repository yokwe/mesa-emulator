/*
Copyright (c) 2019, Yasuhiro Hasegawa
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
// Service.h
//

#ifndef COURIER_SERVICE_H__
#define COURIER_SERVICE_H__

#include "../courier/Courier.h"
#include "../courier/Block.h"
#include "../courier/Protocol.h"

namespace Courier {
namespace Service {

class ServiceBase {
public:
    const char*   programName;
    const quint32 programCode;
    const quint16 versionCode;

    ServiceBase(const char* programName_, quint32 programCode_, quint16 versionCode_) :
    	programName(programName_), programCode(programCode_), versionCode(versionCode_) {}
    virtual ~ServiceBase();

    virtual bool        isProcedureValid    (quint16 code) const = 0;
    virtual bool        isProcedureInstalled(quint16 code) const = 0;
    virtual const char* getProcedureName    (quint16 code) const = 0;

    virtual void        init   ();
    virtual void        destroy();
    virtual void        service(Protocol::Protocol3::CallMessage& callMessage, Block& request, Block& response) const = 0;
};

class ServiceManager {
public:
	void addService(ServiceBase* service);

	void init   ();
	void destroy();
    void service(Protocol::Protocol2::CallMessage& callMessage, Block& request, Block& response) const;
    void service(Protocol::Protocol3::CallMessage& callMessage, Block& request, Block& response) const;

private:
    //   program       version
    QMap<quint32, QMap<quint16, ServiceBase*>> serviceMap;
};

}
}

#endif
