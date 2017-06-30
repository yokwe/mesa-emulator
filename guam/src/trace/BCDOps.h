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
// BCDOps.h
//

#ifndef BCDOPS_H_
#define BCDOPS_H_ 1

#include "../mesa/Pilot.h"

#include "Trace.h"

#include <QtCore>

class VersionStamp {
public:
	CARD8     net;
	CARD8     host;
	QDateTime time;

	VersionStamp(const TimeStamp::Stamp& stamp) : net(stamp.net), host(stamp.host), time(QDateTime::fromTime_t(Util::toUnixTime(stamp.time))) {}
	VersionStamp(const VersionStamp& that) : net(that.net), host(that.host), time(that.time) {}
	VersionStamp() : net(0), host(0), time(QDateTime::fromTime_t(0)) {}

	QString toString();
};

class FTRecord {
public:
	QString      name;
	VersionStamp stamp;
	FTRecord(const QString& name_, const VersionStamp& stamp_) : name(name_), stamp(stamp_) {}
	FTRecord(const FTRecord& that) : name(that.name), stamp(that.stamp) {}
	FTRecord() : name(""), stamp(VersionStamp()) {}

	QString toString();
};

//Namee: TYPE = RECORD [
//  SELECT type: * FROM
//    config => [cti: CTIndex],
//    module => [mti: MTIndex],
//    import => [impi: IMPIndex],
//    export => [expi: EXPIndex]
//    ENDCASE];
class Namee {
public:
	enum class Type {
		config  = BcdDefs::N_config,
		module  = BcdDefs::N_module,
		import  = BcdDefs::N_import,
		exports = BcdDefs::N_export,
	};
	Type   type;
	CARD16 index;

	Namee(Type type_, CARD16 index_) : type(type_), index(index_) {}
	Namee(const Namee& that) : type(that.type), index(that.index) {}
	Namee() : type(Type::config), index(0) {}

	QString toString();
	QString toString(Type type);
};

//CTRecord: TYPE = --MACHINE DEPENDENT-- RECORD [
//  name: NameRecord,
//  file: FTIndex,
//  config: CTIndex,
//  namedInstance: BOOLEAN,
//  nControls: NATURAL,
//  controls: ARRAY [0..0) OF Namee];  -- only config or module are valid
class CTRecord {
public:
	QString        name;
	FTRecord       file;
	CARD16         config;
	bool           namedInstance;
	QVector<Namee> controls;

	QString toString();
};

//ENRecord: TYPE = RECORD [
//   nEntries: CARDINAL, initialPC: ARRAY [0..0) OF PrincOps.BytePC];
class ENRecord {
public:
	QVector<CARD16> initialPC;

	ENRecord(const QVector<CARD16>& initialPC_) : initialPC(initialPC_) {}
	ENRecord(const ENRecord& that) : initialPC(that.initialPC) {}
	ENRecord() : initialPC(QVector<CARD16>()) {}

	QString toString();
};

//SegClass: TYPE = {code, symbols, acMap, other};
//SGRecord: TYPE = RECORD [
//  file: FTIndex, base: CARDINAL,
//  pages: [0..256), extraPages: [0..64), class: SegClass];
class SGRecord {
public:
	enum class SegClass {
		code    = BcdDefs::SC_code,
		symbols = BcdDefs::SC_symbols,
		acMap   = BcdDefs::SC_acMap,
		other   = BcdDefs::SC_other,
	};

	FTRecord file;
	CARD16   base;
	CARD16   pages;
	CARD16   extraPages;
	SegClass segClass;

	QString toString();
	static QString toString(SegClass segClass);
};

//CodeDesc: TYPE = RECORD [
//  sgi: SGIndex, offset, length: CARDINAL];
class CodeDesc {
public:
	CARD16 sgi;
	CARD16 offset;
	CARD16 length;

	CodeDesc(CARD16 sgi_, CARD16 offset_, CARD16 length_) : sgi(sgi_), offset(offset_), length(length_) {}
	CodeDesc(const CodeDesc& that) : sgi(that.sgi), offset(that.offset), length(that.length) {}
	CodeDesc() : sgi(0), offset(0), length(0) {}
};

//Link: TYPE = MACHINE DEPENDENT RECORD [
//  rep(0): SELECT tag(0:0..1): LinkTag FROM
//    procedure => [gfi(0:2..15): GFIndex, ep(1): CARDINAL],
//    signal => [gfi(0:2..15): GFIndex, index(1): CARDINAL],
//    variable => [gfi(0:2..15): GFIndex, offset(1): CARDINAL],
//    type => [fill(0:2..15): [0..37777B], typeID(1): TYPIndex],
//    ENDCASE];
//NullLink, nullLink: Link = [procedure[0, 0]];
//UnboundLink, unboundLink: Link = [variable[0, 0]];
class Link {
public:
	enum class Tag {
		proc = BcdDefs::LT_prodecure,
		sig  = BcdDefs::LT_signal,
		var  = BcdDefs::LT_variable,
		type = BcdDefs::LT_type,
	};

	Tag    tag;
	CARD16 gfi;
	CARD16 value;

	QString toString(Tag tag);
	QString toString();
};

//LinkFrag: TYPE = RECORD [frag: SEQUENCE length: NAT OF Link];
class LinkFrag {
public:
	CARD16 length;
	QVector<Link> frag;

	QString toString();
};

//MTRecord: TYPE = --MACHINE DEPENDENT-- RECORD [
//  name: NameRecord,
//  file: FTIndex,
//  config: CTIndex,
//  code: CodeDesc,
//  sseg: SGIndex,
//  links: LFIndex,
//  linkLoc: LinkLocation,
//  namedInstance, initial: BOOLEAN,
//  boundsChecks, nilChecks: BOOLEAN,
//  tableCompiled, residentFrame, crossJumped, packageable: BOOLEAN,
//  packed: BOOLEAN, linkspace: BOOLEAN,
//  spare: PACKED ARRAY [0..4) OF BOOLEAN,
//  framesize: [0..PrincOps.MaxFrameSize),
//  entries: ENIndex,
//  atoms: ATIndex];
class MTRecord {
public:
	QString  name;
	FTRecord file;
	CARD16   config;  // CTIndex
	CodeDesc code;
	SGRecord sseg;    // SGIndex
	CARD16   links;   // LFIndex
	CARD16   linkLoc;
	bool     namedInstance;
	bool     initial;
	bool     boundsChecks;
	bool     nilChecks;
	bool     tableCompiled;
	bool     residentFrame;
	bool     crossJumped;
	bool     packageable;
	bool     packed;
	bool     linkspace;
	CARD16   framesize;
	ENRecord entries;
	CARD16   atoms;
};

//Portable: TYPE = {module, interface};
enum class Portable {
	module    = BcdDefs::P_module,
	interface = BcdDefs::P_interface,
};

//IMPRecord: TYPE = RECORD [
//  name: NameRecord,
//  port: Portable,
//  namedInstance: BOOLEAN,
//  file: FTIndex,
//  gfi: GFIndex];
class IMPRecord {
public:
	QString name;
	Portable port;
	bool     namedInstance;
	FTRecord file;
	CARD16   gfi;

	QString toString();
};

//EXPRecord: TYPE = --MACHINE DEPENDENT-- RECORD [
//  name: NameRecord,
//  size: [0..377b],
//  port: Portable,
//  namedInstance, typeExported: BOOLEAN,
//  file: FTIndex,
//  links: ARRAY [0..0) OF Link];
class EXPRecord {
public:
	QString       name;
	CARD16        size;
	Portable      port;
	bool          namedInstance;
	bool          typeExported;
	FTRecord      file;
	QVector<Link> links;

	QString toString();
};


class BCDOps {
public:
	BCDOps(CARD32 ptr);

	QString      name;
	VersionStamp version;
	VersionStamp creator;

	FTRecord sourceFile;
	CARD32   nPages;
	CARD32   firstDummy;
	bool     definitions;

	CARD32  nConfigs;
	CARD32  nModules;
	CARD32  nImports;
	CARD32  nExports;

	QMap<CARD16, QString>   ss;
	QMap<CARD16, FTRecord>  ft;
	QMap<CARD16, ENRecord>  en;
	QMap<CARD16, SGRecord>  sg;
	QMap<CARD16, CTRecord>  ct;
	QMap<CARD16, MTRecord>  mt;
	QMap<CARD16, LinkFrag>  lf;
	QMap<CARD16, IMPRecord> imp;
	QMap<CARD16, EXPRecord> exp;

	QString getName(CARD16 nameRecord);

private:
	BcdDefs::BCD header;
};

#endif


