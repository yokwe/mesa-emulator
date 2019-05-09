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
// Programs.h
//

#ifndef COURIER_PROGRAMS_H__
#define COURIER_PROGRAMS_H__

#include "../courier/Block.h"
#include "../stub/IDP.h"

namespace Courier {
namespace Programs {

struct Info {
	quint32     programCode;
	const char* programName;
	quint16     versionCode;
	quint16     procedureCode;
	const char* procedureName;
};


class Procedure {
public:
	quint16 code;
	QString name;

	Procedure() : code(0xFFFFU), name("?") {}
	Procedure(int code_, QString name_) : code(code_), name(name_) {}

	Procedure(const Procedure& that) : code(that.code), name(that.name) {}
	Procedure& operator=(const Procedure& that) {
		code = that.code;
		name = that.name;
		return *this;
	}
};
class Program;
class Version {
public:
	quint16 code;

	Version() : code(0xFFFF) {}
	Version(quint16 code_) : code(code_) {}
	Version(const Version& that) : code(that.code) {}
	Version& operator=(const Version& that) {
		code = that.code;
		return *this;
	}

	void       addPocedure (Procedure& value);
	Procedure& getProcedure(int procedureCode);
	bool       contains    (int procedureCode);

private:
	QMap<quint16, Procedure> procedureMap;
};
class Program {
public:
	quint32 code;
	QString name;

	Program() : code(-1), name("?") {}
	Program(quint32 code_, QString name_) : code(code_), name(name_) {}
	Program(const Program& that) : code(that.code), name(that.name) {}
	Program& operator=(const Program& that) {
		code = that.code;
		name = that.name;
		return *this;
	}

	void     addVersion(quint16 versionCode);
	Version& getVersion(quint16 versionCode);
	bool     contains  (quint16 versionCode);

	static void       addEntry(quint32 programCode, QString programName, quint16 versionCode, quint16 procudureCode, QString procedureName);
	static Procedure& getProcedure(quint32 programCode, quint16 versionCode, quint16 procedureCode);

	static Program& getProgram(quint32 programCode);
	static void init();
private:
	static QMap<quint32, Program> programMap;

	QMap<quint16, Version> versionMap;
};

}
}

#endif
