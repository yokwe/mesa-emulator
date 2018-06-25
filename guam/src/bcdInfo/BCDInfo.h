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

#define SET_JSON_FIELD(JSON,FIELD) JSONBase::setJsonValue(JSON, #FIELD, FIELD)
#define GET_JSON_FIELD(JSON,FIELD) JSONBase::getJsonValue(JSON, #FIELD, FIELD)

class JSONBase {
public:
	virtual ~JSONBase() {}

	// read value from jsonObject
	virtual void getJsonValue(const QJsonObject& json) = 0;
	// write value to jsonObject
	virtual void setJsonValue(QJsonObject& json) = 0;

	// read value from jsonObject
	static void getJsonValue(const QJsonObject& json, const QString& key, QString& value);
	static void getJsonValue(const QJsonObject& json, const QString& key, CARD16&  value);
	static void getJsonValue(const QJsonObject& json, const QString& key, quint64& value);
	static void getJsonValue(const QJsonObject& json, const QString& key, bool&    value);

	// write value to jsonObject
	static void setJsonValue(QJsonObject& json, const QString& key, const QString& value);
	static void setJsonValue(QJsonObject& json, const QString& key, const CARD16&  value);
	static void setJsonValue(QJsonObject& json, const QString& key, const quint64& value);
	static void setJsonValue(QJsonObject& json, const QString& key, const bool&    value);
};

class FTInfo : public JSONBase {
public:
	QString name;
	quint64 version;

	FTInfo(FTRecord& ftRecoed);
	FTInfo(QJsonObject& jsonObject);
	virtual ~FTInfo() {}

	// read value from jsonObject
	void getJsonValue(const QJsonObject& json);
	// write value to jsonObject
	void setJsonValue(QJsonObject& json);
};

class BCDInfo : public JSONBase {
public:
	QString path;

	quint64 version;

	bool definitions;

//	FTInfo* sourceFile;
//	FTInfo* unpackagedFile;

	BCDInfo(BCD& bcd);
	BCDInfo(QJsonObject& jsonObject);
	virtual ~BCDInfo() {}

	// read value from jsonObject
	void getJsonValue(const QJsonObject& json);
	// write value to jsonObject
	void setJsonValue(QJsonObject& json);
};

#endif
