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
// ModuleInfo.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("moduleInfo");

#include "../mesa/Memory.h"

#include "../symbols/BCD.h"

#include <bcdInfo/ModuleInfo.h>

QString ModuleInfo::toString() const {
	QString entriesString;
	entriesString.append(QString("(%1)[").arg(entries.size()));
	for(CARD16 i = 0; i < entries.size(); i++) {
		if (0 < i) entriesString.append(", ");
		entriesString.append(QString("%1").arg(entries[i]));
	}
	entriesString.append(']');

	return QString("%1 %2 %3 %4 %5").arg(fileName).arg(fileVersion).arg(codeLength).arg(frameSize).arg(entriesString);
}

ModuleInfo* ModuleInfo::getInstance(BCD& bcd, MTRecord& mt) {
	if (mt.isNull()) {
		logger.fatal("mt is null");
		ERROR();
	}

	quint64 bcdVersion  = bcd.version->value;
	QString bcdPath     = bcd.getPath();

	QString name        = mt.name;

	quint64 fileVersion = mt.file->version->value;
	QString fileName    = mt.file->name;

	CARD16  codeOffset  = mt.code->offset;
	CARD16  codeLength  = mt.code->length;
	CARD16  frameSize   = mt.framesize;

	QVector<CARD16> entries = mt.entries->initialPC; // index is entryIndex and value is initialPC

	if (mt.file->isSelf()) {
		fileName    = mt.name;
		fileVersion = bcd.version->value;
	}

	return new ModuleInfo(bcdVersion, bcdPath, name, fileVersion, fileName, codeOffset, codeLength, frameSize, entries);
}

static void getValue(QJsonObject jsonObject, QString key, QString& value) {
	if (jsonObject.contains(key)) {
		if (jsonObject[key].isString()) {
			value = jsonObject[key].toString();
		} else {
			logger.fatal("value is not string. key = %s", key.toLocal8Bit().constData());
			ERROR();
		}
	} else {
		logger.fatal("key doesn't exist. key = %s", key.toLocal8Bit().constData());
		ERROR();
	}
}

static void getValue(QJsonObject jsonObject, QString key, quint64& value) {
	if (jsonObject.contains(key)) {
		if (jsonObject[key].isDouble()) {
			bool ok;
			value = jsonObject[key].toVariant().toULongLong(&ok);
			if (!ok) {
				logger.fatal("value is not q. key = %s", key.toLocal8Bit().constData());
			}
		} else {
			logger.fatal("value is not string. key = %s", key.toLocal8Bit().constData());
			ERROR();
		}
	} else {
		logger.fatal("key doesn't exist. key = %s", key.toLocal8Bit().constData());
		ERROR();
	}
}

static void getValue(QJsonObject jsonObject, QString key, CARD16& value) {
	if (jsonObject.contains(key)) {
		if (jsonObject[key].isDouble()) {
			bool ok;
			value = (CARD16)jsonObject[key].toVariant().toUInt(&ok);
			if (!ok) {
				logger.fatal("value is not UInt. key = %s", key.toLocal8Bit().constData());
				ERROR();
			}
		} else {
			logger.fatal("value is not double. key = %s", key.toLocal8Bit().constData());
			ERROR();
		}
	} else {
		logger.fatal("key doesn't exist. key = %s", key.toLocal8Bit().constData());
		ERROR();
	}
}

static void getValue(QJsonObject jsonObject, QString key, bool& value) {
	if (jsonObject.contains(key)) {
		if (jsonObject[key].isBool()) {
			value = jsonObject[key].toBool();
		} else {
			logger.fatal("value is not bool. key = %s", key.toLocal8Bit().constData());
			ERROR();
		}
	} else {
		logger.fatal("key doesn't exist. key = %s", key.toLocal8Bit().constData());
		ERROR();
	}
}

static void getValue(QJsonObject jsonObject, QString key, QVector<CARD16>& value) {
	if (jsonObject.contains(key)) {
		if (jsonObject[key].isArray()) {
			QJsonArray array = jsonObject[key].toArray();
			int size = array.size();
			for(int i = 0; i < size; i++) {
				QJsonValue element = array.at(i);
				if (element.isDouble()) {
					CARD16 elementValue = (CARD16)element.toDouble();
					value.append(elementValue);
				} else {
					logger.fatal("value is not expected type = %d. key = %s", element.type(), key.toLocal8Bit().constData());
					ERROR();
				}
			}
		} else {
			logger.fatal("value is not string. key = %s", key.toLocal8Bit().constData());
			ERROR();
		}
	} else {
		logger.fatal("key doesn't exist. key = %s", key.toLocal8Bit().constData());
		ERROR();
	}
}

ModuleInfo* ModuleInfo::readJson(QString& path) {
	QFile file(path);
	if (!file.exists()) {
		logger.fatal("File does not exist. path = %s", path.toLocal8Bit().constData());
		ERROR();
	}
	if (!file.open(QIODevice::OpenModeFlag::ReadOnly)) {
		logger.fatal("File open error %s", file.errorString().toLocal8Bit().constData());
		ERROR();
	}
	QByteArray fileContents = file.readAll();

	QJsonParseError jsonError;
	QJsonDocument jsonDoc = QJsonDocument::fromJson(fileContents, &jsonError);
	if (jsonDoc.isNull()) {
		logger.fatal("Parser error %s. path = %s", jsonError.errorString().toLocal8Bit().constData(), path.toLocal8Bit().constData());
		ERROR();
	}

	QJsonObject jsonObject = jsonDoc.object();
	{
		quint64 bcdVersion;
		QString bcdPath;
		QString name;
		quint64 fileVersion;
		QString fileName;
		CARD16  codeOffset;
		CARD16  codeLength;
		CARD16  frameSize;
		bool    packed;
		QVector<CARD16> entries;

		getValue(jsonObject, "bcdVersion",  bcdVersion);
		getValue(jsonObject, "bcdPath",     bcdPath);
		getValue(jsonObject, "name",        name);
		getValue(jsonObject, "fileVersion", fileVersion);
		getValue(jsonObject, "fileName",    fileName);
		getValue(jsonObject, "codeOffset",  codeOffset);
		getValue(jsonObject, "codeLength",  codeLength);
		getValue(jsonObject, "frameSize",   frameSize);
		getValue(jsonObject, "entries",     entries);

		return new ModuleInfo(bcdVersion, bcdPath, name, fileVersion, fileName, codeOffset, codeLength, frameSize, entries);
	}
}

void ModuleInfo::writeJson(QString& path) const {
	QFile file(path);
	if (!file.open(QIODevice::OpenModeFlag::WriteOnly)) {
		logger.fatal("File open error %s", file.errorString().toLocal8Bit().constData());
		ERROR();
	}


	QJsonObject jsonObject;
	{
		QJsonArray entriesValue;
		for(int i = 0; i < entries.size(); i++) {
			entriesValue.append(entries[i]);
		}
		jsonObject.insert("bcdVersion",  QJsonValue::fromVariant(QVariant(bcdVersion)));
		jsonObject.insert("bcdPath",     QJsonValue::fromVariant(QVariant(bcdPath)));
		jsonObject.insert("name",        QJsonValue::fromVariant(QVariant(name)));
		jsonObject.insert("fileVersion", QJsonValue::fromVariant(QVariant(fileVersion)));
		jsonObject.insert("fileName",    QJsonValue::fromVariant(QVariant(fileName)));
		jsonObject.insert("codeOffset",  QJsonValue::fromVariant(QVariant(codeOffset)));
		jsonObject.insert("codeLength",  QJsonValue::fromVariant(QVariant(codeLength)));
		jsonObject.insert("frameSize",   QJsonValue::fromVariant(QVariant(frameSize)));
		jsonObject.insert("entries",     entriesValue);
	}

	QJsonDocument jsonDoc(jsonObject);
	QByteArray fileContents = jsonDoc.toJson(QJsonDocument::JsonFormat::Indented); // Compact or Indented
	int fileLength = file.write(fileContents);
	if (fileLength != fileContents.size()) {
		logger.fatal("File write error %s  fileLength = %d  fileContents = %d", file.errorString().toLocal8Bit().constData(), fileLength, fileContents.size());
		ERROR();
	}
	file.close();
}
