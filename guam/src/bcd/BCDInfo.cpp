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
// BCDInfo.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("bcdInfo");

#include "BCDInfo.h"

#include <QCryptographicHash>


//
// FTInfo
//
QString FTInfo::VALUE_NULL("#NULL#");
QString FTInfo::VALUE_SELF("#SELF#");

FTInfo::FTInfo(const FTRecord& ft) {
	if (ft.isNull()) {
		this->name    = VALUE_NULL;
		this->version = VALUE_NULL;
	} else if (ft.isSelf()) {
		this->name    = VALUE_SELF;
		this->version = VALUE_SELF;
	} else {
		this->name    = ft.name;
		this->version = ft.version->value;
	}
}
void FTInfo::getJsonValue(const QJsonObject& json) {
	JSON::getJsonValue(json, "name",    name);
	JSON::getJsonValue(json, "version", version);
}
void FTInfo::setJsonValue(QJsonObject& json) const {
	JSON::setJsonValue(json, "name",    name);
	JSON::setJsonValue(json, "version", version);
}

void FTInfo::getJsonValue(const QJsonObject& json, const QString& key, FTInfo&  value) {
	QJsonObject jo;
	JSON::getJsonValue(json, key, jo);
	value.getJsonValue(jo);
}
void FTInfo::setJsonValue(QJsonObject& json, const QString& key, const FTInfo&  value) {
	QJsonObject jo;
	value.setJsonValue(jo);
	JSON::setJsonValue(json, key, jo);
}
void FTInfo::getJsonArray(const QJsonArray& ja, QList<FTInfo>& list) {
	for(const QJsonValue& e: ja) {
		if (e.isObject()) {
			QJsonObject o = e.toObject();
			FTInfo ft(o);
			list.append(ft);
		} else {
			logger.fatal("Unexpected type = %d", e.type());
			logger.fatal("Expected type = Object");
			ERROR();
		}
	}
}
void FTInfo::setJsonArray(QJsonArray& ja, const QList<FTInfo>& list) {
	for(const FTInfo& e: list) {
		QJsonObject jo;
		e.setJsonValue(jo);
		ja.append(jo);
	}
}


//
// SGInfo
//
SGInfo::SGInfo(const SGRecord& that) {
	this->file       = FTInfo(*that.file);
	this->base       = that.base;
	this->pages      = that.pages;
	this->extraPages = that.extraPages;
	this->segClass   = SGRecord::toString(that.segClass);
}
void SGInfo::getJsonValue(const QJsonObject& json) {
	FTInfo::getJsonValue(json, "file",       file);
	JSON::  getJsonValue(json, "base",       base);
	JSON::  getJsonValue(json, "pages",      pages);
	JSON::  getJsonValue(json, "extraPages", extraPages);
	JSON::  getJsonValue(json, "segClass",   segClass);
}
void SGInfo::setJsonValue(QJsonObject& json) const {
	FTInfo::setJsonValue(json, "file",       file);
	JSON::  setJsonValue(json, "base",       base);
	JSON::  setJsonValue(json, "pages",      pages);
	JSON::  setJsonValue(json, "extraPages", extraPages);
	JSON::  setJsonValue(json, "segClass",   segClass);
}

void SGInfo::getJsonValue(const QJsonObject& json, const QString& key, SGInfo&  value) {
	QJsonObject jo;
	JSON::getJsonValue(json, key, jo);
	value.getJsonValue(jo);
}
void SGInfo::setJsonValue(QJsonObject& json, const QString& key, const SGInfo&  value) {
	QJsonObject jo;
	value.setJsonValue(jo);
	JSON::setJsonValue(json, key, jo);
}
void SGInfo::getJsonArray(const QJsonArray& ja, QList<SGInfo>& list) {
	for(const QJsonValue& e: ja) {
		if (e.isObject()) {
			QJsonObject o = e.toObject();
			SGInfo ft(o);
			list.append(ft);
		} else {
			logger.fatal("Unexpected type = %d", e.type());
			logger.fatal("Expected type = Object");
			ERROR();
		}
	}
}
void SGInfo::setJsonArray(QJsonArray& ja, const QList<SGInfo>& list) {
	for(const SGInfo& e: list) {
		QJsonObject jo;
		e.setJsonValue(jo);
		ja.append(jo);
	}
}


//
// CodeInfo
//
CodeInfo::CodeInfo(const CodeDesc& that) {
	this->sg     = SGInfo(*that.sgi);
	this->offset = that.offset;
	this->length = that.length;
}
void CodeInfo::getJsonValue(const QJsonObject& json) {
	SGInfo::getJsonValue(json, "sg", sg);
	JSON::getJsonValue(json, "offset", offset);
	JSON::getJsonValue(json, "length", length);
}
void CodeInfo::setJsonValue(QJsonObject& json) const {
	SGInfo::setJsonValue(json, "sg", sg);
	JSON::setJsonValue(json, "offset", offset);
	JSON::setJsonValue(json, "length", length);
}
void CodeInfo::getJsonValue(const QJsonObject& json, const QString& key, CodeInfo&  value) {
	QJsonObject jo;
	JSON::getJsonValue(json, key, jo);
	value.getJsonValue(jo);
}
void CodeInfo::setJsonValue(QJsonObject& json, const QString& key, const CodeInfo&  value) {
	QJsonObject jo;
	value.setJsonValue(jo);
	JSON::setJsonValue(json, key, jo);
}


//
// ENInfo
//
ENInfo::ENInfo(const ENRecord& that) {
	this->initialPC = that.initialPC;
}
void ENInfo::getJsonValue(const QJsonObject& json) {
	initialPC.clear();

	QJsonArray ja;
	JSON::getJsonValue(json, "initialPC", ja);
	for(QJsonValue v: ja) {
		if (v.isDouble()) {
			bool ok;
			CARD16 value = (CARD16)v.toVariant().toInt(&ok);
			if (!ok) {
				logger.fatal("Unexpected not ok. value = %s", v.toString().toLocal8Bit().constData());
				ERROR();
			}
			initialPC.append(value);
		} else {
			logger.fatal("Unexpected type = %s", JSON::getJasonValueTypeName(v.type()).toLocal8Bit().constData());
			logger.fatal("Expected type = Double");
			ERROR();
		}
	}
}
void ENInfo::setJsonValue(QJsonObject& json) const {
	QJsonArray ja;
	for(CARD16 value: initialPC) {
		QJsonValue jv(value);
		ja.append(value);
	}
	JSON::setJsonValue(json, "initialPC", ja);
}
void ENInfo::getJsonValue(const QJsonObject& json, const QString& key, ENInfo&  value) {
	QJsonObject jo;
	JSON::getJsonValue(json, key, jo);
	value.getJsonValue(jo);
}
void ENInfo::setJsonValue(QJsonObject& json, const QString& key, const ENInfo&  value) {
	QJsonObject jo;
	value.setJsonValue(jo);
	JSON::setJsonValue(json, key, jo);
}


//
// MTInfo
//
MTInfo::MTInfo(const MTRecord& that) {
	this->name      = that.name;
	this->file      = FTInfo(*that.file);
	this->code      = CodeInfo(*that.code);
	this->sseg      = SGInfo(*that.sseg);
	this->framesize = that.framesize;
	this->entries   = ENInfo(*that.entries);
}
void MTInfo::getJsonValue(const QJsonObject& json) {
	JSON::    getJsonValue(json, "name",     name);
	FTInfo::  getJsonValue(json, "file",     file);
	CodeInfo::getJsonValue(json, "code",     code);
	SGInfo::  getJsonValue(json, "sseg",      sseg);
	JSON::    getJsonValue(json, "framesize", framesize);
	ENInfo::  getJsonValue(json, "entries",   entries);
}
void MTInfo::setJsonValue(QJsonObject& json) const {
	JSON::    setJsonValue(json, "name",     name);
	FTInfo::  setJsonValue(json, "file",     file);
	CodeInfo::setJsonValue(json, "code",     code);
	SGInfo::  setJsonValue(json, "sseg",      sseg);
	JSON::    setJsonValue(json, "framesize", framesize);
	ENInfo::  setJsonValue(json, "entries",   entries);
}

void MTInfo::getJsonValue(const QJsonObject& json, const QString& key, MTInfo&  value) {
	QJsonObject jo;
	JSON::getJsonValue(json, key, jo);
	value.getJsonValue(jo);
}
void MTInfo::setJsonValue(QJsonObject& json, const QString& key, const MTInfo&  value) {
	QJsonObject jo;
	value.setJsonValue(jo);
	JSON::setJsonValue(json, key, jo);
}

void MTInfo::getJsonArray(const QJsonArray& ja, QList<MTInfo>& list) {
	for(const QJsonValue& e: ja) {
		if (e.isObject()) {
			QJsonObject o = e.toObject();
			MTInfo mt(o);
			list.append(mt);
		} else {
			logger.fatal("Unexpected type = %d", e.type());
			logger.fatal("Expected type = Object");
			ERROR();
		}
	}
}
void MTInfo::setJsonArray(QJsonArray& ja, const QList<MTInfo>& list) {
	for(const MTInfo& e: list) {
		QJsonObject jo;
		e.setJsonValue(jo);
		ja.append(jo);
	}
}


//
// BCDInfo
//

static QCryptographicHash md5(QCryptographicHash::Algorithm::Md5);

QString getHash(QString path) {
	QFile file(path);
	bool result = file.open(QIODevice::OpenModeFlag::ReadOnly);
	if (!result) {
		logger.fatal("File open error -- %s", file.errorString().toLocal8Bit().constData());
		logger.fatal("path %s", path.toLocal8Bit().constData());
		ERROR();
	}
	QByteArray data = file.readAll();
	file.close();

	md5.reset();
	md5.addData(data);
	return md5.result().toHex();
}
BCDInfo::BCDInfo(BCD& bcd) {
	path        = bcd.getPath();
	hash        = getHash(path);

	version     = bcd.version->toString();

	sourceFile     = FTInfo(*bcd.sourceFile);
	unpackagedFile = FTInfo(*bcd.unpackagedFile);

	nConfigs = bcd.nConfigs;
	nModules = bcd.nModules;
	nImports = bcd.nImports;
	nExports = bcd.nExports;
	nPages   = bcd.nPages;

	definitions   = bcd.definitions;
	repackaged    = bcd.repackaged;
	typeExported  = bcd.typeExported;
	tableCompiled = bcd.tableCompiled;

	for(MTRecord* e: bcd.mt.values()) {
		// Skip null entry
		if (e->isNull()) continue;

		MTInfo mt(*e);
		mtList.append(mt);
	}
}
BCDInfo::BCDInfo(QJsonObject& json) {
	getJsonValue(json);
}

// read value from jsonObject
void BCDInfo::getJsonValue(const QJsonObject& json) {
	JSON::  getJsonValue(json, "path",           path);
	JSON::  getJsonValue(json, "hash",           hash);

	JSON::  getJsonValue(json, "version",        version);

	FTInfo::getJsonValue(json, "sourceFile",     sourceFile);
	FTInfo::getJsonValue(json, "unpackagedFile", unpackagedFile);

	JSON::  getJsonValue(json, "nConfigs",       nConfigs);
	JSON::  getJsonValue(json, "nModules",       nModules);
	JSON::  getJsonValue(json, "nImports",       nImports);
	JSON::  getJsonValue(json, "nExports",       nExports);
	JSON::  getJsonValue(json, "nPages",         nPages);

	JSON::  getJsonValue(json, "definitions",    definitions);
	JSON::  getJsonValue(json, "repackaged",     repackaged);
	JSON::  getJsonValue(json, "typeExported",   typeExported);
	JSON::  getJsonValue(json, "tableCompiled",  tableCompiled);

	QJsonArray ja;
	JSON::getJsonValue(json, "mt", ja);
	MTInfo::getJsonArray(ja, mtList);
}
// write value to jsonObject
void BCDInfo::setJsonValue(QJsonObject& json) const {
	JSON::  setJsonValue(json, "path",           path);
	JSON::  setJsonValue(json, "hash",           hash);

	JSON::  setJsonValue(json, "version",        version);

	FTInfo::setJsonValue(json, "sourceFile",     sourceFile);
	FTInfo::setJsonValue(json, "unpackagedFile", unpackagedFile);

	JSON::  setJsonValue(json, "nConfigs",       nConfigs);
	JSON::  setJsonValue(json, "nModules",       nModules);
	JSON::  setJsonValue(json, "nImports",       nImports);
	JSON::  setJsonValue(json, "nExports",       nExports);
	JSON::  setJsonValue(json, "nPages",         nPages);

	JSON::  setJsonValue(json, "definitions",    definitions);
	JSON::  setJsonValue(json, "repackaged",     repackaged);
	JSON::  setJsonValue(json, "typeExported",   typeExported);
	JSON::  setJsonValue(json, "tableCompiled",  tableCompiled);

	QJsonArray ja;
	MTInfo::setJsonArray(ja, mtList);
	JSON::setJsonValue(json, "mt", ja);
}
