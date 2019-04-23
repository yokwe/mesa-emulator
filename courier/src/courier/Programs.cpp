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
// Programs.cpp
//

#include "../util/Debug.h"
#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("cr/programs");

#include "../courier/Courier.h"
#include "../courier/Programs.h"

#include "../stub/Programs.h"

QMap<quint32, Courier::Programs::Program> Courier::Programs::Program::programMap;

// Version
void Courier::Programs::Version::addPocedure(Procedure& value) {
	int procedureCode = value.code;
	if (procedureMap.contains(procedureCode)) {
        logger.error("Unexpected code = %d", procedureCode);
        COURIER_FATAL_ERROR();
	} else {
		procedureMap[procedureCode] = value;
	}
}
Courier::Programs::Procedure& Courier::Programs::Version::getProcedure(int procedureCode) {
	if (procedureMap.contains(procedureCode)) {
        return procedureMap[procedureCode];
	} else {
        logger.error("Unexpected procedureCode = %d", procedureCode);
        COURIER_FATAL_ERROR();
	}
}
bool Courier::Programs::Version::contains(int procedureCode) {
	return procedureMap.contains(procedureCode);
}

// Program
void     Courier::Programs::Program::addVersion(quint16 versionCode) {
	if (versionMap.contains(versionCode)) {
        logger.error("Unexpected versionCode = %d", versionCode);
        COURIER_FATAL_ERROR();
	} else {
		versionMap[versionCode] = Version(versionCode);
	}
}
Courier::Programs::Version& Courier::Programs::Program::getVersion(quint16 versionCode) {
	if (versionMap.contains(versionCode)) {
        return versionMap[versionCode];
	} else {
        logger.error("Unexpected versionCode = %d", versionCode);
        COURIER_FATAL_ERROR();
	}
}
bool Courier::Programs::Program::contains(quint16 code) {
	return versionMap.contains(code);
}

void     Courier::Programs::Program::addEntry(quint32 programCode, QString programName, quint16 versionCode, quint16 procedureCode, QString procedureName) {
	if (!programMap.contains(programCode)) {
		Courier::Programs::Program program(programCode, programName);
		programMap[programCode] = program;
	}
	Courier::Programs::Program& program = programMap[programCode];
	if (!program.contains(versionCode)) {
		program.addVersion(versionCode);
	}
	Courier::Programs::Version& version = program.versionMap[versionCode];
	if (version.contains(procedureCode)) {
        logger.error("Unexpected procedureCode = %d", procedureCode);
        COURIER_FATAL_ERROR();
	} else {
		Procedure procedure(procedureCode, procedureName);
		version.addPocedure(procedure);
	}
}
Courier::Programs::Procedure& Courier::Programs::Program::getProcedure(quint32 programCode, quint16 versionCode, quint16 procedureCode) {
	Program& program = Courier::Programs::Program::getProgram(programCode);
	Version& version = program.getVersion(versionCode);
	return version.getProcedure(procedureCode);
}
Courier::Programs::Program& Courier::Programs::Program::getProgram(quint32 programCode) {
	if (programMap.contains(programCode)) {
        return programMap[programCode];
	} else {
        logger.error("Unexpected programCode = %d", programCode);
        COURIER_FATAL_ERROR();
	}
}

void Courier::Programs::Program::init() {
	static bool called = false;
	if (called) return;

	for(Info& info: Courier::Stub::Programs::infoList) {
		Program::addEntry(info.programCode, info.programName, info.versionCode, info.procedureCode, info.procedureName);
	}
	called = true;
}

class StaticInitializer {
public:
	StaticInitializer() {
		Courier::Programs::Program::init();
	}
};
StaticInitializer staticInitializer;
