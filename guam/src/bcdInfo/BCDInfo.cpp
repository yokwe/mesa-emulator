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

#include "../mesa/Memory.h"

#include "../symbols/BCD.h"

#include "BCDInfo.h"


//
// JSONBase
//

// read value from jsonObject
void JSONBase::getJsonValue(const QJsonObject& jsonObject, const QString& key, QString& value) {
	if (jsonObject.contains(key)) {
		QJsonValue     j = jsonObject.value(key);
		QVariant       v = j.toVariant();
		QVariant::Type t = v.type();

		if (t == QVariant::Type::String) {
			value = v.toString();
		} else {
			logger.fatal("Unexpected key = %s type = %s", key.toLocal8Bit().constData(), QVariant::typeToName(t));
			logger.fatal("Expected type = String");
			ERROR();
		}
	} else {
		logger.fatal("Key doesn't exist. key = %s", key.toLocal8Bit().constData());
		ERROR();
	}

}
void JSONBase::getJsonValue(const QJsonObject& jsonObject, const QString& key, CARD16&  value) {
	if (jsonObject.contains(key)) {
		QJsonValue     j = jsonObject.value(key);
		QVariant       v = j.toVariant();
		QVariant::Type t = v.type();

		if (t == QVariant::Type::Int) {
			bool ok;
			value = (CARD16)v.toUInt(&ok);
			if (!ok) {
				logger.fatal("Unexpected not ok. key = %s  value = %s", key.toLocal8Bit().constData(), v.toString().toLocal8Bit().constData());
				ERROR();
			}
		} else {
			logger.fatal("Unexpected key = %s type = %s", key.toLocal8Bit().constData(), QVariant::typeToName(t));
			logger.fatal("Expected type = Int");
			ERROR();
		}
	} else {
		logger.fatal("Key doesn't exist. key = %s", key.toLocal8Bit().constData());
		ERROR();
	}

}
void JSONBase::getJsonValue(const QJsonObject& jsonObject, const QString& key, quint64& value) {
	if (jsonObject.contains(key)) {
		QJsonValue     j = jsonObject.value(key);
		QVariant       v = j.toVariant();
		QVariant::Type t = v.type();

		if (t == QVariant::Type::LongLong) {
			bool ok;
			value = (quint64)v.toLongLong(&ok);
			if (!ok) {
				logger.fatal("Unexpected not ok. key = %s", key.toLocal8Bit().constData());
				ERROR();
			}
		} else {
			logger.fatal("Unexpected key = %s type = %s", key.toLocal8Bit().constData(), QVariant::typeToName(t));
			logger.fatal("Expected type = LongLong");
			ERROR();
		}
	} else {
		logger.fatal("Key doesn't exist. key = %s", key.toLocal8Bit().constData());
		ERROR();
	}
}
void JSONBase::getJsonValue(const QJsonObject& jsonObject, const QString& key, bool&    value) {
	if (jsonObject.contains(key)) {
		QJsonValue     j = jsonObject.value(key);
		QVariant       v = j.toVariant();
		QVariant::Type t = v.type();

		if (t == QVariant::Type::Bool) {
			value = v.toBool();
		} else {
			logger.fatal("Unexpected key = %s type = %s", key.toLocal8Bit().constData(), QVariant::typeToName(t));
			logger.fatal("Expected type = Bool");
			ERROR();
		}
	} else {
		logger.fatal("Key doesn't exist. key = %s", key.toLocal8Bit().constData());
		ERROR();
	}
}


void JSONBase::setJsonValue(QJsonObject& jsonObject, const QString& key, const QString& value) {
	if (jsonObject.contains(key)) {
		logger.fatal("Duplicate key = %s", key.toLocal8Bit().constData());
	} else {
		jsonObject[key] = QJsonValue::fromVariant(QVariant(value));
	}
}
void JSONBase::setJsonValue(QJsonObject& jsonObject, const QString& key, const CARD16& value) {
	if (jsonObject.contains(key)) {
		logger.fatal("Duplicate key = %s", key.toLocal8Bit().constData());
	} else {
		jsonObject[key] = QJsonValue::fromVariant(QVariant(value));
	}
}
void JSONBase::setJsonValue(QJsonObject& jsonObject, const QString& key, const quint64& value) {
	if (jsonObject.contains(key)) {
		logger.fatal("Duplicate key = %s", key.toLocal8Bit().constData());
	} else {
		jsonObject[key] = QJsonValue::fromVariant(QVariant(value));
	}
}
void JSONBase::setJsonValue(QJsonObject& jsonObject, const QString& key, const bool& value) {
	if (jsonObject.contains(key)) {
		logger.fatal("Duplicate key = %s", key.toLocal8Bit().constData());
	} else {
		jsonObject[key] = QJsonValue::fromVariant(QVariant(value));
	}
}


//
// BCDInfo
//
BCDInfo::BCDInfo(BCD& bcd) {
	path        = bcd.getPath();
	version     = bcd.version->value;
	definitions = bcd.definitions;
}
BCDInfo::BCDInfo(QJsonObject& json) {
	getJsonValue(json);
}

// read value from jsonObject
void BCDInfo::getJsonValue(const QJsonObject& json) {
	GET_JSON_FIELD(json, path);
	GET_JSON_FIELD(json, version);
	GET_JSON_FIELD(json, definitions);
}
// write value to jsonObject
void BCDInfo::setJsonValue(QJsonObject& json) {
	SET_JSON_FIELD(json, path);
	SET_JSON_FIELD(json, version);
	SET_JSON_FIELD(json, definitions);
}
