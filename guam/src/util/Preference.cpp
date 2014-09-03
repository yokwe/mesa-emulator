/*
Copyright (c) 2014, Yasuhiro Hasegawa
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
// Preference.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("preference");

#include "Preference.h"

#include <QStringList>

static const char* PREFERENCE_PATH  = "data/Guam/Preference.ini";

Preference::Preference() : settings(PREFERENCE_PATH, QSettings::IniFormat) {
	if (settings.status() != QSettings::NoError) {
		logger.fatal("ERROR path = %s!", PREFERENCE_PATH);
		ERROR();
	}
}

QString Preference::getAsString(QString group, QString key) {
	QString defaultValue = "__DEFAULT__";
	settings.beginGroup(group);
	QString ret = settings.value(key, defaultValue).toString();
	settings.endGroup();
	if (ret == defaultValue) {
		logger.fatal("ERROR group = %s!  key = %s!", qPrintable(group), qPrintable(key));
		ERROR();
	}
	//logger.debug("group = %s  key = %s  ret = %s!", qPrintable(group), qPrintable(key), qPrintable(ret));
	return ret;
}

quint32 Preference::getAsUINT32(QString group, QString key) {
	QString string = getAsString(group, key);
	bool ok;
	quint32 ret =  string.toUInt(&ok, 0);
	if (!ok) {
		logger.fatal("ERROR group = %s!  key = %s!  string = %s!", qPrintable(group), qPrintable(key), qPrintable(string));
		ERROR();
	}
	return ret;
}

QStringList Preference::getChildKeys(QString group) {
	settings.beginGroup(group);
	QStringList ret = settings.childKeys();
	settings.endGroup();
	return ret;
}
