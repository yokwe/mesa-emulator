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
// GuamObject.h
//

#ifndef GUAMOBJECT_H__
#define GUAMOBJECT_H__

#include <QtCore>
#include <QtGui>

#include "../util/Preference.h"

#include "../mesa/MesaProcessor.h"

class GuamObject : public QObject {
	Q_OBJECT

public:
	GuamObject();

	void init();

	int isEmulatorRunning() {
		return mesaProcessor.isRunning();
	}
	void stopEmulator() {
		if (mesaProcessor.isRunning()) {
			// Interrupt must be enabled to stop processor thread
			if (!InterruptThread::isEnabled()) InterruptThread::setWDC(0);
			mesaProcessor.stop();
			mesaProcessor.wait();
		}
	}

	QImage* getDisplayImage();

signals:
	void cursorPatternChanged(quint16 pattern[16]);
	void emulatorStopped();

public slots:
	void startEmulator();

private:
	Preference* preference;
	QString     section;

	quint32     displayWidth;
	quint32     displayHeight;

	MesaProcessor mesaProcessor;
};

#endif
