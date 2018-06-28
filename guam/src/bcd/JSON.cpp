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
// JSON.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("JSON");

#include "JSON.h"


static QMap<QJsonValue::Type, QString> typeNameMap = {
	{QJsonValue::Null,      "NULL"},
	{QJsonValue::Bool,      "Bool"},
	{QJsonValue::Double,    "Double"},
	{QJsonValue::String,    "String"},
	{QJsonValue::Array,     "Array"},
	{QJsonValue::Object,    "Object"},
	{QJsonValue::Undefined, "Undefined"},
};

QString JSON::getJasonValueTypeName(QJsonValue::Type type) {
	if (typeNameMap.contains(type)) {
		return typeNameMap[type];
	} else {
		return QString("Unknwn-0x%1").arg((int)type, 0, 16);
	}
}

// read value from jsonObject
void JSON::getJsonValue(const QJsonObject& jsonObject, const QString& key, QString& value) {
	if (jsonObject.contains(key)) {
		QJsonValue v = jsonObject.value(key);

		if (v.isString()) {
			value = v.toString();
		} else {
			logger.fatal("Unexpected key = %s type = %s", key.toLocal8Bit().constData(), getJasonValueTypeName(v.type()).toLocal8Bit().constData());
			logger.fatal("Expected type = String");
			ERROR();
		}
	} else {
		logger.fatal("Key doesn't exist. key = %s", key.toLocal8Bit().constData());
		ERROR();
	}

}
void JSON::getJsonValue(const QJsonObject& jsonObject, const QString& key, quint16&  value) {
	if (jsonObject.contains(key)) {
		QJsonValue v = jsonObject.value(key);

		if (v.isDouble()) {
			bool ok;
			value = (quint16)v.toVariant().toInt(&ok);
			if (!ok) {
				logger.fatal("Unexpected not ok. key = %s  value = %s", key.toLocal8Bit().constData(), v.toString().toLocal8Bit().constData());
				ERROR();
			}
		} else {
			logger.fatal("Unexpected key = %s type = %s", key.toLocal8Bit().constData(), getJasonValueTypeName(v.type()).toLocal8Bit().constData());
			logger.fatal("Expected type = Double");
			ERROR();
		}
	} else {
		logger.fatal("Key doesn't exist. key = %s", key.toLocal8Bit().constData());
		ERROR();
	}

}
void JSON::getJsonValue(const QJsonObject& jsonObject, const QString& key, bool&    value) {
	if (jsonObject.contains(key)) {
		QJsonValue v = jsonObject.value(key);

		if (v.isBool()) {
			value = v.toBool();
		} else {
			logger.fatal("Unexpected key = %s type = %s", key.toLocal8Bit().constData(), getJasonValueTypeName(v.type()).toLocal8Bit().constData());
			logger.fatal("Expected type = Bool");
			ERROR();
		}
	} else {
		logger.fatal("Key doesn't exist. key = %s", key.toLocal8Bit().constData());
		ERROR();
	}
}
void JSON::getJsonValue(const QJsonObject& jsonObject, const QString& key, QJsonObject&  value) {
	if (jsonObject.contains(key)) {
		QJsonValue v = jsonObject.value(key);

		if (v.isObject()) {
			value = v.toObject();
		} else {
			logger.fatal("Unexpected key = %s type = %s", key.toLocal8Bit().constData(), getJasonValueTypeName(v.type()).toLocal8Bit().constData());
			logger.fatal("Expected type = Object");
			ERROR();
		}
	} else {
		logger.fatal("Key doesn't exist. key = %s", key.toLocal8Bit().constData());
		ERROR();
	}
}
void JSON::getJsonValue(const QJsonObject& jsonObject, const QString& key, QJsonArray&  value) {
	if (jsonObject.contains(key)) {
		QJsonValue v = jsonObject.value(key);

		if (v.isArray()) {
			value = v.toArray();
		} else {
			logger.fatal("Unexpected key = %s type = %s", key.toLocal8Bit().constData(), getJasonValueTypeName(v.type()).toLocal8Bit().constData());
			logger.fatal("Expected type = Array");
			ERROR();
		}
	} else {
		logger.fatal("Key doesn't exist. key = %s", key.toLocal8Bit().constData());
		ERROR();
	}
}



void JSON::setJsonValue(QJsonObject& jsonObject, const QString& key, const QString& value) {
	if (jsonObject.contains(key)) {
		logger.fatal("Duplicate key = %s", key.toLocal8Bit().constData());
	} else {
		jsonObject[key] = QJsonValue::fromVariant(QVariant(value));
	}
}
void JSON::setJsonValue(QJsonObject& jsonObject, const QString& key, const quint16& value) {
	if (jsonObject.contains(key)) {
		logger.fatal("Duplicate key = %s", key.toLocal8Bit().constData());
	} else {
		jsonObject[key] = QJsonValue::fromVariant(QVariant(value));
	}
}
void JSON::setJsonValue(QJsonObject& jsonObject, const QString& key, const bool& value) {
	if (jsonObject.contains(key)) {
		logger.fatal("Duplicate key = %s", key.toLocal8Bit().constData());
	} else {
		jsonObject[key] = QJsonValue::fromVariant(QVariant(value));
	}
}
void JSON::setJsonValue(QJsonObject& jsonObject, const QString& key, const QJsonObject& value) {
	if (jsonObject.contains(key)) {
		logger.fatal("Duplicate key = %s", key.toLocal8Bit().constData());
	} else {
		jsonObject[key] = value;
	}
}
void JSON::setJsonValue(QJsonObject& jsonObject, const QString& key, const QJsonArray& value) {
	if (jsonObject.contains(key)) {
		logger.fatal("Duplicate key = %s", key.toLocal8Bit().constData());
	} else {
		jsonObject[key] = value;
	}
}
