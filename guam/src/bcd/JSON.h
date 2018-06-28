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
// JSON.h
//

#ifndef JSON_H__
#define JSON_H__

class JSON {
public:
	virtual ~JSON() {}

	// read value from jsonObject
	virtual void getJsonValue(const QJsonObject& json) = 0;
	// write value to jsonObject
	virtual void setJsonValue(QJsonObject& json) const = 0;

	// read value from jsonObject
	static void getJsonValue(const QJsonObject& json, const QString& key, QString&     value);
	static void getJsonValue(const QJsonObject& json, const QString& key, quint16&     value);
	static void getJsonValue(const QJsonObject& json, const QString& key, bool&        value);
	static void getJsonValue(const QJsonObject& json, const QString& key, QJsonObject& value);
	static void getJsonValue(const QJsonObject& json, const QString& key, QJsonArray&  value);

	// write value to jsonObject
	static void setJsonValue(QJsonObject& json, const QString& key, const QString&     value);
	static void setJsonValue(QJsonObject& json, const QString& key, const quint16&     value);
	static void setJsonValue(QJsonObject& json, const QString& key, const bool&        value);
	static void setJsonValue(QJsonObject& json, const QString& key, const QJsonObject& value);
	static void setJsonValue(QJsonObject& json, const QString& key, const QJsonArray&  value);

	static QString getJasonValueTypeName(QJsonValue::Type type);
};

#endif
