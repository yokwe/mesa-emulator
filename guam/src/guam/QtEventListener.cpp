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
// QtEventListener.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("qtevent");

#include "../util/Debug.h"
#include "../util/Preference.h"

#include "../agent/Agent.h"
#include "../agent/AgentKeyboard.h"
#include "../agent/AgentMouse.h"

#include "QtEventListener.h"

QHash<quint32,         quint32>QtEventListener::keyMap;
QHash<Qt::MouseButton, quint32>QtEventListener::buttonMap;

void QtEventListener::keyPress    (quint32 scancode) {
	quint32 keyName = 0;

	if (keyMap.contains(scancode)) {
		keyName = keyMap.value(scancode);
	}
	if (keyName) {
		if (DEBUG_SHOW_EVENT_KEY) logger.debug("%-12s  %4X %3d", __FUNCTION__, scancode, keyName);
		keyboard->keyPress((LevelVKeys::KeyName)keyName);
	} else {
		logger.warn("%-12s  %4X", __FUNCTION__, scancode);
	}
}
void QtEventListener::keyRelease  (quint32 scancode) {
	quint32 keyName = 0;

	if (keyMap.contains(scancode)) {
		keyName = keyMap.value(scancode);
	}
	if (keyName) {
		if (DEBUG_SHOW_EVENT_KEY) logger.debug("%-12s  %4X %3d", __FUNCTION__, scancode, keyName);
		keyboard->keyRelease((LevelVKeys::KeyName)keyName);
	} else {
		logger.warn("%-12s  %4X", __FUNCTION__, scancode);
	}
}
void QtEventListener::mousePress  (Qt::MouseButton button) {
	quint32 keyName = 0;

	if (buttonMap.contains(button)) {
		keyName = buttonMap.value(button);
	}

	if (keyName) {
		if (DEBUG_SHOW_EVENT_MOUSE) logger.debug("%-12s  %4X %3d", __FUNCTION__, (int)button, keyName);
		keyboard->keyPress((LevelVKeys::KeyName)keyName);
	} else {
		logger.warn("%-12s  %4X", __FUNCTION__, (int)button);
	}
}
void QtEventListener::mouseRelease(Qt::MouseButton button) {
	quint32 keyName = 0;

	if (buttonMap.contains(button)) {
		keyName = buttonMap.value(button);
	}

	if (keyName) {
		if (DEBUG_SHOW_EVENT_MOUSE) logger.debug("%-12s  %4X %3d", __FUNCTION__, (int)button, keyName);
		keyboard->keyRelease((LevelVKeys::KeyName)keyName);
	} else {
		logger.warn("%-12s  %4X", __FUNCTION__, (int)button);
	}
}
void QtEventListener::mouseMove   (int x, int y) {
	mouse->setPosition(x, y);
}

static const char* KEYSYMBOL_PATH = "data/Guam/KeySymbol.ini";
static void readSymbolGroup(QHash<QString, quint32>& symbolMap, QSettings& settings, QString group) {
	settings.beginGroup(group);
	const QStringList childKeys = settings.childKeys();

	foreach (const QString &key, childKeys) {
		QString valueString = settings.value(key).toString();
		bool ok;
		quint32 value = valueString.toInt(&ok, 0);
		if (!ok) {
			logger.fatal("valueString = %s", qPrintable(valueString));
			ERROR();
		}

		symbolMap[key] = value;
		//logger.debug("key = %s  %s  %d!", qPrintable(key), qPrintable(valueString), value);
	}

	settings.endGroup();
}
static void readSymbol(QHash<QString, quint32>& symbolMap) {
	QSettings settings(KEYSYMBOL_PATH, QSettings::IniFormat);

	// add dummy entry for KeyMap
	symbolMap["0"] = 0;

	// add LevelVKeys
	readSymbolGroup(symbolMap, settings, "LevelVKeys");
	// add Qt
	readSymbolGroup(symbolMap, settings, "Qt");
	// add Scan-Windows or Scan-Linux
#ifdef __linux__
	readSymbolGroup(symbolMap, settings, "Scan-Linux");
#elif __WIN32__
	readSymbolGroup(symbolMap, settings, "Scan-Windows");
#else
#error "Unpexpected"
#endif
}

QtEventListener::QtEventListener() {
	keyboard = (AgentKeyboard*)Agent::getAgent(GuamInputOutput::keyboard);
	if (keyboard == 0) {
		logger.fatal("keyboard == 0");
		ERROR();
	}
	mouse    = (AgentMouse*)Agent::getAgent(GuamInputOutput::mouse);
	if (mouse == 0) {
		logger.fatal("mouse == 0");
		ERROR();
	}

	QHash<QString, quint32>symbolMap;
	readSymbol(symbolMap);


	Preference* preference = new Preference();
	// [KeyMap]
	{
		QString group("KeyMap");
		QStringList childKeys = preference->getChildKeys(group);
		bool abort = false;
		foreach (const QString &key, childKeys) {
			// if first char of key is '#', treat as comment
			if (key[0] == QChar('#')) continue;

			QString value = preference->getAsString(group, key);
			if (!symbolMap.contains(key))   {
				logger.warn("Group = %s  key   = %s! is not defined", qPrintable(group), qPrintable(key));
				abort = 1;
				continue;
			}
			if (!symbolMap.contains(value)) {
				logger.fatal("Group = %s  value = %s! is not defined", qPrintable(group), qPrintable(value));
				abort = 1;
				continue;
			}

			quint32 lhs = symbolMap[key];
			quint32 rhs = symbolMap[value];

			if (rhs) keyMap.insert(rhs, lhs);
		}
		if (abort) ERROR();
	}

	// [ButtonMap]
	{
		QString group("ButtonMap");
		QStringList childKeys = preference->getChildKeys(group);
		bool abort = false;
		foreach (const QString &key, childKeys) {
			// if first char of key is '#', treat as comment
			if (key[0] == QChar('#')) continue;

			QString value = preference->getAsString(group, key);
			if (!symbolMap.contains(key))   {
				logger.warn("Group = %s  key   = %s! is not defined", qPrintable(group), qPrintable(key));
				abort = 1;
				continue;
			}
			if (!symbolMap.contains(value)) {
				logger.fatal("Group = %s  value = %s! is not defined", qPrintable(group), qPrintable(value));
				abort = 1;
				continue;
			}
			quint32 lhs = symbolMap[key];
			Qt::MouseButton rhs = (Qt::MouseButton)symbolMap[value];

			buttonMap.insert(rhs, lhs);
		}
		if (abort) ERROR();
	}
}
