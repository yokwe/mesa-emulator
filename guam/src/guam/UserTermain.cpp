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
// UserTerminal.cpp
//


#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("userterm");

#include "UserTerminal.h"

class NullEventListener : public UserTerminal::EventListener  {
	void keyPress    (quint32 scancode) {
		logger.debug("Press   scan = %8X", scancode);
	}
	void keyRelease  (quint32 scancode) {
		logger.debug("Release scan = %8X", scancode);
	}
	void mousePress  (Qt::MouseButton button) {
		logger.debug("Press   mouse = %x", button);
	}
	void mouseRelease(Qt::MouseButton button) {
		logger.debug("Release mouse = %x", button);
	}
	//void mouseMove   (int x, int y) {
	void mouseMove   (int, int) {
		//logger.debug("Move    mouse  %4d %4d", x, y);
	}
};

void UserTerminal::setEventListener(UserTerminal::EventListener* eventListener_) {
	this->eventListener = eventListener_;
}

UserTerminal::UserTerminal(QImage* image_) : image(image_) {
	quint32 width  = image->width();
	quint32 height = image->height();
	logger.debug("UserTerminal  %4d %4d  -- %4d", width, height, image->bytesPerLine() * 8);

	// change size
	resize(width, height);
	setMinimumSize(width, height);
	setMaximumSize(width, height);

	// enable mouse position tracking
	setMouseTracking(1);
	setFocusPolicy(Qt::StrongFocus);

	// clear eventListener
	eventListener = new NullEventListener;
}

// keyboard press/release
void UserTerminal::keyPressEvent (QKeyEvent* event) {
	eventListener->keyPress(event->nativeScanCode());
}
void UserTerminal::keyReleaseEvent(QKeyEvent* event) {
	eventListener->keyRelease(event->nativeScanCode());
}

// mouse press/release/move
void UserTerminal::mousePressEvent(QMouseEvent* event) {
	eventListener->mousePress(event->button());
}
void UserTerminal::mouseReleaseEvent(QMouseEvent* event) {
	eventListener->mouseRelease(event->button());
}
void UserTerminal::mouseMoveEvent(QMouseEvent* event) {
	eventListener->mouseMove(event->pos().x(), event->pos().y());
}

//
void UserTerminal::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    // If call painter.drawImage(0, 0, *image), this call generate segmentation fault.
    // To avoid segmentation fault, need to convert format of image from Format_Mono to Format_RGB16.
    QImage rgb16 = image->convertToFormat(QImage::Format_RGB16, Qt::MonoOnly);
    rgb16.invertPixels();
    painter.drawImage(0, 0, rgb16);
}

void UserTerminal::setCursorPattern(GuiOp::CursorPattern* data) {
	QSize size(16, 16);
	QBitmap bitmap = QBitmap::fromData(size, (uchar*)(data->data), QImage::Format_Mono);
	QCursor cursor(bitmap, bitmap, 0, 0);

	setCursor(cursor);
}

void UserTerminal::updateDisplay(GuiOp::Rect*) {
	update();
}
