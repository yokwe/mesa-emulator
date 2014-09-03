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
// MainWindow.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("mainwindow");

#include "../mesa/Memory.h"

#include "MainWindow.h"

#include "GuamObject.h"
#include "QtEventListener.h"
#include "QtGuiOp.h"


MainWindow::MainWindow() : QMainWindow(0, Qt::Window) {
	guamThread = new QThread;
	guamObject = new GuamObject;

	guamObject->init();
	guamObject->moveToThread(guamThread);

	QtGuiOp* qtGuiOp = new QtGuiOp;
	GuiOp::setContext(qtGuiOp);

	{
		QImage* image = guamObject->getDisplayImage();
		userTerminal = new UserTerminal(image);
		userTerminal->setParent(this);
		setCentralWidget(userTerminal);

		eventListener = new QtEventListener();
		userTerminal->setEventListener(eventListener);
	}


	{
		QStatusBar *status = new QStatusBar;

		mpPanel = new QLCDNumber(4);
		//label->setFrameStyle(QFrame::Box | QFrame::Plain);
		mpPanel->setMode(QLCDNumber::Dec);
		mpPanel->setSegmentStyle(QLCDNumber::Flat);
		mpPanel->display(0);

		status->addWidget(mpPanel);
		qtGuiOp->setMPPanel(mpPanel);
		GuiOp::setMP(8888);

		setStatusBar(status);
	}

	{
		bootAction = new QAction("Boot", this);

		connect(bootAction, SIGNAL(triggered()), guamObject, SLOT(startEmulator()));

		fileMenu = menuBar()->addMenu("&File");
		fileMenu->addAction(bootAction);
	}

	QObject::connect(qtGuiOp, SIGNAL(cursorPatternChanged(GuiOp::CursorPattern*)), userTerminal, SLOT(setCursorPattern(GuiOp::CursorPattern*)), Qt::QueuedConnection);
	QObject::connect(qtGuiOp, SIGNAL(displayChanged(GuiOp::Rect*)), userTerminal, SLOT(updateDisplay(GuiOp::Rect*)), Qt::QueuedConnection);

	connect(guamObject, SIGNAL(emulatorStopped()), this, SLOT(close()));
	guamThread->start();
}

void MainWindow::closeEvent (QCloseEvent* event) {
	if (this->guamObject->isEmulatorRunning()) {
		QMessageBox::StandardButtons buttons = QMessageBox::question(this, "Confirmation", "Do you want to stop emulator?", (QMessageBox::Cancel | QMessageBox::Ok), QMessageBox::Cancel);
		switch(buttons) {
		case QMessageBox::Cancel:
			event->ignore();
			break;
		case QMessageBox::Ok:
			this->guamObject->stopEmulator();
			break;
		default:
			ERROR();
		}
	}
}
