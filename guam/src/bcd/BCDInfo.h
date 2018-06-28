/*
Copyright (c) 2018, Yasuhiro Hasegawa
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
// BCDInfo.h
//

#ifndef BCDINFO_H__
#define BCDINFO_H__

#include "../util/Util.h"
#include "../mesa/MesaBasic.h"

#include "../symbols/BCD.h"

#include "JSON.h"


class FTInfo : public JSON {
public:
	static QString VALUE_NULL;
	static QString VALUE_SELF;

	static void getJsonValue(const QJsonObject& json, const QString& key,       FTInfo& value);
	static void setJsonValue(      QJsonObject& json, const QString& key, const FTInfo& value);

	static void setJsonArray(      QJsonArray& ja, const QList<FTInfo>& list);
	static void getJsonArray(const QJsonArray& ja,       QList<FTInfo>& list);


	QString name;
	QString version;

	// Assignable data type must provide default constructor, a copy constructor, and an assignment operator.
	FTInfo() {
		this->name    = "#NULL#";
		this->version = "#NULL#";
	}
	FTInfo(const FTInfo& that) {
		this->name    = that.name;
		this->version = that.version;
	}
	FTInfo &operator=(const FTInfo& that) {
		this->name    = that.name;
		this->version = that.version;
		return *this;
	}

	virtual ~FTInfo() {}

	FTInfo(const FTRecord& that);
	FTInfo(const QJsonObject& jsonObject) {
		getJsonValue(jsonObject);
	}

	// read value from jsonObject
	void getJsonValue(const QJsonObject& json);
	// write value to jsonObject
	void setJsonValue(QJsonObject& json) const;

	bool isNull() const {
		return version == VALUE_NULL;
	}
	bool isSelf() const {
		return version == VALUE_SELF;
	}
};


class SGInfo : public JSON {
public:
	static void getJsonValue(const QJsonObject& json, const QString& key,       SGInfo& value);
	static void setJsonValue(      QJsonObject& json, const QString& key, const SGInfo& value);

	static void setJsonArray(      QJsonArray& ja, const QList<SGInfo>& list);
	static void getJsonArray(const QJsonArray& ja,       QList<SGInfo>& list);

	FTInfo  file;
	CARD16  base;
	CARD16  pages;
	CARD16  extraPages;
	QString segClass;   // CODE, SYMBOLS, AC_MAP or OTHER

	// Assignable data type must provide default constructor, a copy constructor, and an assignment operator.
	SGInfo() {
		this->file       = FTInfo();
		this->base       = 0;
		this->pages      = 0;
		this->extraPages = 0;
		this->segClass   = "OTHER";
	}
	SGInfo(const SGInfo& that) {
		this->file       = that.file;
		this->base       = that.base;
		this->pages      = that.pages;
		this->extraPages = that.extraPages;
		this->segClass   = that.segClass;
	}
	SGInfo &operator=(const SGInfo& that) {
		this->file       = that.file;
		this->base       = that.base;
		this->pages      = that.pages;
		this->extraPages = that.extraPages;
		this->segClass   = that.segClass;
		return *this;
	}

	virtual ~SGInfo() {}

	SGInfo(const SGRecord& that);
	SGInfo(const QJsonObject& jsonObject) {
		getJsonValue(jsonObject);
	}

	// read value from jsonObject
	void getJsonValue(const QJsonObject& json);
	// write value to jsonObject
	void setJsonValue(QJsonObject& json) const;
};

class CodeInfo : public JSON {
public:
	static void getJsonValue(const QJsonObject& json, const QString& key,       CodeInfo& value);
	static void setJsonValue(      QJsonObject& json, const QString& key, const CodeInfo& value);

	SGInfo sg;
	CARD16 offset;
	CARD16 length;

	// Assignable data type must provide default constructor, a copy constructor, and an assignment operator.
	CodeInfo() {
		this->sg     = SGInfo();
		this->offset = 0;
		this->length = 0;
	}
	CodeInfo(const CodeInfo& that) {
		this->sg     = that.sg;
		this->offset = that.offset;
		this->length = that.length;
	}
	CodeInfo &operator=(const CodeInfo& that) {
		this->sg     = that.sg;
		this->offset = that.offset;
		this->length = that.length;
		return *this;
	}

	virtual ~CodeInfo() {}

	CodeInfo(const CodeDesc& that);
	CodeInfo(const QJsonObject& jsonObject) {
		getJsonValue(jsonObject);
	}

	// read value from jsonObject
	void getJsonValue(const QJsonObject& json);
	// write value to jsonObject
	void setJsonValue(QJsonObject& json) const;
};

class ENInfo : public JSON {
public:
	static void getJsonValue(const QJsonObject& json, const QString& key,       ENInfo& value);
	static void setJsonValue(      QJsonObject& json, const QString& key, const ENInfo& value);

	QVector<CARD16> initialPC;

	// Assignable data type must provide default constructor, a copy constructor, and an assignment operator.
	ENInfo() {
		this->initialPC.clear();
	}
	ENInfo(const ENInfo& that) {
		this->initialPC = that.initialPC;
	}
	ENInfo &operator=(const ENInfo& that) {
		this->initialPC = that.initialPC;
		return *this;
	}

	virtual ~ENInfo() {}

	ENInfo(const ENRecord& that);
	ENInfo(const QJsonObject& jsonObject) {
		getJsonValue(jsonObject);
	}

	// read value from jsonObject
	void getJsonValue(const QJsonObject& json);
	// write value to jsonObject
	void setJsonValue(QJsonObject& json) const;
};

class MTInfo : JSON {
public:
	static void getJsonValue(const QJsonObject& json, const QString& key,       MTInfo& value);
	static void setJsonValue(      QJsonObject& json, const QString& key, const MTInfo& value);

	static void setJsonArray(      QJsonArray& ja, const QList<MTInfo>& list);
	static void getJsonArray(const QJsonArray& ja,       QList<MTInfo>& list);

//	CARD16    index;
	QString   name;
	FTInfo    file;
//	CARD16    config;
	CodeInfo  code;
	SGInfo    sseg;
//	CARD16    links;
//	CARD16    linkLoc;
//	bool      namedInstance;
//	bool      initial;
//	bool      boundsChecks;
//	bool      nilChecks;
//	bool      tableCompiled;
//	bool      residentFrame;
//	bool      crossJumped;
//	bool      packageable;
//	bool      packed;
//	bool      linkspace;
//	bool      spare0;
//	bool      spare1;
//	bool      spare2;
//	bool      spare3;
	CARD16    framesize;
	ENInfo    entries;
//	CARD16    atoms;

	// Assignable data type must provide default constructor, a copy constructor, and an assignment operator.
	MTInfo() {
		this->name      = "#UNDEF#";
		this->file      = FTInfo();
		this->code      = CodeInfo();
		this->sseg      = SGInfo();
		this->framesize = 0;
		this->entries   = ENInfo();
	}
	MTInfo(const MTInfo& that) {
		this->name      = that.name;
		this->file      = that.file;
		this->code      = that.code;
		this->sseg      = that.sseg;
		this->framesize = that.framesize;
		this->entries   = that.entries;
	}
	MTInfo &operator=(const MTInfo& that) {
		this->name      = that.name;
		this->file      = that.file;
		this->code      = that.code;
		this->sseg      = that.sseg;
		this->framesize = that.framesize;
		this->entries   = that.entries;
		return *this;
	}

	virtual ~MTInfo() {}

	MTInfo(const MTRecord& that);
	MTInfo(const QJsonObject& jsonObject) {
		getJsonValue(jsonObject);
	}

	// read value from jsonObject
	void getJsonValue(const QJsonObject& json);
	// write value to jsonObject
	void setJsonValue(QJsonObject& json) const;

};


class BCDInfo : public JSON {
public:
	QString path;
	QString hash;

	QString version;

	FTInfo sourceFile;
	FTInfo unpackagedFile;

	CARD16 nConfigs;
	CARD16 nModules;
	CARD16 nImports;
	CARD16 nExports;
	CARD16 nPages;

	bool   definitions;
	bool   repackaged;
	bool   typeExported;
	bool   tableCompiled;

	QList<MTInfo> mtList; // Treat as QJsonArray

	// Assignable data type must provide default constructor, a copy constructor, and an assignment operator.
	BCDInfo() {
		path           = "#UNDEF#";
		hash           = "#UNDEF#";

		version        = "#UNDEF#";

		sourceFile     = FTInfo();
		unpackagedFile = FTInfo();

		nConfigs       = 0;
		nModules       = 0;
		nImports       = 0;
		nExports       = 0;
		nPages         = 0;

		definitions    = false;
		repackaged     = false;
		typeExported   = false;
		tableCompiled  = false;

		mtList.clear();
	}
	BCDInfo(const BCDInfo& that) {
		this->path           = that.path;
		this->hash           = that.hash;

		this->version        = that.version;

		this->sourceFile     = that.sourceFile;
		this->unpackagedFile = that.unpackagedFile;

		this->nConfigs       = that.nConfigs;
		this->nModules       = that.nModules;
		this->nImports       = that.nImports;
		this->nExports       = that.nExports;
		this->nPages         = that.nPages;

		this->definitions    = that.definitions;
		this->repackaged     = that.repackaged;
		this->typeExported   = that.typeExported;
		this->tableCompiled  = that.tableCompiled;

		this->mtList         = that.mtList;
	}
	BCDInfo &operator=(const BCDInfo& that) {
		this->path           = that.path;
		this->hash           = that.hash;

		this->version        = that.version;

		this->sourceFile     = that.sourceFile;
		this->unpackagedFile = that.unpackagedFile;

		this->nConfigs       = that.nConfigs;
		this->nModules       = that.nModules;
		this->nImports       = that.nImports;
		this->nExports       = that.nExports;
		this->nPages         = that.nPages;

		this->definitions    = that.definitions;
		this->repackaged     = that.repackaged;
		this->typeExported   = that.typeExported;
		this->tableCompiled  = that.tableCompiled;

		this->mtList         = that.mtList;

		return *this;
	}

	virtual ~BCDInfo() {}

	BCDInfo(BCD& bcd);
	BCDInfo(QJsonObject& jsonObject);

	// read value from jsonObject
	void getJsonValue(const QJsonObject& json);
	// write value to jsonObject
	void setJsonValue(QJsonObject& json) const;
};

#endif
