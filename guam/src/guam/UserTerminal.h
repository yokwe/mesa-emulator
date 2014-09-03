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
// UserTerminal.h
//

#ifndef USERTERMINAL_H__
#define USERTERMINAL_H__

#include <QtGui>
#include <QWidget>

#include "../util/GuiOp.h"

// Override paintEvent to show graphics QImage
class UserTerminal : public QWidget {
	Q_OBJECT

public:
	class EventListener {
	public:
		virtual ~EventListener(){}

		// To distinguish left/right shift key, We need virtcode (X11 keysem)
		virtual void keyPress    (quint32 scancode) = 0;
		virtual void keyRelease  (quint32 scancode) = 0;
		virtual void mousePress  (Qt::MouseButton button) = 0;        // button is Qt::MouseButton
		virtual void mouseRelease(Qt::MouseButton button) = 0;        // button is Qt::MouseButton
		virtual void mouseMove   (int x, int y) = 0;
	};

	UserTerminal(QImage* image);

	void setEventListener(EventListener *eventListener);

public slots:
	void setCursorPattern(GuiOp::CursorPattern* data);
	void updateDisplay(GuiOp::Rect* rect);

protected:
	// key press/release
	void keyPressEvent  (QKeyEvent* event);
	void keyReleaseEvent(QKeyEvent* event);

	// mouse press/release/move
	void mousePressEvent  (QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void mouseMoveEvent   (QMouseEvent* event);

	// draw
	void paintEvent(QPaintEvent * event);

private:
	EventListener* eventListener;
	QImage*        image;
};

#endif
